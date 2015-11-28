//-------------------------------------------------------------------------
#include "FKFramework.h"
#include "SimpleAudioEngine.h"
//-------------------------------------------------------------------------
FKFramework::FKFramework()
{

}
//-------------------------------------------------------------------------
FKFramework::~FKFramework()
{

}
//-------------------------------------------------------------------------
// 收到输入消息的回调处理函数
bool FKFramework::OnInputMessage( unsigned int unMsgType, int nTouchID, 
							   float fPosX, float fPosY, void* pParam )
{
	return true;
}
//-------------------------------------------------------------------------
// 初始化
bool FKFramework::Init( float fWidth, float fHeight )
{
	CCEGLView* pEglView = CCEGLView::sharedOpenGLView();
	pEglView->setViewName("FKEngine");
	pEglView->setFrameSize( fWidth, fHeight );
	return true;
}
//-------------------------------------------------------------------------
// 初始化工作
bool FKFramework::InitSystem()
{
	GetSceneSystem();
	GetAudioSystem();
	GetNetworkSystem();
	GetTimerSystem();
	GetMsgDispatchSystem();
	GetEventSystem();
	GetUISystem();
	GetInputSystem();
	GetDebugSystem();
	
	GetInputSystem()->Init();
	GetInputSystem()->SetInputMsgCallback( this );
	GetInputSystem()->SetInputEnabled( true );

	return SubInit();
}
//-------------------------------------------------------------------------
// 帧更新
void FKFramework::UpdateFrame( float dt )
{
	GetNetworkSystem()->UpdateEvent();
	GetTimerSystem()->UpdateTimer();
	SubUpdate( dt );
}
//-------------------------------------------------------------------------
// 释放工作
void FKFramework::Release()
{
	SubRelease();

	cocos2d::extension::CCArmatureDataManager::purge();
	cocos2d::extension::DictionaryHelper::purgeDictionaryHelper();
	GetInputSystem()->SetInputEnabled( false );

	GetDebugSystem()->Release();
	GetInputSystem()->Release();
	GetUISystem()->Release();
	GetEventSystem()->Release();
	GetMsgDispatchSystem()->Release();
	GetTimerSystem()->Release();
	GetNetworkSystem()->Release();
	GetAudioSystem()->Release();
	GetSceneSystem()->end();
}
//-------------------------------------------------------------------------
// 执行循环
int FKFramework::Run()
{
	return CCApplication::sharedApplication()->run();
}
//-------------------------------------------------------------------------
// 进入应用程序
bool FKFramework::applicationDidFinishLaunching()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
		return false;
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	pDirector->setDisplayStats( true );
	pDirector->setAnimationInterval(1.0 / 60);

	// 自己的初始化
	if( ! InitSystem() )
		return false;

	return true;
}
//-------------------------------------------------------------------------
// 应用程序进入后台
void FKFramework::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->stopAnimation();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}
//-------------------------------------------------------------------------
// 应用程序呼出前台
void FKFramework::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->startAnimation();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
//-------------------------------------------------------------------------