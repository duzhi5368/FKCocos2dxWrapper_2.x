//-------------------------------------------------------------------------
#include "FKNetworkSocketConnecter.h"
#include "FKNetworkSocket.h"
#include "FKNetworkMacros.h"
//-------------------------------------------------------------------------
FKNetworkSocketConnecter::FKNetworkSocketConnecter( INetworkSocket* pOwner )
{
	m_pOwner = pOwner;
}
//-------------------------------------------------------------------------
// 收到消息包的回调
void FKNetworkSocketConnecter::onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer)
{
	if( m_pOwner == NULL )
		return;
	FKNetworkSocket* p = dynamic_cast<FKNetworkSocket*>( m_pOwner );
	if( p )
		p->OnRecv( oBuffer.data(), oBuffer.length() );
}
//-------------------------------------------------------------------------
// 连接成功时的回调
void FKNetworkSocketConnecter::onConnected()
{
	if( m_pOwner == NULL )
		return;

	FKNetworkSocket* p = dynamic_cast<FKNetworkSocket*>( m_pOwner );
	if( p )
		p->OnConnencted();
}
//-------------------------------------------------------------------------
// 连接超时时的回调
void FKNetworkSocketConnecter::onConnectTimeout()
{
	if( m_pOwner == NULL )
		return;

	FKNetworkSocket* p = dynamic_cast<FKNetworkSocket*>( m_pOwner );
	if( p )
		p->OnConnectTimeout();
}
//-------------------------------------------------------------------------
// 连接失败时的回调
void FKNetworkSocketConnecter::onDisconnected()
{
	if( m_pOwner == NULL )
		return;

	FKNetworkSocket* p = dynamic_cast<FKNetworkSocket*>( m_pOwner );
	if( p )
		p->OnDisConnected();
}
//-------------------------------------------------------------------------
// 出错时的回调
void FKNetworkSocketConnecter::onExceptionCaught(ENUM_SocketStatus eStatus)
{
	if( m_pOwner == NULL )
		return;

	FKNetworkSocket* p = dynamic_cast<FKNetworkSocket*>( m_pOwner );
	if( p == NULL )
		return;

	if( eStatus == eSocketIoError )
	{
		p->OnConnectError( eCNET_IoError, gs_ClientNetErrorInfo[eCNET_IoError].c_str() );
	}
	else
	{
		p->OnConnectError( eCNET_Unknown, gs_ClientNetErrorInfo[eCNET_Unknown].c_str() );
	}
}
//-------------------------------------------------------------------------