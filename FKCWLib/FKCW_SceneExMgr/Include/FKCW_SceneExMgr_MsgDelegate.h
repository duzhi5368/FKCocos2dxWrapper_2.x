//*************************************************************************
//	创建日期:	2014-11-11
//	文件名称:	FKCW_SceneExMgr_MsgDelegate.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr.h"
//-------------------------------------------------------------------------
// 消息数据
struct SSenceExMsg
{
	CCObject*    pMsgObj;
	void*        wParam;
	void*        lParam;
	unsigned int uMsg;
};
//-------------------------------------------------------------------------
// 消息接收器，继承此类对象可获得消息公告
class FKCW_SceneExMgr_MsgDelegate
{
public:
	FKCW_SceneExMgr_MsgDelegate(){};

public:
	virtual void onMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam){};
};
//-------------------------------------------------------------------------
// 给指定目标的消息数据
struct SSceneExMsgForTarget
{
	SSenceExMsg tMessage;
	FKCW_SceneExMgr_MsgDelegate* pDelegate;
};
//-------------------------------------------------------------------------