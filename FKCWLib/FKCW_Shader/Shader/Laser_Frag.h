//*************************************************************************
//	创建日期:	2014-10-29
//	文件名称:	Laser_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_CC;
//-------------------------------------------------------------------------
// 默认情况下，cocos2dx将CC_Time视为Vec4,其中y值是 秒
const char* ShaderLaserFrag = "\n\
								  #ifdef GL_ES\n\
								  precision lowp float;\n\
								  #endif\n\
								  \n\
								  varying vec2 v_texCoord;\n\
								  varying vec4 v_fragmentColor;\n\
								  \n\
								  void main()	{\n\
								  float fPosX = v_texCoord.x;\n\
								  float fPosY = v_texCoord.y;\n\
								  \n\
								  float fR = 1.0;\n\
								  float fG = 1.0;\n\
								  float fB = 1.0;\n\
								  float fA = 1.0;\n\
								  \n\
								  float fTime = abs(sin(CC_Time.y));\n\
								  if(fTime < 0.3)	{\n\
								  fTime = 0.3;\n\
								  }\n\
								  \n\
								  fR = fTime / abs(fPosX - 0.5) * 0.6;\n\
								  fG = fR * 0.3 * fTime;\n\
								  fB = fR * (1.0 - fPosY) * fTime;\n\
								  vec3 Color = vec3(fR, fG, fB);\n\
								  \n\
								  float fScale = 1.0;\n\
								  float fPercent = 0.2;\n\
								  float fDis = fPercent;\n\
								  if(fPosX < fPercent) {\n\
								  fDis = fPosX;\n\
								  }\n\
								  if(fPosX > 1.0 - fPercent) {\n\
								  fDis = 1.0 - fPosX;\n\
								  }\n\
								  fScale = fDis / fPercent;\n\
								  fA *= fScale;\n\
								  \n\
								  gl_FragColor.rgb = Color;\n\
								  gl_FragColor.a = fA;\n\
								  }";