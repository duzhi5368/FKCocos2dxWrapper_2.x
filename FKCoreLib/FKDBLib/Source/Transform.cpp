//-------------------------------------------------------------------------
#include "../Include/Matrix.h"
#include "../Include/Transform.h"
#include "../Include/BaseMath.h"
#include <cmath>
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 转换 Transform 为 矩阵
void CTransform::TransformTransformToMatrix( CTransform& p_tagTransform, FK_OUT CMatrix& p_OutMatrix )
{
	p_OutMatrix.m_fA = p_tagTransform.m_fScaleX * cos( p_tagTransform.m_fSkewY  );
	p_OutMatrix.m_fB = p_tagTransform.m_fScaleX * sin( p_tagTransform.m_fSkewY  );
	p_OutMatrix.m_fC = - p_tagTransform.m_fScaleY * sin( p_tagTransform.m_fSkewX );
	p_OutMatrix.m_fD = p_tagTransform.m_fScaleY * cos( p_tagTransform.m_fSkewX );
	p_OutMatrix.m_fX = p_tagTransform.m_fPosX;
	p_OutMatrix.m_fY = p_tagTransform.m_fPosY;
}
//-------------------------------------------------------------------------
// 转换 子类当前矩阵 为 父级矩阵
void CTransform::TransformPointWithParent( CTransform& p_tagTransform, CTransform& p_tagParent )
{
	CMatrix tagTmpMatrix;
	TransformTransformToMatrix( p_tagParent, tagTmpMatrix );
	tagTmpMatrix = CMatrix::InvertMatrix( tagTmpMatrix );

	f32 fOldPosX = p_tagTransform.m_fPosX;
	f32 fOldPosY = p_tagTransform.m_fPosY;

	p_tagTransform.m_fPosX = tagTmpMatrix.m_fA * fOldPosX + tagTmpMatrix.m_fC * fOldPosY + tagTmpMatrix.m_fX;
	p_tagTransform.m_fPosY = tagTmpMatrix.m_fD * fOldPosY + tagTmpMatrix.m_fB * fOldPosX + tagTmpMatrix.m_fY;

	p_tagTransform.m_fSkewX  = FormatRadian( p_tagTransform.m_fSkewX - p_tagParent.m_fSkewX );
	p_tagTransform.m_fSkewY  = FormatRadian( p_tagTransform.m_fSkewY - p_tagParent.m_fSkewY );
}
//-------------------------------------------------------------------------
f32	CTransform::GetEaseValue( f32 p_fValue, f32 p_fEasing )
{
	f32 fValueEase = 0.0f;
	if( p_fEasing > 1 )
	{
		fValueEase = 0.5f * ( 1 - cos( p_fValue * F_PI )) - p_fValue;
		p_fEasing -= 1.0f;
	}
	else if( p_fEasing > 0 )
	{
		fValueEase = sin( p_fValue * HALF_PI ) - p_fValue;
	}
	else if( p_fEasing < 0 )
	{
		fValueEase = 1 - cos( p_fValue * HALF_PI ) - p_fValue;
		p_fEasing *= -1;
	}
	return fValueEase * p_fEasing + p_fValue;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------