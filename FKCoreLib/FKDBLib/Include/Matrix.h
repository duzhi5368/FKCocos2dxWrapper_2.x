//*************************************************************************
//	创建日期:	2014-8-11   11:18
//	文件名称:	Matrix.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Point.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class CMatrix
{
public:
	f32 m_fA, m_fB, m_fC, m_fD;
	f32 m_fX, m_fY;
public:
	CMatrix()
		: m_fA( 0.0f )
		, m_fB( 0.0f )
		, m_fC( 0.0f )
		, m_fD( 0.0f )
		, m_fX( 0.0f )
		, m_fY( 0.0f )
	{

	}
public:
	// 生成一个矩阵
	static CMatrix	MakeMatrix( f32 p_fA, f32 p_fB, f32 p_fC, f32 p_fD, f32 p_fX, f32 p_fY );
	// 将一个点通过矩阵转换
	static CPoint	MutilMaxtrix( const CPoint& p_tagPoint, const CMatrix& p_tagMatrix );
	// 生成一个单元矩阵
	static CMatrix	MakeIndentityMatrix();
	// 点对称矩阵
	static CMatrix	TranslateMatrix( const CMatrix& p_tagMatrix, f32 p_fX, f32 p_fY );
	// 旋转矩阵
	static CMatrix	RotateMatrix( const CMatrix& p_tagMatrix, f32 p_fAngle );
	// 缩放矩阵
	static CMatrix	ScaleMatrix( const CMatrix& p_tagMatrix, f32 p_fXScale, f32 p_fYScale );
	// 连接矩阵
	static CMatrix	ConcatMatrix( const CMatrix& p_tagMatrix1, const CMatrix& p_tagMatrix2 );
	// 反转矩阵
	static CMatrix	InvertMatrix( const CMatrix& p_tagMatrix );
	// 比较两个矩阵是否相等
	static bool		IsEqual( const CMatrix& p_tagMatrix1, const CMatrix& p_tagMatrix2 );
};

// 单位矩阵
extern const CMatrix MatrixIdentity;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------