//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionFadeDir.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeTR::FKCW_SceneExMgr_SceneExTransitionFadeTR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeTR::~FKCW_SceneExMgr_SceneExTransitionFadeTR()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeTR* FKCW_SceneExMgr_SceneExTransitionFadeTR::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionFadeTR* pScene = new FKCW_SceneExMgr_SceneExTransitionFadeTR();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFadeTR::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFadeTR::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();

	CCSize s = CCDirector::sharedDirector()->getWinSize();
	float aspect = s.width / s.height;
	int x = (int)(12 * aspect);
	int y = 12;

	CCActionInterval* action  = actionWithSize(CCSizeMake(x,y));

	m_pOutScene->runAction
		(
		CCSequence::create
		(
		easeActionWithAction(action),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		CCStopGrid::create(),
		NULL
		)
		);
}
//-------------------------------------------------------------------------
CCActionInterval*  FKCW_SceneExMgr_SceneExTransitionFadeTR::actionWithSize(const CCSize& size)
{
	return CCFadeOutTRTiles::create(m_fDuration, size);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionFadeTR:: easeActionWithAction(CCActionInterval* action)
{
	return action;
}
//-------------------------------------------------------------------------
// FadeBL Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeBL::FKCW_SceneExMgr_SceneExTransitionFadeBL()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeBL::~FKCW_SceneExMgr_SceneExTransitionFadeBL()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeBL* FKCW_SceneExMgr_SceneExTransitionFadeBL::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionFadeBL* pScene = new FKCW_SceneExMgr_SceneExTransitionFadeBL();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCActionInterval*  FKCW_SceneExMgr_SceneExTransitionFadeBL::actionWithSize(const CCSize& size)
{
	return CCFadeOutBLTiles::create(m_fDuration, size);
}
//-------------------------------------------------------------------------
// FadeUp Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeUp::FKCW_SceneExMgr_SceneExTransitionFadeUp()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeUp::~FKCW_SceneExMgr_SceneExTransitionFadeUp()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeUp* FKCW_SceneExMgr_SceneExTransitionFadeUp::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionFadeUp* pScene = new FKCW_SceneExMgr_SceneExTransitionFadeUp();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionFadeUp::actionWithSize(const CCSize& size)
{
	return CCFadeOutUpTiles::create(m_fDuration, size);
}
//-------------------------------------------------------------------------
// FadeDown Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeDown::FKCW_SceneExMgr_SceneExTransitionFadeDown()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeDown::~FKCW_SceneExMgr_SceneExTransitionFadeDown()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeDown* FKCW_SceneExMgr_SceneExTransitionFadeDown::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionFadeDown* pScene = new FKCW_SceneExMgr_SceneExTransitionFadeDown();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionFadeDown::actionWithSize(const CCSize& size)
{
	return CCFadeOutDownTiles::create(m_fDuration, size);
}
//-------------------------------------------------------------------------