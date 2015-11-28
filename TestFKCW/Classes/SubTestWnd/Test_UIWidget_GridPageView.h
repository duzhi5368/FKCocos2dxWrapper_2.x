//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_UIWidget_GridPageView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_GridPageViewLayer : public CCLayer
{
public:
	bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		for(int i = 0 ; i < 72; i++)
		{
			char buff[64];
			sprintf(buff, "%d", i);
			m_vData.push_back(buff);
		}
		//背景
		CCSprite* pBg = CCSprite::create(s_szBackground2Path);
		pBg->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pBg);
		//容器
		//参数1 容器大小
		//参数2 容器内小容器大小
		//参数3 元素大小
		//参数4 数据源	*跟IOS的列表控件使用方式差不多
		pTable = FKCW_UIWidget_GridPageView::create(
			CCSizeMake(320, 390),
			CCSizeMake(320 / 4, 390 / 5),
			72, this, 
			fkcw_uiwidget_datasource_adapter_selector(CTest_UIWidget_GridPageViewLayer::GridPageViewDataSource));
		pTable->setColumns(4);
		pTable->setRows(5);
		pTable->setAutoRelocate(true);	//重新排列
		pTable->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pTable);
		pTable->reloadData();	//装载资源

		//创建元素Sprite
		m_pSelectedSprite = CCSprite::create(s_szIcon);
		m_pSelectedSprite->setOpacity(170);
		m_pSelectedSprite->setZOrder(1);
		m_pSelectedSprite->setVisible(false);
		m_pWindow->addChild(m_pSelectedSprite);

		m_pWindow->setOnTouchMovedAfterLongClickListener(this, fkcw_uiwidget_afterlongclick_selector(CTest_UIWidget_GridPageViewLayer::onLayoutTouchMovedAfterLongClick));
		m_pWindow->setOnTouchEndedAfterLongClickListener(this, fkcw_uiwidget_afterlongclick_selector(CTest_UIWidget_GridPageViewLayer::onLayoutTouchEndedAfterLongClick));
		m_pWindow->setOnTouchCancelledAfterLongClickListener(this, fkcw_uiwidget_afterlongclick_selector(CTest_UIWidget_GridPageViewLayer::onLayoutTouchEndedAfterLongClick));

		//创建销毁区域
		m_pToggleImage = FKCW_UIWidget_ToggleView::create(s_szToggle1Path, s_szToggle2Path);
		m_pToggleImage->setPosition(CCPoint(200, 320));
		m_pToggleImage->initText(FKCW_Base_StringConvert::a2u("ICON拖到此处销毁").c_str(), "Altas", 26 );
		m_pWindow->addChild(m_pToggleImage);

		return true;
	}
public:
	CCObject* GridPageViewDataSource(CCObject* pConvertCell, unsigned int idx)
	{
		FKCW_UIWidget_GridPageViewCell* pCell = (FKCW_UIWidget_GridPageViewCell*) pConvertCell;
		FKCW_UIWidget_Button* pIconButton = NULL;

		if(!pCell)
		{
			pCell = new FKCW_UIWidget_GridPageViewCell();
			pCell->autorelease();

			FKCW_UIWidget_Scale9Sprite* pBg = FKCW_UIWidget_Scale9Sprite::create( s_szSprite9Btn1Path );
			pBg->setContentSize(CCSize(70, 70));
			pBg->setPosition(CCPoint(320 / 4 / 2, 390 / 5 / 2));
			pCell->addChild(pBg);

			pIconButton = FKCW_UIWidget_Button::create( s_szIcon );
			pIconButton->setOnLongClickListener(this, fkcw_uiwidget_longclick_selector(CTest_UIWidget_GridPageViewLayer::onItemLongClick));
			pIconButton->getLabel()->setFontSize(25);
			pIconButton->setPosition(CCPoint(320 / 4 / 2, 390 / 5 / 2));
			pIconButton->setLabelOffset(CCPoint(-15, -15));
			pIconButton->setTag(1);
			if( idx % 2 == 0 )
			{
				pIconButton->setColor( ccRED );
			}
			pCell->addChild(pIconButton);
		}
		else
		{
			pIconButton = (FKCW_UIWidget_Button*) pCell->getChildByTag(1);
		}

		if( idx != CC_INVALID_INDEX )
		{
			pIconButton->setVisible(true);

			pIconButton->getLabel()->setString(m_vData[idx].c_str());
			pIconButton->setUserTag(idx);
		}
		else
		{
			pIconButton->setVisible(false);
		}
		return pCell;
	}

	bool onItemLongClick(CCObject* pSender, CCTouch* pTouch)
	{
		FKCW_UIWidget_Button* pIconButton = (FKCW_UIWidget_Button*) pSender;
		pIconButton->setVisible(false);
		CCPoint tPoint = pIconButton->getParent()->convertToWorldSpace(pIconButton->getPosition());

		m_pSelectedSprite->setVisible(true);
		m_pSelectedSprite->setPosition(pTouch->getLocation());

		return true;
	}
	void onLayoutTouchMovedAfterLongClick(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		m_pSelectedSprite->setPosition(pTouch->getLocation());

		CCPoint tLayoutPoint = m_pWindow->convertTouchToNodeSpace(pTouch);
		if( m_pToggleImage->boundingBox().containsPoint(tLayoutPoint) )
		{
			m_pToggleImage->setChecked(true);
		}
		else
		{
			m_pToggleImage->setChecked(false);
		}
	}
	void onLayoutTouchEndedAfterLongClick(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		FKCW_UIWidget_Button* pIconButton = (FKCW_UIWidget_Button*) pSender;

		m_pSelectedSprite->setPosition(pTouch->getLocation());

		pIconButton->setVisible(true);
		m_pSelectedSprite->setVisible(false);
		m_pToggleImage->setChecked(false);

		CCPoint tLayoutPoint = m_pWindow->convertTouchToNodeSpace(pTouch);
		if( m_pToggleImage->boundingBox().containsPoint(tLayoutPoint) )
		{
			m_vData.erase(m_vData.begin() + pIconButton->getUserTag());
			pTable->setCountOfCell(m_vData.size());
			pTable->reloadData();
			return;
		}
	}
public:
	CREATE_FUNC( CTest_UIWidget_GridPageViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_GridPageView* pTable;
	CCSprite* m_pSelectedSprite;
	FKCW_UIWidget_ToggleView* m_pToggleImage;
	vector<string> m_vData;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, GridPageView, 网格容器控件|横向（可左右拉动），物件ICON可长按拖拽)