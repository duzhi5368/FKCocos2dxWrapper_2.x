//-------------------------------------------------------------------------
#include "../Include/Slot.h"
#include "../Include/BaseMath.h"
#include "../Include/Armature.h"
#include "../Interface/IDisplayBridge.h"
#include "../Include/DisplayData.h"
#include "../Include/Bone.h"
#include "../Include/Matrix.h"
#include "../Include/Animation.h"
#include "../Include/AnimationState.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

CSlot::CSlot( IDisplayBridge* p_pDisplayBridge )
{
	m_pDisplayBridge	= p_pDisplayBridge;
	m_nDisplayIndex		= -1;
	m_nScaleType		= 1;
	m_fOffsetZOrder		= 0.0f;
	m_fTweenZOrder		= 0.0f;
	m_fOriginZOrder		= 0.0f;
	m_bIsDisplayOnStage	= false;
	m_bIsHideDisplay	= false;
	m_szBlendMode		= "normal";
	if( m_pDisplayBridge->GetDisplay() )
	{
		m_pDisplayBridge->UpdateBlendMode( m_szBlendMode );
	}
}
//-------------------------------------------------------------------------
CSlot::~CSlot()
{
	Clear();
}
//-------------------------------------------------------------------------
f32 CSlot::GetZOrder()
{
	return m_fOriginZOrder + m_fTweenZOrder + m_fOffsetZOrder;
}
//-------------------------------------------------------------------------
void CSlot::SetZOrder( f32 p_fValue )
{
	if( IsEqual( GetZOrder(), p_fValue ) )
		return;

	m_fOffsetZOrder = p_fValue - m_fOriginZOrder - m_fTweenZOrder;
	if( m_pArmature )
		m_pArmature->m_bIsSlotsZOrderChanged = true;
}
//-------------------------------------------------------------------------
string CSlot::GetBlendMode()
{
	return m_szBlendMode;
}
//-------------------------------------------------------------------------
void CSlot::SetBlendMode( const string& p_szMode )
{
	if( p_szMode == m_szBlendMode )
		return;

	m_szBlendMode = p_szMode;
	if( m_pDisplayBridge->GetDisplay() )
	{
		m_pDisplayBridge->UpdateBlendMode( m_szBlendMode );
	}
}
//-------------------------------------------------------------------------
IObject* CSlot::GetDisplay()
{
	IObject* pDisplay = m_vecDisplayList[m_nDisplayIndex];
	if( dynamic_cast<CArmature*>(pDisplay) )
	{
		return (static_cast<CArmature*>( pDisplay ))->m_pDisplay;
	}
	return pDisplay;
}
//-------------------------------------------------------------------------
void CSlot::SetDisplayValue( IObject* p_pValue )
{
	m_vecDisplayList[m_nDisplayIndex] = p_pValue;
	SetDisplay( p_pValue );
}
//-------------------------------------------------------------------------
CArmature* CSlot::GetChildArmature()
{
	if( dynamic_cast<CArmature*>(m_vecDisplayList[m_nDisplayIndex]) )
	{
		return (static_cast<CArmature*>(m_vecDisplayList[m_nDisplayIndex]));
	}
	return NULL;
}
//-------------------------------------------------------------------------
void CSlot::SetChildArmature( CArmature* p_pArmature )
{
	m_vecDisplayList[m_nDisplayIndex] = p_pArmature;
	if( p_pArmature != NULL )
		SetDisplay( p_pArmature->m_pDisplay );
}
//-------------------------------------------------------------------------
const vector<IObject*>& CSlot::GetDisplayList()
{
	return m_vecDisplayList;
}
//-------------------------------------------------------------------------
void CSlot::SetDisplayList( const vector<IObject*>& p_Value )
{
	int i = p_Value.size();
	m_vecDisplayList.resize( i );
	while( i-- )
	{
		m_vecDisplayList[i] = p_Value[i];
	}
	if( m_nDisplayIndex >= 0 )
	{
		int nDisplayIndexBackup = m_nDisplayIndex;
		m_nDisplayIndex = -1;
		ChangeDisplay( nDisplayIndexBackup );
	}
}
//-------------------------------------------------------------------------
void CSlot::SetDisplay( IObject* p_pDisplay )
{
	if( m_pDisplayBridge->GetDisplay() )
	{
		m_pDisplayBridge->SetDisplay( p_pDisplay );
	}
	else
	{
		m_pDisplayBridge->SetDisplay( p_pDisplay );
		if( m_pArmature )
		{
			m_pDisplayBridge->AddDisplay( m_pArmature->m_pDisplay );
			m_pArmature->m_bIsSlotsZOrderChanged = true;
		}
	}

	UpdateChlidArmatureAnimation();

	if( !m_bIsHideDisplay && m_pDisplayBridge->GetDisplay() )
	{
		m_bIsDisplayOnStage = true;
		m_pDisplayBridge->UpdateBlendMode( m_szBlendMode );
	}
	else
	{
		m_bIsDisplayOnStage = false;
	}
}
//-------------------------------------------------------------------------
void CSlot::ChangeDisplay( int p_nDisplayIndex )
{
	if( p_nDisplayIndex < 0 )
	{
		if( !m_bIsHideDisplay )
		{
			m_bIsHideDisplay = true;
			m_pDisplayBridge->RemoveDisplay();
			UpdateChlidArmatureAnimation();
		}
	}
	else
	{
		bool bIsChangeShowState = false;
		if( m_bIsHideDisplay )
		{
			m_bIsHideDisplay = false;
			bIsChangeShowState = true;
			if( m_pArmature )
			{
				m_pDisplayBridge->AddDisplay( m_pArmature->m_pDisplay );
				m_pArmature->m_bIsSlotsZOrderChanged = true;
			}
		}

		int nLength = m_vecDisplayList.size();
		if( p_nDisplayIndex >= nLength && nLength > 0 )
		{
			p_nDisplayIndex = nLength - 1;
		}
		if( m_nDisplayIndex != p_nDisplayIndex )
		{
			m_nDisplayIndex = p_nDisplayIndex;

			IObject* pContent = m_vecDisplayList[m_nDisplayIndex];
			if( dynamic_cast<CArmature*>(pContent) )
			{
				SetDisplay(( static_cast<CArmature*>(pContent) )->m_pDisplay );
			}
			else
			{
				SetDisplay( pContent );
			}

			if( !m_vecDisplayDataList.empty() && m_nDisplayIndex <= static_cast<int>(m_vecDisplayDataList.size()) )
			{
				m_OriginTransform = m_vecDisplayDataList[m_nDisplayIndex]->m_Transform;
			}
		}
		else if( bIsChangeShowState )
		{
			UpdateChlidArmatureAnimation();
		}
	}

	if( !m_bIsHideDisplay && m_pDisplayBridge->GetDisplay() )
	{
		m_bIsDisplayOnStage = true;
	}
	else
	{
		m_bIsDisplayOnStage = false;
	}
}
//-------------------------------------------------------------------------
void CSlot::UpdateVisible( bool p_bValue )
{
	m_pDisplayBridge->SetVisible( m_pParent->m_bIsVisible && m_bIsVisible && p_bValue );
}
//-------------------------------------------------------------------------
void CSlot::SetVisible( bool p_bValue )
{
	if( p_bValue != m_bIsVisible )
	{
		m_bIsVisible = p_bValue;
		UpdateVisible( m_bIsVisible );
	}
}
//-------------------------------------------------------------------------
void CSlot::SetArmature(  CArmature* p_pValue  )
{
	CDragonBonesObject::SetArmature( p_pValue );
	if( m_pArmature != NULL )
	{
		m_pArmature->m_bIsSlotsZOrderChanged = true;
		m_pDisplayBridge->AddDisplay( m_pArmature->m_pDisplay );
	}
	else
	{
		m_pDisplayBridge->RemoveDisplay();
	}
}
//-------------------------------------------------------------------------
void CSlot::Clear()
{
	if( m_pDisplayBridge == NULL )
		return;

	CDragonBonesObject::Clear();

	for( unsigned int i = 0; i < m_vecDisplayList.size(); ++i )
	{
		delete m_vecDisplayList[i];
	}
	m_vecDisplayList.clear();

	SAFE_DELETE( m_pDisplayBridge );
	m_vecDisplayDataList.clear();
}
//-------------------------------------------------------------------------
void CSlot::Update()
{
	// added by freeknight  ÓÅ»¯
	if( !m_pParent->m_bIsVisible || !m_bIsVisible )
	{
		return;
	}

	CDragonBonesObject::Update();

	if( m_bIsDisplayOnStage )
	{
		f32 fPivotX = m_pParent->m_TweenPivot.m_fX;
		f32 fPivotY = m_pParent->m_TweenPivot.m_fY;

		if( fPivotY || fPivotX )
		{
			CMatrix tagParentMatrix			= m_pParent->m_GlobalTransformMatrix;
			m_GlobalTransformMatrix.m_fX	+= tagParentMatrix.m_fA * fPivotX + tagParentMatrix.m_fC * fPivotY;
			m_GlobalTransformMatrix.m_fY	+= tagParentMatrix.m_fB * fPivotX + tagParentMatrix.m_fD * fPivotY;
		}

		m_pDisplayBridge->UpdateTransform( m_GlobalTransformMatrix, m_GlobalTransform );
	}
}
//-------------------------------------------------------------------------
void CSlot::UpdateChlidArmatureAnimation()
{
	CArmature* pChildArmature = GetChildArmature();
	if( pChildArmature )
	{
		if( m_bIsHideDisplay )
		{
			pChildArmature->m_pAnimation->Stop();
			pChildArmature->m_pAnimation->m_pLastAnimationState = NULL;
		}
		else
		{
			if( m_pArmature && m_pArmature->m_pAnimation->m_pLastAnimationState &&
				pChildArmature->m_pAnimation->IsHasAnimation( m_pArmature->m_pAnimation->m_pLastAnimationState->m_szName ) )
			{
				pChildArmature->m_pAnimation->GotoAndPlay( m_pArmature->m_pAnimation->m_pLastAnimationState->m_szName );
			}
			else
			{
				pChildArmature->m_pAnimation->Play();
			}
		}
	}
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------