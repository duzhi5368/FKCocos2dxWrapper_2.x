//*************************************************************************
//	创建日期:	2014-9-11   16:57
//	文件名称:	FKImage.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKPicCommonDef.h"
//-------------------------------------------------------------------------
class IFKFile;
class CFKImage
{
	typedef struct SFKImageInfo
	{
		DWORD			m_dwEffWidth;				// 扫描线宽度
		BYTE*			m_byImage;					// 图像实际数据
		CFKImage*		m_pGhost;					// 如果这个图是一个备份，则该值指向其原始对象
		CFKImage*		m_pParent;					// 如果这个图是一个层，则该值指向其主体
		DWORD			m_dwType;					// 原始图形格式
		char			m_szLastError[256];			// 调试信息，最后的错误信息
		long			m_lProgress;				// 监视循环的次数
		long			m_lEscape;					// 跳出标志
		long			m_lBkgndIndex;				// 背景色索引（GIF,PNG,MNG使用）
		SRGBQuad		m_tagBkgndColor;			// 背景色（RGB三原色）
		float			m_fQuality;					// JPEG品质（0.0f - 100.0f, JEPG使用 ）
		BYTE			m_byJpegScale;				// JPEG缩放比（JPEG使用）
		long			m_lFrame;					// 当前帧数（TIF，GIF，MNG使用）
		long			m_lNumFrames;				// 总帧数（TIF，GIF，MNG使用）
		DWORD			m_dwFrameDelay;				// 帧间隔（GIF，MNG使用）
		long			m_lXDpi;					// 水平分辨率
		long			m_lYDpi;					// 垂直分辨率
		SRect			m_tagSelectionBox;			// 包围盒
		BYTE			m_byAlphaMax;				// 最大不透明度
		bool			m_bAlphaPaletteEnable;		// 如果调色板中有Alpha通道，则该值为true
		bool			m_bEnabled;					// 是否开启绘图函数
		long			m_lXOffset;					// X偏移量
		long			m_lYOffset;					// Y偏移量
		DWORD			m_dwCodeOpt[eFormats_Max];	// 一些编码选项（GIF,TIF使用）
		SRGBQuad		m_tagLastC;					// 一些优化选项
		BYTE			m_byLastCIndex;
		bool			m_bLastCIsValid;
		long			m_lNumLayers;				// 总层数
		DWORD			m_dwFlag;					// 标示位【0x??000000 保留位, 0x00??0000 混合模式, 0x0000???? 层ID】
		BYTE			m_byDispmeth;
		bool			m_bGetAllFrames;
		bool			m_bLittleEndianHost;		// 是否小尾数据
		BYTE			m_byZipScale;				// Zip压缩比率(FKP使用)
	}FKIMAGEINFO;

public:
	CFKImage( DWORD p_dwImageType = 0 );
	virtual ~CFKImage();
	CFKImage& operator = ( const CFKImage& p_tagOther );
public:
	void*				Create( DWORD p_dwWidth, DWORD p_dwHeight, DWORD p_dwBpp, DWORD p_dwImageType = 0 );
	bool				Destroy();
	bool				DestroyFrames();
	void				Copy( const CFKImage& p_tagSrc, bool p_bCopyPixels = true, 
		bool p_bCopySelection = true, bool p_bCopyAlpha = true );
	bool				Load( const TCHAR* p_szFileName, DWORD p_dwImageType = 0 );
	bool				Decode( FILE* p_pFile, DWORD p_dwImageType );
	bool				Decode( IFKFile* p_pFile, DWORD p_dwImageType );
	bool				Decode( BYTE* p_szBuffer, DWORD p_dwSize, DWORD p_dwImageType );
	bool				Transfer( CFKImage &p_From, bool p_bTransferFrames = true );
	bool				Save( const TCHAR* p_szFileName, DWORD p_dwImageType = 0 );
	bool				Encode( FILE* p_pFile, DWORD p_dwImageType );
	bool				Encode( IFKFile* p_pFile, DWORD p_dwImageType );
	bool				Encode( BYTE*& p_Buffer, long& p_dwSize, DWORD p_dwImageType );
public:
	long				GetXDpi() const;
	long				GetYDpi() const;
	void				SetXDpi( long p_lDpi );
	void				SetYDpi( long p_lDpi );
	DWORD				GetClrImportant() const;
	void				SetClrImportant( DWORD p_dwColors = 0 );
	SRGBQuad			GetTransColor();
	long				GetSize();
	DWORD				GetPaletteSize();
	SRGBQuad*			GetPalette() const;
	void				SetGrayPalette();
	void				SetPalette( DWORD p_dwN, BYTE* p_byRed, BYTE* p_byGreen, BYTE* p_byBlue );
	void				SetPalette( SRGBQuad* p_pPalette,DWORD p_dwColors = 256 );
	void				SetPalette( SRGBColor* p_pRgb,DWORD p_dwColors = 256 );
	SRGBQuad			GetPaletteColor( BYTE p_byIndex );
	bool				GetPaletteColor( BYTE p_byIndex, BYTE* p_pRed, BYTE* p_pGreen, BYTE* p_pBlue );
	void				SetPaletteColor( BYTE p_byIndx, SRGBQuad p_tagRGB );
	void				SetPaletteColor( BYTE p_byIndx, BYTE p_byRed, BYTE p_byGreen, BYTE p_byBlue, BYTE p_byAlpha = 0 );
	void				SwapIndex( BYTE p_byIndex1, BYTE p_byIndex2 );
	BYTE*				GetBits( DWORD p_dwRow = 0 );
	DWORD				GetHeight() const;
	DWORD				GetWidth() const;
	DWORD				GetNumColors() const;
	WORD				GetBpp() const;
	DWORD				GetType() const;
	DWORD				GetEffWidth() const;
	BYTE				GetJpegQuality() const;
	void				SetJpegQuality( BYTE p_byQuality );
	BYTE				GetJpegScale() const;
	void				SetJpegScale( BYTE p_byScale );
	void				SetZipQuality( BYTE p_byQuality );
	BYTE				GetZipQuality() const;
	bool				IsGrayScale();
	DWORD				GetCodecOption( DWORD p_dwImageType = 0 );
	bool				SetCodecOption( DWORD p_dwOpt, DWORD p_dwImageType = 0 );
	const char*			GetLastError();
	bool				IsInside( long p_lX, long p_lY );
	bool				Flip( bool p_bFlipSelection = false, bool p_bFlipAlpha = true );
	SRGBQuad			GetPixelColor( long p_lX, long p_lY, bool p_bGetAlpha = true );


#if USE_FK_SELECTION
	bool				DeleteSelection();
	bool				FlipSelection();
#endif

#if USE_FK_ALPHA
	void				DeleteAlpha();
	bool				CreateAlpha();
	bool				IsAlphaValid();
	void				SetAlpha( const long p_lX, const long p_lY, const BYTE p_byLevel );
	BYTE				GetAlpha( const long p_lX, const long p_lY );
	void				InvertAlpha();
	bool				FilpAlpha();
	BYTE*				GetAlphaPointer( const long p_lX = 0, const long p_lY = 0 );
protected:
	BYTE				_BlindGetAlpha( const long p_lX, const long p_lY );
#endif

protected:
	void				_StartUp( DWORD p_dwImageType );
	bool				_IsLittleEndian();
	void				_CopyInfo( const CFKImage& p_Src );
	void				_Ghost( const CFKImage* p_pSrc );
	long				_Ntohl( const long p_lDword );
	short				_Ntohs( const short p_sWord );
	void				_RGBtoBGR( BYTE* p_pBuffer, int p_nLength );
	void				_Bitfield2RGB( BYTE* p_pSrc, DWORD p_dwRedMask, 
		DWORD p_dwGreenMask, DWORD p_dwBlueMask, BYTE p_byBpp);
	void				_Bihtoh( SBitmapInfoHeader* p_pBih );
	bool				_EncodeSafeCheck( IFKFile* p_hFile );
	BYTE				_BlindGetPixelIndex( const long x, const long y );
	void				_BlindSetPixelIndex( long x, long y, BYTE p_byByte );
	SRGBQuad			_BlindGetPixelColor( long p_lX, long p_lY, bool p_bGetAlpha = true );
	bool				_IsHadFreeKnightSign( IFKFile* p_hFile );
protected:
	void*				m_pDib;						// 包含文件头，调色板，像素数据
	SBitmapInfoHeader	m_tagHead;					// 标准文件头
	SFKImageInfo		m_tagInfo;					// 额外信息
	BYTE*				m_pSelection;				// 选择区域
	BYTE*				m_pAlpha;					// Alpha通道
	CFKImage**			m_ppLayers;					// 子层级
	CFKImage**			m_ppFrames;					// 子帧
};
//-------------------------------------------------------------------------