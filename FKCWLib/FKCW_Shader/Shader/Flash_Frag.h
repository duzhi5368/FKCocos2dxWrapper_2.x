//*************************************************************************
//	创建日期:	2014-10-29
//	文件名称:	Flash_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_flashColor "CC_flashColor"
#define kCCUniform_flashTime "CC_flashTime"

static GLint sUniform_pos_CC_flashColor = -1;
static GLint sUniform_pos_CC_flashTime = -1;

const char* ShaderFlashFrag = "\n\
								  #ifdef GL_ES \n\
								  precision lowp float; \n\
								  #endif \n\
								  \n\
								  varying vec4 v_fragmentColor; \n\
								  varying vec2 v_texCoord; \n\
								  uniform sampler2D CC_Texture0; \n\
								  uniform vec3 CC_flashColor; \n\
								  uniform float CC_flashTime; \n\
								  \n\
								  void main()	{ \n\
								  gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n\
								  if(gl_FragColor.a > 0.0) { \n\
								  vec3 deltaColor = (CC_flashColor - gl_FragColor.xyz) * CC_flashTime * gl_FragColor.a; \n\
								  gl_FragColor.xyz += deltaColor; \n\
								  } \n\
								  }";