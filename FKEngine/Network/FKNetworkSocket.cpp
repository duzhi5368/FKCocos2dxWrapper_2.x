//-------------------------------------------------------------------------
#include "FKNetworkSocket.h"
#include "FKNetworkSystem.h"
//-------------------------------------------------------------------------
FKNetworkSocket::FKNetworkSocket( unsigned int unSocketID, INetworkSystem* pSystem )
{
	m_unSocketID	= unSocketID;
	m_pOwner		= pSystem;
	m_pConnecter	= new FKNetworkSocketConnecter( this );
}
//-------------------------------------------------------------------------
FKNetworkSocket::~FKNetworkSocket()
{
	CC_SAFE_DELETE( m_pConnecter );
}
//-------------------------------------------------------------------------
// 客户端接收到数据
void FKNetworkSocket::OnRecv( const char* szBuf, int nLen )
{
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->PushRecvMsg( m_unSocketID, szBuf, nLen );
}
//-------------------------------------------------------------------------
// 连接成功消息
void FKNetworkSocket::OnConnencted()
{
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;
	
	p->PushConnectSuccessMsg( m_unSocketID, this );
}
//-------------------------------------------------------------------------
// 连接关闭消息
void FKNetworkSocket::OnDisConnected()
{
	// 通知system
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->PushCloseMsg( m_unSocketID );
}
//-------------------------------------------------------------------------
// 连接超时消息
void FKNetworkSocket::OnConnectTimeout()
{
	// 通知system
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->PushConnectErrorMsg( m_unSocketID, eCNET_ConnectTimeout, gs_ClientNetErrorInfo[eCNET_ConnectTimeout].c_str() );
}
//-------------------------------------------------------------------------
// 连接错误消息
void FKNetworkSocket::OnConnectError( int nErrorCode, const char* strErrorMsg )
{
	CloseConnect();

	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->PushConnectErrorMsg( m_unSocketID, nErrorCode, strErrorMsg );
}
//-------------------------------------------------------------------------
// 释放清理
void FKNetworkSocket::Release()
{
	CloseConnect();
}
//-------------------------------------------------------------------------
// 连接服务器
bool FKNetworkSocket::Connect( const char* szServerIP, unsigned short usPort )
{
	if( m_pConnecter == NULL )
		return false;
	FKCW_SimpleNet_InetAddress oAddres;
	oAddres.setIp( szServerIP );
	oAddres.setPort( usPort );
	m_pConnecter->setInetAddress( oAddres );
	m_pConnecter->setSoTimeout( gs_MAX_TIMEOUT_TIME );
	return m_pConnecter->connect();
}
//-------------------------------------------------------------------------
// 主动关闭连接
void FKNetworkSocket::CloseConnect()
{
	if( m_pConnecter == NULL )
		return;
	m_pConnecter->disconnect();

	// 通知system
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->PushCloseMsg( m_unSocketID );
}
//-------------------------------------------------------------------------
// 发送数据
bool FKNetworkSocket::Send( const char* szBuf, int nLen )
{
	if( m_pConnecter == NULL )
		return false;
	m_pConnecter->send( const_cast<char*>(szBuf), nLen );
	return true;
}
//-------------------------------------------------------------------------
// 是否当前处于连接中
bool FKNetworkSocket::IsConnecting()
{
	if( m_pConnecter == NULL )
		return false;
	return m_pConnecter->isConnected();
}
//-------------------------------------------------------------------------
// 获取ID
int FKNetworkSocket::GetSocketID()
{
	return m_unSocketID;
}
//-------------------------------------------------------------------------
// 设置消息回调处理
void FKNetworkSocket::SetMsgHandler( INetworkMsgHandler* pMsgHandler )
{
	if ( m_pOwner == NULL )
		return;
	FKNetworkSystem* p = dynamic_cast<FKNetworkSystem*>( m_pOwner );
	if( p == NULL )
		return;

	p->SetMsgHandler( m_unSocketID, pMsgHandler );
}
//-------------------------------------------------------------------------