//*************************************************************************
//	创建日期:	2014-8-20   13:49
//	文件名称:	Cocos2dxDBAnimation.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "CommonHead.h"
#include "cocos-ext.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class CCocos2dxDBNode;
class CCocos2dxDBAnimation : public CCObject, public IAnimation
{
public:
	CCocos2dxDBAnimation();
	~CCocos2dxDBAnimation();
public:
	// 设置渲染节点
	void			SetDBNode( CCocos2dxDBNode* p_pDBNode );

	// 设置动画播放回调函数
	virtual void	SetAniCallback( IAnimationCallback* p_pAniCallBack, void* p_pContext );

	// 播放动画，若该动作处于停止状态，则继续播放
	// Loop -1 表示持续播放
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

	// 设置动画循环次数
	virtual void	SetAniLoop( int nLoop );
private:
	// 动作回调函数
	void			OnMovementEventCallFunc( extension::CCArmature* p_pArmature,
		extension::MovementEventType p_eMovementEventType, const char* p_szEventName );

	// 帧事件回调
	void			OnFrameEventCallFunc( extension::CCBone* p_pBone, 
		const char* p_szEventName, int p_nFrameID, int p_nTime );

	// 骨骼动画回调
	void			OnDBEventCallFunc( CCNode* pNode, void* p_pEvent );
private:
	int					m_nState;			// 播放状态 0正常 1暂停 2停止
	float				m_fSpeed;			// 动画播放速度
	string				m_szCurActionName;	// 当前动作名称
	CAnimation*			m_pAnimation;		// 动画数据
	CArmature*			m_pArmature;		// 骨骼动画
	CCocos2dxDBNode*	m_pDBNode;			// 渲染节点对象
	IAnimationCallback*	m_pAniCallback;		// 动画播放回调
	void*				m_pCallbackContext;	// 动画回调参数
};