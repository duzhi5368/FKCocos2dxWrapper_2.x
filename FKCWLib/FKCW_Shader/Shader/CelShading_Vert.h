//*************************************************************************
//	创建日期:	2014-10-29
//	文件名称:	Blur_Vert.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
const char* ShaderCelShadingVert = "					\n\
								   attribute vec4 a_position;					\n\
								   attribute vec2 a_texCoord;					\n\
								   attribute vec4 a_color;						\n\
								   \n\
								   #ifdef GL_ES								\n\
								   varying lowp vec4 v_fragmentColor;		\n\
								   varying mediump vec2 v_texCoord;		\n\
								   #else										\n\
								   varying vec4 v_fragmentColor;			\n\
								   varying vec2 v_texCoord;				\n\
								   #endif										\n\
								   \n\
								   void main()									\n\
								   {											\n\
								   gl_Position = CC_PMatrix * a_position;	\n\
								   v_fragmentColor = a_color;				\n\
								   v_texCoord = a_texCoord;				\n\
								   }";
//-------------------------------------------------------------------------