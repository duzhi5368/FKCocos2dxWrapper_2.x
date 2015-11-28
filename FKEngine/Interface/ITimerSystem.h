//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	ITimerSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/*
	TODO
*/
//-------------------------------------------------------------------------
// 定时器最大回调次数
const static unsigned int	gs_INFINITY_CALLTIME		= 0xffffffff;
//-------------------------------------------------------------------------
// 定时器回调接口
class ITimerHander
{
public:
	// 定时器时间触发回调
	virtual void		OnTimer( unsigned int unTimerID ) = 0;


	// 下面是一些简便方法，用户无需关心
public:
	virtual void**		GetTimerInfoPtr(){ return &m_pTmpInfoPtr; }
public:
	ITimerHander():m_pTmpInfoPtr( 0 ){}
protected:
	void*	m_pTmpInfoPtr;
};
//-------------------------------------------------------------------------
// 定时器系统
struct ITimerSystem
{
public:
	// 销毁
	virtual void		Release() = 0;
	// 创建一个定时器
	// 参数：unTimerID - 定时器ID
	//		 unIntervalTime - 定时器调用间隔时间
	//		 pHandler - 回调函数接口类
	//		 unCallTime - 回调多少次
	//		 szDebugInfo - 调试信息
	virtual bool		CreateTimer( unsigned int unTimerID, unsigned int unIntervalTime,
		ITimerHander* pHandler, unsigned int unCallTime = gs_INFINITY_CALLTIME, const char* szDebugInfo = 0 ) = 0;
	// 销毁一个定时器
	// 参数：unTimerID - 定时器ID
	//		 pHandler - 回调函数接口类
	virtual bool		DestroyTimer( unsigned int unTimerID, ITimerHander* pHandler ) = 0;
	// 逻辑更新
	virtual void		UpdateTimer() = 0;
};
//-------------------------------------------------------------------------
// 获取定时器系统接口
extern ITimerSystem* GetTimerSystem();
//-------------------------------------------------------------------------