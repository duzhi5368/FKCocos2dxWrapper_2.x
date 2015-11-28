//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExMgr.h"
#include "../Include/FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
using std::make_pair;
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExMgr::FKCW_SceneExMgr_SceneExMgr()
	: m_bSendCleanupToScene(false)
	, m_pRunningScene(NULL)
	, m_pNextScene(NULL)
{
	m_pMsgManager = FKCW_SceneExMgr_MsgManager::sharedManager();
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExMgr::~FKCW_SceneExMgr_SceneExMgr()
{
	m_pMsgManager->release();

	if(!m_lSceneSwitchQueue.empty())
	{
		list<SSceneSwitchInfo>::iterator itr = m_lSceneSwitchQueue.begin();
		list<SSceneSwitchInfo>::iterator end = m_lSceneSwitchQueue.end();

		for(; itr != end; ++itr )
		{
			CC_SAFE_RELEASE(itr->pNextScene);
			CC_SAFE_RELEASE(itr->pExtra);
		}
	}
	m_lSceneSwitchQueue.clear();

	removeAllCachedScenes();

	if(!m_vRunningSceneStack.empty())
	{
		vector<FKCW_SceneExMgr_SceneEx*>::iterator itr = m_vRunningSceneStack.begin();
		vector<FKCW_SceneExMgr_SceneEx*>::iterator end = m_vRunningSceneStack.end();

		for(; itr != end; ++itr )
		{
			CC_SAFE_RELEASE(*itr);
		}
	}
	m_vRunningSceneStack.clear();

	if(!m_vRunningUIScenes.empty())
	{
		vector<FKCW_SceneExMgr_SceneEx*>::iterator itr = m_vRunningUIScenes.begin();
		vector<FKCW_SceneExMgr_SceneEx*>::iterator end = m_vRunningUIScenes.end();

		for(; itr != end; ++itr )
		{
			CC_SAFE_RELEASE(*itr);
		}
	}
	m_vRunningUIScenes.clear();
	m_mFnSceneCreatePointers.clear();

	CC_SAFE_RELEASE(m_pRunningScene);
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExMgr* FKCW_SceneExMgr_SceneExMgr::sharedManager()
{
	static FKCW_SceneExMgr_SceneExMgr* pInstance = NULL;
	if( pInstance == NULL ) {
		pInstance = new FKCW_SceneExMgr_SceneExMgr();
		pInstance->init();
		CCDirector::sharedDirector()->runWithScene(pInstance);
		pInstance->release();
	}
	return pInstance;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::visit()
{
	mainLoop();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::mainLoop()
{
	// 更新消息管理器
	m_pMsgManager->update();

	// 只能更替
	do {
		CC_BREAK_IF(m_lSceneSwitchQueue.empty());
		SSceneSwitchInfo& tSceneSwitch = m_lSceneSwitchQueue.front();

		CC_BREAK_IF(tSceneSwitch.bLockedSwitch);
		handleSceneSwitch(tSceneSwitch);
		m_lSceneSwitchQueue.pop_front();
	}
	while(0);

	do {
		CC_BREAK_IF(m_lUISceneSwitchQueue.empty());
		SUISceneSwitchInfo& tSceneSwitch = m_lUISceneSwitchQueue.front();

		CC_BREAK_IF(tSceneSwitch.bLockedSwitch);
		handleUISceneSwitch(tSceneSwitch);
		m_lUISceneSwitchQueue.pop_front();
	}
	while(1);

	// 若需要进行场景替换
	if( m_pNextScene )
	{
		setNextScene();
	}
	// 绘制运行中场景
	if( m_pRunningScene )
	{
		m_pRunningScene->visit();
	}

	// 绘制UI场景
	visitUIScenes();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::runWithScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra)
{
	CCAssert(pScene != NULL, "This command can only be used to start the CCDirector. There is already a scene present.");
	CCAssert(m_pRunningScene == NULL, "m_pRunningScene should be null");

	pushScene(pScene, pExtra);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::pushScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra)
{
	CCAssert(pScene, "the scene should not null");

	if( isSceneRunning(getSceneClassName(pScene)) )
		return;

	SSceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pNextScene = pScene;
	tSceneSwitch.pExtra = NULL;
	tSceneSwitch.eType = eSceneSwitchPushScene;
	tSceneSwitch.bLockedSwitch = true;

	setExtraToScene(pScene, pExtra);
	m_lSceneSwitchQueue.push_back(tSceneSwitch);

	CC_SAFE_RETAIN(pScene);

	if( !loadSceneResources(pScene) )
	{
		m_lSceneSwitchQueue.back().bLockedSwitch = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::replaceScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra)
{
	CCAssert(m_pRunningScene, "Use runWithScene: instead to start the director");
	CCAssert(pScene != NULL, "the scene should not be null");

	if( isSceneRunning(getSceneClassName(pScene)) )
		return;

	SSceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pNextScene = pScene;
	tSceneSwitch.pExtra = NULL;
	tSceneSwitch.eType = eSceneSwitchReplaceScene;
	tSceneSwitch.bLockedSwitch = true;

	setExtraToScene(pScene, pExtra);
	m_lSceneSwitchQueue.push_back(tSceneSwitch);

	CC_SAFE_RETAIN(pScene);

	if( !loadSceneResources(pScene) )
	{
		m_lSceneSwitchQueue.back().bLockedSwitch = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::popScene(CCObject* pExtra)
{
	CCAssert(m_pRunningScene != NULL, "running scene should not null");

	SSceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pNextScene = NULL;
	tSceneSwitch.pExtra = pExtra;
	tSceneSwitch.eType = eSceneSwitchPopScene;
	tSceneSwitch.bLockedSwitch = false;

	CC_SAFE_RETAIN(pExtra);

	m_lSceneSwitchQueue.push_back(tSceneSwitch);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::popToRootScene(CCObject* pExtra)
{
	popToSceneStackLevel(1, pExtra);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::popToSceneStackLevel(int nLevel, CCObject* pExtra)
{
	CCAssert(m_pRunningScene != NULL, "A running Scene is needed");

	SSceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pNextScene = NULL;
	tSceneSwitch.pExtra = pExtra;
	tSceneSwitch.eType = eSceneSwitchPopToSceneStackLevel;
	tSceneSwitch.nPopSceneStackLevel = nLevel;
	tSceneSwitch.bLockedSwitch = false;

	CC_SAFE_RETAIN(pExtra);

	m_lSceneSwitchQueue.push_back(tSceneSwitch);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::end()
{
	autorelease();
	CCDirector::sharedDirector()->end();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::runUIScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra)
{
	CCAssert(pScene != NULL && !dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(pScene), "should not null and not transition");

	if( isSceneRunning(getSceneClassName(pScene)) )
		return;

	SUISceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pScene = pScene;
	tSceneSwitch.eType = eUISceneSwitchRunScene;
	tSceneSwitch.bLockedSwitch = true;
	m_lUISceneSwitchQueue.push_back(tSceneSwitch);

	CC_SAFE_RETAIN(pScene);
	setExtraToScene(pScene, pExtra);

	if( !loadSceneResources(pScene) )
	{
		m_lUISceneSwitchQueue.back().bLockedSwitch = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::popUIScene(FKCW_SceneExMgr_SceneEx* pScene)
{
	CCAssert(pScene != NULL, "A running Scene is needed");

	if(!isSceneRunning(getSceneClassName(pScene)) )
		return;

	SUISceneSwitchInfo tSceneSwitch;
	tSceneSwitch.pScene = pScene;
	tSceneSwitch.eType = eUISceneSwitchPopScene;
	tSceneSwitch.bLockedSwitch = false;
	m_lUISceneSwitchQueue.push_back(tSceneSwitch);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::popAllUIScene()
{
	unsigned int i = 0;
	unsigned int c = m_vRunningUIScenes.size();

	for(; i < c; ++i )
	{
		SUISceneSwitchInfo tSceneSwitch;
		tSceneSwitch.pScene = m_vRunningUIScenes[i];
		tSceneSwitch.eType = eUISceneSwitchPopScene;
		tSceneSwitch.bLockedSwitch = false;
		m_lUISceneSwitchQueue.push_back(tSceneSwitch);
	}
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExMgr::isSceneRunning(const char* pSceneName)
{
	vector<FKCW_SceneExMgr_SceneEx*>::iterator ritr = m_vRunningSceneStack.begin();
	vector<FKCW_SceneExMgr_SceneEx*>::iterator rend = m_vRunningSceneStack.end();
	for(; ritr != rend; ++ritr )
	{
		if( strcmp((*ritr)->getClassName(), pSceneName) == 0 )
		{
			return true;
		}
	}

	vector<FKCW_SceneExMgr_SceneEx*>::iterator uitr = m_vRunningUIScenes.begin();
	vector<FKCW_SceneExMgr_SceneEx*>::iterator uend = m_vRunningUIScenes.end();
	for(; uitr != uend; ++uitr )
	{
		if( strcmp((*uitr)->getClassName(), pSceneName) == 0 )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx* FKCW_SceneExMgr_SceneExMgr::getRunningScene() const
{
	return m_pRunningScene;
}

void FKCW_SceneExMgr_SceneExMgr::setNextScene()
{
	bool runningIsTransition = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(m_pRunningScene) != NULL;
	bool newIsTransition = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(m_pNextScene) != NULL;

	// 如果不是过渡场景，则直接调用 onExit 和 cleanup
	if (! newIsTransition)
	{
		if (m_pRunningScene)
		{
			m_pRunningScene->onExitTransitionDidStart();
			m_pRunningScene->onExit();
		}

		if (m_bSendCleanupToScene && m_pRunningScene)
		{
			m_pRunningScene->cleanup();
		}
	}

	if (m_pRunningScene)
	{
		m_pRunningScene->release();
	}
	m_pRunningScene = m_pNextScene;
	m_pNextScene->retain();
	m_pNextScene = NULL;

	if ((! runningIsTransition) && m_pRunningScene)
	{
		m_pRunningScene->onEnter();
		m_pRunningScene->onEnterTransitionDidFinish();
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::handleSceneSwitch(SSceneSwitchInfo& tSceneSwitch)
{
	switch( tSceneSwitch.eType )
	{
	case eSceneSwitchPushScene:
		{
			CCAssert(tSceneSwitch.pNextScene, "the scene should not null");
			FKCW_SceneExMgr_SceneEx* pScene = tSceneSwitch.pNextScene;
			pScene->autorelease();

			addCachableScene(pScene);

			m_bSendCleanupToScene = false;

			m_vRunningSceneStack.push_back(pScene);
			pScene->retain();
			m_pNextScene = pScene;

#if(COCOS2D_DEBUG >= 1)
			debugSceneSwitchInfo();
#endif
		}
		break;
	case eSceneSwitchReplaceScene:
		{
			CCAssert(m_pRunningScene, "Use runWithScene: instead to start the director");
			CCAssert(tSceneSwitch.pNextScene != NULL, "the scene should not be null");
			FKCW_SceneExMgr_SceneEx* pScene = tSceneSwitch.pNextScene;
			pScene->autorelease();

			addCachableScene(pScene);

			m_bSendCleanupToScene = true;
			m_vRunningSceneStack.back()->release();
			m_vRunningSceneStack.pop_back();
			m_vRunningSceneStack.push_back(pScene);
			pScene->retain();

			m_pNextScene = pScene;

#if(COCOS2D_DEBUG >= 1)
			debugSceneSwitchInfo();
#endif
		}
		break;
	case eSceneSwitchPopScene:
		{
			CCAssert(m_pRunningScene != NULL, "running scene should not null");

			m_vRunningSceneStack.back()->release();
			m_vRunningSceneStack.pop_back();
			unsigned int c = m_vRunningSceneStack.size();

			if (c == 0)
			{
				CC_SAFE_RELEASE(tSceneSwitch.pExtra);
				end();
			}
			else
			{
				setExtraToScene(m_vRunningSceneStack.back(), tSceneSwitch.pExtra);
				CC_SAFE_RELEASE(tSceneSwitch.pExtra);

				m_bSendCleanupToScene = true;
				m_pNextScene = m_vRunningSceneStack[c - 1];
			}

#if(COCOS2D_DEBUG >= 1)
			debugSceneSwitchInfo();
#endif
		}
		break;
	case eSceneSwitchPopToSceneStackLevel:
		{
			CCAssert(m_pRunningScene != NULL, "A running Scene is needed");
			int c = (int) m_vRunningSceneStack.size();
			int level = tSceneSwitch.nPopSceneStackLevel;

			if (level == 0)
			{
				CC_SAFE_RELEASE(tSceneSwitch.pExtra);
				end();
				return;
			}

			if (level >= c)
			{
				CC_SAFE_RELEASE(tSceneSwitch.pExtra);
				return;
			}

			// 清除场景堆栈，直到找到指定的层级
			while (c > level)
			{
				FKCW_SceneExMgr_SceneEx* current = m_vRunningSceneStack.back();

				if (current->isRunning())
				{
					current->onExitTransitionDidStart();
					current->onExit();
					if( current == m_pRunningScene )
					{
						current->release();
						m_pRunningScene = NULL;
					}
				}

				current->cleanup();
				m_vRunningSceneStack.back()->release();
				m_vRunningSceneStack.pop_back();
				c--;
			}

			setExtraToScene(m_vRunningSceneStack.back(), tSceneSwitch.pExtra);
			CC_SAFE_RELEASE(tSceneSwitch.pExtra);

			m_pNextScene = m_vRunningSceneStack.back();
			m_bSendCleanupToScene = false;

#if(COCOS2D_DEBUG >= 1)
			debugSceneSwitchInfo();
#endif
		}
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::unlockSceneSwitch(const char* pClassName)
{
	if( m_lSceneSwitchQueue.empty() )
		return;

	list<SSceneSwitchInfo>::iterator itr = m_lSceneSwitchQueue.begin();
	list<SSceneSwitchInfo>::iterator end = m_lSceneSwitchQueue.end();

	for(; itr != end; ++itr )
	{
		FKCW_SceneExMgr_SceneExTransition* pTransitionScene = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(itr->pNextScene);
		FKCW_SceneExMgr_SceneEx* pInScene = itr->pNextScene;

		if( pTransitionScene )
		{
			pInScene = pTransitionScene->getInScene();
		}

		if( strcmp(pInScene->getClassName(), pClassName) == 0 )
		{
			itr->bLockedSwitch = false;
			return;
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::handleUISceneSwitch(SUISceneSwitchInfo& tSceneSwitch)
{
	switch( tSceneSwitch.eType )
	{
	case eUISceneSwitchRunScene:
		{
			tSceneSwitch.pScene->autorelease();
			addCachableScene(tSceneSwitch.pScene);

			tSceneSwitch.pScene->retain();
			tSceneSwitch.pScene->onEnter();
			tSceneSwitch.pScene->onEnterTransitionDidFinish();
			m_vRunningUIScenes.push_back(tSceneSwitch.pScene);
		}
		break;
	case eUISceneSwitchPopScene:
		{
			//vector<FKCW_SceneExMgr_SceneEx*>::iterator itr = std::find(m_vRunningUIScenes.begin(), m_vRunningUIScenes.end(), tSceneSwitch.pScene);
			vector<FKCW_SceneExMgr_SceneEx*>::iterator itr = m_vRunningUIScenes.end();
			for (vector<FKCW_SceneExMgr_SceneEx*>::iterator itrTemp = m_vRunningUIScenes.begin(); 
				itrTemp != m_vRunningUIScenes.end();
				++itrTemp)
			{
				if (tSceneSwitch.pScene == *itrTemp)
				{
					itr = itrTemp;
					break;
				}
				
			}
			
			if( itr != m_vRunningUIScenes.end() )
			{
				tSceneSwitch.pScene->onExitTransitionDidStart();
				tSceneSwitch.pScene->onExit();
				tSceneSwitch.pScene->release();
				m_vRunningUIScenes.erase(itr);
			}
		}
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::unlockUISceneSwitch(const char* pClassName)
{
	if( m_lUISceneSwitchQueue.empty() )
		return;

	list<SUISceneSwitchInfo>::iterator itr = m_lUISceneSwitchQueue.begin();
	list<SUISceneSwitchInfo>::iterator end = m_lUISceneSwitchQueue.end();

	for(; itr != end; ++itr )
	{
		if( strcmp(itr->pScene->getClassName(), pClassName) == 0 )
		{
			itr->bLockedSwitch = false;
			return;
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::visitUIScenes()
{
	unsigned int i = 0;
	unsigned int c = m_vRunningUIScenes.size();

	for(; i < c; ++i )
	{
		m_vRunningUIScenes[i]->visit();
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::removeCachedScene(const char* pSceneName)
{
	map<string, FKCW_SceneExMgr_SceneEx*>::iterator itr = m_mSceneCachePool.find(pSceneName);
	if( itr != m_mSceneCachePool.end() )
	{
		itr->second->release();
		m_mSceneCachePool.erase(itr);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::removeAllCachedScenes()
{
	if( m_mSceneCachePool.empty() )
		return;

	map<string, FKCW_SceneExMgr_SceneEx*>::iterator itr = m_mSceneCachePool.begin();
	for(; itr != m_mSceneCachePool.end(); ++itr)
	{
		itr->second->release();
	}
	m_mSceneCachePool.clear();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::removeUnusedCachedScenes()
{
	if( m_mSceneCachePool.empty() )
		return;

	vector<map<string, FKCW_SceneExMgr_SceneEx*>::iterator> removeStack;
	map<string, FKCW_SceneExMgr_SceneEx*>::iterator itr = m_mSceneCachePool.begin();
	map<string, FKCW_SceneExMgr_SceneEx*>::iterator end = m_mSceneCachePool.end();

	for(; itr != end; ++itr )
	{
		if( itr->second->isSingleReference() )
		{
			itr->second->release();
			removeStack.push_back(itr);
		}
	}

	unsigned int i = 0;
	unsigned int c = m_mSceneCachePool.size();

	for(; i < c; ++i )
	{
		m_mSceneCachePool.erase(removeStack[i]);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::registerSceneClass(const char* pSceneName, Fn_CreateSceneExtension pFn)
{
	if( pFn && pSceneName && strlen(pSceneName) )
	{
		m_mFnSceneCreatePointers[pSceneName] = pFn;
	}
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx* FKCW_SceneExMgr_SceneExMgr::loadScene(const char* pSceneName)
{
	CCAssert(pSceneName && strlen(pSceneName), "should not null");

	map<string, FKCW_SceneExMgr_SceneEx*>::iterator mitr = m_mSceneCachePool.find(pSceneName);
	if( mitr != m_mSceneCachePool.end() )
	{
		return mitr->second;
	}

	map<string, Fn_CreateSceneExtension>::iterator itr = m_mFnSceneCreatePointers.find(pSceneName);
	if( itr != m_mFnSceneCreatePointers.end() )
	{
		FKCW_SceneExMgr_SceneEx* pScene = (*itr->second)();
		pScene->setClassName(pSceneName);
		pScene->init();
		pScene->autorelease();
		return pScene;
	}

	return NULL;
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx* FKCW_SceneExMgr_SceneExMgr::seekScene(const char* pSceneName)
{
	CCAssert(pSceneName && strlen(pSceneName), "should not null");

	vector<FKCW_SceneExMgr_SceneEx*>::iterator ritr = m_vRunningSceneStack.begin();
	vector<FKCW_SceneExMgr_SceneEx*>::iterator rend = m_vRunningSceneStack.end();
	for(; ritr != rend; ++ritr )
	{
		if( strcmp((*ritr)->getClassName(), pSceneName) == 0 )
		{
			return (*ritr);
		}
	}

	vector<FKCW_SceneExMgr_SceneEx*>::iterator uitr = m_vRunningUIScenes.begin();
	vector<FKCW_SceneExMgr_SceneEx*>::iterator uend = m_vRunningUIScenes.end();
	for(; uitr != uend; ++uitr )
	{
		if( strcmp((*uitr)->getClassName(), pSceneName) == 0 )
		{
			return (*uitr);
		}
	}

	map<string, FKCW_SceneExMgr_SceneEx*>::iterator mitr = m_mSceneCachePool.find(pSceneName);
	if( mitr != m_mSceneCachePool.end() )
	{
		return mitr->second;
	}

	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExMgr::loadSceneResources(FKCW_SceneExMgr_SceneEx* pScene)
{
	FKCW_SceneExMgr_SceneExTransition* pTransitionScene = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(pScene);
	FKCW_SceneExMgr_SceneEx* pInScene = pScene;

	if( pTransitionScene )
	{
		pInScene = pTransitionScene->getInScene();
	}

	if( !pInScene->isLoaded() )
	{
		pInScene->onLoadResources();
		pInScene->loadResourcesSync();

		if( pInScene->isLoadingResourcesAsync() )
		{
			pInScene->loadResourcesAsync();
			pInScene->setLoaded(true);
			return true;
		}
		else
		{
			pInScene->onLoadResourcesCompleted();
			pInScene->onLoadScene();
		}

		pInScene->setLoaded(true);
	}

	return false;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::setExtraToScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra)
{
	FKCW_SceneExMgr_SceneExTransition* pTransitionScene = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(pScene);
	FKCW_SceneExMgr_SceneEx* pInScene = pScene;

	if( pTransitionScene )
	{
		pInScene = pTransitionScene->getInScene();
	}

	if( pExtra )
	{
		pInScene->setExtraObject(pExtra);
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneExMgr::addCachableScene(FKCW_SceneExMgr_SceneEx* pScene)
{
	FKCW_SceneExMgr_SceneExTransition* pTransitionScene = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(pScene);
	FKCW_SceneExMgr_SceneEx* pInScene = pScene;

	if( pTransitionScene )
	{
		pInScene = pTransitionScene->getInScene();
	}

	map<string, FKCW_SceneExMgr_SceneEx*>::iterator mitr = m_mSceneCachePool.find(pInScene->getClassName());
	if( mitr == m_mSceneCachePool.end() && pInScene->isCachable() )
	{
		m_mSceneCachePool.insert(make_pair(pInScene->getClassName(), pInScene));
		pInScene->retain();
	}
}
//-------------------------------------------------------------------------
const char* FKCW_SceneExMgr_SceneExMgr::getSceneClassName(FKCW_SceneExMgr_SceneEx* pScene)
{
	FKCW_SceneExMgr_SceneExTransition* pTransitionScene = dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(pScene);
	FKCW_SceneExMgr_SceneEx* pInScene = pScene;

	if( pTransitionScene )
	{
		pInScene = pTransitionScene->getInScene();
	}

	return pInScene->getClassName();
}
//-------------------------------------------------------------------------
int FKCW_SceneExMgr_SceneExMgr::getTouchPriority()
{
	static int nTouchPriority = 0;
	return nTouchPriority--;
}
//-------------------------------------------------------------------------
#if(COCOS2D_DEBUG >= 1)
void FKCW_SceneExMgr_SceneExMgr::debugSceneSwitchInfo()
{
	string strStackText = "Running Scene : ";
	vector<FKCW_SceneExMgr_SceneEx*>::iterator itr = m_vRunningSceneStack.begin();
	vector<FKCW_SceneExMgr_SceneEx*>::iterator end = m_vRunningSceneStack.end();

	for(; itr != end; ++itr )
	{
		if( dynamic_cast<FKCW_SceneExMgr_SceneExTransition*>(*itr) )
		{
			strStackText.append("Transition -> ");
		}
		else
		{
			const char* pClassName = (*itr)->getClassName();
			strStackText.append(pClassName).append(" -> ");
		}
	}
	strStackText.erase(strStackText.size() - 4);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SetWindowTextA(CCEGLView::sharedOpenGLView()->getHWnd(), strStackText.c_str());
	CCLOG(strStackText.c_str());
#else
	CCLOG(strStackText.c_str());
#endif
}
#endif
//-------------------------------------------------------------------------
