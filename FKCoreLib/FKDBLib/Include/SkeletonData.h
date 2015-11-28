//*************************************************************************
//	创建日期:	2014-8-12   11:25
//	文件名称:	SkeletonData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "ArmatureData.h"
#include "Point.h"
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 这其实是一个完整xml文件的数据了，并不是一个骨架或者一个Bones骨骼的信息
class CSkeletonData
{
public:
	string						m_szName;
	map< string, CPoint >		m_mapSubTexturePivots;		// 子纹理中心点位置表
	vector< CArmatureData* >	m_vecArmatureDataList;		// 这里是整个xml文件中所记录的子Armature列表
public:
	CSkeletonData()
	{
		m_szName.clear();
		m_vecArmatureDataList.clear();
		m_mapSubTexturePivots.clear();
	}
	virtual ~CSkeletonData()
	{
		Clear();
	}
public:
	vector<string>		GetArmatureNameList()
	{
		vector<string>		vecNameList;
		vecNameList.clear();
		for( unsigned int i = 0; i < m_vecArmatureDataList.size(); ++i )
		{
			vecNameList.push_back( m_vecArmatureDataList[i]->m_szName );
		}
		return vecNameList;
	}

	virtual void Clear()
	{
		for( unsigned int i = 0; i < m_vecArmatureDataList.size(); ++i )
		{
			m_vecArmatureDataList[i]->Clear();
			delete m_vecArmatureDataList[i];
		}
		m_vecArmatureDataList.clear();
		m_mapSubTexturePivots.clear();
	}

	CArmatureData* GetArmatureDataByName( const string& p_szArmatureName )
	{
		for( unsigned int i = 0; i < m_vecArmatureDataList.size(); ++i )
		{
			if( m_vecArmatureDataList[i]->m_szName == p_szArmatureName )
			{
				return m_vecArmatureDataList[i];
			}
		}
		return NULL;
	}

	void AddArmatureData( CArmatureData* p_pArmatureData )
	{
		if( p_pArmatureData == NULL )
			return;
		m_vecArmatureDataList.push_back( p_pArmatureData );
	}

	void RemoveArmatureData( CArmatureData* p_pArmatureData )
	{
		if( p_pArmatureData == NULL )
			return;
		if( p_pArmatureData->m_szName.empty() )
			return;
		RemoveArmatureDataByName( p_pArmatureData->m_szName );
	}

	void RemoveArmatureDataByName( const string& p_szArmatureName )
	{
		for( unsigned int i = 0; i < m_vecArmatureDataList.size(); ++i )
		{
			if( m_vecArmatureDataList[i]->m_szName == p_szArmatureName )
			{
				vector<CArmatureData*>::iterator Ite = m_vecArmatureDataList.begin() + i;
				delete *Ite;
				m_vecArmatureDataList.erase( m_vecArmatureDataList.begin() + i );
				return;
			}
		}
	}

	CPoint* GetSubTexturePivot( const string& p_szSubTextureName )
	{
		map<string, CPoint>::iterator Ite = m_mapSubTexturePivots.find( p_szSubTextureName );
		if( Ite == m_mapSubTexturePivots.end() )
			return NULL;
		return &(Ite->second);
	}

	CPoint	AddSubTexturePivot( f32 p_fX, f32 p_fY, const string& p_szSubTextureName )
	{
		map< string, CPoint >::iterator Ite = m_mapSubTexturePivots.find( p_szSubTextureName );
		if( Ite != m_mapSubTexturePivots.end() )
		{
			// 若已存在，则更新位置
			Ite->second.m_fX = p_fX;
			Ite->second.m_fY = p_fY;
			return Ite->second;
		}

		CPoint& pt = m_mapSubTexturePivots[p_szSubTextureName];
		pt.m_fX = p_fX;
		pt.m_fY = p_fY;
		return pt;
	}

	void RemoveSubTexturePivot( const string& p_szSubTextureName )
	{
		map< string, CPoint >::iterator Ite = m_mapSubTexturePivots.find( p_szSubTextureName );
		if( Ite == m_mapSubTexturePivots.end() )
			return;
		m_mapSubTexturePivots.erase( Ite );
	}

	void RemoveAllSubTexturePivots()
	{
		m_mapSubTexturePivots.clear();
	}
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------