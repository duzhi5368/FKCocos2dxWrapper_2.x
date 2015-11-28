//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_AutoRenderMenuItemSprite.h"
#include "../../FKCW_Base/Include/FKCW_Base_ColorConvert.h"
//-------------------------------------------------------------------------
void FKCW_UI_AutoRenderMenuItemSprite::updateImagesVisibility()
{
	if (m_pNormalImage)
		m_pNormalImage->setVisible(false);
	if (m_pSelectedImage)
		m_pSelectedImage->setVisible(false);
	if (m_pDisabledImage)
		m_pDisabledImage->setVisible(false);
	if (m_pFocusImage)
		m_pFocusImage->setVisible(false);

	if(m_pFocusImage) 
	{
		if(m_bIsFocusIsAttachment) 
		{
			m_pFocusImage->setVisible(m_bIsFocus || isSelected());
		} 
		else 
		{
			m_pFocusImage->setVisible(m_bIsFocus);
		}
	}
	CCMenuItemSprite::updateImagesVisibility();
}
//-------------------------------------------------------------------------
// 让全状态的图片中心对齐
void FKCW_UI_AutoRenderMenuItemSprite::_CenterAlignImgaes()
{
	CCSize s = CCSizeZero;
	if(m_pNormalImage) {
		CCSize _s = m_pNormalImage->getContentSize();
		s.width = MAX(s.width, _s.width);
		s.height = MAX(s.height, _s.height);
	}
	if(m_pSelectedImage) {
		CCSize _s = m_pSelectedImage->getContentSize();
		s.width = MAX(s.width, _s.width);
		s.height = MAX(s.height, _s.height);
	}
	if(m_pFocusImage) {
		CCSize _s = m_pFocusImage->getContentSize();
		s.width = MAX(s.width, _s.width);
		s.height = MAX(s.height, _s.height);
	}
	if(m_pDisabledImage) {
		CCSize _s = m_pDisabledImage->getContentSize();
		s.width = MAX(s.width, _s.width);
		s.height = MAX(s.height, _s.height);
	}
	setContentSize(s);

	if(m_pNormalImage) {
		CCSize _s = m_pNormalImage->getContentSize();
		m_pNormalImage->setPosition(ccp((s.width - _s.width) / 2,
			(s.height - _s.height) / 2));
	}
	if(m_pSelectedImage) {
		CCSize _s = m_pSelectedImage->getContentSize();
		m_pSelectedImage->setPosition(ccp((s.width - _s.width) / 2,
			(s.height - _s.height) / 2));
	}
	if(m_pFocusImage) {
		CCSize _s = m_pFocusImage->getContentSize();
		m_pFocusImage->setPosition(ccp((s.width - _s.width) / 2,
			(s.height - _s.height) / 2));
	}
	if(m_pDisabledImage) {
		CCSize _s = m_pDisabledImage->getContentSize();
		m_pDisabledImage->setPosition(ccp((s.width - _s.width) / 2,
			(s.height - _s.height) / 2));
	}
}
//-------------------------------------------------------------------------
FKCW_UI_AutoRenderMenuItemSprite::FKCW_UI_AutoRenderMenuItemSprite()
	: m_bIsFocus( false )
	, m_bIsFocusIsAttachment( false )
	, m_pFocusImage( NULL )
	, m_pSelectedEventSelector( NULL )
	, m_pSelectedEventTarget( NULL )
	, m_pUnselectedEventSelector( NULL )
	, m_pUnselectedEventTarget( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_UI_AutoRenderMenuItemSprite::~FKCW_UI_AutoRenderMenuItemSprite()
{

}
//-------------------------------------------------------------------------
// 创建一个菜单选项
// 参数：sprite 正常状态下的精灵，target 消息事件通知对象，selector 选中事件回调函数
FKCW_UI_AutoRenderMenuItemSprite* FKCW_UI_AutoRenderMenuItemSprite::Create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_AutoRenderMenuItemSprite* pRet = new FKCW_UI_AutoRenderMenuItemSprite();
	pRet->initWithNormalSprite(sprite, NULL, NULL, target, selector);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
// 参数：normalImage 正常状态下的精灵图片，focusImage 获得焦点状态下的精灵图片
FKCW_UI_AutoRenderMenuItemSprite* FKCW_UI_AutoRenderMenuItemSprite::Create(CCNode* normalImage, CCNode* focusImage)
{
	FKCW_UI_AutoRenderMenuItemSprite* pRet = new FKCW_UI_AutoRenderMenuItemSprite();
	pRet->initWithNormalSprite(normalImage, NULL, NULL, NULL, NULL);
	pRet->SetFocusImage(focusImage);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
FKCW_UI_AutoRenderMenuItemSprite* FKCW_UI_AutoRenderMenuItemSprite::Create(CCNode* normalImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_AutoRenderMenuItemSprite* pRet = new FKCW_UI_AutoRenderMenuItemSprite();
	pRet->initWithNormalSprite(normalImage, NULL, NULL, target, selector);
	pRet->SetFocusImage(focusImage);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
FKCW_UI_AutoRenderMenuItemSprite* FKCW_UI_AutoRenderMenuItemSprite::Create(CCNode* normalImage, CCNode* selectedImage,
	CCNode* focusImage, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_AutoRenderMenuItemSprite* pRet = new FKCW_UI_AutoRenderMenuItemSprite();
	pRet->initWithNormalSprite(normalImage, selectedImage, NULL, target, selector);
	pRet->SetFocusImage(focusImage);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
FKCW_UI_AutoRenderMenuItemSprite* FKCW_UI_AutoRenderMenuItemSprite::Create(CCNode* normalImage, CCNode* selectedImage, 
	CCNode* disabledImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_AutoRenderMenuItemSprite* pRet = new FKCW_UI_AutoRenderMenuItemSprite();
	pRet->initWithNormalSprite(normalImage, selectedImage, disabledImage, target, selector);
	pRet->SetFocusImage(focusImage);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
// 设置选中事件消息对象和回调
void FKCW_UI_AutoRenderMenuItemSprite::SetSelectedEvent(CCObject* target, SEL_MenuHandler selector)
{
	m_pSelectedEventTarget		= target;
	m_pSelectedEventSelector	= selector;
}
//-------------------------------------------------------------------------
// 设置取消选中事件消息对象和回调
void FKCW_UI_AutoRenderMenuItemSprite::SetUnselectedEvent(CCObject* target, SEL_MenuHandler selector)
{
	m_pUnselectedEventTarget	= target;
	m_pUnselectedEventSelector	= selector;
}
//-------------------------------------------------------------------------
void FKCW_UI_AutoRenderMenuItemSprite::selected()
{
	CCMenuItemSprite::selected();
	updateImagesVisibility();
	CCMenuItemSprite::selected();

	// 设置灰色
	if(!getSelectedImage()) 
	{
		m_pOldColor = getColor();
		SColorHSV hsv = C3B2HSV(m_pOldColor);
		hsv.v = MAX(0, hsv.v - 0.2f);
		ccColor3B c = HSV2C3B(hsv);
		setColor(c);
	}

	// 事件通知
	if (m_pSelectedEventTarget && m_pSelectedEventSelector) 
	{
		(m_pSelectedEventTarget->*m_pSelectedEventSelector)(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_AutoRenderMenuItemSprite::unselected()
{
	CCMenuItemSprite::unselected();
	updateImagesVisibility();

	// 还原原色
	if(!getSelectedImage()) 
	{
		setColor(m_pOldColor);
	}

	// 事件通知
	if (m_pUnselectedEventTarget && m_pUnselectedEventSelector) 
	{
		(m_pUnselectedEventTarget->*m_pUnselectedEventSelector)(this);
	}
}
//-------------------------------------------------------------------------
// 设置为焦点/取消焦点
void FKCW_UI_AutoRenderMenuItemSprite::SetFocus( bool p_bFlag )
{
	m_bIsFocus = p_bFlag;
	updateImagesVisibility();
}
//-------------------------------------------------------------------------
// 是否是焦点
bool FKCW_UI_AutoRenderMenuItemSprite::IsFocus()
{
	return m_bIsFocus;
}
//-------------------------------------------------------------------------
// 设置焦点状态图片
void FKCW_UI_AutoRenderMenuItemSprite::SetFocusImage( CCNode* focusImage )
{
	if (focusImage != m_pFocusImage) 
	{
		if (focusImage) 
		{
			addChild(focusImage, 0);
			focusImage->setAnchorPoint(ccp(0, 0));
		}

		if (m_pFocusImage) 
		{
			removeChild(m_pFocusImage, true);
		}

		m_pFocusImage = focusImage;
		updateImagesVisibility();
	}
}
//-------------------------------------------------------------------------
// 获取焦点状态图片
CCNode*	FKCW_UI_AutoRenderMenuItemSprite::GetFocusImage()
{
	return m_pFocusImage;
}
//-------------------------------------------------------------------------