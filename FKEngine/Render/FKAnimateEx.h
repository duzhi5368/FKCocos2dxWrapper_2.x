//*************************************************************************
//	创建日期:	2015-1-22
//	文件名称:	FKAnimateEx.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKAnimateExRes.h"
//-------------------------------------------------------------------------
// 动画播放状态
enum ENUM_AnimState
{
	eAnimState_Normal	= 0,	// 正常播放状态
	eAnimState_Pause	= 1,	// 暂停状态
	eAnimState_Stop		= 2,	// 停止状态
};
//-------------------------------------------------------------------------
class FKAnimateExRes;
class FKAnimateEx : public IAnimation
{
public:
	FKAnimateEx();
	~FKAnimateEx();
public:
	// 重置
	virtual void	Reset();
	// 设置动画资源
	virtual void	SetAnimateExRes( FKAnimateExRes* pAniRes );
	// 设置目标对象
	virtual void	SetTargetNode( CCNode* pTaeget );
	// 帧更新
	virtual void	Update( float dt );
public:
	// 设置动画播放回调函数
	virtual void	SetAniCallback( IAnimationCallback* p_pAniCallBack, void* p_pContext );

	// 播放动画，若该动作处于停止状态，则继续播放
	virtual void	Play( const std::string& p_szAniName, float p_fSpeed = 1.0f,
		int p_nLoops = -1, bool p_bForceUpdate = false );

	// 停止动画
	virtual void	Stop();

	// 暂停动画
	virtual void	Pause();

	// 是否动画在播放
	virtual bool	IsPlaying();

	// 是否动画在暂停状态
	virtual bool	IsPause();

	// 是否存在指定名称的动作
	virtual bool	IsAniExit( const string& p_szAniName );

	// 获取动作列表
	virtual void	GetAnimationList( FK_OUT list<string>& p_listAnimations );

	// 设置/获取播放速度
	virtual void	SetSpeed( float p_fSpeed );
	virtual float	GetSpeed();
protected:
	// 动画回调
	void			_OnAniCallback( float fLastTime, float fCurTime, float fDuration );
private:
	bool			m_bIsResValid;		// 是否资源有效
	float			m_fTime;			// 动画播放时间
	float			m_fSpeed;			// 动画播放速度
	int				m_nLoopTime;		// 动画播放次数
	int				m_nNextFrame;		// 下一帧索引
	bool			m_bIsNewLoop;		// 是否是新的播放循环
	string			m_szCurAniName;		// 当前动作名
	FKAnimateExRes*	m_pAniRes;			// 动作动画资源
	ENUM_AnimState	m_eState;			// 动画播放状态
	CCAnimation*	m_pCurAnimation;	// 当前动作
	CCNode*			m_pTarget;			// 目标对象
	vector<float>	m_vecFrameTimes;	// 帧时间

	IAnimationCallback*			m_pAniCallback;			// 动作播放回调
	void*						m_pAniCallbackContext;	// 动作回调参数
	vector<SAnimationCallback>	m_vecAniCallbacks;		// 动作回调数组
};