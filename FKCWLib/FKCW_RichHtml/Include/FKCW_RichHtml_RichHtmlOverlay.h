//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlOverlay.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RichHtml_Macros.h"
#include "FKCW_RichHtml_BaseStruct.h"
#include "FKCW_RichHtml_HelpFunc.h"
#include "FKCW_RichHtml_Interface.h"
//-------------------------------------------------------------------------
class FKCW_RichHtml_RichOverlay : public CCLayer
{
public:
	static FKCW_RichHtml_RichOverlay* create();

	virtual bool init();
	virtual void append(class FKCW_RichHtml_IRichElement* ele);
	virtual void reset();

	virtual void draw();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerListener(void* target, FKCW_RichHtml_IRichEventHandler* listener);
	virtual void removeListener(void* target);

	FKCW_RichHtml_RichOverlay();
	virtual ~FKCW_RichHtml_RichOverlay();

private:
	FKCW_RichHtml_IRichNode* getContainer();

	std::list<class FKCW_RichHtml_EleHTMLTouchable*> m_elements;
	std::list<class FKCW_RichHtml_EleHTMLTouchable*> m_touchables;

	std::map<void*, FKCW_RichHtml_IRichEventHandler*> m_eventhandlers;
	class FKCW_RichHtml_EleHTMLTouchable* m_touched;
	class FKCW_RichHtml_IRichNode* m_container;
};