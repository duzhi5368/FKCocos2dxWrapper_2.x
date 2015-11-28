//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	FKCW_Pixel_BaseFunc.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Pixel_Macros.h"
//-------------------------------------------------------------------------
// 旋转纹理
CCTexture2D*				ScaleTexUsingRTT(CCTexture2D*texture,float scaleX,float scaleY);
// 获取一个纹理图片的颜色矩阵
vector<vector<ccColor4B> >	GetColorMatFromImage(CCImage *image);