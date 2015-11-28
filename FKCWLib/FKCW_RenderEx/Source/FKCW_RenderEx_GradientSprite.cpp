//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_GradientSprite.h"
//-------------------------------------------------------------------------
#pragma warning( disable : 4244 )
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::_UpdateColor()
{
	CCSprite::updateColor();

	// 方向向量长度，如果是0，则什么也不做
	float h = ccpLength(m_tagAlongVector);
	if (h > 0) 
	{
		float c = sqrtf(2.0f);
		CCPoint u = ccp(m_tagAlongVector.x / h, m_tagAlongVector.y / h);

		// 压缩补差模式
		if (m_bIsCompressedInterpolation) 
		{
			float h2 = 1 / ( fabsf(u.x) + fabsf(u.y) );
			u = ccpMult(u, h2 * (float)c);
		}

		float opacityf = (float)_displayedOpacity / 255.0f;

		ccColor4F S = {
			_displayedColor.r / 255.0f,
			_displayedColor.g / 255.0f,
			_displayedColor.b / 255.0f,
			m_cStartOpacity * opacityf / 255.0f
		};

		ccColor4F E = {
			m_tagEndColor.r / 255.0f,
			m_tagEndColor.g / 255.0f,
			m_tagEndColor.b / 255.0f,
			m_cEndOpacity * opacityf / 255.0f
		};

		// bl
		m_sQuad.bl.colors.r = (E.r + (S.r - E.r) * ((c + u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.bl.colors.g = (E.g + (S.g - E.g) * ((c + u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.bl.colors.b = (E.b + (S.b - E.b) * ((c + u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.bl.colors.a = (E.a + (S.a - E.a) * ((c + u.x + u.y) / (2.0f * c))) * 255;

		// br
		m_sQuad.br.colors.r = (E.r + (S.r - E.r) * ((c - u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.br.colors.g = (E.g + (S.g - E.g) * ((c - u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.br.colors.b = (E.b + (S.b - E.b) * ((c - u.x + u.y) / (2.0f * c))) * 255;
		m_sQuad.br.colors.a = (E.a + (S.a - E.a) * ((c - u.x + u.y) / (2.0f * c))) * 255;

		// tl
		m_sQuad.tl.colors.r = (E.r + (S.r - E.r) * ((c + u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tl.colors.g = (E.g + (S.g - E.g) * ((c + u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tl.colors.b = (E.b + (S.b - E.b) * ((c + u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tl.colors.a = (E.a + (S.a - E.a) * ((c + u.x - u.y) / (2.0f * c))) * 255;

		// tr
		m_sQuad.tr.colors.r = (E.r + (S.r - E.r) * ((c - u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tr.colors.g = (E.g + (S.g - E.g) * ((c - u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tr.colors.b = (E.b + (S.b - E.b) * ((c - u.x - u.y) / (2.0f * c))) * 255;
		m_sQuad.tr.colors.a = (E.a + (S.a - E.a) * ((c - u.x - u.y) / (2.0f * c))) * 255;
	}

	// 如果使用batchNode渲染
	if (m_pobBatchNode) 
	{
		if (m_uAtlasIndex != CCSpriteIndexNotInitialized) 
		{
			m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
		} 
		else 
		{
			// 这里不需要递归了
			setDirty(true);
		}
	}
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite::FKCW_RenderEx_GradientSprite()
	: m_tagStartColor( ccWHITE )
	, m_tagEndColor( ccWHITE )
	, m_cEndOpacity(  0 )
	, m_cStartOpacity( 255 )
	, m_tagAlongVector( CCPointZero )
	, m_bIsCompressedInterpolation( true )
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite::~FKCW_RenderEx_GradientSprite()
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::create()
{
	FKCW_RenderEx_GradientSprite *pSprite = new FKCW_RenderEx_GradientSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::create(const char *pszFileName)
{
	FKCW_RenderEx_GradientSprite *pobSprite = new FKCW_RenderEx_GradientSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::create(const char *pszFileName, const CCRect& rect)
{
	FKCW_RenderEx_GradientSprite *pobSprite = new FKCW_RenderEx_GradientSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::createWithTexture(CCTexture2D *pTexture)
{
	FKCW_RenderEx_GradientSprite *pobSprite = new FKCW_RenderEx_GradientSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	FKCW_RenderEx_GradientSprite *pobSprite = new FKCW_RenderEx_GradientSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
	FKCW_RenderEx_GradientSprite *pobSprite = new FKCW_RenderEx_GradientSprite();
	if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_GradientSprite* FKCW_RenderEx_GradientSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
	return createWithSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setColor(const ccColor3B& color3)
{
	CCNodeRGBA::setColor(color3);
	_UpdateColor();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::updateDisplayedColor(const ccColor3B& parentColor)
{
	CCNodeRGBA::updateDisplayedColor(parentColor);
	_UpdateColor();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setOpacity(GLubyte opacity)
{
	CCNodeRGBA::setOpacity(opacity);
	_UpdateColor();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setOpacityModifyRGB(bool modify)
{
	if (m_bOpacityModifyRGB != modify) 
	{
		m_bOpacityModifyRGB = modify;
		_UpdateColor();
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::updateDisplayedOpacity(GLubyte parentOpacity)
{
	CCNodeRGBA::updateDisplayedOpacity(parentOpacity);
	_UpdateColor();
}
//-------------------------------------------------------------------------
// 设置渐变色
void FKCW_RenderEx_GradientSprite::setColor( const ccColor4B& start, const ccColor4B& end, const CCPoint& v )
{
	CCSprite::setColor(ccc3(start.r, start.g, start.b));
	m_tagEndColor.r = end.r;
	m_tagEndColor.g = end.g;
	m_tagEndColor.b = end.b;
	m_cEndOpacity = end.a;
	m_cStartOpacity = start.a;
	m_tagAlongVector = v;

	_UpdateColor();
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_GradientSprite::getCompressedInterpolation()
{
	return m_bIsCompressedInterpolation;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setCompressedInterpolation(bool p_bCompress)
{
	m_bIsCompressedInterpolation = p_bCompress;
	_UpdateColor();
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_RenderEx_GradientSprite::getStartColor() 
{
	return _realColor; // 注意这里
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_RenderEx_GradientSprite::getEndColor() 
{
	return m_tagEndColor;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setStartColor(const ccColor3B& color) 
{
	setColor(color);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setEndColor(const ccColor3B& color) 
{
	m_tagEndColor = color;
	_UpdateColor();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setStartOpacity(GLubyte o) 
{
	m_cStartOpacity = o;
	_UpdateColor();
}
//-------------------------------------------------------------------------
GLubyte FKCW_RenderEx_GradientSprite::getStartOpacity() 
{
	return m_cStartOpacity;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setEndOpacity(GLubyte o) 
{
	m_cEndOpacity = o;
	_UpdateColor();
}
//-------------------------------------------------------------------------
GLubyte FKCW_RenderEx_GradientSprite::getEndOpacity() 
{
	return m_cEndOpacity;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_GradientSprite::setVector(const CCPoint& var) 
{
	m_tagAlongVector = var;
	_UpdateColor();
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_RenderEx_GradientSprite::getVector() 
{
	return m_tagAlongVector;
}
//-------------------------------------------------------------------------