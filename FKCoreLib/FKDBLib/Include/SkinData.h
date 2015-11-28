//*************************************************************************
//	创建日期:	2014-8-11   16:35
//	文件名称:	SkinData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "SlotData.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class CSkinData
{
public:
	string						m_szName;
	vector<CSlotData*>			m_vecSlotDataList;
public:
	CSkinData()
	{
		m_szName.clear();
		m_vecSlotDataList.clear();
	}
	virtual ~CSkinData()
	{
		Clear();
	}
public:
	virtual void Clear()
	{
		for( unsigned int i = 0; i < m_vecSlotDataList.size(); ++i )
		{
			m_vecSlotDataList[i]->Clear();
			delete m_vecSlotDataList[i];
		}
		m_vecSlotDataList.clear();
	}

	CSlotData* GetSlotDataByName( const string& p_szSlotName )
	{
		for( unsigned int i = 0; i < m_vecSlotDataList.size(); ++i )
		{
			if( m_vecSlotDataList[i]->m_szName == p_szSlotName )
			{
				return m_vecSlotDataList[i];
			}
		}
		return NULL;
	}

	void AddSlotData( CSlotData* p_pData )
	{
		if( NULL == p_pData )
			return;
		CSlotData* p = GetSlotDataByName( p_pData->m_szName );
		if( p != NULL )
			return;
		m_vecSlotDataList.push_back( p_pData );
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------