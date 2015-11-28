//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_ColorView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_ColorView 
	: public CCNodeRGBA
	, public FKCW_UIWidget_Widget
	, public CCBlendProtocol
	, public FKCW_UIWidget_ClickableProtocol
	, public FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_ColorView();
	virtual ~FKCW_UIWidget_ColorView();
	virtual bool init();
	virtual bool initWithColor(const ccColor4B& color);
	static FKCW_UIWidget_ColorView* create();
	static FKCW_UIWidget_ColorView* create(const ccColor4B& color);

	virtual void setContentSize(const CCSize& size);
	virtual void setColor(const ccColor3B& color);
	virtual void setOpacity(GLubyte opacity);
	virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
	virtual void setCascadeColorEnabled(bool cascadeColorEnabled);
	virtual GLubyte getOpacity();
	virtual GLubyte getDisplayedOpacity();
	virtual const ccColor3B& getColor();
	virtual const ccColor3B& getDisplayedColor();
	virtual bool isCascadeColorEnabled();
	virtual bool isCascadeOpacityEnabled();

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

	virtual void updateDisplayedOpacity(GLubyte parentOpacity);
	virtual void updateDisplayedColor(const ccColor3B& parentColor);
	virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool isOpacityModifyRGB() { return false; }
	virtual void draw();
	virtual ccBlendFunc getBlendFunc(){ return m_tBlendFunc; }
	virtual void setBlendFunc(ccBlendFunc tBlendFunc){ m_tBlendFunc = tBlendFunc; }

	FKCW_UIWIDGET_LONGCLICK_SCHEDULE(FKCW_UIWidget_ColorView);

protected:
	ccBlendFunc m_tBlendFunc;

protected:
	ccVertex2F m_pSquareVertices[4];
	ccColor4F  m_pSquareColors[4];
	virtual void updateColor();
};