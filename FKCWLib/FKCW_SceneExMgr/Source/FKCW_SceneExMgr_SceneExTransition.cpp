//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransition.h"
#include "../Include/FKCW_SceneExMgr_SceneExMgr.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransition::FKCW_SceneExMgr_SceneExTransition()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransition::~FKCW_SceneExMgr_SceneExTransition()
{
	m_pInScene->release();
	m_pOutScene->release();
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransition * FKCW_SceneExMgr_SceneExTransition::create(float t, FKCW_SceneExMgr_SceneEx *scene)
{
	FKCW_SceneExMgr_SceneExTransition * pScene = new FKCW_SceneExMgr_SceneExTransition();
	if(pScene && pScene->initWithDuration(t,scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExTransition::initWithDuration(float t, FKCW_SceneExMgr_SceneEx *scene)
{
	CCAssert( scene != NULL, "Argument scene must be non-nil");

	if (FKCW_SceneExMgr_SceneEx::init())
	{
		m_fDuration = t;
		m_pInScene = scene;
		m_pInScene->retain();
		m_pOutScene = FKCW_SceneExMgr_SceneExMgr::sharedManager()->getRunningScene();
		if (m_pOutScene == NULL)
		{
			m_pOutScene = new FKCW_SceneExMgr_SceneEx();
			m_pOutScene->init();
		}
		m_pOutScene->retain();

		CCAssert( m_pInScene != m_pOutScene, "Incoming scene must be different from the outgoing scene" );

		sceneOrder();

		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::sceneOrder()
{
	m_bIsInSceneOnTop = true;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::draw()
{
	FKCW_SceneExMgr_SceneEx::draw();

	if( m_bIsInSceneOnTop ) {
		m_pOutScene->visit();
		m_pInScene->visit();
	} else {
		m_pInScene->visit();
		m_pOutScene->visit();
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::finish()
{  
	m_pInScene->setVisible(true);
	m_pInScene->setAnchorPoint(ccp(0, 0));
	m_pInScene->setPosition(ccp(0, 0));
	m_pInScene->setScale(1.0f);
	m_pInScene->setRotation(0.0f);
	m_pInScene->getCamera()->restore();

	m_pOutScene->setVisible(false);
	m_pInScene->setAnchorPoint(ccp(0, 0));
	m_pOutScene->setPosition(ccp(0,0));
	m_pOutScene->setScale(1.0f);
	m_pOutScene->setRotation(0.0f);
	m_pOutScene->getCamera()->restore();

	this->schedule(schedule_selector(FKCW_SceneExMgr_SceneExTransition::setNewScene), 0);

}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::setNewScene(float dt)
{    
	CC_UNUSED_PARAM(dt);

	this->unschedule(schedule_selector(FKCW_SceneExMgr_SceneExTransition::setNewScene));

	// 替换之前，先保存清除状态
	FKCW_SceneExMgr_SceneExMgr* manager = FKCW_SceneExMgr_SceneExMgr::sharedManager();
	m_bIsSendCleanupToScene = manager->isSendCleanupToScene();

	manager->replaceScene(m_pInScene);

	m_pOutScene->setVisible(true);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::hideOutShowIn()
{
	m_pInScene->setVisible(true);
	m_pOutScene->setVisible(false);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::onEnter()
{
	FKCW_SceneExMgr_SceneEx::onEnter();

	// 过渡时关闭消息处理
	CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);

	// 进行双向通知
	m_pOutScene->onExitTransitionDidStart();
	m_pInScene->onEnter();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::onExit()
{
	FKCW_SceneExMgr_SceneEx::onExit();

	// 过渡关闭开启消息处理
	CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);

	// 进行双向通知
	m_pOutScene->onExit();
	m_pInScene->onEnterTransitionDidFinish();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransition::cleanup()
{
	FKCW_SceneExMgr_SceneEx::cleanup();

	if( m_bIsSendCleanupToScene )
		m_pOutScene->cleanup();
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx* FKCW_SceneExMgr_SceneExTransition::getInScene()
{
	return m_pInScene;
}
//-------------------------------------------------------------------------