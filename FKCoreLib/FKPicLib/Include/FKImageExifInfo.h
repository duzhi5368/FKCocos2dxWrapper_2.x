//*************************************************************************
//	创建日期:	2014-9-16   15:06
//	文件名称:	FKImageExifInfo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKImage.h"
#include "FKFile.h"
//-------------------------------------------------------------------------
#if USE_FK_EXIF

#define M_SOF0  0xC0            // Start Of Frame N
#define M_SOF1  0xC1            // N indicates which compression process
#define M_SOF2  0xC2            // Only SOF0-SOF2 are now in common use
#define M_SOF3  0xC3
#define M_SOF5  0xC5            // NB: codes C4 and CC are NOT SOF markers
#define M_SOF6  0xC6
#define M_SOF7  0xC7
#define M_SOF9  0xC9
#define M_SOF10 0xCA
#define M_SOF11 0xCB
#define M_SOF13 0xCD
#define M_SOF14 0xCE
#define M_SOF15 0xCF
#define M_SOI   0xD8            // Start Of Image (beginning of datastream)
#define M_EOI   0xD9            // End Of Image (end of datastream)
#define M_SOS   0xDA            // Start Of Scan (begins compressed data)
#define M_JFIF  0xE0            // Jfif marker
#define M_EXIF  0xE1            // Exif marker
#define M_COM   0xFE            // COMment 

#define PSEUDO_IMAGE_MARKER			0x123		 // Extra value.
#define EXIF_MAX_COMMENT			1000
#define EXIF_MAX_SECTIONS			20

#define NUM_FORMATS		12
static const int s_BytesPerFormat[] = {0,1,1,2,4,8,1,1,2,4,8,4,8};
#define FMT_BYTE		 1 
#define FMT_STRING		 2
#define FMT_USHORT		 3
#define FMT_ULONG		 4
#define FMT_URATIONAL	 5
#define FMT_SBYTE		 6
#define FMT_UNDEFINED	 7
#define FMT_SSHORT		 8
#define FMT_SLONG		 9
#define FMT_SRATIONAL	10
#define FMT_SINGLE		11
#define FMT_DOUBLE		12

#define TAG_EXIF_VERSION      0x9000
#define TAG_EXIF_OFFSET       0x8769
#define TAG_INTEROP_OFFSET    0xa005

#define TAG_MAKE              0x010F
#define TAG_MODEL             0x0110

#define TAG_ORIENTATION       0x0112
#define TAG_XRESOLUTION       0x011A
#define TAG_YRESOLUTION       0x011B
#define TAG_RESOLUTIONUNIT    0x0128

#define TAG_EXPOSURETIME      0x829A
#define TAG_FNUMBER           0x829D

#define TAG_SHUTTERSPEED      0x9201
#define TAG_APERTURE          0x9202
#define TAG_BRIGHTNESS        0x9203
#define TAG_MAXAPERTURE       0x9205
#define TAG_FOCALLENGTH       0x920A

#define TAG_DATETIME_ORIGINAL 0x9003
#define TAG_USERCOMMENT       0x9286

#define TAG_SUBJECT_DISTANCE  0x9206
#define TAG_FLASH             0x9209

#define TAG_FOCALPLANEXRES    0xa20E
#define TAG_FOCALPLANEYRES    0xa20F
#define TAG_FOCALPLANEUNITS   0xa210
#define TAG_EXIF_IMAGEWIDTH   0xA002
#define TAG_EXIF_IMAGELENGTH  0xA003

/* the following is added 05-jan-2001 vcs */
#define TAG_EXPOSURE_BIAS     0x9204
#define TAG_WHITEBALANCE      0x9208
#define TAG_METERING_MODE     0x9207
#define TAG_EXPOSURE_PROGRAM  0x8822
#define TAG_ISO_EQUIVALENT    0x8827
#define TAG_COMPRESSION_LEVEL 0x9102

#define TAG_THUMBNAIL_OFFSET  0x0201
#define TAG_THUMBNAIL_LENGTH  0x0202

enum ENUM_EXIF_READ_MODE
{
	eExifReadExif		= 0x01,
	eExifReadImage		= 0x02,
	eExifReadAll		= 0x03,
};

typedef struct SExifInfo {
	char  Version      [5];
	char  CameraMake   [32];
	char  CameraModel  [40];
	char  DateTime     [20];
	int   Height, Width;
	int   Orientation;
	int   IsColor;
	int   Process;
	int   FlashUsed;
	float FocalLength;
	float ExposureTime;
	float ApertureFNumber;
	float Distance;
	float CCDWidth;
	float ExposureBias;
	int   Whitebalance;
	int   MeteringMode;
	int   ExposureProgram;
	int   ISOequivalent;
	int   CompressionLevel;
	float FocalplaneXRes;
	float FocalplaneYRes;
	float FocalplaneUnits;
	float Xresolution;
	float Yresolution;
	float ResolutionUnit;
	float Brightness;
	char  Comments[EXIF_MAX_COMMENT];

	unsigned char * ThumbnailPointer;	/* Pointer at the thumbnail */
	unsigned ThumbnailSize;				/* Size of thumbnail. */

	bool  IsExif;
} EXIFINFO;

typedef struct SSection{
	BYTE*    Data;
	int      Type;
	unsigned Size;
} SECTION_T;

class CFKExifInfo
{
public:
	CFKExifInfo( SExifInfo* p_pInfo = NULL );
	~CFKExifInfo();
public:
	bool			DecodeExif( IFKFile* p_hFile, int p_nReadMode = eExifReadExif );
	bool			EncodeExif( IFKFile* p_hFile );
	void			DiscardAllButExif();
protected:
	bool			_ProcessExif( unsigned char* p_ucBuf, unsigned int p_unLen );
	void			_ProcessCom( const BYTE* p_byData, int p_nLen );
	void			_ProcessSOFn( const BYTE* p_byData, int p_nMarker );
	bool			_ProcessExifDir( unsigned char* p_szDirStart, unsigned char* p_szOffsetBase, unsigned int p_unExifLength,
		SExifInfo* const p_pInfo, unsigned char** const p_ppLastExifRef, int p_nNestingLevel = 0 );
	int				_Get16u( void* p_pShort );
	int				_Get16m( void* p_pShort );
	long			_Get32s( void* p_pLong );
	unsigned long	_Get32u( void* p_pLong );
	double			_ConvertAnyFormat( void* p_pValuePtr, int p_nFormat );
	void*			_FindSection( int p_nSectionType );
public:
	SExifInfo*		m_pExifInfo;
	char			m_szLastError[256];
	int				m_nExifImageWidth;
	int				m_nMotorolaOrder;
	SSection		m_vecSections[EXIF_MAX_SECTIONS];
	int				m_nSectionsRead;
	bool			m_bIsFreeInfo;
};

#endif