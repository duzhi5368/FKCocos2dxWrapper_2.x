//*************************************************************************
//	创建日期:	2014-10-30
//	文件名称:	FKCW_Json_Parser.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Json_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Json_Parser
{
public:
	// 解析一段内存中的json字符串
	static CCObject*		Load( const char* p_szJsonString, size_t p_unLength );
	// 解析一个json文件
	static CCObject*		Load( const string& p_szPath );
};