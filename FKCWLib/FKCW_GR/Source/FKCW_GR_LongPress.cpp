//-------------------------------------------------------------------------
#include "../Include/FKCW_GR_LongPress.h"
//-------------------------------------------------------------------------
bool FKCW_GR_LongPressGestureRecognizer::init()
{
	m_bIsRecognizing		= false;
	m_pCurrTouch			= NULL;
	m_pCurrEvent			= NULL;

	setMinimumPressDuration( m_fMinimumPressDuration );
	return true;
}
//-------------------------------------------------------------------------
void FKCW_GR_LongPressGestureRecognizer::TimerDidEnd(float dt)
{
	FKCW_GR_LongPress * longPress = FKCW_GR_LongPress::create();
	longPress->m_tagLocation = m_tagCurrLocation;

	_GestureRecognized(longPress);
	if (m_bIsCancelsTouchesInView) 
		_StopTouchesPropagation(_CreateSetWithTouch(m_pCurrTouch), m_pCurrEvent);

	__StopGestureRecognition();
}
//-------------------------------------------------------------------------
FKCW_GR_LongPressGestureRecognizer::FKCW_GR_LongPressGestureRecognizer()
	: m_fMinimumPressDuration( s_LongPressMinDuration )
{

}
//-------------------------------------------------------------------------
FKCW_GR_LongPressGestureRecognizer::~FKCW_GR_LongPressGestureRecognizer()
{

}
//-------------------------------------------------------------------------
bool FKCW_GR_LongPressGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
	if (m_bIsRecognizing) 
	{
		__StopGestureRecognition();
		return false;
	}

	m_tagCurrLocation = pTouch->getLocation();
	if (!_IsPositionBetweenBounds(m_tagCurrLocation)) 
		return false;

	m_pCurrEvent = pEvent;
	m_pCurrTouch = pTouch;

	schedule(schedule_selector(FKCW_GR_LongPressGestureRecognizer::TimerDidEnd), m_fMinimumPressDuration);

	m_bIsRecognizing = true;
	return true;
}
//-------------------------------------------------------------------------
void FKCW_GR_LongPressGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
	__StopGestureRecognition();
}
//-------------------------------------------------------------------------
void FKCW_GR_LongPressGestureRecognizer::__StopGestureRecognition()
{
	if (!m_bIsRecognizing) 
		return;

	m_pCurrTouch = NULL;
	m_pCurrEvent = NULL;
	unschedule(schedule_selector(FKCW_GR_LongPressGestureRecognizer::TimerDidEnd));
	m_bIsRecognizing = false;
}
//-------------------------------------------------------------------------