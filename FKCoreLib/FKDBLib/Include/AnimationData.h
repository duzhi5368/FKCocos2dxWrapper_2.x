//*************************************************************************
//	创建日期:	2014-8-11   17:02
//	文件名称:	AnimationData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "TransformTimeLine.h"
#include "BaseMath.h"
#include <string>
#include <map>
using std::string;
using std::map;
//-------------------------------------------------------------------------
typedef map< string, CTimeLine* >		TimeLines;
//-------------------------------------------------------------------------
class CAnimationData : public CTimeLine
{
public:
	uint			m_unFrameRate;
	string			m_szName;
	int				m_nLoop;
	f32				m_fTweenEasing;
	TimeLines		m_mapTimeLines;
	f32				m_fFadeInTime;
public:
	CAnimationData()
		: m_nLoop( 0 )
		, m_unFrameRate( 0 )
		, m_fTweenEasing( NaN )
		, m_fFadeInTime( 0 )
	{
		m_szName.clear();
		m_mapTimeLines.clear();
	}

	virtual ~CAnimationData()
	{
		Clear();
	}

	virtual void Clear()
	{
		TimeLines::iterator Ite = m_mapTimeLines.begin();
		TimeLines::const_iterator IteEnd = m_mapTimeLines.end();
		for( ; Ite != IteEnd; ++Ite )
		{
			if( Ite->second != &CTransformTimeLine::EMPTY_TIMELINE )
			{
				Ite->second->Clear();
				delete Ite->second;
			}
		}
		m_mapTimeLines.clear();
	}

	CTransformTimeLine* GetTransformTimeLineByName( const string& p_szTimeLineName )
	{
		TimeLines::iterator Ite = m_mapTimeLines.find( p_szTimeLineName );
		if( Ite == m_mapTimeLines.end() )
			return NULL;

		return static_cast<CTransformTimeLine*>( Ite->second );
	}

	void AddTimeLine( CTransformTimeLine* p_pTimeLine, const string& p_szName )
	{
		m_mapTimeLines[p_szName] = p_pTimeLine;
	}
};
//-------------------------------------------------------------------------