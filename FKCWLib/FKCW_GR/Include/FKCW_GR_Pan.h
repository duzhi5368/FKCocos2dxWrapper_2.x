/********************************************************************
	created:	2014/11/05
	file base:	FKCW_GR_Pan
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_GR_GestureRecognizer.h"
//-------------------------------------------------------------------------
// »¬¶¯²Ù×÷,ÂýËÙÍÏÒÆ
class FKCW_GR_Pan : public CCObject
{
public:
	bool init() {return true;}
	CREATE_FUNC(FKCW_GR_Pan);
	CCPoint m_tagLocation;
	CCPoint m_tagDelta;
};
//--------------------------------------------------------------------
class FKCW_GR_PanGestureRecognizer : public FKCW_GR_GestureRecognizer
{
public:
	bool init();
	~FKCW_GR_PanGestureRecognizer();
	CREATE_FUNC(FKCW_GR_PanGestureRecognizer);

	virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
};