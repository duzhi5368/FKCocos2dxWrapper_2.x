//*************************************************************************
//	创建日期:	2014-10-29
//	文件名称:	Lighting_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_lightingMul "CC_lightingMul"
#define kCCUniform_lightingAdd "CC_lightingAdd"

static GLint sUniform_pos_CC_lightingMul = -1;
static GLint sUniform_pos_CC_lightingAdd = -1;

const char* ShaderLightingFrag = "\n\
									 #ifdef GL_ES \n\
									 precision lowp float; \n\
									 #endif \n\
									 \n\
									 varying vec4 v_fragmentColor; \n\
									 varying vec2 v_texCoord; \n\
									 uniform sampler2D CC_Texture0; \n\
									 uniform vec4 CC_lightingMul; \n\
									 uniform vec3 CC_lightingAdd; \n\
									 \n\
									 void main()	{ \n\
									 gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
									 vec3 c = CC_lightingAdd * gl_FragColor.a; \n\
									 gl_FragColor *= CC_lightingMul; \n\
									 gl_FragColor.xyz += c; \n\
									 }";