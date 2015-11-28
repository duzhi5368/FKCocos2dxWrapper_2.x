//*************************************************************************
//	创建日期:	2014-8-15   9:32
//	文件名称:	Animation.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "BaseMath.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
//-------------------------------------------------------------------------
class CArmature;
class CAnimationState;
class CAnimationData;
class CAnimation
{
public:
	const static string NONE;
	const static string	SAME_LAYER;
	const static string SAME_GROUP;
	const static string SAME_LAYER_AND_GROUP;
	const static string ALL;
public:
	bool				m_bIsTweenEnabled;
	bool				m_bIsActive;
	bool				m_bIsPlaying;
	f32					m_fTimeScale;
	CArmature*			m_pArmature;
	CAnimationState*	m_pLastAnimationState;
	vector< string >	m_vecAnimationList;
	vector< CAnimationData* >					m_vecAnimationDataList;
	vector< vector< CAnimationState* >* >		m_vecAnimationLayer;
public:
	CAnimation( CArmature* p_pArmature );
	virtual ~CAnimation();
public:
	void				Clear();
	CAnimationState*	GotoAndPlay( const string& p_szAnimationName, f32 p_fFadeInTime = -1,
		f32 p_fDuration = -1, f32 p_fLoop = NaN, uint p_unLayer = 0, const string& p_szGroup = "",
		const string& p_szFadeOutMode = SAME_LAYER_AND_GROUP, bool p_bDisplayControl = true, 
		bool p_bPauseFadeOut = true, bool p_bPauseFadeIn = true );
	void				Play();
	void				Stop();
	void				Update( f32 p_fPassedTime );

	CAnimationState*	GetStateByName( const string& p_szName, uint p_unLayer = 0 );
	bool				IsHasAnimation( const string& p_szAnimationName );
	void				SetActive( CAnimationState* p_pAnimationState, bool p_bActive );
	uint				AddLayer( uint p_unLayer );
	void				AddState( CAnimationState* p_pAnimationState );
	void				RemoveState( CAnimationState* p_pAnimationState );

	const vector<CAnimationData*>	GetAnimationDataList();
	void							SetAnimationDataList( const vector<CAnimationData*>& p_Value );
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------