//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	FKCW_RenderEx_SpotLightSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	激光灯效果精灵
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
class FKCW_RenderEx_SpotLightSprite : public CCSprite
{
private:
	CCRenderTexture*	m_pRenderTex;
	float				m_fSpotLightRadius;
	ccColor4B			m_tagRenderColor;
	GLfloat *			m_pVertices;
	GLfloat *			m_pCoordinates;
	ccColor4B *			m_pColors;
private:
	FKCW_RenderEx_SpotLightSprite*		InitWithRenderTexture( CCRenderTexture* pRenderTex,
		float fRadius, ccColor4B tagRenderColor );
	void								SpreadOutTimer(float dt);
	void								Tick(float dt);
public:
	static FKCW_RenderEx_SpotLightSprite* Create( CCRenderTexture* pRenderTex,
		float fRadius, ccColor4B tagRenderColor );
public:
	void								RemoveSelf();
};
//-------------------------------------------------------------------------