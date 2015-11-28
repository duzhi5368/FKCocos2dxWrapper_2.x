//-------------------------------------------------------------------------
#include "../Include/FKCW_GR_Pinch.h"
//-------------------------------------------------------------------------
bool FKCW_GR_PinchGestureRecognizer::init()
{
	m_nTouchNumber		= 0;
	m_fLastDistance		= 0;
	m_pTouches			= CCArray::create();
	m_pTouches->retain();
	return true;
}
//-------------------------------------------------------------------------
FKCW_GR_PinchGestureRecognizer::~FKCW_GR_PinchGestureRecognizer()
{
	m_pTouches->release();
}
//-------------------------------------------------------------------------
bool FKCW_GR_PinchGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
	if (m_bIsRecognizing || !_IsPositionBetweenBounds(pTouch->getLocation())) 
	{
		return false;
	}

	m_nTouchNumber++;
	m_pTouches->addObject(pTouch);

	// 俩指头触摸，就开始记录了
	if (m_nTouchNumber == 2) 
	{
		m_bIsRecognizing = true;
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_GR_PinchGestureRecognizer::ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)
{
	if (!m_bIsRecognizing) 
	{
		return;
	}

	CCTouch * touch1 = (CCTouch*)m_pTouches->objectAtIndex(0);
	CCTouch * touch2 = (CCTouch*)m_pTouches->objectAtIndex(1);
	CCPoint delta1 = touch1->getDelta();
	CCPoint delta2 = touch2->getDelta();

	if ((fabs(delta1.x)< s_PinchThreshold&& fabs(delta1.y)<s_PinchThreshold) 
		|| (fabs(delta2.x)<s_PinchThreshold && fabs(delta2.y)<s_PinchThreshold)) 
	{
		return;
	}

	float distance = _DistanceBetweenPoints(touch1->getLocation(), touch2->getLocation());
	if (!m_fLastDistance) 
	{
		m_fLastDistance = distance;
		return;
	}

	FKCW_GR_Pinch* pinch = FKCW_GR_Pinch::create();

	if (m_fLastDistance<=distance) 
	{
		pinch->m_eType = eGRPinchType_Open;
	}
	else 
	{
		pinch->m_eType = eGRPinchType_Close;
	}

	if ((delta1.x>0 && delta2.x<0) || (delta1.x<0 && delta2.x>0) ||
		(delta1.y>0 && delta2.y<0) || (delta1.y<0 && delta2.y>0)) 
	{
		_GestureRecognized(pinch);
	}
}
//-------------------------------------------------------------------------
void FKCW_GR_PinchGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
	m_bIsRecognizing	= false;
	m_fLastDistance		= 0;
	m_nTouchNumber--;
	m_pTouches->removeObject(pTouch);

	if (m_bIsCancelsTouchesInView) 
	{
		_StopTouchesPropagation(_CreateSetWithTouch(pTouch), pEvent);
	}
}
//-------------------------------------------------------------------------