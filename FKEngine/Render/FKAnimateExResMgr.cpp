//-------------------------------------------------------------------------
#include "FKAnimateExResMgr.h"
#include "FKAnimateExRes.h"
//-------------------------------------------------------------------------
FKAnimateExResMgr::FKAnimateExResMgr()
{

}
//-------------------------------------------------------------------------
FKAnimateExResMgr::~FKAnimateExResMgr()
{

}
//-------------------------------------------------------------------------
// 创建动画资源
// 参数：szAnimationExResName 动画资源文件名
// 注：若该资源已经存在，则直接返回原资源指针
FKAnimateExRes* FKAnimateExResMgr::GetAnimateExRes( const char* szAnimationExResName )
{
	if( szAnimationExResName == NULL )
		return NULL;

	AnimationExResMap::iterator Ite = m_mapAnimationExRes.find( szAnimationExResName );
	if( Ite != m_mapAnimationExRes.end() )
	{
		Ite->second->retain();
		return Ite->second;
	}

	FKAnimateExRes* pAnimateExRes = new FKAnimateExRes;
	if( pAnimateExRes == nullptr )
		return NULL;
	if( !pAnimateExRes->LoadRes( szAnimationExResName ) )
		return NULL;

	m_mapAnimationExRes[szAnimationExResName] = pAnimateExRes;
	return pAnimateExRes;
}
//-------------------------------------------------------------------------
// 删除动画资源
void FKAnimateExResMgr::ReleaseAnimationExRes( FKAnimateExRes* pAniRes )
{
	if( pAniRes == NULL )
		return;

	if( pAniRes->isSingleReference() )
	{
		m_mapAnimationExRes.erase( pAniRes->GetResFileName() );
	}
}
//-------------------------------------------------------------------------
// 释放全部资源
void FKAnimateExResMgr::ReleaseAllResource()
{
	AnimationExResMap::iterator Ite = m_mapAnimationExRes.begin();
	for( ; Ite != m_mapAnimationExRes.end(); )
	{
		if( Ite->second == NULL )
			continue;

		CCAssert( Ite->second->isSingleReference(), "动画引用计数必须为1" );
		Ite->second->release();
	}
	m_mapAnimationExRes.clear();
}
//-------------------------------------------------------------------------
// 释放没有释放的资源
void FKAnimateExResMgr::ReleaseUnusedResource()
{
	AnimationExResMap::iterator Ite = m_mapAnimationExRes.begin();
	for( ; Ite != m_mapAnimationExRes.end(); )
	{
		if( Ite->second == NULL )
			continue;

		if( Ite->second->isSingleReference() )
		{
			Ite->second->release();
			m_mapAnimationExRes.erase( Ite++ );
		}
		else
		{
			Ite++;
		}
	}
}
//-------------------------------------------------------------------------