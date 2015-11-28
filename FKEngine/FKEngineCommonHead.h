//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	FKEngineCommonHead.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
#include "Interface/IAudioSystem.h"				// 音乐音效处理系统
#include "Interface/INetworkSystem.h"			// 网络连接处理系统
#include "Interface/ITimerSystem.h"				// 定时器管理系统
#include "Interface/IMsgDispatchSystem.h"		// 消息分发系统
#include "Interface/IEventSystem.h"				// 事件管理系统
#include "Interface/ISceneSystem.h"				// 场景管理系统
#include "Interface/IUISystem.h"				// UI管理系统 - 依赖场景管理系统
#include "Interface/IInputSystem.h"				// 输入管理系统 - 依赖UISystem
#include "Interface/IConfigFileSystem.h"		// 配置文件管理系统
#include "Interface/IDebugSystem.h"				// Debug调试系统
#include "Interface/IRenderSystem.h"			// 渲染系统
#include "Interface/ICameraSystem.h"			// 摄像机控制系统系统

#include "Interface/IRenderable.h"
#include "Interface/IRenderObj.h"
#include "Interface/IRenderText.h"
#include "Interface/IRenderObjModel.h"
#include "Interface/IMapObj.h"

#include "Render/FKMapSystemMacros.h"
#include "Input/FKInputSystemMacros.h"

#include "Framework/FKFramework.h"

