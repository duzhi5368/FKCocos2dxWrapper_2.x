/********************************************************************
	created:	2014/11/05
	file base:	FKCW_GR_Tap
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_GR_GestureRecognizer.h"
//--------------------------------------------------------------------
static const int		s_TapMaxDuration			= 150;	// 单次点击按下到放开的间隔时间（若超过，则视为长按等事件）
static const int		s_TapMaxDurationBetweenTaps	= 220;	// 两次点击按下的间隔时间（若超过，则视为非连续点击）
static const int		s_TapMaxDistance			= 20;	// 单次点击按下到放开的间隔时间（若超过，则视为滑动等事件）
static const int		s_TapMaxDistanceBetweenTaps	= 20;	// 两次点击按下的距离（若超过该值，则视为非同一点点击）
//--------------------------------------------------------------------
class FKCW_GR_Tap : public CCObject
{
public:
	bool init() {return true;}
	CREATE_FUNC(FKCW_GR_Tap);
	CCPoint m_tagLocation;
};
//--------------------------------------------------------------------
// 点击事件
// 注：默认处理单击消息;若处理双击事件，则需要调用 setNumberOfTapsRequired(2)
class FKCW_GR_TapGestureRecognizer : public FKCW_GR_GestureRecognizer
{
public:
	bool init();
	~FKCW_GR_TapGestureRecognizer();
	CREATE_FUNC(FKCW_GR_TapGestureRecognizer);

	virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent){};
	virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
private:
	void	__StopGestureRecognition();
private:
	int		m_nTaps;
	CCPoint m_tagInitialPosition;
	CCPoint	m_tagFinalPosition;
	struct cc_timeval m_tagStartTime, m_tagEndTime;
	CC_SYNTHESIZE(unsigned int, m_unNumberOfTapsRequired, NumberOfTapsRequired);
};