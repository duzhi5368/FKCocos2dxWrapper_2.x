//*************************************************************************
//	创建日期:	2014-11-18
//	文件名称:	FKCW_EffectNode_Shatter.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
class FKCW_EffectNode_ShatterFragSprite:public CCSprite
{
public:
	int m_randomNumber;
	FKCW_EffectNode_ShatterFragSprite()
	{
		m_randomNumber=-1;
	}
};
//-------------------------------------------------------------------------
class FKCW_Action_ShatterAction;
class FKCW_EffectNode_ShatterSprite:public CCSprite
{
public:
	friend class FKCW_Action_ShatterAction;
public:
	FKCW_EffectNode_ShatterSprite();
	virtual~FKCW_EffectNode_ShatterSprite();

	bool		init(const string&texFileName);

protected:
	void		_CreateShatter();
	void		_ResetShatter();
	void		_UpdateShatterAction(float time,float dt,float growSpeedOfTargetR);
protected:
	float m_gridSideLen;				// 格子边长，也就是碎片边长大小
	float m_initalFrageScale;			// 我们需要开始将格子略微放大
	vector<vector<FKCW_EffectNode_ShatterFragSprite*> > m_grid;// 全部碎片列表
	CCSpriteBatchNode* m_fragBatchNode;	// 将全部碎片添加到本batchNode中
};
//-------------------------------------------------------------------------
class FKCW_Action_ShatterAction : public CCActionInterval
{
public:
	FKCW_Action_ShatterAction();
	virtual~FKCW_Action_ShatterAction();

	static FKCW_Action_ShatterAction* create(float fDuration);
	bool			initWithDuration(float fDuration);

	virtual void	stop();
	virtual void	startWithTarget(CCNode *pTarget);
	virtual void	update(float time);
	virtual bool	isDone();
protected:
	void			_InitMembers();
protected:
	float m_timeFoe;
	float m_timeCur;
	float m_growSpeedOfTargetR;
};
//-------------------------------------------------------------------------