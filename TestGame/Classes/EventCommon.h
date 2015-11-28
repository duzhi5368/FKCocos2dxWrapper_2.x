//*************************************************************************
//	创建日期:	2015-3-23
//	文件名称:	EventCommon.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

enum ENMU_EVENT_SRC_ID
{
	eEvent_Src_Min = 0,
	eEvent_Scene_Switch,			// 场景切换消息
	eEvent_Play_ButtonClick,		// 
	eEvent_Src_Max,
};

enum ENUM_EVENT_TYPE_ID
{
	eEvent_Min = 0,
	eEvent_UIEvent,				// 用户操作触发事件
	eEventE_Max,
};