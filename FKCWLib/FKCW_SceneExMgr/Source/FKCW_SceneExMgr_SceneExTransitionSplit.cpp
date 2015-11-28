//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionSplit.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitCols::FKCW_SceneExMgr_SceneExTransitionSplitCols()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitCols::~FKCW_SceneExMgr_SceneExTransitionSplitCols()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitCols* FKCW_SceneExMgr_SceneExTransitionSplitCols::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSplitCols* pScene = new FKCW_SceneExMgr_SceneExTransitionSplitCols();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionSplitCols::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	m_pInScene->setVisible(false);

	CCActionInterval* split = action();
	CCActionInterval* seq = (CCActionInterval*)CCSequence::create
		(
		split,
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::hideOutShowIn)),
		split->reverse(),
		NULL
		);

	this->runAction
		( 
		CCSequence::create
		(
		easeActionWithAction(seq),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		CCStopGrid::create(),
		NULL
		)
		);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSplitCols:: action()
{
	return CCSplitCols::create(m_fDuration/2.0f, 3);
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSplitCols::easeActionWithAction(CCActionInterval * action)
{
	return CCEaseInOut::create(action, 3.0f);
}
//-------------------------------------------------------------------------
// SplitRows Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitRows::FKCW_SceneExMgr_SceneExTransitionSplitRows()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitRows::~FKCW_SceneExMgr_SceneExTransitionSplitRows()
{
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionSplitRows::action()
{
	return CCSplitRows::create(m_fDuration/2.0f, 3);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionSplitRows* FKCW_SceneExMgr_SceneExTransitionSplitRows::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionSplitRows* pScene = new FKCW_SceneExMgr_SceneExTransitionSplitRows();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------