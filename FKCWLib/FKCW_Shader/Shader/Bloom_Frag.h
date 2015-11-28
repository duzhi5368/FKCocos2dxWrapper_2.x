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
const char* ShaderBloomFrag = "																\n\
							 \n\
							 #ifdef GL_ES																				\n\
							 precision mediump float;																	\n\
							 #endif																						\n\
							 \n\
							 varying vec4 v_fragmentColor;																\n\
							 varying vec2 v_texCoord;																	\n\
							 \n\
							 const float blurSize = 1.0/512.0;															\n\
							 const float intensity = 0.35;																\n\
							 void main()																					\n\
							 {																							\n\
							 vec4 sum = vec4(0);																		\n\
							 vec2 texcoord = v_texCoord.xy;															\n\
							 int j;																					\n\
							 int i;																					\n\
							 \n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x - 4.0*blurSize, texcoord.y)) * 0.05;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x - 3.0*blurSize, texcoord.y)) * 0.09;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x - 2.0*blurSize, texcoord.y)) * 0.12;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x - blurSize, texcoord.y)) * 0.15;			\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y)) * 0.16;						\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x + blurSize, texcoord.y)) * 0.15;			\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x + 2.0*blurSize, texcoord.y)) * 0.12;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x + 3.0*blurSize, texcoord.y)) * 0.09;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x + 4.0*blurSize, texcoord.y)) * 0.05;		\n\
							 \n\
							 // blur in y (vertical)																	\n\
							 // take nine samples, with the distance blurSize between them							\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y - 4.0*blurSize)) * 0.05;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y - 3.0*blurSize)) * 0.09;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y - 2.0*blurSize)) * 0.12;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y - blurSize)) * 0.15;			\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y)) * 0.16;						\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y + blurSize)) * 0.15;			\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y + 2.0*blurSize)) * 0.12;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y + 3.0*blurSize)) * 0.09;		\n\
							 sum += texture2D(CC_Texture0, vec2(texcoord.x, texcoord.y + 4.0*blurSize)) * 0.05;		\n\
							 \n\
							 //increase blur with intensity!															\n\
							 gl_FragColor = sum*intensity + texture2D(CC_Texture0, texcoord); 						\n\
							 return;																					\n\
							 }";