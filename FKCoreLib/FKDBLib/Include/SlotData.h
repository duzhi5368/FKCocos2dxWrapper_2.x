//*************************************************************************
//	创建日期:	2014-8-11   16:20
//	文件名称:	SlotData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "DisplayData.h"
#include <string>
#include <vector>
using std::vector;
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
#define DEFAULT_BLEND_MODE		"normal"
//-------------------------------------------------------------------------
class CSlotData
{
public:
	string						m_szName;
	string						m_szParent;
	string						m_szBlendMode;
	f32							m_fZOrder;
	vector<CDisplayData*>		m_vecDisplayDataList;
public:
	CSlotData()
		: m_fZOrder( 0.0f )
	{
		m_szName.clear();
		m_szParent.clear();
		m_vecDisplayDataList.clear();
		m_szBlendMode = DEFAULT_BLEND_MODE;
	}

	virtual ~CSlotData()
	{
		Clear();
	}
public:
	virtual void Clear()
	{
		for( unsigned int i = 0; i < m_vecDisplayDataList.size(); ++i )
		{
			m_vecDisplayDataList[i]->Clear();
			delete m_vecDisplayDataList[i];
		}
		m_vecDisplayDataList.clear();
	}

	CDisplayData* GetDisplayeDataByName( const string& p_szDisplayName )
	{
		for( unsigned int i = 0; i < m_vecDisplayDataList.size(); ++i )
		{
			if( m_vecDisplayDataList[i]->m_szName == p_szDisplayName )
			{
				return m_vecDisplayDataList[i];
			}
		}
		return NULL;
	}

	void AddDisplayData( CDisplayData* p_pData )
	{
		if( NULL == p_pData )
			return;
		CDisplayData* p = GetDisplayeDataByName( p_pData->m_szName );
		if( p != NULL )
			return;
		m_vecDisplayDataList.push_back( p_pData );
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------