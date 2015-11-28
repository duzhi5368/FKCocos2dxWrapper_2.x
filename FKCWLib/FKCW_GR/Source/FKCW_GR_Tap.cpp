//--------------------------------------------------------------------
#include "../Include/FKCW_GR_Tap.h"
//--------------------------------------------------------------------
bool FKCW_GR_TapGestureRecognizer::init()
{
	setNumberOfTapsRequired(1);
	m_nTaps = 0;
	return true;
}
//--------------------------------------------------------------------
FKCW_GR_TapGestureRecognizer::~FKCW_GR_TapGestureRecognizer()
{

}
//--------------------------------------------------------------------
bool FKCW_GR_TapGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
	if (m_bIsRecognizing && m_nTaps==0) 
	{
		__StopGestureRecognition();
		return false;
	}

	m_tagInitialPosition = pTouch->getLocation();

	if (!_IsPositionBetweenBounds(m_tagInitialPosition)) 
		return false;

	CCTime::gettimeofdayCocos2d(&m_tagStartTime, NULL);

	if (m_nTaps>0 && static_cast<unsigned int>(m_nTaps)<m_unNumberOfTapsRequired) 
	{
		float distance = _DistanceBetweenPoints(m_tagFinalPosition, m_tagInitialPosition);
		double duration = CCTime::timersubCocos2d(&m_tagEndTime, &m_tagStartTime); 
		if (duration>s_TapMaxDurationBetweenTaps || distance>s_TapMaxDistanceBetweenTaps) 
		{
			__StopGestureRecognition();
		}
	}

	m_bIsRecognizing = true;
	return true;
}
//--------------------------------------------------------------------
void FKCW_GR_TapGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
	// 计算本次按下到放开之间的延迟
	CCTime::gettimeofdayCocos2d(&m_tagEndTime, NULL);
	double duration = CCTime::timersubCocos2d(&m_tagStartTime, &m_tagEndTime);

	// 计算本次按下到放开之间的距离
	m_tagFinalPosition = pTouch->getLocation();
	float distance = _DistanceBetweenPoints(m_tagInitialPosition, m_tagFinalPosition);

	// 检查本次是否点击成功
	if (duration<=s_TapMaxDuration && distance<=s_TapMaxDistance) 
	{
		m_nTaps++;
		if (m_nTaps==m_unNumberOfTapsRequired) {
			FKCW_GR_Tap * tap = FKCW_GR_Tap::create();
			tap->m_tagLocation = m_tagInitialPosition;

			_GestureRecognized(tap);
			if (m_bIsCancelsTouchesInView) 
				_StopTouchesPropagation(_CreateSetWithTouch(pTouch), pEvent);

			__StopGestureRecognition();
		}
	}
	else 
	{
		// 不是点击
		__StopGestureRecognition();
	}
}
//--------------------------------------------------------------------
void FKCW_GR_TapGestureRecognizer::__StopGestureRecognition()
{
	m_nTaps = 0;
	m_bIsRecognizing = false;
}
//--------------------------------------------------------------------