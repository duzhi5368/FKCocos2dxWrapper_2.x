//-------------------------------------------------------------------------
#include "FKImageChunk.h"
#include "FKRenderObj.h"
//-------------------------------------------------------------------------
FKImageChunk::FKImageChunk( RENDER_MAP_CHUNK_ID id )
	: FKMapChunk( id )
	, m_bIsNeedUpdateTerrainObj( false )
{
	m_mapTerrainObjs.clear();
	m_mapRenderObj2TerrainObj.clear();
	m_setTerrainRenderObjs.clear();
}
//-------------------------------------------------------------------------
FKImageChunk::~FKImageChunk()
{
	IRenderSystem* pSystem = GetRenderSystem();
	TerrainObjsMap::iterator Ite = m_mapTerrainObjs.begin();
	for( ; Ite != m_mapTerrainObjs.end(); ++Ite )
	{
		if( Ite->second.m_pRenderObj )
		{
			removeChild( ((FKRenderObj*)Ite->second.m_pRenderObj) );
			if( pSystem )
				pSystem->RemoveRenderObj( Ite->second.m_pRenderObj );
			Ite->second.m_pRenderObj = NULL;
		}
	}
	m_mapTerrainObjs.clear();
	m_mapRenderObj2TerrainObj.clear();
	m_setTerrainRenderObjs.clear();
}
//-------------------------------------------------------------------------
void FKImageChunk::draw()
{
	// 暂时不做，之后要绘制边缘
}
//-------------------------------------------------------------------------
// 帧更新
void FKImageChunk::_SubUpdate( float dt )
{
	TerrainRenderObjsSet::iterator Ite = m_setTerrainRenderObjs.begin();
	for( ; Ite != m_setTerrainRenderObjs.end(); ++Ite )
	{
		TerrainObjsMap::iterator IteTerrainObj = m_mapTerrainObjs.find( *Ite );
		if( IteTerrainObj == m_mapTerrainObjs.end() )
			continue;

		FKRenderObj* pRenderObj = (FKRenderObj*)IteTerrainObj->second.m_pRenderObj;
		if( pRenderObj )
			pRenderObj->update( dt );
	}

	if( m_bIsNeedUpdateTerrainObj )
	{
		__UpdateVisibleObject();
		m_bIsNeedUpdateTerrainObj = false;
	}
}
//-------------------------------------------------------------------------
// 添加渲染对象
bool FKImageChunk::_SubAddRenderObj( RENDER_OBJ_MAP_ID id, const string& strTerrainObjName, CCRect& rc,
									CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor )
{
	STerrainObj tagObj;
	tagObj.m_tagOffset	= tagOffset;
	tagObj.m_tagScale	= tagScale;
	tagObj.m_tagRc		= rc;
	tagObj.m_tagColor	= tagColor;
	tagObj.m_strFileName= strTerrainObjName;
	tagObj.m_unChunkID	= GetChunkID();
	if( m_pOwnerLayer )
		tagObj.m_unLayerID	= m_pOwnerLayer->GetLayerID();
	tagObj.m_unID		= id;
	tagObj.m_unTime		= FKCW_Base_Utils::GetTimeTickCount();

	m_mapTerrainObjs[id] = tagObj;

	// 通知更新
	m_bIsNeedUpdateTerrainObj = true;
	return true;
}
//-------------------------------------------------------------------------
// 添加渲染对象
bool FKImageChunk::_SubAddRenderObj( RENDER_OBJ_MAP_ID id,IRenderObj* pRenderObj )
{
	if( pRenderObj == NULL )
		return false;
	if( pRenderObj->IsInScene() )
		return false;

	STerrainObj tagObj;
	tagObj.m_strFileName	= pRenderObj->GetFileName();
	tagObj.m_unTime			= FKCW_Base_Utils::GetTimeTickCount();
	tagObj.m_unChunkID		= GetChunkID();
	if( m_pOwnerLayer )
		tagObj.m_unLayerID	= m_pOwnerLayer->GetLayerID();
	tagObj.m_pRenderObj		= pRenderObj;
	tagObj.m_unID			= id;


	m_mapTerrainObjs[id] = tagObj;

	m_mapRenderObj2TerrainObj[pRenderObj->GetID()] = id;
	m_setTerrainRenderObjs.insert( id );

	// 通知更新
	m_bIsNeedUpdateTerrainObj = true;
	return true;
}
//-------------------------------------------------------------------------
// 移除渲染对象
void FKImageChunk::_SubDelRenderObj( RENDER_OBJ_MAP_ID id )
{
	IRenderSystem* pSystem = GetRenderSystem();
	if( pSystem == NULL )
		return;

	TerrainObjsMap::iterator Ite = m_mapTerrainObjs.find( id );
	if( Ite != m_mapTerrainObjs.end() )
	{
		if( Ite->second.m_pRenderObj )
		{
			removeChild( (FKRenderObj*)Ite->second.m_pRenderObj );
			if( pSystem )
				pSystem->RemoveRenderObj( Ite->second.m_pRenderObj );
			Ite->second.m_pRenderObj = NULL;
		}

		m_mapTerrainObjs.erase( Ite );
	}
	m_setTerrainRenderObjs.erase( id );
}
//-------------------------------------------------------------------------
// 获取渲染对象
IRenderObj* FKImageChunk::_SubGetRenderObj( RENDER_OBJ_MAP_ID id )
{
	TerrainObjsMap::iterator Ite = m_mapTerrainObjs.find( id );
	if( Ite != m_mapTerrainObjs.end() )
	{
		if( Ite->second.m_pRenderObj )
		{
			return Ite->second.m_pRenderObj;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
// 更新可视对象集
void FKImageChunk::__UpdateVisibleObject()
{
	IRenderSystem* pRenderSystem = GetRenderSystem();
	if( pRenderSystem == NULL )
		return;

	TerrainRenderObjsSet::iterator Ite = m_setTerrainRenderObjs.begin();
	for( ; Ite != m_setTerrainRenderObjs.end(); ++Ite )
	{
		TerrainObjsMap::iterator IteTerrainObj = m_mapTerrainObjs.find( *Ite );
		if( IteTerrainObj == m_mapTerrainObjs.end() )
			continue;

		FKRenderObj* pRenderObj = (FKRenderObj*)IteTerrainObj->second.m_pRenderObj;
		// 创建
		if( pRenderObj == NULL )
		{
			IteTerrainObj->second.m_pRenderObj = pRenderSystem->CreateRenderObj( 
				IteTerrainObj->second.m_strFileName.c_str() );
			pRenderObj = (FKRenderObj*)IteTerrainObj->second.m_pRenderObj;

			// 加入记录
			m_mapRenderObj2TerrainObj[pRenderObj->GetID()] = IteTerrainObj->second.m_unID;
			m_setTerrainRenderObjs.insert( IteTerrainObj->second.m_unID );
		}
		
		// 加入场景
		if( !pRenderObj->IsInScene() )
		{
			addChild( pRenderObj );
			// 调整渲染属性
			pRenderObj->SetInScene( true );
			pRenderObj->SetColor( ccc3(IteTerrainObj->second.m_tagColor.r, 
				IteTerrainObj->second.m_tagColor.g, IteTerrainObj->second.m_tagColor.b ) );
			pRenderObj->SetOpacity( IteTerrainObj->second.m_tagColor.a );
			pRenderObj->SetPos( IteTerrainObj->second.m_tagOffset );
			pRenderObj->SetScale( IteTerrainObj->second.m_tagScale );
		}
	}
}
//-------------------------------------------------------------------------