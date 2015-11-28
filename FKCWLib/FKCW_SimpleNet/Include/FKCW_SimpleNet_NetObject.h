//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_SimpleNet_NetObject.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SimpleNet_Macro.h"
#include "FKCW_SimpleNet_Buffer.h"
#include "FKCW_SimpleNet_InetAddress.h"
#include "FKCW_SimpleNet_TCPSocket.h"
//-------------------------------------------------------------------------
// 该类需要被继承，继承之后就实现具体函数就可以负责网络连接和消息处理
class FKCW_SimpleNet_NetObject : public CCObject
{
public:
	FKCW_SimpleNet_NetObject();
	virtual ~FKCW_SimpleNet_NetObject();

public:
	// 收到消息包的回调
	virtual void onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer) = 0;
	// 连接成功时的回调
	virtual void onConnected() = 0;
	// 连接超时时的回调
	virtual void onConnectTimeout() = 0;
	// 连接失败时的回调
	virtual void onDisconnected() = 0;
	// 出错时的回调
	virtual void onExceptionCaught(ENUM_SocketStatus eStatus) = 0;

public:
	// 设置连接目标地址
	void setInetAddress(const FKCW_SimpleNet_InetAddress& oInetAddress);
	// 获取连接目标地址
	const FKCW_SimpleNet_InetAddress& getInetAddress() const;
	// 设置连接超时时间
	void setSoTimeout(float fSoTimeout);
	// 获取连接超时时间
	float getSoTimeout() const;
	// 发送消息包给目标
	void send(char* pBuffer, unsigned int uLen);
	// 发送消息包给目标
	void send(FKCW_SimpleNet_Buffer* pBuffer);
	// 检查当前是否保持连接
	bool isConnected();
	// 关闭当前连接
	void close();
	// 连接目标地址
	bool connect();
	// 断开当前连接
	void disconnect();
	// 获取当前连接状态
	ENUM_SocketStatus getSocketStatus(){ return m_eStatus; }

private:
	// 每帧读取数据
	bool runRead();
	// 每帧检查是否有数据发送，若有则进行发送
	bool runWrite();
	// 每帧回调更新函数
	void runSchedule(float dt);
	// 注册帧回调函数
	void registerScheduler();
	// 注销帧回调函数
	void unregisterScheduler();

private:
	// 发送消息包定义
	struct SSendBuffer
	{
		char* pBuffer;       // 数据信息
		int nOffset;         // 发送数据偏移
		int nLength;         // 数据长
	};
	
private:
	bool							m_bRunSchedule;
	float							m_fConnectingDuration;
	float							m_fSoTimeout;
	std::list<SSendBuffer>			m_lSendBuffers;
	FKCW_SimpleNet_Buffer			m_oReadBuffer;
	FKCW_SimpleNet_InetAddress		m_oInetAddress;
	FKCW_SimpleNet_TCPSocket		m_oSocket;
	char							m_pReadBuffer[SOCKET_READ_BUFFER_SIZE];
	ENUM_SocketStatus				m_eStatus;
};