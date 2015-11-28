//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Sobel_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// Sobel边缘检测
static const char* ShaderSobelFrag =
	"																	\n\
	precision mediump float;											\n\
	uniform sampler2D u_Texture;										\n\
	uniform vec2 u_TextureCoordOffset[25];								\n\
	varying vec2 v_texCoord; 											\n\
	varying vec4 v_fragmentColor;										\n\
	\n\
	void main(void)														\n\
	{																	\n\
	vec4 sample[25];												\n\
	\n\
	for (int i = 0; i < 25; i++)									\n\
	{																\n\
	sample[i] = texture2D(u_Texture, 								\n\
	v_texCoord.st + u_TextureCoordOffset[i]);						\n\
	}																\n\
	// Sobel x:														\n\
	// 1  2  0 -2 -1												\n\
	// 4  8  0 -8 -4												\n\
	// 6 12  0 -12-6	/ 12										\n\
	// 4  8  0 -8 -4												\n\
	// 1  2  0 -2 -1												\n\
	// Sobel y:														\n\
	// -1 -4 -6 -4 -1												\n\
	// -2 -8 -12-8 -2												\n\
	//  0  0  0  0  0	/ 12										\n\
	//  2  8 12  8  2												\n\
	//  1  4  6  4  1												\n\
	\n\
	vec4 vertEdge = sample[0] + 4.0 * sample[1] +					\n\
	6.0 * sample[2] + 4.0 * sample[3] + sample[4] +					\n\
	2.0 * sample[5] + 8.0 * sample[6] + 12.0 * sample[7] +			\n\
	8.0 * sample[8] + 2.0 * sample[9] - 2.0 * sample[15] -			\n\
	8.0 * sample[16] - 12.0 * sample[17] - 8.0 * sample[18] - 		\n\
	2.0 * sample[19] - sample[20] - 4.0 * sample[21] - 				\n\
	6.0 * sample[22] - 4.0 * sample[23] - sample[24];				\n\
	\n\
	vec4 horizEdge = - sample[0] - 2.0 * sample[1] +				\n\
	2.0 * sample[3] + sample[4] - 4.0 * sample[5] - 				\n\
	8.0 * sample[6] + 8.0 * sample[8] + 4.0 * sample[9] - 			\n\
	6.0 * sample[10] - 12.0 * sample[11] + 12.0 * sample[13] +		\n\
	6.0 * sample[14] - 4.0 * sample[15] - 8.0 * sample[16] +		\n\
	8.0 * sample[18] + 4.0 * sample[19] - sample[20] -				\n\
	2.0 * sample[21] + 2.0 * sample[23] + sample[24];				\n\
	\n\
	//gl_FragColor.rgb = sqrt(horizEdge.rgb) + sqrt(vertEdge.rgb);	\n\
	gl_FragColor.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + 		\n\
	(vertEdge.rgb * vertEdge.rgb)) / 12.0f;						\n\
	gl_FragColor.a = 1.0;											\n\
	}";