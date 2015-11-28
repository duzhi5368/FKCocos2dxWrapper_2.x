//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_GR_LongPress.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_GR_GestureRecognizer.h"
//-------------------------------------------------------------------------
// 定义长按时间为0.5秒
static const float		s_LongPressMinDuration	= 0.5f;
//-------------------------------------------------------------------------
// 长按手势
class FKCW_GR_LongPress : public CCObject
{
public:
	bool		init(){ return true; }
	CREATE_FUNC(FKCW_GR_LongPress);
	CCPoint		m_tagLocation;
};
//-------------------------------------------------------------------------
class FKCW_GR_LongPressGestureRecognizer : public FKCW_GR_GestureRecognizer
{
public:
	bool			init();
	void			TimerDidEnd(float dt);
	FKCW_GR_LongPressGestureRecognizer();
	~FKCW_GR_LongPressGestureRecognizer();
	CREATE_FUNC(FKCW_GR_LongPressGestureRecognizer) ;

	virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent){};
	virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
private:
	CCPoint			m_tagCurrLocation;
	CCTouch*		m_pCurrTouch;
	CCEvent*		m_pCurrEvent;
	CC_SYNTHESIZE(float, m_fMinimumPressDuration, MinimumPressDuration);	// 最小点击间隔时间

	void		__StopGestureRecognition();
};