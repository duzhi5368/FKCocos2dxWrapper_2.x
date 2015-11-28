//*************************************************************************
//	创建日期:	2015-1-14
//	文件名称:	FKInputSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKInputSystem : public IInputSystem, public CCTargetedTouchDelegate,
	public CCKeypadDelegate, public CCObject
{
public:
	FKInputSystem();
	~FKInputSystem();
public:
	// 初始化工作
	virtual bool	Init();
	// 销毁释放
	virtual void	Release();
	// 设置消息回调处理函数
	virtual void	SetInputMsgCallback( IInputMsgHandler* pInputMsgCallback );
	// 设置是否启动输入处理
	virtual void	SetInputEnabled( bool bEnable );
	// 获取消息回调处理函数
	virtual IInputMsgHandler* GetInputMsgCallback() const;
protected:
	// 触摸开始回调
	virtual bool	ccTouchBegan(CCTouch* touch, CCEvent* event);
	// 触摸移动回调
	virtual void	ccTouchMoved(CCTouch* touch, CCEvent* event);
	// 触摸结束回调
	virtual void	ccTouchEnded(CCTouch* touch, CCEvent* event);
	// Android和iOS上的后退键按下回调
	virtual void	keyBackClicked();
	// Android和iOS上的Menu键按下回调
	virtual void	keyMenuClicked();
protected:
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// WINDOWS 消息
	static void WinKeyboardHook( UINT message,WPARAM wParam, LPARAM lParam );
#endif
	// 复杂手势支持
	void onLongPress(CCObject* object);
	void onPan(CCObject* object);
	void onPinch(CCObject* object);
	void onSwipe(CCObject* object);
	void onTap(CCObject* object);
private:
	IInputMsgHandler*		m_pMsgCallback;		// 消息回调接口
};
//-------------------------------------------------------------------------
extern FKInputSystem gs_FKInputSystem;
//-------------------------------------------------------------------------