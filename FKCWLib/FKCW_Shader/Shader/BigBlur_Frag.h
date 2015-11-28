//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	KeyBigBlur.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 模糊效果
static const char* ShaderBigBlurFrag =
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
		// Gaussian weighting:											\n\
		// 1  4  7  4 1													\n\
		// 4 16 26 16 4													\n\
		// 7 26 41 26 7 / 273 (i.e. divide by total of weightings)		\n\
		// 4 16 26 16 4													\n\
		// 1  4  7  4 1													\n\
																		\n\
		gl_FragColor = (												\n\
		(1.0  * (sample[0] + sample[4]  + sample[20] + sample[24])) +	\n\
		(4.0  * (sample[1] + sample[3]  + sample[5]  + sample[9] + 		\n\
			sample[15] + sample[19] + sample[21] + sample[23])) +		\n\
		(7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +	\n\
		(16.0 * (sample[6] + sample[8]  + sample[16] + sample[18])) +	\n\
		(26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +	\n\
		(41.0 * sample[12])												\n\
		) / 273.0;														\n\
	}";