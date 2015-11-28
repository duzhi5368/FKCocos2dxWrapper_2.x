//*************************************************************************
//	创建日期:	2014-8-13   11:20
//	文件名称:	TimeLineState.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Transform.h"
#include "Point.h"
#include "AnimationState.h"
#include "ColorTransform.h"
#include <vector>
#include <stdlib.h>
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CBone;
class CTransformFrame;
class CTransformTimeLine;
class CTimeLineState
{
public:
	static vector<CTimeLineState*>		s_vecTimeLineStatePool;
public:
	static CTimeLineState*		BorrowObject();
	static void					ReturnObject( CTimeLineState* p_pTimeLineState );
	static void					ClearAll();
public:
	CTimeLineState();
	~CTimeLineState();
public:
	void						FadeIn( CBone* p_pBone, CAnimationState* p_pAnimationState, CTransformTimeLine* p_pTimeLine );
	void						FadeOut();
	void						Update( f32 p_fPassedTime );
	void						Clear();
public:
	CTransform					m_Transform;
	CTransform					m_OriginTransform;
	CTransform					m_DurationTransform;
	CColorTransform				m_DurationColor;
	CPoint						m_Pivot;
	CPoint						m_DurationPivot;
	CPoint						m_OriginPivot;
	bool						m_bIsTweenActive;
	bool						m_bIsTweenTransform;
	bool						m_bIsTweenColor;
	int							m_nUpdateState;
	CAnimationState*			m_pAnimationState;
	CBone*						m_pBone;
	CTransformFrame*			m_pCurrentFrame;
	CTransformTimeLine*			m_pTimeLine;
	f32							m_fCurrentFramePosition;
	f32							m_fCurrentFrameDuration;
	f32							m_fTweenEasing;
	f32							m_fTotalTime;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------