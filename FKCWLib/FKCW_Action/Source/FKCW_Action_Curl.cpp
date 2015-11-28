//--------------------------------------------------------------------
#include "../Include/FKCW_Action_Curl.h"
//--------------------------------------------------------------------
FKCW_Action_Curl::FKCW_Action_Curl() 
{
}
//--------------------------------------------------------------------
FKCW_Action_Curl::~FKCW_Action_Curl() 
{
}
//--------------------------------------------------------------------
FKCW_Action_Curl* FKCW_Action_Curl::Create(float duration, CCPoint center, float toRadius, float angularVelocity) 
{
	FKCW_Action_Curl* a = new FKCW_Action_Curl();
	if(a->initWithDuration(duration, center, toRadius, angularVelocity)) 
	{
		a->autorelease();
		return a;
	}
	CC_SAFE_RELEASE(a);
	return NULL;
}
//--------------------------------------------------------------------
bool FKCW_Action_Curl::initWithDuration(float d, CCPoint center, float toRadius, float angularVelocity) 
{
	setDuration(d);
	m_center = center;
	m_toRadius = toRadius;
	m_angularVelocity = angularVelocity;

	return true;
}
//--------------------------------------------------------------------
void FKCW_Action_Curl::startWithTarget(CCNode* pTarget) 
{
	CCActionInterval::startWithTarget(pTarget);

	// 获取起始半径和角度
	CCPoint v = ccpSub(pTarget->getPosition(), m_center);
	m_fromRadius = ccpLength(v);
	m_initAngle = v.getAngle();
}
//--------------------------------------------------------------------
void FKCW_Action_Curl::update(float time) 
{
	// 半径和角度
	float radius = m_fromRadius + time * (m_toRadius - m_fromRadius);
	float angle = m_initAngle + time * m_fDuration * m_angularVelocity;

	// 设置目标点
	CCPoint v = ccpForAngle(angle) * radius;
	CCPoint p = ccpAdd(m_center, v);
	getTarget()->setPosition(p);
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_Curl::copyWithZone(CCZone *pZone) 
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Curl* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		pCopy = (FKCW_Action_Curl*)(pZone->m_pCopyObject);
	} 
	else 
	{
		pCopy = new FKCW_Action_Curl();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->initWithDuration(m_fDuration, m_center, m_toRadius, m_angularVelocity);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
CCActionInterval* FKCW_Action_Curl::reverse() 
{
	return FKCW_Action_Curl::Create(getDuration(), m_center, m_fromRadius, -m_angularVelocity);
}
//--------------------------------------------------------------------