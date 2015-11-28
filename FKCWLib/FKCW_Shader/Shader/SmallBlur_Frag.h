//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	SmallBlur_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
static const char* ShaderSmallBlurFrag =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
		vec4 sample[25];												\n\
																		\n\
		for (int i = 0; i < 25; i++)									\n\
		{																\n\
		sample[i] = texture2D(u_Texture, 								\n\
		v_texCoord.st + u_TextureCoordOffset[i]);						\n\
		}																\n\
		//--------------------------------------------------------		\n\
		//   1 3 1														\n\
		//   3 1 3   / 11												\n\
		//   1 3 1														\n\
																		\n\
		gl_FragColor = (sample[0] + (3.0*sample[1]) + sample[2] + 	\n\
			(3.0*sample[3]) + sample[4] + (3.0*sample[5]) + 			\n\
			sample[6] + (3.0*sample[7]) + sample[8]) / 11.0;			\n\
	}";