//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_ScrollView.h
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
class CTest_UIWidget_ScrollViewLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);


		CCSprite* pBg = CCSprite::create( s_szBackground1Path );
		pBg->setPosition(CCPoint(680, 320));
		m_pWindow->addChild(pBg);

		pScroll = FKCW_UIWidget_ScrollView::create(SCREEN_CENTER_POS);
		pScroll->setPosition(CCPoint(680, 320));
		pScroll->setDirection(eScrollViewDirectionHorizontal);
		m_pWindow->addChild(pScroll);

		for( int j = 0; j < 8; ++j )
		{
			for(int i = 0; i < 8; i++)
			{
				FKCW_UIWidget_ToggleView* pToggleButton = FKCW_UIWidget_ToggleView::create(s_szToggle1Path, s_szToggle2Path);
				pToggleButton->setAnchorPoint(CCPointZero);
				pToggleButton->setPosition(CCPoint(static_cast<float>(130 * i), static_cast<float>(130 * j)));
				pToggleButton->setExclusion(1);
				//注意：应该通过如下方法想滚动View上添加内容
				pScroll->getContainer()->addChild(pToggleButton);
			}
		}

		FKCW_UIWidget_ToggleView* pToggle2 = FKCW_UIWidget_ToggleView::createWith9Sprite(CCSize(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pToggle2->initText(FKCW_Base_StringConvert::a2u("是否横向").c_str(), "", 28);
		pToggle2->setPosition(150, 400);
		pToggle2->setChecked(true);
		pToggle2->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ScrollViewLayer::onClick));
		pToggle2->setUserTag(2);
		m_pWindow->addChild(pToggle2);

		FKCW_UIWidget_ToggleView* pToggle3 = FKCW_UIWidget_ToggleView::createWith9Sprite(CCSize(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pToggle3->initText(FKCW_Base_StringConvert::a2u("是否可滑动").c_str(), "", 28);
		pToggle3->setPosition(150, 320);
		pToggle3->setChecked(true);
		pToggle3->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ScrollViewLayer::onClick));
		pToggle3->setUserTag(3);
		m_pWindow->addChild(pToggle3);


		FKCW_UIWidget_ToggleView* pToggle4 = FKCW_UIWidget_ToggleView::createWith9Sprite(CCSize(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pToggle4->initText(FKCW_Base_StringConvert::a2u("是否响应触碰").c_str(), "", 28);
		pToggle4->setPosition(150, 240);
		pToggle4->setChecked(true);
		pToggle4->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ScrollViewLayer::onClick));
		pToggle4->setUserTag(4);
		m_pWindow->addChild(pToggle4);

		return true;
	}
public:
	void onClick(CCObject* pSender)
	{
		FKCW_UIWidget_ToggleView* pToggle = (FKCW_UIWidget_ToggleView*) pSender;
		if( pToggle->getUserTag() == 2 )
		{
			if( pToggle->isChecked() )
			{
				pScroll->setDirection( eScrollViewDirectionHorizontal );
			}
			else
			{
				pScroll->setDirection( eScrollViewDirectionVertical );
			}
		}
		else if( pToggle->getUserTag() == 3 )
		{
			pScroll->setDragable(pToggle->isChecked());
		}
		else if( pToggle->getUserTag() == 4 )
		{
			pScroll->setTouchEnabled(pToggle->isChecked());
		}
	}
public:
	CREATE_FUNC( CTest_UIWidget_ScrollViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_ScrollView* pScroll;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ScrollView, 滑动容器)