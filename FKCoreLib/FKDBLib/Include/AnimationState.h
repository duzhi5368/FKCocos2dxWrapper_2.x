//*************************************************************************
//	创建日期:	2014-8-13   10:47
//	文件名称:	AnimationState.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include <string>
#include <map>
#include <vector>
using std::string;
using std::map;
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class CFrame;
class CArmature;
class CAnimationData;
class CTimeLineState;
class CAnimationState
{
	static vector<CAnimationState*>	s_vecAnimationStatePool;
public:
	static CAnimationState*			BorrowObject();
	static void						ReturnObject( CAnimationState* p_pAnimationState );
	static void						ClearAll();
public:
	bool							m_bIsTweenEnabled;
	bool							m_bIsBlend;
	bool							m_bIsFadeIn;
	bool							m_bIsFadeOut;
	bool							m_bIsPlaying;
	bool							m_bIsComplete;
	bool							m_bIsDiaplayControl;
	string							m_szName;
	string							m_szGroup;
	f32								m_fWeight;
	f32								m_fFadeWeight;
	f32								m_fFadeInTime;
	f32								m_fFadeOutTime;
	f32								m_fFadeOutBeginTime;
	f32								m_fFadeOutWeight;
	f32								m_fTotalTime;
	f32								m_fCurrentTime;
	f32								m_fTimeScale;
	CArmature*						m_pArmature;
	CFrame*							m_pCurrentFrame;
	map<string, CTimeLineState*>	m_mapTimeLineStates;
	map<string, int>				m_mapMixingTransforms;
	int								m_nFadeState;
	int								m_nPauseBeforeFadeInCompleteState;
	int								m_nLoopCount;
	int								m_nLoop;
	uint							m_unLayer;
	CAnimationData*					m_pClip;
public:
	CAnimationState();
	~CAnimationState();
public:
	void							SetCurrentTime( f32 p_fValue );
	void							SetTimeScale( f32 p_fValue );
	void							FadeIn( CArmature* p_pArmature, CAnimationData* p_pClip, f32 p_fFadeInTime,
		f32 p_fTimeScale, int p_nLoop, uint p_unLayer, bool p_bIsDisplayControl, bool p_bIsPauseBeforeFadeInComplete );
	void							FadeOut( f32 p_fFadeOutTime, bool p_bPause = false );
	void							Play();
	void							Stop();
	int								GetMixingTransform( const string& p_szTimeLineName );
	void							AddMixingTransform( const string& p_szTimeLineName, int p_nType = 2, bool p_bResursive = true );
	void							RemoveMixingTransform( const string& p_szTimeLineName = "", bool p_bResursive = true );
	bool							Update( f32 p_fPassedTime );
	void							UpdateTimeLineStates();
	void							AddTimeLineState( const string& p_szTimeLineName );
	void							RemoveTimeLineState( const string& p_szTimeLineName );
	void							Clear();
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------