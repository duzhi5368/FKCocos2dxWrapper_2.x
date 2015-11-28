//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_Texture2DMutable.h"
#include "platform/marmalade/CCGL.h"
//-------------------------------------------------------------------------
FKCW_RenderEx_Texture2DMutable::FKCW_RenderEx_Texture2DMutable()
{
	m_pTexData = NULL;
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	m_pOriginalData = NULL;
#endif
}
//-------------------------------------------------------------------------
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
void* FKCW_RenderEx_Texture2DMutable::getOriginalTexData() 
{
	return m_pOriginalData;
}
#endif
//-------------------------------------------------------------------------
void* FKCW_RenderEx_Texture2DMutable::getTexData() 
{
	return m_pTexData;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::setTexData(void *var) 
{
	m_pTexData = var;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::updateData() 
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM !=CC_PLATFORM_ANDROID

	glBindTexture(GL_TEXTURE_2D, m_uName);

	switch(m_ePixelFormat)
	{
	case kTexture2DPixelFormat_RGBA8888:
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTexData);
		break;

	case kTexture2DPixelFormat_RGBA4444:
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, m_pTexData);
		break;

	case kTexture2DPixelFormat_RGB565:
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_pTexData);
		break;

	case kTexture2DPixelFormat_RGB5A1:
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, m_pTexData);
		break;

	default:
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	m_bIsDirty = false;
#else


