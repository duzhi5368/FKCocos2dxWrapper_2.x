//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Constrast_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
// 对比效果
// 使用一个灰色图作为基础图像，和彩色图混合。
// 逐像素的将图片颜色差向灰度两边扩大，得到对比效果ShaderContrastFrag
static const char* ShaderContrastFrag = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	\n\
	void main()											\n\
	{													\n\
	float T = 2.0;									\n\
	vec2 st = v_texCoord.st;						\n\
	vec3 irgb = texture2D(u_Texture, st).rgb;		\n\
	vec3 target = vec3(0.5, 0.5, 0.5);				\n\
	gl_FragColor = vec4(mix(target, irgb, T), 1.);	\n\
	}";