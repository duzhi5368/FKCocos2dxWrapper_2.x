//*************************************************************************
//	创建日期:	2015-1-14
//	文件名称:	main.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "main.h"
#include "Classes/TestApp.h"
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
#pragma comment( lib, "FKEngine.lib")
#pragma comment( lib, "pthreadVCE2.lib")

//#pragma comment( lib,"Shiny.lib")
//-------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	TestApp* app = GetApp();
	if( !app->Init( 720, 1280 ) )
	{
		return -1;
	}
	int nRet = app->Run();
	return nRet;
}
