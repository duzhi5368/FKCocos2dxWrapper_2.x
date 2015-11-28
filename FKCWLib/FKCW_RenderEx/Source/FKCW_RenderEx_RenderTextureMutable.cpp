//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_RenderTextureMutable.h"
#include "support/ccUtils.h"
#include "platform/CCFileUtils.h"

#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 )
#include <GLES/gl.h>  
#endif
//-------------------------------------------------------------------------
FKCW_RenderEX_RenderTextureMutable::FKCW_RenderEX_RenderTextureMutable()
: m_pSprite(NULL)
, m_uFBO(0)
, m_nOldFBO(0)
, m_pTexture(0)
, m_pUITextureImage(NULL)
, m_ePixelFormat(kCCTexture2DPixelFormat_RGBA8888)
{
}
//-------------------------------------------------------------------------
FKCW_RenderEX_RenderTextureMutable::~FKCW_RenderEX_RenderTextureMutable()
{
    removeAllChildrenWithCleanup(true);
    glDeleteFramebuffers(1, &m_uFBO);

	CC_SAFE_DELETE(m_pUITextureImage);
}
//-------------------------------------------------------------------------
CCSprite * FKCW_RenderEX_RenderTextureMutable::getSprite()
{
	return m_pSprite;
}
//-------------------------------------------------------------------------
void FKCW_RenderEX_RenderTextureMutable::setSprite(CCSprite* var)
{
	m_pSprite = var;
}
//-------------------------------------------------------------------------
FKCW_RenderEX_RenderTextureMutable * FKCW_RenderEX_RenderTextureMutable::Create(int w, int h, CCTexture2DPixelFormat eFormat)
{
    FKCW_RenderEX_RenderTextureMutable *pRet = new FKCW_RenderEX_RenderTextureMutable();

    if(pRet && pRet->initWithWidthAndHeight(w, h, eFormat))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEX_RenderTextureMutable* FKCW_RenderEX_RenderTextureMutable::Create(int w, int h)
{
	FKCW_RenderEX_RenderTextureMutable *pRet = new FKCW_RenderEX_RenderTextureMutable();

    if(pRet && pRet->initWithWidthAndHeight(w, h, kCCTexture2DPixelFormat_RGBA8888))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEX_RenderTextureMutable::initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat)
{
    bool bRet = false;
    do 
    {
        w *= (int)CC_CONTENT_SCALE_FACTOR();
        h *= (int)CC_CONTENT_SCALE_FACTOR();

        glGetIntegerv( GL_FRAMEBUFFER_BINDING, &m_nOldFBO);

		// 纹理必须是2的N次方
        unsigned int powW = ccNextPOT(w);
        unsigned int powH = ccNextPOT(h);

        void *data = malloc((int)(powW * powH * 4));
        CC_BREAK_IF(! data);

        memset(data, 0, (int)(powW * powH * 4));
        m_ePixelFormat = eFormat;

        m_pTexture = new FKCW_RenderEx_Texture2DMutable();
        CC_BREAK_IF(! m_pTexture);

        m_pTexture->initWithData(data, (CCTexture2DPixelFormat)m_ePixelFormat, powW, powH, CCSizeMake((float)w, (float)h));
        free( data );

        // 生成FBO
		glGenFramebuffers(1, &m_uFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

		// 连接纹理和FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getName(), 0);

        // 检查当前执行状态
        GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            CCAssert(0, "Render Texture : Could not attach texture to framebuffer");
            CC_SAFE_DELETE(m_pTexture);
            break;
        }

        m_pTexture->setAliasTexParameters();

        m_pSprite = CCSprite::createWithTexture(m_pTexture);

        m_pTexture->release();
        m_pSprite->setScaleY(-1);
        this->addChild(m_pSprite);

        ccBlendFunc tBlendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
        m_pSprite->setBlendFunc(tBlendFunc);

        glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
        bRet = true;
    } while (0);
    return bRet;  
}
//-------------------------------------------------------------------------
void FKCW_RenderEX_RenderTextureMutable::begin()
{
	// 保存当前矩阵
	glPushMatrix();
	//kmGLPushMatrix();

	const CCSize& texSize = m_pTexture->getContentSizeInPixels();

	// 根据旧项目和新项目计算调整比率
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	float widthRatio = size.width / texSize.width;
	float heightRatio = size.height / texSize.height;

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	glOrtho((float)-1.0 / widthRatio,  
		(float)1.0 / widthRatio, 
		(float)-1.0 / heightRatio, 
		(float)1.0 / heightRatio, 
		-1,
		1);
#else
	glOrthof((float)-1.0 / widthRatio,  
		(float)1.0 / widthRatio, 
		(float)-1.0 / heightRatio, 
		(float)1.0 / heightRatio, 
		-1,
		1);
	FKLOG("at Platform android GL lib have some Error");
#endif

	// 调整正交矩阵和视图矩阵
	glViewport(0, 0, (GLsizei)texSize.width, (GLsizei)texSize.height);

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);// 直接绘制到上面创建的帧缓冲区中

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	
}
//-------------------------------------------------------------------------
void FKCW_RenderEX_RenderTextureMutable::beginWithClear(float r, float g, float b, float a)
{
    this->begin();

	// 保存清理的颜色
	GLfloat	clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE,clearColor); 

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);     
}
//-------------------------------------------------------------------------
void FKCW_RenderEX_RenderTextureMutable::end(bool bIsTOCacheTexture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);

	// 保存原本的矩阵和视图
	glPopMatrix();
	//kmGLPopMatrix();
	
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCDirector::sharedDirector()->getOpenGLView()->setViewPortInPoints(0, 0, size.width, size.height);

