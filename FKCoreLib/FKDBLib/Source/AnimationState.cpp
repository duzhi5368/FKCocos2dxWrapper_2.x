//-------------------------------------------------------------------------
#include "../Include/AnimationState.h"
#include "../Include/TimeLineState.h"
#include "../Include/BaseMath.h"
#include "../Include/AnimationData.h"
#include "../Include/Armature.h"
#include "../Include/Bone.h"
#include "../Include/AnimationEvent.h"
#include "../Include/Animation.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vector<CAnimationState*>	CAnimationState::s_vecAnimationStatePool;
//-------------------------------------------------------------------------
CAnimationState* CAnimationState::BorrowObject()
{
	if( s_vecAnimationStatePool.empty() )
	{
		return new CAnimationState();
	}
	CAnimationState* pState = s_vecAnimationStatePool.back();
	if( pState == NULL )
	{
		return new CAnimationState();
	}
	s_vecAnimationStatePool.pop_back();
	return pState;
}
//-------------------------------------------------------------------------
void CAnimationState::ReturnObject( CAnimationState* p_pAnimationState )
{
	if( p_pAnimationState == NULL )
		return;
	p_pAnimationState->Clear();
	if( GetVectorItemIndex( s_vecAnimationStatePool, p_pAnimationState ) < 0 )
	{
		s_vecAnimationStatePool.push_back( p_pAnimationState );
	}
}
//-------------------------------------------------------------------------
void CAnimationState::ClearAll()
{
	int i = s_vecAnimationStatePool.size();
	while( i-- )
	{
		s_vecAnimationStatePool[i]->Clear();
	}
	s_vecAnimationStatePool.clear();
	CTimeLineState::ClearAll();
}
//-------------------------------------------------------------------------
void CAnimationState::SetCurrentTime( f32 p_fValue )
{
	if( p_fValue <  0 || IsNaN(p_fValue) )
	{
		p_fValue = 0.0f;
	}
	m_fCurrentTime = p_fValue;
}
//-------------------------------------------------------------------------
void CAnimationState::SetTimeScale( f32 p_fValue )
{
	if( p_fValue < 0 )
		p_fValue = 0.0f;
	else if( IsNaN( p_fValue ) )
		p_fValue = 1.0f;
	else if( IsEqual( p_fValue, FLOAT_INFINITE ))
		p_fValue = 1.0f;

	m_fTimeScale = p_fValue;
}
//-------------------------------------------------------------------------
CAnimationState::CAnimationState()
	: m_bIsTweenEnabled( false )
	, m_bIsBlend( false )
	, m_bIsFadeIn( false )
	, m_bIsFadeOut( false )
	, m_bIsPlaying( false )
	, m_bIsComplete( false )
	, m_bIsDiaplayControl( false )
{
	m_szGroup.clear();
	m_szName.clear();
	m_fWeight = m_fFadeWeight = m_fFadeInTime = m_fFadeOutTime = m_fFadeOutBeginTime =
		m_fFadeOutWeight = m_fTotalTime = m_fCurrentTime = m_fTimeScale = 0.0f;
	m_pArmature = NULL;
	m_pCurrentFrame = NULL;
	m_pClip = NULL;
	m_nFadeState = m_nPauseBeforeFadeInCompleteState = m_nLoop = m_nLoopCount = 0;
	m_unLayer = 0;
}
//-------------------------------------------------------------------------
CAnimationState::~CAnimationState()
{

}
//-------------------------------------------------------------------------
void CAnimationState::FadeIn( CArmature* p_pArmature, CAnimationData* p_pClip, f32 p_fFadeInTime,
		f32 p_fTimeScale, int p_nLoop, uint p_unLayer, bool p_bIsDisplayControl, bool p_bIsPauseBeforeFadeInComplete )
{
	if( p_pClip == NULL )
		return;
	m_pArmature			= p_pArmature;
	m_pClip				= p_pClip;
	m_szName			= p_pClip->m_szName;
	m_unLayer			= p_unLayer;
	m_fTotalTime		= p_pClip->m_fDuration;

	if( Round(p_pClip->m_fDuration * p_pClip->m_unFrameRate) < 2 || IsEqual( p_fTimeScale, FLOAT_INFINITE ) )
	{
		m_fTimeScale	= 1.0f;
		m_fCurrentTime	= m_fTotalTime;
		if( m_nLoop >= 0 )
		{
			m_nLoop = 1;
		}
		else
		{
			m_nLoop = -1;
		}
	}
	else
	{
		m_fTimeScale	= p_fTimeScale;
		m_fCurrentTime	= 0.0f;
		m_nLoop			= p_nLoop;
	}

	if( p_bIsPauseBeforeFadeInComplete )
	{
		m_nPauseBeforeFadeInCompleteState = -1;
	}
	else
	{
		m_nPauseBeforeFadeInCompleteState = 1;
	}


	m_fFadeInTime		= p_fFadeInTime * m_fTimeScale;

	m_nLoopCount		= -1;
	m_nFadeState		= 1;
	m_fFadeOutBeginTime	= 0.0f;
	m_fFadeOutWeight	= -1.0f;
	m_fFadeWeight		= 0.0f;
	m_bIsPlaying		= true;
	m_bIsComplete		= false;
	m_bIsFadeIn			= true;
	m_bIsFadeOut		= false;
	m_bIsDiaplayControl	= p_bIsDisplayControl;
	m_fWeight			= 1.0f;
	m_bIsBlend			= true;
	m_bIsTweenEnabled	= true;
	
	UpdateTimeLineStates();
}
//-------------------------------------------------------------------------
void CAnimationState::FadeOut( f32 p_fFadeOutTime, bool p_bPause )
{
	if( m_pArmature == NULL || m_fFadeOutWeight >= 0 )
	{
		return;
	}
	m_nFadeState		= -1;
	m_fFadeOutWeight	= m_fFadeWeight;
	m_fFadeOutTime		= p_fFadeOutTime * m_fTimeScale;
	m_fFadeOutBeginTime	= m_fCurrentTime;
	m_bIsPlaying		= !p_bPause;
	m_bIsFadeOut		= true;
	m_bIsDiaplayControl	= false;

	for( map<string, CTimeLineState*>::iterator Ite = m_mapTimeLineStates.begin();
		Ite != m_mapTimeLineStates.end();
		++Ite )
	{
		Ite->second->FadeOut();
	}
}
//-------------------------------------------------------------------------
void CAnimationState::Play()
{
	m_bIsPlaying = true;
}
//-------------------------------------------------------------------------
void CAnimationState::Stop()
{
	m_bIsPlaying = false;
}
//-------------------------------------------------------------------------
int CAnimationState::GetMixingTransform( const string& p_szTimeLineName )
{
	map<string, int>::iterator Ite = m_mapMixingTransforms.find( p_szTimeLineName );
	return Ite != m_mapMixingTransforms.end() ? Ite->second : -1;
}
//-------------------------------------------------------------------------
void CAnimationState::AddMixingTransform( const string& p_szTimeLineName, int p_nType, bool p_bResursive )
{
	if( m_pClip && m_pClip->GetTransformTimeLineByName( p_szTimeLineName ) )
	{
		if( p_bResursive )
		{
			int i = m_pArmature->m_vecBoneList.size();
			CBone* pBone = NULL;
			CBone* pCurrentBone = NULL;
			while( i-- )
			{
				pBone = m_pArmature->m_vecBoneList[i];
				if( pBone->m_szName == p_szTimeLineName )
				{
					pCurrentBone = pBone;
				}
				if( pCurrentBone && (pCurrentBone == pBone || pCurrentBone->IsContains(pBone)) )
				{
					m_mapMixingTransforms[pBone->m_szName] = p_nType;
				}
			}
		}
		else
		{
			m_mapMixingTransforms[ p_szTimeLineName ] = p_nType;
		}

		UpdateTimeLineStates();
	}
	else
	{
		printf("AddMixingTransform argument error.");
	}
}
//-------------------------------------------------------------------------
void CAnimationState::RemoveMixingTransform( const string& p_szTimeLineName, bool p_bResursive )
{
	if( !p_szTimeLineName.empty() )
	{
		if( p_bResursive )
		{
			int i = m_pArmature->m_vecBoneList.size();
			CBone* pBone = NULL;
			CBone* pCurrentBone = NULL;
			while( i-- )
			{
				pBone = m_pArmature->m_vecBoneList[i];
				if( pBone->m_szName == p_szTimeLineName )
				{
					pCurrentBone = pBone;
				}
				if( pCurrentBone && ( pCurrentBone == pBone || pCurrentBone->IsContains(pBone) ) )
				{
					map<string, int>::iterator Ite = m_mapMixingTransforms.find( pBone->m_szName );
					if( Ite != m_mapMixingTransforms.end() )
					{
						m_mapMixingTransforms.erase( Ite );
					}
				}
			}
		}
		else
		{
			map<string, int>::iterator Ite = m_mapMixingTransforms.find( p_szTimeLineName );
			if( Ite != m_mapMixingTransforms.end() )
			{
				m_mapMixingTransforms.erase( Ite );
			}
		}
	}

	UpdateTimeLineStates();
}
//-------------------------------------------------------------------------
bool CAnimationState::Update( f32 p_fPassedTime )
{
	CAnimateEvent* pEvent = NULL;
	bool bIsComplete = false;

	if( m_bIsFadeIn )
	{
		m_bIsFadeIn = false;
		m_pArmature->m_pAnimation->SetActive( this, true );
		if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_IN ) )
		{
			pEvent = new CAnimateEvent( CAnimateEvent::FADE_IN );
			pEvent->m_pAnimationState = this;
			m_pArmature->m_vecEventList.push_back( pEvent );
		}
	}

	if( m_bIsFadeOut )
	{
		m_bIsFadeOut = false;
		m_pArmature->m_pAnimation->SetActive( this, true );
		if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_OUT ) )
		{
			pEvent = new CAnimateEvent( CAnimateEvent::FADE_OUT );
			pEvent->m_pAnimationState = this;
			m_pArmature->m_vecEventList.push_back( pEvent );
		}
	}

	m_fCurrentTime += p_fPassedTime * m_fTimeScale;

	if( m_bIsPlaying && (!m_bIsComplete) && ( m_nPauseBeforeFadeInCompleteState != 0 ) )
	{
		f32 fProgress = 0.0f;
		int nCurrentLoopCount = 0;
		if( m_nPauseBeforeFadeInCompleteState == -1 )
		{
			m_nPauseBeforeFadeInCompleteState = 0;
			fProgress = 0.0f;
			nCurrentLoopCount = static_cast<int>( fProgress );
		}
		else
		{
			fProgress = m_fCurrentTime / m_fTotalTime;
			nCurrentLoopCount = static_cast<int>( fProgress );
			if( nCurrentLoopCount != m_nLoopCount )
			{
				if( m_nLoopCount == -1 )
				{
					m_pArmature->m_pAnimation->SetActive( this, true );
					if( m_pArmature->IsHadEventListener( CAnimateEvent::START ) )
					{
						pEvent = new CAnimateEvent( CAnimateEvent::START );
						pEvent->m_pAnimationState = this;
						m_pArmature->m_vecEventList.push_back( pEvent );
					}
				}

				m_nLoopCount = nCurrentLoopCount;

				if( m_nLoopCount != 0 )
				{
					if(( m_nLoop != 0 ) && (m_nLoopCount * m_nLoopCount >= m_nLoop * m_nLoop - 1 ))
					{
						m_bIsComplete = true;
						fProgress = 1;
						nCurrentLoopCount = 0;
						if( m_pArmature->IsHadEventListener( CAnimateEvent::COMPLETE ) )
						{
							pEvent = new CAnimateEvent( CAnimateEvent::COMPLETE );
							pEvent->m_pAnimationState = this;
							m_pArmature->m_vecEventList.push_back( pEvent );
						}
					}
					else
					{
						if( m_pArmature->IsHadEventListener( CAnimateEvent::LOOP_COMPLETE ) )
						{
							pEvent = new CAnimateEvent( CAnimateEvent::LOOP_COMPLETE );
							pEvent->m_pAnimationState = this;
							m_pArmature->m_vecEventList.push_back( pEvent );
						}
					}
				}
			}
		}


		for( map<string, CTimeLineState*>::iterator Ite = m_mapTimeLineStates.begin();
			Ite != m_mapTimeLineStates.end();
			++Ite )
		{
			Ite->second->Update( fProgress );
		}

		if( m_pClip->m_vecFrameList.size() > 0 )
		{
			f32 fPlayedTime = m_fTotalTime * ( fProgress - nCurrentLoopCount );
			bool bIsArrivedFrame = false;
			int nFrameIndex = 0;

			while( !m_pCurrentFrame || fPlayedTime > m_pCurrentFrame->m_fPos + m_pCurrentFrame->m_fDuration ||
				fPlayedTime < m_pCurrentFrame->m_fPos )
			{
				if( bIsArrivedFrame )
				{
					m_pArmature->ArriveAtFrame( m_pCurrentFrame, 0, this, true );
				}
				bIsArrivedFrame = true;

				if( m_pCurrentFrame )
				{
					nFrameIndex = GetVectorItemIndex( m_pClip->m_vecFrameList, m_pCurrentFrame );
					nFrameIndex++;
					if( nFrameIndex >= static_cast<int>( m_pClip->m_vecFrameList.size() ) )
					{
						nFrameIndex = 0;
					}
					m_pCurrentFrame = m_pClip->m_vecFrameList[nFrameIndex];
				}
				else
				{
					m_pCurrentFrame = m_pClip->m_vecFrameList[0];
				}
			}

			if( bIsArrivedFrame )
			{
				m_pArmature->ArriveAtFrame( m_pCurrentFrame, 0, this, false );
			}
		}
	}

	if( m_nFadeState > 0 )
	{
		if( IsEqual( m_fFadeInTime, 0.0f ) )
		{
			m_fFadeWeight	= 1.0f;
			m_nFadeState	= 0;
			m_nPauseBeforeFadeInCompleteState = 1;
			m_pArmature->m_pAnimation->SetActive( this, false );

			if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_IN_COMPLETE ))
			{
				pEvent = new CAnimateEvent( CAnimateEvent::FADE_IN_COMPLETE );
				pEvent->m_pAnimationState = this;
				m_pArmature->m_vecEventList.push_back( pEvent );
			}
		}
		else
		{
			m_fFadeWeight = m_fCurrentTime / m_fFadeInTime;
			if( m_fFadeWeight >= 1.0f )
			{
				m_fFadeWeight	= 1.0f;
				m_nFadeState	= 0;
				if( m_nPauseBeforeFadeInCompleteState == 0 )
				{
					m_fCurrentTime -= m_fFadeInTime;
				}

				m_nPauseBeforeFadeInCompleteState = 1;
				m_pArmature->m_pAnimation->SetActive( this, false );
				if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_IN_COMPLETE ))
				{
					pEvent = new CAnimateEvent( CAnimateEvent::FADE_IN_COMPLETE );
					pEvent->m_pAnimationState = this;
					m_pArmature->m_vecEventList.push_back( pEvent );
				}
			}
		}
	}
	else if( m_nFadeState < 0 )
	{
		if( IsEqual( m_fFadeOutTime, 0.0f ) )
		{
			m_fFadeWeight	= 0.0f;
			m_nFadeState	= 0;
			m_pArmature->m_pAnimation->SetActive( this, false );

			if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_OUT_COMPLETE ))
			{
				pEvent = new CAnimateEvent( CAnimateEvent::FADE_OUT_COMPLETE );
				pEvent->m_pAnimationState = this;
				m_pArmature->m_vecEventList.push_back( pEvent );
			}
			return true;
		}
		else
		{
			m_fFadeWeight = ( 1 - ( m_fCurrentTime - m_fFadeOutBeginTime ) / m_fFadeOutTime ) * m_fFadeOutWeight;
			if( m_fFadeWeight <= 0.0f )
			{
				m_fFadeWeight	= 0.0f;
				m_nFadeState	= 0;
				m_pArmature->m_pAnimation->SetActive( this, false );

				if( m_pArmature->IsHadEventListener( CAnimateEvent::FADE_OUT_COMPLETE ))
				{
					pEvent = new CAnimateEvent( CAnimateEvent::FADE_OUT_COMPLETE );
					pEvent->m_pAnimationState = this;
					m_pArmature->m_vecEventList.push_back( pEvent );
				}
				return true;
			}	
		}
	}

	if( bIsComplete )
	{
		m_bIsComplete = true;
		if( m_nLoop < 0 )
		{
			FadeOut( ( m_fFadeOutWeight ? m_fFadeOutWeight : m_fFadeOutTime )/ m_fTimeScale, true );
		}
		else
		{
			m_pArmature->m_pAnimation->SetActive( this, false );
		}
	}

	return false;
}
//-------------------------------------------------------------------------
void CAnimationState::UpdateTimeLineStates()
{
	if( !m_mapMixingTransforms.empty() )
	{
		for( map<string, CTimeLineState*>::iterator Ite = m_mapTimeLineStates.begin();
			Ite != m_mapTimeLineStates.end(); )
		{
			if( m_mapMixingTransforms.find(Ite->first) == m_mapMixingTransforms.end() )
			{
				if( Ite != m_mapTimeLineStates.end() )
				{
					CTimeLineState::ReturnObject( static_cast<CTimeLineState*>(Ite->second) );
					Ite = m_mapTimeLineStates.erase( Ite );
				}
				else
				{
					Ite++;
				}
			}
		}

		for( map<string, int>::iterator Ite = m_mapMixingTransforms.begin();
			Ite != m_mapMixingTransforms.end();
			++Ite )
		{
			if( m_mapTimeLineStates.find(Ite->first) == m_mapTimeLineStates.end() )
			{
				AddTimeLineState( Ite->first );
			}
		}
	}
	else
	{
		for( map<string, CTimeLine*>::iterator Ite = m_pClip->m_mapTimeLines.begin();
			Ite != m_pClip->m_mapTimeLines.end();
			++Ite )
		{
			if( m_mapTimeLineStates.find(Ite->first) == m_mapTimeLineStates.end() )
			{
				AddTimeLineState( Ite->first );
			}
		}
	}
}
//-------------------------------------------------------------------------
void CAnimationState::AddTimeLineState( const string& p_szTimeLineName )
{
	CBone* pBone = m_pArmature->GetBoneByName( p_szTimeLineName );
	if( pBone )
	{
		CTimeLineState* pTimeLineState = CTimeLineState::BorrowObject();
		CTransformTimeLine* pTimeLine = m_pClip->GetTransformTimeLineByName( p_szTimeLineName );
		pTimeLineState->FadeIn( pBone, this, pTimeLine );
		m_mapTimeLineStates[p_szTimeLineName] = pTimeLineState;
	}
}
//-------------------------------------------------------------------------
void CAnimationState::RemoveTimeLineState( const string& p_szTimeLineName )
{
	map< string, CTimeLineState* >::iterator Ite = m_mapTimeLineStates.find( p_szTimeLineName );
	if( Ite != m_mapTimeLineStates.end() )
	{
		CTimeLineState::ReturnObject( static_cast<CTimeLineState*>(Ite->second ));
		m_mapTimeLineStates.erase( Ite );
	}
}
//-------------------------------------------------------------------------
void CAnimationState::Clear()
{
	m_pArmature			= NULL;
	m_pCurrentFrame		= NULL;
	m_pClip				= NULL;
	m_mapMixingTransforms.clear();

	for( map<string, CTimeLineState*>::iterator Ite = m_mapTimeLineStates.begin();
		Ite != m_mapTimeLineStates.end(); )
	{
		if( Ite != m_mapTimeLineStates.end() )
		{
			CTimeLineState::ReturnObject( static_cast<CTimeLineState*>(Ite->second) );
			Ite = m_mapTimeLineStates.erase( Ite );
		}
		else
		{
			++Ite;
		}
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------