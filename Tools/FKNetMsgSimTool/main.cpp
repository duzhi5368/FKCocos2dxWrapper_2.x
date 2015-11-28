//*************************************************************************
//	创建日期:	2015-1-6
//	文件名称:	main.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "resource2.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
#pragma comment( lib, "libcocos2d.lib")
#pragma comment( lib, "libExtensions.lib")
#pragma comment( lib, "lua51.lib")
#pragma comment( lib, "liblua.lib")
#pragma comment( lib, "libzlib.lib")
#pragma comment( lib, "libCocosDenshion.lib")

#pragma comment( lib, "FKCW_Base.lib")
#pragma comment( lib, "FKCW_IO.lib")
#pragma comment( lib, "FKCW_Shader.lib")
#pragma comment( lib, "FKCW_Database.lib")
#pragma comment( lib, "FKCW_Json.lib")
#pragma comment( lib, "FKCW_Network.lib")
#pragma comment( lib, "FKCW_SimpleNet.lib")
#pragma comment( lib, "FKCW_Action.lib")
#pragma comment( lib, "FKCW_RenderEx.lib")
#pragma comment( lib, "FKCW_UI.lib")
#pragma comment( lib, "FKCW_GR.lib")
#pragma comment( lib, "FKCW_SceneExMgr.lib")
#pragma comment( lib, "FKCW_EffectNode.lib")
#pragma comment( lib, "FKCW_UIWidget.lib")
#pragma comment( lib, "FKCW_3D.lib")
#pragma comment( lib, "FKCW_TMX.lib")
#pragma comment( lib, "FKCW_Font.lib")
#pragma comment( lib, "FKCW_RichHtml.lib")
#pragma comment( lib, "FKCW_Story.lib")
#pragma comment( lib, "FKCW_Pixel.lib")

#pragma comment( lib, "FKPicLib.lib")
#pragma comment( lib, "libjpeg.lib")
#pragma comment( lib, "FKDBLib.lib")
#pragma comment( lib, "FKDBCocos2dxLib.lib")
//-------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName(A2U("网络消息模拟器").c_str());
    eglView->setFrameSize(1165, 900);
    return CCApplication::sharedApplication()->run();
}
