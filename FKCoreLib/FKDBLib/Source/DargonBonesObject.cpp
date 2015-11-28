//-------------------------------------------------------------------------
#include "../Include/DargonBonesObject.h"
#include "../Include/Armature.h"
#include "../Include/Matrix.h"
#include "../Include/Bone.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

CDragonBonesObject::CDragonBonesObject()
	: m_pUserData( NULL )
	, m_pParent( NULL )
	, m_nScaleType( 0 )
	, m_pArmature( NULL )
{
	m_bIsVisible = true;
	m_bFixedRotation = m_bIsColorChanged = false;
	m_szName.clear();
	m_TweenTransform.m_fScaleX = m_TweenTransform.m_fScaleY = 0.0f;
}
//-------------------------------------------------------------------------
CDragonBonesObject::~CDragonBonesObject()
{
	Clear();
}
//-------------------------------------------------------------------------
void CDragonBonesObject::SetArmature( CArmature* p_pValue )
{
	if( m_pArmature )
	{
		m_pArmature->RemoveDragonBonesObject( this );
	}
	m_pArmature = p_pValue;
	if( m_pArmature )
	{
		m_pArmature->AddDragonBonesObject( this );
	}
}
//-------------------------------------------------------------------------
void CDragonBonesObject::Clear()
{
	m_pUserData			= NULL;
	m_pParent			= NULL;
	m_pArmature			= NULL;
	m_bFixedRotation	= false;
	m_bIsColorChanged	= false;
	m_bIsVisible		= false;
	m_nScaleType		= 0;
	m_GlobalTransform	= m_OriginTransform = m_OffsetTransform = m_TweenTransform = CTransform();
	m_TweenTransform.m_fScaleX = m_TweenTransform.m_fScaleY = 0.0f;
	m_GlobalTransformMatrix		= CMatrix();
}
//-------------------------------------------------------------------------
void CDragonBonesObject::Update()
{
	m_GlobalTransform.m_fScaleX	= ( m_OriginTransform.m_fScaleX + m_TweenTransform.m_fScaleX ) * m_OffsetTransform.m_fScaleX;
	m_GlobalTransform.m_fScaleY	= ( m_OriginTransform.m_fScaleY + m_TweenTransform.m_fScaleY ) * m_OffsetTransform.m_fScaleY;

	if( m_pParent )
	{
		f32 fX = m_OriginTransform.m_fPosX + m_OffsetTransform.m_fPosX + m_TweenTransform.m_fPosX;
		f32 fY = m_OriginTransform.m_fPosY + m_OffsetTransform.m_fPosY + m_TweenTransform.m_fPosY;
		CMatrix tagParentMatrix = m_pParent->m_GlobalTransformMatrix;

		m_GlobalTransformMatrix.m_fX = m_GlobalTransform.m_fPosX = tagParentMatrix.m_fA * fX + tagParentMatrix.m_fC * fY + tagParentMatrix.m_fX;
		m_GlobalTransformMatrix.m_fY = m_GlobalTransform.m_fPosY = tagParentMatrix.m_fD * fY + tagParentMatrix.m_fB * fX + tagParentMatrix.m_fY;

		if( m_bFixedRotation )
		{
			m_GlobalTransform.m_fSkewX = m_OriginTransform.m_fSkewX + m_OffsetTransform.m_fSkewX + m_TweenTransform.m_fSkewX;
			m_GlobalTransform.m_fSkewY = m_OriginTransform.m_fSkewY + m_OffsetTransform.m_fSkewY + m_TweenTransform.m_fSkewY;
		}
		else
		{
			m_GlobalTransform.m_fSkewX = m_OriginTransform.m_fSkewX + m_OffsetTransform.m_fSkewX + m_TweenTransform.m_fSkewX + m_pParent->m_GlobalTransform.m_fSkewX;
			m_GlobalTransform.m_fSkewY = m_OriginTransform.m_fSkewY + m_OffsetTransform.m_fSkewY + m_TweenTransform.m_fSkewY + m_pParent->m_GlobalTransform.m_fSkewY;
		}

		if( m_pParent->m_nScaleMode >= m_nScaleType )
		{
			m_GlobalTransform.m_fScaleX *= m_pParent->m_GlobalTransform.m_fScaleX;
			m_GlobalTransform.m_fScaleY *= m_pParent->m_GlobalTransform.m_fScaleY;
		}
	}
	else
	{
		m_GlobalTransformMatrix.m_fX	= m_GlobalTransform.m_fPosX = m_OriginTransform.m_fPosX + m_OffsetTransform.m_fPosX + m_TweenTransform.m_fPosX;
		m_GlobalTransformMatrix.m_fY	= m_GlobalTransform.m_fPosY = m_OriginTransform.m_fPosY + m_OffsetTransform.m_fPosY + m_TweenTransform.m_fPosY;
		m_GlobalTransform.m_fSkewX		= m_OriginTransform.m_fSkewX + m_OffsetTransform.m_fSkewX + m_TweenTransform.m_fSkewX;
		m_GlobalTransform.m_fSkewY		= m_OriginTransform.m_fSkewY + m_OffsetTransform.m_fSkewY + m_TweenTransform.m_fSkewY;
	}

	m_GlobalTransformMatrix.m_fA	= m_GlobalTransform.m_fScaleX * cos( m_GlobalTransform.m_fSkewY );
	m_GlobalTransformMatrix.m_fB	= m_GlobalTransform.m_fScaleX * sin( m_GlobalTransform.m_fSkewY );
	m_GlobalTransformMatrix.m_fC	= -m_GlobalTransform.m_fScaleY * sin( m_GlobalTransform.m_fSkewX );
	m_GlobalTransformMatrix.m_fD	= m_GlobalTransform.m_fScaleY * cos( m_GlobalTransform.m_fSkewX );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------