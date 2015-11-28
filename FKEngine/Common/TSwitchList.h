//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	TSwitchList.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
template< class SCommand >
class TSwitchList
{
public:
	typedef list<SCommand>		COMMAND_LIST;		// 命令列表
private:
	COMMAND_LIST				m_OutCommandList;
	COMMAND_LIST				m_InCommandList;
	COMMAND_LIST*				m_pOutCommandList;
	COMMAND_LIST*				m_pInCommandList;
	pthread_mutex_t				m_pMutexLock;		// 线程锁
public:
	TSwitchList() 
		: m_pOutCommandList(&m_OutCommandList)
		, m_pInCommandList(&m_InCommandList)
	{
		pthread_mutex_init(&m_pMutexLock, NULL);
	}
	virtual ~TSwitchList()
	{		
		pthread_mutex_destroy(&m_pMutexLock);
	}
	void PushFront(SCommand *pCommand)
	{
		if (pCommand == NULL)
			return;
		pthread_mutex_lock(&m_pMutexLock);	
		m_pInCommandList->push_front(*pCommand);
		pthread_mutex_unlock(&m_pMutexLock);
	}
	void PushBack(SCommand *pCommand)
	{
		if (pCommand == NULL)
			return;
		pthread_mutex_lock(&m_pMutexLock);
		m_pInCommandList->push_back(*pCommand);
		pthread_mutex_unlock(&m_pMutexLock);
	}
	// 反转
	void SwitchState()
	{
		pthread_mutex_lock(&m_pMutexLock);
		if (m_pOutCommandList->size() > 0)
		{
			pthread_mutex_unlock(&m_pMutexLock);
			return;
		}
		COMMAND_LIST *pList = m_pInCommandList;
		m_pInCommandList = m_pOutCommandList;
		m_pOutCommandList = pList;
		pthread_mutex_unlock(&m_pMutexLock);
	}
	COMMAND_LIST& GetOutList()
	{
		return m_OutCommandList;
	}
};