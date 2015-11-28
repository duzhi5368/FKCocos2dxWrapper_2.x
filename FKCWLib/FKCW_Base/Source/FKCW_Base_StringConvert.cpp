//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_StringConvert.h"
//-------------------------------------------------------------------------
int FKCW_Base_StringConvert::__CodeConvert( const char* from_charset, const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t outlen )
{
	iconv_t cd;
	

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	const char* temp = inbuf;
	const char** pin = &temp;
#else
	char* temp = const_cast<char *>(inbuf);
	char** pin = &temp;
#endif

	char** pout = &outbuf;
	memset( outbuf, 0, outlen );
	cd = iconv_open( to_charset, from_charset );
	if( cd == 0 ) return -1;
	if( iconv( cd, pin, &inlen, pout, &outlen ) == -1 ) 
	{
		return -1;
	}
	iconv_close( cd );
	return 0;
}
//-------------------------------------------------------------------------
/* UTF8 -> GB2312 */
std::string FKCW_Base_StringConvert::u2a( const char* inbuf )
{
	size_t inlen = strlen( inbuf );
	char* outbuf = new char[inlen * 2 + 2 ];
	std::string strRet;
	if( __CodeConvert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2 ) == 0 )
	{
		strRet = outbuf;
	}
	delete [] outbuf;
	return strRet;
}
//-------------------------------------------------------------------------
/* GB2312 -> UTF8 */
std::string FKCW_Base_StringConvert::a2u( const char* inbuf )
{
	size_t inlen = strlen( inbuf );
	char* outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if( __CodeConvert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2 ) == 0 )
	{
		strRet = outbuf;
	}
	delete [] outbuf;
	return strRet;
}
//-------------------------------------------------------------------------