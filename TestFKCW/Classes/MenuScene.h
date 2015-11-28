//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	MenuScene.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "ListControllerLayer.h"
//-------------------------------------------------------------------------
class CMenuScene : public FKCW_SceneExMgr_SceneEx
{
public:
	CREATE_SCENE_FUNC(CMenuScene);
public:
	CMenuScene();
	virtual ~CMenuScene();

	virtual void onLoadScene();
private:
	CListControllerLayer*		m_pLayer;
};