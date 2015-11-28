//-------------------------------------------------------------------------
#include "../Include/FKCW_GR_GestureRecognizer.h"
//-------------------------------------------------------------------------
FKCW_GR_GestureRecognizer::FKCW_GR_GestureRecognizer()
{
	m_bIsRecognizing = false;
	m_pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	setTouchEnabled( true );
	setCancelsTouchesInView( false );
}
//-------------------------------------------------------------------------
FKCW_GR_GestureRecognizer::~FKCW_GR_GestureRecognizer()
{
	m_pDispatcher->removeDelegate(this);
}
//-------------------------------------------------------------------------
void FKCW_GR_GestureRecognizer::SetTarget( CCObject* tar, SEL_CallFuncO sel )
{
	m_pTarget = tar;
	m_pSelector = sel;
}
//-------------------------------------------------------------------------
void FKCW_GR_GestureRecognizer::setParent(CCNode* parent)
{
	CCLayer::setParent(parent);

	if ( NULL != parent ) 
	{
		CCSize size = parent->getContentSize();
		setContentSize(size);
		setPosition(parent->getPosition());
		m_tagFrame = parent->boundingBox();
	}
}
//-------------------------------------------------------------------------
void FKCW_GR_GestureRecognizer::registerWithTouchDispatcher(void)
{
	m_pDispatcher->addTargetedDelegate(this, -256, false);
}
//-------------------------------------------------------------------------
void FKCW_GR_GestureRecognizer::_GestureRecognized(CCObject * gesture)
{
	if (m_pTarget && m_pSelector) 
		(m_pTarget->*m_pSelector)(gesture);
}
//-------------------------------------------------------------------------
void FKCW_GR_GestureRecognizer::_StopTouchesPropagation(CCSet * pTouches, CCEvent * pEvent)
{
	// 取消touch，以至于touch消息不会被分发
	m_pDispatcher->touchesCancelled(pTouches, pEvent);
}
//-------------------------------------------------------------------------
bool FKCW_GR_GestureRecognizer::_IsPositionBetweenBounds(CCPoint pos)
{
	return m_tagFrame.containsPoint(pos);
}
//-------------------------------------------------------------------------
float FKCW_GR_GestureRecognizer::_DistanceBetweenPoints(CCPoint p1, CCPoint p2)
{
	float deltaX = p2.x-p1.x;
	float deltaY = p2.y-p1.y;
	return fabs(sqrtf(deltaX*deltaX+deltaY*deltaY));
}
//-------------------------------------------------------------------------
CCSet* FKCW_GR_GestureRecognizer::_CreateSetWithTouch(CCTouch * pTouch)
{
	CCSet * set = new CCSet();
	set->addObject(pTouch);
	return set;
}
//-------------------------------------------------------------------------