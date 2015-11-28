//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_MsgManager.h"
#include <string>
//-------------------------------------------------------------------------
FKCW_SceneExMgr_MsgManager::FKCW_SceneExMgr_MsgManager()
	: m_bDispatchMsgLocked(false)
{

}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_MsgManager::~FKCW_SceneExMgr_MsgManager()
{
	if(!m_vMsgQueue.empty())
	{
		vector<SSenceExMsg>::iterator citr = m_vMsgQueue.begin();
		vector<SSenceExMsg>::iterator cend = m_vMsgQueue.end();
		for(; citr != cend; ++citr )
		{
			SSenceExMsg& tagMsg = *citr;
			CC_SAFE_RELEASE(tagMsg.pMsgObj);
		}
	}
	m_vMsgQueue.clear();

	if(!m_vTargetMsgQueue.empty())
	{
		vector<SSceneExMsgForTarget>::iterator citr = m_vTargetMsgQueue.begin();
		vector<SSceneExMsgForTarget>::iterator cend = m_vTargetMsgQueue.end();
		for(; citr != cend; ++citr )
		{
			SSceneExMsgForTarget& tagTarget = *citr;
			SSenceExMsg& tagMsg = tagTarget.tMessage;
			CCObject* pObj = dynamic_cast<CCObject*>(tagTarget.pDelegate);
			CC_SAFE_RELEASE(pObj);
			CC_SAFE_RELEASE(tagMsg.pMsgObj);
		}
	}
	m_vTargetMsgQueue.clear();

	if(!m_vMsgDelegates.empty())
	{
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = m_vMsgDelegates.begin();
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator end = m_vMsgDelegates.end();
		for(; itr != end; ++itr )
		{
			CCObject* pObj = dynamic_cast<CCObject*>(*itr);
			CC_SAFE_RELEASE(pObj);
		}
	}
	m_vMsgDelegates.clear();

	if(!m_vMsgDelegatesAdded.empty())
	{
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = m_vMsgDelegatesAdded.begin();
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator end = m_vMsgDelegatesAdded.end();
		for(; itr != end; ++itr )
		{
			CCObject* pObj = dynamic_cast<CCObject*>(*itr);
			CC_SAFE_RELEASE(pObj);
		}
	}
	m_vMsgDelegatesAdded.clear();
	m_vMsgDelegatesRemoved.clear();
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_MsgManager* FKCW_SceneExMgr_MsgManager::sharedManager()
{
	static FKCW_SceneExMgr_MsgManager* pInstance = NULL;
	if( pInstance == NULL ) 
	{
		pInstance = new FKCW_SceneExMgr_MsgManager();
	}
	return pInstance;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_MsgManager::update()
{
	// 上锁
	m_bDispatchMsgLocked = true;

	if(!m_vMsgQueue.empty())
	{
		vector<SSenceExMsg>::iterator citr = m_vMsgQueue.begin();
		vector<SSenceExMsg>::iterator cend = m_vMsgQueue.end();
		for(; citr != cend; ++citr )
		{
			SSenceExMsg& tagMsg = *citr;
			vector<FKCW_SceneExMgr_MsgDelegate*>::iterator hitr = m_vMsgDelegates.begin();
			vector<FKCW_SceneExMgr_MsgDelegate*>::iterator hend = m_vMsgDelegates.end();
			for(; hitr != hend; ++hitr )
			{
				(*hitr)->onMessage(tagMsg.uMsg, tagMsg.pMsgObj, tagMsg.wParam, tagMsg.lParam);
			}
			CC_SAFE_RELEASE(tagMsg.pMsgObj);
		}
		m_vMsgQueue.clear();
	}

	if(!m_vTargetMsgQueue.empty())
	{
		vector<SSceneExMsgForTarget>::iterator citr = m_vTargetMsgQueue.begin();
		vector<SSceneExMsgForTarget>::iterator cend = m_vTargetMsgQueue.end();
		for(; citr != cend; ++citr )
		{
			SSceneExMsgForTarget& tagTarget = *citr;
			SSenceExMsg& tagMsg = tagTarget.tMessage;
			tagTarget.pDelegate->onMessage(tagMsg.uMsg, tagMsg.pMsgObj, tagMsg.wParam, tagMsg.lParam);

			CCObject* pObj = dynamic_cast<CCObject*>(tagTarget.pDelegate);
			CC_SAFE_RELEASE(pObj);
			CC_SAFE_RELEASE(tagMsg.pMsgObj);
		}
		m_vTargetMsgQueue.clear();
	}

	m_bDispatchMsgLocked = false;
	// 解锁

	while(!m_vMsgDelegatesAdded.empty())
	{
		FKCW_SceneExMgr_MsgDelegate* pDelegate = m_vMsgDelegatesAdded.front();
		m_vMsgDelegates.insert(m_vMsgDelegates.begin(), pDelegate);
		m_vMsgDelegatesAdded.erase(m_vMsgDelegatesAdded.begin());
	}

	while(!m_vMsgDelegatesRemoved.empty())
	{
		FKCW_SceneExMgr_MsgDelegate* pDelegate = m_vMsgDelegatesRemoved.front();

		//vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = std::find(m_vMsgDelegates.begin(),m_vMsgDelegates.end(),pDelegate);
		//限制于android平台不兼容find 所以使用下面的循环查找
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = m_vMsgDelegates.end();
		for (vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itrTemp = m_vMsgDelegates.begin();itrTemp != m_vMsgDelegates.end();++itrTemp)
		{
			if (pDelegate == *itrTemp)
			{
				itr = itrTemp;
				break;
			}
			
		}
		

		if( itr != m_vMsgDelegates.end() )
		{
			m_vMsgDelegates.erase(itr);
		}

		CCObject* pObject = dynamic_cast<CCObject*>(pDelegate);
		pObject->release();
		m_vMsgDelegatesRemoved.erase(m_vMsgDelegatesRemoved.begin());
	}

	if(!m_vMsgTemps.empty())
	{
		m_vMsgQueue.insert(m_vMsgQueue.end(), m_vMsgTemps.begin(), m_vMsgTemps.end());
		m_vMsgTemps.clear();
	}

	if(!m_vTargetMsgTemps.empty())
	{
		m_vTargetMsgQueue.insert(m_vTargetMsgQueue.end(), m_vTargetMsgTemps.begin(), m_vTargetMsgTemps.end());
		m_vTargetMsgTemps.clear();
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_MsgManager::PostMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam)
{
	SSenceExMsg     tagMsg;
	tagMsg.uMsg    = uMsg;
	tagMsg.pMsgObj = pMsgObj;
	tagMsg.wParam  = wParam;
	tagMsg.lParam  = lParam;

	CC_SAFE_RETAIN(pMsgObj);

	if( m_bDispatchMsgLocked )
	{
		m_vMsgTemps.push_back(tagMsg);
	}
	else
	{
		m_vMsgQueue.push_back(tagMsg);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_MsgManager::PostMessage(FKCW_SceneExMgr_MsgDelegate* pDelegate, unsigned int uMsg,
												   CCObject* pMsgObj, void* wParam, void* lParam)
{
	if( !pDelegate )
		return;

	CCAssert(dynamic_cast<CCObject*>(pDelegate), "this target should not null");

	SSceneExMsgForTarget        tagMsg;
	tagMsg.pDelegate        = pDelegate;
	tagMsg.tMessage.uMsg    = uMsg;
	tagMsg.tMessage.pMsgObj = pMsgObj;
	tagMsg.tMessage.wParam  = wParam;
	tagMsg.tMessage.lParam  = lParam;

	CCObject* pObj = dynamic_cast<CCObject*>(pDelegate);
	CC_SAFE_RETAIN(pObj);
	CC_SAFE_RETAIN(pMsgObj);

	if( m_bDispatchMsgLocked )
	{
		m_vTargetMsgTemps.push_back(tagMsg);
	}
	else
	{
		m_vTargetMsgQueue.push_back(tagMsg);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_MsgManager::RegisterMsgDelegate(FKCW_SceneExMgr_MsgDelegate* pDelegate)
{
	CCAssert(pDelegate && dynamic_cast<CCObject*>(pDelegate), "should not null");

	//vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr;
	//itr = std::find(m_vMsgDelegates.begin(), m_vMsgDelegates.end(), pDelegate);

	//限制于android平台不兼容find 所以使用下面的循环查找
	vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = m_vMsgDelegates.end();
	for (vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itrTemp = m_vMsgDelegates.begin();itrTemp != m_vMsgDelegates.end();++itrTemp)
	{
		if (pDelegate == *itrTemp)
		{
			itr = itrTemp;
			break;
		}

	}

	if( itr != m_vMsgDelegates.end() )
	{
		return;
	}

	CCObject* pObj = dynamic_cast<CCObject*>(pDelegate);
	CC_SAFE_RETAIN(pObj);

	if( m_bDispatchMsgLocked )
	{
		m_vMsgDelegatesAdded.push_back(pDelegate);
	}
	else
	{
		m_vMsgDelegates.insert(m_vMsgDelegates.begin(), pDelegate);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_MsgManager::UnregisterMsgDelegate(FKCW_SceneExMgr_MsgDelegate* pDelegate)
{
	CCAssert(pDelegate && dynamic_cast<CCObject*>(pDelegate), "should not null");

	if( m_bDispatchMsgLocked )
	{
		m_vMsgDelegatesRemoved.push_back(pDelegate);
	}
	else
	{
		//vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr;
		//itr = std::find(m_vMsgDelegates.begin(), m_vMsgDelegates.end(), pDelegate);
		//限制于android平台不兼容find 所以使用下面的循环查找
		vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itr = m_vMsgDelegates.end();
		for (vector<FKCW_SceneExMgr_MsgDelegate*>::iterator itrTemp = m_vMsgDelegates.begin();itrTemp != m_vMsgDelegates.end();++itrTemp)
		{
			if (pDelegate == *itrTemp)
			{
				itr = itrTemp;
				break;
			}

		}

		if( itr != m_vMsgDelegates.end() )
			m_vMsgDelegates.erase(itr);

		CCObject* pObj = dynamic_cast<CCObject*>(pDelegate);
		CC_SAFE_RELEASE(pObj);
	}
}
//-------------------------------------------------------------------------