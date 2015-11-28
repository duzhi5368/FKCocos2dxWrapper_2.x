//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	FKTimerSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKTimerSystem : public ITimerSystem
{
public:
	FKTimerSystem();
	~FKTimerSystem();
public:
	// 销毁
	virtual void		Release();
	// 创建一个定时器
	// 参数：unTimerID - 定时器ID
	//		 unIntervalTime - 定时器调用间隔时间
	//		 pHandler - 回调函数接口类
	//		 unCallTime - 回调多少次
	//		 szDebugInfo - 调试信息
	virtual bool		CreateTimer( unsigned int unTimerID, unsigned int unIntervalTime,
		ITimerHander* pHandler, unsigned int unCallTime = gs_INFINITY_CALLTIME, const char* szDebugInfo = 0 );
	// 销毁一个定时器
	// 参数：unTimerID - 定时器ID
	//		 pHandler - 回调函数接口类
	virtual bool		DestroyTimer( unsigned int unTimerID, ITimerHander* pHandler );
	// 逻辑更新
	virtual void		UpdateTimer();
protected:
	struct STimerInfo
	{
		unsigned int		m_unTimerID;				// 用户自定义定时器ID
		unsigned int		m_unIntervalTime;			// 触发间隔时间
		unsigned int		m_unCallTimes;				// 调用次数
		unsigned int		m_unLastCallTick;			// 最后一次被调用的时间
		unsigned int		m_unTimerGridIndex;			// 所在的时间刻度
		ITimerHander*		m_pHandler;					// 消息回调所在类
		string				m_strDebugInfo;				// debug信息
		list<STimerInfo*>::iterator m_ItePos;			// 在时间刻度中的位置，保存该值为加速搜索
	};
	typedef list<STimerInfo>	TIMER_INFOS;			// 在一个ITimerHandler中的定时器临时信息
	typedef list<STimerInfo*>	TIMER_LIST;				// 一个时间刻度中的定时器列表
	typedef vector<TIMER_LIST>	TIMER_AXIS;				// 保存全部时间刻度的时间轴结构
protected:
	TIMER_AXIS				m_TimerAxis;				// 保存全部时间刻度的时间轴结构
	unsigned int			m_unLastCheckTick;			// 最后一次检查的时间
	unsigned int			m_unInitializeTime;			// 时间轴初始时间
};
//-------------------------------------------------------------------------
extern FKTimerSystem gs_FKTimerSystem;
//-------------------------------------------------------------------------