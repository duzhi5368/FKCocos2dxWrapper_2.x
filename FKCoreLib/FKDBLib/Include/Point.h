//*************************************************************************
//	创建日期:	2014-8-11   10:34
//	文件名称:	Point.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include <math.h>
//-------------------------------------------------------------------------
class CPoint
{
public:
	CPoint( f32 p_fX, f32 p_fY )
		: m_fX( p_fX )
		, m_fY( p_fY )
	{

	}
	CPoint()
		: m_fX( 0.0f )
		, m_fY( 0.0f )
	{

	}
public:
	bool operator == ( const CPoint& p_Other )
	{
		return (fabs(this->m_fX - p_Other.m_fX) < FK_EPSILON)
			&& (fabs(this->m_fY - p_Other.m_fY) < FK_EPSILON);
	}

public:
	f32 m_fX;
	f32 m_fY;
};


//-------------------------------------------------------------------------
