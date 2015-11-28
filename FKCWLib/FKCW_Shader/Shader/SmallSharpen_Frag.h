//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	SmallSharpen_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 磨砂
static const char* ShaderSmallSharpenFrag =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
																	\n\
	void main(void)													\n\
	{																\n\
		vec4 sample[25];											\n\
																	\n\
		for (int i = 0; i < 25; i++)								\n\
		{															\n\
			sample[i] = texture2D(u_Texture, 						\n\
				v_texCoord.st + u_TextureCoordOffset[i]);			\n\
		}															\n\
		//--------------------------------------------------------	\n\
		//   -1 -1 -1												\n\
		//   -1  9 -1												\n\
		//   -1 -1 -1												\n\
																	\n\
		gl_FragColor = (sample[4] * 9.0) -							\n\
			(sample[0] + sample[1] + sample[2] + 					\n\
			sample[3] + sample[5] + 								\n\
			sample[6] + sample[7] + sample[8]);						\n\
	}";