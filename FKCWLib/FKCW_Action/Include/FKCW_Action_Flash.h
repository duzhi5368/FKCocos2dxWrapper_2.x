//*************************************************************************
//	创建日期:	2014-11-3
//	文件名称:	FKCW_Action_Flash.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	闪光效果
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//-------------------------------------------------------------------------
// 对一个节点的不透明区域执行闪光效果
class FKCW_Action_Flash : public CCActionInterval
{
private:
	CCGLProgram*			m_pOldProgram;		// 记录上一次的program
	ccColor3B				m_tagColor;			// 颜色
	float					m_fR;
	float					m_fG;
	float					m_fB;
protected:
	FKCW_Action_Flash();
public:
	virtual ~FKCW_Action_Flash();

	// 创建一个闪光对象
	// 参数：p_fDuration 闪光动作持续时间 p_tagColor 闪光色
	static FKCW_Action_Flash* Create(float p_fDuration, ccColor3B p_tagColor );

	bool InitWithDurationAndColor(float d, ccColor3B c);

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual void stop();
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
};