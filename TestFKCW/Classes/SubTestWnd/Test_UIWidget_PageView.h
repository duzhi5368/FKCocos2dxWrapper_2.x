//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_PageView.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	页面滑动的容器控件
//  测试结果:		
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_PageViewLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		FKCW_UIWidget_PageView* pPageView = FKCW_UIWidget_PageView::create(
			CCSize(484, 320),
			10, this,
			fkcw_uiwidget_datasource_adapter_selector(CTest_UIWidget_PageViewLayer::pageviewDataSource));
		pPageView->setOnPageChangedListener(this, fkcw_uiwidget_pagechanged_selector(CTest_UIWidget_PageViewLayer::onPageChanged));
		pPageView->setPosition(SCREEN_CENTER_POS);
		pPageView->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(pPageView);
		pPageView->reloadData();

		m_pText = FKCW_UIWidget_Label::create();
		m_pText->setAnchorPoint(CCPoint(0, 0.5));
		m_pText->setPosition(CCPoint(800, 320));
		m_pText->setFontSize(35.0f);
		m_pText->setString("0");
		m_pWindow->addChild(m_pText);

		return true;
	}
public:
	CCObject* pageviewDataSource(CCObject* pConvertCell, unsigned int nPageIdx)
	{
		FKCW_UIWidget_PageViewCell* pPageCell = (FKCW_UIWidget_PageViewCell*) pConvertCell;
		FKCW_UIWidget_Button* pButton = NULL;

		if( !pPageCell )
		{
			pPageCell = new FKCW_UIWidget_PageViewCell();
			pPageCell->autorelease();
			pPageCell->setBackgroundImage( s_szBackground1Path );

			pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path );
			pButton->getLabel()->setFontSize(30.0f);
			pButton->setPosition(CCPoint(484 / 2, 320 / 2));
			pButton->setTag(1);
			pPageCell->addChild(pButton);
		}
		else
		{
			pButton = (FKCW_UIWidget_Button*) pPageCell->getChildByTag(1);
		}

		char buf[48] = {0};
		sprintf(buf, "%u", nPageIdx);
		pButton->getLabel()->setString(buf);

		return pPageCell;
	}

	void onPageChanged(CCObject* pSender, unsigned int nPageIndx)
	{
		char buf[48] = {0};
		sprintf(buf, "%u", nPageIndx);
		m_pText->setString(buf);
	}
public:
	CREATE_FUNC( CTest_UIWidget_PageViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label* m_pText;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, PageView, 页面滑动的容器控件)