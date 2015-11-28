/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_Shake
	author:		FreeKnight
	
	purpose:	震动
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 节点随机在一个圆内运动，就像在震动
class FKCW_Action_Shake : public CCActionInterval
{
private:
	enum ENUM_ShakeMode{
		eShakeMode_Ring,
		eShakeMode_DynamicRing,
	};
protected:
	float		m_fMinRadius;			// 震动圈最小半径
	float		m_fMaxRadius;			// 震动圈最大半径
	float		m_fEndMaxRadius;		// eShakeMode_DynamicRing模式下使用。最大偏移范围
	float		m_fEndMinRadius;		// eShakeMode_DynamicRing模式下使用。最小偏移范围
	float		m_fOriginalX;			// 节点的基准点x
	float		m_fOriginalY;			// 节点的基准点y
	ENUM_ShakeMode	m_eMode;			// 节点震动模式
protected:
	FKCW_Action_Shake();
public:
	// 创建一个静态圈震动动作
	// 参数：duration 持续时间
	static FKCW_Action_Shake* Create( float duration, float maxRadius, float minRadius = 0 );

	// 创建一个动态圈震动动作
	static FKCW_Action_Shake* Create( float duration, float startMaxRadius, float endMaxRadius, float startMinRadius, float endMinRadius );

	virtual ~FKCW_Action_Shake();

	bool	InitWithRing( float duration, float maxRadius, float minRadius );
	bool	InitWithDynamicRing( float duration, float startMaxRadius, float endMaxRadius, float startMinRadius, float endMinRadius );

	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
	virtual void update(float t);
	virtual void startWithTarget(CCNode* pTarget);
};