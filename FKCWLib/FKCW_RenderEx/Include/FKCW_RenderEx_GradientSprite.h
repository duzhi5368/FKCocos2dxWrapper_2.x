//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_RenderEx_GradientSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	渐变精灵
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 一个会渐变效果的精灵
class FKCW_RenderEx_GradientSprite : public CCSprite
{
private:
	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_tagStartColor, StartColor)
	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_tagEndColor, EndColor)
	CC_PROPERTY(GLubyte, m_cStartOpacity, StartOpacity)
	CC_PROPERTY(GLubyte, m_cEndOpacity, EndOpacity)
	CC_PROPERTY_PASS_BY_REF(CCPoint, m_tagAlongVector, Vector)					// 颜色渐变向量
	CC_PROPERTY(bool, m_bIsCompressedInterpolation, CompressedInterpolation);	// 是否使用压缩补差
private:
	// 更新颜色
	void			_UpdateColor();
public:
	FKCW_RenderEx_GradientSprite();
	virtual ~FKCW_RenderEx_GradientSprite();

	static	FKCW_RenderEx_GradientSprite*	create();
	static	FKCW_RenderEx_GradientSprite*	create(const char *pszFileName);
	static	FKCW_RenderEx_GradientSprite*	create(const char *pszFileName, const CCRect& rect);
	static	FKCW_RenderEx_GradientSprite*	createWithTexture(CCTexture2D *pTexture);
	static	FKCW_RenderEx_GradientSprite*	createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	static	FKCW_RenderEx_GradientSprite*	createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	static	FKCW_RenderEx_GradientSprite*	createWithSpriteFrameName(const char *pszSpriteFrameName);

	// 继承自CCNodeRGBA
	virtual void setColor(const ccColor3B& color3);
	virtual void updateDisplayedColor(const ccColor3B& parentColor);
	virtual void setOpacity(GLubyte opacity);
	virtual void setOpacityModifyRGB(bool modify);
	virtual void updateDisplayedOpacity(GLubyte parentOpacity);

	// 设置渐变色
	virtual void setColor( const ccColor4B& start, const ccColor4B& end, const CCPoint& v );
	// 便捷
	bool	IsCompressedInterpolation(){ return getCompressedInterpolation(); }
};