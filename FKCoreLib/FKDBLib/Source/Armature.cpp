//-------------------------------------------------------------------------
#include "../Include/Armature.h"
#include "../Include/Animation.h"
#include "../Include/Slot.h"
#include "../Include/Bone.h"
#include "../Include/ArmatureEvent.h"
#include "../Interface/IDisplayBridge.h"
#include "../Include/Frame.h"
#include "../Include/FrameEvent.h"
#include "../Include/SoundEvent.h"
#include "../Include/SoundEventManager.h"
#include "../Include/AnimationState.h"
#include <algorithm>
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

CArmature::CArmature( IObject* p_pDisplay )
{
	m_pDisplay				= p_pDisplay;
	m_pAnimation			= new CAnimation( this );
	m_bIsSlotsZOrderChanged = false;
}
//-------------------------------------------------------------------------
CArmature::~CArmature()
{
	Clear();
}
//-------------------------------------------------------------------------
void CArmature::Clear()
{
	if( m_pAnimation == NULL )
		return;

	m_pAnimation->Clear();

	for( unsigned int i = 0; i < m_vecSlotList.size(); ++i )
	{
		m_vecSlotList[i]->Clear();
	}
	for( unsigned int i = 0; i < m_vecBoneList.size(); ++i )
	{
		m_vecBoneList[i]->Clear();
	}
	for( unsigned int i = 0; i < m_vecSlotList.size(); ++i )
	{
		SAFE_DELETE( m_vecSlotList[i] );
	}
	for( unsigned int i = 0; i < m_vecBoneList.size(); ++i )
	{
		SAFE_DELETE( m_vecBoneList[i] );
	}
	m_vecBoneList.clear();
	m_vecSlotList.clear();

	for( unsigned int i = 0; i < m_vecEventList.size(); ++i )
	{
		SAFE_DELETE( m_vecEventList[i] );
	}
	m_vecEventList.clear();

	SAFE_DELETE( m_pAnimation );
	SAFE_DELETE( m_pDisplay );
}
//-------------------------------------------------------------------------
void CArmature::Update( f32 p_fPassedTime )
{
	int i						= 0;
	CSlot* pSlot				= NULL;
	CArmature* pChildArmature	= NULL;
	if(( m_pAnimation->m_bIsPlaying && m_pAnimation->m_bIsActive ) )
	{
		m_pAnimation->Update( p_fPassedTime );
		p_fPassedTime *= m_pAnimation->m_fTimeScale;

		i = m_vecBoneList.size();
		while( i-- )
		{
			m_vecBoneList[i]->Update();
		}

		i = m_vecSlotList.size();
		while( i-- )
		{
			pSlot = m_vecSlotList[i];
			pSlot->Update();
			if( pSlot->m_bIsDisplayOnStage )
			{
				pChildArmature = pSlot->GetChildArmature();
				if( pChildArmature )
				{
					pChildArmature->Update( p_fPassedTime );
				}
			}
		}

		if( m_bIsSlotsZOrderChanged )
		{
			UpdateSlotsZOrder();

			if( IsHadEventListener( CArmatureEvent::Z_ORDER_UPDATED ) )
			{
				DispatchEvent( new CArmatureEvent(CArmatureEvent::Z_ORDER_UPDATED) );
			}
		}

		if( !m_vecEventList.empty() )
		{
			for( unsigned int i = 0; i < m_vecEventList.size(); ++i )
			{
				DispatchEvent( m_vecEventList[i] );
			}
			m_vecEventList.clear();
		}
	}
	else
	{
		p_fPassedTime *= m_pAnimation->m_fTimeScale;
		i = m_vecSlotList.size();
		while( i-- )
		{
			pSlot = m_vecSlotList[i];
			if( pSlot->m_bIsDisplayOnStage )
			{
				pChildArmature = pSlot->GetChildArmature();
				if( pChildArmature )
				{
					pChildArmature->Update( p_fPassedTime );
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void CArmature::UpdateSlotsZOrder()
{
	std::sort( m_vecSlotList.begin(), m_vecSlotList.end(), SortSlotFunc  );
	int i = m_vecSlotList.size();
	CSlot* pSlot = NULL;
	while( i-- )
	{
		pSlot = m_vecSlotList[i];
		if( pSlot->m_bIsDisplayOnStage )
		{
			pSlot->m_pDisplayBridge->AddDisplay( m_pDisplay );
		}
	}

	m_bIsSlotsZOrderChanged = false;
}
//-------------------------------------------------------------------------
void CArmature::ArriveAtFrame( CFrame* p_pFrame, CTimeLineState* p_pTimeLineState, 
								  CAnimationState* p_pAnimationState, bool p_bIsCross )
{
	if( !p_pFrame->m_szEvent.empty() && IsHadEventListener( CFrameEvent::ANIMATION_FRAME_EVENT ) )
	{
		CFrameEvent* pEvent = new CFrameEvent( CFrameEvent::ANIMATION_FRAME_EVENT );
		pEvent->m_pAnimationState	= p_pAnimationState;
		pEvent->m_szFrameLabel		= p_pFrame->m_szEvent;
		m_vecEventList.push_back( pEvent );
	}

	if( !p_pFrame->m_szSound.empty() && CSoundEventManager::GetInstance()->IsHadEventListener( CSoundEvent::SOUND ) )
	{
		CSoundEvent* pEvent = new CSoundEvent( CSoundEvent::SOUND );
		pEvent->m_pArmature = this;
		pEvent->m_pAnimationState = p_pAnimationState;
		pEvent->m_szSound	= p_pFrame->m_szSound;
		CSoundEventManager::GetInstance()->DispatchEvent( pEvent );
	}

	if( !p_pFrame->m_szAction.empty() )
	{
		if( p_pAnimationState->m_bIsPlaying && !p_pAnimationState->m_bIsComplete )
		{
			m_pAnimation->GotoAndPlay( p_pFrame->m_szAction );
		}
	}
}
//-------------------------------------------------------------------------
vector<CSlot*> CArmature::GetSlots()
{
	return m_vecSlotList;
}
//-------------------------------------------------------------------------
vector<CBone*> CArmature::GetBones()
{
	return m_vecBoneList;
}
//-------------------------------------------------------------------------
CSlot* CArmature::GetSlotByName( const string& p_szSlotName )
{
	int i = m_vecSlotList.size();
	while( i-- )
	{
		if( m_vecSlotList[i]->m_szName == p_szSlotName )
		{
			return m_vecSlotList[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
CSlot* CArmature::GetSlotByDisplay( IObject* p_pDisplay )
{
	if( p_pDisplay == NULL )
		return NULL;

	int i = m_vecSlotList.size();
	while( i-- )
	{
		if( m_vecSlotList[i]->GetDisplay() == p_pDisplay )
		{
			return m_vecSlotList[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
void CArmature::RemoveSlot( CSlot* p_pSlot )
{
	if( p_pSlot == NULL )
	{
		//throw std::invalid_argument("RemoveSlot Slot is null.");
		//android 平台不支持c++11特性
		printf("RemoveSlot Slot is null.");
	}

	if( GetVectorItemIndex( m_vecSlotList, p_pSlot ) >= 0 )
	{
		p_pSlot->m_pParent->RemoveChild( p_pSlot );
	}
	else
	{
		//throw std::invalid_argument("RemoveSlot Slot is null.");
		//android 平台不支持c++11特性
		printf("RemoveSlot Slot is null.");
	}
}
//-------------------------------------------------------------------------
void CArmature::RemoveSlotByName( const string& p_szSlotName )
{
	if( p_szSlotName.empty() )
		return;

	CSlot* pSlot = GetSlotByName( p_szSlotName );
	if( pSlot )
	{
		RemoveSlot( pSlot );
	}
}
//-------------------------------------------------------------------------
CBone* CArmature::GetBoneByName( const string& p_szBoneName )
{
	int i = m_vecBoneList.size();
	while( i-- )
	{
		if( m_vecBoneList[i]->m_szName == p_szBoneName )
		{
			return m_vecBoneList[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
CBone* CArmature::GetBoneByDisplay( IObject* p_pDisplay )
{
	CSlot* pSlot = GetSlotByDisplay( p_pDisplay );
	if( pSlot == NULL )
		return NULL;

	return pSlot->m_pParent;
}
//-------------------------------------------------------------------------
void CArmature::RemoveBone( CBone* p_pBone )
{
	if( p_pBone == NULL )
	{
		//throw std::invalid_argument("RemoveSlot Bone is null.");
		//android 平台不支持c++11特性
		printf("RemoveSlot Bone is null.");
	}

	if( GetVectorItemIndex( m_vecBoneList, p_pBone ) >= 0 )
	{
		if( p_pBone->m_pParent )
		{
			p_pBone->m_pParent->RemoveChild( p_pBone );
		}
		else
		{
			p_pBone->SetArmature( NULL );
		}
	}
	else
	{
		//throw std::invalid_argument("RemoveSlot Bone can't find.");
		//android 平台不支持c++11特性
		printf("RemoveSlot Bone  can't find.");
	}
}
//-------------------------------------------------------------------------
void CArmature::RemoveBoneByName( const string& p_szBoneName )
{
	if( p_szBoneName.empty() )
		return;

	CBone* pBone = GetBoneByName( p_szBoneName );
	if( pBone )
	{
		RemoveBone( pBone );
	}
}
//-------------------------------------------------------------------------
void CArmature::AddBone( CBone* p_pBone, const string& p_szParentname )
{
	AddChild( p_pBone, p_szParentname );
}
//-------------------------------------------------------------------------
void CArmature::AddChild( CDragonBonesObject* p_pObject, const string& p_szParentName )
{
	if( p_pObject == NULL )
	{
		//throw std::invalid_argument("AddChild Bone is null.");
		//android 平台不支持c++11特性
		printf("AddChild Bone is null.");
	}


	if( !p_szParentName.empty() )
	{
		CBone* pBoneParent = GetBoneByName( p_szParentName );
		if( pBoneParent )
		{
			pBoneParent->AddChild( p_pObject );
		}
		else
		{
			//throw std::invalid_argument("AddChild ParentName bone is empty.");
			//android 平台不支持c++11特性
			printf("AddChild ParentName bone is empty.");
		}
	}
	else
	{
		if( p_pObject->m_pParent )
		{
			p_pObject->m_pParent->RemoveChild( p_pObject );
		}
		p_pObject->SetArmature( this );
	}
}
//-------------------------------------------------------------------------
void CArmature::AddDragonBonesObject( CDragonBonesObject* p_pObject )
{
	if( dynamic_cast<CSlot*>(p_pObject) )
	{
		CSlot* pSlot = static_cast<CSlot*>( p_pObject );
		if( GetVectorItemIndex( m_vecSlotList, pSlot ) < 0 )
		{
			m_vecSlotList.push_back( pSlot );
		}
	}
	else if( dynamic_cast<CBone*>(p_pObject) )
	{
		CBone* pBone = static_cast<CBone*>( p_pObject );
		if( GetVectorItemIndex( m_vecBoneList, pBone ) < 0 )
		{
			m_vecBoneList.push_back( pBone );
			SortBoneList();
		}
	}
}
//-------------------------------------------------------------------------
void CArmature::RemoveDragonBonesObject( CDragonBonesObject* p_pObject )
{
	if( dynamic_cast<CSlot*>(p_pObject) )
	{
		CSlot* pSlot = static_cast<CSlot*>( p_pObject );
		int nIndex = GetVectorItemIndex( m_vecSlotList, pSlot );
		if( nIndex >= 0 )
		{
			m_vecSlotList.erase( m_vecSlotList.begin() + nIndex );
		}
	}
	else if( dynamic_cast<CBone*>(p_pObject) )
	{
		CBone* pBone = static_cast<CBone*>( p_pObject );
		int nIndex = GetVectorItemIndex( m_vecBoneList, pBone );
		if( nIndex >= 0 )
		{
			m_vecBoneList.erase( m_vecBoneList.begin() + nIndex );
		}
	}
}
//-------------------------------------------------------------------------
bool CArmature::SortBoneFunc( const std::pair<int, CBone*>&a, const std::pair<int, CBone*>&b )
{
	return a.first > b.first;
}
//-------------------------------------------------------------------------
void CArmature::SortBoneList()
{
	int i = m_vecBoneList.size();
	if( i == 0 )
		return;
	int		nLevel		= 0;
	CBone*	pBone		= NULL;
	CBone*	pParentBone = NULL;
	vector< std::pair<int, CBone*> >		vecTmpArray;

	while( i-- )
	{
		nLevel	= 0;
		pBone	= m_vecBoneList[i];
		pParentBone = pBone;
		while( pParentBone )
		{
			nLevel++;
			pParentBone = pParentBone->m_pParent;
		}
		vecTmpArray.push_back( std::make_pair( nLevel, pBone ) );
	}

	std::sort( vecTmpArray.begin(), vecTmpArray.end(), SortBoneFunc );

	i = vecTmpArray.size();
	while( i-- )
	{
		m_vecBoneList[i] = vecTmpArray[i].second;
	}
}
//-------------------------------------------------------------------------
bool CArmature::SortSlotFunc( CSlot* p_pSlotA, CSlot* p_pSlotB )
{
	return p_pSlotA->GetZOrder() < p_pSlotB->GetZOrder();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------