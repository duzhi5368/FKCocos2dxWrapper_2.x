//*************************************************************************
//	创建日期:	2014-8-11   10:25
//	文件名称:	BaseMacro.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include <vector>
//-------------------------------------------------------------------------
// 数据格式定义
//-------------------------------------------------------------------------
typedef float			f32;
typedef unsigned int	uint;
typedef unsigned char	uc8;
typedef char			c8;
//-------------------------------------------------------------------------
// 常用宏
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#endif
//-------------------------------------------------------------------------
// 标示参数是 传出目的参数
#ifndef FK_OUT
#define FK_OUT
#endif
//-------------------------------------------------------------------------
#ifndef FK_INLINE

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define FK_INLINE	inline
#else
#define FK_INLINE	__forceinline
#endif

#endif
//-------------------------------------------------------------------------
// float浮点值比较阀值
#ifndef FK_EPSILON
#define FK_EPSILON     1.192092896e-07F
#endif
//-------------------------------------------------------------------------
template< typename T >
int GetVectorItemIndex( std::vector<T>& vec, const T& value )
{
	for( unsigned int i = 0; i < vec.size(); ++i )
	{
		if( vec[i] == value )
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}
//-------------------------------------------------------------------------