//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_RenderEx_TiledSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 一个能够在垂直和水平方向上进行平铺纹理的精灵
// 注：它很适合填充做游戏背景，因为很容易进行动态大小区域的填充
// 注：通过setContentSize()指定贴片面积，则精灵会自动平铺纹理，以填充满整个区域
// 注：当使用atlas纹理的时候，注意应当给额外 1个像素的拉伸。TexturePacker支持拉伸选项（Extrude）
class FKCW_RenderEx_TiledSprite : public CCSprite
{
private:
	CCSprite*			m_pSprite;			// 基本精灵
	bool				m_bDirty;			// 脏标示
	CCTextureAtlas*		m_pAtlas;			// 纹理贴图集
private:
	// 更新纹理贴图集
	void				__UpdateAtlas();
public:
	FKCW_RenderEx_TiledSprite(CCSprite* sprite);
	virtual ~FKCW_RenderEx_TiledSprite();

	static FKCW_RenderEx_TiledSprite* create(const char* fileImage);
	static FKCW_RenderEx_TiledSprite* createWithTexture(CCTexture2D* tex);
	static FKCW_RenderEx_TiledSprite* createWithSpriteFrameName(const char* name);
	static FKCW_RenderEx_TiledSprite* createWithSpriteFrame(CCSpriteFrame* frame);
	static FKCW_RenderEx_TiledSprite* createWithSprite(CCSprite* sprite);

	virtual bool initWithTexture(CCTexture2D* pTexture, const CCRect& rect, bool rotated);
	virtual void setOpacity(GLubyte opacity);
	virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);
	virtual void draw();
	virtual void setContentSize(const CCSize& contentSize);

	virtual void setHeight(float h);
	virtual void setWidth(float w);
};