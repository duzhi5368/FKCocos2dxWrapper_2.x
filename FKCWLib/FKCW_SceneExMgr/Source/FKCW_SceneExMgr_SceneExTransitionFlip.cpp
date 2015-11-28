//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionFlip.h"
//-------------------------------------------------------------------------
// FlipX
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipX::FKCW_SceneExMgr_SceneExTransitionFlipX()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipX::~FKCW_SceneExMgr_SceneExTransitionFlipX()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFlipX::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationRightOver )
	{
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	}
	else
	{
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}

	inA = (CCActionInterval*)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCShow::create(),
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		NULL
		);

	outA = (CCActionInterval *)CCSequence::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),                            
		NULL 
		);

	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipX* FKCW_SceneExMgr_SceneExTransitionFlipX::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionFlipX* pScene = new FKCW_SceneExMgr_SceneExTransitionFlipX();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipX* FKCW_SceneExMgr_SceneExTransitionFlipX::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionFlipX::create(t, s, kCCTransitionOrientationRightOver);
}
//-------------------------------------------------------------------------
// FlipY Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipY::FKCW_SceneExMgr_SceneExTransitionFlipY()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipY::~FKCW_SceneExMgr_SceneExTransitionFlipY()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFlipY::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationUpOver ) 
	{
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	} 
	else 
	{
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}

	inA = (CCActionInterval*)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCShow::create(),
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		NULL
		);
	outA = (CCActionInterval*)CCSequence::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0),
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),                            
		NULL
		);

	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipY* FKCW_SceneExMgr_SceneExTransitionFlipY::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionFlipY* pScene = new FKCW_SceneExMgr_SceneExTransitionFlipY();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipY* FKCW_SceneExMgr_SceneExTransitionFlipY::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionFlipY::create(t, s, kCCTransitionOrientationUpOver);
}
//-------------------------------------------------------------------------
// FlipAngular Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipAngular::FKCW_SceneExMgr_SceneExTransitionFlipAngular()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipAngular::~FKCW_SceneExMgr_SceneExTransitionFlipAngular()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionFlipAngular::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationRightOver ) 
	{
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	} 
	else 
	{
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}

	inA = (CCActionInterval *)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCShow::create(),
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)), 
		NULL
		);
	outA = (CCActionInterval *)CCSequence::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 45, 0),
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),                            
		NULL
		);

	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipAngular* FKCW_SceneExMgr_SceneExTransitionFlipAngular::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionFlipAngular* pScene = new FKCW_SceneExMgr_SceneExTransitionFlipAngular();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionFlipAngular* FKCW_SceneExMgr_SceneExTransitionFlipAngular::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
}
//-------------------------------------------------------------------------
// ZoomFlipX Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipX::FKCW_SceneExMgr_SceneExTransitionZoomFlipX()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipX::~FKCW_SceneExMgr_SceneExTransitionZoomFlipX()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionZoomFlipX::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationRightOver ) {
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	} 
	else 
	{
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}
	inA = (CCActionInterval *)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),
		CCScaleTo::create(m_fDuration/2, 1),
		CCShow::create(),
		NULL
		),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		);
	outA = (CCActionInterval *)CCSequence::create
		(
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
		CCScaleTo::create(m_fDuration/2, 0.5f),
		NULL
		),
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),                            
		NULL
		);

	m_pInScene->setScale(0.5f);
	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipX* FKCW_SceneExMgr_SceneExTransitionZoomFlipX::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionZoomFlipX* pScene = new FKCW_SceneExMgr_SceneExTransitionZoomFlipX();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipX* FKCW_SceneExMgr_SceneExTransitionZoomFlipX::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionZoomFlipX::create(t, s, kCCTransitionOrientationRightOver);
}
//-------------------------------------------------------------------------
// ZoomFlipY Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipY::FKCW_SceneExMgr_SceneExTransitionZoomFlipY()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipY::~FKCW_SceneExMgr_SceneExTransitionZoomFlipY()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionZoomFlipY::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation== kCCTransitionOrientationUpOver ) {
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	} else {
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}

	inA = (CCActionInterval *)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0),
		CCScaleTo::create(m_fDuration/2, 1),
		CCShow::create(),
		NULL
		),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		);

	outA = (CCActionInterval *)CCSequence::create
		(
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0),
		CCScaleTo::create(m_fDuration/2, 0.5f),
		NULL
		),                            
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),
		NULL
		);

	m_pInScene->setScale(0.5f);
	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipY* FKCW_SceneExMgr_SceneExTransitionZoomFlipY::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionZoomFlipY* pScene = new FKCW_SceneExMgr_SceneExTransitionZoomFlipY();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipY* FKCW_SceneExMgr_SceneExTransitionZoomFlipY::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionZoomFlipY::create(t, s, kCCTransitionOrientationUpOver);
}
//-------------------------------------------------------------------------
// ZoomFlipAngular Transition
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular()
{
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::~FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular()
{
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::onEnter()
{
	FKCW_SceneExMgr_SceneExTransitionOriented::onEnter();

	CCActionInterval *inA, *outA;
	m_pInScene->setVisible(false);

	float inDeltaZ, inAngleZ;
	float outDeltaZ, outAngleZ;

	if( m_eOrientation == kCCTransitionOrientationRightOver ) {
		inDeltaZ = 90;
		inAngleZ = 270;
		outDeltaZ = 90;
		outAngleZ = 0;
	} 
	else 
	{
		inDeltaZ = -90;
		inAngleZ = 90;
		outDeltaZ = -90;
		outAngleZ = 0;
	}

	inA = (CCActionInterval *)CCSequence::create
		(
		CCDelayTime::create(m_fDuration/2),
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0),
		CCScaleTo::create(m_fDuration/2, 1),
		CCShow::create(),
		NULL
		),
		CCShow::create(),
		CCCallFunc::create(this, callfunc_selector(FKCW_SceneExMgr_SceneExTransition::finish)),
		NULL
		);
	outA = (CCActionInterval *)CCSequence::create
		(
		CCSpawn::create
		(
		CCOrbitCamera::create(m_fDuration/2, 1, 0 , outAngleZ, outDeltaZ, 45, 0),
		CCScaleTo::create(m_fDuration/2, 0.5f),
		NULL
		),                            
		CCHide::create(),
		CCDelayTime::create(m_fDuration/2),                            
		NULL
		);

	m_pInScene->setScale(0.5f);
	m_pInScene->runAction(inA);
	m_pOutScene->runAction(outA);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular* FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::create(float t, FKCW_SceneExMgr_SceneEx* s, tOrientation o)
{
	FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular* pScene = new FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular();
	pScene->initWithDuration(t, s, o);
	pScene->autorelease();

	return pScene;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular* FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::create(float t, FKCW_SceneExMgr_SceneEx* s)
{
	return FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
}
//-------------------------------------------------------------------------