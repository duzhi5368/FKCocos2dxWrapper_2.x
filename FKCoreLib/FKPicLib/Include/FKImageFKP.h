//*************************************************************************
//	创建日期:	2014-9-19   14:00
//	文件名称:	FKImageFKP.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKImage.h"
#include "FKImageExifInfo.h"
extern "C"
{
#include "jpeglib.h"
#include "jerror.h"
#include "zlib.h"
};
//-------------------------------------------------------------------------
class CFKImageFKP : public CFKImage
{
public:
	CFKImageFKP();
	~CFKImageFKP();
public:
	bool	Decode( IFKFile* p_hFile );
	bool	Decode( FILE* p_hFile );

	bool	Encode( IFKFile* p_hFile );
	bool	Encode( FILE* p_hFile );
public:
	static void JpegErrorExit( j_common_ptr p_tagInfo );
#if USE_FK_EXIF
	bool	DecodeExif( IFKFile* p_hFile );
	bool	DecodeExif( FILE* p_hFile );
public:
	CFKExifInfo*		m_pExif;
	SExifInfo			m_tagExifInfo;
#endif
	J_DITHER_MODE		m_nDither;
};
//-------------------------------------------------------------------------