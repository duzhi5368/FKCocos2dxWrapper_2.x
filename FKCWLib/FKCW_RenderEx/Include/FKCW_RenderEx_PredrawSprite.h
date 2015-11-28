//*************************************************************************
//	创建日期:	2014-11-4
//	文件名称:	FKCW_RenderEx_PredrawSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 对CCSprite的加强
// 1：当我们更换texture之后，可以保存原本的blendFuncion。
// 2：添加一个预渲染函数，意味着当我们在CCSprite渲染之前，可以设置一些自定义shader参数
class FKCW_RenderEx_PredrawSprite : public CCSprite
{
private:
	CC_SYNTHESIZE_BOOL(m_bIsShouldUpdateBlendFunc, ShouldUpdateBlendFunc);		// true表示不更新blendFuction
	CC_SYNTHESIZE_RETAIN(CCCallFuncO*, m_pPreDrawFunction, PreDrawFunction);	// 预渲染函数
protected:
	FKCW_RenderEx_PredrawSprite();
public:
	virtual ~FKCW_RenderEx_PredrawSprite();

	static FKCW_RenderEx_PredrawSprite* create();
	static FKCW_RenderEx_PredrawSprite* create(const char *pszFileName);
	static FKCW_RenderEx_PredrawSprite* create(const char *pszFileName, const CCRect& rect);
	static FKCW_RenderEx_PredrawSprite* createWithTexture(CCTexture2D *pTexture);
	static FKCW_RenderEx_PredrawSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	static FKCW_RenderEx_PredrawSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	static FKCW_RenderEx_PredrawSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);

	// 这里添加了一个预渲染回调
	virtual void draw();

	// 该函数将直接出发 updateBlendFunc.
	// 在多包纹理（一个TexturePacker的特性）中，如果一个动画中的不同帧使用了不同的纹理，
	// 并且精灵使用了非默认的混合模式，则将在 setDisplayFrame() 后自动恢复混合模式。所以，
	// 解决方案是重写 setTexture()并在更新混合函数之前设置一个检查标识
	virtual void setTexture(CCTexture2D *texture);
};