//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	FKNetworkSocket.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKNetworkSocketConnecter.h"
#include "FKNetworkMacros.h"
//-------------------------------------------------------------------------
class FKNetworkSocket : public INetworkSocket
{
	friend class FKNetworkSocketConnecter;
public:
	FKNetworkSocket( unsigned int unSocketID, INetworkSystem* pSystem );
	~FKNetworkSocket();
public:
	// 释放清理
	virtual void			Release();
	// 连接服务器
	virtual bool			Connect( const char* szServerIP, unsigned short usPort );
	// 主动关闭连接
	virtual void			CloseConnect();
	// 发送数据
	virtual bool			Send( const char* szBuf, int nLen );
	// 是否当前处于连接中
	virtual bool			IsConnecting();
	// 获取ID
	virtual int				GetSocketID();
	// 设置消息回调处理
	virtual void			SetMsgHandler( INetworkMsgHandler* pMsgHandler );
private:
	// 客户端接收到数据
	void OnRecv( const char* szBuf, int nLen );
	// 连接成功消息
	void OnConnencted();
	// 连接关闭消息
	void OnDisConnected();
	// 连接超时消息
	void OnConnectTimeout();
	// 连接错误消息
	void OnConnectError( int nErrorCode, const char* strErrorMsg );
private:
	unsigned int				m_unSocketID;		// 本socket唯一编号
	INetworkSystem*				m_pOwner;			// 网络管理器
	FKNetworkSocketConnecter*	m_pConnecter;		// 网络连接器
};