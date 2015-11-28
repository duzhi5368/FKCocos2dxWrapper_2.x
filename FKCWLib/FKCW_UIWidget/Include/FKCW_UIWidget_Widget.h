//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_Widget.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	组件
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Macros.h"
//-------------------------------------------------------------------------
class FKCW_UIWidget_Widget : public FKCW_UIWidget_TouchProtocol
{
public:
	FKCW_UIWidget_Widget();
	virtual ~FKCW_UIWidget_Widget();
public:
	// 获取用户自定义标签（这个标签和cocos2dx的tag没有任何关联）
	int getUserTag() const;
	// 设置用户自定义标签
	void setUserTag(int nTag);

	// 获取组件ID
	const char* getId();
	// 设置组件ID
	void setId(const char* id);

	// 获取组件描述信息
	const char* getDescription();
	// 设置组件描述信息
	void setDescription(const char* description);

	// 判断组件是否有效（若无效，不处理任何触摸消息）
	virtual bool isEnabled();
	// 设置组件有效性
	virtual void setEnabled(bool bEnabled);

	// 判断组件是否触摸有效（若无效，则不处理任何触摸消息）
	virtual bool isTouchEnabled();
	// 设置组件是否处理触摸消息
	virtual void setTouchEnabled(bool bTouchEnabled);

	// 中断当前控件的触碰消息
	virtual void interruptTouchCascade(CCTouch* pTouch, float fDuration);
	// 中断当前控件的触碰消息并立刻调用touch取消
	virtual void interruptTouch(CCTouch* pTouch, float fDuration);
	// 判断触摸是否中断
	virtual bool isTouchInterrupted();

	// 设置触摸 开始 / 移动 / 结束 / 取消 消息监听器
	virtual void setOnTouchBeganListener(CCObject* pListener, SEL_TouchBeganHandler pHandler);
	virtual void setOnTouchMovedListener(CCObject* pListener, SEL_TouchEventHandler pHandler);
	virtual void setOnTouchEndedListener(CCObject* pListener, SEL_TouchEventHandler pHandler);
	virtual void setOnTouchCancelledListener(CCObject* pListener, SEL_TouchEventHandler pHandler);

public:
	// 下列函数子类实现
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch){ return eWidgetTouchNone; }
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration){}
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration){}
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration){}
public:
	// 执行触摸 开始 / 移动 / 结束 / 取消 消息处理
	virtual ENUM_WidgetTouchModel executeTouchBeganHandler(CCTouch* pTouch);
	virtual void executeTouchMovedHandler(CCTouch* pTouch, float fDuration);
	virtual void executeTouchEndedHandler(CCTouch* pTouch, float fDuration);
	virtual void executeTouchCancelledHandler(CCTouch* pTouch, float fDuration);

public:
	virtual void setLongClickTouchHandlerWidget(CCNode* pWidget, int id);

protected:
	// 保存对象
	void					setThisObject(CCObject* pThis);

protected:
	std::string				m_strId;				// 组件ID
	std::string				m_strDescription;		// 组件描述性文字
	int						m_nUserTag;				// 用户自定义标签，和cocos2dx的tag没有任何关联性
	bool					m_bEnabled;				// 组件是否有效，若无效，不接收任何消息
	bool					m_bTouchEnabled;		// 组件是否处理触摸消息
	bool					m_bTouchInterrupt;

	CCObject*				m_pThisObject;
	CCObject*				m_pTouchBeganListener;
	CCObject*				m_pTouchMovedListener;
	CCObject*				m_pTouchEndedListener;
	CCObject*				m_pTouchCancelledListener;

	SEL_TouchBeganHandler	m_pTouchBeganHandler;
	SEL_TouchEventHandler	m_pTouchMovedHandler;
	SEL_TouchEventHandler	m_pTouchEndedHandler;
	SEL_TouchEventHandler	m_pTouchCancelledHandler;

#if FKCW_WIDGET_USING_LUA
protected:
	int						m_nTouchBeganScriptHandler;
	int						m_nTouchMovedScriptHandler;
	int						m_nTouchEndedScriptHandler;
	int						m_nTouchCancelledScriptHandler;
public:
	virtual void			setOnTouchBeganScriptHandler(int nHandler);
	virtual void			setOnTouchMovedScriptHandler(int nHandler);
	virtual void			setOnTouchEndedScriptHandler(int nHandler);
	virtual void			setOnTouchCancelledScriptHandler(int nHandler);
	virtual void			removeOnTouchBeganScriptHandler();
	virtual void			removeOnTouchMovedScriptHandler();
	virtual void			removeOnTouchEndedScriptHandler();
	virtual void			removeOnTouchCancelledScriptHandler();
#endif
};