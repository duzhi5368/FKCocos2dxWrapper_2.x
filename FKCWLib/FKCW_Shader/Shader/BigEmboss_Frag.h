//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	BigEmboss_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_bigEmbossTime "CC_bigEmbossTime"

static GLint sUniform_pos_CC_bigEmbossTime = -1;

const char* ShaderBigEmbossFrag = 
	"																					\n\
	#ifdef GL_ES																		\n\
	precision mediump float;															\n\
	#endif																				\n\
	varying vec2 v_texCoord;															\n\
	uniform sampler2D CC_Texture0;														\n\
	uniform float CC_bigEmbossTime;																\n\
	void main()																			\n\
	{																					\n\
	vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);										\n\
	vec2 texCoord = v_texCoord;															\n\
	texCoord.x += sin(CC_bigEmbossTime) * (onePixel.x * 6.0);									\n\
	texCoord.y += cos(CC_bigEmbossTime) * (onePixel.y * 6.0);									\n\
	vec4 color;																			\n\
	color.rgb = vec3(0.5);																\n\
	color -= texture2D(CC_Texture0, texCoord - onePixel) * 5.0;							\n\
	color += texture2D(CC_Texture0, texCoord + onePixel) * 5.0;							\n\
	color.rgb = vec3((color.r + color.g + color.b) / 3.0);								\n\
	gl_FragColor = vec4(color.rgb, 1);													\n\
	}";
//--------------------------------------------------------