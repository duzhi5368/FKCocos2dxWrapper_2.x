//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	INetworkSystem.h
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
class INetworkSocket;
// 消息回调类
class INetworkMsgHandler
{
public:
	// 客户端接收到数据
	virtual void			OnRecv( const char* szBuf, int nLen ) = 0;
	// 连接成功消息
	virtual void			OnConnencted( INetworkSocket* pSocket ) = 0;
	// 连接关闭消息
	virtual void			OnClose() = 0;
	// 连接错误消息
	virtual void			OnConnectError( int nErrorCode, const char* strErrorMsg ) = 0;
};
//-------------------------------------------------------------------------
// 单一网络socket
class INetworkSocket
{
public:
	// 释放清理
	virtual void			Release() = 0;
	// 连接服务器
	virtual bool			Connect( const char* szServerIP, unsigned short usPort ) = 0;
	// 主动关闭连接
	virtual void			CloseConnect() = 0;
	// 发送数据
	virtual bool			Send( const char* szBuf, int nLen ) = 0;
	// 是否当前处于连接中
	virtual bool			IsConnecting() = 0;
	// 获取ID
	virtual int				GetSocketID() = 0;

	// 设置消息回调处理
	virtual void			SetMsgHandler( INetworkMsgHandler* pMsgHandler ) = 0;
};
//-------------------------------------------------------------------------
// 网络模块系统
class INetworkSystem
{
public:
	// 清理网络系统
	virtual void			Release() = 0;
	// 创建客户端连接socket
	virtual INetworkSocket*	CreateSocket( INetworkMsgHandler* pMsgHandler ) = 0;
	// 清理客户端指定连接
	virtual void			ClearSocketByID( int nSocketID ) = 0;
	// 设置指定连接的回调
	virtual void			SetMsgHandler( int nSocketID, INetworkMsgHandler* pMsgHandler ) = 0;

	// 逻辑更新
	virtual void			UpdateEvent() = 0;
};
//-------------------------------------------------------------------------
// 获取网络接口
extern INetworkSystem* GetNetworkSystem();
//-------------------------------------------------------------------------