//FKCW_UIWidget_ListView
//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_ToggleView.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	链表滑动容器
//  测试结果:		
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
struct SItem
{
	CCSize tSize;
	ccColor3B tColor;
};
class CTest_UIWidget_ListViewLayer : public CCLayer
{
public:
	bool init()
	{
		last_add_idx = 0;
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		m_pListView = FKCW_UIWidget_ListView::create(CCSize(SCREEN_CENTER_POS_X, 300));
		m_pListView->setBackgroundImage(s_szBackground1Path);
		m_pListView->setPosition(SCREEN_CENTER_POS+ccp(0, 100));
		m_pListView->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(m_pListView);

		FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton->setPosition(CCPoint(150, 510));
		pButton->setUserTag(1);
		pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ListViewLayer::onClick));
		pButton->initText(FKCW_Base_StringConvert::a2u("添加到尾部").c_str(), "", 30);
		m_pWindow->addChild(pButton);

		FKCW_UIWidget_Button* pButton2 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton2->setPosition(CCPoint(150, 440));
		pButton2->setUserTag(2);
		pButton2->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ListViewLayer::onClick));
		pButton2->initText(FKCW_Base_StringConvert::a2u("添加到头部").c_str(), "", 30);
		m_pWindow->addChild(pButton2);

		FKCW_UIWidget_Button* pButton3 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton3->setPosition(CCPoint(150, 370));
		pButton3->setUserTag(3);
		pButton3->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ListViewLayer::onClick));
		pButton3->initText(FKCW_Base_StringConvert::a2u("从尾部移除").c_str(), "", 30);
		m_pWindow->addChild(pButton3);

		FKCW_UIWidget_Button* pButton4 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton4->setPosition(CCPoint(150, 300));
		pButton4->setUserTag(4);
		pButton4->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ListViewLayer::onClick));
		pButton4->initText(FKCW_Base_StringConvert::a2u("从头部移除").c_str(), "", 30);
		m_pWindow->addChild(pButton4);

		//----------------------------------------------
		// test2
		//----------------------------------------------
		SItem tag1 = { CCSize(SCREEN_CENTER_POS_X, 10), ccWHITE };
		SItem tag2 = { CCSize(SCREEN_CENTER_POS_X, 20), ccRED };
		SItem tag3 = { CCSize(SCREEN_CENTER_POS_X, 30), ccGREEN };
		SItem tag4 = { CCSize(100, 40), ccORANGE };
		SItem tag5 = { CCSize(SCREEN_CENTER_POS_X, 50), ccMAGENTA };
		SItem tag6 = { CCSize(400, 60), ccBLUE };
		SItem tag7 = { CCSize(300, 80), ccRED };
		SItem tag8 = { CCSize(SCREEN_CENTER_POS_X, 30), ccGRAY };
		SItem tag9 = { CCSize(SCREEN_CENTER_POS_X, 40), ccYELLOW };
		m_lDatas.push_back(tag1);
		m_lDatas.push_back(tag2);
		m_lDatas.push_back(tag3);
		m_lDatas.push_back(tag4);
		m_lDatas.push_back(tag5);
		m_lDatas.push_back(tag6);
		m_lDatas.push_back(tag7);
		m_lDatas.push_back(tag8);
		m_lDatas.push_back(tag9);

		m_pListView2 = FKCW_UIWidget_ListView::create(CCSize(SCREEN_CENTER_POS_X, 220));
		m_pListView2->setBackgroundImage(s_szBackground1Path);
		m_pListView2->setPosition(SCREEN_CENTER_POS+ccp(0,-200));
		m_pListView2->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(m_pListView2);

		FKCW_UIWidget_Button* pButton5 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton5->setPosition(CCPoint(150, 120));
		pButton5->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ListViewLayer::onClick2));
		pButton5->initText(FKCW_Base_StringConvert::a2u("添加随机色块").c_str(), "", 30);
		m_pWindow->addChild(pButton5);

		return true;
	}
public:
	void onClick2(CCObject* pSender)
	{
		if( !m_lDatas.empty() )
		{
			SItem& tItem = m_lDatas.front();
			CCNode* pNode = CCNode::create();
			pNode->setContentSize(tItem.tSize);

			FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
			pLayout->setBackgroundColor(ccc4(tItem.tColor.r,
				tItem.tColor.g,
				tItem.tColor.b,
				255));

			pLayout->ignoreAnchorPointForPosition(false);
			pLayout->setAnchorPoint(CCPoint(0.5f, 0.5f));
			pLayout->setContentSize(CCSize(tItem.tSize.width - 4, tItem.tSize.height - 2));
			pLayout->setPosition(CCPoint(tItem.tSize.width/2, tItem.tSize.height/2));
			pNode->addChild(pLayout);
			m_pListView2->insertNodeAtLast(pNode);
			m_pListView2->reloadData();
			m_lDatas.pop_front();
		}
	}
	void onClick(CCObject* pSender)
	{
		FKCW_UIWidget_Button* pButton = (FKCW_UIWidget_Button*) pSender;
		switch(pButton->getUserTag())
		{
		case 1:
			{
				FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
				pLayout->setContentSize(CCSize(SCREEN_CENTER_POS_X, 30));

				FKCW_UIWidget_ColorView* pColor = FKCW_UIWidget_ColorView::create(ccc4(128, 0, 0, 255));
				pColor->setContentSize(CCSize(478, 28));
				pColor->setPosition(CCPoint(SCREEN_CENTER_POS_X/2, 30/2));
				pLayout->addChild(pColor);

				FKCW_UIWidget_Button* pButton  =  FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 25),
					s_szSprite9Btn1Path, s_szSprite9Btn2Path );
				pButton->setPosition(CCPoint(SCREEN_CENTER_POS_X /2, 30 /2));
				char str[64] = {0};
				sprintf(str, "%d", last_add_idx);
				pButton->initText(str, "", 20);
				pLayout->addChild(pButton);
				last_add_idx++;

				m_pListView->insertNodeAtLast(pLayout);
				m_pListView->reloadData();
			}
			break;
		case 2:
			{
				FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
				pLayout->setContentSize(CCSize(SCREEN_CENTER_POS_X, 30));

				FKCW_UIWidget_ColorView* pColor = FKCW_UIWidget_ColorView::create(ccc4(0, 128, 0, 255));
				pColor->setContentSize(CCSize(478, 28));
				pColor->setPosition(CCPoint(SCREEN_CENTER_POS_X/2, 30/2));
				pLayout->addChild(pColor);

				FKCW_UIWidget_Button* pButton =  FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 25),
					s_szSprite9Btn1Path, s_szSprite9Btn2Path );
				pButton->setPosition(CCPoint(SCREEN_CENTER_POS_X /2, 30 /2));
				char str[64] = {0};
				sprintf(str, "%d", last_add_idx);
				pButton->initText(str, "", 20);
				pLayout->addChild(pButton);
				last_add_idx++;

				m_pListView->insertNodeAtFront(pLayout);
				m_pListView->reloadData();
			}
			break;
		case 3:
			{
				m_pListView->removeLastNode();
				m_pListView->reloadData();
			}
			break;
		case 4:
			{
				m_pListView->removeFrontNode();
				m_pListView->reloadData();
			}
			break;
		default:
			break;
		}
	}
public:

public:
	CREATE_FUNC( CTest_UIWidget_ListViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_ListView* m_pListView;
	FKCW_UIWidget_ListView* m_pListView2;
	list<SItem> m_lDatas;
	int last_add_idx;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ListView, 链表滑动容器)