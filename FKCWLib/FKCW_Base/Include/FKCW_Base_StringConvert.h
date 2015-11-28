//*************************************************************************
//	创建日期:	2014-11-26
//	文件名称:	FKCW_Base_StringConvert.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#pragma comment(lib,"libiconv.lib") 
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include "../../FKCW_Depend/libiconv/include/iconv.h"
#else
#include "iconv\iconv.h"
#endif



//--------------------------------------------------------------------
class FKCW_Base_StringConvert
{
public:
	static int __CodeConvert( const char* from_charset, const char* to_charset, 
		const char* inbuf, size_t inlen, char* outbuf, size_t outlen );
	/* UTF8 - GB2312 */
	static std::string u2a( const char* inbuf );
	/* GB2312 - UTF8 */
	static std::string a2u( const char* inbuf );
};