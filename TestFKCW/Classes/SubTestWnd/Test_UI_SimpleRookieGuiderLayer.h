//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	Test_UI_SimpleRookieGuiderLayer.h
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
class CTest_UI_SimpleRookieGuiderLayer : public CCLayer
{
public:
	virtual bool init()
	{
		// 这里做部分初始化工作
		CCLabelTTF* pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我将开始新手引导").c_str(), "Arial", 36 );
		pLabel->setColor( ccc3(230,155,3) );

		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(pLabel, this, menu_selector(CTest_UI_SimpleRookieGuiderLayer::ShowGuiderCallback));
		CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
		pMenu->setPosition( SCREEN_CENTER_POS + ccp( 0, -50 ));
		addChild(pMenu, 1);

		m_pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("").c_str(), "Arial", 36 );
		m_pLabel->setPosition( SCREEN_CENTER_POS + ccp( 0, -200 ));
		addChild(m_pLabel);

		return true;
	}
public:
	void ShowGuiderCallback(CCObject* pSender)
	{
		m_pGuideLayer = FKCW_UI_SimpleRookieGuiderLayer::Create();
		m_pGuideLayer->setArrow( CCSprite::create( s_szArrowPath ));
		m_pGuideLayer->AddRegion( CCRectMake( 300, 300, 50, 50),
			CCCallFunc::create( this,callfunc_selector( CTest_UI_SimpleRookieGuiderLayer::OnClickFirstRegion )), false );
		m_pGuideLayer->PointToRegionCenter( 0, 60, 120.0f, 250.0f );
		addChild( m_pGuideLayer );
	}
public:
	void OnClickFirstRegion()
	{
		m_pLabel->setString( FKCW_Base_StringConvert::a2u("完成第一步点击").c_str() );

		m_pStatic = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("引导测试文字").c_str(), "Arial", 26 );
		m_pStatic->setColor( ccBLUE );
		m_pStatic->setPosition( SCREEN_CENTER_POS + ccp( 50, 200 ));
		addChild( m_pStatic, 1 );

		removeChild( m_pGuideLayer );
		m_pGuideLayer = FKCW_UI_SimpleRookieGuiderLayer::Create();
		m_pGuideLayer->setArrow( CCSprite::create( s_szArrowPath ));
		m_pGuideLayer->AddRegion( m_pStatic,
			CCCallFunc::create( this,callfunc_selector( CTest_UI_SimpleRookieGuiderLayer::OnClickSecondRegion )), false );
		m_pGuideLayer->PointToRegionCenter( 0, 60, 120.0f, 250.0f );
		addChild( m_pGuideLayer );
	}

	void OnClickSecondRegion()
	{
		m_pLabel->setString( FKCW_Base_StringConvert::a2u("完成第二步点击,下一次将需要强制等待3秒").c_str() );

		removeChild( m_pStatic );
		removeChild( m_pGuideLayer );
		m_pGuideLayer = FKCW_UI_SimpleRookieGuiderLayer::Create();
		m_pGuideLayer->setArrow( CCSprite::create( s_szArrowPath ));
		m_pGuideLayer->AddRegion( CCRectMake( 500, 400, 20, 30), NULL, true );
		m_pGuideLayer->AddRegion( CCRectMake( 150, 200, 100, 60), NULL, true );
		m_pGuideLayer->PointToRegionCenter( 0, 60, 120.0f, 250.0f );
		m_pGuideLayer->EnableRegionCheckAfter( 3.0f );
		addChild( m_pGuideLayer );
	}
public:
	CREATE_FUNC( CTest_UI_SimpleRookieGuiderLayer );
private:
	CCLabelTTF*							m_pStatic;
	FKCW_UI_SimpleRookieGuiderLayer*	m_pGuideLayer;
	CCLabelTTF*							m_pLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UI, SimpleRookieGuider, 新手引导层，可同时进行多个区域(或节点)遮罩，触摸后（触摸最短时间可限制）可回调。常在回调中创建新引导层)
//-------------------------------------------------------------------------