#endif

}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::updateData(unsigned int fbo, unsigned int oldfbo)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	switch(m_ePixelFormat)
	{
	case kTexture2DPixelFormat_RGBA8888:
		(0, 
			0, 
			m_uPixelsWide,
			m_uPixelsHigh, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			m_pTexData);
		break;

	case kTexture2DPixelFormat_RGBA4444:
		glReadPixels(0, 
			0, 
			m_uPixelsWide, 
			m_uPixelsHigh,  
			GL_RGBA, 
			GL_UNSIGNED_SHORT_4_4_4_4, 
			m_pTexData);
		break;

	case kTexture2DPixelFormat_RGB565:
		glReadPixels(0, 
			0, 
			m_uPixelsWide, 
			m_uPixelsHigh, 
			GL_RGB, 
			GL_UNSIGNED_SHORT_5_6_5, 
			m_pTexData);
		break;

	case kTexture2DPixelFormat_RGB5A1:
		glReadPixels(0, 
			0, 
			m_uPixelsWide, 
			m_uPixelsHigh, 
			GL_RGBA, 
			GL_UNSIGNED_SHORT_5_5_5_1, 
			m_pTexData);
		break;

	default:
		break;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, oldfbo);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::releaseData(void* data)
{
	// 暂时不要进行操作
}
//-------------------------------------------------------------------------
void* FKCW_RenderEx_Texture2DMutable::keepData(void* data, unsigned int lenght)
{
	void *newData = malloc(lenght);
	memmove(newData, data, lenght);
	return newData;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int width, unsigned int height, const CCSize& size)
{
	if(!CCTexture2D::initWithData(data, pixelFormat, width, height, size)) 
	{
		return false;
	}

	switch (pixelFormat) 
	{
	case kTexture2DPixelFormat_RGBA8888:	m_unBytesPerPixel = 4; break;
	case kTexture2DPixelFormat_A8:			m_unBytesPerPixel = 1; break;
	case kTexture2DPixelFormat_RGBA4444:
	case kTexture2DPixelFormat_RGB565:
	case kTexture2DPixelFormat_RGB5A1:
		m_unBytesPerPixel = 2;
		break;
	default:break;
	}
	m_pTexData = (void*) data;
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	unsigned int max = width*height*m_unBytesPerPixel;
	m_pOriginalData = malloc(max);
	memcpy(m_pOriginalData, m_pTexData, max);
	m_pTexData = m_pOriginalData;
#endif

	return true;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::initWithString(const char *text, const char *fontName, float fontSize)
{
	CCSize dimensions = CCSizeMake(0,0);
	CCTextAlignment hAlignment = kCCTextAlignmentCenter;
	CCVerticalTextAlignment vAlignment = kCCVerticalTextAlignmentTop;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	ccFontDefinition tempDef;

	tempDef.m_shadow.m_shadowEnabled = false;
	tempDef.m_stroke.m_strokeEnabled = false;


	tempDef.m_fontName      = std::string(fontName);
	tempDef.m_fontSize      = fontSize;
	tempDef.m_dimensions    = dimensions;
	tempDef.m_alignment     = hAlignment;
	tempDef.m_vertAlignment = vAlignment;
	tempDef.m_fontFillColor = ccWHITE;

	return cocos2d::CCTexture2D::initWithString(text, &tempDef);


#else


#if CC_ENABLE_CACHE_TEXTURE_DATA
	// cache the texture data
	VolatileTexture::addStringTexture(this, text, dimensions, hAlignment, vAlignment, fontName, fontSize);
#endif

	bool bRet = false;
	CCImage::ETextAlign eAlign;

	if (kCCVerticalTextAlignmentTop == vAlignment)
	{
		eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignTop
			: (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignTopLeft : CCImage::kAlignTopRight;
	}
	else if (kCCVerticalTextAlignmentCenter == vAlignment)
	{
		eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignCenter
			: (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignLeft : CCImage::kAlignRight;
	}
	else if (kCCVerticalTextAlignmentBottom == vAlignment)
	{
		eAlign = (kCCTextAlignmentCenter == hAlignment) ? CCImage::kAlignBottom
			: (kCCTextAlignmentLeft == hAlignment) ? CCImage::kAlignBottomLeft : CCImage::kAlignBottomRight;
	}
	else
	{
		CCAssert(false, "Not supported alignment format!");
		return false;
	}

	do
	{
		CCImage* pImage = new CCImage();
		CC_BREAK_IF(NULL == pImage);
		bRet = pImage->initWithString(text, (int)dimensions.width, (int)dimensions.height, eAlign, fontName, (int)fontSize);
		CC_BREAK_IF(!bRet);
		bRet = initWithImage(pImage);
		CC_SAFE_RELEASE(pImage);
	} while (0);


	return bRet;


#endif
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::initWithImage(CCImage *uiImage)
{
	if (uiImage == NULL)
	{
		CCLOG("cocos2d: CCTexture2D. Can't create Texture. UIImage is nil");
		return false;
	}

	unsigned int imageWidth = uiImage->getWidth();
	unsigned int imageHeight = uiImage->getHeight();

	CCConfiguration *conf = CCConfiguration::sharedConfiguration();

	unsigned maxTextureSize = conf->getMaxTextureSize();
	if (imageWidth > maxTextureSize || imageHeight > maxTextureSize) 
	{
		CCLOG("cocos2d: WARNING: Image (%u x %u) is bigger than the supported %u x %u", imageWidth, imageHeight, maxTextureSize, maxTextureSize);
		return false;
	}

	// always load premultiplied images
	return initPremultipliedATextureWithImage(uiImage, imageWidth, imageHeight);
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::initPremultipliedATextureWithImage(CCImage *image, unsigned int width, unsigned int height)
{
	unsigned char*            tempData = image->getData();
	unsigned int*             inPixel32  = NULL;
	unsigned char*            inPixel8 = NULL;
	unsigned short*           outPixel16 = NULL;
	bool                      hasAlpha = image->hasAlpha();
	CCSize                    imageSize = CCSizeMake((float)(image->getWidth()), (float)(image->getHeight()));
	CCTexture2DPixelFormat    pixelFormat;
	size_t                    bpp = image->getBitsPerComponent();

	// compute pixel format
	if (hasAlpha)
	{
		pixelFormat = kCCTexture2DPixelFormat_Default;
	}
	else
	{
		if (bpp >= 8)
		{
			pixelFormat = kCCTexture2DPixelFormat_RGB888;
		}
		else 
		{
			pixelFormat = kCCTexture2DPixelFormat_RGB565;
		}

	}

	// Repack the pixel data into the right format
	unsigned int length = width * height;

	if (pixelFormat == kCCTexture2DPixelFormat_RGB565)
	{
		if (hasAlpha)
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"

			tempData = new unsigned char[width * height * 2];
			outPixel16 = (unsigned short*)tempData;
			inPixel32 = (unsigned int*)image->getData();

			for(unsigned int i = 0; i < length; ++i, ++inPixel32)
			{
				*outPixel16++ = 
					((((*inPixel32 >>  0) & 0xFF) >> 3) << 11) |  // R
					((((*inPixel32 >>  8) & 0xFF) >> 2) << 5)  |  // G
					((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);    // B
			}
		}
		else 
		{
			// Convert "RRRRRRRRRGGGGGGGGBBBBBBBB" to "RRRRRGGGGGGBBBBB"

			tempData = new unsigned char[width * height * 2];
			outPixel16 = (unsigned short*)tempData;
			inPixel8 = (unsigned char*)image->getData();

			for(unsigned int i = 0; i < length; ++i)
			{
				*outPixel16++ = 
					(((*inPixel8++ & 0xFF) >> 3) << 11) |  // R
					(((*inPixel8++ & 0xFF) >> 2) << 5)  |  // G
					(((*inPixel8++ & 0xFF) >> 3) << 0);    // B
			}
		}    
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_RGBA4444)
	{
		// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"

		inPixel32 = (unsigned int*)image->getData();  
		tempData = new unsigned char[width * height * 2];
		outPixel16 = (unsigned short*)tempData;

		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
		}
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
	{
		// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGBBBBBA"
		inPixel32 = (unsigned int*)image->getData();   
		tempData = new unsigned char[width * height * 2];
		outPixel16 = (unsigned short*)tempData;

		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel16++ = 
				((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | // R
				((((*inPixel32 >> 8) & 0xFF) >> 3) <<  6) | // G
				((((*inPixel32 >> 16) & 0xFF) >> 3) << 1) | // B
				((((*inPixel32 >> 24) & 0xFF) >> 7) << 0);  // A
		}
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_A8)
	{
		// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "AAAAAAAA"
		inPixel32 = (unsigned int*)image->getData();
		tempData = new unsigned char[width * height];
		unsigned char *outPixel8 = tempData;

		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel8++ = (*inPixel32 >> 24) & 0xFF;  // A
		}
	}

	if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
	{
		// Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRGGGGGGGGBBBBBBBB"
		inPixel32 = (unsigned int*)image->getData();
		tempData = new unsigned char[width * height * 3];
		unsigned char *outPixel8 = tempData;

		for(unsigned int i = 0; i < length; ++i, ++inPixel32)
		{
			*outPixel8++ = (*inPixel32 >> 0) & 0xFF; // R
			*outPixel8++ = (*inPixel32 >> 8) & 0xFF; // G
			*outPixel8++ = (*inPixel32 >> 16) & 0xFF; // B
		}
	}

	initWithData(tempData, pixelFormat, width, height, imageSize);

	if (tempData != image->getData())
	{
		delete [] tempData;
	}

	m_bHasPremultipliedAlpha = image->isPremultipliedAlpha();
	return true;
}
//-------------------------------------------------------------------------
ccColor4B FKCW_RenderEx_Texture2DMutable::pixelAt(const CCPoint& pt)
{
	ccColor4B c = {0, 0, 0, 0};
	if(!m_pTexData) return c;
	if(pt.x < 0 || pt.y < 0) return c;
	if(pt.x >= m_tContentSize.width || pt.y >= m_tContentSize.height) return c;

	unsigned int x = static_cast<unsigned int>(pt.x), y = static_cast<unsigned int>(pt.y);

	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		unsigned int *pixel = (unsigned int *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		c.r = *pixel & 0xff;
		c.g = (*pixel >> 8) & 0xff;
		c.b = (*pixel >> 16) & 0xff;
		c.a = (*pixel >> 24) & 0xff;
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		c.a = ((*pixel & 0xf) << 4) | (*pixel & 0xf);
		c.b = (((*pixel >> 4) & 0xf) << 4) | ((*pixel >> 4) & 0xf);
		c.g = (((*pixel >> 8) & 0xf) << 4) | ((*pixel >> 8) & 0xf);
		c.r = (((*pixel >> 12) & 0xf) << 4) | ((*pixel >> 12) & 0xf);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.g = ((*pixel >> 6) & 0x1f)<<3;
		c.b = ((*pixel >> 1) & 0x1f)<<3;
		c.a = (*pixel & 0x1)*255;
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		c.b = (*pixel & 0x1f)<<3;
		c.g = ((*pixel >> 5) & 0x3f)<<2;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.a = 255;
	} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
		GLubyte *pixel = (GLubyte *)m_pTexData;
		c.a = pixel[(y * m_uPixelsWide) + x];
		// 默认为白色
		c.r = 255;
		c.g = 255;
		c.b = 255;
	}

	return c;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::setPixelAt(const CCPoint& pt, const ccColor4B& c)
{
	if(!m_pTexData)return false;
	if(pt.x < 0 || pt.y < 0) return false;
	if(pt.x >= m_tContentSize.width || pt.y >= m_tContentSize.height) return false;
	unsigned int x = static_cast<unsigned int>(pt.x), y = static_cast<unsigned int>(pt.y);

	m_bIsDirty = true;

	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		unsigned int *pixel = (unsigned int *)m_pTexData;
		pixel[(y * m_uPixelsWide) + x] = (c.a << 24) | (c.b << 16) | (c.g << 8) | c.r;
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		*pixel = ((c.r >> 4) << 12) | ((c.g >> 4) << 8) | ((c.b >> 4) << 4) | (c.a >> 4);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a > 0);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
		GLushort *pixel = (GLushort *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 2) << 5) | (c.b >> 3);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
		GLubyte *pixel = (GLubyte *)m_pTexData;
		pixel[(y * m_uPixelsWide) + x] = c.a;
	} else {
		m_bIsDirty = false;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::fill(const ccColor4B& c)
{
	unsigned int color;
	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		color = (c.a << 24) | (c.b << 16) | (c.g << 8) | c.r;

	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
		color = ((c.r >> 4) << 12) | ((c.g >> 4) << 8) | ((c.b >> 4) << 4) | (c.a >> 4);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
		color = ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a > 0);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
		color = ((c.r >> 3) << 11) | ((c.g >> 2) << 5) | (c.b >> 3);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
		color = c.a;
	}
	for(int y = 0; y < m_tContentSize.height; ++y)
		for(int x = 0; x < m_tContentSize.width; ++x) {
			if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
				unsigned int *pixel = (unsigned int *)m_pTexData;
				pixel[(y * m_uPixelsWide) + x] = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
				GLubyte *pixel = (GLubyte *)m_pTexData;
				pixel[(y * m_uPixelsWide) + x] = color;
			}
		}
}
//-------------------------------------------------------------------------
unsigned int FKCW_RenderEx_Texture2DMutable::pixelAt(int x, int y) {
	if(!m_pTexData) return 0;
	if(x < 0 || y < 0) return 0;
	if(x >= m_tContentSize.width || y >= m_tContentSize.height) return 0;

	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		unsigned int *pixel = (unsigned int *)m_pTexData;
		pixel = pixel + (y * m_uPixelsWide) + x;

		return *pixel;
	}

	return 0;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_Texture2DMutable::setPixelAt(int x, int y, unsigned int c) 
{
	if(!m_pTexData) return 0;
	if(x < 0 || y < 0) return 0;
	if(x >= m_tContentSize.width || y >= m_tContentSize.height) return 0;

	m_bIsDirty = true;

	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		unsigned int *pixel = (unsigned int *)m_pTexData;
		*pixel = c;

	} else {
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::fillRect(const CCRect& rect, unsigned int color) 
{
	CCRect rectToFill = rect;
	if(rectToFill.size.width + rectToFill.origin.x > m_tContentSize.width)
		rectToFill.size.width = m_tContentSize.width - rectToFill.origin.x;
	if(rectToFill.size.height + rectToFill.origin.y > m_tContentSize.height)
		rectToFill.size.height = m_tContentSize.height - rectToFill.origin.y;

	for(int y = static_cast<int>(rectToFill.origin.y); 
		y < static_cast<int>(rectToFill.origin.y + rectToFill.size.height); 
		++y)
	{
		for(int x = static_cast<int>(rectToFill.origin.x);
			x < static_cast<int>(rectToFill.origin.x + rectToFill.size.width);
			++x) 
		{
			if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
				unsigned int *pixel = (unsigned int *)m_pTexData;
				pixel[(y * m_uPixelsWide) + x] = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
				GLushort *pixel = (GLushort *)m_pTexData;
				pixel = pixel + (y * m_uPixelsWide) + x;
				*pixel = color;
			} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
				GLubyte *pixel = (GLubyte *)m_pTexData;
				pixel[(y * m_uPixelsWide) + x] = color;
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::fillRect(const CCRect& rect, const ccColor4B& c) {
	unsigned int color;
	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888){
		color = (c.a << 24) | (c.b << 16) | (c.g << 8) | c.r;
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGBA4444){
		color = ((c.r >> 4) << 12) | ((c.g >> 4) << 8) | ((c.b >> 4) << 4) | (c.a >> 4);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB5A1){
		color = ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a > 0);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_RGB565){
		color = ((c.r >> 3) << 11) | ((c.g >> 2) << 5) | (c.b >> 3);
	} else if(m_ePixelFormat == kTexture2DPixelFormat_A8){
		color = c.a;
	}
	FKCW_RenderEx_Texture2DMutable::fillRect(rect, color);
}
//-------------------------------------------------------------------------
unsigned int* FKCW_RenderEx_Texture2DMutable::getDataRGBA() {
	if(m_ePixelFormat == kTexture2DPixelFormat_RGBA8888) {
		return (unsigned int*)m_pTexData;
	}
	return 0;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::putDataRGBA() {
	m_bIsDirty = true;
	apply();
}
//-------------------------------------------------------------------------
CCTexture2D* FKCW_RenderEx_Texture2DMutable::copyMutable(bool isMutable )
{	
	CCTexture2D* co;
	if(isMutable)
	{
		int mem = m_uPixelsWide*m_uPixelsHigh*m_unBytesPerPixel;
		void *newData = malloc(mem);
		memcpy(newData, m_pTexData, mem);
		co = new FKCW_RenderEx_Texture2DMutable();
		if (!co->initWithData(newData, m_ePixelFormat, m_uPixelsWide, m_uPixelsHigh, m_tContentSize)) {
			delete co;
			co = NULL;
		}
	}else {

		co = new CCTexture2D();
		if (!co->initWithData(m_pTexData, m_ePixelFormat, m_uPixelsWide, m_uPixelsHigh, m_tContentSize)) {
			delete co;
			co = NULL;
		}
	}

	return co;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_Texture2DMutable* FKCW_RenderEx_Texture2DMutable::copy()
{
	return (FKCW_RenderEx_Texture2DMutable*)this->copyMutable( true );
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::copy(FKCW_RenderEx_Texture2DMutable* textureToCopy, const CCPoint& offset)
{
	for(int r = 0; r < m_tContentSize.height;++r){
		for(int c = 0; c < m_tContentSize.width; ++c){
			setPixelAt(CCPointMake(c + offset.x, r + offset.y), textureToCopy->pixelAt(CCPointMake(c, r)));
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::restore()
{
#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	memcpy(m_pTexData, m_pOriginalData, m_unBytesPerPixel*m_uPixelsWide*m_uPixelsHigh);
	this->apply();
#else
	CCAssert(false, "Exception:  CCMutableTexture.restore was disabled by the user.");
#endif
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_Texture2DMutable::apply()
{
	if(!m_pTexData) return;

	glBindTexture(GL_TEXTURE_2D, m_uName);

	switch(m_ePixelFormat)
	{
	case kTexture2DPixelFormat_RGBA8888:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uPixelsWide, m_uPixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTexData);
		break;
	case kTexture2DPixelFormat_RGBA4444:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uPixelsWide, m_uPixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, m_pTexData);
		break;
	case kTexture2DPixelFormat_RGB5A1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uPixelsWide, m_uPixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, m_pTexData);
		break;
	case kTexture2DPixelFormat_RGB565:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_uPixelsWide, m_uPixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, m_pTexData);
		break;
	case kTexture2DPixelFormat_A8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_uPixelsWide, m_uPixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_pTexData);
		break;
	default:
		CCAssert(false, "NSInternalInconsistencyException");

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	m_bIsDirty = false;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_Texture2DMutable::~FKCW_RenderEx_Texture2DMutable(void)
{
	CCLOGINFO("cocos2d: deallocing %p", this);

#if FKCW_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	if( m_pTexData != m_pOriginalData )
		free(m_pOriginalData);
#endif
	// free(m_pTexData);
}
//-------------------------------------------------------------------------