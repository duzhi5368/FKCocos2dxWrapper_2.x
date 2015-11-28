/********************************************************************
	created:	2014/11/04
	file base:	FKCW_Action_Shine
	author:		FreeKnight
	
	purpose:	动态光线效果
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 在一个node上显示一系列光线效果
// 该效果为从左向右移动一次，自身为梯度状，并可被设置最多三种颜色
class FKCW_Action_Shine : public CCActionInterval
{
private:
	CCGLProgram*				m_pOldProgram;	// 老的Program
	CCSize						m_tagSize;		// 目标大小
	CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_tagColor1, Color1);
	CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_tagColor2, Color2);
	CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_tagColor3, Color3);
	CC_SYNTHESIZE_PASS_BY_REF(ccVertex3F, m_tagGradientPositions, GradientPositions);	// 梯度位置
protected:
	FKCW_Action_Shine();
public:
	virtual ~FKCW_Action_Shine();

	static FKCW_Action_Shine* Create(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions);
	bool InitWithColors(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions);

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual void stop();
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
};