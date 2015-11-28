/********************************************************************
	created:	2014/10/31
	file base:	FKCW_Network_Packet
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
//--------------------------------------------------------------------
class FKCW_Network_Packet : public CCObject
{
public:
	typedef struct SHeader
	{
		char magic[4];
		int protocolVersion;
		int serverVersion;
		int command;
		int encryptAlgorithm;
		int length;					// 头部之后的数据长度
	}PACKET_HEADER;

	CC_SYNTHESIZE_PASS_BY_REF_NC(SHeader, m_Header, Header);
	CC_SYNTHESIZE(char*, m_pBuffer, Buffer);
	CC_SYNTHESIZE_READONLY(size_t, m_unPacketLength, PacketLength);
	CC_SYNTHESIZE_READONLY(bool, m_bIsRaw, Raw);
protected:
	FKCW_Network_Packet();

	virtual bool	InitWithStandardBuf( const char* p_szBuf, size_t p_unLen );
	virtual bool	InitWithRawBuf( const char* p_szBuf, size_t p_unLen, int p_nAlgorithm );
	virtual bool	InitWithJson( const string& p_strMagic, int p_nCommand, FKCW_Json_Object* p_pJson,
		int p_nProtocolVersion, int p_nServerVersion, int p_nAlgorithm );

	// 分配一块内存
	void			Allocate( size_t p_unLen );
	// 写消息头
	void			WriteHeader();
public:
	virtual			~FKCW_Network_Packet();
	// 通过一个标准缓存创建一个头
	static FKCW_Network_Packet* CreateStandardPacket( const char* p_szBuf, size_t p_unLen );
	// 创建一个不含包头的原始数据包。包体来自于从socket接收到的原始数据。
	// 注：若数据需要加密，可设置 p_nAlgorithm 参数。默认-1表示不加密原始数据
	static FKCW_Network_Packet* CreateRawPacket( const char* p_szBuf, size_t p_unLen, int p_nAlgorithm = -1 );
	// 创建一个标准消息包，其包体来源于一个 json 对象。
	static FKCW_Network_Packet* CreateStandardPacket( const string& p_strMagic, int p_nCommand, 
		FKCW_Json_Object* p_pJson, int p_nAlgorithm = -1 );
	static FKCW_Network_Packet* CreateStandardPacket( const string& p_strMagic, int p_nCommand, 
		FKCW_Json_Object* p_pJson, int p_nProtocolVersion, int p_nServerVersion, int p_nAlgorithm = -1 );

	// 获取消息包体的大小
	int				GetBodyLength();
	// 获取消息包体指针
	const char*		GetBody();
};