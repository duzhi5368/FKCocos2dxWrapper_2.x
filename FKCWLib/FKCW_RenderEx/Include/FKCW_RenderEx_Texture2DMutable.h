//*************************************************************************
//	创建日期:	2014-11-13
//	文件名称:	FKCW_RenderEx_Texture2DMutable.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	加强版的纹理，可以实现后期渲染处理，例如取得某点的颜色，设置某些点的颜色等
/*
例子：实现简单的灰度图
	FKCW_RenderEx_Texture2DMutable* texture = mRenderLayer->getTexture(); 
	int width = texture->getContentSize().width; 
	int height = texture->getContentSize().height; 

	unsigned int* data = texture->getDataRGBA(); 
	assert(data); 
	// 逐像素处理
	for(int y = 0; y < height; ++y) 
	{ 
		for(int x = 0; x < width; ++x)
		{ 
			uint32 color =  data[y * texture->getPixelsWide() + x]; 
			uint8 r = COLOR_GETR_RGBA(color); 
			uint8 g = COLOR_GETG_RGBA(color); 
			uint8 b = COLOR_GETB_RGBA(color); 
			uint8 a = COLOR_GETA_RGBA(color); 

			if(a == 0)	// 如果透明则不处理
			continue; 

			uint8 avg = 0.3 * r + 0.59 * g + 0.11 * b; 
			data[y * texture->getPixelsWide() + x] = COLOR_RGBA(avg, avg, avg, a); 
		} 
	} 
	texture->putDataRGBA();		// 设置修改后的数据
*/
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 是否保存源文件
#define FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA 0
//-------------------------------------------------------------------------
class FKCW_RenderEx_Texture2DMutable : public CCTexture2D
{
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	CC_PROPERTY_READONLY(void*, m_pOriginalData, OriginalTexData);
#endif
	CC_PROPERTY(void*, m_pTexData, TexData);
	unsigned int		m_unBytesPerPixel;
	bool				m_bIsDirty;
public:
	FKCW_RenderEx_Texture2DMutable();
	~FKCW_RenderEx_Texture2DMutable(void);
	// 通过一个texture2d进行初始化
	bool	initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, 
		unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);
	bool initWithString(const char *text, const char *fontName, float fontSize);
	bool initWithImage(CCImage *uiImage);
	bool initPremultipliedATextureWithImage(CCImage *image, unsigned int width, unsigned int height);

	// 从Texture中重加载资源数据
	void			updateData();
	// 从FBO中重新加载资源数据
	void			updateData(unsigned int fbo, unsigned int oldfbo);

	// 创建可变纹理功能接口
	void			releaseData(void *data);
	void*			keepData(void *data, unsigned int length);
	void			restore();

	// 获取/填充 RGBA8888 颜色数据
	// 参数：pt 是从左上角视为 0,0 点
	ccColor4B		pixelAt(const CCPoint& pt);
	unsigned int	pixelAt(int x, int y);
	bool			setPixelAt(int x, int y, unsigned int c);
	bool			setPixelAt(const CCPoint& pt, const ccColor4B& c);

	// 获取纹理完整数据，以RGBA8888格式
	unsigned int*	getDataRGBA();
	void			putDataRGBA();
	// 实际使用我们添加的新数据进行纹理更新
	void			apply();

	// 对一个区域填充指定颜色
	void			fillRect(const CCRect& rect, unsigned int c);
	void			fillRect(const CCRect& rect, const ccColor4B& c);
	// 填充颜色到整个纹理
	void			fill(const ccColor4B& c);

	CCTexture2D*	copyMutable(bool isMutable);
	FKCW_RenderEx_Texture2DMutable* copy();
	void			copy(FKCW_RenderEx_Texture2DMutable* textureToCopy, const CCPoint& offset);
};