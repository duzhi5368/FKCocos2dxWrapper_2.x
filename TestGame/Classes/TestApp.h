//*************************************************************************
//	创建日期:	2015-1-15
//	文件名称:	TestApp.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
#include "EventBridge.h"
//-------------------------------------------------------------------------
class TestApp : public FKFramework
{
public:
	virtual bool	SubInit();
	virtual void	SubUpdate( float dt );
	virtual void	SubRelease();
public:

	// 收到输入消息的回调处理函数
	// 参数：unMsgType 定义的消息类型【参见ENUM_InputMsgType】
	// 参数：nTouchID 触摸消息的ID，若为key消息，则该值无效为0
	// 参数：fPosX x位置
	// 参数：fPosY y消息
	// 参数：详细数据指针，例如捏合消息有额外的数据（放大还是缩小）滑动的额外数据（滑动方向）等，均记录在此。
	// 返回值：若返回true表示忽略后面的调用，消息不再向后传递；若返回false则表示消息将继续向下传递
	virtual bool	OnInputMessage( unsigned int unMsgType, int nTouchID, 
		float fPosX, float fPosY, void* pParam );
	
private:
	// 自动屏幕适配
	void			__AutomaticScreenAdaptation();
	// 加载全局资源
	void			__LoadGlobleResources();
	// 释放全局资源
	void			__RelesaeGlobleResources();

private:
	EventBridge m_EventBridge;

};
//-------------------------------------------------------------------------
extern TestApp* GetApp();
//-------------------------------------------------------------------------