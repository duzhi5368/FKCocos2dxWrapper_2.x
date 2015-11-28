//*************************************************************************
//	创建日期:	2015-3-23
//	文件名称:	AchieTestScene.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	成就系统
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CAchieTestScene :
	public FKCW_SceneExMgr_SceneEx
{
public:
	CREATE_SCENE_FUNC(CAchieTestScene);
public:
	CAchieTestScene();
	// 初始异步加载资源
	virtual void onLoadResources();
	// 其他逻辑初始化
	virtual void onLoadScene();
	// 当场景显示时调用
	virtual void onEnterScene();
	// 当场景隐藏时调用
	virtual void onExitScene();

	//载入本地csv成就数据
	virtual void onLoadCVS();
	//将csv成就数据加载到ListViews上
	virtual void _AddToList(const char* pStr);

public:

	void	onClick(CCObject* pSender);

public:
	FKCW_UIWidget_ImageView* m_pBackGroundImage;
	FKCW_Base_CSVLoader*		m_pLoader;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCEditBox*					m_pFileEdit;
	FKCW_UIWidget_ListView*		m_pListView;

};

