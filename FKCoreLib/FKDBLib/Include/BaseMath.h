//*************************************************************************
//	创建日期:	2014-8-11   14:34
//	文件名称:	BaseMath.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include <math.h>
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
static const f32		F_PI				= 3.14159265f;
static const f32		HALF_PI				= F_PI * 0.5f;
static const f32		DOUBLE_PI			= F_PI * 2;
static const f32		ANGLE_TO_RADIN		= F_PI / 180.0f;
static const f32		FLOAT_INFINITE		= FLT_MAX;
//-------------------------------------------------------------------------
FK_INLINE f32 FormatRadian( f32 p_fRadian )
{
	f32 fRadion = p_fRadian;
	fRadion = fmod( fRadion, DOUBLE_PI );
	if( fRadion > F_PI )
		fRadion -= DOUBLE_PI;
	if( fRadion < -F_PI )
		fRadion += DOUBLE_PI;
	return fRadion;
}
//-------------------------------------------------------------------------
static const unsigned int MAX_U	= ~0;
static const f32 NaN = *(static_cast<f32*>(static_cast<void*>(const_cast<unsigned int*>(&MAX_U))));
//-------------------------------------------------------------------------
FK_INLINE bool IsEqual( f32 p_fA, f32 p_fB )
{
	return ( fabs( p_fA - p_fB ) < FK_EPSILON );
}
//-------------------------------------------------------------------------
FK_INLINE bool IsEqual( f32 p_fA, int p_nB )
{
	return ( fabs( p_fA - static_cast<f32>(p_nB) ) < FK_EPSILON );
}
//-------------------------------------------------------------------------
FK_INLINE bool IsNaN( f32 p_fNum )
{
	return !( p_fNum > 0 || p_fNum < 0 || p_fNum == 0);
}
//-------------------------------------------------------------------------
FK_INLINE f32 Round( f32 p_fNum )
{
	return static_cast<f32>( static_cast<int>( p_fNum + 0.5f ) );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------