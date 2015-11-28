//-------------------------------------------------------------------------
#include "../Include/BaseFactory.h"
#include "../Include/SkeletonData.h"
#include "../Interface/ITextureAtlas.h"
#include "../Include/Armature.h"
#include "../Include/Animation.h"
#include "../Include/Bone.h"
#include "../Include/Slot.h"
#include "../Include/ConstString.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
CBaseFactory::CBaseFactory()
{

}
//-------------------------------------------------------------------------
bool CBaseFactory::IsExistSkeletonDataInDic( const string& p_szName )
{
	if( m_mapDataDic.find( p_szName ) != m_mapDataDic.end() )
		return true;
	return false;
}
//-------------------------------------------------------------------------
bool CBaseFactory::IsExistTextureDataInDic( const string& p_szName )
{
	if( m_mapTextureAtlasDic.find( p_szName )!= m_mapTextureAtlasDic.end() )
		return true;
	return false;
}
//-------------------------------------------------------------------------
CSkeletonData* CBaseFactory::GetSkeletonData( const string& p_szName )
{
	map<string, CSkeletonData*>::iterator Ite = m_mapDataDic.find( p_szName );
	if( Ite == m_mapDataDic.end() )
		return NULL;
	return Ite->second;
}
//-------------------------------------------------------------------------
void CBaseFactory::AddSkeletonData( CSkeletonData* p_szData, const string& p_szName )
{
	if( p_szData == NULL )
	{
		//throw std::invalid_argument("Skeleton Data is null!");
		//android 平台不支持c++11特性
		printf("Skeleton Data is null!");
	}

	string szName = p_szName.empty() ? p_szData->m_szName : p_szName ;
	if( szName.empty() )
	{
		//throw std::invalid_argument("Skeleton Name is null!");
		//android 平台不支持c++11特性
		printf("Skeleton Name is null!");
	}

	m_mapDataDic[szName] = p_szData;
}
//-------------------------------------------------------------------------
void CBaseFactory::RemoveSkeletonData( const string& p_szName )
{
	map< string, CSkeletonData* >::iterator Ite = m_mapDataDic.find( p_szName );
	if( Ite == m_mapDataDic.end() )
		return;

	m_mapDataDic.erase( Ite );
}
//-------------------------------------------------------------------------
ITextureAtlas* CBaseFactory::GetTextureAtlas( const string& p_szName )
{
	map< string, ITextureAtlas* >::iterator Ite = m_mapTextureAtlasDic.find( p_szName );
	if( Ite == m_mapTextureAtlasDic.end() )
		return NULL;

	return Ite->second;
}
//-------------------------------------------------------------------------
void CBaseFactory::AddTextureAtlas( ITextureAtlas* p_pTextureAtlas, const string& p_szName )
{
	if( p_pTextureAtlas == NULL )
	{
		//throw std::invalid_argument("TextureAtlas Data is null!");
		//android 平台不支持c++11特性
		printf("TextureAtlas Data is null!");
	}
	
	string szName = p_szName.empty() ? p_pTextureAtlas->GetName() : p_szName;
	if( szName.empty() )
	{
		//throw std::invalid_argument("TextureAtlas Name is null!");
		//android 平台不支持c++11特性
		printf("TextureAtlas Name is null!");
	}

	m_mapTextureAtlasDic[szName] = p_pTextureAtlas;
}
//-------------------------------------------------------------------------
void CBaseFactory::RemoveTextureAtlas( const string& p_szName )
{
	map< string, ITextureAtlas* >::iterator Ite = m_mapTextureAtlasDic.find( p_szName );
	if( Ite == m_mapTextureAtlasDic.end() )
		return;

	m_mapTextureAtlasDic.erase( Ite );
}
//-------------------------------------------------------------------------
void CBaseFactory::Clear( bool p_bIsClearData )
{
	if( p_bIsClearData )
	{
		for( map<string, CSkeletonData*>::iterator Ite = m_mapDataDic.begin();
			Ite != m_mapDataDic.end();
			++Ite )
		{
			Ite->second->Clear();
		}

		for( map<string, ITextureAtlas*>::iterator Ite = m_mapTextureAtlasDic.begin();
			Ite != m_mapTextureAtlasDic.end();
			++Ite )
		{
			Ite->second->Clear();
		}
	}
	m_mapDataDic.clear();
	m_mapTextureAtlasDic.clear();
	m_mapTextureAtlasLoadingDic.clear();
}
//-------------------------------------------------------------------------
CArmature* CBaseFactory::BuildArmature( const string& p_szArmatureName, const string& p_szAnimationName, const string& p_szPicName,
							  const string& p_szSkeletonName, const string& p_szTextureAltasName, const string& p_szSkinName )
{
	// step 1 ： 获取Armature骨架数据
	CArmatureData* pArmatureData = NULL;
	CSkeletonData* pSkeletonData = NULL;
	if( !p_szSkeletonName.empty() )
	{
		map<string, CSkeletonData*>::iterator Ite = m_mapDataDic.find( p_szSkeletonName );
		if( Ite != m_mapDataDic.end() )
		{
			pSkeletonData = Ite->second;
			pArmatureData = pSkeletonData->GetArmatureDataByName( p_szArmatureName );
		}
	}

	if( pArmatureData == NULL )
		return NULL;

	// step 2 : 创建一个空的骨架
	CArmature* pArmature = GenerateArmature( p_szPicName );
	pArmature->m_szName = p_szArmatureName;


	// step 3 : 添加Bone
	CBone* pBone = NULL;
	for( unsigned int i = 0; i < pArmatureData->m_vecBoneDataList.size(); ++i )
	{
		CBoneData* pBoneData	= pArmatureData->m_vecBoneDataList[i];
		pBone					= new CBone();
		pBone->m_szName			= pBoneData->m_szName;
		pBone->m_bFixedRotation	= pBoneData->m_bIsRotationFixed;
		pBone->m_nScaleMode		= pBoneData->m_nScaleMode;
		pBone->m_OriginTransform= pBoneData->m_Transform;

		if( pArmatureData->GetBoneDataByName( pBoneData->m_szParent ) )
		{
			pArmature->AddBone( pBone, pBoneData->m_szParent );
		}
		else
		{
			pArmature->AddBone( pBone );
		}
	}

	CArmatureData* pAnimationArmatureData = NULL;
	CSkinData* pSkinDataCopy = NULL;
	if(( !p_szAnimationName.empty() ) && ( p_szAnimationName != p_szArmatureName ))
	{
		CArmatureData* pAnimationArmatureDataCopy = pSkeletonData->GetArmatureDataByName( p_szAnimationName );
		if( pAnimationArmatureDataCopy )
		{
			pSkinDataCopy = pAnimationArmatureDataCopy->GetSkinDataByName( "" );
		}
	}


	// step 4 : 保存动画数据信息
	//if( pAnimationArmatureData )
	//	pArmature->m_pAnimation->SetAnimationDataList( pAnimationArmatureData->m_vecAnimationDataList );
	//else
		pArmature->m_pAnimation->SetAnimationDataList( pArmatureData->m_vecAnimationDataList );


	// step 5 : 为每个bone找到对应的slot，然后绑定display对象
	CSkinData* pSkinData = pArmatureData->GetSkinDataByName( p_szSkinName );
	if( pSkinData == NULL )
		return NULL;

	CSlot* pSlot = NULL;
	CDisplayData* pDisplayData = NULL;
	CArmature* pChildAramture = NULL;
	int i = 0;

	for( unsigned int j = 0; j < pSkinData->m_vecSlotDataList.size(); ++j )
	{
		CSlotData* pSlotData = pSkinData->m_vecSlotDataList[j];
		pBone = pArmature->GetBoneByName( pSlotData->m_szParent );
		if( pBone == NULL )
			continue;

		pSlot						= GenerateSlot();
		pSlot->m_szName				= pSlotData->m_szName;
		pSlot->SetBlendMode( pSlotData->m_szBlendMode );
		pSlot->m_fOriginZOrder		= pSlotData->m_fZOrder;
		pSlot->m_vecDisplayDataList	= pSlotData->m_vecDisplayDataList;

		vector< IObject* > vecTmpArray;
		i = pSlotData->m_vecDisplayDataList.size();
		vecTmpArray.resize( i );
		// 为每个slot创建不同的display对象，并进行挂接到slot上
		while( i-- )
		{
			pDisplayData = pSlotData->m_vecDisplayDataList[i];

			if( pDisplayData->m_szType == ConstString::ARMATURE )
			{
				CDisplayData* pDisplayDataCopy = NULL;
				if( pSkinDataCopy )
				{
					CSlotData* pSlotDataCopy = pSkinDataCopy->GetSlotDataByName( pSlotData->m_szName );
					if( pSlotDataCopy )
					{
						pDisplayDataCopy = pSlotDataCopy->m_vecDisplayDataList[i];
					}
				}
				else
				{
					pDisplayDataCopy = NULL;
				}

				pChildAramture = BuildArmature( pDisplayData->m_szName, pDisplayDataCopy ? pDisplayDataCopy->m_szName : "",
					p_szPicName, p_szSkeletonName, p_szTextureAltasName.empty() ? p_szSkeletonName : p_szTextureAltasName );
				if( pChildAramture )
				{
					vecTmpArray[i] = pChildAramture;
				}

				continue;
			}
			else
			{
				vecTmpArray[i] = GenerateDisplay( GetTextureAtlas( p_szTextureAltasName.empty() ? p_szSkeletonName : p_szTextureAltasName ), pDisplayData->m_szName,
					pDisplayData->m_Pivot.m_fX, pDisplayData->m_Pivot.m_fY );
			}
		}

		pSlot->SetDisplayList( vecTmpArray );
		pSlot->ChangeDisplay( 0 );
		pBone->AddChild( pSlot );
	}

	i = pArmature->m_vecBoneList.size();
	while( i-- )
	{
		pArmature->m_vecBoneList[i]->Update();
	}
	i = pArmature->m_vecSlotList.size();
	while( i-- )
	{
		pArmature->m_vecSlotList[i]->Update();
	}

	// 更新全部slot的Z值,保证期覆盖顺序正常
	pArmature->UpdateSlotsZOrder();
	return pArmature;
}
//-------------------------------------------------------------------------
IObject* CBaseFactory::GetTextureDisplay( const string& p_szTextureName, const string& p_szTextureAtlasName,
								  f32 p_fPivotX, f32 p_fPivotY )
{
	ITextureAtlas* pTextureAltas = NULL;
	if( !p_szTextureAtlasName.empty() )
	{
		map<string, ITextureAtlas* >::iterator Ite = m_mapTextureAtlasDic.find( p_szTextureAtlasName );
		if(Ite != m_mapTextureAtlasDic.end() )
		{
			pTextureAltas = Ite->second;
		}
	}

	if( pTextureAltas )
	{
		if( IsNaN(p_fPivotX) || (IsNaN(p_fPivotY)) )
		{
			CSkeletonData* pData = m_mapDataDic[p_szTextureAtlasName];
			if( pData )
			{
				CPoint* pPivot = pData->GetSubTexturePivot( p_szTextureName );
				if( pPivot )
				{
					p_fPivotX = pPivot->m_fX;
					p_fPivotY = pPivot->m_fY;
				}
			}
		}
		return GenerateDisplay( pTextureAltas, p_szTextureName, p_fPivotX, p_fPivotY );
	}

	return NULL;
}
//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------