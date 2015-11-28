//*************************************************************************
//	创建日期:	2015-1-12
//	文件名称:	FKMsgDispatchSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKMsgDispatchSystem : public IMsgDispatchSystem
{
public:
	FKMsgDispatchSystem();
	~FKMsgDispatchSystem();
public:
	// 释放
	virtual void		Release();
	// 分发消息
	// 参数：szMsgData 需要分发的数据指针
	// 参数：nLen 需要分发的数据长度
	virtual void		DispatchMsg( const char* szMsgData, int nLen );
	// 注册消息处理器类
	// 参数：pHandler 消息接收器类
	// 参数：usMainType 主消息码
	// 参数：usSubType 子消息码
	virtual bool		RegisteMsgHandler( IMessageHandler* pHandler, 
		MSG_MAIN_TYPE usMainType, MSG_SUB_TYPE usSubType );
	// 注销消息处理器类
	virtual bool		UnregisterMsgHandler( IMessageHandler* pHandler, 
		MSG_MAIN_TYPE usMainType, MSG_SUB_TYPE usSubType );
private:
	typedef list<IMessageHandler*>			MESSAGE_RECEIVER_LIST;
	typedef map<MSG_MAIN_TYPE,MESSAGE_RECEIVER_LIST>	MESSAGE_RECEIVER_ARRAY;

	MESSAGE_RECEIVER_ARRAY		m_MsgReceiverArray;
};
//-------------------------------------------------------------------------
extern FKMsgDispatchSystem gs_FKMsgDispatchSystem;
//-------------------------------------------------------------------------