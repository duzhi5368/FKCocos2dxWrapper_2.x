//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	FKCW_Action_Curl.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
class FKCW_Action_Curl : public CCActionInterval
{
private:
	CCPoint m_center;			// 中心点
	float m_fromRadius;			// From半径
	float m_toRadius;			// To半径
	float m_angularVelocity;	// 角速度
	float m_initAngle;			// 初始角度

public:
	FKCW_Action_Curl();
	virtual ~FKCW_Action_Curl();

	static FKCW_Action_Curl* Create(float duration, CCPoint center, float toRadius, float angularVelocity);
	bool initWithDuration(float d, CCPoint center, float toRadius, float angularVelocity);

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
};