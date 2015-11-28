//-------------------------------------------------------------------------
#include "FKNetworkCommand.h"
//-------------------------------------------------------------------------
FKNetworkCommand::FKNetworkCommand()
	: m_unType( 0 )
	, m_unSocketID( 0 )
	, m_unSocket( 0 )
	, m_unLen( 0 )
{

}
//-------------------------------------------------------------------------
FKNetworkCommand::FKNetworkCommand( const FKNetworkCommand& other )
{
	m_unType		= other.m_unType;
	m_unSocket		= other.m_unSocket;
	m_unSocketID	= other.m_unSocketID;
	m_unLen			= other.m_unLen;
	CC_ASSERT( m_unLen != 0 );
	m_szMsg.resize( m_unLen );

	if( m_unLen <= 0 || m_unLen > SOCKET_READ_BUFFER_SIZE )
		return;

	m_szMsg.clear();
	if( m_szMsg.capacity() >= m_unLen )
	{
		std::copy( other.m_szMsg.begin(), other.m_szMsg.end(),
			m_szMsg.begin() );
	}
	else
	{
		// 分配内存失败,不做copy，丢弃本数据
		m_unType	= eCNMT_Error;
		m_unSocket	= eCNET_NotEnoughMemory;
		m_unLen		= 0;
	}
}
//-------------------------------------------------------------------------
FKNetworkCommand& FKNetworkCommand::operator= ( const FKNetworkCommand& other )
{
	m_unType		= other.m_unType;
	m_unSocket		= other.m_unSocket;
	m_unSocketID	= other.m_unSocketID;
	m_unLen			= other.m_unLen;
	CC_ASSERT( m_unLen != 0 );
	m_szMsg.resize( m_unLen );

	if( m_unLen <= 0 || m_unLen > SOCKET_READ_BUFFER_SIZE )
	{
		return *this;
	}

	m_szMsg.clear();
	if( m_szMsg.capacity() >= m_unLen )
	{
		std::copy( other.m_szMsg.begin(), other.m_szMsg.end(),
			m_szMsg.begin() );
	}
	else
	{
		// 分配内存失败,不做copy，丢弃本数据
		m_unType	= eCNMT_Error;
		m_unSocket	= eCNET_NotEnoughMemory;
		m_unLen		= 0;
	}

	return *this;
}
//-------------------------------------------------------------------------