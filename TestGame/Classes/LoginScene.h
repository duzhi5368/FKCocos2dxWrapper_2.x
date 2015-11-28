//*************************************************************************
//	创建日期:	2015-1-15
//	文件名称:	LoginScene.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
#include "MsgBoxMgr.h"
//-------------------------------------------------------------------------
class CLoginScene : public FKCW_SceneExMgr_SceneEx,public IMsgBoxHandle
{
public:
	CREATE_SCENE_FUNC(CLoginScene);
public:
	CLoginScene();
	// 初始异步加载资源
	virtual void onLoadResources();
	// 其他逻辑初始化
	virtual void onLoadScene();
	// 当场景显示时调用
	virtual void onEnterScene();
	// 当场景隐藏时调用
	virtual void onExitScene();
	// MessageBox按下后的回调消息
	virtual void OnMessageBoxCallBack(int nMessageKey);

public:
	// 退出游戏按钮
	void OnExitBtn(CCObject* pSender);
	// 功能按钮回调 - 内部测试了IEventSystem
	void OnSelectBtn(CCObject* pSender);

private:
	// 创建登录时全部UI
	void __CreateMainUI();
	// 便捷封装，创建单个button
	FKCW_UIWidget_Button* __CreateButton(
		int nType,CCPoint& pos,const char* pszImageName,
		const char* pszImageSecectName,const char* pszName);
};