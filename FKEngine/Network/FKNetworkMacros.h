//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	FKNetworkMacros.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 客户端最大socket连接数
static const int		gs_MAX_CLIENT_SOCKET_NUM		= 16;
// 客户端连接超时时间
static const float		gs_MAX_TIMEOUT_TIME				= 3.0f;
//-------------------------------------------------------------------------
// 客户端连接消息类型枚举
enum ENUM_ClientNetMsgType
{
	eCNMT_ConnectSuccess		= 0,		// 连接成功消息
	eCNMT_RecvMsg,							// 接收到网络消息
	eCNMT_Close,							// 连接关闭
	eCNMT_Error,							// 异常错误
};
//-------------------------------------------------------------------------
// 客户端连接错误类型枚举
enum ENUM_ClientNetErrorType
{
	eCNET_Unknown				= 0,		// 未明确的异常
	eCNET_IoError,							// IO读写错误
	eCNET_ConnectTimeout,					// 连接超时
	eCNET_NotEnoughMemory,					// 内存不足

	// todo...

	eCNET_Max,
};
static const string gs_ClientNetErrorInfo[eCNET_Max] = 
{
	"未明确的异常",
	"读写错误",
	"连接超时",
	"内存不足",
};
//-------------------------------------------------------------------------
