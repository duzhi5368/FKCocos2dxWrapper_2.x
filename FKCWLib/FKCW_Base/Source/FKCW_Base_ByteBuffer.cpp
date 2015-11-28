//--------------------------------------------------------------------
#include "../Include/FKCW_Base_ByteBuffer.h"
//--------------------------------------------------------------------
#define DEFAULT_SIZE 0x1000
//--------------------------------------------------------------------
FKCW_Base_ByteBuffer::FKCW_Base_ByteBuffer()
	: m_pBuffer( NULL )
	, m_unReadPos( 0 )
	, m_unWritePos( 0 )
	, m_bExternal( false )
{
	_Reserve( DEFAULT_SIZE );
}
//--------------------------------------------------------------------
FKCW_Base_ByteBuffer::FKCW_Base_ByteBuffer( size_t p_unRes )
	: m_pBuffer( NULL )
	, m_unReadPos( 0 )
	, m_unWritePos( 0 )
	, m_bExternal( false )
{
	_Reserve( p_unRes );
}
//--------------------------------------------------------------------
FKCW_Base_ByteBuffer::FKCW_Base_ByteBuffer( const FKCW_Base_ByteBuffer& p_Other )
	: m_pBuffer( NULL )
	, m_unReadPos( 0 )
	, m_unWritePos( 0 )
	, m_bExternal( false )
{
	_Reserve( p_Other.m_unBufferSize );
	memcpy( p_Other.m_pBuffer, m_pBuffer, p_Other.m_unWritePos );
	m_unReadPos		= p_Other.m_unReadPos;
	m_unWritePos	= p_Other.m_unWritePos;
}
//--------------------------------------------------------------------
// 它包含了一个外部数据，而不是自己内分内存
// 参数：p_szBuffer 缓冲区 p_unBufSize 缓冲区大小 p_unDataLen 在缓冲区中的可用数据
FKCW_Base_ByteBuffer::FKCW_Base_ByteBuffer( const char* p_szBuffer, size_t p_unBufSize, size_t p_unDataLen )
	: m_pBuffer( (uint8*)p_szBuffer )
	, m_unReadPos( 0 )
	, m_unWritePos( p_unDataLen )
	, m_bExternal( true )
	, m_unBufferSize( p_unBufSize )
{

}
//--------------------------------------------------------------------
FKCW_Base_ByteBuffer::~FKCW_Base_ByteBuffer()
{
	if( !m_bExternal )
	{
		CC_SAFE_FREE( m_pBuffer );
	}
}
//--------------------------------------------------------------------
// 创建一个默认大小的 FKCW_Base_ByteBuffer
FKCW_Base_ByteBuffer* FKCW_Base_ByteBuffer::Create()
{
	FKCW_Base_ByteBuffer* p = new FKCW_Base_ByteBuffer();
	return (FKCW_Base_ByteBuffer*)p->autorelease();
}
//--------------------------------------------------------------------
// 创建一个指定大小的 FKCW_Base_ByteBuffer、
FKCW_Base_ByteBuffer* FKCW_Base_ByteBuffer::Create( size_t p_unRes )
{
	FKCW_Base_ByteBuffer* p = new FKCW_Base_ByteBuffer(p_unRes);
	return (FKCW_Base_ByteBuffer*)p->autorelease();
}
//--------------------------------------------------------------------
// 分配一个制定大小的内存
void FKCW_Base_ByteBuffer::_Reserve( size_t p_unRes )
{
	if( m_bExternal )
		return;

	if( m_pBuffer )
		m_pBuffer = (uint8*)realloc( m_pBuffer, p_unRes );
	else
		m_pBuffer = (uint8*)malloc( p_unRes );

	m_unBufferSize = p_unRes;
}
//--------------------------------------------------------------------
// 确保缓冲区足够大，能够写指定字节大小的数据
void FKCW_Base_ByteBuffer::_EnsureCanWrite( size_t p_unSize )
{
	size_t new_size = m_unWritePos + p_unSize;
	if(new_size > m_unBufferSize) {
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		_Reserve(new_size);
	}
}
//--------------------------------------------------------------------
// 重置读写指针位置
void FKCW_Base_ByteBuffer::Clear()
{
	m_unReadPos = m_unWritePos = 0;
}
//--------------------------------------------------------------------
// 返回缓冲区指针
const uint8* FKCW_Base_ByteBuffer::GetBuffer()
{
	return m_pBuffer;
}
//--------------------------------------------------------------------
// 获取可读缓冲大小
size_t FKCW_Base_ByteBuffer::GetAvailable()
{
	return m_unWritePos - m_unReadPos;
}
//--------------------------------------------------------------------
// 向前移动读指针
void FKCW_Base_ByteBuffer::Skip( size_t p_unLen )
{
	if(m_unReadPos + p_unLen > m_unWritePos)
		p_unLen = (m_unWritePos - m_unReadPos);
	m_unReadPos += p_unLen;
}
//--------------------------------------------------------------------
// 向后移动读指针
void FKCW_Base_ByteBuffer::Revoke( size_t p_unLen )
{
	m_unReadPos -= p_unLen;
	m_unReadPos = MAX(0, m_unReadPos);
}
//--------------------------------------------------------------------
// 移动当前数据到缓冲区首部
void FKCW_Base_ByteBuffer::Compact()
{
	if( m_unReadPos >  0 )
	{
		memmove( m_pBuffer, m_pBuffer + m_unReadPos, GetAvailable() );
		m_unWritePos -= m_unReadPos;
		m_unReadPos = 0;
	}
}
//--------------------------------------------------------------------
// 从缓冲区读取指定大小的数据，若读取Len超过了缓冲区大小，则仅读缓冲区可用字节
// 返回值：返回实际读取的字节数
size_t FKCW_Base_ByteBuffer::Read( uint8* p_pBuffer, size_t p_unLen )
{
	if( m_unReadPos + p_unLen > m_unWritePos )
		p_unLen = ( m_unWritePos - m_unReadPos );

	memcpy( p_pBuffer, &m_pBuffer[m_unReadPos], p_unLen );
	m_unReadPos += p_unLen;
	return p_unLen;
}
//--------------------------------------------------------------------
// 读取一个string
void FKCW_Base_ByteBuffer::Read( string& p_strDest )
{
	ReadCString( p_strDest );
}
//--------------------------------------------------------------------
// 读取一个C string
void FKCW_Base_ByteBuffer::ReadCString( string& p_strDest )
{
	p_strDest.clear();
	char c;
	while( true )
	{
		c = Read<char>();
		if( c == 0 )
			break;
		p_strDest += c;
	}
}
//--------------------------------------------------------------------
// 读取一个pascal 字符串，即前两个字节必须是字符串长度
void FKCW_Base_ByteBuffer::ReadPascalString( string& p_strDest )
{
	p_strDest.clear();
	uint16 len = Read<uint16>();
	while(len-- > 0) 
	{
		p_strDest += Read<char>();
	}
}
//--------------------------------------------------------------------
// 读取一个字符串，除非到了新行或者到了结尾才会停止
void FKCW_Base_ByteBuffer::ReadLine( string& p_strDest )
{
	p_strDest.clear();
	char c;
	while(m_unReadPos < m_unBufferSize)	
	{
		c = Read<char>();
		if(c == '\r')
			continue;
		else if(c == 0 || c == '\n')
			break;
		p_strDest += c;
	}
}
//--------------------------------------------------------------------
// 写入指定的数据
void FKCW_Base_ByteBuffer::Write( const uint8* p_pData, size_t p_unSize )
{
	size_t new_size = m_unWritePos + p_unSize;
	if(new_size > m_unBufferSize) 
	{
		if(m_bExternal) 
		{
			CCLOGWARN("external mode: buffer size is not enough to write");
			return;
		}
		new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
		_Reserve(new_size);
	}

	memcpy(&m_pBuffer[m_unWritePos], p_pData, p_unSize);
	m_unWritePos += p_unSize;
}
//--------------------------------------------------------------------
// 写入一个string
void FKCW_Base_ByteBuffer::Write( const string& p_strData )
{
	WriteCString( p_strData );
}
//--------------------------------------------------------------------
// 写入一个C string
void FKCW_Base_ByteBuffer::WriteCString( const string& p_strData )
{
	if(m_unWritePos + p_strData.length() + 1 > m_unBufferSize) 
	{
		if(m_bExternal) 
		{
			CCLOGWARN("external mode: buffer size is not enough to write");
			return;
		}
		_EnsureCanWrite(p_strData.length() + 1);
	}

	memcpy(&m_pBuffer[m_unWritePos], p_strData.c_str(), p_strData.length() + 1);
	m_unWritePos += (p_strData.length() + 1);
}
//--------------------------------------------------------------------
// 写入一个pascal 字符串，即前两个字节必须是字符串长度，但注意，字符串不会尾部补0
void FKCW_Base_ByteBuffer::WritePascalString( const string& p_strData )
{
	if(m_unWritePos + p_strData.length() + sizeof(uint16) > m_unBufferSize) 
	{
		if(m_bExternal) 
		{
			CCLOGWARN("external mode: buffer size is not enough to write");
			return;
		}
		_EnsureCanWrite(p_strData.length() + sizeof(uint16));
	}

	Write<uint16>(p_strData.length());
	memcpy(&m_pBuffer[m_unWritePos], p_strData.c_str(), p_strData.length());
	m_unWritePos += p_strData.length();
}
//--------------------------------------------------------------------
// 写入一个string并换行
void FKCW_Base_ByteBuffer::WriteLine( const string& p_strData )
{
	if(m_unWritePos + p_strData.length() + 2 * sizeof(char) > m_unBufferSize) 
	{
		if(m_bExternal) 
		{
			CCLOGWARN("external mode: buffer size is not enough to write");
			return;
		}
		_EnsureCanWrite(p_strData.length() + 2 * sizeof(char));
	}

	memcpy(&m_pBuffer[m_unWritePos], p_strData.c_str(), p_strData.length());
	m_unWritePos += p_strData.length();
	Write<char>('\r');
	Write<char>('\n');
}
//--------------------------------------------------------------------
// 获取读指针位置
size_t FKCW_Base_ByteBuffer::GetReadPos()
{
	return m_unReadPos;
}
//--------------------------------------------------------------------
// 设置读指针
void FKCW_Base_ByteBuffer::SetReadPos( size_t p_unPos )
{
	if(p_unPos <= m_unWritePos) 
		m_unReadPos = p_unPos;
}
//--------------------------------------------------------------------
// 设置写指针
void FKCW_Base_ByteBuffer::SetWritePos( size_t p_unPos )
{
	if(p_unPos <= m_unBufferSize)
		m_unWritePos = p_unPos;
}
//--------------------------------------------------------------------