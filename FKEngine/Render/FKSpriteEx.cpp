//-------------------------------------------------------------------------
#include "FKSpriteEx.h"
//-------------------------------------------------------------------------
FKSpriteEx::FKSpriteEx()
	: m_bIsVailed( false )
{
	m_tagRect = CCRectZero;
}
//-------------------------------------------------------------------------
FKSpriteEx::~FKSpriteEx()
{

}
//-------------------------------------------------------------------------
FKSpriteEx* FKSpriteEx::Create()
{
	FKSpriteEx* p = new FKSpriteEx;
	if( p && p->init() )
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE( p );
	return NULL;
}
//-------------------------------------------------------------------------
FKSpriteEx* FKSpriteEx::Create(const char *pszFileName)
{
	FKSpriteEx* p = new FKSpriteEx;
	if( p && p->initWithFile( pszFileName ) )
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE( p );
	return NULL;
}
//-------------------------------------------------------------------------
FKSpriteEx* FKSpriteEx::Create(const char *pszFileName, const CCRect& rect)
{
	FKSpriteEx* p = new FKSpriteEx;
	if( p && p->initWithFile( pszFileName, rect ) )
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE( p );
	return NULL;
}
//-------------------------------------------------------------------------
void FKSpriteEx::draw()
{
	if( !m_bIsVailed )
		return;

	// 这里做很多渲染的shader的事

	CCSprite::draw();
}
//-------------------------------------------------------------------------
bool FKSpriteEx::init()
{
	if( CCSprite::init() )
	{
		m_bIsVailed = true;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
bool FKSpriteEx::initWithFile(const char *pszFilename)
{
	if( pszFilename == NULL )
		return false;

	retain();
	CCTextureCache::sharedTextureCache()->addImageAsync( pszFilename,this, 
		callfuncO_selector(FKSpriteEx::_OnImageLoadOver) );
	m_tagRect = CCRectZero;

	return true;
}
//-------------------------------------------------------------------------
bool FKSpriteEx::initWithFile(const char *pszFilename, const CCRect& rect)
{
	if( pszFilename == NULL )
		return false;

	retain();
	CCTextureCache::sharedTextureCache()->addImageAsync( pszFilename,this, 
		callfuncO_selector(FKSpriteEx::_OnImageLoadOver) );
	m_tagRect = rect;

	return true;
}
//-------------------------------------------------------------------------
void FKSpriteEx::_OnImageLoadOver( CCObject* Sender )
{
	CCTexture2D* pTex = (CCTexture2D*)Sender;
	if( pTex )
	{
		if( m_tagRect.equals( CCRectZero ) )
		{
			m_tagRect.size = pTex->getContentSize();
		}
		initWithTexture( pTex, m_tagRect );
	}

	release();
	m_bIsVailed = true;
}
//-------------------------------------------------------------------------