//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_Button.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Label.h"
#include "FKCW_UIWidget_Scale9Sprite.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_Button : public CCNodeRGBA, public FKCW_UIWidget_Widget,
	public FKCW_UIWidget_ClickableProtocol, public FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_Button();
	virtual ~FKCW_UIWidget_Button();

	virtual bool		init();
	virtual bool		initWith9Sprite(const CCSize& tSize, const char* pNormal, const char* pSelected, const char* pDisabled);
	virtual bool		initWithFile(const char* pNormal, const char* pSelected, const char* pDisabled);
	static FKCW_UIWidget_Button*		create();
	static FKCW_UIWidget_Button*		create(const char* pNormal, const char* pSelected = NULL, const char* pDisabled = NULL);
	static FKCW_UIWidget_Button*		createWith9Sprite(const CCSize& tSize, const char* pNormal, const char* pSelected = NULL, const char* pDisabled = NULL);

	virtual void		setContentSize(const CCSize& contentSize);
	virtual void		setEnabled(bool bEnabled);
	void				setLabelOffset(const CCPoint& tOffset);
	void				setText(const char* pText);
	const char*			getText();
	void				setTextFontName(const char* pTextFontName);
	const char*			getTextFontName();
	void				setTextFontSize(float fTextFontSize);
	float				getTextFontSize();
	void				setTextColor(const ccColor3B& color);
	const ccColor3B&	getTextColor();
	void				initText(const char* pText, const char* pFontName, float fFontSize);
	virtual FKCW_UIWidget_Label*	getLabel();
	virtual bool		isScale9Enabled();
	virtual void		setScale9Enabled(bool bEnabled);
	virtual void		setCascadeTextSizeEnabled(bool bEnabled, const CCSize& tPadding = CCSize(50, 30));

	virtual void		setNormalImage(const char* pFile);
	virtual void		setSelectedImage(const char* pFile);
	virtual void		setDisabledImage(const char* pFile);
	virtual void		setNormalTexture(CCTexture2D *pTexture);
	virtual void		setSelectedTexture(CCTexture2D *pTexture);
	virtual void		setDisabledTexture(CCTexture2D *pTexture);
	virtual void		setNormalSpriteFrameName(const char* pSpriteName);
	virtual void		setSelectedSpriteFrameName(const char* pSpriteName);
	virtual void		setDisabledSpriteFrameName(const char* pSpriteName);
	virtual void		setNormalSpriteFrame(CCSpriteFrame* pFrame);
	virtual void		setSelectedSpriteFrame(CCSpriteFrame* pFrame);
	virtual void		setDisabledSpriteFrame(CCSpriteFrame* pFrame);
	virtual CCNode*		getNormalImage();
	virtual CCNode*		getSelectedImage();
	virtual CCNode*		getDisabledImage();

	FKCW_UIWIDGET_LONGCLICK_SCHEDULE(FKCW_UIWidget_Button);

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

protected:
	void updateCascadeTextSize();

protected:
	CCNode*					m_pNormalImage;
	CCNode*					m_pSelectedImage;
	CCNode*					m_pDisabledImage;
	bool					m_bScale9Enabled;
	FKCW_UIWidget_Label*	m_pLabel;
	CCPoint					m_tTextOffset;
	bool					m_bCascadeTextSizeEnabled;
	CCSize					m_tCascadeTextSizePadding;
};