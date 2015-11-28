//*************************************************************************
//	创建日期:	2014-10-29
//	文件名称:	Blur_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_blurSize		"CC_blurSize"
#define kCCUniform_blurSubtract "CC_blurSubtract"

static GLint sUniform_pos_CC_blurSize = -1;
static GLint sUniform_pos_CC_blurSubtract = -1;

const char* ShaderBlurFrag = "\n\
								 #ifdef GL_ES \n\
								 precision lowp float;\n\
								 #endif\n\
								 \n\
								 varying vec4 v_fragmentColor;\n\
								 varying vec2 v_texCoord;\n\
								 uniform sampler2D CC_Texture0;\n\
								 uniform vec2 CC_blurSize;\n\
								 uniform vec4 CC_blurSubtract;\n\
								 \n\
								 void main() {\n\
								 vec4 sum = vec4(0.0);\n\
								 sum += texture2D(CC_Texture0, v_texCoord - 4.0 * CC_blurSize) * 0.05;\n\
								 sum += texture2D(CC_Texture0, v_texCoord - 3.0 * CC_blurSize) * 0.09;\n\
								 sum += texture2D(CC_Texture0, v_texCoord - 2.0 * CC_blurSize) * 0.12;\n\
								 sum += texture2D(CC_Texture0, v_texCoord - 1.0 * CC_blurSize) * 0.15;\n\
								 sum += texture2D(CC_Texture0, v_texCoord) * 0.16;\n\
								 sum += texture2D(CC_Texture0, v_texCoord + 1.0 * CC_blurSize) * 0.15;\n\
								 sum += texture2D(CC_Texture0, v_texCoord + 2.0 * CC_blurSize) * 0.12;\n\
								 sum += texture2D(CC_Texture0, v_texCoord + 3.0 * CC_blurSize) * 0.09;\n\
								 sum += texture2D(CC_Texture0, v_texCoord + 4.0 * CC_blurSize) * 0.05;\n\
								 \n\
								 gl_FragColor = (sum - CC_blurSubtract) * v_fragmentColor;\n\
								 }";