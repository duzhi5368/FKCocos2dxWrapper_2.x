//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_UIWidget_Button.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	UIWidget按钮
//  测试结果:	
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_ButtonLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		m_pButton = FKCW_UIWidget_Button::create(s_szButtonNormalPath, s_szButtonPressPath, s_szButtonDisablePath);
		m_pButton->initText(FKCW_Base_StringConvert::a2u("我是按钮").c_str(), "", 28);
		m_pButton->setPosition(SCREEN_CENTER_POS);
		//绑定点击事件
		m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ButtonLayer::onClickListener));
		//绑定长按事件
		m_pButton->setOnLongClickListener(this, fkcw_uiwidget_longclick_selector(CTest_UIWidget_ButtonLayer::onLongClickListener));
		m_pWindow->addChild(m_pButton);

		m_pButton->setOnTouchBeganListener(this, fkcw_uiwidget_touchbegan_selector(CTest_UIWidget_ButtonLayer::onTouchBegan));
		m_pButton->setOnTouchMovedListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_ButtonLayer::onTouchMoved));
		m_pButton->setOnTouchEndedListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_ButtonLayer::onTouchEnded));
		m_pButton->setOnTouchCancelledListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_ButtonLayer::onTouchCancelled));

		m_pInfo = FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u("消息反馈信息条").c_str(), "Altas", 26 );
		m_pInfo->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		m_pWindow->addChild( m_pInfo );

		m_pInfo2 = FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u("消息反馈信息条2").c_str(), "Altas", 26 );
		m_pInfo2->setPosition(SCREEN_CENTER_POS + CCPoint(0, -250));
		m_pWindow->addChild( m_pInfo2 );

		return true;
	}
public:
	virtual void onClickListener(CCObject* pSender)
	{
		if( m_pInfo )
			m_pInfo->setString(FKCW_Base_StringConvert::a2u("单击消息").c_str());
	}
	virtual bool onLongClickListener(CCObject* pSender, CCTouch* pTouch)
	{
		if( m_pInfo )
			m_pInfo->setString(FKCW_Base_StringConvert::a2u("长按消息").c_str());
		//禁用按钮
		m_pButton->setEnabled(false);

		//返回true会继续产生click事件，否则不产生事件
		return false;
	}
	ENUM_WidgetTouchModel onTouchBegan(CCObject* pSender, CCTouch* pTouch)
	{
		if( m_pInfo2 )
			m_pInfo2->setString(FKCW_Base_StringConvert::a2u("Down消息").c_str());
		return eWidgetTouchTransient;
	}
	bool onTouchMoved(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		if( m_pInfo2 )
			m_pInfo2->setString(FKCW_Base_StringConvert::a2u("Moved消息").c_str());
		return true;
	}
	bool onTouchEnded(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		if( m_pInfo2 )
			m_pInfo2->setString(FKCW_Base_StringConvert::a2u("Ended消息").c_str());
		return true;
	}
	bool onTouchCancelled(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		if( m_pInfo2 )
			m_pInfo2->setString(FKCW_Base_StringConvert::a2u("Cancelled消息").c_str());
		return true;
	}
public:
	CREATE_FUNC( CTest_UIWidget_ButtonLayer );
private:
	FKCW_UIWidget_Button* m_pButton;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label* m_pInfo;
	FKCW_UIWidget_Label* m_pInfo2;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, Button, UIWidget按钮|长按按钮后会禁用该按钮)