//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBAtlasNode.h"
//-------------------------------------------------------------------------
CCocos2dxDBAtlasNode::CCocos2dxDBAtlasNode()
	: m_pTextureAtlas( NULL )
	, m_bIsOpacityModifyRGB( false )
	, m_bIsIgnoreContentScaleFactor( false )
	, m_nUniformColor( 0 )
{

}
//-------------------------------------------------------------------------
CCocos2dxDBAtlasNode::~CCocos2dxDBAtlasNode()
{
	CC_SAFE_RELEASE( m_pTextureAtlas );
}
//-------------------------------------------------------------------------
CCocos2dxDBAtlasNode* CCocos2dxDBAtlasNode::Create( CCTextureAtlas* p_pTextureAtlas, unsigned int p_unQuadIndex,
		const CCRect& p_tagSourceRect )
{
	CCocos2dxDBAtlasNode* pRet = new CCocos2dxDBAtlasNode();
	if( pRet->InitWithTextureAtlas( p_pTextureAtlas, p_unQuadIndex, p_tagSourceRect ) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE( pRet );
	return NULL;
}
//-------------------------------------------------------------------------
bool CCocos2dxDBAtlasNode::InitWithTextureAtlas( CCTextureAtlas* p_pTextureAtlas, unsigned int p_unQuadIndex, 
		const CCRect& p_tagSourceRect )
{
	m_unQuadIndex			= p_unQuadIndex;
	m_tagColorUnmodified	= ccWHITE;
	m_bIsOpacityModifyRGB	= true;

	m_tagBlendFunc.src		= CC_BLEND_SRC;
	m_tagBlendFunc.dst		= CC_BLEND_DST;

	m_pTextureAtlas			= p_pTextureAtlas;
	if( !m_pTextureAtlas )
	{
		CCLOG("Init CCocos2dxDBAtlasNode failed. Empty CCTextureAtlas.");
		return false;
	}
	m_pTextureAtlas->retain();

	__UpdateBlendFunc();
	__UpdateOpacityModifyRGB();

	setShaderProgram( CCShaderCache::sharedShaderCache()->programForKey( kCCShader_PositionTexture_uColor) );
	m_nUniformColor = glGetUniformLocation( getShaderProgram()->getProgram(), "u_color" );

	ccV3F_C4B_T2F_Quad& tagQuad = m_pTextureAtlas->getQuads()[m_unQuadIndex];
	tagQuad.bl.vertices.x		= p_tagSourceRect.getMinX();
	tagQuad.bl.vertices.y		= p_tagSourceRect.getMinY();
	tagQuad.bl.vertices.z		= 0;
	tagQuad.br.vertices.x		= p_tagSourceRect.getMaxX();
	tagQuad.br.vertices.y		= p_tagSourceRect.getMinY();
	tagQuad.br.vertices.z		= 0;
	tagQuad.tl.vertices.x		= p_tagSourceRect.getMinX();
	tagQuad.tl.vertices.y		= p_tagSourceRect.getMaxY();
	tagQuad.tl.vertices.z		= 0;
	tagQuad.tr.vertices.x		= p_tagSourceRect.getMaxX();
	tagQuad.tr.vertices.y		= p_tagSourceRect.getMaxY();
	tagQuad.tr.vertices.z		= 0;

#if USE_BATCH_NODE == 1
	return initWithTexture( m_pTextureAtlas->getTexture(), p_tagSourceRect );
#else
	return true;
#endif
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::draw()
{
	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc( m_tagBlendFunc.src, m_tagBlendFunc.dst );

	GLfloat fColor[4] = {
		_displayedColor.r	/ 255.0f,
		_displayedColor.g	/ 255.0f,
		_displayedColor.b	/ 255.0f,
		_displayedOpacity	/ 255.0f
	};

	getShaderProgram()->setUniformLocationWith4fv( m_nUniformColor, fColor, 1 );

	m_pTextureAtlas->drawNumberOfQuads( 1, m_unQuadIndex );
	m_pTextureAtlas->getQuads();
}
//-------------------------------------------------------------------------
CCTexture2D* CCocos2dxDBAtlasNode::getTexture()
{
	return m_pTextureAtlas->getTexture();
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setTexture(CCTexture2D* p_pTexture)
{
#if USE_BATCH_NODE == 1
	CCSprite::setTexture( p_pTexture );
#endif
	m_pTextureAtlas->setTexture( p_pTexture );
	__UpdateBlendFunc();
	__UpdateOpacityModifyRGB();
}
//-------------------------------------------------------------------------
bool CCocos2dxDBAtlasNode::isOpacityModifyRGB()
{
	return m_bIsOpacityModifyRGB;
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setOpacityModifyRGB( bool p_bIsOpacityModifyRGB )
{
	ccColor3B tagOldColor = getColor();
	m_bIsOpacityModifyRGB = p_bIsOpacityModifyRGB;
	setColor( tagOldColor );
}
//-------------------------------------------------------------------------
const ccColor3B& CCocos2dxDBAtlasNode::getColor()
{
	if( m_bIsOpacityModifyRGB )
	{
		return m_tagColorUnmodified;
	}
	return CCNodeRGBA::getColor();
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setColor( const ccColor3B& p_Color )
{
	ccColor3B tmpColor = p_Color;
	m_tagColorUnmodified = p_Color;

	if( m_bIsOpacityModifyRGB )
	{
		tmpColor.r = tmpColor.r * _displayedOpacity / 255;
		tmpColor.g = tmpColor.g * _displayedOpacity / 255;
		tmpColor.b = tmpColor.b * _displayedOpacity / 255;
	}
#if USE_BATCH_NODE == 1
	CCNodeRGBA::setColor( _displayedColor );
#else
	CCNodeRGBA::setColor( tmpColor );
#endif
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setOpacity( GLubyte p_ucOpacity )
{
	CCNodeRGBA::setOpacity( p_ucOpacity );

	if( m_bIsOpacityModifyRGB )
		setColor( m_tagColorUnmodified );
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::__UpdateBlendFunc()
{
	if( m_pTextureAtlas->getTexture()->m_bHasPremultipliedAlpha )
		return;

	m_tagBlendFunc.src = GL_SRC_ALPHA;
	m_tagBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::__UpdateOpacityModifyRGB()
{
	m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture()->m_bHasPremultipliedAlpha;
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::__SetIgnoreContentScaleFactor( bool p_bIgnoreContentScaleFactor )
{
	m_bIsIgnoreContentScaleFactor = p_bIgnoreContentScaleFactor;
}
//-------------------------------------------------------------------------
ccBlendFunc CCocos2dxDBAtlasNode::getBlendFunc()
{
	return m_tagBlendFunc;
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setBlendFunc(ccBlendFunc var)
{
	m_tagBlendFunc = var;
}
//-------------------------------------------------------------------------
CCTextureAtlas* CCocos2dxDBAtlasNode::getTextureAtlas()
{
	return m_pTextureAtlas;
}
//-------------------------------------------------------------------------
void CCocos2dxDBAtlasNode::setTextureAtlas(CCTextureAtlas* var )
{
	CC_SAFE_RETAIN( var );
	CC_SAFE_RELEASE( m_pTextureAtlas );
	m_pTextureAtlas = var;
}
//-------------------------------------------------------------------------