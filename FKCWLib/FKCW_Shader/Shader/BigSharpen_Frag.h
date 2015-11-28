//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	BigSharpen_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 磨砂
static const char* ShaderBigSharpenFrag =
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
		// Sharpen weighting:										\n\
		//  0 -1 -1 -1  0											\n\
		// -1  2 -4  2 -1 											\n\
		// -1 -4 13 -4 -1 											\n\
		// -1  2 -4  2 -1											\n\
		//  0 -1 -1 -1  0											\n\
																	\n\
		gl_FragColor = (											\n\
		(-1.0  * ( sample[1] + sample[2]  + sample[3] + sample[5] +	\n\
		sample[9] + sample[10] + sample[14] + sample[15] + 			\n\
		sample[19] + sample[21] + sample[22] + sample[23]) ) +		\n\
		(2.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +	\n\
		(-4.0 *(sample[7] + sample[11] + sample[13] + sample[17]))+	\n\
		( 21.0 * sample[12] )										\n\
		);															\n\
	}";