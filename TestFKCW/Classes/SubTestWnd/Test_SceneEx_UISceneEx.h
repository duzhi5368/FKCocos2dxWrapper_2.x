//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_SceneEx_UISceneEx.h
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
// 测试UIScene
//-------------------------------------------------------------------------
class CTestUIScene : public FKCW_SceneExMgr_SceneEx
{
public:
	CREATE_SCENE_FUNC(CTestUIScene);
public:

	virtual void onLoadResources()
	{
		addImage(s_szSmallPanelPath);
	}
	virtual void onLoadScene()
	{
		setAutoRemoveUnusedTexture(true);

		CCSprite* bg = CCSprite::create(s_szSmallPanelPath);
		bg->setPosition(SCREEN_CENTER_POS);
		addChild(bg);

		CCLabelTTF* pLabel = CCLabelTTF::create( A2U("子UI面板").c_str(), "", 26);
		pLabel->setFontSize( 36 );
		pLabel->setPosition(SCREEN_CENTER_POS + ccp(0, 50));
		addChild(pLabel);

		m_pMenu = CCMenu::create();
		m_pMenu->setPosition(CCPointZero);
		addChild(m_pMenu);

		CCMenuItemImage* pBtn = CCMenuItemImage::create(s_szCloseBtnImagePath, s_szCloseBtnImagePath);
		pBtn->setTarget(this, menu_selector(CTestUIScene::onClick));
		pBtn->setPosition(SCREEN_CENTER_POS + ccp(0, -50));
		m_pMenu->addChild(pBtn);
	}
	virtual void onEnterScene()
	{
		m_pMenu->setTouchPriority(FKCW_SceneExMgr_SceneExMgr::sharedManager()->getTouchPriority());
	}

