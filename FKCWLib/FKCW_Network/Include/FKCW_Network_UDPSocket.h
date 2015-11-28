/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network_UDPSocket
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
#include "FKCW_Network_NetworkCommon.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
//--------------------------------------------------------------------
class FKCW_Network_Packet;
class FKCW_Network_UDPSocketHub;
//--------------------------------------------------------------------
class FKCW_Network_UDPSocket : public CCObject
{
	friend class FKCW_Network_UDPSocketHub;
private:
	char			m_szInBuf[SocketInputBufferDefaultSize];				// 读取环状缓存
	int				m_nInBufLen;											// 读取缓存中的有效数据长度
	int				m_nBlockSec;											// 等待socket连接的阻塞时间
	pthread_mutex_t	m_Mutex;												// 线程互斥量
	CC_SYNTHESIZE_READONLY(int, m_nSocket, Socket);							// socket句柄
	CC_SYNTHESIZE_READONLY(int, m_nTag, Tag);								// socket标签
	CC_SYNTHESIZE(FKCW_Network_UDPSocketHub*, m_pHub, Hub);					// 管理中心指针
	CC_SYNTHESIZE_READONLY_BOOL(m_bIsConnected, Connected);					// 是否连接
	CC_SYNTHESIZE_BOOL(m_bIsStop, Stop);									// 是否断开
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, m_szHostname, Hostname);		// 服务器名
	CC_SYNTHESIZE_READONLY(int, m_nPort, Port);								// 服务器端口
	CC_SYNTHESIZE_PASS_BY_REF(sockaddr_in, m_tagSrvAddr, ServerAddress);	// 服务器地址
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCArray, m_arrSendQueue, SendQueue);	// 发送队列
private:
	// 我们在这里等待直到socket连接成功或者超时
	static void*	__UdpThreadEntry( void* p_pArg );
	// 从socket接收消息直到没有数据，或者缓冲区满了，或者出现了某种错误
	void			__RecvFromSocket();
	// 判断是否出现了错误
	bool			__HasError();
	// 关闭套接字
	void			__CloseSocket();
protected:
	// 初始化套接字
	/* 参数：
			p_strHostName 远程主机名或IPv4 IP地址
			p_nPort 远程端口
			p_nTag 套接字标签
			p_nBlockSec 套接字连接时的等待阻塞时间，0表示无限等待
	*/

	bool			_Init( const string& p_strHostName, int p_nPort, int p_nTag = -1,
		int p_nBlockSec = SocketDefaultTimeout );
public:
	FKCW_Network_UDPSocket();
	virtual ~FKCW_Network_UDPSocket();
	// 创建套接字实例
	// 参数参见 _Init()
	static FKCW_Network_UDPSocket* Create( const string& p_strHostName, int p_nPort, int p_nTag = -1,
		int p_nBlockSec = SocketDefaultTimeout );
	// 添加一个包到发送队列
	void			SendPacket( FKCW_Network_Packet* p_pPacket );
};