//*************************************************************************
//	创建日期:	2014-8-11   15:12
//	文件名称:	TimeLine.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include <vector>
#include "Frame.h"
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 时间轴
class CTimeLine
{
public:
	vector<CFrame*>		m_vecFrameList;			// 本时间轴上的关键帧列表
	f32					m_fDuration;			// 本时间抽持续时间
	f32					m_fScale;				// 本事件缩放比
public:
	CTimeLine()
		: m_fScale ( 1.0f )
		, m_fDuration( 0.0f )
	{
		m_vecFrameList.clear();
	}
	virtual ~CTimeLine()
	{
		Clear();
	}
public:
	virtual void Clear()
	{
		for( unsigned int i = 0; i < m_vecFrameList.size(); ++i )
		{
			m_vecFrameList[i]->Clear();
			delete m_vecFrameList[i];
		}
		m_vecFrameList.clear();
	}

	virtual void AddFrame( CFrame* p_pFrame )
	{
		if( p_pFrame == NULL )
			return;
		m_vecFrameList.push_back( p_pFrame );
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------