	void onClick(CCObject* sender)
	{
		FKCW_SceneExMgr_SceneExMgr::sharedManager()->popUIScene(this);
	}
private:
	CCMenu* m_pMenu;
};
//-------------------------------------------------------------------------
class CTest_SceneEx_TestTransitionScene : public FKCW_SceneExMgr_SceneEx
{
public:
	CREATE_SCENE_FUNC(CTest_SceneEx_TestTransitionScene);
public:
	CTest_SceneEx_TestTransitionScene()
	{
	}
	virtual void onLoadResources()
	{
		addImageAsync( s_szBackground1Path );
	}
	virtual void onLoadScene()
	{
		setAutoRemoveUnusedTexture(true);

		CCSprite* bg = CCSprite::create( s_szBackground1Path );
		bg->setPosition(SCREEN_CENTER_POS);
		bg->setColor( ccRED );
		bg->setScale( 2.0f );
		addChild(bg);

		m_pMenu = CCMenu::create();
		m_pMenu->setPosition(CCPointZero);
		addChild(m_pMenu);

		CCMenuItemFont* pFont = CCMenuItemFont::create( A2U("返回上一场景").c_str() );
		pFont->setTarget(this, menu_selector(CTest_SceneEx_TestTransitionScene::onClick));
		pFont->setFontSizeObj(26);
		pFont->setPosition(CCPoint(800, 40));
		pFont->setTag(1);
		m_pMenu->addChild(pFont);

		CCLabelTTF* lable1 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("涡轮效果场景切换").c_str(), "Arial", 26 );
		lable1->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable1 = CCMenuItemLabel::create(lable1, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable1->setTag( 1 );
		itemLable1->setPosition( SCREEN_CENTER_POS+ccp(0, 250+25) );

		CCLabelTTF* lable2 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("跳跃缩放场景切换").c_str(), "Arial", 26 );
		lable2->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable2 = CCMenuItemLabel::create(lable2, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable2->setTag( 2 );
		itemLable2->setPosition( SCREEN_CENTER_POS+ccp(0, 210+25) );

		CCLabelTTF* lable3 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("覆盖切换场景切换").c_str(), "Arial", 26 );
		lable3->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable3 = CCMenuItemLabel::create(lable3, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable3->setTag( 3 );
		itemLable3->setPosition( SCREEN_CENTER_POS+ccp(0, 170+25) );

		CCLabelTTF* lable4 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("推动切换场景切换").c_str(), "Arial", 26 );
		lable4->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable4 = CCMenuItemLabel::create(lable4, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable4->setTag( 4 );
		itemLable4->setPosition( SCREEN_CENTER_POS+ccp(0, 130+25) );

		CCLabelTTF* lable5 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("环绕切换场景切换").c_str(), "Arial", 26 );
		lable5->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable5 = CCMenuItemLabel::create(lable5, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable5->setTag( 5 );
		itemLable5->setPosition( SCREEN_CENTER_POS+ccp(0, 90+25) );

		CCLabelTTF* lable6 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("旋转切换场景切换").c_str(), "Arial", 26 );
		lable6->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable6 = CCMenuItemLabel::create(lable6, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable6->setTag( 6 );
		itemLable6->setPosition( SCREEN_CENTER_POS+ccp(0, 50+25) );

		CCLabelTTF* lable7 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("旋转缩放场景切换").c_str(), "Arial", 26 );
		lable7->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable7 = CCMenuItemLabel::create(lable7, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable7->setTag( 7 );
		itemLable7->setPosition( SCREEN_CENTER_POS+ccp(0, 10+25) );

		CCLabelTTF* lable8 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("淡入淡出场景切换").c_str(), "Arial", 26 );
		lable8->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable8 = CCMenuItemLabel::create(lable8, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable8->setTag( 8 );
		itemLable8->setPosition( SCREEN_CENTER_POS+ccp(0, -30+25) );

		CCLabelTTF* lable9 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("融合混合场景切换").c_str(), "Arial", 26 );
		lable9->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable9 = CCMenuItemLabel::create(lable9, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable9->setTag( 9 );
		itemLable9->setPosition( SCREEN_CENTER_POS+ccp(0, -70+25) );

		CCLabelTTF* lable10 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("随机格子场景切换").c_str(), "Arial", 26 );
		lable10->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable10 = CCMenuItemLabel::create(lable10, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable10->setTag( 10 );
		itemLable10->setPosition( SCREEN_CENTER_POS+ccp(0, -110+25) );

		CCLabelTTF* lable11 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("横条滑动场景切换").c_str(), "Arial", 26 );
		lable11->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable11 = CCMenuItemLabel::create(lable11, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable11->setTag( 11 );
		itemLable11->setPosition( SCREEN_CENTER_POS+ccp(0, -150+25) );

		CCLabelTTF* lable12 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("方格渐隐场景切换").c_str(), "Arial", 26 );
		lable12->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable12 = CCMenuItemLabel::create(lable12, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable12->setTag( 12 );
		itemLable12->setPosition( SCREEN_CENTER_POS+ccp(0, -190+25) );

		CCLabelTTF* lable13 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("前后翻页场景切换").c_str(), "Arial", 26 );
		lable13->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable13 = CCMenuItemLabel::create(lable13, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable13->setTag( 13 );
		itemLable13->setPosition( SCREEN_CENTER_POS+ccp(0, -230+25) );

		CCLabelTTF* lable14 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("钟表进度场景切换").c_str(), "Arial", 26 );
		lable14->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable14 = CCMenuItemLabel::create(lable14, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable14->setTag( 14 );
		itemLable14->setPosition( SCREEN_CENTER_POS+ccp(0, -270+25) );

		CCLabelTTF* lable15 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("中心扩散场景切换").c_str(), "Arial", 26 );
		lable15->setColor(ccc3(230,155,3));
		CCMenuItemLabel* itemLable15 = CCMenuItemLabel::create(lable15, this, menu_selector(CTest_SceneEx_TestTransitionScene::OnChangeScene));
		itemLable15->setTag( 15 );
		itemLable15->setPosition( SCREEN_CENTER_POS+ccp(0, -310+25) );

		CCMenu* menu = CCMenu::create(itemLable1, itemLable2, itemLable3, itemLable4, itemLable5,
			itemLable6, itemLable7, itemLable8, itemLable9, itemLable10, itemLable11,
			itemLable12, itemLable13, itemLable14, itemLable15, NULL);
		menu->setPosition(ccp(0,0));
		this->addChild(menu);
	}
public:
	void onClick(CCObject* sender)
	{
		CCNode* pNode = (CCNode*) sender;
		switch( pNode->getTag() )
		{
		case 1:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene(LoadScene("CTest_SceneEx_UISceneExScene") );
				//FKCW_SceneExMgr_SceneExMgr::sharedManager()->popScene();
			}
			break;
		}
	}
	void OnChangeScene(CCObject* sender)
	{
		CCNode* pNode = (CCNode*) sender;
		switch( pNode->getTag() )
		{
		case 1:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionRotoZoom::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 2:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionJumpZoom::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 3:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionMoveInL::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 4:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionSlideInL::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 5:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionShrinkGrow::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 6:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionFlipAngular::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 7:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionZoomFlipY::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 8:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionFadeColor::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 9:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionCrossFade::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 10:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 11:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionSplitRows::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 12:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionFadeTR::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 13:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionPageTurn::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene"), true ) );
			}
			break;
		case 14:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		case 15:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene( 
					FKCW_SceneExMgr_SceneExTransitionProgressInOut::create(2.0f,
					LoadScene("CTest_SceneEx_UISceneExScene") ) );
			}
			break;
		}
	}
