//*************************************************************************
//	创建日期:	2014-8-11   16:15
//	文件名称:	DisplayData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Transform.h"
#include "Point.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class CDisplayData
{
public:
	string			m_szName;
	string			m_szType;
	CTransform		m_Transform;
	CPoint			m_Pivot;
public:
	CDisplayData()
	{
		m_szName.clear();
		m_szType.clear();
	}

	virtual ~CDisplayData()
	{
		Clear();
	}
public:
	virtual void Clear()
	{
		m_szName.clear();
		m_szType.clear();
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------