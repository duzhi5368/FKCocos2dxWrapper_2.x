//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_UI_MenuItemColor.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	一个纯颜色样式的菜单项
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
class FKCW_UI_MenuItemColorStateListener;
// 一个纯颜色样式的菜单项
// 注：因为该菜单不基于纹理贴图，所以你需要指定其大小
class FKCW_UI_MenuItemColor : public CCMenuItem, public CCBlendProtocol 
{
private:
	ccColor4B				m_pNormalColor;			// 常态下的颜色
	ccColor4B				m_pSelectedColor;		// 选中的颜色
	ccColor4B				m_pDisabledColor;		// 无效时的颜色
	ccColor4B				m_pFocusColor;			// 获得焦点时的颜色

	ccVertex2F				m_pSquareVertices[4];	// 顶点
	ccColor4F				m_pSquareColors[4];		// 颜色

	// 事件状态监听器
	CC_SYNTHESIZE(FKCW_UI_MenuItemColorStateListener*, m_pStateListener, StateListener);
	// 是否焦点状态
	CC_SYNTHESIZE_BOOL_SETTER(m_bIsFocus, Focus);
	// 混合方式
	CC_PROPERTY(ccBlendFunc, m_tagBlendFunc, BlendFunc);
protected:
	FKCW_UI_MenuItemColor();
	// 更新颜色
	virtual void updateColor();
public:
	virtual ~FKCW_UI_MenuItemColor();

	static FKCW_UI_MenuItemColor* Create(ccColor4B normal);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B focus);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B selected, ccColor4B focus);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B focus, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B selected, ccColor4B focus, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_MenuItemColor* Create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector);

	// 初始化
	void InitWithColorAndTarget(ccColor4B normal, ccColor4B focus, ccColor4B selected, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector);

	// 父类继承函数
	virtual void draw();
	virtual void setContentSize(const CCSize& size);
	virtual void selected();
	virtual void unselected();
	virtual void setEnabled(bool value);

	// 不触发状态事件的函数
	void SelectedSilent();
	void UnselectedSilent();
	void SetEnabledSilent(bool value);
	void SetFocusSilent(bool flag);
};