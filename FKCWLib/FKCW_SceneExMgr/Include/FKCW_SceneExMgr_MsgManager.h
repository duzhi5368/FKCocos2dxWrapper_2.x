//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_MsgManager.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_MsgDelegate.h"
#include "FKCW_SceneExMgr.h"
//-------------------------------------------------------------------------
// 消息管理器
class FKCW_SceneExMgr_MsgManager : public CCObject
{
public:
	virtual ~FKCW_SceneExMgr_MsgManager();
	static FKCW_SceneExMgr_MsgManager* sharedManager();

	// 下一帧，公告消息给全部接收器
	void PostMessage(unsigned int uMsg, CCObject* pMsgObj = NULL, void* wParam = NULL, void* lParam = NULL);
	// 下一帧，发布消息给指定接收器
	void PostMessage(FKCW_SceneExMgr_MsgDelegate* pDelegate, unsigned int uMsg,
		CCObject* pMsgObj = NULL, void* wParam = NULL, void* lParam = NULL);
	// 注册一个新的消息接收器
	void RegisterMsgDelegate(FKCW_SceneExMgr_MsgDelegate* pDelegate);
	// 卸载一个消息接收器
	void UnregisterMsgDelegate(FKCW_SceneExMgr_MsgDelegate* pDelegate);

public:
	// 每帧更新
	void update();

private:
	FKCW_SceneExMgr_MsgManager();

	bool											m_bDispatchMsgLocked;
	std::vector<SSenceExMsg>						m_vMsgTemps;
	std::vector<SSenceExMsg>						m_vMsgQueue;
	std::vector<SSceneExMsgForTarget>				m_vTargetMsgTemps;
	std::vector<SSceneExMsgForTarget>				m_vTargetMsgQueue;
	std::vector<FKCW_SceneExMgr_MsgDelegate*>		m_vMsgDelegates;
	std::vector<FKCW_SceneExMgr_MsgDelegate*>		m_vMsgDelegatesRemoved;
	std::vector<FKCW_SceneExMgr_MsgDelegate*>		m_vMsgDelegatesAdded;

};