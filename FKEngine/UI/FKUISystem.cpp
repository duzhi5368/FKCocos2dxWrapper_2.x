//-------------------------------------------------------------------------
#include "FKUISystem.h"
//-------------------------------------------------------------------------
FKUISystem gs_FKUISystem;
IUISystem* gs_pUISystem;
//-------------------------------------------------------------------------
IUISystem* GetUISystem()
{
	return gs_pUISystem;
}
//-------------------------------------------------------------------------
FKUISystem::FKUISystem()
{
	m_pRootWindow = FKCW_UIWidget_WidgetWindow::create();
	m_pRootWindow->retain();
	m_pRootWindow->setMultiTouchEnabled(true);

	gs_pUISystem = this;
}
//-------------------------------------------------------------------------
FKUISystem::~FKUISystem()
{
	gs_pUISystem = NULL;
}
//-------------------------------------------------------------------------
// 释放销毁
void FKUISystem::Release()
{
	m_pRootWindow->release();
}
//-------------------------------------------------------------------------
// 获取UI系统根节点窗口
FKCW_UIWidget_WidgetWindow*	FKUISystem::GetRoot() const
{
	return m_pRootWindow;
}
//-------------------------------------------------------------------------