//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_Base_PinYin.h
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
class CTest_Base_PinYinLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		m_pButtonConvert = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		m_pButtonConvert->setPosition(SCREEN_CENTER_POS+ccp(0, 0));
		m_pButtonConvert->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_PinYinLayer::onConvert));
		m_pButtonConvert->initText(FKCW_Base_StringConvert::a2u("转换为拼音").c_str(), "", 30);
		m_pWindow->addChild(m_pButtonConvert);

		CCScale9Sprite* pEditBoxBG1 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG1 );
		m_pSrcEdit->setPosition( SCREEN_CENTER_POS + ccp(0, 100));
		m_pSrcEdit->setFontSize( 48 );
		m_pSrcEdit->setText( A2U("这里输入中文").c_str());
		m_pWindow->addChild( m_pSrcEdit );

		CCScale9Sprite* pEditBoxBG2 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pDestEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG2 );
		m_pDestEdit->setPosition( SCREEN_CENTER_POS + ccp(0, -100) );
		m_pDestEdit->setFontColor( ccRED );
		m_pDestEdit->setFontSize( 48 );
		m_pDestEdit->setText( A2U("这里输出拼音").c_str());
		m_pDestEdit->setEnabled( false );
		m_pWindow->addChild( m_pDestEdit );

		return true; 
	}
public:
	void onConvert( CCObject* pSender )
	{
		m_pDestEdit->setText( FKCW_Base_PinYinUtils::Chs2Pinyin( m_pSrcEdit->getText() ).c_str() );
	}
public:
	CREATE_FUNC( CTest_Base_PinYinLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Button*		m_pButtonConvert;
	CCEditBox*					m_pDestEdit;
	CCEditBox*					m_pSrcEdit;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, PinYin, 拼音解析)
//-------------------------------------------------------------------------