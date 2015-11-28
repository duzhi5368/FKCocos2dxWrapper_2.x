//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	FKCW_Pixel_StaticPixelSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Pixel_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Pixel_StaticPixelSprite:public CCSprite
{
public:
	FKCW_Pixel_StaticPixelSprite();
	virtual ~FKCW_Pixel_StaticPixelSprite();
	bool	init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize);
protected:
	CCSize m_perPixelSize;
};