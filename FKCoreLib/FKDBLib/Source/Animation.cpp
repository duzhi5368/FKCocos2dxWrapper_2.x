//-------------------------------------------------------------------------
#include "../Include/Animation.h"
#include "../Include/AnimationData.h"
#include "../Include/AnimationState.h"
#include "../Include/TimeLineState.h"
#include "../Include/Slot.h"
#include "../Include/Bone.h"
#include "../Include/Armature.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

const string CAnimation::NONE					= "none";
const string CAnimation::SAME_LAYER				= "sameLayer";
const string CAnimation::SAME_GROUP				= "sameGroup";
const string CAnimation::SAME_LAYER_AND_GROUP	= "sameLayerAndGroup";
const string CAnimation::ALL					= "all";

//-------------------------------------------------------------------------
CAnimation::CAnimation( CArmature* p_pArmature )
{
	m_pLastAnimationState = NULL;
	m_pArmature = p_pArmature;
	m_bIsPlaying = false;
	m_bIsActive = false;
	m_bIsTweenEnabled = true;
	m_fTimeScale = 1.0f;
}
//-------------------------------------------------------------------------
CAnimation::~CAnimation()
{
	Clear();
}
//-------------------------------------------------------------------------
void CAnimation::Clear()
{
	if( m_pArmature == NULL )
		return;

	Stop();

	int i = static_cast<int>( m_vecAnimationLayer.size() );
	while( i-- )
	{
		vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[i];
		int j = pVecAnimationStateList->size();
		while( j-- )
		{
			CAnimationState::ReturnObject( pVecAnimationStateList->at(j) );
		}
		pVecAnimationStateList->clear();
		delete pVecAnimationStateList;
	}

	m_vecAnimationLayer.clear();
	m_vecAnimationList.clear();
	m_pArmature = NULL;
	m_vecAnimationDataList.clear();
}
//-------------------------------------------------------------------------
CAnimationState* CAnimation::GotoAndPlay( const string& p_szAnimationName, f32 p_fFadeInTime,
								f32 p_fDuration, f32 p_fLoop, uint p_unLayer, const string& p_szGroup,
								const string& p_szFadeOutMode, bool p_bDisplayControl, 
								bool p_bPauseFadeOut, bool p_bPauseFadeIn )
{
	if( m_vecAnimationDataList.empty() )
		return NULL;

	// 查找该动作信息
	int i = m_vecAnimationDataList.size();
	CAnimationData* pAnimationData = NULL;
	while( i-- )
	{
		if( m_vecAnimationDataList.at(i)->m_szName == p_szAnimationName )
		{
			pAnimationData = m_vecAnimationDataList[i];
			break;
		}
	}
	if( pAnimationData == NULL )
		return NULL;

	m_bIsPlaying = true;

	p_fFadeInTime = ( p_fFadeInTime < 0 ) ? ( pAnimationData->m_fFadeInTime < 0 ? 0.3f : pAnimationData->m_fFadeInTime ) : p_fFadeInTime;

	f32 fDurationScale = 0.0f;
	if( p_fDuration < 0.0f )
	{
		fDurationScale = pAnimationData->m_fScale < 0.0f ? 1.0f : pAnimationData->m_fScale;
	}
	else
	{
		fDurationScale = p_fDuration /  pAnimationData->m_fDuration;
	}

	p_fLoop = IsNaN( p_fLoop ) ? pAnimationData->m_nLoop : p_fLoop;
	p_unLayer = AddLayer( p_unLayer );


	CAnimationState* pAnimationState = NULL;
	if( p_szFadeOutMode == NONE )
	{

	}
	else if( p_szFadeOutMode == SAME_LAYER )
	{
		vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[p_unLayer];
		int j = pVecAnimationStateList->size();
		while( j-- )
		{
			pAnimationState = pVecAnimationStateList->at( j );
			pAnimationState->FadeOut( p_fFadeInTime, p_bPauseFadeOut );
		}
	}
	else if( p_szFadeOutMode == SAME_GROUP )
	{
		int j = m_vecAnimationLayer.size();
		while( j-- )
		{
			vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[p_unLayer];
			i = pVecAnimationStateList->size();
			while( i-- )
			{
				pAnimationState = pVecAnimationStateList->at( i );
				if( pAnimationState->m_szGroup == p_szGroup )
				{
					pAnimationState->FadeOut( p_fFadeInTime, p_bPauseFadeOut );
				}
			}
		}
	}
	else if( p_szFadeOutMode == ALL )
	{
		int j = m_vecAnimationLayer.size();
		while( j-- )
		{
			vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[p_unLayer];
			i = pVecAnimationStateList->size();
			while( i-- )
			{
				pAnimationState = pVecAnimationStateList->at( i );
				pAnimationState->FadeOut( p_fFadeInTime, p_bPauseFadeOut );
			}
		}
	}
	else
	{
		vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[p_unLayer];
		i = pVecAnimationStateList->size();
		while( i-- )
		{
			pAnimationState = pVecAnimationStateList->at( i );
			if( pAnimationState->m_szGroup == p_szGroup )
			{
				pAnimationState->FadeOut( p_fFadeInTime, p_bPauseFadeOut );
			}
		}
	}

	m_pLastAnimationState						= pAnimationState->BorrowObject();
	m_pLastAnimationState->m_szGroup			= p_szGroup;
	m_pLastAnimationState->m_bIsTweenEnabled	= m_bIsTweenEnabled;
	m_pLastAnimationState->FadeIn( m_pArmature, pAnimationData, p_fFadeInTime, 
		1 / fDurationScale, static_cast<int>( p_fLoop ), p_unLayer, p_bDisplayControl, p_bPauseFadeOut );

	AddState( m_pLastAnimationState );

	vector<CSlot*>& vecSlotList = m_pArmature->m_vecSlotList;
	CSlot* pSlot = NULL;
	i  = vecSlotList.size();
	while( i-- )
	{
		pSlot = vecSlotList[i];
		if( pSlot->GetChildArmature() && pSlot->GetChildArmature()->m_pAnimation )
		{
			pSlot->GetChildArmature()->m_pAnimation->GotoAndPlay( p_szAnimationName, p_fFadeInTime );
		}
	}

	m_pLastAnimationState->Update( 0.0f );
	return m_pLastAnimationState;
}
//-------------------------------------------------------------------------
void CAnimation::Play()
{
	if( m_vecAnimationDataList.empty() )
		return;
	if( !m_pLastAnimationState  )
	{
		GotoAndPlay( m_vecAnimationDataList[0]->m_szName );
	}
	else if( !m_bIsPlaying )
	{
		m_bIsPlaying = true;
	}
	else 
	{
		GotoAndPlay( m_pLastAnimationState->m_szName );
	}
}
//-------------------------------------------------------------------------
void CAnimation::Stop()
{
	m_bIsPlaying = false;
}
//-------------------------------------------------------------------------
void CAnimation::Update( f32 p_fPassedTime )
{
	p_fPassedTime *= m_fTimeScale;

	int l = m_pArmature->m_vecBoneList.size();
	int i, j, k;
	i = j = 0; k = l;
	uint unStateListLength = 0;
	CBone* pBone = NULL;
	string szBoneName;
	f32 fWeightLeft = 0.0f;

	f32 fX, fY, fSkewX, fSkewY, fScaleX, fScaleY, fPivotX, fPivotY;
	f32 fLayerTotalWeight = 0.0f;
	CAnimationState* pAnimationState = NULL;
	CTimeLineState* pTimeLineSate = NULL;
	f32 fWeight = 0.0f;
	CTransform* pTransform = NULL;
	CPoint* pPivot = NULL;

	l--;
	while( k-- )
	{
		pBone = m_pArmature->m_vecBoneList[k];
		szBoneName = pBone->m_szName;
		fWeightLeft = 1.0f;

		fX = fY = fSkewX = fSkewY = fScaleX = fScaleY = fPivotX = fPivotY = 0.0f;

		i = m_vecAnimationLayer.size();
		while( i-- )
		{
			fLayerTotalWeight = 0.0f;
			vector<CAnimationState*>* pVecAnimationStateList = m_vecAnimationLayer[i];
			unStateListLength = pVecAnimationStateList->size();
			for( j = 0; j < static_cast<int>(unStateListLength); ++j )
			{
				pAnimationState = pVecAnimationStateList->at( j );
				if( k == l )
				{
					if( pAnimationState->Update( p_fPassedTime ) )
					{
						RemoveState( pAnimationState );
						j--;
						unStateListLength--;
						continue;
					}
				}

				pTimeLineSate = pAnimationState->m_mapTimeLineStates[szBoneName];

				if( pTimeLineSate && pTimeLineSate->m_bIsTweenActive )
				{
					fWeight		= pAnimationState->m_fFadeWeight * pAnimationState->m_fWeight * fWeightLeft;
					pTransform	= &pTimeLineSate->m_Transform;
					pPivot		= &pTimeLineSate->m_Pivot;

					fX			+= pTransform->m_fPosX * fWeight;
					fY			+= pTransform->m_fPosY * fWeight;
					fSkewX		+= pTransform->m_fSkewX * fWeight;
					fSkewY		+= pTransform->m_fSkewY * fWeight;
					fScaleX		+= pTransform->m_fScaleX * fWeight;
					fScaleY		+= pTransform->m_fScaleY * fWeight;
					fPivotX		+= pPivot->m_fX * fWeight;
					fPivotY		+= pPivot->m_fY * fWeight;

					fLayerTotalWeight += fWeight;
				}
			}

			if( fLayerTotalWeight >= fWeightLeft )
			{
				break;
			}
			else
			{
				fWeightLeft -= fLayerTotalWeight;
			}
		}

		pTransform	= &pBone->m_TweenTransform;
		pPivot		= &pBone->m_TweenPivot;

		pTransform->m_fPosX		= fX;
		pTransform->m_fPosY		= fY;
		pTransform->m_fSkewX	= fSkewX;
		pTransform->m_fSkewY	= fSkewY;
		pTransform->m_fScaleX	= fScaleX;
		pTransform->m_fScaleY	= fScaleY;

		pPivot->m_fX			= fPivotX;
		pPivot->m_fY			= fPivotY;
	}
}
//-------------------------------------------------------------------------
CAnimationState* CAnimation::GetStateByName( const string& p_szName, uint p_unLayer )
{
	int l = m_vecAnimationLayer.size();
	if( l == NULL )
		return NULL;
	if( static_cast<int>( p_unLayer ) >= l )
		p_unLayer = l - 1;

	vector< CAnimationState* >* pAnimationStateList = m_vecAnimationLayer[p_unLayer];
	if( pAnimationStateList == NULL )
		return NULL;

	int i = pAnimationStateList->size();
	while( i-- )
	{
		if( pAnimationStateList->at(i)->m_szName == p_szName )
		{
			return pAnimationStateList->at( i );
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
bool CAnimation::IsHasAnimation( const string& p_szAnimationName )
{
	int i = static_cast< int >( m_vecAnimationDataList.size() );
	while( i-- )
	{
		 if( m_vecAnimationDataList.at(i)->m_szName == p_szAnimationName )
		 {
			 return true;
		 }
	}
	return false;
}
//-------------------------------------------------------------------------
void CAnimation::SetActive( CAnimationState* p_pAnimationState, bool p_bActive )
{
	if( p_bActive )
	{
		m_bIsActive = true;
	}
	else
	{
		int i = m_vecAnimationLayer.size();
		int j = 0;
		while( i-- )
		{
			vector<CAnimationState*>* pVecAnimationStateList = m_vecAnimationLayer[i];
			j = pVecAnimationStateList->size();
			while( j-- )
			{
				if( pVecAnimationStateList->at(j)->m_bIsPlaying &&
					(!pVecAnimationStateList->at(j)->m_bIsComplete) )
				{
					return;
				}
			}
		}
		m_bIsActive = false;
	}
}
//-------------------------------------------------------------------------
uint CAnimation::AddLayer( uint p_unLayer )
{
	if( p_unLayer >= m_vecAnimationLayer.size() )
	{
		m_vecAnimationLayer.push_back( new vector<CAnimationState*>() );
	}
	return p_unLayer;
}
//-------------------------------------------------------------------------
void CAnimation::AddState( CAnimationState* p_pAnimationState )
{
	if( p_pAnimationState == NULL )
		return;
	vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[ p_pAnimationState->m_unLayer ];
	if( pVecAnimationStateList == NULL )
		return;
	pVecAnimationStateList->push_back( p_pAnimationState );
}
//-------------------------------------------------------------------------
void CAnimation::RemoveState( CAnimationState* p_pAnimationState )
{
	if( p_pAnimationState == NULL )
		return;
	int nLayer = static_cast<int>( p_pAnimationState->m_unLayer );
	vector< CAnimationState* >* pVecAnimationStateList = m_vecAnimationLayer[nLayer];
	if( pVecAnimationStateList != NULL )
	{
		pVecAnimationStateList->erase( pVecAnimationStateList->begin() + 
			GetVectorItemIndex( *pVecAnimationStateList, p_pAnimationState ) );
	}
	p_pAnimationState->ReturnObject( p_pAnimationState );

	if( pVecAnimationStateList->size() == 0 && ( nLayer == m_vecAnimationLayer.size() - 1 ) )
	{
		m_vecAnimationLayer.pop_back();
	}
}

//-------------------------------------------------------------------------
const vector<CAnimationData*> CAnimation::GetAnimationDataList()
{
	return m_vecAnimationDataList;
}
//-------------------------------------------------------------------------
void CAnimation::SetAnimationDataList( const vector<CAnimationData*>& p_Value )
{
	m_vecAnimationDataList = p_Value;
	m_vecAnimationList.clear();
	for( unsigned int i = 0; i < m_vecAnimationDataList.size(); ++i )
	{
		m_vecAnimationList.push_back( m_vecAnimationDataList[i]->m_szName );
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------