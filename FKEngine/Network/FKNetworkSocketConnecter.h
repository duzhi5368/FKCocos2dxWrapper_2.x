//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	FKNetworkSocketConnecter.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKNetworkSocketConnecter : public FKCW_SimpleNet_NetObject
{
public:
	FKNetworkSocketConnecter( INetworkSocket* pOwner );
public:
	// 收到消息包的回调
	virtual void onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer);
	// 连接成功时的回调
	virtual void onConnected();
	// 连接超时时的回调
	virtual void onConnectTimeout();
	// 连接失败时的回调
	virtual void onDisconnected();
	// 出错时的回调
	virtual void onExceptionCaught(ENUM_SocketStatus eStatus);
private:
	INetworkSocket*				m_pOwner;
};