//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_WidgetWindow.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	组件窗口
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UIWidget_Macros.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
//-------------------------------------------------------------------------
class FKCW_UIWidget_WidgetWindow : public CCNodeRGBA, public CCTouchDelegate
{
public:
	FKCW_UIWidget_WidgetWindow();
	virtual ~FKCW_UIWidget_WidgetWindow();
public:
	// 默认初始化
	virtual bool init();
	// 默认创建
	static FKCW_UIWidget_WidgetWindow* create();

	// 获取触摸消息层级
	virtual int getTouchPriority();
	// 设置消息触摸层级（相对于组件树根节点）
	virtual void setTouchPriority(int nTouchPriority);

	// 是否当前开启了触摸消息处理
	virtual bool isTouchEnabled();
	// 设置触摸消息的开启关闭
	virtual void setTouchEnabled(bool bTouchEnabled);

	// 是否处理多点触摸消息
	bool isMultiTouchEnabled() const;
	// 设置是否处理多点触摸消息
	void setMultiTouchEnabled(bool bEnabled);

	// 设置是否为模态
	void setModalable(bool bModalable);
	// 是否为模态
	bool isModalable() const;

	// 查找组件树中第一个ID相符的组件
	CCObject* findWidgetById(const char* id);

	// 长按后的移动 消息监听器
	virtual void setOnTouchMovedAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler);
	// 长按后的抬起 消息监听器
	virtual void setOnTouchEndedAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler);
	// 长按后的取消 消息监听器
	virtual void setOnTouchCancelledAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler);

public:
	virtual void	visit();
	virtual void	onEnter();
	virtual void	onExit();

	virtual bool	ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void	ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void	ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void	ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);

	virtual void	ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void	ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void	ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	virtual void	ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent);

	virtual void	setLongClickTouchHandlerWidget(CCObject* pWidgetObject, int nTouchId);

	CCObject*		find(CCArray* pChidren, const char* id);

	virtual void	executeTouchMovedAfterLongClickHandler(CCObject* pSender, CCTouch* pTouch, float fDuration);
	virtual void	executeTouchEndedAfterLongClickHandler(CCObject* pSender, CCTouch* pTouch, float fDuration);
	virtual void	executeTouchCancelledAfterLongClickHandler(CCObject* pSender, CCTouch* pTouch, float fDuration);

protected:
	struct SMultiTouchTarget
	{
		FKCW_UIWidget_Widget*	pWidget;
		CCObject*				pLongClickedWidgetObject;
		float					fTouchedDuration;
	};
	std::map<int, SMultiTouchTarget> m_mMultiTouchKeeper;

protected:
	int							m_nTouchPriority;
	bool						m_bTouchEnabled;
	float						m_fTouchedDuration;
	bool						m_bIsTouched;
	bool						m_bMultiTouchEnabled;
	bool						m_bModalable;

	CCObject*					m_pLongClickedWidgetObject;
	FKCW_UIWidget_Widget*		m_pSelectedWidget;

	CCObject*					m_pTouchMovedAfterLongClickListener;
	CCObject*					m_pTouchEndedAfterLongClickListener;
	CCObject*					m_pTouchCancelledAfterLongClickListener;

	SEL_AfterLongClickHandler	m_pTouchMovedAfterLongClickHandler;
	SEL_AfterLongClickHandler	m_pTouchEndedAfterLongClickHandler;
	SEL_AfterLongClickHandler	m_pTouchCancelledAfterLongClickHandler;

#if FKCW_WIDGET_USING_LUA
protected:
	int							m_nTouchMovedAfterLongClickScriptHandler;
	int							m_nTouchEndedAfterLongClickScriptHandler;
	int							m_pTouchCancelledAfterLongClickScriptHandler;
public:
	void						setOnTouchMovedAfterLongClickScriptHandler(int nHandler);
	void						setOnTouchEndedAfterLongClickScriptHandler(int nHandler);
	void						setOnTouchCancelledAfterLongClickScriptHandler(int nHandler);
	void						removeOnTouchMovedAfterLongClickScriptHandler();
	void						removeOnTouchEndedAfterLongClickScriptHandler();
	void						removeOnTouchCancelledAfterLongClickScriptHandler();
#endif
};