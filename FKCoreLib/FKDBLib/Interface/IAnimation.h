//*************************************************************************
//	创建日期:	2014-8-18   18:20
//	文件名称:	IAnimation.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../Include/BaseMacro.h"
#include <string>
#include <list>
using std::string;
using std::list;
//-------------------------------------------------------------------------
struct SAnimationCallbackInfo
{
	char	m_szActName[32];	// 动作名称
	char	m_szName[32];		// 回调名称
	float	m_fFrameTime;		// 帧回调时间

	SAnimationCallbackInfo()
	{
		memset( this, 0, sizeof(*this) );
	}
};

// 动画播放接口
class IAnimationCallback
{
public:
	virtual void	OnAnimaitonCallback( const SAnimationCallbackInfo& p_tagInfo, void* p_pContext ) = 0;
};

class IAnimation
{
public:
	// 设置动画播放回调函数
	virtual void	SetAniCallback( IAnimationCallback* p_pAniCallBack, void* p_pContext ) = 0;

	// 播放动画，若该动作处于停止状态，则继续播放
	virtual void	Play( const std::string& p_szAniName, float p_fSpeed = 1.0f, int p_nLoops = -1, bool p_bForceUpdate = false ) = 0;

	// 停止动画
	virtual void	Stop() = 0;

	// 暂停动画
	virtual void	Pause() = 0;

	// 是否动画在播放
	virtual bool	IsPlaying() = 0;

	// 是否动画在暂停状态
	virtual bool	IsPause() = 0;

	// 是否存在指定名称的动作
	virtual bool	IsAniExit( const string& p_szAniName ) = 0;

	// 获取动作列表
	virtual void	GetAnimationList( FK_OUT list<string>& p_listAnimations ) = 0;

	// 设置/获取播放速度
	virtual void	SetSpeed( float p_fSpeed ) = 0;
	virtual float	GetSpeed() = 0;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------