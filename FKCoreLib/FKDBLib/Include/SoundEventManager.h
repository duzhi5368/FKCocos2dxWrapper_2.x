//*************************************************************************
//	创建日期:	2014-8-13   16:30
//	文件名称:	SoundEventManager.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "EventDispatcher.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CSoundEventManager : public CEventDispatcher
{
private:
	static CSoundEventManager*		s_Instance;
public:
	static CSoundEventManager*		GetInstance()
	{
		if( s_Instance == NULL )
		{
			s_Instance = new CSoundEventManager();
		}
		return s_Instance;
	}

	CSoundEventManager()
	{
		if( s_Instance != NULL )
		{
			//throw std::invalid_argument("Singleton CSoundEventManager has already constructed");
			printf("CSoundEventManager Singleton CSoundEventManager has already constructed");
		}
	}
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------