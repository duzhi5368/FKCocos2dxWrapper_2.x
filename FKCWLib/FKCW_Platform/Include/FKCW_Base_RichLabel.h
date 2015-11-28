//*************************************************************************
//	创建日期:	2014-10-27
//	文件名称:	FKCW_Base_RichLabel.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include <vector>
#include "FKCW_Base_Types.h"
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
USING_NS_CC;
using std::vector;
//-------------------------------------------------------------------------
// 超链接区域的元信息
struct SLinkMeta
{
	int			m_nTag;					// 超链接标签（允许多链接同一标签）
	int			m_nNormalBGColor;
	int			m_nSelectedBGColor;
	// 链接区
	float		m_fX;
	float		m_fY;
	float		m_fWidth;
	float		m_fHeight;
};
typedef vector<SLinkMeta>		LinkMetaList;
//-------------------------------------------------------------------------
class FKCW_Base_RichLabel : public CCImage
{
public:
	FKCW_Base_RichLabel();
	virtual ~FKCW_Base_RichLabel();
public:
	// 计算一个富文本字符串所占显示大小（不需要使用openGL纹理计算）
	/* 参数：
	* p_szText 富文本字符串
	* p_szFontName 字体名称
	* p_nSize 字体大小
	* p_nMaxWidth 最大显示宽度。0表示单行文本
	* p_fShadowOffsetX 阴影X偏移
	* p_fShadowOffsetY 阴影Y偏移
	* p_fStrokeSize 边框线宽度
	* p_fLineSpacing 行间距
	* p_fGlobalImageScaleFactor富文本中的图片缩放比
	* p_pDecryptFunc 如果富文本中有图片加密，则需要提供解密函数
	*/
	static CCSize MeasureRichString( const char* p_szText, const char* p_szFontName = NULL,
		int p_nSize = 0, int p_nMaxWidth = 0, float p_fShadowOffsetX = 0.0f, float p_fShadowOffsetY = 0.0f,
		float p_fStrokeSize = 0.0f, float p_fLineSpacing = 0.0f, 
		float p_fGlobalImageScaleFactor = 1.0f, DECRYPT_FUNC p_pDecryptFunc = NULL );
public:
	bool InitWithRichStringShadowStroke( const char* p_szText,
		int			p_nWidth			= 0,
		int			p_nHeight			= 0,
		ETextAlign	p_eAlignMask		= kAlignCenter,
		const char* p_szFontName		= NULL,
		int			p_nSize				= 0,
		float		p_fTextTintR		= 0.0f,
		float		p_fTextTintG		= 0.0f,
		float		p_fTextTintB		= 0.0f,
		bool		p_nShadow			= false,
		float		p_fShadowOffsetX	= 0.0f,
		float		p_fShadowOffsetY	= 0.0f,
		int			p_nShadowColor		= 0,
		float		p_nShadowBlur		= 0.0f,
		bool		p_nStroke			= false,
		float		p_fStrokeR			= 1.0f,
		float		p_fStrokeG			= 1.0f,
		float		p_fStrokeB			= 1.0f,
		float		p_fStrokeSize		= 1.0f,
		float		p_fLineSpacing		= 0.0f,
		float		p_fGlobalImageScale	= 1.0f,
		int			p_nToCharIndex		= -1,
		float		p_fElapsedTime		= 0.0f,
		DECRYPT_FUNC p_pDecryptFunc		= NULL
		);

public:
	// 阴影和边框线值
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint, m_tagShadowStrokePadding, ShadowStrokePadding);
	// 链接元表
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(LinkMetaList, m_vecLinkMetas, LinkMetas);
	// 图片元表
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(vector<CCRect>, m_vecImageRects, ImageRects);
	// 无特殊样式的字符串长度
	CC_SYNTHESIZE(int, m_nRealLength, RealLength);
	// 是否需要每帧更新( 若有特效时，该值为true，则进行帧更新)
	CC_SYNTHESIZE_BOOL(m_bIsNeedUpdate, NeedUpdate);
};