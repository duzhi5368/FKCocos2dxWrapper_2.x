//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_BaseStruct.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RichHtml_Macros.h"
//-------------------------------------------------------------------------
// RichHtml元素位置
struct FKCW_RichHtml_Pos
{
	short x;	// X偏移
	short y;	// Y偏移

	FKCW_RichHtml_Pos(): x(0), y(0) {}
	FKCW_RichHtml_Pos(short _x, short _y): x(_x), y(_y) {}
	inline FKCW_RichHtml_Pos add(const FKCW_RichHtml_Pos& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	inline FKCW_RichHtml_Pos sub(const FKCW_RichHtml_Pos& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};
//-------------------------------------------------------------------------
// RichHtml元素大小
struct FKCW_RichHtml_Size
{
	short w;	// 宽度
	short h;	// 高度

	FKCW_RichHtml_Size(): w(0), h(0) {}
	FKCW_RichHtml_Size(short _w, short _h): w(_w), h(_h) {}
};
//-------------------------------------------------------------------------
// RichHtml元素矩形
struct FKCW_RichHtml_Rect
{
	FKCW_RichHtml_Pos	pos; // 左上角坐标
	FKCW_RichHtml_Size	size;

	inline short min_x() const
	{
		return pos.x;
	}
	inline short max_x() const
	{
		return pos.x + size.w;
	}
	inline short min_y() const
	{
		return pos.y - size.h;
	}
	inline short max_y() const
	{
		return pos.y;
	}
	inline bool is_zero() const
	{
		return size.w == 0 && size.h == 0;
	}
	inline void extend(const FKCW_RichHtml_Rect& other)
	{
		short mix = MIN( min_x(), other.min_x() );
		short max = MAX( max_x(), other.max_x() );
		short miy = MIN( min_y(), other.min_y() );
		short may = MAX( max_y(), other.max_y() );

		pos.x = mix;
		pos.y = may;
		size.w = max - mix;
		size.h = may - miy;
	}
	inline void subtract(const FKCW_RichHtml_Rect& other)
	{
		pos.x = MAX(pos.x, other.pos.x);
		pos.y = MIN(pos.y, other.pos.y);

		short tmp_max_x = MAX(max_x(), other.max_x());
		short tmp_min_y = MIN(min_y(), other.min_y());

		size.w = tmp_max_x - pos.x;
		size.h = -(tmp_min_y - pos.y);

		size.w = MAX(size.w, 0);
		size.h = MAX(size.h, 0);
	}
};
//-------------------------------------------------------------------------
// 绝对大小和缩放比
struct FKCW_RichHtml_OptSize
{
	short absolute;
	float ratio; 

	FKCW_RichHtml_OptSize()
		: absolute(0), ratio(.0f)
	{
	}

	inline bool isZero() { return absolute == 0 && (ratio < 0.001f && ratio > -0.001f); }
	inline short getValueReal(short v) { return absolute > 0 ? absolute : (short)(v * ratio); }
};
//-------------------------------------------------------------------------
// 余量
struct FKCW_RichHtml_Margin
{
	short top;
	short right;
	short bottom;
	short left;

	FKCW_RichHtml_Margin()
		: top(0), right(0), bottom(0), left(0)
	{
	}
};
//-------------------------------------------------------------------------
// RichHtml元素矩阵
struct FKCW_RichHtml_Metrics
{
	FKCW_RichHtml_Rect rect;
	FKCW_RichHtml_Pos advance;
};
//-------------------------------------------------------------------------
// 纹理矩阵
struct FKCW_RichHtml_Texture
{
	FKCW_RichHtml_Texture() 
		:rect(), texture(NULL) 
	{
	}
	~FKCW_RichHtml_Texture() 
	{
		CC_SAFE_RELEASE(texture);
	}

	inline void setTexture(CCTexture2D* _texture)
	{
		texture = _texture;
		texture->retain();
	}

	inline CCTexture2D* getTexture()
	{
		return texture;
	}

public:
	FKCW_RichHtml_Rect rect;
private:
	CCTexture2D* texture;
};
//-------------------------------------------------------------------------
// 画布
struct FKCW_RichHtml_Canvas
{
	class FKCW_RichHtml_IRichNode* root;
	FKCW_RichHtml_Rect rect;
};
//-------------------------------------------------------------------------
// 排序朝向
enum ENUM_Alignment
{
	// 横向的
	e_align_left = 0,
	e_align_right = 1,
	e_align_center = 2,
	e_align_justify,		// 未支持
	e_align_char,			// 未支持
	// 纵向的
	e_align_bottom = 0,
	e_align_top = 1,
	e_align_middle = 2,
	e_align_baseline		// 未支持
};
//-------------------------------------------------------------------------
class FKCW_RichHtml_ICompositCache;
// 组合器状态
struct FKCW_RichHtml_MetricsState
{
	FKCW_RichHtml_Rect zone;

	short pen_x;
	short pen_y;

	FKCW_RichHtml_ICompositCache* elements_cache;

	FKCW_RichHtml_MetricsState()
		: pen_x(0), pen_y(0), elements_cache(NULL)
	{
	}
};
//-------------------------------------------------------------------------
// 渲染状态
struct FKCW_RichHtml_RenderState
{
	unsigned int color;
	const char* font_alias;

	FKCW_RichHtml_RenderState()
		: color(0xffffffff), font_alias(FKCW_Font_Default_FontAlias)
	{

	}
};
//-------------------------------------------------------------------------