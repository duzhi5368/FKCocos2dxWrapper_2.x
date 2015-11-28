//*************************************************************************
//	创建日期:	2014-11-13
//	文件名称:	FKCW_RenderEx_RenderTextureMutable.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
#include "FKCW_RenderEx_Texture2DMutable.h"
//-------------------------------------------------------------------------
class FKCW_RenderEX_RenderTextureMutable : public CCNode 
{
	CC_PROPERTY(CCSprite*, m_pSprite, Sprite)
public:
	FKCW_RenderEX_RenderTextureMutable();
	virtual ~FKCW_RenderEX_RenderTextureMutable();

	// 创建一块渲染纹理对象
	// 注：仅支持RGB和RGBA
    static FKCW_RenderEX_RenderTextureMutable* Create(int w, int h, CCTexture2DPixelFormat eFormat);
	// 创建一块RGBA8888的渲染纹理对象
    static FKCW_RenderEX_RenderTextureMutable* Create(int w, int h);
    bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat);

    // 开始抓取
	void begin();
	// 开始渲染到纹理上之前，最好先清理一下纹理。
	// 注：真正最有效的方式是先调用clear在调用begin抓取。
    void beginWithClear(float r, float g, float b, float a);
	// 抓取结束
	// para bIsTOCacheTexture       the parameter is only used for android to cache the texture
	void end(bool bIsTOCacheTexture = true);
    // 用单色清除纹理
    void clear(float r, float g, float b, float a);

	// 保存当前纹理到一个文件中
	// 参数：szFilePath 保存到的文件路径 x,y 本缓冲区的左下角坐标，nWidth，nHeight 宽高
	bool saveBuffer(const char *szFilePath, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);
	// 参数：format 保存的图片格式，支持kCCImageFormatPNG 和kCCImageFormatJPG
	bool saveBuffer(int format, const char *name, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);

	FKCW_RenderEx_Texture2DMutable* getTexture() const;

	// 将缓冲区的数据保存到CCImage中
	// 注：只能保存一个RGBA8888格式的Image
	bool getUIImageFromBuffer(CCImage *pImage, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0);

protected:
	GLuint							m_uFBO;
	GLint							m_nOldFBO;
	FKCW_RenderEx_Texture2DMutable*	m_pTexture;
	CCImage*						m_pUITextureImage;
    GLenum							m_ePixelFormat;
};