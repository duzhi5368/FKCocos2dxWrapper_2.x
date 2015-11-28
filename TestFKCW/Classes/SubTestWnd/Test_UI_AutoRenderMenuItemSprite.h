//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	Test_UI_AutoRenderMenuItemSprite.h
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
class CTest_UI_AutoRenderMenuItemSpriteLayer : public CCLayer
{
public:
	virtual bool init()
	{
		CCSprite* p1 = CCSprite::create( s_szButtonNormalPath );
		FKCW_UI_AutoRenderMenuItemSprite* pMenuItemSprite1 = FKCW_UI_AutoRenderMenuItemSprite::Create( p1, this,
			menu_selector(CTest_UI_AutoRenderMenuItemSpriteLayer::MenuHandlerFunc) );
		CCSprite* p2 = CCSprite::create( s_szButtonNormalPath );
		FKCW_UI_AutoRenderMenuItemSprite* pMenuItemSprite2 = FKCW_UI_AutoRenderMenuItemSprite::Create( p2, this,
			menu_selector(CTest_UI_AutoRenderMenuItemSpriteLayer::MenuHandlerFunc) );
		CCMenu* pMenu = CCMenu::create( pMenuItemSprite1, pMenuItemSprite2, NULL );
		pMenu->alignItemsVertically();
		pMenu->setPosition(SCREEN_CENTER_POS);
		this->addChild( pMenu );

		m_pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("").c_str(), "Arial", 26 );
		m_pLabel->setPosition( SCREEN_CENTER_POS + ccp( 0, -250 ));
		addChild(m_pLabel);

		return true;
	}
public:
	void MenuHandlerFunc(CCObject* p)
	{
		char szMsg[32];
		memset( szMsg, 0, 32 );
		sprintf( szMsg, "按钮 %d 被按下事件", p->m_uID );
		m_pLabel->setString( FKCW_Base_StringConvert::a2u(szMsg).c_str() );
	}
public:
	CREATE_FUNC( CTest_UI_AutoRenderMenuItemSpriteLayer );
public:
	CCLabelTTF* m_pLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UI, AutoRenderMenuItemSprite, 一个按下时可自动进行变暗的MenuItem，也可作为Button按钮的方式使用)
//-------------------------------------------------------------------------