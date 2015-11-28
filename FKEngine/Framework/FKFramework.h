//*************************************************************************
//	创建日期:	2015-1-14
//	文件名称:	FKFramework.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKFramework : public CCApplication, public IInputMsgHandler
{
public:
	FKFramework();
	virtual ~FKFramework();
public:
	// 子类继承
	virtual bool	SubInit() = 0;
	virtual void	SubUpdate( float dt ) = 0;
	virtual void	SubRelease() = 0;
public:
	// 初始化
	virtual bool	Init( float fWidth, float fHeight );
	// 初始化系统( 晚于Init )
	virtual bool	InitSystem();
	// 帧更新
	virtual void	UpdateFrame( float dt );
	// 释放工作
	virtual void	Release();
	// 执行循环
	virtual int		Run();
public:
	// 进入应用程序
	bool			applicationDidFinishLaunching();
	// 应用程序进入后台
    void			applicationDidEnterBackground();
	// 应用程序呼出前台
    void			applicationWillEnterForeground();
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

};
//-------------------------------------------------------------------------