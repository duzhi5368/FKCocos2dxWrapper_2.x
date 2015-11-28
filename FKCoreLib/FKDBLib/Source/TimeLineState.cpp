//-------------------------------------------------------------------------
#include "../Include/TimeLineState.h"
#include "../Include/BaseMacro.h"
#include "../Include/TransformTimeLine.h"
#include "../Include/Bone.h"
#include "../Include/BaseMath.h"
#include "../Include/TransformFrame.h"
#include "../Include/AnimationData.h"
//-------------------------------------------------------------------------
vector<CTimeLineState*>		CTimeLineState::s_vecTimeLineStatePool;
//-------------------------------------------------------------------------
CTimeLineState*	CTimeLineState::BorrowObject()
{
	if( s_vecTimeLineStatePool.empty() )
	{
		return new CTimeLineState;
	}
	CTimeLineState* pState = s_vecTimeLineStatePool.back();
	s_vecTimeLineStatePool.pop_back();
	return pState;
}
//-------------------------------------------------------------------------
void CTimeLineState::ReturnObject( CTimeLineState* p_pTimeLineState )
{
	if( GetVectorItemIndex( s_vecTimeLineStatePool, p_pTimeLineState ) < 0 )
	{
		s_vecTimeLineStatePool.push_back( p_pTimeLineState );
	}
	p_pTimeLineState->Clear();
}
//-------------------------------------------------------------------------
void CTimeLineState::ClearAll()
{
	int i = s_vecTimeLineStatePool.size();
	while( i-- )
	{
		s_vecTimeLineStatePool[i]->Clear();
	}
	s_vecTimeLineStatePool.clear();
}
//-------------------------------------------------------------------------
// CTimeLineState
//-------------------------------------------------------------------------
CTimeLineState::CTimeLineState()
{
	m_fCurrentFramePosition = 0.0f;
	m_fCurrentFrameDuration = 0.0f;
}
//-------------------------------------------------------------------------
CTimeLineState::~CTimeLineState()
{
	Clear();
}
//-------------------------------------------------------------------------
void CTimeLineState::FadeIn( CBone* p_pBone, CAnimationState* p_pAnimationState, CTransformTimeLine* p_pTimeLine )
{
	m_pBone				= p_pBone;
	m_pAnimationState	= p_pAnimationState;
	m_pTimeLine			= p_pTimeLine;
	m_OriginTransform	= m_pTimeLine->m_OriginTransform;
	m_OriginPivot		= m_pTimeLine->m_OriginPiovt;
	m_bIsTweenTransform	= false;
	m_bIsTweenColor		= false;
	m_fTotalTime		= m_pAnimationState->m_fTotalTime;
	m_pCurrentFrame		= NULL;
	m_Transform.m_fScaleX			= 0.0f;
	m_Transform.m_fScaleY			= 0.0f;
	m_Transform.m_fSkewX			= 0.0f;
	m_Transform.m_fSkewY			= 0.0f;
	m_Transform.m_fPosX				= 0.0f;
	m_Transform.m_fPosY				= 0.0f;
	m_Pivot.m_fX					= 0.0f;
	m_Pivot.m_fY					= 0.0f;
	m_DurationTransform.m_fScaleX	= 0.0f;
	m_DurationTransform.m_fScaleY	= 0.0f;
	m_DurationTransform.m_fSkewX	= 0.0f;
	m_DurationTransform.m_fSkewY	= 0.0f;
	m_DurationTransform.m_fPosX		= 0.0f;
	m_DurationTransform.m_fPosY		= 0.0f;
	m_DurationPivot.m_fX			= 0.0f;
	m_DurationPivot.m_fY			= 0.0f;

	switch (m_pTimeLine->m_vecFrameList.size())
	{
	case 0:
		m_pBone->ArriveAtFrame( 0, this, m_pAnimationState, false );
		m_nUpdateState = 0;
		break;
	case 1:
		m_nUpdateState = -1;
		break;
	default:
		m_nUpdateState = 1;
		break;
	}
}
//-------------------------------------------------------------------------
void CTimeLineState::FadeOut()
{
	m_Transform.m_fSkewX		= FormatRadian( m_Transform.m_fSkewX );
	m_Transform.m_fSkewY		= FormatRadian( m_Transform.m_fSkewY );
}
//-------------------------------------------------------------------------
void CTimeLineState::Update( f32 p_fPassedTime )
{
	if( m_nUpdateState == 0 )
		return;

	if( m_nUpdateState > 0 )
	{
		if( m_pTimeLine->m_fScale == 0 )
		{
			p_fPassedTime = 1;
		}
		else
		{
			p_fPassedTime /= m_pTimeLine->m_fScale;
		}

		if( p_fPassedTime == 1 )
		{
			p_fPassedTime= 0.999999f;
		}

		p_fPassedTime += m_pTimeLine->m_fOffset;
		int nLoopCount = static_cast< int >( p_fPassedTime );
		p_fPassedTime-= nLoopCount;

		f32 fPlayedTime = m_fTotalTime * p_fPassedTime;
		bool bIsArrivedFrame = false;
		int nFrameIndex = 0;

		while( !m_pCurrentFrame || fPlayedTime > m_fCurrentFramePosition + m_fCurrentFrameDuration || fPlayedTime < m_fCurrentFramePosition )
		{
			if( bIsArrivedFrame )
			{
				m_pBone->ArriveAtFrame( static_cast<CFrame*>(m_pCurrentFrame), this, m_pAnimationState, true );
			}

			bIsArrivedFrame = true;

			if( m_pCurrentFrame != NULL )
			{
				nFrameIndex = GetVectorItemIndex( m_pTimeLine->m_vecFrameList, static_cast<CFrame*>( m_pCurrentFrame ) ) + 1;
				if( nFrameIndex >= static_cast< int >( m_pTimeLine->m_vecFrameList.size() ) )
				{
					nFrameIndex = 0;
				}
				m_pCurrentFrame = static_cast< CTransformFrame* >( m_pTimeLine->m_vecFrameList[nFrameIndex] );
			}
			else
			{
				nFrameIndex = 0;
				m_pCurrentFrame = static_cast< CTransformFrame* >( m_pTimeLine->m_vecFrameList[0] );
			}

			m_fCurrentFrameDuration = m_pCurrentFrame->m_fDuration;
			m_fCurrentFramePosition = m_pCurrentFrame->m_fPos;
		}

		if( bIsArrivedFrame )
		{
			m_bIsTweenActive = ( m_pCurrentFrame->m_nDisplayIndex >= 0 );
			nFrameIndex++;
			if( nFrameIndex >= static_cast<int>( m_pTimeLine->m_vecFrameList.size() ) )
			{
				nFrameIndex = 0;
			}

			CTransformFrame* pNextFrame = static_cast<CTransformFrame*>( m_pTimeLine->m_vecFrameList[nFrameIndex] );
			if( nFrameIndex == 0 && m_pAnimationState->m_nLoop != 0 &&
				( m_pAnimationState->m_nLoopCount >=  abs( m_pAnimationState->m_nLoop ) - 1 ) &&
				((( m_fCurrentFramePosition + m_fCurrentFrameDuration ) / m_fTotalTime + nLoopCount - m_pTimeLine->m_fOffset ) * m_pTimeLine->m_fScale > 0.999999f ) )
			{
				m_nUpdateState = 0;
				m_fTweenEasing = NaN;
			}
			else if( m_pCurrentFrame->m_nDisplayIndex < 0 || pNextFrame->m_nDisplayIndex < 0 || !m_pAnimationState->m_bIsTweenEnabled )
			{
				m_fTweenEasing = NaN;
			}
			else if( IsNaN( m_pAnimationState->m_pClip->m_fTweenEasing ))
			{
				m_fTweenEasing = m_pCurrentFrame->m_fTweenEasing;
			}
			else
			{
				m_fTweenEasing = m_pAnimationState->m_pClip->m_fTweenEasing;
			}

			if( IsNaN( m_fTweenEasing ) )
			{
				m_bIsTweenTransform = false;
				m_bIsTweenColor = false;
			}
			else
			{
				m_DurationTransform.m_fPosX		= pNextFrame->m_Transform.m_fPosX - m_pCurrentFrame->m_Transform.m_fPosX;
				m_DurationTransform.m_fPosY		= pNextFrame->m_Transform.m_fPosY - m_pCurrentFrame->m_Transform.m_fPosY;
				m_DurationTransform.m_fSkewX	= pNextFrame->m_Transform.m_fSkewX - m_pCurrentFrame->m_Transform.m_fSkewX;
				m_DurationTransform.m_fSkewY	= pNextFrame->m_Transform.m_fSkewY - m_pCurrentFrame->m_Transform.m_fSkewY;
				m_DurationTransform.m_fScaleX	= pNextFrame->m_Transform.m_fScaleX - m_pCurrentFrame->m_Transform.m_fScaleX;
				m_DurationTransform.m_fScaleY	= pNextFrame->m_Transform.m_fScaleY - m_pCurrentFrame->m_Transform.m_fScaleY;

				if( nFrameIndex == 0 )
				{
					m_DurationTransform.m_fSkewX = FormatRadian( m_DurationTransform.m_fSkewX );
					m_DurationTransform.m_fSkewY = FormatRadian( m_DurationTransform.m_fSkewY );
				}

				m_DurationPivot.m_fX	= pNextFrame->m_Pivot.m_fX - m_pCurrentFrame->m_Pivot.m_fX;
				m_DurationPivot.m_fY	= pNextFrame->m_Pivot.m_fY - m_pCurrentFrame->m_Pivot.m_fY;

				if( m_DurationTransform.m_fPosX != 0	|| m_DurationTransform.m_fPosY != 0 ||
					m_DurationTransform.m_fSkewX != 0	|| m_DurationTransform.m_fSkewY != 0 ||
					m_DurationTransform.m_fScaleX != 0	|| m_DurationTransform.m_fScaleY != 0 ||
					m_DurationPivot.m_fX != 0			|| m_DurationPivot.m_fY != 0 )
				{
					m_bIsTweenTransform = true;
				}
				else
				{
					m_bIsTweenTransform = false;
				}

				if( m_pCurrentFrame->m_pColorTransform != NULL && pNextFrame->m_pColorTransform != NULL )
				{
					m_DurationColor.m_cAlphaOffset = pNextFrame->m_pColorTransform->m_cAlphaOffset - m_pCurrentFrame->m_pColorTransform->m_cAlphaOffset;
					m_DurationColor.m_cRedOffset = pNextFrame->m_pColorTransform->m_cRedOffset - m_pCurrentFrame->m_pColorTransform->m_cRedOffset;
					m_DurationColor.m_cGreenOffset = pNextFrame->m_pColorTransform->m_cGreenOffset - m_pCurrentFrame->m_pColorTransform->m_cGreenOffset;
					m_DurationColor.m_cBlueOffset = pNextFrame->m_pColorTransform->m_cBlueOffset - m_pCurrentFrame->m_pColorTransform->m_cBlueOffset;

					m_DurationColor.m_fAlphaMulti = pNextFrame->m_pColorTransform->m_fAlphaMulti - m_pCurrentFrame->m_pColorTransform->m_fAlphaMulti;
					m_DurationColor.m_fRedMulti = pNextFrame->m_pColorTransform->m_fRedMulti - m_pCurrentFrame->m_pColorTransform->m_fRedMulti;
					m_DurationColor.m_fGreenMulti = pNextFrame->m_pColorTransform->m_fGreenMulti - m_pCurrentFrame->m_pColorTransform->m_fGreenMulti;
					m_DurationColor.m_fBlueMulti = pNextFrame->m_pColorTransform->m_fBlueMulti - m_pCurrentFrame->m_pColorTransform->m_fBlueMulti;

					if( m_DurationColor.m_cAlphaOffset != 0		|| m_DurationColor.m_cRedOffset != 0	||
						m_DurationColor.m_cGreenOffset != 0		|| m_DurationColor.m_cBlueOffset != 0	||
						!IsEqual( m_DurationColor.m_fAlphaMulti, 0.0f ) || !IsEqual( m_DurationColor.m_fRedMulti, 0.0f ) ||
						!IsEqual( m_DurationColor.m_fGreenMulti, 0.0f ) || !IsEqual( m_DurationColor.m_fBlueMulti, 0.0f ) )
					{
						m_bIsTweenColor = true;
					}
					else
					{
						m_bIsTweenColor = false;
					}
				}
				else if( m_pCurrentFrame->m_pColorTransform != NULL )
				{
					m_DurationColor.m_cAlphaOffset = 0 - m_pCurrentFrame->m_pColorTransform->m_cAlphaOffset;
					m_DurationColor.m_cRedOffset = 0 - m_pCurrentFrame->m_pColorTransform->m_cRedOffset;
					m_DurationColor.m_cGreenOffset = 0 - m_pCurrentFrame->m_pColorTransform->m_cGreenOffset;
					m_DurationColor.m_cBlueOffset = 0 - m_pCurrentFrame->m_pColorTransform->m_cBlueOffset;

					m_DurationColor.m_fAlphaMulti = 1.0f - m_pCurrentFrame->m_pColorTransform->m_fAlphaMulti;
					m_DurationColor.m_fRedMulti = 1.0f - m_pCurrentFrame->m_pColorTransform->m_fRedMulti;
					m_DurationColor.m_fGreenMulti = 1.0f - m_pCurrentFrame->m_pColorTransform->m_fGreenMulti;
					m_DurationColor.m_fBlueMulti = 1.0f - m_pCurrentFrame->m_pColorTransform->m_fBlueMulti;

					m_bIsTweenColor = true;
				}
				else if( pNextFrame->m_pColorTransform != NULL )
				{
					m_DurationColor.m_cAlphaOffset = pNextFrame->m_pColorTransform->m_cAlphaOffset;
					m_DurationColor.m_cRedOffset = pNextFrame->m_pColorTransform->m_cRedOffset;
					m_DurationColor.m_cGreenOffset = pNextFrame->m_pColorTransform->m_cGreenOffset;
					m_DurationColor.m_cBlueOffset = pNextFrame->m_pColorTransform->m_cBlueOffset;

					m_DurationColor.m_fAlphaMulti = pNextFrame->m_pColorTransform->m_fAlphaMulti - 1.0f;
					m_DurationColor.m_fRedMulti = pNextFrame->m_pColorTransform->m_fRedMulti - 1.0f;
					m_DurationColor.m_fGreenMulti = pNextFrame->m_pColorTransform->m_fGreenMulti - 1.0f;
					m_DurationColor.m_fBlueMulti = pNextFrame->m_pColorTransform->m_fBlueMulti - 1.0f;

					m_bIsTweenColor = true;
				}
				else
				{
					m_bIsTweenColor = false;
				}
			}

			if( !m_bIsTweenTransform )
			{
				if( m_pAnimationState->m_bIsBlend )
				{
					m_Transform.m_fPosX			=	m_OriginTransform.m_fPosX + m_pCurrentFrame->m_Transform.m_fPosX;
					m_Transform.m_fPosY			=	m_OriginTransform.m_fPosY + m_pCurrentFrame->m_Transform.m_fPosY;
					m_Transform.m_fSkewX		=	m_OriginTransform.m_fSkewX + m_pCurrentFrame->m_Transform.m_fSkewX;
					m_Transform.m_fSkewY		=	m_OriginTransform.m_fSkewY + m_pCurrentFrame->m_Transform.m_fSkewY;
					m_Transform.m_fScaleX		=	m_OriginTransform.m_fScaleX + m_pCurrentFrame->m_Transform.m_fScaleX;
					m_Transform.m_fScaleY		=	m_OriginTransform.m_fScaleY + m_pCurrentFrame->m_Transform.m_fScaleY;

					m_Pivot.m_fX				=	m_OriginPivot.m_fX + m_pCurrentFrame->m_Pivot.m_fX;
					m_Pivot.m_fY				=	m_OriginPivot.m_fY + m_pCurrentFrame->m_Pivot.m_fY;
				}
				else
				{
					m_Transform.m_fPosX			=	m_pCurrentFrame->m_Transform.m_fPosX;
					m_Transform.m_fPosY			=	m_pCurrentFrame->m_Transform.m_fPosY;
					m_Transform.m_fSkewX		=	m_pCurrentFrame->m_Transform.m_fSkewX;
					m_Transform.m_fSkewY		=	m_pCurrentFrame->m_Transform.m_fSkewY;
					m_Transform.m_fScaleX		=	m_pCurrentFrame->m_Transform.m_fScaleX;
					m_Transform.m_fScaleY		=	m_pCurrentFrame->m_Transform.m_fScaleY;

					m_Pivot.m_fX				=	m_pCurrentFrame->m_Pivot.m_fX;
					m_Pivot.m_fY				=	m_pCurrentFrame->m_Pivot.m_fY;
				}
			}

			if( !m_bIsTweenColor )
			{
				if( m_pCurrentFrame->m_pColorTransform )
				{
					m_pBone->UpdateColor( 
						m_pCurrentFrame->m_pColorTransform->m_cAlphaOffset,
						m_pCurrentFrame->m_pColorTransform->m_cRedOffset,
						m_pCurrentFrame->m_pColorTransform->m_cGreenOffset,
						m_pCurrentFrame->m_pColorTransform->m_cBlueOffset,
						m_pCurrentFrame->m_pColorTransform->m_fAlphaMulti,
						m_pCurrentFrame->m_pColorTransform->m_fRedMulti,
						m_pCurrentFrame->m_pColorTransform->m_fGreenMulti,
						m_pCurrentFrame->m_pColorTransform->m_fBlueMulti,
						true );
				}
				else if( m_pBone->m_bIsColorChanged )
				{
					m_pBone->UpdateColor( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false );
				}
			}

			m_pBone->ArriveAtFrame( m_pCurrentFrame, this, m_pAnimationState, false );
		}

		if( m_bIsTweenTransform || m_bIsTweenColor )
		{
			p_fPassedTime = ( fPlayedTime - m_fCurrentFramePosition ) / m_fCurrentFrameDuration;
			if( m_fTweenEasing )
			{
				p_fPassedTime = CTransform::GetEaseValue( p_fPassedTime, m_fTweenEasing );
			}
		}

		if( m_bIsTweenTransform )
		{
			CTransform	tagCurrentTransform = m_pCurrentFrame->m_Transform;
			CPoint		tagCurrentPivot	= m_pCurrentFrame->m_Pivot;
			if( m_pAnimationState->m_bIsBlend )
			{
				m_Transform.m_fPosX			= m_OriginTransform.m_fPosX + tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;
				m_Transform.m_fPosY			= m_OriginTransform.m_fPosY + tagCurrentTransform.m_fPosY + m_DurationTransform.m_fPosY * p_fPassedTime;
				m_Transform.m_fSkewX		= m_OriginTransform.m_fSkewX + tagCurrentTransform.m_fSkewX + m_DurationTransform.m_fSkewX * p_fPassedTime;
				m_Transform.m_fSkewY		= m_OriginTransform.m_fSkewY + tagCurrentTransform.m_fSkewY + m_DurationTransform.m_fSkewY * p_fPassedTime;
				m_Transform.m_fPosX			= m_OriginTransform.m_fPosX + tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;
				m_Transform.m_fPosX			= m_OriginTransform.m_fPosX + tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;

				m_Pivot.m_fX				= m_OriginPivot.m_fX + tagCurrentPivot.m_fX + m_DurationPivot.m_fX * p_fPassedTime;
				m_Pivot.m_fY				= m_OriginPivot.m_fY + tagCurrentPivot.m_fY + m_DurationPivot.m_fY * p_fPassedTime;
			}
			else
			{
				m_Transform.m_fPosX			= tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;
				m_Transform.m_fPosY			= tagCurrentTransform.m_fPosY + m_DurationTransform.m_fPosY * p_fPassedTime;
				m_Transform.m_fSkewX		= tagCurrentTransform.m_fSkewX + m_DurationTransform.m_fSkewX * p_fPassedTime;
				m_Transform.m_fSkewY		= tagCurrentTransform.m_fSkewY + m_DurationTransform.m_fSkewY * p_fPassedTime;
				m_Transform.m_fPosX			= tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;
				m_Transform.m_fPosX			= tagCurrentTransform.m_fPosX + m_DurationTransform.m_fPosX * p_fPassedTime;

				m_Pivot.m_fX				= tagCurrentPivot.m_fX + m_DurationPivot.m_fX * p_fPassedTime;
				m_Pivot.m_fY				= tagCurrentPivot.m_fY + m_DurationPivot.m_fY * p_fPassedTime;
			}
		}

		if( m_bIsTweenColor )
		{
			if( m_pCurrentFrame->m_pColorTransform )
			{
				m_pBone->UpdateColor( m_pCurrentFrame->m_pColorTransform->m_cAlphaOffset + static_cast< f32>( m_DurationColor.m_cAlphaOffset ) * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_cRedOffset + static_cast< f32>( m_DurationColor.m_cRedOffset ) * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_cGreenOffset + static_cast< f32>( m_DurationColor.m_cGreenOffset ) * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_cBlueOffset + static_cast< f32>( m_DurationColor.m_cBlueOffset ) * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_fAlphaMulti + m_DurationColor.m_fAlphaMulti * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_fRedMulti + m_DurationColor.m_fRedMulti * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_fGreenMulti + m_DurationColor.m_fGreenMulti * p_fPassedTime,
					m_pCurrentFrame->m_pColorTransform->m_fBlueMulti + m_DurationColor.m_fBlueMulti * p_fPassedTime,
					true );
			}
			else
			{
				m_pBone->UpdateColor( - static_cast< f32>( m_DurationColor.m_cAlphaOffset ) * p_fPassedTime,
					- static_cast< f32>( m_DurationColor.m_cRedOffset ) * p_fPassedTime,
					- static_cast< f32>( m_DurationColor.m_cGreenOffset ) * p_fPassedTime,
					- static_cast< f32>( m_DurationColor.m_cBlueOffset ) * p_fPassedTime,
					1 + m_DurationColor.m_fAlphaMulti * p_fPassedTime,
					1 + m_DurationColor.m_fRedMulti * p_fPassedTime,
					1 + m_DurationColor.m_fGreenMulti * p_fPassedTime,
					1 + m_DurationColor.m_fBlueMulti * p_fPassedTime,
					true );
			}
		}
	}
	else
	{
		m_nUpdateState = 0;
		if( m_pAnimationState->m_bIsBlend )
		{
			m_Transform		= m_OriginTransform;
			m_Pivot.m_fX	= m_OriginPivot.m_fX;
			m_Pivot.m_fY	= m_OriginPivot.m_fY;
		}
		else
		{
			m_Transform.m_fPosX = m_Transform.m_fPosY = m_Transform.m_fScaleX = m_Transform.m_fScaleY = m_Transform.m_fSkewX = m_Transform.m_fSkewY = 0.0f;
			m_Pivot.m_fX = m_Pivot.m_fY = 0.0f;
		}

		m_pCurrentFrame = static_cast< CTransformFrame* >( m_pTimeLine->m_vecFrameList[0] );

		m_bIsTweenActive = ( m_pCurrentFrame->m_nDisplayIndex >= 0 );

		if( m_pCurrentFrame->m_pColorTransform != NULL )
		{
			m_pBone->UpdateColor( 
				m_pCurrentFrame->m_pColorTransform->m_cAlphaOffset,
				m_pCurrentFrame->m_pColorTransform->m_cRedOffset,
				m_pCurrentFrame->m_pColorTransform->m_cGreenOffset,
				m_pCurrentFrame->m_pColorTransform->m_cBlueOffset,
				m_pCurrentFrame->m_pColorTransform->m_fAlphaMulti,
				m_pCurrentFrame->m_pColorTransform->m_fRedMulti,
				m_pCurrentFrame->m_pColorTransform->m_fGreenMulti,
				m_pCurrentFrame->m_pColorTransform->m_fBlueMulti,
				true );
		}
		else
		{
			m_pBone->UpdateColor( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false );
		}

		m_pBone->ArriveAtFrame( m_pCurrentFrame, this, m_pAnimationState, false );
	}
}
//-------------------------------------------------------------------------
void CTimeLineState::Clear()
{
	m_nUpdateState = 0;
	m_pBone = NULL;
	m_pAnimationState = NULL;
	m_pCurrentFrame = NULL;
	m_pTimeLine = NULL;
	m_OriginPivot = CPoint();
	m_OriginTransform = CTransform();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------