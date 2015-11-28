/*!
 * \file:	TestScene.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
#include "VisibleRect.h"
#include "TestSceneTemplate.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//--------------------------------------------------------
USING_NS_CC;
using namespace std;
//--------------------------------------------------------
class CTestScene : public FKCW_SceneExMgr_SceneEx
{
public:
	virtual void SetInfo( std::string p_szInfo );
	virtual void onEnterScene();
	virtual void Run() = 0;
	// 返回主菜单时的回调
	virtual void MainMenuCallback(CCObject* pSender);
private:
	std::string				m_szInfo;
};
//--------------------------------------------------------