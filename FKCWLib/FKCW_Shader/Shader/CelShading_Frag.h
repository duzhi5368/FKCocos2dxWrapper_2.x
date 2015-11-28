//*************************************************************************
//	创建日期:	2014-11-21
//	文件名称:	CelShading_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
#define kCCUniform_celShadingSize "CC_CelShadingSize"

static GLint sUniform_pos_CC_celShadingSize = -1;

const char* ShaderCelShadingFrag ="\n\
#ifdef GL_ES																							\n\
	precision mediump float;																			\n\
#endif																									\n\
																										\n\
varying vec4 v_fragmentColor;																			\n\
varying vec2 v_texCoord;																				\n\
																										\n\
uniform vec2 CC_CelShadingSize;																			\n\
																										\n\
#define FILTER_SIZE 3																					\n\
#define COLOR_LEVELS 7.0																				\n\
#define EDGE_FILTER_SIZE 3																				\n\
#define EDGE_THRESHOLD 0.05																				\n\
																										\n\
vec4 edgeFilter(in int px, in int py)																	\n\
{																										\n\
	vec4 color = vec4(0.0);																				\n\
																										\n\
	for (int y = -EDGE_FILTER_SIZE; y <= EDGE_FILTER_SIZE; ++y)											\n\
	{																									\n\
		for (int x = -EDGE_FILTER_SIZE; x <= EDGE_FILTER_SIZE; ++x)										\n\
		{																								\n\
			color += texture2D(CC_Texture0, v_texCoord + vec2(px + x, py + y) / CC_CelShadingSize.xy);	\n\
		}																								\n\
	}																									\n\
																										\n\
	color /= float((2 * EDGE_FILTER_SIZE + 1) * (2 * EDGE_FILTER_SIZE + 1));							\n\
																										\n\
	return color;																						\n\
}																										\n\
																										\n\
void main(void)																							\n\
{																										\n\
	// Shade																							\n\
	vec4 color = vec4(0.0);																				\n\
																										\n\
	for (int y = -FILTER_SIZE; y <= FILTER_SIZE; ++y)													\n\
	{																									\n\
		for (int x = -FILTER_SIZE; x <= FILTER_SIZE; ++x)												\n\
		{																								\n\
			color += texture2D(CC_Texture0, v_texCoord + vec2(x, y) / CC_CelShadingSize.xy);			\n\
		}																								\n\
	}																									\n\
																										\n\
	color /= float((2 * FILTER_SIZE + 1) * (2 * FILTER_SIZE + 1));										\n\
																										\n\
	for (int c = 0; c < 3; ++c)																			\n\
	{																									\n\
		color[c] = floor(COLOR_LEVELS * color[c]) / COLOR_LEVELS;										\n\
	}																									\n\
																										\n\
	// Highlight edges																					\n\
	vec4 sum = abs(edgeFilter(0, 1) - edgeFilter(0, -1));												\n\
	sum += abs(edgeFilter(1, 0) - edgeFilter(-1, 0));													\n\
	sum /= 2.0;																							\n\
																										\n\
	if (length(sum) > EDGE_THRESHOLD)																	\n\
	{																									\n\
		color.rgb = vec3(0.0);																			\n\
	}																									\n\
																										\n\
	gl_FragColor = color;																				\n\
}";