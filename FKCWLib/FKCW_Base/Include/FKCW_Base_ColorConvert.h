/********************************************************************
	created:	2014/10/24
	file base:	FKCW_Base_ColorConvert
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------
typedef struct ccColorHSV { float h, s, v; } SColorHSV;
static inline ccColorHSV CreateHSV(const float h, const float s, const float v) 
{
	ccColorHSV c = { h, s, v };
	return c;
}
//--------------------------------------------------------------------

// 颜色定义
static const ccColor4B cc4RED = { 255, 0, 0, 255 };
static const ccColor4B cc4GREEN = { 0, 255, 0, 255 };
static const ccColor4B cc4BLUE = { 0, 0, 255, 255 };
static const ccColor4B cc4BLACK = { 0, 0, 0, 255 };
static const ccColor4B cc4WHITE = { 255, 255, 255, 255 };
static const ccColor4B cc4TRANSPARENT = { 0, 0, 0, 0 };
static const ccColor4B cc4DIM = { 0, 0, 0, 127 };
static const ccColor4B cc4YELLOW = { 255, 255, 0, 255 };
static const ccColor4F cc4fRED = { 1, 0, 0, 1 };
static const ccColor4F cc4fGREEN = { 0, 1, 0, 1 };
static const ccColor4F cc4fBLUE = { 0, 0, 1, 1 };
static const ccColor4F cc4fBLACK = { 0, 0, 0, 1 };
static const ccColor4F cc4fWHITE = { 1, 1, 1, 1 };
static const ccColor4F cc4fTRANSPARENT = { 0, 0, 0, 0 };
static const ccColor4F cc4fDIM = { 0, 0, 0, 0.5f };
static const ccColor4F cc4fYELLOW = { 1, 1, 0, 1 };

//  0xaarrggbb -> ccColor4B
static inline ccColor4B ccc4FromInt(int c) 
{
	return ccc4((c >> 16) & 0xff,
		(c >> 8) & 0xff,
		c & 0xff,
		(c >> 24) & 0xff);
}

// 0xaarrggbb -> ccColor3B
static inline ccColor3B ccc3FromInt(int c) 
{
	return ccc3((c >> 16) & 0xff,
		(c >> 8) & 0xff,
		c & 0xff);
}

// ccColor4B -> ccColor3B
static inline ccColor3B ccc3FromCCC4(const ccColor4B& c4) 
{
	return ccc3(c4.r, c4.g, c4.b);
}

// 0xaarrggbb -> ccColor4F
static inline ccColor4F ccc4fFromInt(int c) 
{
	return ccc4f(((c >> 16) & 0xff) / 255.0f,
		((c >> 8) & 0xff) / 255.0f,
		(c & 0xff) / 255.0f,
		((c >> 24) & 0xff) / 255.0f);
}

// ccColor4B -> 0xaarrggbb
static inline int ccIntFromC4(const ccColor4B& c) 
{
	return ((c.a & 0xff) << 24) |
		((c.r & 0xff) << 16) |
		((c.g & 0xff) << 8) |
		(c.b & 0xff);
}

// ccColor3B -> 0xaarrggbb
static inline int ccIntFromC3(const ccColor3B& c) 
{
	return 0xff000000 |
		((c.r & 0xff) << 16) |
		((c.g & 0xff) << 8) |
		(c.b & 0xff);
}

// ccColor4F -> 0xaarrggbb
static inline int ccIntFromC4f(const ccColor4F& c) 
{
	return (((int)(c.a * 255) & 0xff) << 24) |
		(((int)(c.r * 255) & 0xff) << 16) |
		(((int)(c.g * 255) & 0xff) << 8) |
		((int)(c.b * 255) & 0xff);
}

// rgb -> hsv
static inline ccColorHSV C3B2HSV( ccColor3B c )
{
	unsigned char min = MIN(c.r, MIN(c.g, c.b));
	unsigned char max = MAX(c.r, MAX(c.g, c.b));
	unsigned char delta = max - min;

	// 获取 v
	float v = max / 255.f;

	// 如果输入的原本就是灰度图
	if(delta == 0) {
		return CreateHSV(0, 0, v);
	}

	// 获取 s
	float s = (float)delta / max;

	// 获取 h
	float h;
	if(c.r == max) {
		h = (float)(c.g - c.b) / delta;
	} else if (c.g == max) {
		h = 2 + (float)(c.b - c.r) / delta;
	} else { // b == max
		h = 4 + (float)(c.r - c.g) / delta;
	}

	// 调整 h
	h *= 60;
	if (h < 0) {
		h += 360;
	}

	return CreateHSV(h, s, v);
}

static inline unsigned char	__UnitScalarToByte( float x )
{
	if (x < 0) {
		return 0;
	}
	if (x >= 1) {
		return 255;
	}
	return (int)(x * (1 << 16)) >> 8;
}

// hsv -> rgb
static inline ccColor3B HSV2C3B( ccColorHSV c )
{
	unsigned char s = __UnitScalarToByte(c.s);
	unsigned char v = __UnitScalarToByte(c.v);

	// 如果 s 为0，则rgb颜色是灰色图
	if (0 == s) {
		return ccc3(v, v, v);
	}

	int hx = (c.h < 0 || c.h >= 360.f) ? 0 : (int)((c.h / 60) * (1 << 16));
	int f = hx & 0xFFFF;

	unsigned char v_scale = v + 1;
	unsigned char p = ((255 - s) * v_scale) >> 8;
	unsigned char q = ((255 - (s * f >> 16)) * v_scale) >> 8;
	unsigned char t = ((255 - (s * ((1 << 16) - f) >> 16)) * v_scale) >> 8;

	unsigned char r, g, b;
	switch (hx >> 16) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:
		r = v;
		g = p;
		b = q;
		break;
	}

	return ccc3(r, g, b);
}
//--------------------------------------------------------------------