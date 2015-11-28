//-------------------------------------------------------------------------
#include "FKRenderableMgr.h"
#include "FKSpriteObj.h"
#include "FKFrameSpriteObj.h"
#include "FKDBSpriteObj.h"
//-------------------------------------------------------------------------
FKRenderableMgr::FKRenderableMgr()
	: m_unCurID( 0 )
{
	
}
//-------------------------------------------------------------------------
FKRenderableMgr::~FKRenderableMgr()
{

}
//-------------------------------------------------------------------------
// 获取渲染对象
IRenderable* FKRenderableMgr::GetRenderable( RENDER_OBJ_ID id )
{
	RenderableMap::iterator Ite = m_mapRenderables.find( id );
	if( Ite != m_mapRenderables.end() )
		return Ite->second;
	return NULL;
}
//-------------------------------------------------------------------------
// 删除渲染对象
void FKRenderableMgr::RemoveRenderable( RENDER_OBJ_ID id )
{
	RenderableMap::iterator Ite = m_mapRenderables.find( id );
	if( Ite != m_mapRenderables.end() )
	{
		if( Ite->second )
		{
			Ite->second->release();
		}
	}
}
//-------------------------------------------------------------------------
// 创建静态精灵对象
ISprite* FKRenderableMgr::CreateSprite( const char* szPicFileName, 
	const CCRect& rect, CCPoint ptOffset )
{
	if( szPicFileName == NULL )
		return NULL;
	if( ++m_unCurID == 0 )
		++m_unCurID;

	IRenderable* p = GetRenderable( m_unCurID );
	while ( p != NULL )
	{
		if( ++m_unCurID == 0 )
			++m_unCurID;
		p = GetRenderable( m_unCurID );
	}

	FKSpriteObj* pSpriteObj = NULL;
	pSpriteObj = new FKSpriteObj( m_unCurID );
	if( pSpriteObj == NULL )
	{
		return NULL;
	}
	if( !pSpriteObj->Create( szPicFileName, rect, ptOffset ) )
	{
		CC_SAFE_DELETE( pSpriteObj );
		return NULL;
	}
	m_mapRenderables[m_unCurID] = pSpriteObj;
	return pSpriteObj;
}
//-------------------------------------------------------------------------
// 创建帧精灵对象
IFrameSprite* FKRenderableMgr::CreateFrameSprite( const char* szFrameFileName, CCPoint ptOffset )
{
	if( szFrameFileName == NULL )
		return NULL;
	if( ++m_unCurID == 0 )
		++m_unCurID;

	IRenderable* p = GetRenderable( m_unCurID );
	while ( p != NULL )
	{
		if( ++m_unCurID == 0 )
			++m_unCurID;
		p = GetRenderable( m_unCurID );
	}

	FKFrameSpriteObj* pSpriteObj = NULL;
	pSpriteObj = new FKFrameSpriteObj( m_unCurID );
	if( pSpriteObj == NULL )
	{
		return NULL;
	}
	if( !pSpriteObj->Create( szFrameFileName, ptOffset ) )
	{
		CC_SAFE_DELETE( pSpriteObj );
		return NULL;
	}
	m_mapRenderables[m_unCurID] = pSpriteObj;
	return pSpriteObj;
}
//-------------------------------------------------------------------------
// 创建DB动画对象
IDragonBonesSprite* FKRenderableMgr::CreateDBSprite( const char* szDBFileName, CCPoint ptOffset )
{
	if( szDBFileName == NULL )
		return NULL;
	if( ++m_unCurID == 0 )
		++m_unCurID;

	IRenderable* p = GetRenderable( m_unCurID );
	while ( p != NULL )
	{
		if( ++m_unCurID == 0 )
			++m_unCurID;
		p = GetRenderable( m_unCurID );
	}

	FKDBSpriteObj* pSpriteObj = NULL;
	pSpriteObj = new FKDBSpriteObj( m_unCurID );
	if( pSpriteObj == NULL )
	{
		return NULL;
	}
	if( !pSpriteObj->Create( szDBFileName, ptOffset ) )
	{
		CC_SAFE_DELETE( pSpriteObj );
		return NULL;
	}
	m_mapRenderables[m_unCurID] = pSpriteObj;
	return pSpriteObj;
}
//-------------------------------------------------------------------------
// 创建CocosBuilder骨骼动画对象
IArmatureSprite* FKRenderableMgr::CreateArmaSprite( const char* szArmaFileName, CCPoint ptOffset )
{
	// todo:
	return NULL;
}
//-------------------------------------------------------------------------
// 创建特效对象
IEffect* FKRenderableMgr::CreateEffect( const char* szEffectFileName, CCPoint ptOffset )
{
	// todo:
	return NULL;
}
//-------------------------------------------------------------------------