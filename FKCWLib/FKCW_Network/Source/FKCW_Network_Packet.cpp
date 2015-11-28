//--------------------------------------------------------------------
#include "../Include/FKCW_Network_Packet.h"
#include "../Include/FKCW_Network_NetworkCommon.h"
#include "../Include/FKCW_Network_TCPSocketHub.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
//--------------------------------------------------------------------
FKCW_Network_Packet::FKCW_Network_Packet()
	: m_pBuffer( NULL )
	, m_unPacketLength( 0 )
	, m_bIsRaw( false )
{
	memset( &m_Header, 0, sizeof(m_Header) );
}
//--------------------------------------------------------------------
FKCW_Network_Packet::~FKCW_Network_Packet()
{
	CC_SAFE_FREE( m_pBuffer );
}
//--------------------------------------------------------------------
// 通过一个标准缓存创建一个头
FKCW_Network_Packet* FKCW_Network_Packet::CreateStandardPacket( const char* p_szBuf, size_t p_unLen )
{
	if( p_unLen < PacketHeaderLength )
		return NULL;

	FKCW_Network_Packet* p = new FKCW_Network_Packet();
	if( p->InitWithStandardBuf(p_szBuf, p_unLen) )
	{
		return (FKCW_Network_Packet*)p->autorelease();
	}
	CC_SAFE_FREE( p );
	return NULL;
}
//--------------------------------------------------------------------
// 创建一个不含包头的原始数据包。包体来自于从socket接收到的原始数据。
// 注：若数据需要加密，可设置 p_nAlgorithm 参数。默认-1表示不加密原始数据
FKCW_Network_Packet* FKCW_Network_Packet::CreateRawPacket( const char* p_szBuf, size_t p_unLen, int p_nAlgorithm )
{
	FKCW_Network_Packet* p = new FKCW_Network_Packet();
	if( p->InitWithRawBuf(p_szBuf, p_unLen, p_nAlgorithm) )
	{
		return (FKCW_Network_Packet*)p->autorelease();
	}
	CC_SAFE_FREE( p );
	return NULL;
}
//--------------------------------------------------------------------
// 创建一个标准消息包，其包体来源于一个 json 对象。
FKCW_Network_Packet* FKCW_Network_Packet::CreateStandardPacket( const string& p_strMagic, int p_nCommand, 
												 FKCW_Json_Object* p_pJson, int p_nAlgorithm )
{
	FKCW_Network_Packet* p = new FKCW_Network_Packet();
	if( p->InitWithJson( p_strMagic, p_nCommand, p_pJson, 0, 0, p_nAlgorithm ) )
	{
		return (FKCW_Network_Packet*)p->autorelease();
	}
	CC_SAFE_FREE( p );
	return NULL;
}
//--------------------------------------------------------------------
FKCW_Network_Packet* FKCW_Network_Packet::CreateStandardPacket( const string& p_strMagic, int p_nCommand, 
	FKCW_Json_Object* p_pJson, int p_nProtocolVersion, int p_nServerVersion, int p_nAlgorithm )
{
	FKCW_Network_Packet* p = new FKCW_Network_Packet();
	if( p->InitWithJson( p_strMagic, p_nCommand, p_pJson, p_nProtocolVersion, p_nServerVersion, p_nAlgorithm ) )
	{
		return (FKCW_Network_Packet*)p->autorelease();
	}
	CC_SAFE_FREE( p );
	return NULL;
}
//--------------------------------------------------------------------
bool FKCW_Network_Packet::InitWithStandardBuf( const char* p_szBuf, size_t p_unLen )
{
	if(p_unLen < PacketHeaderLength)
		return false;
	
	// 消息头
	FKCW_Base_ByteBuffer bb(p_szBuf, p_unLen, p_unLen);
	m_Header.magic[0] = bb.Read<char>();
	m_Header.magic[1] = bb.Read<char>();
	m_Header.magic[2] = bb.Read<char>();
	m_Header.magic[3] = bb.Read<char>();
	m_Header.protocolVersion	= betoh32(bb.Read<int>());
	m_Header.serverVersion		= betoh32(bb.Read<int>());
	m_Header.command			= betoh32(bb.Read<int>());
	m_Header.encryptAlgorithm	= betoh32(bb.Read<int>());
	m_Header.length				= betoh32(bb.Read<int>());

	// 消息body
	if(bb.GetAvailable() >= static_cast<size_t>(m_Header.length)) 
	{
		if(FKCW_Network_TCPSocketHub::GetDecryptFunc()) 
		{
			// 读取body并尝试解密
			char* body = (char*)malloc(m_Header.length * sizeof(char));
			bb.Read((uint8*)body, m_Header.length);
			size_t plainLen;
			char* plain = (char*)(*FKCW_Network_TCPSocketHub::GetDecryptFunc())(body, 
				m_Header.length, &plainLen, m_Header.encryptAlgorithm);
			if(plain != body)
				free(body);

			// 拷贝到主体中
			Allocate(plainLen + PacketHeaderLength + 1);
			memcpy(m_pBuffer + PacketHeaderLength, plain, plainLen);
			m_Header.length = (int)plainLen;
			free(plain);
		} 
		else
		{
			Allocate(m_Header.length + PacketHeaderLength + 1);
			bb.Read((uint8*)m_pBuffer + PacketHeaderLength, m_Header.length);
		}
	}
	else 
	{
		return false;
	}

	// 初始化其他一些信息
	m_bIsRaw = false;
	m_unPacketLength = m_Header.length + PacketHeaderLength;

	// 写入头部
	WriteHeader();

	return true;
}
//--------------------------------------------------------------------
bool FKCW_Network_Packet::InitWithRawBuf( const char* p_szBuf, size_t p_unLen, int p_nAlgorithm )
{
	if(FKCW_Network_TCPSocketHub::GetDecryptFunc() && p_nAlgorithm != -1)
	{
		// 读取body并尝试解密
		size_t plainLen;
		char* plain = (char*)(*FKCW_Network_TCPSocketHub::GetDecryptFunc())
			(p_szBuf, p_unLen, &plainLen, p_nAlgorithm);

		// 拷贝到消息主体中
		Allocate(plainLen + 1);
		memcpy(m_pBuffer, plain, plainLen);
		m_Header.length = (int)plainLen;
		if(plain != p_szBuf)
			free(plain);
	} 
	else 
	{
		m_Header.length = (int)p_unLen;
		Allocate(p_unLen + 1);
		memcpy(m_pBuffer, p_szBuf, p_unLen);
	}

	// 其他
	m_bIsRaw = true;
	m_unPacketLength = m_Header.length;

	return true;
}
//--------------------------------------------------------------------
bool FKCW_Network_Packet::InitWithJson( const string& p_strMagic, int p_nCommand, FKCW_Json_Object* p_pJson,
							 int p_nProtocolVersion, int p_nServerVersion, int p_nAlgorithm )
{
	// 检查魔数
	if( p_strMagic.length() < 4 )
		return false;

	m_Header.magic[0] = p_strMagic.at(0);
	m_Header.magic[1] = p_strMagic.at(1);
	m_Header.magic[2] = p_strMagic.at(2);
	m_Header.magic[3] = p_strMagic.at(3);

	// 协议版本
	m_Header.protocolVersion		= p_nProtocolVersion;
	// 服务器版本
	m_Header.serverVersion			= p_nServerVersion;
	// ID
	m_Header.command				= p_nCommand;
	// 加密
	m_Header.encryptAlgorithm		= p_nAlgorithm;
	
	// 消息主体
	string strBodyStr = p_pJson->ToString();
	char* plain = (char*)strBodyStr.c_str();
	if(FKCW_Network_TCPSocketHub::GetEncryptFunc() && p_nAlgorithm != -1)
	{
		// 需要加密，则先加密body
		size_t bodyLen = strBodyStr.length();
		size_t encLen;
		const char* enc = (*FKCW_Network_TCPSocketHub::GetEncryptFunc())(plain, bodyLen, &encLen, p_nAlgorithm);
		Allocate(encLen + PacketHeaderLength + 1);
		memcpy(m_pBuffer + PacketHeaderLength, enc, encLen);
		if(enc != plain) 
		{
			free((void*)enc);
		}

		// 消息长度
		m_Header.length = (int)encLen;
		m_unPacketLength = encLen + PacketHeaderLength;
	} 
	else 
	{
		// 不需要加密
		m_Header.length = (int)strBodyStr.length();
		Allocate(strBodyStr.length() + PacketHeaderLength + 1);
		memcpy(m_pBuffer + PacketHeaderLength, plain, strBodyStr.length());
		m_unPacketLength = strBodyStr.length() + PacketHeaderLength;
	}

	// 写入头部信息
	WriteHeader();

	// 初始化
	m_bIsRaw = false;

	return true;

}
//--------------------------------------------------------------------
// 分配一块内存
void FKCW_Network_Packet::Allocate( size_t p_unLen )
{
	if( !m_pBuffer )
		m_pBuffer = (char*)calloc( p_unLen, sizeof(char) );
}
//--------------------------------------------------------------------
// 写消息头
void FKCW_Network_Packet::WriteHeader()
{
	FKCW_Base_ByteBuffer bb(m_pBuffer, m_unPacketLength, 0);

	// 魔数
	bb.Write<char>(m_Header.magic[0]);
	bb.Write<char>(m_Header.magic[1]);
	bb.Write<char>(m_Header.magic[2]);
	bb.Write<char>(m_Header.magic[3]);

	bb.Write<int>(htobe32(m_Header.protocolVersion));
	bb.Write<int>(htobe32(m_Header.serverVersion));
	bb.Write<int>(htobe32(m_Header.command));
	bb.Write<int>(htobe32(m_Header.encryptAlgorithm));
	bb.Write<int>(htobe32(m_Header.length));
}
//--------------------------------------------------------------------
// 获取消息包体的大小
int FKCW_Network_Packet::GetBodyLength()
{
	return m_Header.length;
}
//--------------------------------------------------------------------
// 获取消息包体指针
const char* FKCW_Network_Packet::GetBody()
{
	if( m_bIsRaw )
		return m_pBuffer;
	else
		return m_pBuffer + PacketHeaderLength;
}
//--------------------------------------------------------------------