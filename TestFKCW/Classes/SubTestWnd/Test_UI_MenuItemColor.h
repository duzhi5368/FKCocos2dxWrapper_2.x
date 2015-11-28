//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	Test_UI_MenuItemColor.h
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
class CTest_UI_MenuItemColorLayer : public CCLayer
{
public:
	virtual bool init()
	{
		FKCW_UI_MenuItemColor* p1 = FKCW_UI_MenuItemColor::Create( ccc4( 50, 50, 230, 180 ));
		p1->setContentSize( CCSizeMake(50, 50) );
		FKCW_UI_MenuItemColor* p2 = FKCW_UI_MenuItemColor::Create( ccc4( 50, 50, 230, 255 ),
			ccc4( 0, 0, 130, 255 ), ccc4( 255, 0, 0, 255 ), this, 
			menu_selector(CTest_UI_MenuItemColorLayer::MenuHandlerFunc) );
		CCMenu* pMenu = CCMenu::create( p1, p2, NULL );
		pMenu->alignItemsVertically();
		pMenu->setPosition(SCREEN_CENTER_POS);
		this->addChild( pMenu );

		m_pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("").c_str(), "Arial", 26 );
		m_pLabel->setPosition( SCREEN_CENTER_POS + ccp( 0, -200 ));
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
	CREATE_FUNC( CTest_UI_MenuItemColorLayer );
private:
	CCLabelTTF* m_pLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UI, MenuItemColor, 一个自定义矩形方格，大小，各状态颜色均可用户指定，无需纹理图片。可进行消息回馈。可作为MenuItem)
//-------------------------------------------------------------------------