#if CC_ENABLE_CACHE_TEXTURE_DATA
	if (bIsTOCacheTexture)
	{
		CC_SAFE_DELETE(m_pUITextureImage);

		// 获取渲染纹理数据
		const CCSize& s = m_pTexture->getContentSizeInPixels();
		int tx = (int)s.width;
		int ty = (int)s.height;
		m_pUITextureImage = new CCImage;
		if (true == getUIImageFromBuffer(m_pUITextureImage, 0, 0, tx, ty))
		{
			VolatileTexture::addDataTexture(m_pTexture, m_pUITextureImage->getData(), kTexture2DPixelFormat_RGBA8888, s);
		} 
		else
		{
			CCLOG("Cache rendertexture failed!");
		}
	}
#endif
    
#if CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    m_pTexture->updateData();
#else
    m_pTexture->updateData(m_uFBO, m_nOldFBO);
#endif
}
//-------------------------------------------------------------------------
void FKCW_RenderEX_RenderTextureMutable::clear(float r, float g, float b, float a)
{
	this->beginWithClear(r, g, b, a);
	this->end();
}
//-------------------------------------------------------------------------
bool FKCW_RenderEX_RenderTextureMutable::saveBuffer(const char *szFilePath, int x, int y, int nWidth, int nHeight)
{
	bool bRet = false;

	CCImage *pImage = new CCImage();
	if (pImage != NULL && getUIImageFromBuffer(pImage, x, y, nWidth, nHeight))
	{
		bRet = pImage->saveToFile(szFilePath);
	}

	CC_SAFE_DELETE(pImage);
	return bRet;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEX_RenderTextureMutable::saveBuffer(int format, const char *fileName, int x, int y, int nWidth, int nHeight)
{
	bool bRet = false;
	CCAssert(format == kCCImageFormatJPEG || format == kCCImageFormatPNG,
			 "the image can only be saved as JPG or PNG format");

	CCImage *pImage = new CCImage();
	if (pImage != NULL && getUIImageFromBuffer(pImage, x, y, nWidth, nHeight))
	{
		std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
		
		bRet = pImage->saveToFile(fullpath.c_str());
	}

	CC_SAFE_DELETE(pImage);

	return bRet;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEX_RenderTextureMutable::getUIImageFromBuffer(CCImage *pImage, int x, int y, int nWidth, int nHeight)
{
	if (NULL == pImage || NULL == m_pTexture)
	{
		return false;
	}

	const CCSize& s = m_pTexture->getContentSizeInPixels();
	int tx = (int)s.width;
	int ty = (int)s.height;

	if (x < 0 || x >= tx || y < 0 || y >= ty)
	{
		return false;
	}

	if (nWidth < 0 
		|| nHeight < 0
		|| (0 == nWidth && 0 != nHeight)
		|| (0 == nHeight && 0 != nWidth))
	{
		return false;
	}
	
	int nSavedBufferWidth = nWidth;
	int nSavedBufferHeight = nHeight;
	if (0 == nWidth)
	{
		nSavedBufferWidth = tx;
	}
	if (0 == nHeight)
	{
		nSavedBufferHeight = ty;
	}
	nSavedBufferWidth = x + nSavedBufferWidth > tx ? (tx - x): nSavedBufferWidth;
	nSavedBufferHeight = y + nSavedBufferHeight > ty ? (ty - y): nSavedBufferHeight;

	GLubyte *pBuffer = NULL;
	GLubyte *pTempData = NULL;
	bool bRet = false;

	do
	{
		CCAssert(m_ePixelFormat == kCCTexture2DPixelFormat_RGBA8888, "only RGBA8888 can be saved as image");

		CC_BREAK_IF(! (pBuffer = new GLubyte[nSavedBufferWidth * nSavedBufferHeight * 4]));

		int nReadBufferWidth = 0;
		int nReadBufferHeight = 0;
		int nMaxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nMaxTextureSize);

		nReadBufferWidth = ccNextPOT(tx);
		nReadBufferHeight = ccNextPOT(ty);

		CC_BREAK_IF(0 == nReadBufferWidth || 0 == nReadBufferHeight);
		CC_BREAK_IF(nReadBufferWidth > nMaxTextureSize || nReadBufferHeight > nMaxTextureSize);

		CC_BREAK_IF(! (pTempData = new GLubyte[nReadBufferWidth * nReadBufferHeight * 4]));

		this->begin();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0,0,nReadBufferWidth,nReadBufferHeight,GL_RGBA,GL_UNSIGNED_BYTE, pTempData);
		this->end(false);

		for (int i = 0; i < nSavedBufferHeight; ++i)
		{
			memcpy(&pBuffer[i * nSavedBufferWidth * 4], 
				&pTempData[(y + nSavedBufferHeight - i - 1) * nReadBufferWidth * 4 + x * 4], 
				nSavedBufferWidth * 4);
		}

		bRet = pImage->initWithImageData(pBuffer, nSavedBufferWidth * nSavedBufferHeight * 4, CCImage::kFmtRawData, nSavedBufferWidth, nSavedBufferHeight, 8);
	} while (0);

	CC_SAFE_DELETE_ARRAY(pBuffer);
	CC_SAFE_DELETE_ARRAY(pTempData);

	return bRet;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_Texture2DMutable* FKCW_RenderEX_RenderTextureMutable::getTexture() const 
{
	return m_pTexture;
}
//-------------------------------------------------------------------------