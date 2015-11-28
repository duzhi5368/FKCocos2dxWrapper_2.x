//*************************************************************************
//	创建日期:	2015-3-23
//	文件名称:	ShopTestScene.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	商城系统
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
//-------------------------------------------------------------------------
class CShopTestScene :
	public FKCW_SceneExMgr_SceneEx
{
public:
	CREATE_SCENE_FUNC(CShopTestScene);
public:
	CShopTestScene();
	// 初始异步加载资源
	virtual void onLoadResources();
	// 其他逻辑初始化
	virtual void onLoadScene();
	// 当场景显示时调用
	virtual void onEnterScene();
	// 当场景隐藏时调用
	virtual void onExitScene();

public:
	void	onClick(CCObject* pSender);

private:

	FKCW_UIWidget_WidgetWindow*	m_pRootWindow;
};

