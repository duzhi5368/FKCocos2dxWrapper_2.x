//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionTurnOffTiles.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::FKCW_SceneExMgr_SceneExTransitionTurnOffTiles()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::~FKCW_SceneExMgr_SceneExTransitionTurnOffTiles()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionTurnOffTiles* FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionTurnOffTiles* pScene = new FKCW_SceneExMgr_SceneExTransitionTurnOffTiles();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::sceneOrder()
{
	m_bIsInSceneOnTop = false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionTurnOffTiles::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	float aspect = s.width / s.height;
	int x = (int)(12 * aspect);
	int y = 12;

	CCTurnOffTiles* toff = CCTurnOffTiles::create(m_fDuration, CCSizeMake(x,y));
	CCActionInterval* action = easeActionWithAction(toff);
	m_pOutScene->runAction
		(
		CCSequence::create
		(
		action,
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		CCStopGrid::create(),
		NULL
		)
		);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionTurnOffTiles:: easeActionWithAction(CCActionInterval* action)
{
	return action;
}
//-------------------------------------------------------------------------