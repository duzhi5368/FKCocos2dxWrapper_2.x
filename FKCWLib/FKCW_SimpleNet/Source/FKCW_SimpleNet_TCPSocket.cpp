//-------------------------------------------------------------------------
#include "../Include/FKCW_SimpleNet_TCPSocket.h"
//-------------------------------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static bool bStartup = false;
#endif
//-------------------------------------------------------------------------
FKCW_SimpleNet_TCPSocket::FKCW_SimpleNet_TCPSocket()
	: m_uSocket(INVALID_SOCKET)
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	if( !bStartup ) 
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
		bStartup = true;
	}
#endif
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_TCPSocket::~FKCW_SimpleNet_TCPSocket()
{
	Close();
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_TCPSocket::Init()
{
	if( m_uSocket != INVALID_SOCKET )
	{
		Close();
	}

	m_uSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( m_uSocket == INVALID_SOCKET )
	{
		CCLOGERROR("create socket failed");
		return false;
	}

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	unsigned long ul = 1;
	int nRet = ioctlsocket(m_uSocket, FIONBIO, (unsigned long*)&ul);
	if( nRet == SOCKET_ERROR )
	{
		CCLOGERROR("set unblocking failed");
		Close();
		return false;
	}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	int nFlags = fcntl(m_uSocket, F_GETFL, 0);
	int nRet = fcntl(m_uSocket, F_SETFL, nFlags | O_NONBLOCK);
	if( nRet == SOCKET_ERROR )
	{
		CCLOGERROR("set unblocking failed");
		Close();
		return false;
	}
#endif

	int nNoDelay = 1;
	if( setsockopt(m_uSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR )
	{
		CCLOGERROR("set nodelay failed");
		Close();
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_TCPSocket::Close()
{
	if( m_uSocket != INVALID_SOCKET )
	{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		closesocket(m_uSocket);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		close(m_uSocket);        
#endif
		m_uSocket = INVALID_SOCKET;
	}
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_TCPSocket::Connect()
{
	CCAssert(strlen(m_oInetAddress.getIp()) && m_oInetAddress.getPort() != 0, "_connect");

	if( m_uSocket == INVALID_SOCKET )
	{
		if(!Init())
		{
			return false;
		}
	}

	int nRet = ::connect(m_uSocket, m_oInetAddress, m_oInetAddress.getLength());
	if( nRet == 0 )
	{
		return true;
	}
	else
	{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		int nError = WSAGetLastError();
		if( nError ==  WSAEWOULDBLOCK )
		{
			return true;
		}
		else
		{
			return false;
		}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		if( nRet == SOCKET_ERROR && errno == EINPROGRESS )
		{
			return true;
		}
		else
		{
			return false;
		}
#endif
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_TCPSocket::Disconnect()
{
	if( m_uSocket != INVALID_SOCKET )
	{
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
//		shutdown(m_uSocket, SD_BOTH);
//#endif
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
//		shutdown(m_uSocket, SHUT_RDWR);
//#endif
		Close();
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_TCPSocket::SetInetAddress(const FKCW_SimpleNet_InetAddress& oInetAddress)
{
	m_oInetAddress = oInetAddress;
}
//-------------------------------------------------------------------------
int FKCW_SimpleNet_TCPSocket::Read(char* _buff, int _len)
{
	if( m_uSocket == INVALID_SOCKET )
		return eSocketIoError;

	return ::recv(m_uSocket, _buff, _len, 0);
}
//-------------------------------------------------------------------------
int FKCW_SimpleNet_TCPSocket::Write(char* _buff, int _len)
{
	if( m_uSocket == INVALID_SOCKET )
		return eSocketIoError;

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return ::send(m_uSocket, _buff, _len, 0);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return ::send(m_uSocket, _buff, _len, SO_NOSIGPIPE);
#endif
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_TCPSocket::IsReadable()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), &fd, NULL, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_TCPSocket::IsWritable()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
int FKCW_SimpleNet_TCPSocket::IsConnected()
{
	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

			return eSocketConnected;

#elif( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

			int nError;
			socklen_t len = sizeof(nError);
			if( getsockopt(m_uSocket, SOL_SOCKET, SO_ERROR, &nError, &len) < 0 )
			{
				return eSocketConnectFailed;
			}
			if( nError == ECONNREFUSED || nError == ETIMEDOUT )
			{
				return eSocketConnectFailed;
			}
			return eSocketConnected;

#endif
		}
	}
	return eSocketConnecting;
}
//-------------------------------------------------------------------------