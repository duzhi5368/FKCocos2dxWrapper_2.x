//*************************************************************************
//	创建日期:	2014-11-13
//	文件名称:	FKCW_Base_SHA1.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
/*  测试参考
	SHA1("abc") =
		A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
	SHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") =
		84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
	SHA1(一百万个 "a") =
		34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#ifdef _MSC_VER
	#include <stdlib.h>
#endif
//-------------------------------------------------------------------------
// 检查定义大小尾
#if (BYTE_ORDER==LITTLE_ENDIAN)
	#define SHA1_LITTLE_ENDIAN
#endif
//-------------------------------------------------------------------------
// 定义 UINT_8 和 UINT_32
#ifndef UINT_32
	#ifdef _MSC_VER
		#define UINT_8  unsigned __int8
		#define UINT_32 unsigned __int32
	#else
		#define UINT_8 unsigned char

		#if (ULONG_MAX == 0xFFFFFFFF)
			#define UINT_32 unsigned long
		#else
			#define UINT_32 unsigned int
		#endif
	#endif
#endif
//-------------------------------------------------------------------------
typedef union
{
	UINT_8  c[64];
	UINT_32 l[16];
} USHA1WorkspaceBlock;
//-------------------------------------------------------------------------
// SHA1算法，常规数据建议使用HMAC_SHA1封装接口，特别需求可直接使用
class FKCW_Base_SHA1
{
public:
	// 枚举hash值的多种返回方式
	enum ENUM_ReportHashFormat
	{
		eReportHashFormat_Hex = 0,
		eReportHashFormat_Digit = 1
	};
public:
	FKCW_Base_SHA1();
	~FKCW_Base_SHA1();

	UINT_32 m_state[5];
	UINT_32 m_count[2];
	UINT_32 __reserved1[1];
	UINT_8  m_buffer[64];
	UINT_8  m_digest[20];
	UINT_32 __reserved2[3];

	void Reset();

	// Hash一段数据
	void SHA1Hash(UINT_8 *data, UINT_32 len);
	// Hash一个文件
	bool SHA1Hash(char *szFileName);

	// 最终的Hash并计算出Hash值
	void Final();

	// 返回Hash值数组
	void GetHashValue(char *szReport, unsigned char uReportType = eReportHashFormat_Hex);
	void GetHashValue(UINT_8 *puDest);

private:
	// 私有SHA-1转换函数
	void __Transform(UINT_32 *state, UINT_8 *buffer);

private:
	UINT_8					m_pWorkspace[64];
	USHA1WorkspaceBlock*	m_pBlock; // SHA1字节指针
};
//-------------------------------------------------------------------------
// 对FKCW_Base_SHA1的封装
class FKCW_Base_HMAC_SHA1 : public FKCW_Base_SHA1
{
private:
	unsigned char m_ipad[64];
	unsigned char m_opad[64];

	char * szReport ;
	char * SHA1_Key ;
	char * AppendBuf1 ;
	char * AppendBuf2 ;
public:

	enum {
		SHA1_DIGEST_LENGTH	= 20,
		SHA1_BLOCK_SIZE		= 64,
		HMAC_BUF_LEN		= 4096
	} ;

	FKCW_Base_HMAC_SHA1()
		:szReport(new char[HMAC_BUF_LEN]),
		SHA1_Key(new char[HMAC_BUF_LEN]),
		AppendBuf1(new char[HMAC_BUF_LEN]),
		AppendBuf2(new char[HMAC_BUF_LEN])
	{}

	~FKCW_Base_HMAC_SHA1()
	{
		delete[] szReport ;
		delete[] AppendBuf1 ;
		delete[] AppendBuf2 ;
		delete[] SHA1_Key ;
	}

	void HMAC_SHA1(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest);
};
//-------------------------------------------------------------------------