//*************************************************************************
//	创建日期:	2014-8-13   16:25
//	文件名称:	SoundEvent.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Event.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CArmature;
class CAnimationState;
class CSoundEvent : public CEvent
{
public:
	static const string		SOUND;
public:
	CArmature*				m_pArmature;
	CAnimationState*		m_pAnimationState;
	string					m_szSound;
public:
	CSoundEvent( const string& p_szType )
		: CEvent( p_szType )
		, m_pAnimationState( NULL )
		, m_pArmature( NULL )
	{

	}
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------