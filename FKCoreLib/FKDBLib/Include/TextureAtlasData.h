//*************************************************************************
//	创建日期:	2014-8-12   11:51
//	文件名称:	TextureAtlasData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Rect.h"
#include <string>
#include <map>
using std::string;
using std::map;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CTextureAtlasData
{
public:
	string					m_szImagePath;
	string					m_szName;
	map< string, CRect >	m_szRects;
public:
	CTextureAtlasData()
	{

	}

	void AddTextureAtlasData( const string& p_szName, const CRect& p_Rect )
	{
		m_szRects[ p_szName ] = p_Rect;
	}
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------