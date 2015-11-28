//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Brightness_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
// 高光
// 使用添加 或者 减去黑色，达到调整图片光亮度的效果
static const char* ShaderBrightnessFrag= 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	\n\
	void main()											\n\
	{													\n\
	float T = 2.0;										\n\
	vec2 st = v_texCoord.st;							\n\
	vec3 irgb = texture2D(u_Texture, st).rgb;			\n\
	vec3 black = vec3(0., 0., 0.);						\n\
	gl_FragColor = vec4(mix(black, irgb, T), 1.);		\n\
	}";	