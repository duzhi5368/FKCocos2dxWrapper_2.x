//*************************************************************************
//	创建日期:	2014-10-27
//	文件名称:	FKCW_Base_DecodeHtmlEntitiesUTF8.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// 将 src 解码到 dest 中。
// 注：dest缓冲长度应当大于等于 strlen(src) + 1
// 返回值：解码后的字符串长度
size_t decode_html_entities_utf8(char *dest, const char *src);
	
#ifdef __cplusplus
}
#endif