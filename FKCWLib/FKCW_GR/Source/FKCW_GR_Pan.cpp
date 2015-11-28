//--------------------------------------------------------------------
#include "../Include/FKCW_GR_Pan.h"
//--------------------------------------------------------------------
bool FKCW_GR_PanGestureRecognizer::init()
{
	return true;
}
//--------------------------------------------------------------------
FKCW_GR_PanGestureRecognizer::~FKCW_GR_PanGestureRecognizer()
{

}
//--------------------------------------------------------------------
bool FKCW_GR_PanGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
	if (m_bIsRecognizing) 
	{
		m_bIsRecognizing = false;
		return false;
	}

	CCPoint loc = pTouch->getLocation();
	if (!_IsPositionBetweenBounds(loc))
		return false;

	m_bIsRecognizing = true;
	return true;
}
//--------------------------------------------------------------------
void FKCW_GR_PanGestureRecognizer::ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)
{
	FKCW_GR_Pan * pan = FKCW_GR_Pan::create();
	pan->m_tagLocation = pTouch->getLocation();
	pan->m_tagDelta = pTouch->getDelta();
	_GestureRecognized(pan);
}
//--------------------------------------------------------------------
void FKCW_GR_PanGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
	m_bIsRecognizing = false;

	if (m_bIsCancelsTouchesInView)
	{
		_StopTouchesPropagation(_CreateSetWithTouch(pTouch), pEvent);
	}
}
//--------------------------------------------------------------------