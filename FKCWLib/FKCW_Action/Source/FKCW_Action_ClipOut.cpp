//-------------------------------------------------------------------------
#include "../Include/FKCW_Action_ClipOut.h"
#include "../../FKCW_Base/Include/FKCW_Base_ColorConvert.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
//-------------------------------------------------------------------------
FKCW_Action_ClipOut::FKCW_Action_ClipOut()
	: m_tagDirection( ccp(1,0) )
{

}
//-------------------------------------------------------------------------
FKCW_Action_ClipOut::~FKCW_Action_ClipOut()
{
}
//-------------------------------------------------------------------------
// 创建一个 ClipIn 动作实例
// 参数：p_fDuration 动作持续时间
//		p_tagV 该剪辑的移动方向矢量，默认情况下它是(1,0)，表示沿X轴移动
FKCW_Action_ClipOut* FKCW_Action_ClipOut::Create( float p_fDuration, CCPoint p_tagV )
{
	FKCW_Action_ClipOut* p = new FKCW_Action_ClipOut();
	p->InitWithDuration( p_fDuration, p_tagV );
	return ( FKCW_Action_ClipOut* )p->autorelease();
}
//-------------------------------------------------------------------------
bool FKCW_Action_ClipOut::InitWithDuration( float p_fDuration, CCPoint p_tagV )
{
	setDuration( p_fDuration );
	setDirection( p_tagV );
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Action_ClipOut::update(float time)
{
	CCClippingNode* clipper = (CCClippingNode*)getTarget();
	CCDrawNode* stencil = (CCDrawNode*)clipper->getStencil();

	// 清理深度模板
	stencil->clear();

	// 创建三角形
	float distance = m_fDistance * (1 - time);
	if(distance < FLT_EPSILON) 
	{
		CCPoint v[] = 
		{
			CCPointZero,
			CCPointZero,
			CCPointZero,
			CCPointZero
		};
		stencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
	} 
	else 
	{
		CCPoint v[] = 
		{
			m_tagP1,
			m_tagP2,
			ccp(m_tagP2.x + m_fDistance * m_fCos,
			m_tagP2.y + m_fDistance * m_fSin),
			ccp(m_tagP1.x + m_fDistance * m_fCos,
			m_tagP1.y + m_fDistance * m_fSin)
		};
		stencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
	}
}
//-------------------------------------------------------------------------
void FKCW_Action_ClipOut::startWithTarget(CCNode *pTarget)
{
	CCAssert(dynamic_cast<CCClippingNode*>(pTarget) != NULL, "CCClipOut target must be a CCClippingNode instance");

	CCActionInterval::startWithTarget(pTarget);

	// 保证深度clipper是CCDrawNode
	CCClippingNode* clipper = (CCClippingNode*)pTarget;
	CCNode* stencil = clipper->getStencil();
	if(dynamic_cast<CCDrawNode*>(stencil) == NULL) 
	{
		clipper->setStencil(CCDrawNode::create());
	}

	// 方向弧度
	float r = ccpToAngle(ccp(-m_tagDirection.x, -m_tagDirection.y));
	m_fCos = cosf(r);
	m_fSin = sinf(r);

	// 沿矢量方向最大距离
	const CCSize& size = pTarget->getContentSize();
	m_fDistance = size.width * fabsf(m_fCos) + size.height * fabsf(m_fSin);

	// 计算固定边顶点
	if(r > M_PI_2) 
	{
		float p1Len = size.width * cosf(r - M_PI_2);
		m_tagP1.x = size.width - p1Len * cosf(r - M_PI_2);
		m_tagP1.y = -p1Len * sinf(r - M_PI_2);
		float p2Len = size.height * sinf(r - M_PI_2);
		m_tagP2.x = size.width + p2Len * cosf(r - M_PI_2);
		m_tagP2.y = p2Len * sinf(r - M_PI_2);
	} 
	else if(r >= 0) 
	{
		float p1Len = size.height * m_fCos;
		m_tagP1.x = p1Len * cosf(r + M_PI_2);
		m_tagP1.y = p1Len * sinf(r + M_PI_2);
		float p2Len = size.width * m_fSin;
		m_tagP2.x = p2Len * cosf(r - M_PI_2);
		m_tagP2.y = p2Len * sinf(r - M_PI_2);
	} 
	else if(r < -M_PI_2) 
	{
		float p1Len = size.height * sinf(-r - M_PI_2);
		m_tagP1.x = size.width + p1Len * cosf(-r - M_PI_2);
		m_tagP1.y = size.height - p1Len * sinf(-r - M_PI_2);
		float p2Len = size.width * cosf(-r - M_PI_2);
		m_tagP2.x = size.width - p2Len * cosf(-r - M_PI_2);
		m_tagP2.y = size.height + p2Len * sinf(-r - M_PI_2);
	} 
	else 
	{
		float p1Len = size.width * sinf(-r);
		m_tagP1.x = p1Len * cosf(r + M_PI_2);
		m_tagP1.y = size.height + p1Len * sinf(r + M_PI_2);
		float p2Len = size.height * cosf(-r);
		m_tagP2.x = -p2Len * cosf(r + M_PI_2);
		m_tagP2.y = size.height - p2Len * sinf(r + M_PI_2);
	}
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_Action_ClipOut::reverse()
{
	return FKCW_Action_ClipOut::Create(getDuration(), ccp(-m_tagDirection.x, -m_tagDirection.y));
}
//-------------------------------------------------------------------------