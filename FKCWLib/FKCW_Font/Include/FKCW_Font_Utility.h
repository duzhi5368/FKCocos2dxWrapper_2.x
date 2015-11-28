//*************************************************************************
//	创建日期:	2014-12-30
//	文件名称:	FKCW_Font_Utility.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Font_Config.h"
//-------------------------------------------------------------------------
// 获取系统字体目录
extern const char* get_systemfont_path();
// 获取系统默认PPI
extern int get_system_default_ppi();
// 获取系统默认字体大小
extern int get_prefered_default_fontsize();
// 获取系统默认字体
extern const char* get_system_default_fontfile();
// 获取系统默认拉丁字体
extern const char* get_system_default_hacklatin_fontfile();
// 初始化
extern void FKCW_font_default_initialize();
// 拉汀字符集
extern std::set<unsigned long>* latin_charset();

// dump到tga文件中
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

bool dump2tga(const std::string &filename, const unsigned int *pxl, uint16 width, uint16 height);