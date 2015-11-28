//*************************************************************************
//	创建日期:	2014-9-11   19:05
//	文件名称:	FKImageBmp.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKImage.h"
#include "FKFile.h"
//-------------------------------------------------------------------------
#define BFT_BITMAP 0x4d42		 /* 'BM' */

#ifndef WIDTHBYTES
#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */
#endif

#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0											\
								? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight)\
								: (lpbi)->biSizeImage)

#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8	\
								? (int)(1 << (int)(lpbi)->biBitCount)				\
								: (int)(lpbi)->biClrUsed)

#define FixBitmapInfo(lpbi)     if ((lpbi)->biSizeImage == 0)                 \
									(lpbi)->biSizeImage = DibSizeImage(lpbi); \
								if ((lpbi)->biClrUsed == 0)                   \
									(lpbi)->biClrUsed = DibNumColors(lpbi);

const int RLE_COMMAND     = 0;
const int RLE_ENDOFLINE   = 0;
const int RLE_ENDOFBITMAP = 1;
const int RLE_DELTA       = 2;
//-------------------------------------------------------------------------
class CFKImageBMP : public CFKImage
{
public:
	CFKImageBMP();
public:
	bool	Decode( IFKFile* p_hFile );
	bool	Decode( FILE* p_hFile );

	bool	Encode( IFKFile* p_hFile );
	bool	Encode( FILE* p_hFile );
protected:
	bool	_DibReadBitmapInfo( IFKFile* p_hFile, SBitmapInfoHeader* p_pDib );
};
//-------------------------------------------------------------------------