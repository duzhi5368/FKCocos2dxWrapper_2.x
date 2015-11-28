//*************************************************************************
//	��������:	2014-8-13   16:13
//	�ļ�����:	ArmatureEvent.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Event.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CArmatureEvent : public CEvent
{
public:
	const static string Z_ORDER_UPDATED;
public:
	CArmatureEvent( const string& p_szType )
		: CEvent( p_szType )
	{

	}
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------