//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionJumpZoom.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionJumpZoom::FKCW_SceneExMgr_SceneExTransitionJumpZoom()
{

}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionJumpZoom::~FKCW_SceneExMgr_SceneExTransitionJumpZoom()
{

}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionJumpZoom::onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_pInScene->setScale(0.5f);
	m_pInScene->setPosition(ccp(s.width, 0));
	m_pInScene->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pOutScene->setAnchorPoint(ccp(0.5f, 0.5f));

	CCActionInterval *jump = CCJumpBy::create(m_fDuration/4, ccp(-s.width,0), s.width/4, 2);
	CCActionInterval *scaleIn = CCScaleTo::create(m_fDuration/4, 1.0f);
	CCActionInterval *scaleOut = CCScaleTo::create(m_fDuration/4, 0.5f);

	CCActionInterval *jumpZoomOut = (CCActionInterval*)(CCSequence::create(scaleOut, jump, NULL));
	CCActionInterval *jumpZoomIn = (CCActionInterval*)(CCSequence::create(jump, scaleIn, NULL));

	CCActionInterval *delay = CCDelayTime::create(m_fDuration/2);

	m_pOutScene->runAction(jumpZoomOut);
	m_pInScene->runAction
		(
		CCSequence::create
		(
		delay,
		jumpZoomIn,
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		)
		);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionJumpZoom* FKCW_SceneExMgr_SceneExTransitionJumpZoom::create(float t, FKCW_SceneExMgr_SceneEx* scene)
{
	FKCW_SceneExMgr_SceneExTransitionJumpZoom* pScene = new FKCW_SceneExMgr_SceneExTransitionJumpZoom();
	if(pScene && pScene->initWithDuration(t, scene))
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE(pScene);
	return NULL;
}
//-------------------------------------------------------------------------