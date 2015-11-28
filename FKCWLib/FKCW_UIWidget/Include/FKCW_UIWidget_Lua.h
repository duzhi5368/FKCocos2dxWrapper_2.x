//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_Lua.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UIWidget_Macros.h"
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
#ifdef __cplusplus
	extern "C" {
#endif
	#include "tolua++.h"
#ifdef __cplusplus
	}
#endif

TOLUA_API int FKCW_UIWidget_ToLua_Open(lua_State* tolua_S);
#endif
