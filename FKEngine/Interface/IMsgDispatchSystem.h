//*************************************************************************
//	创建日期:	2015-1-12
//	文件名称:	IMsgDispatchSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/*
	注意：该类接口仅提供给网络消息分发。如需要逻辑内部消息分发，请使用EventSystem
*/
//-------------------------------------------------------------------------
// 消息类型级别定义
typedef unsigned short MSG_MAIN_TYPE;		// 主消息码类型
typedef unsigned short MSG_SUB_TYPE;		// 子消息码类型
//-------------------------------------------------------------------------
// 消息接收回调处理类
class IMessageHandler
{
public:
	// 接收到消息[注意，收到的消息内，包含子消息码，但不包含主消息码]
	// 参数：szMsgData 收到的数据指针
	// 参数：nLen 收到的数据长度
	virtual void		OnNetMessage( const char* szMsgData, int nLen ) = 0;
};
//-------------------------------------------------------------------------
// 消息分发器系统
class IMsgDispatchSystem
{
public:
	// 释放
	virtual void		Release() = 0;
	// 分发消息
	// 参数：szMsgData 需要分发的数据指针
	// 参数：nLen 需要分发的数据长度
	virtual void		DispatchMsg( const char* szMsgData, int nLen ) = 0;
	// 注册消息处理器类
	// 参数：pHandler 消息接收器类
	// 参数：usMainType 主消息码
	// 参数：usSubType 子消息码
	virtual bool		RegisteMsgHandler( IMessageHandler* pHandler, 
		MSG_MAIN_TYPE usMainType, MSG_SUB_TYPE usSubType ) = 0;
	// 注销消息处理器类
	virtual bool		UnregisterMsgHandler( IMessageHandler* pHandler,
		MSG_MAIN_TYPE usMainType, MSG_SUB_TYPE usSubType ) = 0;
};
//-------------------------------------------------------------------------
// 获取消息分发器接口
extern IMsgDispatchSystem* GetMsgDispatchSystem();
//-------------------------------------------------------------------------