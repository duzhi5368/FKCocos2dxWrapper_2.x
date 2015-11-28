//-------------------------------------------------------------------------
#include "../Include/EventDispatcher.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

// 使用 EventDispatcher 对象注册事件监听器对象，以使侦听器能够接收事件通知，key是这个回调的唯一标示，用来removeEventListener反注册用的
void CEventDispatcher::AddEventListener( const string& p_szType, CallbackFunc p_pListener, const string& p_szKey )
{
	ListListeners& list = m_mapEventListeners[p_szType];
	list.push_back( Listener(p_pListener, p_szKey));
}
//-------------------------------------------------------------------------
// 进行消息分发
void CEventDispatcher::DispatchEvent( CEvent* p_pEvent )
{
	if( p_pEvent == NULL )
		return;
	EventListeners::iterator Ite = m_mapEventListeners.find( p_pEvent->m_szType );
	if( Ite != m_mapEventListeners.end() )
	{
		ListListeners::iterator IteList = Ite->second.begin();
		for( ; IteList != Ite->second.end(); ++IteList )
		{
			IteList->first( p_pEvent );
		}
	}

	// 注意，这里进行了删除
	delete p_pEvent;
}
//-------------------------------------------------------------------------
// 检查是否注册了某种类型的事件的回调
bool CEventDispatcher::IsHadEventListener( const string& p_szType )const
{
	return ( m_mapEventListeners.find( p_szType ) != m_mapEventListeners.end() );
}
//-------------------------------------------------------------------------
// 删除监听器
void CEventDispatcher::RemoveEventListener( const string& p_szType, const string& p_szKey )
{
	EventListeners::iterator Ite = m_mapEventListeners.find( p_szType );
	if( Ite == m_mapEventListeners.end() )
		return;

	ListListeners::iterator IteList = Ite->second.begin();
	for( ; IteList != Ite->second.end();  )
	{
		if( IteList->second == p_szKey )
		{
			IteList = Ite->second.erase( IteList );
		}
		else
		{
			++IteList;
		}
	}
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------