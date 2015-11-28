//-------------------------------------------------------------------------
#include "LoginScene.h"
#include "TestApp.h"
#include "AudioBridge.h"
//-------------------------------------------------------------------------
CLoginScene::CLoginScene()
{

}
//-------------------------------------------------------------------------
// 初始异步加载资源
void CLoginScene::onLoadResources()
{

}
//-------------------------------------------------------------------------
// 其他逻辑初始化
void CLoginScene::onLoadScene()
{
	setAutoRemoveUnusedTexture( true );

	__CreateMainUI();
}
//-------------------------------------------------------------------------
// 当场景显示时调用
void CLoginScene::onEnterScene()
{
	addChild( GetUISystem()->GetRoot() );

	CAudioBridge::GetInstance().PlayBackGroudMusic(eSubSceneType_Login);
}
//-------------------------------------------------------------------------
// 当场景隐藏时调用
void CLoginScene::onExitScene()
{
	removeChild( GetUISystem()->GetRoot() );

	CAudioBridge::GetInstance().StopBackGroundMusic();	
}
//-------------------------------------------------------------------------
// 退出游戏按钮
void CLoginScene::OnExitBtn(CCObject* pSender)
{
	// 测试输出dump
	GetDebugSystem()->DumpAll();
	
	// 测试退出
	GetApp()->Release();
}
//-------------------------------------------------------------------------
// 功能切换按钮
void CLoginScene::OnSelectBtn(CCObject* pSender)
{
	if (pSender == NULL)
	{
		return;
	}
	FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pSender);
	if (pWidget == NULL)
	{
		return;
	}
	int nTag = pWidget->getUserTag();
	if (nTag <= eSubSceneType_Min)
	{
		return;
	}

	IEventSystem* pEvent = GetEventSystem();
	if (pEvent == NULL)
	{
		return;
	}
	pEvent->FireExec(eEvent_UIEvent,eEvent_Scene_Switch,nTag,NULL,0);
}
//-------------------------------------------------------------------------
void CLoginScene::__CreateMainUI()
{
	IUISystem* pUISystem = GetUISystem();
	if (pUISystem == NULL)
	{
		return;
	}

	FKCW_UIWidget_WidgetWindow* pRootWindow = pUISystem->GetRoot();
	if (pRootWindow == NULL)
	{
		return;
	}

	// 背景小图片们
	{
		FKCW_UIWidget_ImageView* pBackGroundImage = FKCW_UIWidget_ImageView::create(s_szLoginBgPath);
		pBackGroundImage->setScaleX(SCERRN_WIDTH / pBackGroundImage->getContentSize().width);
		pBackGroundImage->setScaleY(SCERRN_HEIGHT / pBackGroundImage->getContentSize().height);
		pBackGroundImage->setPosition( SCREEN_CENTER_POS);
		addChild( pBackGroundImage );

		// cloud
		FKCW_UIWidget_ImageView* pCloudImage = FKCW_UIWidget_ImageView::create(s_szCloudBgPath);
		pCloudImage->setPosition(SCREEN_LEFT_UP_POS + ccp(0,-150));
		float nWidth = pCloudImage->getContentSize().width;
		CCMoveBy* pMoveRight = CCMoveBy::create(15.0f, ccp(nWidth*1.1, 0));
		CCMoveBy* pMoveLeft = CCMoveBy::create(15.0f, ccp(-nWidth*1.1, 0));
		CCSequence* pSeq = CCSequence::create(pMoveRight, pMoveLeft, NULL);
		pCloudImage->runAction(CCRepeatForever::create(pSeq));
		addChild( pCloudImage );

		// logo
		FKCW_UIWidget_ImageView* pLogoImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szLogoFrameName);
		pLogoImage->setPosition( SCREEN_CENTER_POS + ccp(0, pLogoImage->getContentSize().height/2));
		addChild( pLogoImage );

		// brid
		FKCW_UIWidget_ImageView* pBirdImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szBirdFrameName);
		pBirdImage->setPosition( SCREEN_CENTER_POS + ccp(-200, - SCERRN_HEIGHT*0.3));
		CCMoveBy* pMoveRightB = CCMoveBy::create(0.8f, ccp(0, 15));
		CCMoveBy* pMoveLeftB = CCMoveBy::create(0.8f, ccp(0, -15));
		CCSequence* pSeqB = CCSequence::create(pMoveRightB, pMoveLeftB, NULL);
		pBirdImage->runAction(CCRepeatForever::create(pSeqB));
		addChild( pBirdImage );

		// rat
		FKCW_UIWidget_ImageView* pRatImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szRatFrameName);
		pRatImage->setPosition( SCREEN_CENTER_POS + ccp(200, - SCERRN_HEIGHT*0.3));
		CCMoveBy* pMoveRightR = CCMoveBy::create(0.8f, ccp(0, 15));
		CCMoveBy* pMoveLeftR = CCMoveBy::create(0.8f, ccp(0, -15));
		CCSequence* pSeqR = CCSequence::create(pMoveRightR, pMoveLeftR, NULL);
		pRatImage->runAction(CCRepeatForever::create(pSeqR));
		addChild( pRatImage );
	}
	// 小按钮们
	{
		CCPoint btnPos = SCREEN_CENTER_POS + ccp(0,- SCERRN_HEIGHT*0.3);
		FKCW_UIWidget_Button* pBeginBtn = __CreateButton(eSubSceneType_Play,btnPos,
			s_szStartButtonNom,s_szStartButtonTou,"play");
		pRootWindow->addChild( pBeginBtn );

		btnPos = SCREEN_LEFT_UP_POS + ccp(100,-60);
		FKCW_UIWidget_Button* pShopBtn = __CreateButton(eSubSceneType_Shop,btnPos,
			s_szShopButtonNom,s_szShopButtonTou,"shop");
		pRootWindow->addChild( pShopBtn );

		btnPos = SCREEN_LEFT_UP_POS + ccp(80,-200);
		FKCW_UIWidget_Button* pAchieBtn = __CreateButton(eSubSceneType_Achie,btnPos,
			s_szAchButtonNom,s_szAchButtonTou,"achie");
		pRootWindow->addChild( pAchieBtn );
	}

	// 常规退出游戏按钮
	{
		CCLabelTTF* pLabel = CCLabelTTF::create(A2U("退出游戏").c_str(), "Arial", 25 );
		pLabel->setColor( ccc3(209, 73, 78) );
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(pLabel, this, menu_selector(CLoginScene::OnExitBtn));
		CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
		pMenu->setPosition( SCREEN_RIGHT_DOWN_POS + ccp( -100, 40 ));
		addChild(pMenu, 1);
	}

	// test code
	{
		// 隐藏俩按钮
		// m_pShopBtn->setVisible(false);
		// m_pAchieBtn->setVisible(false);

		// 测试一下MessageBox
		// CMsgBoxMgr::instance().ShowAwardBox(this,pRootWindow,eAwardType_PetGift);
	}
}
//-------------------------------------------------------------------------
// 单个button
FKCW_UIWidget_Button* CLoginScene::__CreateButton(
	int nType,CCPoint& pos,const char* pszImageName,const char* pszImageSecectName,const char* pszName)
{
	
	FKCW_UIWidget_Button* pBtn = FKCW_UIWidget_Button::create();
	pBtn->setNormalSpriteFrameName(pszImageName);
	pBtn->setSelectedSpriteFrameName(pszImageSecectName);
	pBtn->setDescription(pszName);
	pBtn->setUserTag(nType);
	pBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(CLoginScene::OnSelectBtn));
	pBtn->setPosition(pos);
	return pBtn;
}
//-------------------------------------------------------------------------
void CLoginScene::OnMessageBoxCallBack(int nMessageKey)
{
}
//-------------------------------------------------------------------------