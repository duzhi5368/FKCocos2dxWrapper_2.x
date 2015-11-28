//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	FKSpriteEx.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
/*
	资源异步加载精灵。
	可以添加自定义shader以及FKCW_Shader的精灵。
	可以自定义绘制顺序的精灵。
	可以简便的添加FKCW_EffectNode效果的精灵。
*/
class FKSpriteEx : public CCSprite
{
public:
	FKSpriteEx();
	~FKSpriteEx();
public:
	static FKSpriteEx* Create();
	static FKSpriteEx* Create(const char *pszFileName);
	static FKSpriteEx* Create(const char *pszFileName, const CCRect& rect);
public:
	virtual void draw();
	virtual bool init();
	virtual bool initWithFile(const char *pszFilename);
	virtual bool initWithFile(const char *pszFilename, const CCRect& rect);
protected:
	// 异步资源加载完毕
	void		_OnImageLoadOver( CCObject* Sender );
private:
	bool			m_bIsVailed;		// 对象是否有效（是否资源加载完毕）
	CCRect			m_tagRect;			// 矩形区域  
};