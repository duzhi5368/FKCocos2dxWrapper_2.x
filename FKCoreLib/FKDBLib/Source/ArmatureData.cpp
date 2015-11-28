//-------------------------------------------------------------------------
#include "../Include/ArmatureData.h"
#include <algorithm>
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void CArmatureData::SortBoneDataList()
{
	if( m_vecBoneDataList.empty() )
		return;

	vector< std::pair<int, CBoneData*> >		vecTmp;	// 骨骼深度表

	for( unsigned int i = 0; i < m_vecBoneDataList.size(); ++i )
	{
		CBoneData* pBoneData = m_vecBoneDataList[i];
		int nLevel = 0;								// 骨骼深度层级
		CBoneData* pParentData = pBoneData;
		// 递归找根节点
		while( pParentData && !( pParentData->m_szParent.empty() ) )
		{
			nLevel++;
			pParentData = GetBoneDataByName( pParentData->m_szParent );
		}
		vecTmp.push_back( std::make_pair( nLevel, pBoneData ) );
	}

	// 排序后，根节点位于vector前端。深度越高的节点越位于尾部
	std::sort( vecTmp.begin(), vecTmp.end(), SortBoneFunc );

	unsigned int i = vecTmp.size();
	while( i-- )
	{
		// 拷贝到自己的 boneList 中
		m_vecBoneDataList[i] = vecTmp[i].second;
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------