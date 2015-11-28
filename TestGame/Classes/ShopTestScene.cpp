//-------------------------------------------------------------------------
#include "ShopTestScene.h"
//-------------------------------------------------------------------------


CShopTestScene::CShopTestScene(void)
{
}

//-------------------------------------------------------------------------
// 初始异步加载资源
void CShopTestScene::onLoadResources()
{

}
//-------------------------------------------------------------------------
// 其他逻辑初始化
void CShopTestScene::onLoadScene()
{
	setAutoRemoveUnusedTexture( true );

	// 根窗口,用来挂载显示物件
	{
		m_pRootWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pRootWindow->setMultiTouchEnabled(true);
		this->addChild(m_pRootWindow);
	}
	
	// 返回按钮
	{
		CCMenu *pMenu = CCMenu::create();
		pMenu->setPosition(CCPointZero);
		addChild(pMenu);
		CCMenuItemImage* pBtn = CCMenuItemImage::create();
		CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("back_button_normal_e.png"); 
		pBtn->setNormalSpriteFrame(pFrame);
		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("back_button_touch_e.png"); 
		pBtn->setSelectedSpriteFrame(pFrame);
		pBtn->setTarget(this, menu_selector(CShopTestScene::onClick));
		pBtn->setPosition(SCREEN_CENTER_POS + ccp(0,400));
		pMenu->addChild(pBtn);
	}
}
//-------------------------------------------------------------------------
void CShopTestScene::onClick(CCObject* sender)
{

	FKCW_SceneExMgr_SceneExMgr::sharedManager()->popScene(this);
}
//-------------------------------------------------------------------------
// 当场景显示时调用
void CShopTestScene::onEnterScene()
{

}
//-------------------------------------------------------------------------
// 当场景隐藏时调用
void CShopTestScene::onExitScene()
{

}
//-------------------------------------------------------------------------
