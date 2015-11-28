/*!
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#include "TestScene.h"
#include "ListControllerLayer.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "resource.h"
//--------------------------------------------------------
void CTestScene::SetInfo( std::string p_szInfo )
{
	m_szInfo = p_szInfo;
}
//--------------------------------------------------------
void CTestScene::onEnterScene()
{
	FKCW_SceneExMgr_SceneEx::onEnterScene();

	CCSprite* pBack = CCSprite::create( s_szCloseBtnImagePath );	
	CCMenuItemSprite* pMenuItem = CCMenuItemSprite::create(pBack, NULL, this, menu_selector(CTestScene::MainMenuCallback));
	CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
	pMenu->setPosition( CCPointZero );
	pMenuItem->setPosition( ccp( CVisibleRect::right().x - 50, CVisibleRect::bottom().y + 50) );
	addChild(pMenu, 1);

	const static unsigned int MAX_LINE_TEXT_NUM = 64;
	if( m_szInfo.length() <= MAX_LINE_TEXT_NUM )
	{
		CCLabelTTF* pInfo = CCLabelTTF::create(FKCW_Base_StringConvert::a2u( m_szInfo.c_str() ).c_str(), "Arial", 26 );
		pInfo->setColor( ccc3(209,73,78) );
		pInfo->setPosition( ccp( CVisibleRect::center().x, CVisibleRect::top().y - 50 ));
		addChild( pInfo, 1 );
	}
	else
	{
		std::string szInfo1 = m_szInfo.substr( 0, MAX_LINE_TEXT_NUM );
		std::string szInfo2 = m_szInfo.substr( MAX_LINE_TEXT_NUM, m_szInfo.length() - MAX_LINE_TEXT_NUM );
		CCLabelTTF* pInfo1 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u( szInfo1.c_str() ).c_str(), "Arial", 26 );
		pInfo1->setColor( ccc3(209,73,78) );
		pInfo1->setPosition( ccp( CVisibleRect::center().x, CVisibleRect::top().y - 50 ));
		addChild( pInfo1, 1 );

		CCLabelTTF* pInfo2 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u( szInfo2.c_str() ).c_str(), "Arial", 26 );
		pInfo2->setColor( ccc3(209,73,78) );
		pInfo2->setPosition( ccp( CVisibleRect::center().x, CVisibleRect::top().y - 90 ));
		addChild( pInfo2, 1 );
	}

	// 自定义初始化
	Run();
}
//--------------------------------------------------------
void CTestScene::MainMenuCallback(CCObject* pSender)
{
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->popScene();
}
//--------------------------------------------------------