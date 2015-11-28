//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	Test_UI_MenuItemProgressTimer.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	带进度时间的菜单选项（多用于带自动CD的技能图标）测试用例
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UI_MenuItemProgressTimerLayer : public CCLayer
{
public:
	virtual bool init()
	{
		CCSprite* spriteMask1 = CCSprite::create(s_szLetterAImagePath);
		CCSprite* spriteMask2 = CCSprite::create(s_szLetterAImagePath);
		CCSprite* spriteNormal = CCSprite::create(s_szLetterBImagePath);
		CCSprite* spriteDis = CCSprite::create(s_szLetterCImagePath);
		
		FKCW_UI_MenuItemProgressTimer* item1 = FKCW_UI_MenuItemProgressTimer::Create(
			spriteMask1, 
			spriteNormal, 
			5.0f, 
			CCProgressTimerType::kCCProgressTimerTypeRadial, 
			this, 
			menu_selector(CTest_UI_MenuItemProgressTimerLayer::PressProgressTimerItemCallback1) );

		FKCW_UI_MenuItemProgressTimer* item2 = FKCW_UI_MenuItemProgressTimer::Create(
			spriteMask2, 
			spriteDis, 
			5.0f, 
			CCProgressTimerType::kCCProgressTimerTypeBar, 
			this, 
			menu_selector(CTest_UI_MenuItemProgressTimerLayer::PressProgressTimerItemCallback2) );

		CCMenu* menu = CCMenu::create(item1,item2, NULL);
		menu->alignItemsHorizontally();
		this->addChild(menu);
		
		m_pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点击图片测试效果").c_str(), "Arial", 36 );
		m_pLabel->setPosition( SCREEN_CENTER_POS + ccp( 0, -200 ));
		addChild(m_pLabel);

		return true;
	}

public:
	CREATE_FUNC( CTest_UI_MenuItemProgressTimerLayer );
public:
	void PressProgressTimerItemCallback1(CCObject* pSender)
	{
		m_pLabel->setString( FKCW_Base_StringConvert::a2u("按下进度条事件[1]").c_str() );
	}
	void PressProgressTimerItemCallback2(CCObject* pSender)
	{
		m_pLabel->setString( FKCW_Base_StringConvert::a2u("按下进度条事件[2]").c_str() );
	}
private:
	CCLabelTTF* m_pLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UI, MenuItemProgressTimer, 带进度时间的菜单选项-技能按钮冷却效果)
//-------------------------------------------------------------------------