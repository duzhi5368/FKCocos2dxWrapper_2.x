//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	FKNetworkCommand.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKNetworkMacros.h"
//-------------------------------------------------------------------------
class FKNetworkCommand
{
public:
	unsigned int			m_unType;		// 消息类型[ENUM_ClientNetMsgType]
	unsigned int			m_unSocketID;	// SocketID
	unsigned int			m_unSocket;		// Socket指针 或者 ErrorCode
	unsigned int			m_unLen;		// 消息长度 或者 错误提示信息长度
	vector<char>			m_szMsg;		// 消息数据 或者 错误提示信息
public:
	FKNetworkCommand();
	FKNetworkCommand( const FKNetworkCommand& other );
	FKNetworkCommand& operator= ( const FKNetworkCommand& other );
};