//*************************************************************************
//	创建日期:	2014-11-3
//	文件名称:	FKCW_Action_Missile.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	导弹/投掷物 跟踪动作
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//-------------------------------------------------------------------------
/*
	让一个node移动到另一个node处。
	执行动作的对象节点 和 移动目标位的目标节点，必须属于同一个父节点。
	当动作开始后，对象节点开始向目标节点弧线移动，弧线弹道将根据目标节点位置每帧进行调整。
	注意，该动作并不会额外保存目标节点，所以用户必须保证动作播放完成之前，目标节点必然存在。
	又因为目标节点可以移动，所以导弹动作的持续时间无法断定。可以用sequence动作协助处理完成回调，
*/
class FKCW_Action_Missile : public CCActionInterval
{
protected:
	float			m_fVelocity;		// 移动速度
	CCNode*			m_pAimedNode;		// 目标节点
	float			m_fPresetDegree;	// 预角度( 动作节点 相对于 目标节点的路径角度)
	float			m_fDstDegree;		// 目标角度
	CCCallFunc*		m_pDoneCallFunc;	// 完成消息回调
	bool			m_bIsDone;			// 是否完成标签
public:
	// 创建一个导弹跟踪动作对象
	// 参数：velocity 移动速度 aimed 目标对象 
	//	targetPresetDegree 对象节点相对目标节点的旋转角度，正值表示顺时针
	static FKCW_Action_Missile* Create( float velocity, CCNode* aimed, float targetPresetDegree, CCCallFunc* doneCallFunc = NULL );

	FKCW_Action_Missile();
	virtual ~FKCW_Action_Missile();

	bool InitWithVelocity( float velocity, CCNode* aimed, float targetPresetDegree, CCCallFunc* doneCallFunc );

	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void step(float dt);
	virtual bool isDone() { return m_bIsDone; }
};