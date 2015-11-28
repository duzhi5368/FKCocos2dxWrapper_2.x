//*************************************************************************
//	创建日期:	2015-1-14
//	文件名称:	FKInputSystemMacros.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 回调处理消息的类型
enum ENUM_InputMsgType
{
	eIMT_UnknownMsg				= 0,
	eIMT_TouchBegin				= 1,		// 触屏开始
	eIMT_TouchMove				= 1,		// 触屏移动中
	eIMT_TouchEnd				= 2,		// 触屏结束

	eIMT_LongPress				= 100,		// 长按触屏
	eIMT_Pan					= 101,		// 滑动拖移
	eIMT_Pinch					= 102,		// 捏合
	eIMT_Swipe					= 103,		// 快速滑动
	eIMT_Tap					= 104,		// 点击

	eIMT_BackKey				= 200,		// Android和iOS上的后退键按下
	eIMT_MenuKey				= 201,		// Android和iOS上的Menu键按下

	eIMT_KeyF1					= 300,		// Win上F1键按下
	eIMT_KeyF2					= 301,		// Win上F2键按下
	eIMT_KeyF3					= 302,		// Win上F3键按下
	eIMT_KeyF4					= 303,		// Win上F4键按下
	eIMT_KeyF5					= 304,		// Win上F5键按下
	eIMT_KeyF6					= 305,		// Win上F6键按下
	eIMT_KeyF7					= 306,		// Win上F7键按下
	eIMT_KeyF8					= 307,		// Win上F8键按下

	eIMT_KeyW					= 308,		// Win上'W'键按下
	eIMT_KeyS					= 309,		// Win上'S'键按下
	eIMT_KeyA					= 310,		// Win上'A'键按下
	eIMT_KeyD					= 311,		// Win上'D'键按下
	eIMT_KeyUp					= 312,		// Win上上箭头键按下
	eIMT_KeyDown				= 313,		// Win上下箭头键按下
	eIMT_KeyLeft				= 314,		// Win上左箭头键按下
	eIMT_KeyRight				= 315,		// Win上右箭头键按下

	eIMT_KeySpace				= 316,		// Win上空格键按下

	eIMT_AnyKeyDown				= 400,		// Win上任意按钮按下消息
	eIMT_AnyKeyUp				= 401,		// Win上任意按钮抬起消息
	eIMT_MouseWheel				= 402,		// Win上滚轮缩放消息

	eIMT_UserData				= 1000,		// 用户自定义消息
};
//-------------------------------------------------------------------------
// 滚轮消息额外信息
struct SMouseWheelParam
{
	bool		m_bIsFront;			// 是否向前滚动（true为向前向上，false为向后向下）
	short		m_sDelta;			// 滚轮滚动角度
	SMouseWheelParam()
		: m_bIsFront( false )
		, m_sDelta( 0 ){}
};
//-------------------------------------------------------------------------
// 滑动拖移额外信息
struct SPanParam
{
	float		m_fDeltaX;
	float		m_fDeltaY;
	SPanParam()
		: m_fDeltaX( 0.0f )
		, m_fDeltaY( 0.0f ){}
};
//-------------------------------------------------------------------------
// 挤压捏合额外信息
struct SPinchParam
{
	bool		m_bIsOpen;			// 是否是放大（true表示是放大，false表示是缩小）
	SPinchParam()
		: m_bIsOpen( false ){}
};
//-------------------------------------------------------------------------
// 快速滑动信息
struct SSwipeParam
{
	bool		m_bIsRight;		// 是否是向右滑动
	bool		m_bIsLeft;		// 是否是向左滑动
	bool		m_bIsUp;		// 是否是向上滑动
	bool		m_bIsDown;		// 是否是向下滑动
	SSwipeParam()
		: m_bIsDown( false )
		, m_bIsUp( false )
		, m_bIsLeft( false )
		, m_bIsRight( false ){}
};
//-------------------------------------------------------------------------