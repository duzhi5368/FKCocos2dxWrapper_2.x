//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionProgress.h"
//-------------------------------------------------------------------------
enum {
	kCCSceneRadial = 0xc001,
};
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgress::FKCW_SceneExMgr_SceneExTransitionProgress()
	: m_fTo(0.0f)
	, m_fFrom(0.0f)
	, m_pSceneToBeModified(NULL)
{

}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgress* FKCW_SceneExMgr_SceneExTransitionProgress::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgress* pScene = new FKCW_SceneExMgr_SceneExTransitionProgress();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgress::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();

	setupTransition();

	// 创建一个带颜色的Layer，用于放置自己的renderTexture
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// 为老场景创建一个渲染纹理
	CCRenderTexture *texture = CCRenderTexture::create((int)size.width, (int)size.height);
	texture->getSprite()->setAnchorPoint(ccp(0.5f,0.5f));
	texture->setPosition(ccp(size.width/2, size.height/2));
	texture->setAnchorPoint(ccp(0.5f,0.5f));

	// 渲染老场景到纹理
	texture->clear(0, 0, 0, 1);
	texture->begin();
	m_pSceneToBeModified->visit();
	texture->end();

	// 隐藏老场景，现在不用它了
	if (m_pSceneToBeModified == m_pOutScene)
	{
		hideOutShowIn();
	}
	//  获取渲染纹理
	CCProgressTimer *pNode = progressTimerNodeWithRenderTexture(texture);

	// 创建混合动作
	CCActionInterval* layerAction = (CCActionInterval*)CCSequence::create(
		CCProgressFromTo::create(m_fDuration, m_fFrom, m_fTo),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransitionProgress::finish)), 
		NULL);
	// 执行混合动作
	pNode->runAction(layerAction);

	// 添加携带有自己renderTexture的Layer到当前场景
	addChild(pNode, 2, kCCSceneRadial);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgress::onExit()
{
	// 移除自身Layer
	removeChildByTag(kCCSceneRadial, true);
	FKCW_SceneExMgr_SceneExTransition::onExit();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgress::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgress::setupTransition()
{
	m_pSceneToBeModified = m_pOutScene;
	m_fFrom = 100;
	m_fTo = 0;
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgress::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{
	CCAssert(false, "override me - abstract class");
	return NULL;
}
//-------------------------------------------------------------------------
// ProgressRadialCCW
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType(kCCProgressTimerTypeRadial);

	pNode->setReverseDirection(false);
	pNode->setPercentage(100);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW* FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
// ProgressRadialCW
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressRadialCW* FKCW_SceneExMgr_SceneExTransitionProgressRadialCW::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressRadialCW* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressRadialCW();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressRadialCW::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType( kCCProgressTimerTypeRadial );

	pNode->setReverseDirection(true);
	pNode->setPercentage(100);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------
// ProgressHorizontal
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressHorizontal* FKCW_SceneExMgr_SceneExTransitionProgressHorizontal::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressHorizontal* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressHorizontal();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressHorizontal::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{    
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType( kCCProgressTimerTypeBar);

	pNode->setMidpoint(ccp(1, 0));
	pNode->setBarChangeRate(ccp(1,0));

	pNode->setPercentage(100);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------
// ProgressVertical
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressVertical* FKCW_SceneExMgr_SceneExTransitionProgressVertical::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressVertical* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressVertical();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressVertical::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{    
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType(kCCProgressTimerTypeBar);

	pNode->setMidpoint(ccp(0, 0));
	pNode->setBarChangeRate(ccp(0,1));

	pNode->setPercentage(100);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------
// ProgressInOut
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressInOut* FKCW_SceneExMgr_SceneExTransitionProgressInOut::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressInOut* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressInOut();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgressInOut::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionProgressInOut::setupTransition()
{
	m_pSceneToBeModified = m_pInScene;
	m_fFrom = 0;
	m_fTo = 100;    
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressInOut::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{    
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType( kCCProgressTimerTypeBar);

	pNode->setMidpoint(ccp(0.5f, 0.5f));
	pNode->setBarChangeRate(ccp(1, 1));

	pNode->setPercentage(0);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------
// ProgressOutIn
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionProgressOutIn* FKCW_SceneExMgr_SceneExTransitionProgressOutIn::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionProgressOutIn* pScene = new FKCW_SceneExMgr_SceneExTransitionProgressOutIn();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCProgressTimer* FKCW_SceneExMgr_SceneExTransitionProgressOutIn::progressTimerNodeWithRenderTexture(CCRenderTexture* texture)
{    
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCProgressTimer* pNode = CCProgressTimer::create(texture->getSprite());

	pNode->getSprite()->setFlipY(true);
	pNode->setType( kCCProgressTimerTypeBar );

	pNode->setMidpoint(ccp(0.5f, 0.5f));
	pNode->setBarChangeRate(ccp(1, 1));

	pNode->setPercentage(100);
	pNode->setPosition(ccp(size.width/2, size.height/2));
	pNode->setAnchorPoint(ccp(0.5f,0.5f));

	return pNode;
}
//-------------------------------------------------------------------------