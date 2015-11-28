//-------------------------------------------------------------------------
#include "AudioBridge.h"
#include "GameResource.h"
#include "GameCommon.h"
//-------------------------------------------------------------------------
CAudioBridge::CAudioBridge(void)
{
	m_mapBGMusicList.clear();
	m_mapEffectList.clear();
	
	__InitAudioResourceMap();
}
//-------------------------------------------------------------------------
CAudioBridge::~CAudioBridge(void)
{
	m_mapBGMusicList.clear();
	m_mapEffectList.clear();
}
//-------------------------------------------------------------------------
void CAudioBridge::__InitAudioResourceMap()
{
	m_mapBGMusicList.insert( std::make_pair(eSubSceneType_Login,		s_szMainMusic) );
	m_mapBGMusicList.insert( std::make_pair(eSubSceneType_Play,			s_szGameMusic) );
	m_mapBGMusicList.insert( std::make_pair(eSubSceneType_Shop,			s_szMainMusic) );
	m_mapBGMusicList.insert( std::make_pair(eSubSceneType_Achie,		s_szMainMusic) );

	m_mapEffectList.insert( std::make_pair(eSound_Button,				s_szButtonSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarOut,				s_szStarOutSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarClick,			s_szStarClickSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarCombo,			s_szStarComboSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarGood,			s_szGOODSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarAmazing,			s_szAmazingSound) );
	m_mapEffectList.insert( std::make_pair(eSound_StarUnbeliveable,	s_szUnbeliveableSound) );
}
//-------------------------------------------------------------------------
void CAudioBridge::PlayBackGroudMusic(int nSrc)
{
	IAudioSystem* pAudioSystem = GetAudioSystem();
	if (NULL == pAudioSystem)
	{
		return;
	}
	if( nSrc >= eSubSceneType_Max || nSrc <= eSubSceneType_Min )
	{
		return;
	}
	map< unsigned int, string >::iterator Ite = m_mapBGMusicList.find(nSrc);
	if( Ite == m_mapBGMusicList.end() )
	{
		return;
	}

	pAudioSystem->PlayBackgroundMusic( Ite->second.c_str(), true );
}
//-------------------------------------------------------------------------
void CAudioBridge::StopBackGroundMusic()
{
	IAudioSystem* pAudioSystem = GetAudioSystem();
	if (NULL == pAudioSystem)
	{
		return;
	}
	pAudioSystem->StopBackgroundMusic(false);
}
//-------------------------------------------------------------------------
void CAudioBridge::PlayEffect(int nSrc,bool bIsLoop)
{
	IAudioSystem* pAudioSystem = GetAudioSystem();
	if (NULL == pAudioSystem)
	{
		return;
	}

	string strEffectName = "";
	map< unsigned int, string >::iterator Ite = m_mapEffectList.find(nSrc);
	if( Ite == m_mapEffectList.end() )
	{
		return;
	}
	strEffectName = Ite->second;

	{
		// ÌØÊâ´¦Àí
		char szBuf[64];
		int nRand = 0;
		if( nSrc == eSound_StarClick )
		{
			memset( szBuf, 0, 64 );
			nRand = (rand() % (3-1+1))+ 1;
			sprintf(szBuf,strEffectName.c_str(),nRand);
			szBuf[63] = '\0';
			strEffectName = szBuf;
		}
	}

	if ( strEffectName == "" || strEffectName.empty() )
	{
		return;
	}

	pAudioSystem->PlayEffect(strEffectName.c_str(),bIsLoop);
}
//-------------------------------------------------------------------------
void CAudioBridge::StopAllEffect()
{
	IAudioSystem* pAudioSystem = GetAudioSystem();
	if (NULL == pAudioSystem)
	{
		return;
	}
	pAudioSystem->StopAllEffects();
}