//*************************************************************************
//	创建日期:	2014-10-30
//	文件名称:	FKCW_Json_Value.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Json_Array.h"
#include "FKCW_Json_Object.h"
#include "FKCW_Json_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Json_Value
{
public:
	// 转换为 bool
	static bool			CastToBool( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 int
	static int			CastToInt( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 long
	static long			CastToLong( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 float
	static float		CastToFloat( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 double
	static double		CastToDouble( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 json object
	static FKCW_Json_Object*	CastToObject( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 json array
	static FKCW_Json_Array*		CastToArray( FKCW_Json_Object::SKeyValue& p_KV );
	// 转换为 string
	static const char*			CastToString( FKCW_Json_Object::SKeyValue& p_KV );
};