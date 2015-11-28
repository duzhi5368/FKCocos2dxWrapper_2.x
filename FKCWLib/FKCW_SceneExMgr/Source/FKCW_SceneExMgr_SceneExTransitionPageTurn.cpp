//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionPageTurn.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionPageTurn::FKCW_SceneExMgr_SceneExTransitionPageTurn()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionPageTurn::~FKCW_SceneExMgr_SceneExTransitionPageTurn()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionPageTurn * FKCW_SceneExMgr_SceneExTransitionPageTurn::create(float t, FKCW_SceneExMgr_SceneEx *scene, bool backwards)
{
	FKCW_SceneExMgr_SceneExTransitionPageTurn * pTransition = new FKCW_SceneExMgr_SceneExTransitionPageTurn();
	pTransition->initWithDuration(t,scene,backwards);
	pTransition->autorelease();
	return pTransition;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExTransitionPageTurn::initWithDuration(float t, FKCW_SceneExMgr_SceneEx *scene, bool backwards)
{
	m_bBack = backwards;

	if (FKCW_SceneExMgr_SceneExTransition::initWithDuration(t, scene))
	{
	}
	return true;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionPageTurn::sceneOrder()
{
	m_bIsInSceneOnTop = m_bBack;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionPageTurn::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	int x,y;
	if (s.width > s.height)
	{
		x=16;
		y=12;
	}
	else
	{
		x=12;
		y=16;
	}

	CCActionInterval *action  = this->actionWithSize(CCSizeMake(x,y));

	if (! m_bBack )
	{
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
	else
	{
		m_pInScene->setVisible(false);
		m_pInScene->runAction
			(
			CCSequence::create
			(
			CCShow::create(),
			action,
			CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
			CCStopGrid::create(),
			NULL
			)
			);
	}
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_SceneExMgr_SceneExTransitionPageTurn:: actionWithSize(const CCSize& vector)
{
	if (m_bBack)
	{
		return CCReverseTime::create(CCPageTurn3D::create(m_fDuration, vector));
	}
	else
	{
		return CCPageTurn3D::create(m_fDuration, vector);
	}
}
//-------------------------------------------------------------------------