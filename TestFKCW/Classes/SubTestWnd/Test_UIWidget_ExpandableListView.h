//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_ExpandableListView.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	ExpandableListView可扩展列表
//  测试结果:
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_ExpandableListViewLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		m_pListView = FKCW_UIWidget_ExpandableListView::create(CCSize(320, 390));
		m_pListView->setPosition(SCREEN_CENTER_POS);
		m_pListView->setBackgroundImage(s_szBackground2Path);
		m_pWindow->addChild(m_pListView);

		for( int i = 0; i < 10; ++i )
		{
			FKCW_UIWidget_ExpandableNode* pExpandableNode = FKCW_UIWidget_ExpandableNode::create();
			pExpandableNode->setContentSize(CCSize(320, 50));
			m_pListView->insertExpandableNodeAtLast(pExpandableNode);

			FKCW_UIWidget_ImageViewScale9* pExpandableNodeBg = FKCW_UIWidget_ImageViewScale9::create(CCSize(320, 45), s_szExpandNodeBgPath);
			pExpandableNodeBg->setPosition(CCPoint(320/2, 50/2));
			pExpandableNode->addChild(pExpandableNodeBg);

			FKCW_UIWidget_Button* pColBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(35, 35), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
			pColBtn->setPosition(ccp(320 - 30, 50 / 2));
			pColBtn->setText("-");
			pColBtn->setUserTag(i);
			pColBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ExpandableListViewLayer::onClick));
			pExpandableNode->addChild(pColBtn);

			char buff[64] = {0};
			sprintf(buff, "扩展列表节点 : %d", i);
			FKCW_UIWidget_Label* pTitleLabel = FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u(buff).c_str(), "", 28);
			pTitleLabel->setPosition(CCPoint(10, 50/2));
			pTitleLabel->setAnchorPoint(CCPoint(0, 0.5));
			pExpandableNode->addChild(pTitleLabel);


			sprintf(buff, "文本 %d - 1", i);
			pExpandableNode->insertItemNodeAtLast(FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u(buff).c_str(), "", 30));
			sprintf(buff, "文本 %d - 2", i);
			pExpandableNode->insertItemNodeAtLast(FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u(buff).c_str(), "", 30));
			sprintf(buff, "文本 %d - 3", i);
			pExpandableNode->insertItemNodeAtLast(FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u(buff).c_str(), "", 30));

			FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 38),
				s_szSprite9Btn1Path, s_szSprite9Btn2Path );
			pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ExpandableListViewLayer::onClick));
			pButton->initText(FKCW_Base_StringConvert::a2u("折叠").c_str(), "", 28);
			pButton->setUserTag(i);
			pButton->setPosition(CCPoint(100, 40/2));
			FKCW_UIWidget_Layout* pBtnLayout = FKCW_UIWidget_Layout::create(CCSize(320, 40));
			pBtnLayout->addChild(pButton);
			pExpandableNode->insertItemNodeAtLast(pBtnLayout);
		}
		m_pListView->reloadData();
		return true;
	}
public:
	void onClick(CCObject* pSender)
	{
		FKCW_UIWidget_Button* pButton = (FKCW_UIWidget_Button*) pSender;

		m_pListView->collapse(pButton->getUserTag());
		m_pListView->reloadData();
	}
public:
	CREATE_FUNC( CTest_UIWidget_ExpandableListViewLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_ExpandableListView* m_pListView;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ExpandableListView, 可扩展列表)