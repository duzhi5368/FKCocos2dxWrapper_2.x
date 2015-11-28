//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	MotionBlur_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 运动模糊
static const GLchar* ShaderMotionBlurFrag =
	"																	\n\
	precision mediump float;											\n\
	uniform sampler2D u_Texture;										\n\
	uniform vec2 u_TextureCoordOffset[25];								\n\
	varying vec2 v_texCoord; 											\n\
	varying vec4 v_fragmentColor;										\n\
	\n\
	const float contrast = 1.6;											\n\
	const float brightness = 0.3;										\n\
	const float factor = -0.5 * contrast + brightness;					\n\
	\n\
	void main()															\n\
	{																	\n\
	gl_FragColor = vec4(0.0);										\n\
	//vec4 c = texture2D(u_Texture,v_texCoord.st);					\n\
	//gl_FragColor = clamp(c * contrast + factor, 0.0, 1.);			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.028))*0.0044299121055113265;		\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.024))*0.00895781211794;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.020))*0.0215963866053;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.016))*0.0443683338718;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.012))*0.0776744219933;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.008))*0.115876621105;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0, -0.004))*0.147308056121;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord         )*0.159576912161;						\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.004))*0.147308056121;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.008))*0.115876621105;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.012))*0.0776744219933;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.016))*0.0443683338718;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.020))*0.0215963866053;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.024))*0.00895781211794;			\n\
	gl_FragColor += texture2D(u_Texture, v_texCoord + vec2(0.0,  0.028))*0.0044299121055113265;		\n\
	}";