//*************************************************************************
//	创建日期:	2014-11-21
//	文件名称:	EdgeDetection_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_EdgeDetectionSize "CC_EdgeDetectionSize"

static GLint sUniform_pos_CC_edgeDetectionSize = -1;

const char* ShaderEdgeDetectionFrag = "							\n\
#ifdef GL_ES													\n\
	precision mediump float;									\n\
#endif															\n\
																\n\
varying vec4 v_fragmentColor;									\n\
varying vec2 v_texCoord;										\n\
																\n\
uniform vec2 CC_EdgeDetectionSize;								\n\
																\n\
float lookup(vec2 p, float dx, float dy)						\n\
{																\n\
	vec2 uv = p.xy + vec2(dx , dy ) / CC_EdgeDetectionSize.xy;	\n\
	vec4 c = texture2D(CC_Texture0, uv.xy);						\n\
	return 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;				\n\
}																\n\
																\n\
void main(void)													\n\
{																\n\
	vec2 p = v_texCoord.xy;										\n\
	float gx = 0.0;												\n\
	gx += -1.0 * lookup(p, -1.0, -1.0);							\n\
	gx += -2.0 * lookup(p, -1.0,  0.0);							\n\
	gx += -1.0 * lookup(p, -1.0,  1.0);							\n\
	gx +=  1.0 * lookup(p,  1.0, -1.0);							\n\
	gx +=  2.0 * lookup(p,  1.0,  0.0);							\n\
	gx +=  1.0 * lookup(p,  1.0,  1.0);							\n\
																\n\
	float gy = 0.0;												\n\
	gy += -1.0 * lookup(p, -1.0, -1.0);							\n\
	gy += -2.0 * lookup(p,  0.0, -1.0);							\n\
	gy += -1.0 * lookup(p,  1.0, -1.0);							\n\
	gy +=  1.0 * lookup(p, -1.0,  1.0);							\n\
	gy +=  2.0 * lookup(p,  0.0,  1.0);							\n\
	gy +=  1.0 * lookup(p,  1.0,  1.0);							\n\
																\n\
	float g = gx*gx + gy*gy;									\n\
																\n\
	gl_FragColor.xyz = vec3(1.-g);								\n\
	gl_FragColor.w = 1.;										\n\
}";