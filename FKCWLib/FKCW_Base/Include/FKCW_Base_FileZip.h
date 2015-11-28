//*************************************************************************
//	创建日期:	2014-12-31
//	文件名称:	FKCW_Base_FileZip.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif
//-------------------------------------------------------------------------
namespace FKCW_Base_FileZip
{
	// 压缩源文件到目标文件
	extern "C" int	Compress(FILE *source, FILE *dest, int level);
	// 解压缩文件
	extern "C" int	Decompress(FILE *source, FILE *dest, double* progress = NULL);
	// 将压缩和解压缩的结果翻译为错误文字信息
	extern "C" void OutputError(int ret);
}