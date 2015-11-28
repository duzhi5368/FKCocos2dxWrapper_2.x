//--------------------------------------------------------------------
#include "../Include/FKCW_Action_Shake.h"
//--------------------------------------------------------------------
FKCW_Action_Shake::FKCW_Action_Shake()
{

}
//--------------------------------------------------------------------
FKCW_Action_Shake::~FKCW_Action_Shake()
{

}
//--------------------------------------------------------------------
FKCW_Action_Shake* FKCW_Action_Shake::Create( float duration, float maxRadius, float minRadius )
{
	FKCW_Action_Shake* a = new FKCW_Action_Shake();
	a->InitWithRing(duration, maxRadius, minRadius);
	return (FKCW_Action_Shake*)a->autorelease();
}
//--------------------------------------------------------------------
FKCW_Action_Shake* FKCW_Action_Shake::Create( float duration, float startMaxRadius, float endMaxRadius, float startMinRadius, float endMinRadius )
{
	FKCW_Action_Shake* a = new FKCW_Action_Shake();
	a->InitWithDynamicRing(duration, startMaxRadius, endMaxRadius, startMinRadius, endMinRadius);
	return (FKCW_Action_Shake*)a->autorelease();
}
//--------------------------------------------------------------------
bool FKCW_Action_Shake::InitWithRing( float duration, float maxRadius, float minRadius )
{
	m_eMode			= eShakeMode_Ring;
	m_fDuration		= duration;
	m_fMaxRadius	= maxRadius;
	m_fMinRadius	= minRadius;
	m_fEndMaxRadius = maxRadius;
	m_fEndMinRadius = minRadius;
	return true;
}
//--------------------------------------------------------------------
bool FKCW_Action_Shake::InitWithDynamicRing( float duration, float startMaxRadius, float endMaxRadius, float startMinRadius, float endMinRadius )
{
	m_eMode			= eShakeMode_DynamicRing;
	m_fDuration		= duration;
	m_fMaxRadius	= startMaxRadius;
	m_fMinRadius	= startMinRadius;
	m_fEndMaxRadius = endMaxRadius;
	m_fEndMinRadius = endMinRadius;
	return true;
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_Shake::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Shake* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		pCopy = (FKCW_Action_Shake*)(pZone->m_pCopyObject);
	} 
	else
	{
		pCopy = new FKCW_Action_Shake();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	switch (m_eMode) 
	{
	case eShakeMode_Ring:
		pCopy->InitWithDynamicRing(m_fDuration, m_fMaxRadius, m_fEndMaxRadius, m_fMinRadius, m_fEndMinRadius);
		break;
	default:
		pCopy->InitWithRing(m_fDuration, m_fMaxRadius, m_fMinRadius);
		break;
	}

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
CCActionInterval* FKCW_Action_Shake::reverse()
{
	switch (m_eMode) 
	{
	case eShakeMode_DynamicRing:
		return Create(m_fDuration, m_fMaxRadius, m_fEndMaxRadius, m_fMinRadius, m_fEndMinRadius);
	default:
		return Create(m_fDuration, m_fMaxRadius, m_fMinRadius);
	}
}
//--------------------------------------------------------------------
void FKCW_Action_Shake::update(float t)
{
	if(t >= 1)
	{
		getTarget()->setPosition(m_fOriginalX, m_fOriginalY);
	} 
	else 
	{
		switch (m_eMode)
		{
		case eShakeMode_Ring:
		case eShakeMode_DynamicRing:
			{
				float radian = static_cast<float>(CCRANDOM_0_1() * M_PI * 2);
				float maxR = m_fMaxRadius + (m_fEndMinRadius - m_fMaxRadius) * t;
				float minR = m_fMinRadius + (m_fEndMinRadius - m_fMinRadius) * t;
				float r = CCRANDOM_0_1() * (maxR - minR) + minR;
				float x = m_fOriginalX + cosf(radian) * r;
				float y = m_fOriginalY + sinf(radian) * r;
				getTarget()->setPosition(ccp(x, y));
				break;
			}
		default:
			break;
		}
	}
}
//--------------------------------------------------------------------
void FKCW_Action_Shake::startWithTarget(CCNode* pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	m_fOriginalX = pTarget->getPositionX();
	m_fOriginalY = pTarget->getPositionY();
}
//--------------------------------------------------------------------