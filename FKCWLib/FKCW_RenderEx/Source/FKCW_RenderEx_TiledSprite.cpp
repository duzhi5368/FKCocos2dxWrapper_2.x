//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_TiledSprite.h"
//-------------------------------------------------------------------------
// 更新纹理贴图集
void FKCW_RenderEx_TiledSprite::__UpdateAtlas()
{
	// 数据纠正
	CCSize size = getContentSize();
	CCSize tileSize = m_pSprite->getContentSize();
	int hTiles = (int)ceilf(size.width / tileSize.width);
	int vTiles = (int)ceilf(size.height / tileSize.height);
	if(m_pAtlas->getCapacity() < static_cast<unsigned int>(hTiles * vTiles)) 
	{
		m_pAtlas->resizeCapacity(hTiles * vTiles);
	}

	// 清除矩形信息
	m_pAtlas->removeAllQuads();
	ccV3F_C4B_T2F_Quad originalQuad = m_pSprite->getQuad();

	// 填充纹理集
	ccV3F_C4B_T2F_Quad quad = originalQuad;
	float x = 0, y = 0;
	while(y < size.height)
	{
		if(y + tileSize.height > size.height)
		{
			float p = (size.height - y) / tileSize.height;
			quad.tl.vertices.y = quad.bl.vertices.y * (1 - p) + quad.tl.vertices.y * p;
			quad.tr.vertices.y = quad.br.vertices.y * (1 - p) + quad.tr.vertices.y * p;
			quad.tl.texCoords.v = quad.bl.texCoords.v * (1 - p) + quad.tl.texCoords.v * p;
			quad.tr.texCoords.v = quad.br.texCoords.v * (1 - p) + quad.tr.texCoords.v * p;
		}

		while(x < size.width) 
		{
			if(x + tileSize.width > size.width)
			{
				float p = (size.width - x) / tileSize.width;
				quad.br.vertices.x = quad.bl.vertices.x * (1 - p) + quad.br.vertices.x * p;
				quad.tr.vertices.x = quad.tl.vertices.x * (1 - p) + quad.tr.vertices.x * p;
				quad.br.texCoords.u = quad.bl.texCoords.u * (1 - p) + quad.br.texCoords.u * p;
				quad.tr.texCoords.u = quad.tl.texCoords.u * (1 - p) + quad.tr.texCoords.u * p;
			}

			// 添加一个纹理信息方格
			m_pAtlas->insertQuad(&quad, m_pAtlas->getTotalQuads());

			// 增加
			x += tileSize.width;
			quad.bl.vertices.x += tileSize.width;
			quad.br.vertices.x += tileSize.width;
			quad.tl.vertices.x += tileSize.width;
			quad.tr.vertices.x += tileSize.width;
		}

		// 移动
		x = 0;
		y += tileSize.height;

		// 重置信息方格，并增加y值
		quad = originalQuad;
		quad.bl.vertices.y += y;
		quad.br.vertices.y += y;
		quad.tl.vertices.y += y;
		quad.tr.vertices.y += y;
	}
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite::FKCW_RenderEx_TiledSprite(CCSprite* sprite)
	: m_pSprite( NULL )
	, m_bDirty( false )
	, m_pAtlas( NULL )
{
	CCAssert(sprite != NULL, "CCTiledSprite doesn't accept NULL sprite");

	// 保存精灵
	m_pSprite = sprite;
	CC_SAFE_RETAIN(m_pSprite);

	//保存纹理集
	m_pAtlas = CCTextureAtlas::createWithTexture(m_pSprite->getTexture(), kDefaultSpriteBatchCapacity);
	CC_SAFE_RETAIN(m_pAtlas);

	// shaderProgram
	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));

	// 设置容量大小
	setContentSize(m_pSprite->getContentSize());
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite::~FKCW_RenderEx_TiledSprite()
{
	CC_SAFE_RELEASE( m_pSprite );
	CC_SAFE_RELEASE( m_pAtlas );
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite* FKCW_RenderEx_TiledSprite::create(const char* fileImage)
{
	return FKCW_RenderEx_TiledSprite::createWithSprite(CCSprite::create(fileImage));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite* FKCW_RenderEx_TiledSprite::createWithTexture(CCTexture2D* tex)
{
	return FKCW_RenderEx_TiledSprite::createWithSprite(CCSprite::createWithTexture(tex));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite* FKCW_RenderEx_TiledSprite::createWithSpriteFrameName(const char* name)
{
	return createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite* FKCW_RenderEx_TiledSprite::createWithSpriteFrame(CCSpriteFrame* frame)
{
	return FKCW_RenderEx_TiledSprite::createWithSprite(CCSprite::createWithSpriteFrame(frame));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TiledSprite* FKCW_RenderEx_TiledSprite::createWithSprite(CCSprite* sprite)
{
	FKCW_RenderEx_TiledSprite* s = new FKCW_RenderEx_TiledSprite(sprite);
	if(s->initWithTexture(sprite->getTexture(), sprite->getTextureRect(), sprite->isTextureRectRotated())) 
	{
		return (FKCW_RenderEx_TiledSprite*)s->autorelease();
	}
	CC_SAFE_RELEASE(s);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_TiledSprite::initWithTexture(CCTexture2D* pTexture, const CCRect& rect, bool rotated)
{
	if(!CCSprite::initWithTexture(pTexture, rect, rotated)) 
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::setOpacity(GLubyte opacity)
{
	CCSprite::setOpacity(opacity);
	m_bDirty = true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::setDisplayFrame(CCSpriteFrame *pNewFrame)
{
	CCSize oldSize = getContentSize();
	CCSprite::setDisplayFrame(pNewFrame);
	m_pSprite->setDisplayFrame(pNewFrame);
	if(m_pSprite->getTexture() != m_pAtlas->getTexture()) 
	{
		m_pAtlas->setTexture(m_pSprite->getTexture());
	}
	setContentSize(oldSize);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::draw()
{
	// 创建纹理贴图集
	if(m_bDirty) 
	{
		__UpdateAtlas();
		m_bDirty = false;
	}

	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "FKCW_RenderEx_TiledSprite - draw");
	CC_NODE_DRAW_SETUP();

	// 修改混合方式
	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	// 实际绘制
	if(m_pAtlas)
		m_pAtlas->drawQuads();

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "FKCW_RenderEx_TiledSprite - draw");
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::setContentSize(const CCSize& contentSize)
{
	CCSprite::setContentSize(contentSize);
	m_bDirty = true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::setHeight(float h)
{
	CCSize size = getContentSize();
	size.height = h;
	setContentSize(size);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TiledSprite::setWidth(float w)
{
	CCSize size = getContentSize();
	size.width = w;
	setContentSize(size);
}
//-------------------------------------------------------------------------