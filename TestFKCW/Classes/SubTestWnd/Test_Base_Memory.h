//*************************************************************************
//	创建日期:	2014-12-22
//	文件名称:	Test_Base_Memory.h
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
class CTest_Base_MemoryLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);
		
		m_pButtonDump = FKCW_UIWidget_Button::createWith9Sprite(CCSize(350, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		m_pButtonDump->setPosition(SCREEN_CENTER_POS+ccp(0, 0));
		m_pButtonDump->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_MemoryLayer::onShowDump));
		m_pButtonDump->initText(FKCW_Base_StringConvert::a2u("控制台输出内存泄露情况").c_str(), "", 30);
		m_pWindow->addChild(m_pButtonDump);

		m_pButtonUsage = FKCW_UIWidget_Button::createWith9Sprite(CCSize(350, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		m_pButtonUsage->setPosition(SCREEN_CENTER_POS+ccp(0, -100));
		m_pButtonUsage->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_MemoryLayer::onShowUsage));
		m_pButtonUsage->initText(FKCW_Base_StringConvert::a2u("控制台输出内存占用情况").c_str(), "", 30);
		m_pWindow->addChild(m_pButtonUsage);

		return true;
	}
public:
	void onShowDump(CCObject* pSender)
	{
		FKCW_Base_Memory::PrintDump();
	}

	void onShowUsage(CCObject* pSender)
	{
		FKCW_Base_Memory::PrintUasge();
	}
public:
	CREATE_FUNC( CTest_Base_MemoryLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Button*		m_pButtonDump;
	FKCW_UIWidget_Button*		m_pButtonUsage;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, Memory, 内存检查)
//-------------------------------------------------------------------------