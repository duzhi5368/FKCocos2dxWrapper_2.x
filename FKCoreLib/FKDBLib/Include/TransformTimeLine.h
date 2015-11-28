//*************************************************************************
//	创建日期:	2014-8-11   15:22
//	文件名称:	TransformTimeLine.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "TimeLine.h"
#include "Transform.h"
#include "Point.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class CTransformTimeLine : public CTimeLine
{
public:
	static CTransformTimeLine EMPTY_TIMELINE;
public:
	bool		m_bTransformed;			// 是否已经转换过矩阵
	CTransform	m_OriginTransform;		// 时间轴自身原转换矩阵
	CPoint		m_OriginPiovt;			// 时间轴自身原轴心
	f32			m_fOffset;				// 偏移量
public:
	CTransformTimeLine()
		: m_fOffset( 0.0f )
		, m_bTransformed( false )
	{

	}
public:
	bool operator == ( const CTransformTimeLine& p_Other )
	{
		return ( m_bTransformed == p_Other.m_bTransformed &&
			m_OriginPiovt == p_Other.m_OriginPiovt &&
			m_OriginTransform == p_Other.m_OriginTransform &&
			(fabs(this->m_fOffset - p_Other.m_fOffset) < FK_EPSILON) );
	}
public:
	virtual void Clear()
	{
		if( this == &EMPTY_TIMELINE )
			return;

		CTimeLine::Clear();
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------