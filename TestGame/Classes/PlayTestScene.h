//*************************************************************************
//	创建日期:	2015-3-19
//	文件名称:	AudioTestScene.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:  消除模块 - Play
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
#include "StarManger.h"
//-------------------------------------------------------------------------
class CPlayTestScene :
	public FKCW_SceneExMgr_SceneEx,public IMessageHandler
{
public:
	CREATE_SCENE_FUNC(CPlayTestScene);
public:
	CPlayTestScene();
	// 初始异步加载资源
	virtual void onLoadResources();
	// 其他逻辑初始化
	virtual void onLoadScene();
	// 当场景显示时调用
	virtual void onEnterScene();
	// 当场景隐藏时调用
	virtual void onExitScene();
	// 接收到消息[注意，收到的消息内，包含子消息码，但不包含主消息码]
	// 参数：szMsgData 收到的数据指针
	// 参数：nLen 收到的数据长度
	virtual void OnNetMessage( const char* szMsgData, int nLen );

public:

	void	onClick(CCObject* pSender);
	void	OnSelectBtn(CCObject* pSender);
	bool	OnLabelTouchEnded(CCObject* pSender, CCTouch* pTouch, float fDuration);
private:

	void	__AddListenNetMessage();
	void	__RemovListenNetMessage();
	void	__CreatePlayUI();
	void	__CreateBGUI();
	void	__CreateStar();
	FKCW_UIWidget_Button* __CreateButton(
		int nType,CCPoint& pos,const char* pszImageName,const char* pszImageSecectName,const char* pszName);

private:
	FKCW_UIWidget_WidgetWindow*	m_pRootWindow;
	FKCW_UIWidget_WidgetWindow* m_pStarWindow;
	CStarManger					m_StarMgr;
	FKCW_UIWidget_WidgetWindow* m_pEffectsWindow;
	FKCW_UIWidget_Label*		m_pMoneyLabel;
	FKCW_UIWidget_ImageView*	m_pItemBgImage;
	FKCW_UIWidget_ImageView*	m_pSoundBtnHideImage;
	FKCW_UIWidget_LabelAtlas*	m_pChapterLabel;
	FKCW_UIWidget_LabelAtlas*	m_pTargetLabel;
	FKCW_UIWidget_LabelAtlas*	m_pCurrentLabel;

public:
};

