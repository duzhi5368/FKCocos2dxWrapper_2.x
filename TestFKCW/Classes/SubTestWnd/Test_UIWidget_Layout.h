//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_UIWidget_Layout.h
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
class CTest_UIWidget_LayoutLayer : public CCLayer
{
public:
	bool init()
	{
		m_nIndex = 0;
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		pLayout = FKCW_UIWidget_Layout::create();
		pLayout->setPosition(SCREEN_CENTER_POS);
		pLayout->setContentSize(CCSize(SCREEN_CENTER_POS_X, 320));
		pLayout->setBackgroundImage(s_szBackground1Path);
		m_pWindow->addChild(pLayout);

		FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(
			CCSize(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
		pButton->setPosition(CCPoint(240, 100));
		pLayout->addChild(pButton);

		FKCW_UIWidget_ImageView* pImage = FKCW_UIWidget_ImageView::create(s_szIcon);
		pImage->setPosition(CCPoint(240, 200));
		pLayout->addChild(pImage);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我可改变容器布局").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_UIWidget_LayoutLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		switch ( m_nIndex % 3 )
		{
		case 0:
			pLayout->setBackgroundGradient(ccc4(255,0,0,255), ccc4(0, 255, 0, 128), CCPoint(1.0f, 0.0f));
			break;
		case 1:
			pLayout->setBackgroundColor(ccc4(255, 0, 0, 255));
			break;
		case 2:
			pLayout->setBackgroundImage(s_szBackground1Path);
			break;
		default:
			break;
		}
		m_nIndex++;
	}
public:
	CREATE_FUNC( CTest_UIWidget_LayoutLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Layout* pLayout;
	int m_nIndex;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, Layout, 空容器布局)