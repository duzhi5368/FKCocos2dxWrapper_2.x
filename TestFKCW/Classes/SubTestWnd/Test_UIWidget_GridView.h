//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_UIWidget_GridView.h
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
class CTest_UIWidget_GridViewLayer : public CCLayer
{
public:
	bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		CCSprite* pBg = CCSprite::create(s_szBackground1Path);
		pBg->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pBg);

		FKCW_UIWidget_GridView* pGridView = FKCW_UIWidget_GridView::create(
			CCSize(SCREEN_CENTER_POS_X, 320),
			CCSize(SCREEN_CENTER_POS_X / 5, 320 / 4),
			96, this,
			fkcw_uiwidget_datasource_adapter_selector(CTest_UIWidget_GridViewLayer::GridViewDataSource));
		pGridView->setColumns(5);
		pGridView->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pGridView);
		pGridView->setAutoRelocate(true);
		pGridView->reloadData();

		return true;
	}
public:
	CCObject* GridViewDataSource(CCObject* pConvertView, unsigned int idx)
	{
		FKCW_UIWidget_GridViewCell* pCell = (FKCW_UIWidget_GridViewCell*) pConvertView;
		FKCW_UIWidget_Button* pButton = NULL;

		if(!pCell)
		{
			pCell = new FKCW_UIWidget_GridViewCell();
			pCell->autorelease();

			pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(70, 70), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
			pButton->setPosition(CCPoint(SCREEN_CENTER_POS_X / 5 / 2, 320 / 4 / 2));
			pButton->getLabel()->setFontSize(25.0f);
			pButton->setTag(1);
			pCell->addChild(pButton);
		}
		else
		{
			pButton = (FKCW_UIWidget_Button*) pCell->getChildByTag(1);
		}

		char buff[64];
		sprintf(buff, "%u", idx);
		pButton->getLabel()->setString(buff);
		pButton->setUserTag(idx);

		return pCell;
	}
public:
	CREATE_FUNC( CTest_UIWidget_GridViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, GridView, 格子容器控件|纵向（可上下拉动）)