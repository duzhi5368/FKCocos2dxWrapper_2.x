//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	BlackWhite_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 黑白
// 转为灰度图，然后根据阀值，转为黑白
static const char* ShaderBlackWhiteFrag = 
	"													\n\
	precision mediump float;							\n\
	uniform sampler2D u_Texture;						\n\
	varying vec2 v_texCoord; 							\n\
	varying vec4 v_fragmentColor;						\n\
	const vec4 W = vec4(0.2125, 0.7154, 0.0721, 0);		\n\
	\n\
	void main()											\n\
	{													\n\
	vec4 col = texture2D(u_Texture, v_texCoord.st);	\n\
	float lum = dot(col, W);						\n\
	if (0.5 < lum) {								\n\
	gl_FragColor = v_fragmentColor;					\n\
	} else {										\n\
	gl_FragColor = vec4(0, 0, 0, 1);}				\n\
	}";