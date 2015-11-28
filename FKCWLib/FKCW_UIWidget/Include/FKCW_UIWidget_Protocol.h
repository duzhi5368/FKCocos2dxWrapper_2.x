//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_Protocol.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UIWidget_Macros.h"
//-------------------------------------------------------------------------
// 长按消息时间
static const float		FKCW_UIWIDGET_LONGPRESS_TIME = 0.5f;
// UI控件基准锚点和默认大小
static const CCPoint	FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT	= CCPoint(0.5f, 0.5f);
static const CCSize		FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE	= CCSize(100, 100);
// UI控件层容器基准锚点和默认大小
static const CCPoint	FKCW_UIWIDGET_LAYOUT_DEFAULT_ANCHOR_POINT	= CCPoint(0.5f, 0.5f);
static const CCSize		FKCW_UIWIDGET_LAYOUT_DEFAULT_CONTENT_SIZE	= CCSize(300, 300);
//-------------------------------------------------------------------------
class FKCW_UIWidget_Widget;
//-------------------------------------------------------------------------
// 控件触摸消息模式
enum ENUM_WidgetTouchModel
{
	// 不处理任何触摸消息事件
	eWidgetTouchNone,
	// 处理消息，但可以被父结构中断
	eWidgetTouchTransient,
	// 始终处理该消息，仅仅可能被自己中断
	eWidgetTouchSustained,
};
//-------------------------------------------------------------------------
typedef void					(CCObject::*SEL_ClickHandler)(CCObject *pSender);
typedef bool					(CCObject::*SEL_LongClickHandler)(CCObject* pSender, CCTouch* pTouch);
typedef void					(CCObject::*SEL_AfterLongClickHandler)(CCObject* pSender, CCTouch* pTouch, float fDuration);
typedef void					(CCObject::*SEL_CheckHandler)(CCObject *pSender, bool bChecked);
typedef void					(CCObject::*SEL_ControlHandler)(CCObject* pSender, float fx, float fy);
typedef void					(CCObject::*SEL_ValueChangedHandler)(CCObject* pSender, int nValue);
typedef void					(CCObject::*SEL_ProgressEndedHandler)(CCObject* pSender);
typedef void					(CCObject::*SEL_ScrollingHandler)(CCObject* pSender);
typedef void					(CCObject::*SEL_PageChangedHandler)(CCObject* pSender, unsigned int nPageIdx);
typedef CCObject*				(CCObject::*SEL_DataSoucreAdapterHandler)(CCObject* pConvertCell, unsigned int uIdx);
typedef ENUM_WidgetTouchModel	(CCObject::*SEL_TouchBeganHandler)(CCObject* pSender, CCTouch* pTouch);
typedef bool					(CCObject::*SEL_TouchEventHandler)(CCObject* pSender, CCTouch* pTouch, float fDuration);
typedef void					(CCObject::*SEL_TextRichClickHandler)(CCObject* pSender, const char* description);
//-------------------------------------------------------------------------
#define fkcw_uiwidget_click_selector(_SELECTOR_)				(SEL_ClickHandler)(&_SELECTOR_)
#define fkcw_uiwidget_longclick_selector(_SELECTOR_)			(SEL_LongClickHandler)(&_SELECTOR_)
#define fkcw_uiwidget_afterlongclick_selector(_SELECTOR_)		(SEL_AfterLongClickHandler)(&_SELECTOR_)
#define fkcw_uiwidget_check_selector(_SELECTOR_)				(SEL_CheckHandler)(&_SELECTOR_)
#define fkcw_uiwidget_control_selector(_SELECTOR_)				(SEL_ControlHandler)(&_SELECTOR_)
#define fkcw_uiwidget_valuechanged_selector(_SELECTOR_)			(SEL_ValueChangedHandler)(&_SELECTOR_)
#define fkcw_uiwidget_progressended_selector(_SELECTOR_)		(SEL_ProgressEndedHandler)(&_SELECTOR_)
#define fkcw_uiwidget_scrolling_selector(_SELECTOR_)			(SEL_ScrollingHandler)(&_SELECTOR_)
#define fkcw_uiwidget_pagechanged_selector(_SELECTOR_)			(SEL_PageChangedHandler)(&_SELECTOR_)
#define fkcw_uiwidget_datasource_adapter_selector(__SELECTOR__) (SEL_DataSoucreAdapterHandler)(&__SELECTOR__)
#define fkcw_uiwidget_touchbegan_selector(_SELECTOR_)			(SEL_TouchBeganHandler)(&_SELECTOR_)
#define fkcw_uiwidget_touchevent_selector(_SELECTOR_)			(SEL_TouchEventHandler)(&_SELECTOR_)
#define fkcw_uiwidget_textrichclick_selector(_SELECTOR_)		(SEL_TextRichClickHandler)(&_SELECTOR_)
//-------------------------------------------------------------------------
// 控件触摸消息
class FKCW_UIWidget_TouchProtocol
{
public:
    virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch) = 0;
    virtual void onTouchMoved(CCTouch* pTouch, float fDuration) = 0;
    virtual void onTouchEnded(CCTouch* pTouch, float fDuration) = 0;
    virtual void onTouchCancelled(CCTouch* pTouch, float fDuration) = 0;
};
//-------------------------------------------------------------------------
// 数据格处理消息
class FKCW_UIWidget_DataSourceAdapterProtocol
{
public:
	FKCW_UIWidget_DataSourceAdapterProtocol();
	virtual ~FKCW_UIWidget_DataSourceAdapterProtocol();
	virtual void setDataSourceAdapter(CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);

protected:
	CCObject* m_pDataSourceAdapterListener;
	SEL_DataSoucreAdapterHandler m_pDataSourceAdapterHandler;
	CCObject* executeDataSourceAdapterHandler(CCObject* pConvertCell, unsigned int uIdx);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nDataSourceAdapterScriptHandler;
	CCObject* executeDataSourceAdapterScriptHandler(CCObject* pConvertCell, unsigned int uIdx);
public:
	virtual void setDataSourceAdapterScriptHandler(int nHandler);
	virtual void removeDataSourceAdapterScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// RichText超链接按下处理消息
class FKCW_UIWidget_TextRichClickableProtocol
{
public:
	FKCW_UIWidget_TextRichClickableProtocol();
	virtual ~FKCW_UIWidget_TextRichClickableProtocol();
	void setOnTextRichClickListener(CCObject* pListener, SEL_TextRichClickHandler pHandler);

protected:
	CCObject* m_pRichTextClickListener;
	SEL_TextRichClickHandler m_pRichTextClickHandler;
	void executeTextRichClickHandler(CCObject* pSender, const char* pDescription);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nRichTextClickScriptHandler;
	void executeTextRichScriptHandler(CCObject* pSender, const char* pDescription);
public:
	virtual void setOnTextRichClickScriptHandler(int nHandler);
	virtual void removeOnTextRichClickScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 容器层消息
class FKCW_UIWidget_LayoutableProtocol
{
public:
	FKCW_UIWidget_LayoutableProtocol();
	virtual ~FKCW_UIWidget_LayoutableProtocol();

protected:
	FKCW_UIWidget_Widget*	m_pSelectedWidget;
    ENUM_WidgetTouchModel	m_eSelectedWidgetTouchModel;
};
//-------------------------------------------------------------------------
// 翻页消息
class FKCW_UIWidget_PageChangeableProtocol
{
public:
	FKCW_UIWidget_PageChangeableProtocol();
	virtual ~FKCW_UIWidget_PageChangeableProtocol();
	void setOnPageChangedListener(CCObject* pListener, SEL_PageChangedHandler pHandler);

protected:
	unsigned int m_nPageIndex;
	CCObject* m_pPageChangedListener;
	SEL_PageChangedHandler m_pPageChangedHandler;
	void executePageChangedHandler(CCObject* pSender, unsigned int uPageIdx);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nPageChangedScriptHandler;
	void executePageChangedScriptHandler(CCObject* pSender, unsigned int uPageIdx);
public:
	virtual void setOnPageChangedScriptHandler(int nHandler);
	virtual void removeOnPageChangedScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 滚动条消息
class FKCW_UIWidget_ScrollableProtocol
{
public:
	FKCW_UIWidget_ScrollableProtocol();
	virtual ~FKCW_UIWidget_ScrollableProtocol();
	void setOnScrollingListener(CCObject* pListener, SEL_ScrollingHandler pHandler);

protected:
	CCObject* m_pScrollingListener;
	SEL_ScrollingHandler m_pScrollingHandler;
	void executeScrollingHandler(CCObject* pSender);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nScrollingScriptHandler;
	void executeScrollingScriptHandler(CCObject* pSender);
public:
	virtual void setOnScrollingScriptHandler(int nHandler);
	virtual void removeOnScrollingScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 进度条到结束消息
class FKCW_UIWidget_ProgressEndedProtocol
{
public:
	FKCW_UIWidget_ProgressEndedProtocol();
	virtual ~FKCW_UIWidget_ProgressEndedProtocol();
	void setOnProgressEndedListener(CCObject* pListener, SEL_ProgressEndedHandler pHandler);

protected:
	CCObject* m_pProgressEndedListener;
	SEL_ProgressEndedHandler m_pProgressEndedHandler;
	void executeProgressEndedHandler(CCObject* pSender);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nProgressEndedScriptHandler;
	void executeProgressEndedScriptHandler(CCObject* pSender);
public:
	virtual void setOnProgressEndedScriptHandler(int nHandler);
	virtual void removeOnProgressEndedScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 值更变消息
class FKCW_UIWidget_ValueChangeableProtocol
{
public:
	FKCW_UIWidget_ValueChangeableProtocol();
	virtual ~FKCW_UIWidget_ValueChangeableProtocol();
	void setOnValueChangedListener(CCObject* pListener, SEL_ValueChangedHandler pHandler);

protected:
	int m_nValue;
	CCObject* m_pValueChangedListener;
	SEL_ValueChangedHandler m_pValueChangedHandler;
	void executeValueChangedHandler(CCObject* pSender, int nValue);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nValueChangedScriptHandler;
	void executeValueChangedScriptHandler(CCObject* pSender, int nValue);
public:
	virtual void setOnValueChangedScriptHandler(int nHandler);
	virtual void removeOnValueChangedScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 点击消息
class FKCW_UIWidget_ClickableProtocol
{
public:
	FKCW_UIWidget_ClickableProtocol();
	virtual ~FKCW_UIWidget_ClickableProtocol();
	void setOnClickListener(CCObject* pListener, SEL_ClickHandler pHandler);

protected:
	CCObject* m_pClickListener;
	SEL_ClickHandler m_pClickHandler;
	void executeClickHandler(CCObject* pSender);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nClickScriptHandler;
	void executeClickScriptHandler(CCObject* pSender);
public:
	virtual void setOnClickScriptHandler(int nHandler);
	virtual void removeOnClickScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 长按消息
class FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_LongClickableProtocol();
	virtual ~FKCW_UIWidget_LongClickableProtocol();
	void setOnLongClickListener(CCObject* pListener, SEL_LongClickHandler pHandler);

protected:
	CCObject* m_pLongClickListener;
	SEL_LongClickHandler m_pLongClickHandler;
	bool executeLongClickHandler(CCObject* pSender, CCTouch* pTouch);

protected:
	bool m_bLongClickedUpdate;
	float m_fLongClickTimeCounter;
	bool m_bLongClickEnabled;

