//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_UI_AutoRenderMenuItemSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	一个菜单项精灵，能够自动呈现选中状态
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
class FKCW_UI_AutoRenderMenuItemSprite : public CCMenuItemSprite
{
private:
	ccColor3B				m_pOldColor;				// 之前的颜色
	bool					m_bIsFocus;					// 是否当前是焦点
	CCNode*					m_pFocusImage;				// 焦点状态的图片

	CCObject*				m_pSelectedEventTarget;		// 被选中之后的事件
	SEL_MenuHandler			m_pSelectedEventSelector;	// 被选中之后的事件

	CCObject*				m_pUnselectedEventTarget;	// 取消选中的事件
	SEL_MenuHandler			m_pUnselectedEventSelector;	// 取消选中的事件

	CC_SYNTHESIZE_BOOL(m_bIsFocusIsAttachment, FocusIsAttachment);	// true表示焦点状态可以和其他三种状态同时出现
protected:
	virtual void			updateImagesVisibility();
	// 让全状态的图片中心对齐
	void					_CenterAlignImgaes();
public:
	FKCW_UI_AutoRenderMenuItemSprite();
	virtual ~FKCW_UI_AutoRenderMenuItemSprite();
	// 创建一个菜单选项
	// 参数：sprite 正常状态下的精灵，target 消息事件通知对象，selector 选中事件回调函数
	static FKCW_UI_AutoRenderMenuItemSprite* Create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector);
	// 参数：normalImage 正常状态下的精灵图片，focusImage 获得焦点状态下的精灵图片
	static FKCW_UI_AutoRenderMenuItemSprite* Create(CCNode* normalImage, CCNode* focusImage);
	static FKCW_UI_AutoRenderMenuItemSprite* Create(CCNode* normalImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_AutoRenderMenuItemSprite* Create(CCNode* normalImage, CCNode* selectedImage,
		CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_AutoRenderMenuItemSprite* Create(CCNode* normalImage, CCNode* selectedImage, 
		CCNode* disabledImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector);

	// 设置选中事件消息对象和回调
	void	SetSelectedEvent(CCObject* target, SEL_MenuHandler selector);
	// 设置取消选中事件消息对象和回调
	void	SetUnselectedEvent(CCObject* target, SEL_MenuHandler selector);
	
	virtual void selected();
	virtual void unselected();

	// 设置为焦点/取消焦点
	void	SetFocus( bool p_bFlag );
	// 是否是焦点
	bool	IsFocus();
	// 设置焦点状态图片
	void	SetFocusImage( CCNode* focusImage );
	// 获取焦点状态图片
	CCNode*	GetFocusImage();
};