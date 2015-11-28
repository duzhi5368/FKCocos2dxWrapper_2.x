//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	OldPhoto_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
static const char* ShaderOldPhotoFrag = 
	"																													\n\
	#ifdef GL_ES 								 																		\n\
		precision mediump float; 					 																	\n\
	#endif 																												\n\
																														\n\
	uniform sampler2D u_texture; 				 																		\n\
	varying vec2 v_texCoord; 					 																		\n\
	varying vec4 v_fragmentColor; 																						\n\
																														\n\
	void main(void) 							 																		\n\
	{ 											 																		\n\
		// vec3( 0.299, 0.587, 0.114 ) 是RGB转YUV的参数值，生成灰色图														\n\
		float MixColor = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114));							\n\
		// 使用灰色图进行颜色混合																							\n\
		vec4 blendColor = vec4( 1.2, 1.0, 0.8, 1.0 ); // 调整这个值以修改最终混合色值										\n\
		gl_FragColor = vec4(MixColor * blendColor.r, MixColor * blendColor.g, MixColor * blendColor.b, blendColor.a);	\n\
	}";
//-------------------------------------------------------------------------