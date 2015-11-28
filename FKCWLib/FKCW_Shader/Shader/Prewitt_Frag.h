//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Prewitt_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// Prewitt边缘检测
static const char* ShaderPrewittFrag =
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
	// Prewitt x:													\n\
	// -5 -4  0  4  5												\n\
	// -8-10  0 10  8												\n\
	//-10-20  0 20 10	/ 20										\n\
	// -8-10  0 10  8												\n\
	// -5 -4  0  4  5												\n\
	// Prewitt y:													\n\
	//  5  8 10  8  5												\n\
	//  4 10 20 10  4												\n\
	//  0  0  0  0  0	/ 20										\n\
	// -4-10-20-10 -4												\n\
	// -5 -8-10 -8 -5												\n\
	\n\
	vec4 vertEdge = - 5.0 * sample[0] - 8.0 * sample[1] -			\n\
	10.0 * sample[2] - 8.0 * sample[3] - 5.0 * sample[4] -			\n\
	4.0 * sample[5] - 10.0 * sample[6] - 20.0 * sample[7] -			\n\
	10.0 * sample[8] - 4.0 * sample[9] + 4.0 * sample[15] +			\n\
	10.0 * sample[16] + 20.0 * sample[17] + 10.0 * sample[18] + 	\n\
	4.0 * sample[19] + 5.0 * sample[20] + 8.0 * sample[21] + 		\n\
	10.0 * sample[22] + 8.0 * sample[23] + 5.0 * sample[24];		\n\
	\n\
	vec4 horizEdge = 5.0 * sample[0] + 4.0 * sample[1] -			\n\
	4.0 * sample[3] - 5.0 * sample[4] + 8.0 * sample[5] + 			\n\
	10.0 * sample[6] - 10.0 * sample[8] - 8.0 * sample[9] + 		\n\
	10.0 * sample[10] + 20.0 * sample[11] - 20.0 * sample[13] -		\n\
	10.0 * sample[14] + 8.0 * sample[15] + 10.0 * sample[16] -		\n\
	10.0 * sample[18] - 8.0 * sample[19] + 5.0 * sample[20] +		\n\
	4.0 * sample[21] - 4.0 * sample[23] - 5.0 * sample[24];			\n\
	\n\
	gl_FragColor.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + 		\n\
	(vertEdge.rgb * vertEdge.rgb)) / 20.0f;						\n\
	gl_FragColor.a = 1.0;											\n\
	}";