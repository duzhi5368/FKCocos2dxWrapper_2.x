//*************************************************************************
//	创建日期:	2014-11-3
//	文件名称:	FKCW_Action_ClipOut.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//-------------------------------------------------------------------------
// 逐渐隐藏一个节点
// 注：该动作仅可被 CCClippingNode 执行，且该 CCClippingNode 必须有一个CCDrawNode模板
class FKCW_Action_ClipOut : public CCActionInterval
{
private:
	float			m_fCos;			// 方向cos值
	float			m_fSin;			// 方向sin值
	float			m_fDistance;	// 完整距离
	CCPoint			m_tagP1;		// 底边固定点1
	CCPoint			m_tagP2;		// 底边固定点2
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint, m_tagDirection, Direction);
protected:
	FKCW_Action_ClipOut();
public:
	virtual ~FKCW_Action_ClipOut();
	// 创建一个 ClipIn 动作实例
	// 参数：p_fDuration 动作持续时间
	//		p_tagV 该剪辑的移动方向矢量，默认情况下它是(1,0)，表示沿X轴移动
	static FKCW_Action_ClipOut* Create( float p_fDuration, CCPoint p_tagV = ccp(1, 0 ));

	bool	InitWithDuration( float p_fDuration, CCPoint p_tagV );

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual CCActionInterval* reverse();
};