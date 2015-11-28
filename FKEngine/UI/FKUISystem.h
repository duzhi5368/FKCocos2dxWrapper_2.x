//*************************************************************************
//	创建日期:	2015-1-16
//	文件名称:	FKUISystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class CRootUIScene;
class FKUISystem : public IUISystem
{
public:
	FKUISystem();
	~FKUISystem();
public:
	// 释放销毁
	virtual void							Release();
	// 获取UI系统根节点窗口
	virtual FKCW_UIWidget_WidgetWindow*		GetRoot() const;
private:
	FKCW_UIWidget_WidgetWindow*				m_pRootWindow;
};
//-------------------------------------------------------------------------
extern FKUISystem gs_FKUISystem;
//-------------------------------------------------------------------------