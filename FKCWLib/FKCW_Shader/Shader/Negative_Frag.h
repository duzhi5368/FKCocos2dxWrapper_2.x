//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Negative_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
// 负色
// 逐像素，使用白色减去当前像素颜色得到
static const GLchar* ShaderNegativeFrag =					
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	void main()											\n\
	{													\n\
	float T = 1.0; 										\n\
	vec2 st = v_texCoord.st;							\n\
	vec3 irgb = texture2D(u_Texture, st).rgb;			\n\
	vec3 neg = vec3(1., 1., 1.)-irgb;					\n\
	gl_FragColor = vec4(mix(irgb,neg, T), 1.);			\n\
	}";