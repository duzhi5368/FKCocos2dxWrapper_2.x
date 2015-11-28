//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_Macros.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCW_Base/FKCW_Base.h"
#include "../../FKCW_Font/FKCW_Font.h"
#include <list>
using std::list;
//-------------------------------------------------------------------------
static const int ZORDER_CONTEXT		= 100;
static const int ZORDER_OVERLAY		= 0;
static const int ZORDER_BACKGROUND	= -100;
//-------------------------------------------------------------------------
// 是否开启RichHtml的Debug模式
extern bool g_bIsFKCWRichHtmlDebug;
// 是否使用Lua支持
#define FKCW_RICHHTML_USING_LUA 0
//-------------------------------------------------------------------------