//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionSlideIn.h"
//-------------------------------------------------------------------------
#ifndef ADJUST_FACTOR
#define ADJUST_FACTOR 0.5f
#endif
//-------------------------------------------------------------------------
// SlideInL
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInL::FKCW_SceneExMgr_SceneExTransitionSlideInL()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInL::~FKCW_SceneExMgr_SceneExTransitionSlideInL()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInL::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	this->initScenes();

	CCActionInterval *in = this->action();
	CCActionInterval *out = this->action();

	CCActionInterval* inAction = easeActionWithAction(in);
	CCActionInterval* outAction = (CCActionInterval*)CCSequence::create
		(
		easeActionWithAction(out),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		NULL
		);
	m_pInScene->runAction(inAction);
	m_pOutScene->runAction(outAction);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInL::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInL:: initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(-(s.width-ADJUST_FACTOR),0) );
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSlideInL::action()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	return CCMoveBy::create(m_fDuration, ccp(s.width-ADJUST_FACTOR,0));
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSlideInL::easeActionWithAction(CCActionInterval* action)
{
	return CCEaseOut::create(action, 2.0f);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInL* FKCW_SceneExMgr_SceneExTransitionSlideInL::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSlideInL* pScene = new FKCW_SceneExMgr_SceneExTransitionSlideInL();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
// SlideInR
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInR::FKCW_SceneExMgr_SceneExTransitionSlideInR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInR::~FKCW_SceneExMgr_SceneExTransitionSlideInR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInR* FKCW_SceneExMgr_SceneExTransitionSlideInR::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSlideInR* pScene = new FKCW_SceneExMgr_SceneExTransitionSlideInR();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInR::sceneOrder()
{
	m_bIsInSceneOnTop = true;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInR::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(s.width-ADJUST_FACTOR,0) );
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSlideInR:: action()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	return CCMoveBy::create(m_fDuration, ccp(-(s.width-ADJUST_FACTOR),0));
}
//-------------------------------------------------------------------------
// SlideInT
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInT::FKCW_SceneExMgr_SceneExTransitionSlideInT()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInT::~FKCW_SceneExMgr_SceneExTransitionSlideInT()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInT* FKCW_SceneExMgr_SceneExTransitionSlideInT::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSlideInT* pScene = new FKCW_SceneExMgr_SceneExTransitionSlideInT();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInT::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInT::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(0,s.height-ADJUST_FACTOR) );
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSlideInT::action()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	return CCMoveBy::create(m_fDuration, ccp(0,-(s.height-ADJUST_FACTOR)));
}
//-------------------------------------------------------------------------
// SlideInB
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInB::FKCW_SceneExMgr_SceneExTransitionSlideInB()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInB::~FKCW_SceneExMgr_SceneExTransitionSlideInB()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSlideInB* FKCW_SceneExMgr_SceneExTransitionSlideInB::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSlideInB* pScene = new FKCW_SceneExMgr_SceneExTransitionSlideInB();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInB::sceneOrder()
{
	m_bIsInSceneOnTop = true;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSlideInB:: initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(0,-(s.height-ADJUST_FACTOR)) );
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSlideInB:: action()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	return CCMoveBy::create(m_fDuration, ccp(0,s.height-ADJUST_FACTOR));
}
//-------------------------------------------------------------------------