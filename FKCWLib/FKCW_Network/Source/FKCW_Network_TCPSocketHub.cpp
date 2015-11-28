//--------------------------------------------------------------------
#include "../Include/FKCW_Network_TCPSocketHub.h"
//--------------------------------------------------------------------
static MULTI_ENCRYPT_FUNC s_EncryptFunc = NULL;
static MULTI_DECRYPT_FUNC s_DecryptFunc = NULL;
//--------------------------------------------------------------------
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	#pragma comment(lib, "Ws2_32.lib")
#endif
//-------------------------------------------------------------------------
FKCW_Network_TCPSocketHub::FKCW_Network_TCPSocketHub()
	: m_bIsRawPolicy( false )
{
	// 初始化WSA
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
	pthread_mutex_init( &m_Mutex, NULL );

	// 开启主循环
	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->scheduleSelector(schedule_selector(FKCW_Network_TCPSocketHub::_MainLoop), this, 0, false);
}
//--------------------------------------------------------------------
// 监听套接字，并进行读写操作
void FKCW_Network_TCPSocketHub::_MainLoop( float p_fDelta )
{
	pthread_mutex_lock(&m_Mutex);

	// 通知中心
	CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();

	// 链接消息
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecConnectedSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		nc->postNotification( NotificationTCPSocketConnected, s);
	}
	m_vecConnectedSockets.removeAllObjects();

	// 数据事件
	CCARRAY_FOREACH(&m_vecPackets, obj) 
	{
		FKCW_Network_Packet* p = (FKCW_Network_Packet*)obj;
		nc->postNotification( NotificationPacketReceived, p);
	}
	m_vecPackets.removeAllObjects();

	// 断开链接消息
	CCARRAY_FOREACH(&m_vecDisConnectedSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		nc->postNotification( NotificationTCPSocketDisconnected, s);
		m_vecSockets.removeObject(s);
	}
	m_vecDisConnectedSockets.removeAllObjects();

	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 添加一个套接字到管理中心
bool FKCW_Network_TCPSocketHub::_AddSocket( FKCW_Network_TCPSocket* p_pSocket )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
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
void FKCW_Network_TCPSocketHub::_OnSocketConnectedThreadSafe( FKCW_Network_TCPSocket* p_pSocket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecConnectedSockets.addObject(p_pSocket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 提供给socket调用的函数，当它断开连接时调用该函数通知socket管理中心
void FKCW_Network_TCPSocketHub::_OnSocketDisConnectedThreadSafe( FKCW_Network_TCPSocket* p_pSocket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecDisConnectedSockets.addObject(p_pSocket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 提供给socket调用的函数，当它准备发送一个包时调用该函数通知socket管理中心
void FKCW_Network_TCPSocketHub::_OnPacketReceivedThreadSafe( FKCW_Network_Packet* p_pPacket )
{
	pthread_mutex_lock(&m_Mutex);
	m_vecPackets.addObject(p_pPacket);
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
FKCW_Network_TCPSocketHub::~FKCW_Network_TCPSocketHub()
{
	// 释放锁
	pthread_mutex_destroy( &m_Mutex );
}
//--------------------------------------------------------------------
FKCW_Network_TCPSocketHub* FKCW_Network_TCPSocketHub::Create( MULTI_ENCRYPT_FUNC p_pEncryptFunc, MULTI_DECRYPT_FUNC p_pDecryptFunc )
{
	FKCW_Network_TCPSocketHub* h = new FKCW_Network_TCPSocketHub();
	s_EncryptFunc = p_pEncryptFunc;
	s_DecryptFunc = p_pDecryptFunc;
	return (FKCW_Network_TCPSocketHub*)h->autorelease();
}
//--------------------------------------------------------------------
MULTI_ENCRYPT_FUNC FKCW_Network_TCPSocketHub::GetEncryptFunc()
{
	return s_EncryptFunc;
}
//--------------------------------------------------------------------
MULTI_DECRYPT_FUNC FKCW_Network_TCPSocketHub::GetDecryptFunc()
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
p_bIsKeepAlive 是否保持该套接字存活
*/
FKCW_Network_TCPSocket*	FKCW_Network_TCPSocketHub::CreateSocket(const string& p_strHostName, int p_nPort, int p_nTag,
									 int p_nBlockSec, bool p_bIsKeepAlive )
{
	FKCW_Network_TCPSocket* s = FKCW_Network_TCPSocket::Create(
		p_strHostName, p_nPort, p_nTag, p_nBlockSec, p_bIsKeepAlive );
	if(s)
	{
		_AddSocket(s);
	}
	return s;
}
//--------------------------------------------------------------------
// 断开一个socket
void FKCW_Network_TCPSocketHub::Disconnect( int p_nTag )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecPackets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		if(s->getTag() == p_nTag) 
		{
			s->setStop(true);
			break;
		}
	}
}
//--------------------------------------------------------------------
// 停止全部socket
void FKCW_Network_TCPSocketHub::StopAll()
{
	// 释放socket
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		if(s->isConnected()) 
		{
			s->m_bIsConnected = false;
			s->__CloseSocket();
			CCNotificationCenter::sharedNotificationCenter()->postNotification( NotificationTCPSocketDisconnected, s);
		}
		s->setStop(true);
		s->setHub(NULL);
	}
	m_vecSockets.removeAllObjects();

	// 停止更新
	CCScheduler* s = CCDirector::sharedDirector()->getScheduler();
	s->unscheduleSelector(schedule_selector(FKCW_Network_TCPSocketHub::_MainLoop), this);
}
//--------------------------------------------------------------------
// 通过tag获取socket对象指针
FKCW_Network_TCPSocket*	FKCW_Network_TCPSocketHub::GetSocket( int p_nTag )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		if(s->getTag() == p_nTag) 
		{
			return s;
		}
	}
	return NULL;
}
//--------------------------------------------------------------------
// 发送一个消息包
void FKCW_Network_TCPSocketHub::SendPacket( int p_nTag, FKCW_Network_Packet* p_pPacket )
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecSockets, obj) 
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		if(s->getTag() == p_nTag)
		{
			s->SendPacket(p_pPacket);
			break;
		}
	}
}
//--------------------------------------------------------------------