private:
	CCMenu* m_pMenu;
};
//-------------------------------------------------------------------------
class CTest_SceneEx_UISceneExScene : public CTestScene
{
public:
	CREATE_SCENE_FUNC(CTest_SceneEx_UISceneExScene);
public:
	CTest_SceneEx_UISceneExScene()
	{
		SetInfo("UISceneEx用来管理全部UI对象");
	}
	virtual void onLoadResources()
	{
		addImageAsync( s_szBackground1Path );
		addImageAsync( s_szButtonNormalPath );
		addImageAsync( s_szButtonPressPath );
	}
	virtual void onLoadScene()
	{
		setAutoRemoveUnusedTexture(true);

		CCSprite* bg = CCSprite::create( s_szBackground1Path );
		bg->setPosition(SCREEN_CENTER_POS);
		bg->setScale( 2.0f );
		addChild(bg);

		m_pMenu = CCMenu::create();
		m_pMenu->setPosition(CCPointZero);
		addChild(m_pMenu);

		CCMenuItemFont* pFont = CCMenuItemFont::create( A2U("进入过渡场景测试").c_str() );
		pFont->setTarget(this, menu_selector(CTest_SceneEx_UISceneExScene::onClick));
		pFont->setFontSizeObj(26);
		pFont->setPosition(CCPoint(800, 40));
		pFont->setTag(1);
		m_pMenu->addChild(pFont);

		CCMenuItemImage* pUiSceneBtn = CCMenuItemImage::create( s_szButtonNormalPath, s_szButtonPressPath);
		pUiSceneBtn->setTarget(this, menu_selector(CTest_SceneEx_UISceneExScene::onClick));
		pUiSceneBtn->setPosition(SCREEN_CENTER_POS + ccp(0, -100));
		pUiSceneBtn->setTag(2);
		m_pMenu->addChild(pUiSceneBtn);

		CCLabelTTF* pLabel1 = CCLabelTTF::create( A2U("运行UIScene").c_str() , "", 30);
		pLabel1->setColor(ccRED);
		pLabel1->setPosition(SCREEN_CENTER_POS + ccp(0, -100));
		addChild(pLabel1);

		CCLabelTTF* pLabel = CCLabelTTF::create( A2U("演示SceneEx").c_str(), "", 26);
		pLabel->setColor(ccRED);
		pLabel->setFontSize( 36 );
		pLabel->setPosition(SCREEN_CENTER_POS + ccp(0, 100));
		addChild(pLabel);

		CCMenuItemImage* pBtn = CCMenuItemImage::create(s_szCloseBtnImagePath, s_szCloseBtnImagePath);
		pBtn->setTarget(this, menu_selector(CTest_SceneEx_UISceneExScene::onClose));
		pBtn->setPosition(ccp( CVisibleRect::right().x - 50, CVisibleRect::bottom().y + 50) );
		m_pMenu->addChild(pBtn);

		// 注册UI场景
		REGISTER_SCENE_FUNC(CTestUIScene);
		// 注册子场景
		REGISTER_SCENE_FUNC(CTest_SceneEx_TestTransitionScene);
	}
	virtual void onEnterScene()
	{
		m_pMenu->setTouchPriority(FKCW_SceneExMgr_SceneExMgr::sharedManager()->getTouchPriority());
	}
	virtual void Run()
	{
		// do nothing.
	}
public:

	void onClose(CCObject* sender)
	{
		FKCW_SceneExMgr_SceneExMgr::sharedManager()->popScene();
	}

	void onClick(CCObject* sender)
	{
		CCNode* pNode = (CCNode*) sender;
		switch( pNode->getTag() )
		{
		case 1:
			{
				FKCW_SceneExMgr_SceneEx* pTestUIScene = SeekScene("CTestUIScene");
				if (pTestUIScene != NULL)
				{
					FKCW_SceneExMgr_SceneExMgr::sharedManager()->popUIScene(pTestUIScene);
				}
				
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->replaceScene(LoadScene("CTest_SceneEx_TestTransitionScene") );
			}
			break;
		case 2:
			{
				FKCW_SceneExMgr_SceneExMgr::sharedManager()->runUIScene(LoadScene("CTestUIScene"));
			}
			break;
		}
	}
private:
	CCMenu* m_pMenu;
};
//-------------------------------------------------------------------------