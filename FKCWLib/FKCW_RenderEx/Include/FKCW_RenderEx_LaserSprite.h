//*************************************************************************
//	创建日期:	2014-11-7
//	文件名称:	FKCW_RenderEx_LaserSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 激光，镭射精灵
class FKCW_RenderEx_LaserSprite : public CCSprite
{
protected:
	FKCW_RenderEx_LaserSprite();
public:
	virtual ~FKCW_RenderEx_LaserSprite();
	static FKCW_RenderEx_LaserSprite* Create( float w, float h );

	virtual bool initWithTexture(CCTexture2D *pTexture);
	virtual void setTexture(CCTexture2D *texture);

	void setWidth(float w);
	void setHeight(float h);
};