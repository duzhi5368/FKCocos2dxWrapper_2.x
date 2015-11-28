//-------------------------------------------------------------------------
#include "../Include/Bone.h"
#include "../Include/Slot.h"
#include "../Include/Armature.h"
#include "../Include/DargonBonesObject.h"
#include "../Interface/IDisplayBridge.h"
#include "../Include/AnimationState.h"
#include "../Include/TransformFrame.h"
#include "../Include/BaseMath.h"
#include "../Include/FrameEvent.h"
#include "../Include/SoundEvent.h"
#include "../Include/SoundEventManager.h"
#include "../Include/Animation.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

CBone::CBone()
	: m_nScaleMode( 1 )
	, m_pSlot( NULL )
{
	m_nScaleType = 2;
}
//-------------------------------------------------------------------------
CBone::~CBone()
{
	Clear();
}
//-------------------------------------------------------------------------
void CBone::Clear()
{
	CDragonBonesObject::Clear();

	int i = m_vecChildren.size();
	while( i-- )
	{
		m_vecChildren[i]->Clear();
	}
	m_vecChildren.clear();
	m_pSlot = NULL;
	m_TweenPivot = CPoint();
}
//-------------------------------------------------------------------------
bool CBone::IsContains( CDragonBonesObject* p_pChild )
{
	if( p_pChild == NULL )
	{
		//throw std::invalid_argument("IsContains CDragonBonesObject child is null!");
		//android 平台不支持c++11特性
		printf("IsContains CDragonBonesObject child is null!");
	}

	if( p_pChild == this )
		return false;

	CDragonBonesObject* pAncestor = p_pChild;
	while( !( pAncestor == this || pAncestor == NULL ) )
	{
		pAncestor = pAncestor->m_pParent;
	}
	return ( pAncestor == this );
}
//-------------------------------------------------------------------------
void CBone::AddChild( CDragonBonesObject* p_pChild )
{
	if( p_pChild == NULL )
	{
		//throw std::invalid_argument("AddChild CDragonBonesObject child is null!");
		//android 平台不支持c++11特性
		printf("AddChild CDragonBonesObject child is null!");
	}

	if( p_pChild == this || ( dynamic_cast<CBone*>(p_pChild) && static_cast<CBone*>(p_pChild)->IsContains(this) ) )
	{
		//throw std::invalid_argument("AddChild can't add self as a child!");
		//android 平台不支持c++11特性
		printf("AddChild can't add self as a child!");
	}

	if( p_pChild->m_pParent )
		p_pChild->m_pParent->RemoveChild( p_pChild );

	m_vecChildren.push_back( p_pChild );

	p_pChild->m_pParent = this;
	p_pChild->SetArmature( m_pArmature );

	if( !m_pSlot && dynamic_cast<CSlot*>(p_pChild) )
	{
		m_pSlot = static_cast<CSlot*>( p_pChild );
	}
}
//-------------------------------------------------------------------------
void CBone::RemoveChild( CDragonBonesObject* p_pChild )
{
	if( p_pChild == NULL )
	{
		//throw std::invalid_argument("RemoveChild CDragonBonesObject child is null!");
		//android 平台不支持c++11特性
		printf("RemoveChild CDragonBonesObject child is null!");
	}
		
	int nIndex = GetVectorItemIndex( m_vecChildren, p_pChild );
	if( nIndex < 0 )
	{
		//throw std::invalid_argument("RemoveChild CDragonBonesObject child can't find!");
		//android 平台不支持c++11特性
		printf("RemoveChild CDragonBonesObject child can't find!");
	}

	m_vecChildren.erase( m_vecChildren.begin() + nIndex );
	p_pChild->m_pParent = NULL;
	p_pChild->SetArmature( NULL );
	if( p_pChild == m_pSlot )
	{
		m_pSlot = NULL;
	}
}
//-------------------------------------------------------------------------
CArmature* CBone::GetChildArmature()
{
	if( m_pSlot == NULL )
		return NULL;

	return m_pSlot->GetChildArmature();
}
//-------------------------------------------------------------------------
void CBone::SetArmature( CArmature* p_pArmature )
{
	CDragonBonesObject::SetArmature( p_pArmature );
	int i = m_vecChildren.size();
	while( i -- )
	{
		m_vecChildren[i]->SetArmature( p_pArmature );
	}
}
//-------------------------------------------------------------------------
IObject* CBone::GetDisplay()
{
	if( m_pSlot == NULL )
		return NULL;

	return m_pSlot->GetDisplay();
}
//------------------------------------------------------------------------
void CBone::SetDisplay( IObject* p_pValue )
{
	if( m_pSlot )
		m_pSlot->SetDisplayValue( p_pValue );
}
//-------------------------------------------------------------------------
void CBone::SetVisible( bool p_bVisible )
{
	if( m_bIsVisible == p_bVisible )
		return;

	m_bIsVisible = p_bVisible;
	int i = m_vecChildren.size();
	while( i-- )
	{
		CDragonBonesObject* pChild = m_vecChildren[i];
		if( dynamic_cast<CSlot*>( pChild ) )
		{
			( static_cast<CSlot*>(pChild) )->UpdateVisible( p_bVisible );
		}
	}
}
//-------------------------------------------------------------------------
list<CSlot*> CBone::GetSlots()
{
	list<CSlot*> SlotList;
	int i = m_vecChildren.size();
	while( i-- )
	{
		if( dynamic_cast<CSlot*>(m_vecChildren[i]) )
		{
			SlotList.push_front( static_cast<CSlot*>(m_vecChildren[i]) );
		}
	}
	return SlotList;
}
//-------------------------------------------------------------------------
void CBone::ArriveAtFrame( CFrame* p_pFrame, CTimeLineState* p_pTimeLineState, 
							  CAnimationState* p_pAnimationState, bool p_bIsCross )
{
	if( p_pFrame != NULL )
	{
		int nMixingType = p_pAnimationState->GetMixingTransform( m_szName );
		if( p_pAnimationState->m_bIsDiaplayControl &&( nMixingType == 2 || nMixingType == -1 ) )
		{
			if( m_szDisplayController.empty() || m_szDisplayController == p_pAnimationState->m_szName )
			{
				CTransformFrame* pTranformFrame = static_cast<CTransformFrame*>( p_pFrame );
				if( m_pSlot != NULL )
				{
					int nDisplayIndex = pTranformFrame->m_nDisplayIndex;
					if( nDisplayIndex >= 0 )
					{
						if(( !IsNaN(pTranformFrame->m_fZOrder) ) &&
							( pTranformFrame->m_fZOrder != m_pSlot->m_fTweenZOrder ))
						{
							m_pSlot->m_fTweenZOrder		= pTranformFrame->m_fZOrder;
							m_pArmature->m_bIsSlotsZOrderChanged	= true;
						}
					}
					m_pSlot->ChangeDisplay( nDisplayIndex );
					m_pSlot->UpdateVisible( pTranformFrame->m_bIsVisible );
				}
			}
		}

		if( ( !p_pFrame->m_szEvent.empty() ) &&
			( m_pArmature->IsHadEventListener( CFrameEvent::BONE_FRAME_EVENT ) ) )
		{
			CFrameEvent* pFrameEvent		= new CFrameEvent( CFrameEvent::BONE_FRAME_EVENT );
			pFrameEvent->m_pBone			= this;
			pFrameEvent->m_pAnimationState	= p_pAnimationState;
			pFrameEvent->m_szFrameLabel		= p_pFrame->m_szEvent;
			m_pArmature->m_vecEventList.push_back( pFrameEvent );
		}

		if( ( !p_pFrame->m_szSound.empty() ) &&
			( CSoundEventManager::GetInstance()->IsHadEventListener( CSoundEvent::SOUND ) ) )
		{
			CSoundEvent* pSoundEvent		= new CSoundEvent( CSoundEvent::SOUND );
			pSoundEvent->m_pArmature		= m_pArmature;
			pSoundEvent->m_pAnimationState	= p_pAnimationState;
			pSoundEvent->m_szSound			= p_pFrame->m_szSound;
			CSoundEventManager::GetInstance()->DispatchEvent( pSoundEvent );
		}

		if( !p_pFrame->m_szAction.empty() )
		{
			for( unsigned int i = 0; i < m_vecChildren.size(); ++i )
			{
				CDragonBonesObject* pChild = m_vecChildren[i];
				if( dynamic_cast<CSlot*>( pChild ) )
				{
					CArmature* pChildArmature = (static_cast<CSlot*>(pChild))->GetChildArmature();
					if( pChildArmature )
					{
						pChildArmature->m_pAnimation->GotoAndPlay( p_pFrame->m_szAction );
					}
				}
			}
		}
	}
	else
	{
		if( m_pSlot != NULL )
			m_pSlot->ChangeDisplay( -1 );
	}
}
//-------------------------------------------------------------------------
void CBone::UpdateColor( f32 p_fA, f32 p_fR, f32 p_fG, f32 p_fB,
	f32 p_fAMultiplier, f32 p_fRMultiplier, f32 p_fGMultiplier, f32 p_fBMultiplier, bool p_bIsColorChanged )
{
	if( p_bIsColorChanged || m_bIsColorChanged )
	{
		m_pSlot->m_pDisplayBridge->UpdateColor( p_fA, p_fR, p_fG, p_fB, 
			p_fAMultiplier, p_fRMultiplier, p_fGMultiplier, p_fBMultiplier );
	}
	m_bIsColorChanged = p_bIsColorChanged;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------