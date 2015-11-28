//*************************************************************************
//	创建日期:	2015-3-24
//	文件名称:	MsgBoxMgr.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
//-------------------------------------------------------------------------

class IMsgBoxHandle
{
public:
	virtual void	OnMessageBoxCallBack(int nMessageKey) = 0;
};
class CMsgBoxMgr : public CCObject,public ITimerHander
{
	CMsgBoxMgr(void);
	~CMsgBoxMgr(void);
public:
	static CMsgBoxMgr& instance()
	{
		static CMsgBoxMgr _inst;
		return _inst;
	}

public:
	// 显示领奖提示框
	//		pHandle 回调 一般传自己
	//		pRootWindow 跟窗口,提示框会addChild到此窗口上
	//		nAwardType 奖励类型 详细请查看GameCommon的枚举
	//		nDuration 万一玩家没点击领取按钮,则在这个时间后提示框自动消失 默认 30s(30s = 30*1000 ms)
	bool	ShowAwardBox(IMsgBoxHandle* pHandle,FKCW_UIWidget_WidgetWindow* pRootWindow,int nAwardType,float nDuration = 30000.0f);
	// 显示普通提示框
	bool	Show(IMsgBoxHandle* pHandle,FKCW_UIWidget_WidgetWindow* pRootWindow,int nType,float nDuration = 30000.0f);
	// 隐藏销毁
	void	Hide();
	// 判断当前是否有提示框显示在界面上
	bool	IsShow(){ return m_bIsShowing; };
	// 按钮回调
	void	OnBtnHandle(CCObject* pSender);
	// 定时器时间触发回调
	virtual void		OnTimer( unsigned int unTimerID );

private:
	void	__ShowAwardBox(int nAwardType,float nDuration = 30000.0f);
	void	__ShowNormalBox(int nType,float nDuration);
	void	__Clear();

private:

	bool						m_bIsShowing;
	IMsgBoxHandle*				m_pHandle;
	FKCW_UIWidget_WidgetWindow* m_pRootWindow;
	FKCW_UIWidget_Button*		m_pAwardBtn;
	CCNode*						m_pMsgNode;
	int							m_nMsgKey;
	
};

