//-------------------------------------------------------------------------
#include "FKMapChunk.h"
#include "FKMapStruct.h"
//-------------------------------------------------------------------------
FKMapChunk::FKMapChunk( RENDER_MAP_CHUNK_ID id )
	: m_pOwnerLayer( NULL )
	, m_bIsUseDebugRender( false )
	, m_bIsVisible( false )
{
	m_pMapChunkInfo = new SMapChunkInfo();
	if( m_pMapChunkInfo )
	{
		m_pMapChunkInfo->unChunkID = id;
	}
}
//-------------------------------------------------------------------------
FKMapChunk::~FKMapChunk()
{
	Release();
	CC_SAFE_DELETE( m_pMapChunkInfo );
}
//-------------------------------------------------------------------------
// 释放全部信息
void FKMapChunk::Release()
{

}
//-------------------------------------------------------------------------
// 帧更新
void FKMapChunk::Update( float dt )
{
	if( IsVisiable() )
	{
		_SubUpdate( dt );
	}
}
//-------------------------------------------------------------------------
// 获取ChunkID
unsigned int FKMapChunk::GetChunkID() const
{
	if( m_pMapChunkInfo )
		return m_pMapChunkInfo->GetChunkID();
	return 0;
}
//-------------------------------------------------------------------------
// 设置ChunkID
void FKMapChunk::SetChunkID( unsigned int unChunkID )
{
	if( m_pMapChunkInfo )
		m_pMapChunkInfo->unChunkID = unChunkID;
}
//-------------------------------------------------------------------------
// 获取当前区域格在大地图中所在位置
void FKMapChunk::GetGlobalPosition( CCPoint& tagPos )
{
	if( m_pMapChunkInfo )
	{
		tagPos = m_pMapChunkInfo->tagPos;
		return;
	}
	tagPos = getPosition();
}
//-------------------------------------------------------------------------
// 设置当前区域格所在大地图中的位置
void FKMapChunk::SetGlobalPosition( const CCPoint& tagPos )
{
	if( m_pMapChunkInfo )
		m_pMapChunkInfo->tagPos = tagPos;
	setPosition( tagPos );
}
//-------------------------------------------------------------------------
// 获取当前区域大小
void FKMapChunk::GetSize( CCSize& tagSize )
{
	tagSize = CCSizeZero;
	if( m_pMapChunkInfo )
	{
		m_pMapChunkInfo->GetSize( tagSize );
	}
}
//-------------------------------------------------------------------------
// 获取当前区域范围
void FKMapChunk::GetRect( CCRect& rect )
{
	rect = CCRectZero;
	if( m_pMapChunkInfo )
	{
		m_pMapChunkInfo->GetRect( rect );
	}
}
//-------------------------------------------------------------------------
// 获取地图格类型( 返回 ENUM_MapChunkType )
int FKMapChunk::GetType()
{
	if( m_pMapChunkInfo )
		return m_pMapChunkInfo->GetType();
	return eMCT_Max;
}
//-------------------------------------------------------------------------
// 开启/关闭DEBUG渲染
void FKMapChunk::SetDebugRender( bool bIsUseDebugRender )
{
	m_bIsUseDebugRender = bIsUseDebugRender;
}
//-------------------------------------------------------------------------
// 获取区域所在层
IMapLayer* FKMapChunk::GetLayer()
{
	return m_pOwnerLayer;
}
//-------------------------------------------------------------------------
// 设置区域所在层
void FKMapChunk::SetLayer( IMapLayer* pLayer )
{
	m_pOwnerLayer = pLayer;
}
//-------------------------------------------------------------------------
// 是否可见
bool FKMapChunk::IsVisiable()
{
	return m_bIsVisible;
}
//-------------------------------------------------------------------------
// 设置是否可见
void FKMapChunk::SetIsVisiable( bool bIsVisiable )
{
	m_bIsVisible = bIsVisiable;
}
//-------------------------------------------------------------------------
// 检查一个点是否在本区域内
bool FKMapChunk::IsPosInChunk( const CCPoint& pos )
{
	if( m_pMapChunkInfo == NULL )
	{
		return false;
	}
	CCRect rect = CCRectZero;
	m_pMapChunkInfo->GetRect( rect );
	return rect.containsPoint( pos );
}
//-------------------------------------------------------------------------
// 添加子格
bool FKMapChunk::AddGrid( IMapGridInfo* pGridInfo )
{
	return _SubAddGrid( pGridInfo );
}
//-------------------------------------------------------------------------
// 移除子格
void FKMapChunk::DelGrid( CCPoint& tagPos )
{
	return _SubDelGrid( tagPos );
}
//-------------------------------------------------------------------------
// 添加渲染对象
bool FKMapChunk::AddRenderObj( RENDER_OBJ_ID id, const string& strTerrainObjName, CCRect& rc,
							  CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor )
{
	return _SubAddRenderObj( id, strTerrainObjName, rc, tagOffset, tagScale, tagColor );
}
//-------------------------------------------------------------------------
// 添加渲染对象
bool FKMapChunk::AddRenderObj( RENDER_OBJ_ID id, IRenderObj* pRenderObj )
{
	return _SubAddRenderObj( id, pRenderObj );
}
//-------------------------------------------------------------------------
// 获取渲染对象
IRenderObj* FKMapChunk::GetRenderObj( RENDER_OBJ_ID id )
{
	return _SubGetRenderObj( id );
}
//-------------------------------------------------------------------------
// 移除渲染对象
void FKMapChunk::DelRenderObj( RENDER_OBJ_ID id )
{
	return _SubDelRenderObj( id );
}
//-------------------------------------------------------------------------
// 转换世界坐标到chunk中的坐标
void FKMapChunk::ConvertGlobalPos2ChunkPos( CCPoint& GlobalPos, CCPoint& ChunkPos )
{
	CCSize tagChunkSize = CCSizeZero;
	GetSize( tagChunkSize );
	CCPoint tagChunkCenterPos = CCPointZero;
	GetGlobalPosition( tagChunkCenterPos );
	ChunkPos = GlobalPos + ccp( tagChunkSize.width / 2, tagChunkSize.height / 2 ) - tagChunkCenterPos;
}
//-------------------------------------------------------------------------
// 转换chunk中的坐标到世界坐标
void FKMapChunk::ConvertChunkPos2GlobalPos( CCPoint& ChunkPos, CCPoint& GlobalPos )
{
	CCSize tagChunkSize = CCSizeZero;
	GetSize( tagChunkSize );
	CCPoint tagChunkCenterPos = CCPointZero;
	GetGlobalPosition( tagChunkCenterPos );
	GlobalPos = ChunkPos + tagChunkCenterPos - ccp( tagChunkSize.width / 2, tagChunkSize.height / 2 );
}
//-------------------------------------------------------------------------