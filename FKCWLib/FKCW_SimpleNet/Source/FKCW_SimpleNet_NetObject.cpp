//-------------------------------------------------------------------------
#include "../Include/FKCW_SimpleNet_NetObject.h"
//-------------------------------------------------------------------------
FKCW_SimpleNet_NetObject::FKCW_SimpleNet_NetObject()
	: m_fSoTimeout(SOCKET_SOTIMEOUT)
	, m_eStatus(eSocketIoClosed)
	, m_fConnectingDuration(0.0f)
	, m_bRunSchedule(false)
{

}
//-------------------------------------------------------------------------
FKCW_SimpleNet_NetObject::~FKCW_SimpleNet_NetObject()
{
	m_oSocket.Close();

	while(!m_lSendBuffers.empty())
	{
		CC_SAFE_DELETE_ARRAY(m_lSendBuffers.front().pBuffer);
		m_lSendBuffers.pop_front();
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::setInetAddress(const FKCW_SimpleNet_InetAddress& oInetAddress)
{
	m_oInetAddress = oInetAddress;
}
//-------------------------------------------------------------------------
const FKCW_SimpleNet_InetAddress& FKCW_SimpleNet_NetObject::getInetAddress() const
{
	return m_oInetAddress;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::setSoTimeout(float fSoTimeout)
{
	m_fSoTimeout = fSoTimeout;
}
//-------------------------------------------------------------------------
float FKCW_SimpleNet_NetObject::getSoTimeout() const
{
	return m_fSoTimeout;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::send(char* pBuffer, unsigned int uLen)
{
	if( !pBuffer || uLen == 0 || !isConnected() )
		return;

#if USING_PACKAGE_HEAD_LENGTH
	FKCW_SimpleNet_Buffer* pBuf = new FKCW_SimpleNet_Buffer(pBuffer, uLen);
	pBuf->autorelease();
	send(pBuf);
#else
	char* pTemp = new char[uLen];
	memcpy(pTemp, pBuffer, uLen);

	SSendBuffer tBuf;
	tBuf.pBuffer = pTemp;
	tBuf.nLength = (int)uLen;
	tBuf.nOffset = 0;

	m_lSendBuffers.push_back(tBuf);
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::send(FKCW_SimpleNet_Buffer* pBuffer)
{
	if( pBuffer->empty() || !isConnected() )
		return;

#if USING_PACKAGE_HEAD_LENGTH
	unsigned int u_len = pBuffer->length();
	pBuffer->moveRight(sizeof(unsigned int));
	pBuffer->moveWriterIndexToFront();
	pBuffer->writeUInt(u_len);
#endif

	pBuffer->moveReaderIndexToFront();
	char* pData = pBuffer->readWholeData();
	int nLength = (int)pBuffer->length();
	pBuffer->moveReaderIndexToFront();

	SSendBuffer tBuf;
	tBuf.pBuffer = pData;
	tBuf.nLength = nLength;
	tBuf.nOffset = 0;

	m_lSendBuffers.push_back(tBuf);
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_NetObject::isConnected()
{
	return m_eStatus == eSocketConnected;
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_NetObject::connect()
{
	if( m_eStatus != eSocketConnected && m_eStatus != eSocketConnecting )
	{
		m_oSocket.SetInetAddress(m_oInetAddress);
		if( m_oSocket.Connect() )
		{
			registerScheduler();
			m_eStatus = eSocketConnecting;
			return true;
		}
		else
		{
			m_oSocket.Close();
			m_eStatus = eSocketConnectFailed;
			onExceptionCaught(eSocketConnectFailed);
		}
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::disconnect()
{
	if( m_eStatus == eSocketConnected )
	{
		unregisterScheduler();
		m_oSocket.Disconnect();
		m_eStatus = eSocketDisconnected;
		onDisconnected();
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::close()
{
	if( m_eStatus == eSocketConnected )
	{
		unregisterScheduler();
		m_oSocket.Close();
		m_eStatus = eSocketIoClosed;
		onDisconnected();
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::runSchedule(float dt)
{
	switch( m_eStatus )
	{
	case eSocketConnecting:
		{
			switch( m_oSocket.IsConnected() )
			{
			case eSocketConnected:
				{
					m_eStatus = eSocketConnected;
					onConnected();
				}
				break;
			case eSocketConnectFailed:
				{
					unregisterScheduler();
					m_oSocket.Close();
					m_eStatus = eSocketConnectFailed;
					onExceptionCaught(eSocketConnectFailed);
				}
				break;
			case eSocketConnecting:
				{
					if( m_fConnectingDuration > m_fSoTimeout )
					{
						unregisterScheduler();
						m_oSocket.Disconnect();
						m_eStatus = eSocketDisconnected;
						onConnectTimeout();
						m_fConnectingDuration = 0.0f;
					}
					else
					{
						m_fConnectingDuration += dt;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case eSocketConnected:
		{
#if HANDLE_ON_SINGLE_FRAME
			while( m_oSocket.IsReadable() )
#else
			if( m_oSocket.ccIsReadable() )
#endif
			{
				if( this->runRead() ) return;
			}

#if HANDLE_ON_SINGLE_FRAME
			while( m_oSocket.IsWritable() && !m_lSendBuffers.empty() )
#else
			if( m_oSocket.ccIsWritable() && !m_lSendBuffers.empty() )
#endif
			{
				if( this->runWrite() ) return;
			}
		}
		break;
	default:
		break;
	}	
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_NetObject::runRead()
{
	int nRet = m_oSocket.Read(m_pReadBuffer, SOCKET_READ_BUFFER_SIZE);
	if( nRet == eSocketIoError || nRet == eSocketIoClosed )
	{
		unregisterScheduler();
		m_oSocket.Close();
		m_eStatus = eSocketIoClosed;
		onDisconnected();
		return true;
	}
	else
	{
#if 1
		CCLOG("FKCW_SimpleNet_Socket ReadBytes = %d", nRet);
#endif
		m_oReadBuffer.writeData(m_pReadBuffer, (unsigned int)nRet);
#if USING_PACKAGE_HEAD_LENGTH
		while( m_oReadBuffer.isReadable(sizeof(int)) )
		{
			m_oReadBuffer.moveReaderIndexToFront();
			int n_head_len = 0;
			n_head_len = m_oReadBuffer.readInt();
			if( n_head_len <= 0 )
			{
				CCLOGERROR("invalidate head length");
				m_oReadBuffer.moveLeft(sizeof(int));
			}

			int n_content_len = (int)m_oReadBuffer.length();
			if( n_content_len - (int)(sizeof(int)) >= n_head_len )
			{
				m_oReadBuffer.moveLeft(sizeof(unsigned int));
				FKCW_SimpleNet_Buffer* pData = m_oReadBuffer.readData(n_head_len);
				m_oReadBuffer.moveLeft(n_head_len);
				m_oReadBuffer.moveReaderIndexToFront();
				m_oReadBuffer.moveWriterIndexToBack();

				onMessageReceived(*pData);
			}
			else
			{
				break;
			}
		}
#else
		FKCW_SimpleNet_Buffer* pData = (FKCW_SimpleNet_Buffer*) m_oReadBuffer.copy();
		pData->autorelease();
		m_oReadBuffer.clear();

		onMessageReceived(*pData);
#endif
	}
	return false;
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_NetObject::runWrite()
{
	SSendBuffer& tBuffer = m_lSendBuffers.front();

	int nRet = m_oSocket.Write(tBuffer.pBuffer + tBuffer.nOffset, tBuffer.nLength - tBuffer.nOffset);
#if 1
	CCLOG("FKCW_SimpleNet_Socket WriteBytes = %d", nRet);
#endif
	if( nRet == eSocketIoError )
	{
		unregisterScheduler();
		m_oSocket.Close();
		m_eStatus = eSocketIoClosed;
		onDisconnected();
		return true;
	}
	else if( nRet == tBuffer.nLength - tBuffer.nOffset )
	{
		CC_SAFE_DELETE_ARRAY(tBuffer.pBuffer);
		m_lSendBuffers.pop_front();
	}
	else
	{
		tBuffer.nOffset += nRet;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::registerScheduler()
{
	if( m_bRunSchedule )
		return;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(FKCW_SimpleNet_NetObject::runSchedule), 
		this, 
		0.0f, 
		false
		);
	m_bRunSchedule = true;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_NetObject::unregisterScheduler()
{
	if( !m_bRunSchedule )
		return;

	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(FKCW_SimpleNet_NetObject::runSchedule),
		this
		);
	m_bRunSchedule = false;
}
//-------------------------------------------------------------------------