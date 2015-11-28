/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network_TCPSocketHub
	author:		FreeKnight
	
	purpose:	管理一组TCPsocket
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
#include "FKCW_Network_TCPSocket.h"
#include "FKCW_Network_Packet.h"
#include "FKCW_Network_NetworkCommon.h"
//--------------------------------------------------------------------
// 对象是一个连接中的套接字
#define NotificationTCPSocketConnected		"NotificationTCPSocketConnected"
// 对象是一个断开的套接字
#define NotificationTCPSocketDisconnected	"NotificationTCPSocketDisconnected"
// 对象是一个包
#define NotificationPacketReceived			"NotificationPacketReceived"
//--------------------------------------------------------------------
// 管理多个socket的管理器。当该管理器被创建，它用来保证每个socket每帧进行更新
class FKCW_Network_TCPSocketHub : public CCObject
{
	friend class FKCW_Network_TCPSocket;
private:
	pthread_mutex_t			m_Mutex;					// 线程临界区锁
	CCArray					m_vecConnectedSockets;		// 连接中的套接字队列
	CCArray					m_vecDisConnectedSockets;	// 断开的套接字队列
	CCArray					m_vecPackets;				// 消息包队列
	CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_vecSockets, Sockets);		// 套接字队列
	CC_SYNTHESIZE_BOOL(m_bIsRawPolicy, RawPolicy);		// 如果为true，则表示收到的包不存在预设的包头，将由开发者自行解析本包
protected:
	FKCW_Network_TCPSocketHub();

	// 监听套接字，并进行读写操作
	void			_MainLoop( float p_fDelta );
	// 添加一个套接字到管理中心
	bool			_AddSocket( FKCW_Network_TCPSocket* p_pSocket );
	// 提供给socket调用的函数，当它成功建立连接时调用该函数通知socket管理中心
	void			_OnSocketConnectedThreadSafe( FKCW_Network_TCPSocket* p_pSocket );
	// 提供给socket调用的函数，当它断开连接时调用该函数通知socket管理中心
	void			_OnSocketDisConnectedThreadSafe( FKCW_Network_TCPSocket* p_pSocket );
	// 提供给socket调用的函数，当它准备发送一个包时调用该函数通知socket管理中心
	void			_OnPacketReceivedThreadSafe( FKCW_Network_Packet* p_pPacket );

public:
	virtual ~FKCW_Network_TCPSocketHub();
	static FKCW_Network_TCPSocketHub* Create( MULTI_ENCRYPT_FUNC p_pEncryptFunc = NULL,
		MULTI_DECRYPT_FUNC p_pDecryptFunc = NULL );
	static MULTI_ENCRYPT_FUNC		GetEncryptFunc();
	static MULTI_DECRYPT_FUNC		GetDecryptFunc();

	// 创建套接字并将其加入管理中心
	/* 参数：
			p_strHostName 远程主机名或IPv4 IP地址
			p_nPort 远程端口
			p_nTag 套接字标签
			p_nBlockSec 套接字连接时的等待阻塞时间，0表示无限等待
			p_bIsKeepAlive 是否保持该套接字存活
	*/
	FKCW_Network_TCPSocket*	CreateSocket(const string& p_strHostName, int p_nPort, int p_nTag,
		int p_nBlockSec = SocketDefaultTimeout, bool p_bIsKeepAlive = false );
	// 断开一个socket
	void			Disconnect( int p_nTag );
	// 停止全部socket
	void			StopAll();
	// 通过tag获取socket对象指针
	FKCW_Network_TCPSocket*		GetSocket( int p_nTag );
	// 发送一个消息包
	void			SendPacket( int p_nTag, FKCW_Network_Packet* p_pPacket );
};