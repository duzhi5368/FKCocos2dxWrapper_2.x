//-------------------------------------------------------------------------
#include "FKTimerSystem.h"
#include "FKTimerSystemMacros.h"
//-------------------------------------------------------------------------
ITimerSystem*	gs_pTimerSystem = NULL;
FKTimerSystem	gs_FKTimerSystem;
//-------------------------------------------------------------------------
// 获取定时器系统接口
ITimerSystem* GetTimerSystem()
{
	return gs_pTimerSystem;
}
//-------------------------------------------------------------------------
FKTimerSystem::FKTimerSystem()
{
	m_TimerAxis.resize(( gs_MAX_TIME_AXIS_LENGTH / gs_DEFAULT_TIME_GRID - 1 ) / gs_DEFAULT_TIME_GRID);
	m_unInitializeTime = FKCW_Base_Utils::GetTimeTickCount();
	m_unLastCheckTick = m_unInitializeTime;

	gs_pTimerSystem = this;
}
//-------------------------------------------------------------------------
FKTimerSystem::~FKTimerSystem()
{
	gs_pTimerSystem = NULL;
}
//-------------------------------------------------------------------------
// 销毁
void FKTimerSystem::Release()
{
	for ( unsigned long i=0;i<m_TimerAxis.size();++i )
	{
		TIMER_LIST& TimerList = m_TimerAxis[i];
		TIMER_LIST::iterator it = TimerList.begin();
		for ( ;it!=TimerList.end();++it )
		{
			STimerInfo * pTimer = *it;
			if( pTimer )
			{
				DestroyTimer(pTimer->m_unTimerID,pTimer->m_pHandler);
			}
		}
	}
	m_TimerAxis.clear();
}
//-------------------------------------------------------------------------
// 创建一个定时器
// 参数：unTimerID - 定时器ID
//		 unIntervalTime - 定时器调用间隔时间
//		 pHandler - 回调函数接口类
//		 unCallTime - 回调多少次
//		 szDebugInfo - 调试信息
bool FKTimerSystem::CreateTimer( unsigned int unTimerID, unsigned int unIntervalTime,
						ITimerHander* pHandler, unsigned int unCallTime, const char* szDebugInfo )
{
	if( pHandler == NULL )
		return false;
	if( unIntervalTime == 0 )
		unIntervalTime = 1;

	void** ppTimerInfo = pHandler->GetTimerInfoPtr();
	if( ppTimerInfo == NULL)
		return false;

	TIMER_INFOS* pTimerInfos = *(TIMER_INFOS**)ppTimerInfo;
	if( ppTimerInfo == NULL || pTimerInfos ==0)
	{
		pTimerInfos = new TIMER_INFOS;
		*ppTimerInfo = pTimerInfos;
	}

	// 检查该Timer是否已经存在
	TIMER_INFOS::iterator Ite = pTimerInfos->begin();
	for( ; Ite != pTimerInfos->end(); ++Ite )
	{
		STimerInfo& tagTimer = *Ite;
		if( tagTimer.m_unTimerID == unTimerID )
		{
			return false;
		}
	}

	STimerInfo tagTimerInfo;
	tagTimerInfo.m_unTimerID		= unTimerID;
	tagTimerInfo.m_unIntervalTime	= unIntervalTime;
	tagTimerInfo.m_unCallTimes		= unCallTime;
	tagTimerInfo.m_unLastCallTick	= m_unLastCheckTick;
	tagTimerInfo.m_pHandler			= pHandler;
	if( szDebugInfo )
		tagTimerInfo.m_strDebugInfo	= szDebugInfo;
	tagTimerInfo.m_unTimerGridIndex	= (( tagTimerInfo.m_unLastCallTick + tagTimerInfo.m_unIntervalTime 
		- m_unInitializeTime ) / gs_DEFAULT_TIME_GRID ) % m_TimerAxis.size();

	pTimerInfos->push_back( tagTimerInfo );

	// 计算填充快速查询的Iter
	STimerInfo& TimerRef = pTimerInfos->back();
	STimerInfo* pTimerIte = &TimerRef;
	m_TimerAxis[tagTimerInfo.m_unTimerGridIndex].push_back( pTimerIte );
	pTimerIte->m_ItePos = --m_TimerAxis[tagTimerInfo.m_unTimerGridIndex].end();

	return true;
}
//-------------------------------------------------------------------------
// 销毁一个定时器
// 参数：unTimerID - 定时器ID
//		 pHandler - 回调函数接口类
bool FKTimerSystem::DestroyTimer( unsigned int unTimerID, ITimerHander* pHandler )
{
	void** ppTimerInfo = pHandler->GetTimerInfoPtr();
	if( ppTimerInfo == NULL )
		return false;

	TIMER_INFOS* pTimerInfos = *(TIMER_INFOS**)ppTimerInfo;

	// 根本没添加
	if( pTimerInfos == NULL )
		return false;

	// 是否添加了本Timer
	TIMER_INFOS::iterator Ite = pTimerInfos->begin();
	for( ;Ite != pTimerInfos->end(); ++Ite )
	{
		STimerInfo& tagTimer = *Ite;
		if( tagTimer.m_unTimerID == unTimerID )
		{
			// 注意，这里并么有直接删除本Timer，因为此时可能正遍历的DestoryTimer.
			// 为避免遍历崩溃，这里不直接删除TimerIte，而是先设置为NULL，之后再删除
			*tagTimer.m_ItePos = NULL;

			pTimerInfos->erase( Ite );
			if( pTimerInfos->empty() )
			{
				delete pTimerInfos;
				*ppTimerInfo = NULL;
			}
			// 成功
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------
// 逻辑更新
void FKTimerSystem::UpdateTimer()
{
	unsigned long unCurTick = FKCW_Base_Utils::GetTimeTickCount();

	// 超过指定检查频率之后才检查
	if ( unCurTick - m_unLastCheckTick< gs_DEFAULT_CHECK_FREQUENCY )
		return;

	unsigned int unStartGrid = ( ( m_unLastCheckTick - m_unInitializeTime ) / gs_DEFAULT_TIME_GRID ) % m_TimerAxis.size();
	unsigned int unCurGrid = ( ( unCurTick - m_unInitializeTime) / gs_DEFAULT_TIME_GRID ) % m_TimerAxis.size();

	// 记录当前Check时间
	m_unLastCheckTick = unCurTick;

	unsigned long i = unStartGrid;

	// 遍历时间刻度
	do
	{
		// 遍历当前时间刻度中的所有待触发定时器
		TIMER_LIST& TimerList = m_TimerAxis[i];
		TIMER_LIST::iterator Ite = TimerList.begin();
		for ( ;Ite != TimerList.end(); )
		{
			STimerInfo* pTimer = *Ite;
			if ( pTimer == NULL )
			{
				// 应当被删除的Timer
				Ite = TimerList.erase(Ite);
				continue;
			}

			// 触发定时器
			if ( unCurTick - pTimer->m_unLastCallTick >= pTimer->m_unIntervalTime )
			{
				// 执行定时器回调
				unsigned long dwTick = FKCW_Base_Utils::GetTimeTickCount();
				pTimer->m_pHandler->OnTimer( pTimer->m_unTimerID );


				// 检查一下这个Timer是否被删除了
				if ( *Ite == pTimer )
				{
					int nCostTime = FKCW_Base_Utils::GetTimeTickCount() - dwTick;
					if( nCostTime > 64 && nCostTime > gs_DEFAULT_TIME_GRID )
					{ 
						FKLOG("定时器频率过低: ID = %d, 期望间隔 = %d, 实际间隔 = %d, 描述 = %s",
							pTimer->m_unTimerID, pTimer->m_unIntervalTime, nCostTime, pTimer->m_strDebugInfo.c_str() );
					}

					pTimer->m_unLastCallTick = unCurTick;
					pTimer->m_unCallTimes    -= 1;

					if ( pTimer->m_unCallTimes == 0 )
					{
						// 调用次数已经够了，开始考虑删除了
						DestroyTimer( pTimer->m_unTimerID,pTimer->m_pHandler );
					}
					else
					{
						// 搬迁到下一次触发的位置
						pTimer->m_unTimerGridIndex = ( (pTimer->m_unLastCallTick + pTimer->m_unIntervalTime - m_unInitializeTime) 
							/ gs_DEFAULT_TIME_GRID ) % m_TimerAxis.size();
						Ite = TimerList.erase(Ite);
						m_TimerAxis[pTimer->m_unTimerGridIndex].push_back(pTimer);
						pTimer->m_ItePos = --m_TimerAxis[pTimer->m_unTimerGridIndex].end();

						continue;
					}
				}
				else
				{
					// timer自己被删除了
					Ite = TimerList.erase(Ite);
					continue;
				}
			}

			// 未被触发的Timer则直接跳过
			++Ite;
		}

		// 递进到下一个刻度
		if ( i == unCurGrid )
			break;
		i = (i+1) % m_TimerAxis.size();

	}while( i != unCurGrid);
}
//-------------------------------------------------------------------------