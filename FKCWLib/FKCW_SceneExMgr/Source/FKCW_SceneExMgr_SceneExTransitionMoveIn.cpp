//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionMoveIn.h"
//-------------------------------------------------------------------------
// MoveInL
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInL::FKCW_SceneExMgr_SceneExTransitionMoveInL()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInL::~FKCW_SceneExMgr_SceneExTransitionMoveInL()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInL* FKCW_SceneExMgr_SceneExTransitionMoveInL::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionMoveInL* pScene = new FKCW_SceneExMgr_SceneExTransitionMoveInL();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionMoveInL::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	this->initScenes();

	CCActionInterval *a = this->action();

	m_pInScene->runAction
		(
		CCSequence::create
		(
		this->easeActionWithAction(a),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		)
		);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionMoveInL::action()
{
	return CCMoveTo::create(m_fDuration, ccp(0,0));
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionMoveInL::easeActionWithAction(CCActionInterval* action)
{
	return CCEaseOut::create(action, 2.0f);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionMoveInL::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition(ccp(-s.width,0));
}
//-------------------------------------------------------------------------
// MoveInR
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInR::FKCW_SceneExMgr_SceneExTransitionMoveInR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInR::~FKCW_SceneExMgr_SceneExTransitionMoveInR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInR* FKCW_SceneExMgr_SceneExTransitionMoveInR::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionMoveInR* pScene = new FKCW_SceneExMgr_SceneExTransitionMoveInR();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionMoveInR::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(s.width,0) );
}
//-------------------------------------------------------------------------
// MoveInT
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInT::FKCW_SceneExMgr_SceneExTransitionMoveInT()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInT::~FKCW_SceneExMgr_SceneExTransitionMoveInT()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInT* FKCW_SceneExMgr_SceneExTransitionMoveInT::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionMoveInT* pScene = new FKCW_SceneExMgr_SceneExTransitionMoveInT();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionMoveInT::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(0,s.height) );
}
//-------------------------------------------------------------------------
// MoveInB
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInB::FKCW_SceneExMgr_SceneExTransitionMoveInB()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInB::~FKCW_SceneExMgr_SceneExTransitionMoveInB()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionMoveInB* FKCW_SceneExMgr_SceneExTransitionMoveInB::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionMoveInB* pScene = new FKCW_SceneExMgr_SceneExTransitionMoveInB();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionMoveInB::initScenes()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_pInScene->setPosition( ccp(0,-s.height) );
}
//-------------------------------------------------------------------------