//*************************************************************************
//	创建日期:	2014-8-11   11:09
//	文件名称:	ColorTransform.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
//-------------------------------------------------------------------------


class CColorTransform
{
public:
	CColorTransform()
		: m_unColor( 0 )
		, m_cAlphaOffset( 0 )
		, m_cRedOffset( 0 )
		, m_cGreenOffset( 0 )
		, m_cBlueOffset( 0 )
		, m_fAlphaMulti( 0.0f )
		, m_fRedMulti( 0.0f )
		, m_fGreenMulti( 0.0f )
		, m_fBlueMulti( 0.0f )
	{

	}
public:
	uint		m_unColor;				// RGB颜色值
	/**
	* 下面的offset取值[-255, 255]，先与Multi参数乘积，再和对应的 透明度通道/颜色通道 值相加
	*/
	f32			m_fAlphaMulti;
	f32			m_fRedMulti;
	f32			m_fGreenMulti;
	f32			m_fBlueMulti;
	c8			m_cAlphaOffset;
	c8			m_cRedOffset;
	c8			m_cGreenOffset;
	c8			m_cBlueOffset;
};


	//-------------------------------------------------------------------------