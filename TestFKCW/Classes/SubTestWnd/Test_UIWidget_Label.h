//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_UIWidget_Label.h
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
class CTest_UIWidget_LabelLayer : public CCLayer
{
public:
	bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_Label* pText1 = FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u("我是静态的别理我").c_str(), "", 30.0f);
		pText1->enableShadow(CCSizeMake(2.0f, -2.0f), 0, 0);
		pText1->setPosition(CCPoint(SCREEN_CENTER_POS_X, 450));
		m_pWindow->addChild(pText1);

		FKCW_UIWidget_Label* pText2 = FKCW_UIWidget_Label::create(FKCW_Base_StringConvert::a2u("点我有消息哦，长点也有").c_str(), "", 30.0f);
		pText2->setTouchEnabled(true);
		pText2->setOnLongClickListener(this, fkcw_uiwidget_longclick_selector(CTest_UIWidget_LabelLayer::onLongClick));
		pText2->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_LabelLayer::onClick));
		pText2->setOnTouchBeganListener(this, fkcw_uiwidget_touchbegan_selector(CTest_UIWidget_LabelLayer::onTouchBegan));
		pText2->setOnTouchMovedListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_LabelLayer::onTouchMoved));
		pText2->setOnTouchEndedListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_LabelLayer::onTouchEnded));
		pText2->setOnTouchCancelledListener(this, fkcw_uiwidget_touchevent_selector(CTest_UIWidget_LabelLayer::onTouchEnded));
		pText2->setColor(ccc3(0, 255, 0));
		pText2->setPosition(CCPoint(SCREEN_CENTER_POS_X, 380));
		m_pWindow->addChild(pText2);

		pText3 = FKCW_UIWidget_Label::create("", "", 30.0f);
		pText3->setPosition(CCPoint(750, 380));
		m_pWindow->addChild(pText3);

		return true;
	}
public:
	ENUM_WidgetTouchModel onTouchBegan(CCObject* pSender, CCTouch* pTouch)
	{
		FKCW_UIWidget_Label* pText = (FKCW_UIWidget_Label*) pSender;
		pText->stopActionByTag(1);
		CCScaleTo* pScaleTo = CCScaleTo::create(0.1f, 1.3f);
		pScaleTo->setTag(1);
		pText->runAction(pScaleTo);

		return eWidgetTouchTransient;
	}
	bool onTouchMoved(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		return true;
	}
	bool onTouchEnded(CCObject* pSender, CCTouch* pTouch, float fDuration)
	{
		FKCW_UIWidget_Label* pText = (FKCW_UIWidget_Label*) pSender;
		pText->stopActionByTag(1);
		CCScaleTo* pScaleTo = CCScaleTo::create(0.1f, 1.0f);
		pScaleTo->setTag(1);
		pText->runAction(pScaleTo);
		return true;
	}
	void onClick(CCObject* pSender)
	{
		pText3->setString("Clicked");
	}
	bool onLongClick(CCObject* pSender, CCTouch* pTouch)
	{
		pText3->setString("LongClicked");
		return false;
	}
public:
	CREATE_FUNC( CTest_UIWidget_LabelLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label* pText3;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, Label, 静态文字标签)