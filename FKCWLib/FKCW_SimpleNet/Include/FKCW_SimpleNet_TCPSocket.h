//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_SimpleNet_TCPSocket.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SimpleNet_Macro.h"
#include "FKCW_SimpleNet_InetAddress.h"
//-------------------------------------------------------------------------
// 枚举当前socket状态
enum ENUM_SocketStatus
{
	eSocketConnected			=  1,
	eSocketConnecting			=  2,
	eSocketDisconnected			=  3,
	eSocketConnectFailed		=  4,
	eSocketConnectTimeout		=  5,
	eSocketReceive				=  6,
	eSocketIoClosed				=  0,
	eSocketIoError				= -1,
};
//-------------------------------------------------------------------------
class FKCW_SimpleNet_TCPSocket
{
public:
	FKCW_SimpleNet_TCPSocket();
	virtual ~FKCW_SimpleNet_TCPSocket();
	bool Init();
	int  Read(char* _buff, int _len);
	int  Write(char* _buff, int _len);
	int  IsConnected();
	bool IsReadable();
	bool IsWritable();
	void Close();
	bool Connect();
	void Disconnect();
	void SetInetAddress(const FKCW_SimpleNet_InetAddress& oInetAddress);

protected:
	SOCKET						m_uSocket;
	FKCW_SimpleNet_InetAddress	m_oInetAddress;
};
