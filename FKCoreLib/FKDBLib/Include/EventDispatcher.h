//*************************************************************************
//	创建日期:	2014-8-13   15:19
//	文件名称:	EventDispatcher.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "Event.h"
#include <string>
#include <functional>
#include <list>
#include <map>
using std::string;
using std::list;
using std::map;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CEventDispatcher
{
public:
	typedef	std::function<void(CEvent*)>		CallbackFunc;
	typedef std::pair<CallbackFunc, string>		Listener;
	typedef std::list<Listener>					ListListeners;
	typedef std::map<string, ListListeners>		EventListeners;
public:
	EventListeners			m_mapEventListeners;
public:
	// 使用 EventDispatcher 对象注册事件监听器对象，以使侦听器能够接收事件通知，key是这个回调的唯一标示，用来removeEventListener反注册用的
	void		AddEventListener( const string& p_szType, CallbackFunc p_pListener, const string& p_szKey );
	// 进行消息分发
	void		DispatchEvent( CEvent* p_pEvent );
	// 检查是否注册了某种类型的事件的回调
	bool		IsHadEventListener( const string& p_szType )const;
	// 删除监听器
	void		RemoveEventListener( const string& p_szType, const string& p_szKey );
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------