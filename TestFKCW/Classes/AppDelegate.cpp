//*************************************************************************
//	创建日期:	2014-11-26
//	文件名称:	AppDelegate.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "MenuScene.h"
//--------------------------------------------------------
USING_NS_CC;
using namespace CocosDenshion;
//--------------------------------------------------------
AppDelegate::AppDelegate() {

}
//--------------------------------------------------------
AppDelegate::~AppDelegate() 
{
	cocos2d::extension::CCArmatureDataManager::purge();
	cocos2d::extension::DictionaryHelper::purgeDictionaryHelper();
}
//--------------------------------------------------------
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    // turn on display FPS
    pDirector->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	// create FK windows console
	FKCW_Base_Utils::CreateWinConsole();

	// add resouce dir
	CCFileUtils::sharedFileUtils()->addSearchPath( "Resources" );

	//  register menu scene
	REGISTER_SCENE_FUNC(CMenuScene);
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->runWithScene( LoadScene("CMenuScene") );

    return true;
}
//--------------------------------------------------------
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
	CCDirector::sharedDirector()->stopAnimation();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}
//--------------------------------------------------------
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	CCDirector::sharedDirector()->startAnimation();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
//--------------------------------------------------------