//-------------------------------------------------------------------------
#include "../Include/Matrix.h"
#include <algorithm>
#include <cmath>
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
extern const CMatrix MatrixIdentity = CMatrix::MakeIndentityMatrix();
//-------------------------------------------------------------------------
// 生成一个矩阵
CMatrix	CMatrix::MakeMatrix( f32 p_fA, f32 p_fB, f32 p_fC, f32 p_fD, f32 p_fX, f32 p_fY )
{
	CMatrix t;
	t.m_fA = p_fA; 
	t.m_fB = p_fB;
	t.m_fC = p_fC;
	t.m_fD = p_fD;
	t.m_fX = p_fX;
	t.m_fY = p_fY;
	return t;
}
//-------------------------------------------------------------------------
// 将一个点通过矩阵转换
CPoint CMatrix::MutilMaxtrix( const CPoint& p_tagPoint, const CMatrix& p_tagMatrix )
{
	CPoint p;
	p.m_fX = static_cast<f32>((double)p_tagMatrix.m_fA * p_tagPoint.m_fX + (double)p_tagMatrix.m_fC * p_tagPoint.m_fY + p_tagMatrix.m_fX );
	p.m_fY = static_cast<f32>((double)p_tagMatrix.m_fB * p_tagPoint.m_fX + (double)p_tagMatrix.m_fD * p_tagPoint.m_fY + p_tagMatrix.m_fY );
	return p;
}
//-------------------------------------------------------------------------
// 生成一个单元矩阵
CMatrix	CMatrix::MakeIndentityMatrix()
{
	return CMatrix::MakeMatrix( 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f );
}
//-------------------------------------------------------------------------
// 点对称矩阵
CMatrix	CMatrix::TranslateMatrix( const CMatrix& p_tagMatrix, f32 p_fX, f32 p_fY )
{
	return CMatrix::MakeMatrix( p_tagMatrix.m_fA, p_tagMatrix.m_fB, p_tagMatrix.m_fC, p_tagMatrix.m_fD,
		p_tagMatrix.m_fX + p_tagMatrix.m_fA * p_fX + p_tagMatrix.m_fC * p_fY,
		p_tagMatrix.m_fY + p_tagMatrix.m_fB * p_fX + p_tagMatrix.m_fD * p_fY );
}
//-------------------------------------------------------------------------
// 旋转矩阵
CMatrix	CMatrix::RotateMatrix( const CMatrix& p_tagMatrix, f32 p_fAngle )
{
	f32 fSin = sin( p_fAngle );
	f32 fCos = cos( p_fAngle );

	return CMatrix::MakeMatrix( p_tagMatrix.m_fA * fCos + p_tagMatrix.m_fC * fSin,
		p_tagMatrix.m_fB * fCos + p_tagMatrix.m_fD * fSin,
		p_tagMatrix.m_fC * fCos - p_tagMatrix.m_fA * fSin,
		p_tagMatrix.m_fD * fCos - p_tagMatrix.m_fB * fSin, 
		p_tagMatrix.m_fX, p_tagMatrix.m_fY );
}
//-------------------------------------------------------------------------
// 缩放矩阵
CMatrix	CMatrix::ScaleMatrix( const CMatrix& p_tagMatrix, f32 p_fXScale, f32 p_fYScale )
{
	return CMatrix::MakeMatrix( p_tagMatrix.m_fA * p_fXScale, p_tagMatrix.m_fB * p_fXScale,
		p_tagMatrix.m_fC * p_fYScale, p_tagMatrix.m_fD * p_fYScale, p_tagMatrix.m_fX, p_tagMatrix.m_fY );
}
//-------------------------------------------------------------------------
// 连接矩阵
CMatrix	CMatrix::ConcatMatrix( const CMatrix& p_tagMatrix1, const CMatrix& p_tagMatrix2 )
{
	return CMatrix::MakeMatrix( p_tagMatrix1.m_fA * p_tagMatrix2.m_fA + p_tagMatrix1.m_fB * p_tagMatrix2.m_fC,
		p_tagMatrix1.m_fA * p_tagMatrix2.m_fB + p_tagMatrix1.m_fB * p_tagMatrix2.m_fD,
		p_tagMatrix1.m_fC * p_tagMatrix2.m_fA + p_tagMatrix1.m_fD * p_tagMatrix2.m_fC,
		p_tagMatrix1.m_fC * p_tagMatrix2.m_fB + p_tagMatrix1.m_fD * p_tagMatrix2.m_fD,
		p_tagMatrix1.m_fX * p_tagMatrix2.m_fA + p_tagMatrix1.m_fY * p_tagMatrix2.m_fC + p_tagMatrix2.m_fX,
		p_tagMatrix1.m_fA * p_tagMatrix2.m_fB + p_tagMatrix1.m_fY * p_tagMatrix2.m_fD + p_tagMatrix2.m_fY );
}
//-------------------------------------------------------------------------
// 反转矩阵
CMatrix	CMatrix::InvertMatrix( const CMatrix& p_tagMatrix )
{
	float fDeterminant = 1 / ( p_tagMatrix.m_fA * p_tagMatrix.m_fD - p_tagMatrix.m_fB * p_tagMatrix.m_fC );

	return MakeMatrix( fDeterminant * p_tagMatrix.m_fD, 
		- fDeterminant * p_tagMatrix.m_fB,
		- fDeterminant * p_tagMatrix.m_fC,
		fDeterminant * p_tagMatrix.m_fA,
		fDeterminant * ( p_tagMatrix.m_fC * p_tagMatrix.m_fY - p_tagMatrix.m_fD * p_tagMatrix.m_fX ),
		fDeterminant * ( p_tagMatrix.m_fB * p_tagMatrix.m_fX - p_tagMatrix.m_fA * p_tagMatrix.m_fY ) );
}
//-------------------------------------------------------------------------
// 比较两个矩阵是否相等
bool CMatrix::IsEqual( const CMatrix& p_tagMatrix1, const CMatrix& p_tagMatrix2 )
{
	return ( p_tagMatrix1.m_fA == p_tagMatrix2.m_fA && 
		p_tagMatrix1.m_fB == p_tagMatrix2.m_fB && 
		p_tagMatrix1.m_fC == p_tagMatrix2.m_fC && 
		p_tagMatrix1.m_fD == p_tagMatrix2.m_fD && 
		p_tagMatrix1.m_fX == p_tagMatrix2.m_fX && 
		p_tagMatrix1.m_fY == p_tagMatrix2.m_fY );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------