//*************************************************************************
//	创建日期:	2015-3-24
//	文件名称:	EventBridge.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class EventBridge : public IExecEventHandler
{
public:
	EventBridge(void);
	~EventBridge(void);

public:
	// 收到执行消息处理回调
	// 参数：usID 事件ID
	// 参数：usSrcType 发送者源类型（参见相关枚举）
	// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
	//		以及某对象指针等，视消息类型逻辑程序员自己规定）
	// 参数：szEvent 消息具体数据指针
	// 参数：nEventLen 消息数据的长度
	virtual void	OnExec( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen );

public:
	// 注册全局事件
	void			RegisteFKEvent();
	// 移除全局事件
	void			UnRegisteFKEvent();

private:
	// 切换Scene
	void			__SwitchScene(int nTag);
	// 按钮回调信息处理
	void			__ClickPlayButton(int nTag);
};

