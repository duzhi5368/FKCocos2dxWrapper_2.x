//*************************************************************************
//	创建日期:	2015-3-24
//	文件名称:	AudioBridge.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class CAudioBridge
{
	CAudioBridge(void);
	~CAudioBridge(void);
public:
	static CAudioBridge& GetInstance()
	{
		static CAudioBridge _inst;
		return _inst;
	}
private:
	void	__InitAudioResourceMap();
public:
	void	PlayBackGroudMusic(int nSrc);
	void	StopBackGroundMusic();
	void	PlayEffect(int nSrc,bool bIsLoop = false);
	void	StopAllEffect();
private:
	map< unsigned int, string >		m_mapBGMusicList;
	map< unsigned int, string >		m_mapEffectList;
};

