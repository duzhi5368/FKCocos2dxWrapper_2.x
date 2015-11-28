//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionFadeColor.h"
//-------------------------------------------------------------------------
const unsigned int kSceneFade = 0xFADEFADE;
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeColor::FKCW_SceneExMgr_SceneExTransitionFadeColor()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeColor::~FKCW_SceneExMgr_SceneExTransitionFadeColor()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeColor * FKCW_SceneExMgr_SceneExTransitionFadeColor::create(float duration, FKCW_SceneExMgr_SceneEx *scene, const ccColor3B& color)
{
	FKCW_SceneExMgr_SceneExTransitionFadeColor * pTransition = new FKCW_SceneExMgr_SceneExTransitionFadeColor();
	pTransition->initWithDuration(duration, scene, color);
	pTransition->autorelease();
	return pTransition;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFadeColor* FKCW_SceneExMgr_SceneExTransitionFadeColor::create(float duration,FKCW_SceneExMgr_SceneEx* scene)
{
	return FKCW_SceneExMgr_SceneExTransitionFadeColor::create(duration, scene, ccBLACK);
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExTransitionFadeColor::initWithDuration(float duration, FKCW_SceneExMgr_SceneEx *scene, const ccColor3B& color)
{
	if (FKCW_SceneExMgr_SceneExTransition::initWithDuration(duration, scene))
	{
		m_tColor.r = color.r;
		m_tColor.g = color.g;
		m_tColor.b = color.b;
		m_tColor.a = 0;
	}
	return true;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExTransitionFadeColor::initWithDuration(float t, FKCW_SceneExMgr_SceneEx *scene)
{
	this->initWithDuration(t, scene, ccBLACK);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFadeColor :: onEnter()
{
	FKCW_SceneExMgr_SceneExTransition::onEnter();

	CCLayerColor* l = CCLayerColor::create(m_tColor);
	m_pInScene->setVisible(false);

	addChild(l, 2, kSceneFade);
	CCNode* f = getChildByTag(kSceneFade);

	CCActionInterval* a = (CCActionInterval *)CCSequence::create
		(
		CCFadeIn::create(m_fDuration/2),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::hideOutShowIn)),
		CCFadeOut::create(m_fDuration/2),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		);
	f->runAction(a);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFadeColor::onExit()
{
	FKCW_SceneExMgr_SceneExTransition::onExit();
	this->removeChildByTag(kSceneFade, false);
}
//-------------------------------------------------------------------------
