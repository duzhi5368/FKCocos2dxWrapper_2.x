//-------------------------------------------------------------------------
#include "FKMapMgr.h"
#include "FKMapObj.h"
//-------------------------------------------------------------------------
FKBannerReelMapMgr::FKBannerReelMapMgr()
	: m_pCurMapObj( NULL )
	, m_pMapRootNode( NULL )
	, m_unMapID( 0 )
{
}
//-------------------------------------------------------------------------
FKBannerReelMapMgr::~FKBannerReelMapMgr()
{

}
//-------------------------------------------------------------------------
// 设置根渲染节点，所有场景全部挂接在本节点上
void FKBannerReelMapMgr::SetRoot( CCNode* pMapRootNode )
{
	m_pMapRootNode = pMapRootNode;
}
//-------------------------------------------------------------------------
// 获取当前运行中的地图场景
IMapObj* FKBannerReelMapMgr::GetCurrentMap()
{
	return m_pCurMapObj;
}
//-------------------------------------------------------------------------
// 更新进入新场景
// 参数：szFKMFileName 地图数据文件
RENDER_MAP_ID FKBannerReelMapMgr::EnterMap( const char* szFKMFileName )
{
	if( m_pMapRootNode == NULL )
		return 0;
	CCDirector *pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
		return 0;
	if( szFKMFileName == NULL )
		return 0;

	FKMapObj* pNewMap = NULL;
	// 生成新ID
	if( ++m_unMapID == 0 )
		++m_unMapID;

	pNewMap = new FKMapObj( m_unMapID );
	if( pNewMap == NULL )
		return 0 ;

	if( !pNewMap->InitFromFile(szFKMFileName) )
	{
		CC_SAFE_DELETE( pNewMap );
		return 0;
	}

	// 移除当前场景
	if( m_pCurMapObj != NULL )
	{
		m_pMapRootNode->removeChild( (FKMapObj*)m_pCurMapObj, true );
		m_pCurMapObj = NULL;
	}

	pNewMap->autorelease();
	m_pMapRootNode->addChild( pNewMap );
	m_pCurMapObj = pNewMap;
	
	return m_pCurMapObj->GetID();
}
//-------------------------------------------------------------------------
// 是否允许Update
void FKBannerReelMapMgr::SetEnableUpdate( bool bIsEnable )
{
	if( m_pCurMapObj )
	{
		m_pCurMapObj->SetEnableUpdate( bIsEnable );
	}
}
//-------------------------------------------------------------------------