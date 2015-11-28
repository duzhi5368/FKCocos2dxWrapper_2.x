//--------------------------------------------------------------------
#include "../Include/FKCW_Network_UDPSocketHub.h"
//--------------------------------------------------------------------
static MULTI_ENCRYPT_FUNC s_EncryptFunc = NULL;
static MULTI_DECRYPT_FUNC s_DecryptFunc = NULL;
//--------------------------------------------------------------------
FKCW_Network_UDPSocketHub::FKCW_Network_UDPSocketHub()
	: m_bIsRawPolicy( false )
{
	pthread_mutex_init( &m_Mutex, NULL );

	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->scheduleSelector(schedule_selector(FKCW_Network_UDPSocketHub::_MainLoop), this, 0, false);
}
//--------------------------------------------------------------------
// 监听套接字，并进行读写操作
void FKCW_Network_UDPSocketHub::_MainLoop( float p_fDelta )
{
	pthread_mutex_lock(&m_Mutex);

	// 消息通知中心
	CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();

	// 连接事件
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecConnectedSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		nc->postNotification( NotificationUDPSocketConnected, s );
	}
	m_vecConnectedSockets.removeAllObjects();

	// 数据事件
	CCARRAY_FOREACH(&m_vecPackets, obj) 
	{
		FKCW_Network_UDPSocket* p = (FKCW_Network_UDPSocket*)obj;
		nc->postNotification( NotificationPacketReceived, p );
	}
	m_vecPackets.removeAllObjects();

	// 断开连接事件
	CCARRAY_FOREACH(&m_vecDisConnectedSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		nc->postNotification( NotificationUDPSocketDisconnected, s );
		m_vecSockets.removeObject(s);
	}
	m_vecDisConnectedSockets.removeAllObjects();

	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 添加一个套接字到管理中心
bool FKCW_Network_UDPSocketHub::_AddSocket( FKCW_Network_UDPSocket* p_pSocket )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		if(s->getSocket() == p_pSocket->getSocket()) 
		{
			return false;
		}
	}
	m_vecSockets.addObject(p_pSocket);
	p_pSocket->setHub(this);
	return true;
}
//--------------------------------------------------------------------
// 提供给socket调用的函数，当它成功建立连接时调用该函数通知socket管理中心
void FKCW_Network_UDPSocketHub::_OnSocketConnectedThreadSafe( FKCW_Network_UDPSocket* p_pSocket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecConnectedSockets.addObject(p_pSocket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 提供给socket调用的函数，当它断开连接时调用该函数通知socket管理中心
void FKCW_Network_UDPSocketHub::_OnSocketDisConnectedThreadSafe( FKCW_Network_UDPSocket* p_pSocket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecDisConnectedSockets.addObject(p_pSocket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 提供给socket调用的函数，当它准备发送一个包时调用该函数通知socket管理中心
void FKCW_Network_UDPSocketHub::_OnPacketReceivedThreadSafe( FKCW_Network_Packet* p_pPacket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecPackets.addObject(p_pPacket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
FKCW_Network_UDPSocketHub::~FKCW_Network_UDPSocketHub()
{
	pthread_mutex_destroy( &m_Mutex );
}
//--------------------------------------------------------------------
FKCW_Network_UDPSocketHub* FKCW_Network_UDPSocketHub::Create( MULTI_ENCRYPT_FUNC p_pEncryptFunc, MULTI_DECRYPT_FUNC p_pDecryptFunc)
{
	FKCW_Network_UDPSocketHub* p = new FKCW_Network_UDPSocketHub();
	s_EncryptFunc = p_pEncryptFunc;
	s_DecryptFunc = p_pDecryptFunc;
	return ( FKCW_Network_UDPSocketHub* )p->autorelease();
}
//--------------------------------------------------------------------
MULTI_ENCRYPT_FUNC FKCW_Network_UDPSocketHub::GetEncryptFunc()
{
	return s_EncryptFunc;
}
//--------------------------------------------------------------------
MULTI_DECRYPT_FUNC FKCW_Network_UDPSocketHub::GetDecryptFunc()
{
	return s_DecryptFunc;
}
//--------------------------------------------------------------------
// 创建套接字并将其加入管理中心
/* 参数：
		p_strHostName 远程主机名或IPv4 IP地址
		p_nPort 远程端口
		p_nTag 套接字标签
		p_nBlockSec 套接字连接时的等待阻塞时间，0表示无限等待
*/
FKCW_Network_UDPSocket*	FKCW_Network_UDPSocketHub::CreateSocket(const string& p_strHostName, int p_nPort, int p_nTag, int p_nBlockSec )
{
	FKCW_Network_UDPSocket* p = FKCW_Network_UDPSocket::Create( p_strHostName, p_nPort, p_nTag, p_nBlockSec );
	if( p )
	{
		_AddSocket( p );
	}
	return p;
}
//--------------------------------------------------------------------
// 断开一个socket
void FKCW_Network_UDPSocketHub::Disconnect( int p_nTag )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecPackets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		if(s->getTag() == p_nTag) 
		{
			s->setStop(true);
			break;
		}
	}
}
//--------------------------------------------------------------------
// 停止全部socket
void FKCW_Network_UDPSocketHub::StopAll()
{
	// 释放socket
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		if(s->isConnected()) 
		{
			s->m_bIsConnected = false;
			s->__CloseSocket();
			CCNotificationCenter::sharedNotificationCenter()->postNotification(NotificationUDPSocketDisconnected, s);
		}
		s->setStop(true);
		s->setHub(NULL);
	}
	m_vecSockets.removeAllObjects();

	// 停止更新
	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleSelector(schedule_selector(FKCW_Network_UDPSocketHub::_MainLoop), this);
}
//--------------------------------------------------------------------
// 通过tag获取socket对象指针
FKCW_Network_UDPSocket* FKCW_Network_UDPSocketHub::GetSocket( int p_nTag )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		if(s->getTag() == p_nTag) 
		{
			return s;
		}
	}
	return NULL;
}
//--------------------------------------------------------------------
// 发送一个消息包
void FKCW_Network_UDPSocketHub::SendPacket( int p_nTag, FKCW_Network_Packet* p_pPacket )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_UDPSocket* s = (FKCW_Network_UDPSocket*)obj;
		if(s->getTag() == p_nTag) 
		{
			s->SendPacket(p_pPacket);
			break;
		}
	}
}
//--------------------------------------------------------------------