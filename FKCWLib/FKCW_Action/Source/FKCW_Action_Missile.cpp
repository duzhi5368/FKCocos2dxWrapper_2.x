//-------------------------------------------------------------------------
#include "../Include/FKCW_Action_Missile.h"
//-------------------------------------------------------------------------
// 创建一个导弹跟踪动作对象
// 参数：velocity 移动速度 aimed 目标对象 
//	targetPresetDegree 对象节点相对目标节点的旋转角度，正值表示顺时针
FKCW_Action_Missile* FKCW_Action_Missile::Create( float velocity, CCNode* aimed, float targetPresetDegree, CCCallFunc* doneCallFunc )
{
	FKCW_Action_Missile* p = new FKCW_Action_Missile();
	p->InitWithVelocity( velocity, aimed, targetPresetDegree, doneCallFunc );
	return ( FKCW_Action_Missile* )p->autorelease();
}
//-------------------------------------------------------------------------
FKCW_Action_Missile::FKCW_Action_Missile()
	: m_bIsDone( false )
	, m_pAimedNode( NULL )
	, m_pDoneCallFunc( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_Action_Missile::~FKCW_Action_Missile()
{
	CC_SAFE_RELEASE( m_pDoneCallFunc );
}
//-------------------------------------------------------------------------
bool FKCW_Action_Missile::InitWithVelocity( float velocity, CCNode* aimed, float targetPresetDegree, CCCallFunc* doneCallFunc )
{
	m_fDstDegree	= 0;
	m_fVelocity		= velocity;
	m_pAimedNode	= aimed;
	m_fPresetDegree	= targetPresetDegree;
	m_pDoneCallFunc	= doneCallFunc;
	CC_SAFE_RETAIN( m_pDoneCallFunc );
	return true;
}
//-------------------------------------------------------------------------
CCObject* FKCW_Action_Missile::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Missile* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		// 子类调用
		pCopy = (FKCW_Action_Missile*)(pZone->m_pCopyObject);
	} else {
		pCopy = new FKCW_Action_Missile();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->InitWithVelocity(m_fVelocity, m_pAimedNode, m_fPresetDegree, m_pDoneCallFunc);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//-------------------------------------------------------------------------
void FKCW_Action_Missile::step(float dt)
{
	// 动作节点和目标节点的位置
	CCNode* target = getTarget();
	CCPoint t = target->getPosition();
	CCPoint a = m_pAimedNode->getPosition();

	// 位置向量角度
	CCPoint v = ccpSub(a, t);
	float r = ccpToAngle(v);
	float d = -CC_RADIANS_TO_DEGREES(r);
	d -= m_fPresetDegree;

	// 保存目标角度
	if(m_fDstDegree != d) 
	{
		m_fDstDegree = d;
	}
	if(( m_fDstDegree < -90.0f && m_fDstDegree > -180.0f )
		||
		( m_fDstDegree > 90.0f && m_fDstDegree < 180.0f ))
	{
		CCSprite* p = dynamic_cast<CCSprite*>(target);
		if( p != NULL )
			p->setFlipY( true );
	}
	else
	{
		CCSprite* p = dynamic_cast<CCSprite*>(target);
		if( p != NULL )
			p->setFlipY( false );
	}

	// 逐渐旋转目标角度
	if(m_fDstDegree > target->getRotation()) 
	{
		float cur = MIN(m_fDstDegree, target->getRotation() + dt * 360);
		target->setRotation(cur);
	}
	else if(m_fDstDegree < target->getRotation()) 
	{
		float cur = MAX(m_fDstDegree, target->getRotation() - dt * 360);
		target->setRotation(cur);
	}

	// 通过速度计算目标移动
	float move = m_fVelocity * dt;
	float distance = ccpLength(v);
	if(move >= distance) 
	{
		target->setPosition(a);
	} 
	else 
	{
		t.x += move * cosf(r);
		t.y += move * sinf(r);
		target->setPosition(t);
	}

	// 是否结束
	m_bIsDone = getTarget()->getPositionX() == m_pAimedNode->getPositionX() &&
		getTarget()->getPositionY() == m_pAimedNode->getPositionY();

	// 结束回调
	if(m_bIsDone && m_pDoneCallFunc)
	{
		m_pDoneCallFunc->execute();
	}
}
//-------------------------------------------------------------------------