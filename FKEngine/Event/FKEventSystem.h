//*************************************************************************
//	创建日期:	2015-1-12
//	文件名称:	FKEventSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "TEventEngine.h"
//-------------------------------------------------------------------------
class OnVoteObj
{
public:
	bool operator() ( IVoteEventHandler* pHandler, EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen )
	{
		if( pHandler == NULL )
			return false;
		return pHandler->OnVote( usID, usSrcType, unSrcID, szEvent, nEventLen );
	}
};
//-------------------------------------------------------------------------
class OnExecObj
{
public:
	bool operator() ( IExecEventHandler* pHandler, EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen )
	{
		if( pHandler == NULL )
			return false;
		pHandler->OnExec( usID, usSrcType, unSrcID, szEvent, nEventLen );
		return true;
	}
};
//-------------------------------------------------------------------------
class FKEventSystem : public IEventSystem
{
private:
	typedef TEventEngine< IVoteEventHandler, OnVoteObj >		TCENTER_VOTE;
	typedef TEventEngine< IExecEventHandler, OnExecObj >		TCENTER_EXEC;
	TCENTER_VOTE				m_VoteCenter;		// 投票中心
	TCENTER_EXEC				m_ExecCenter;		// 执行中心
public:
	FKEventSystem();
	~FKEventSystem();
public:
	// 释放
	virtual void		Release();
	// 发起投票事件
	// 参数说明参见上文 OnVote
	// 返回值：是否全员投票通过
	virtual bool		FireVote( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen );
	// 发起执行事件
	// 参数说明参见上文 OnExec
	virtual void		FireExec( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen );
	// 订阅投票事件
	// 参数：pHandler 订阅者类
	// 参数：usSrcType 发送者源类型（参见相关枚举）
	// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
	//		以及某对象指针等，视消息类型逻辑程序员自己规定）
	//		该值允许为0，则表示订阅全部，不再限定指定ID
	// 参数：szDesc 订阅者类的订阅描述，用来调试输出log
	// 返回值：是否成功订阅
	virtual bool		SubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0, const char* szDesc = NULL );
	// 订阅执行事件
	// 参数返回值：同上函数
	virtual bool		SubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0, const char* szDesc = NULL );
	// 取消一份投票订阅
	virtual bool		UnSubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0 );
	// 取消一份执行订阅
	virtual bool		UnSubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0 );
};
//-------------------------------------------------------------------------
extern FKEventSystem gs_FKEventSystem;
//-------------------------------------------------------------------------