//*************************************************************************
//	创建日期:	2014-8-11   14:58
//	文件名称:	Frame.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 关键帧
class CFrame
{
public:
	f32			m_fPos;			// 帧数位置
	f32			m_fDuration;	// 帧持续时间
public:
	string		m_szAction;		// 动作名
	string		m_szEvent;		// 事件
	string		m_szSound;		// 音效
public:
	CFrame()
		: m_fPos( 0.0f )
		, m_fDuration( 0.0f )
	{
		m_szAction.clear();
		m_szEvent.clear();
		m_szSound.clear();
	}

	virtual ~CFrame()
	{
		Clear();
	}

	virtual void Clear()
	{
		m_fPos = m_fDuration = 0.0f;
		m_szAction.clear();
		m_szEvent.clear();
		m_szSound.clear();
	}
};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------