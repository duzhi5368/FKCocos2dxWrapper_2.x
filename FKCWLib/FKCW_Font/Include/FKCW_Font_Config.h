//*************************************************************************
//	创建日期:	2014-12-30
//	文件名称:	FKCW_Font_Config.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Font_Macros.h"
//-------------------------------------------------------------------------
#pragma comment( lib, "libfreetype2.lib" )
//-------------------------------------------------------------------------
extern bool			g_bIsFKCWFontDebug;
extern const char*	FKCW_font_default_fontpath;
extern int			FKCW_font_default_ppi;

#define FKCW_Font_Default_FontPath			FKCW_font_default_fontpath
#define FKCW_Font_Default_FontAlias			"default"
#define FKCW_Font_Default_FontPPI			FKCW_font_default_ppi

#define FKCW_Font_Bitmap_Padding			1
#define FKCW_Font_Texture_Size_Width		256
#define FKCW_Font_Texture_Size_Height		256
#define FKCW_Font_Max_Texture_Num_RefFont	8
//-------------------------------------------------------------------------