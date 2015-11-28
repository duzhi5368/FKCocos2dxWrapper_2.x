//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_UIWidget_DrawZOrder.h
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
class CTest_UIWidget_DrawZOrderLayer : public CCLayer
{
public:
	bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_Button* pSwapBtn = FKCW_UIWidget_Button::create();
		pSwapBtn->setPosition(SCREEN_CENTER_POS + ccp(0,-200));
		pSwapBtn->setScale9Enabled(true);
		pSwapBtn->setNormalImage(s_szSprite9Btn1Path);
		pSwapBtn->setSelectedImage(s_szSprite9Btn2Path);
		pSwapBtn->initText(FKCW_Base_StringConvert::a2u("交换Z值").c_str(), "", 30);
		pSwapBtn->setCascadeTextSizeEnabled(true, CCSize(50, 30));
		pSwapBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_DrawZOrderLayer::onClick));
		m_pWindow->addChild(pSwapBtn);

		pButton1 = FKCW_UIWidget_Button::create(s_szButtonNormalPath, s_szButtonPressPath);
		pButton1->setZOrder(1);
		pButton1->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pButton1);

		pButton2 = FKCW_UIWidget_Button::create(s_szButtonNormalPath, s_szButtonPressPath);
		pButton2->setZOrder(2);
		pButton2->setPosition(SCREEN_CENTER_POS+ccp(40, 30));
		m_pWindow->addChild(pButton2);

		return true;
	}
public:
	void onClick(CCObject* pSender)
	{
		int z = pButton2->getZOrder();
		pButton2->setZOrder(pButton1->getZOrder());
		pButton1->setZOrder(z);
	}
public:
	CREATE_FUNC( CTest_UIWidget_DrawZOrderLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Button* pButton1;
	FKCW_UIWidget_Button* pButton2;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, DrawZOrder, UIWidget绘制顺序|测试绘制顺序调整)