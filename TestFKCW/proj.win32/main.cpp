#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"
#include "resource1.h"
//--------------------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------------------
#pragma comment( lib, "libcocos2d.lib")
#pragma comment( lib, "libExtensions.lib")
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
//--------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Test_FKCW");
    eglView->setFrameSize(1280, 720);
    return CCApplication::sharedApplication()->run();
}
//--------------------------------------------------------------------