	float m_fLongClickLastTouchDuration;
	CCTouch* m_pLongClickLastTouch;
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nLongClickScriptHandler;
	bool executeLongClickScriptHandler(CCObject* pSender, CCTouch* pTouch);
public:
	virtual void setOnLongClickScriptHandler(int nHandler);
	virtual void removeOnLongClickScriptHandler();
#endif
};
//-------------------------------------------------------------------------
// 点选check消息
class FKCW_UIWidget_CheckableProtocol
{
public:
	FKCW_UIWidget_CheckableProtocol();
	virtual ~FKCW_UIWidget_CheckableProtocol();
	virtual void setChecked(bool bChecked);
	bool isChecked() const;
	virtual void setExclusion(int nExclusion);
	int getExclusion() const;
	void setOnCheckListener(CCObject* pListener, SEL_CheckHandler pHandler);

protected:
	virtual void executeCheckHandler(CCObject* pSender, bool bChecked);
	void setUnCheckedForOtherWidgetsByExclusion(CCNode* pParent);

protected:
	bool m_bChecked;
	int  m_nExclusion;
	CCObject* m_pCheckListener;
	SEL_CheckHandler m_pCheckHandler;
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nCheckScriptHandler;
	void executeCheckScriptHandler(CCObject* pSender, bool bChecked);
public:
	virtual void setOnCheckScriptHandler(int nHandler);
	virtual void removeCheckScriptHandler();
#endif
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_ControlableProtocol
{
public:
	FKCW_UIWidget_ControlableProtocol();
	virtual ~FKCW_UIWidget_ControlableProtocol();
	void setOnControlListener(CCObject* pListener, SEL_ControlHandler pHandler);

protected:
	CCObject* m_pControlListener;
	SEL_ControlHandler m_pControlHandler;
	void executeControlHandler(CCObject* pSender, float cx, float cy);
#if FKCW_WIDGET_USING_LUA
protected:
	int m_nControlScriptHandler;
	void executeControlScriptHandler(CCObject* pSender, float cx, float cy);
public:
	virtual void setOnControlScriptHandler(int nHandler);
	virtual void removeOnControlScriptHandler();
#endif
};
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_LONGCLICK_SCHEDULE
#define FKCW_UIWIDGET_LONGCLICK_SCHEDULE(__CHILDE__) \
protected: \
\
void performLongClickUpdate() \
{ \
	schedule(schedule_selector(__CHILDE__::onLongClickUpdate)); \
} \
\
void stopLongClickUpdate() \
{ \
	unschedule(schedule_selector(__CHILDE__::onLongClickUpdate)); \
} \
\
void onLongClickUpdate(float dt) \
{ \
	if( m_bLongClickedUpdate && m_bLongClickEnabled ) \
	{ \
		if( m_fLongClickTimeCounter > FKCW_UIWIDGET_LONGPRESS_TIME ) \
		{ \
			CCTouch* pTempTouch = m_pLongClickLastTouch; \
			interruptTouchCascade(pTempTouch, m_fLongClickLastTouchDuration); \
			if( executeLongClickHandler(this, pTempTouch) ) \
			{ \
				setLongClickTouchHandlerWidget(this, pTempTouch->getID()); \
			} \
			m_bLongClickedUpdate = false; \
			stopLongClickUpdate(); \
		} \
		else \
		{ \
			m_fLongClickTimeCounter += dt; \
		} \
	} \
}
#endif //FKCW_UIWIDGET_LONGCLICK_SCHEDULE
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN
#define FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN \
m_pLongClickLastTouch = NULL; \
m_fLongClickTimeCounter = 0.0f; \
if( m_bLongClickEnabled ) \
{ \
	m_pLongClickLastTouch = pTouch; \
	m_fLongClickLastTouchDuration = 0.0f; \
	m_bLongClickedUpdate = true; \
	performLongClickUpdate(); \
}
#endif //FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED
#define FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED \
if( m_bLongClickEnabled ) \
{ \
	m_pLongClickLastTouch = pTouch; \
	m_fLongClickLastTouchDuration = fDuration; \
	\
	CCPoint tMovePoint = getParent()->convertToNodeSpace(pTouch->getLocation()); \
	if( !boundingBox().containsPoint(tMovePoint) ) \
	{ \
		m_bLongClickedUpdate = false; \
		stopLongClickUpdate(); \
	} \
}
#endif //FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED
#define FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED \
if( m_bLongClickEnabled ) \
{ \
	m_fLongClickTimeCounter = 0.0f; \
	m_pLongClickLastTouch = NULL; \
	m_bLongClickedUpdate = false; \
	stopLongClickUpdate(); \
}
#endif //FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED
#define FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED \
if( m_bLongClickEnabled ) \
{ \
	m_fLongClickTimeCounter = 0.0f; \
	m_pLongClickLastTouch = NULL; \
	m_bLongClickedUpdate = false; \
	stopLongClickUpdate(); \
}
#endif //FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_UPDATE_BACKGROUND_POS
#define FKCW_UIWIDGET_UPDATE_BACKGROUND_POS \
if(m_pBackgroundImage) m_pBackgroundImage->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
if(m_pBackgroundColor) m_pBackgroundColor->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
if(m_pBackgroundColor) m_pBackgroundColor->setContentSize(m_obContentSize); \
if(m_pBackgroundGradient) m_pBackgroundGradient->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
if(m_pBackgroundGradient) m_pBackgroundGradient->setContentSize(m_obContentSize);
#endif //FKCW_UIWIDGET_UPDATE_BACKGROUND_POS
//-------------------------------------------------------------------------
#ifndef FKCW_UIWIDGET_BACKGROUND
#define FKCW_UIWIDGET_BACKGROUND \
public: \
	void setBackgroundImage(const char* pFile) \
	{ \
		CCAssert(pFile && strlen(pFile), "file path should not null"); \
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile); \
		setBackgroundTexture(pTexture); \
	} \
	\
	void setBackgroundTexture(CCTexture2D* pTexture) \
	{ \
		if( m_pBackgroundColor ) \
		{ \
			removeChild(m_pBackgroundColor); \
			m_pBackgroundColor = NULL; \
		} \
		\
		if( m_pBackgroundGradient ) \
		{ \
			removeChild(m_pBackgroundGradient); \
			m_pBackgroundGradient = NULL; \
		} \
		\
		if( m_pBackgroundImage ) \
		{ \
			CCRect rect; \
			rect.origin = CCPointZero; \
			rect.size = pTexture->getContentSize(); \
			\
			m_pBackgroundImage->setTexture(pTexture); \
			m_pBackgroundImage->setTextureRect(rect); \
		} \
		else \
		{ \
			m_pBackgroundImage = CCSprite::createWithTexture(pTexture); \
			m_pBackgroundImage->setZOrder(-128); \
			addChild(m_pBackgroundImage); \
		} \
		m_pBackgroundImage->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
	} \
	\
	void setBackgroundSpriteFrame(CCSpriteFrame* pSpriteFrame) \
	{ \
		if( m_pBackgroundColor ) \
		{ \
			removeChild(m_pBackgroundColor); \
			m_pBackgroundColor = NULL; \
		} \
		\
		if( m_pBackgroundGradient ) \
		{ \
			removeChild(m_pBackgroundGradient); \
			m_pBackgroundGradient = NULL; \
		} \
		\
		if( m_pBackgroundImage ) \
		{ \
			m_pBackgroundImage->setDisplayFrame(pSpriteFrame); \
		} \
		else \
		{ \
			m_pBackgroundImage = CCSprite::createWithSpriteFrame(pSpriteFrame); \
			m_pBackgroundImage->setZOrder(-128); \
			addChild(m_pBackgroundImage); \
		} \
		m_pBackgroundImage->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
	} \
	\
	void setBackgroundSpriteFrameName(const char* pFrameName) \
	{ \
		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pFrameName); \
		return setBackgroundSpriteFrame(pFrame); \
	} \
	\
	CCSprite* getBackgroundImage() const \
	{ \
		return m_pBackgroundImage; \
	} \
	\
	void removeBackgroundImage() \
	{ \
		if( m_pBackgroundImage ) \
		{ \
			removeChild(m_pBackgroundImage); \
			m_pBackgroundImage = NULL; \
		} \
	} \
	\
protected: \
	CCSprite* m_pBackgroundImage; \
	\
public: \
	void setBackgroundColor(const ccColor4B& tColor) \
	{ \
		if( m_pBackgroundImage ) \
		{ \
			removeChild(m_pBackgroundImage); \
			m_pBackgroundImage = NULL; \
		} \
		\
		if( m_pBackgroundGradient ) \
		{ \
			removeChild(m_pBackgroundGradient); \
			m_pBackgroundGradient = NULL; \
		} \
		\
		if( m_pBackgroundColor ) \
		{ \
			m_pBackgroundColor->setColor(ccc3(tColor.r, tColor.g, tColor.b)); \
			m_pBackgroundColor->setOpacity(tColor.a); \
		} \
		else \
		{ \
			m_pBackgroundColor = FKCW_UIWidget_ColorView::create(tColor); \
			m_pBackgroundColor->setTouchEnabled(false); \
			m_pBackgroundColor->setZOrder(-128); \
			m_pBackgroundColor->setContentSize(m_obContentSize); \
			addChild(m_pBackgroundColor); \
		} \
		m_pBackgroundColor->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
	} \
	\
	void removeBackgroundColor() \
	{ \
		if( m_pBackgroundColor ) \
		{ \
			removeChild(m_pBackgroundColor); \
			m_pBackgroundColor = NULL; \
		} \
	} \
	\
protected: \
	FKCW_UIWidget_ColorView* m_pBackgroundColor; \
	\
public: \
	void setBackgroundGradient(const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& v) \
	{ \
		if( m_pBackgroundImage ) \
		{ \
			removeChild(m_pBackgroundImage); \
			m_pBackgroundImage = NULL; \
		} \
		\
		if( m_pBackgroundColor ) \
		{ \
			removeChild(m_pBackgroundColor); \
			m_pBackgroundColor = NULL; \
		} \
		\
		if( m_pBackgroundGradient ) \
		{ \
			m_pBackgroundGradient->setStartColor(ccc3(tStart.r, tStart.g, tStart.b)); \
			m_pBackgroundGradient->setEndColor(ccc3(tEnd.r, tEnd.g, tEnd.b)); \
			m_pBackgroundGradient->setStartOpacity(tStart.a); \
			m_pBackgroundGradient->setEndOpacity(tEnd.a); \
			m_pBackgroundGradient->setVector(v); \
		} \
		else \
		{ \
			m_pBackgroundGradient = FKCW_UIWidget_GradientView::create(tStart, tEnd, v); \
			m_pBackgroundGradient->setTouchEnabled(false); \
			m_pBackgroundGradient->setZOrder(-128); \
			m_pBackgroundGradient->setContentSize(m_obContentSize); \
			addChild(m_pBackgroundGradient); \
		} \
		m_pBackgroundGradient->setPosition(CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2)); \
	} \
	\
	void removeBackgroundGradient() \
	{ \
		if( m_pBackgroundGradient ) \
		{ \
			removeChild(m_pBackgroundGradient); \
			m_pBackgroundGradient = NULL; \
		} \
	} \
protected: \
	FKCW_UIWidget_GradientView* m_pBackgroundGradient; \
	\
public: \
	void setBackgroundOpacity(GLbyte opacity) \
	{ \
		if( m_pBackgroundImage ) \
		{ \
			m_pBackgroundImage->setOpacity(opacity); \
			return; \
		} \
		\
		if( m_pBackgroundColor ) \
		{ \
			m_pBackgroundColor->setOpacity(opacity); \
			return; \
		} \
		\
		if( m_pBackgroundGradient ) \
		{ \
			m_pBackgroundGradient->setOpacity(opacity);\
			return; \
		} \
	}
#endif //FKCW_UIWIDGET_BACKGROUND
//-------------------------------------------------------------------------