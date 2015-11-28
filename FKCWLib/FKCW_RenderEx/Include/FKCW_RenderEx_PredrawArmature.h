//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_RenderEx_PredrawArmature.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
#include "cocos-ext.h"
//-------------------------------------------------------------------------
USING_NS_CC_EXT;
//-------------------------------------------------------------------------
// 对 CCArmature 的加强
// 添加一个预渲染函数，意味着当我们在CCSprite渲染之前，可以设置一些自定义shader参数
class FKCW_RenderEx_PredrawArmature : public CCArmature
{
public:
	CC_SYNTHESIZE_RETAIN(CCCallFuncO*, m_pPreDrawFunction, PreDrawFunction);		// 预渲染函数
public:
	static FKCW_RenderEx_PredrawArmature* Create();
	static FKCW_RenderEx_PredrawArmature* Create(const char *name);
	static FKCW_RenderEx_PredrawArmature* Create(const char *name, CCBone *parentBone);
public:
	FKCW_RenderEx_PredrawArmature();
	~FKCW_RenderEx_PredrawArmature();
	virtual void draw();
	virtual CCTextureAtlas* getTexureAtlasWithTexture( CCTexture2D* texture );
};