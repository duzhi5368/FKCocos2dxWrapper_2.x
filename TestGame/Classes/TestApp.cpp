//-------------------------------------------------------------------------
#include "TestApp.h"
#include "LoginScene.h"
#include "PlayTestScene.h"
#include "AchieTestScene.h"
#include "ShopTestScene.h"
#include "GameCommon.h"
//-------------------------------------------------------------------------
TestApp gs_App;
TestApp* GetApp()
{
	return &gs_App;
}
//-------------------------------------------------------------------------
bool TestApp::SubInit()
{
	// 自动屏幕适配
	__AutomaticScreenAdaptation();
	// 加载全局公用资源
	__LoadGlobleResources();
	// 注册事件
	m_EventBridge.RegisteFKEvent();
	// 注册场景
	REGISTER_SCENE_FUNC(CLoginScene);
	REGISTER_SCENE_FUNC(CPlayTestScene);
	REGISTER_SCENE_FUNC(CShopTestScene);
	REGISTER_SCENE_FUNC(CAchieTestScene);

	IEventSystem* pEvent = GetEventSystem();
	if (pEvent == NULL)
	{
		return false;
	}
	pEvent->FireExec(eEvent_UIEvent,eEvent_Scene_Switch,eSubSceneType_Login,NULL,0);

	return true;
}
//-------------------------------------------------------------------------
void TestApp::SubUpdate( float dt )
{

}
//-------------------------------------------------------------------------
void TestApp::SubRelease()
{
	// 移除全局事件
	m_EventBridge.UnRegisteFKEvent();

	// 释放全局公用资源
	__RelesaeGlobleResources();
}
//-------------------------------------------------------------------------
// 收到输入消息的回调处理函数
// 参数：unMsgType 定义的消息类型【参见ENUM_InputMsgType】
// 参数：nTouchID 触摸消息的ID，若为key消息，则该值无效为0
// 参数：fPosX x位置
// 参数：fPosY y消息
// 参数：详细数据指针，例如捏合消息有额外的数据（放大还是缩小）滑动的额外数据（滑动方向）等，均记录在此。
// 返回值：若返回true表示忽略后面的调用，消息不再向后传递；若返回false则表示消息将继续向下传递
bool TestApp::OnInputMessage( unsigned int unMsgType, int nTouchID, 
							   float fPosX, float fPosY, void* pParam )
{
	return true;
}
//-------------------------------------------------------------------------
// 自动屏幕适配
void TestApp::__AutomaticScreenAdaptation()
{
	vector<string> vecResourcePaths;
	vecResourcePaths.push_back( "Resources" );
	vecResourcePaths.push_back( "Resources/Game/" );
	CCSize tagScreenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize tagDesignSize = CCSizeMake(720, 1280);
	// todo:屏幕适配
	if( tagScreenSize.width / tagScreenSize.height < 1.4f )
	{
		vecResourcePaths.push_back( "Resources/Game/1280x720/");
	}
	else
	{
		vecResourcePaths.push_back( "Resources/Game/1280x720/");
	}
	CCFileUtils::sharedFileUtils()->setSearchPaths(vecResourcePaths);
}
//-------------------------------------------------------------------------
// 加载全局资源
void TestApp::__LoadGlobleResources()
{
	CCSpriteFrameCache *pCache=CCSpriteFrameCache::sharedSpriteFrameCache();
	if (pCache == NULL)
	{
		return;
	}
	pCache->addSpriteFramesWithFile(s_szUIGPListPath,s_szUIGPngPath);
	pCache->addSpriteFramesWithFile(s_szUIResPlistPath,s_szUIResPngPath);
}
//-------------------------------------------------------------------------
// 释放全局资源
void TestApp::__RelesaeGlobleResources()
{
	// 删除纹理集
	CCSpriteFrameCache *pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if (pCache == NULL)
	{
		return;
	}
	pCache->removeSpriteFramesFromFile(s_szUIGPListPath);
	pCache->removeSpriteFramesFromFile(s_szUIResPlistPath);
}
//-------------------------------------------------------------------------