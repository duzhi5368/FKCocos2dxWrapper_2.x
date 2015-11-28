//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Erode_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 侵蚀效果
static const char* ShaderErodeFrag =
	"																\n\
	precision mediump float;										\n\
	uniform sampler2D u_Texture;									\n\
	uniform vec2 u_TextureCoordOffset[25];							\n\
	varying vec2 v_texCoord; 										\n\
	varying vec4 v_fragmentColor;									\n\
	\n\
	void main(void)													\n\
	{																\n\
	vec4 sample[25];																\n\
	vec4 minValue = vec4(1.0);														\n\
	\n\
	for (int i = 0; i < 25; i++)													\n\
	{																				\n\
	// Sample a grid around and including our texel								\n\
	sample[i] = texture2D(u_Texture, v_texCoord.st + u_TextureCoordOffset[i]);	\n\
	// Keep the minimum value													\n\
	minValue = min(sample[i], minValue);										\n\
	}																				\n\
	\n\
	gl_FragColor = minValue;														\n\
	}";	