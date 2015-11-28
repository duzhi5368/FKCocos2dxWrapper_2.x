/********************************************************************
	created:	2014/10/27
	file base:	FKCW_Base_Base64
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------
class FKCW_Base_Base64
{
public:
	// 加密一段数据
	static string Encode( const void* p_pData, int p_nLen );
	// 解密一段数据
	static const char* Decode( const string& p_strData, int* p_pOutLen );
	// 解密一段数据（后端会补'/0'结束符）
	static const char* DecodeAsCString( const string& p_strData, int* p_pOutLen );
};