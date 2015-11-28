//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionShrinkGrow.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionShrinkGrow::FKCW_SceneExMgr_SceneExTransitionShrinkGrow()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionShrinkGrow::~FKCW_SceneExMgr_SceneExTransitionShrinkGrow()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionShrinkGrow* FKCW_SceneExMgr_SceneExTransitionShrinkGrow::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionShrinkGrow* pScene = new FKCW_SceneExMgr_SceneExTransitionShrinkGrow();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionShrinkGrow::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();

	m_pInScene->setScale(0.001f);
	m_pOutScene->setScale(1.0f); 

	CCActionInterval* scaleOut = CCScaleTo::create(m_fDuration, 0.01f);
	CCActionInterval* scaleIn = CCScaleTo::create(m_fDuration, 1.0f);

	m_pInScene->runAction(this->easeActionWithAction(scaleIn));
	m_pOutScene->runAction
		(
		CCSequence::create
		(
		this->easeActionWithAction(scaleOut),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		NULL
		)
		);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionShrinkGrow:: easeActionWithAction(CCActionInterval* action)
{
	return CCEaseOut::create(action, 1.0f);
}
//-------------------------------------------------------------------------