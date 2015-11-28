//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_RenderEx_EraserSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	橡皮擦效果精灵
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
class FKCW_RenderEx_EraserSprite : public CCSprite, public CCTargetedTouchDelegate
{
public:
	FKCW_RenderEx_EraserSprite();
	~FKCW_RenderEx_EraserSprite();

	// 创建擦除精灵
	// 参数： pszFileName--精灵文件名, eraserImmediately---是否可以立即擦除 true---可以， false---不能
	//  drawType---擦除形状， 0---正方形，1---圆形， 2---自定义形状(可用一张图片代替)
	static FKCW_RenderEx_EraserSprite* create(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0, const float& p_fSize = 10.0f);

	virtual bool init(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0, const float& p_fSize = 10.0f);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

public:
	CC_PROPERTY(bool, m_bEraser, Eraser);			// 设置是否可以擦除
	CC_PROPERTY(float, m_fDrawWidth, DrawWidth);	// 擦除大小，默认为10.f;
	CC_PROPERTY(bool, m_bEraserOk, EraserOk);		// 是否擦除完毕

	// 自定义擦除形状
	// 注：图片要求中间形状是镂空的。
	void setDrawStencil(const char *pszFileName);	
private:
	// 利用混合模式，实现擦除效果
	void eraseByBlend(CCPoint& point);
	void eraseByColorMask();

private:
	void initData(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0, const float fSize = 10.0f );

	CCSize				m_tagSpriteSize;		// 精灵大小
	unsigned int		m_uiDrawType;			// 擦除形状 0---正方形， 1---圆形 2--自定义形状
	CCPoint				m_touchPoint;			// 触摸点
	CCTexture2D*		m_drawTextture;			// 自定义图片时，设置的擦除图片
	CCRenderTexture*	m_pRTex;				// 渲染纹理
	CCDrawNode*			m_pEraser;				// 渲染节点
};
