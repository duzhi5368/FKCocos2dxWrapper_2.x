//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_GR_Pinch.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_GR_GestureRecognizer.h"
//-------------------------------------------------------------------------
enum ENUM_GRPinch_Type
{
	eGRPinchType_Close		= 1 << 0,
	eGRPinchType_Open		= 1 << 1,
};
//-------------------------------------------------------------------------
// 定义捏合阀值
static const float		s_PinchThreshold	= 2.0f;
//-------------------------------------------------------------------------
// 捏合，挤压动作（iOS里常见后果是进行缩放）
class FKCW_GR_Pinch : public CCObject
{
public:
	bool		init(){ return true; }
	CREATE_FUNC(FKCW_GR_Pinch);
	ENUM_GRPinch_Type	m_eType;
};
//-------------------------------------------------------------------------
class FKCW_GR_PinchGestureRecognizer : public FKCW_GR_GestureRecognizer
{
public:
	bool			init();
	~FKCW_GR_PinchGestureRecognizer();
	CREATE_FUNC(FKCW_GR_PinchGestureRecognizer) ;

	virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent);
private:
	int			m_nTouchNumber;
	float		m_fLastDistance;
	CCArray*	m_pTouches;
};