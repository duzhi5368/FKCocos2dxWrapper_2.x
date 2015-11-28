//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneEx.h"
#include "../Include/FKCW_SceneExMgr_SceneExMgr.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx::FKCW_SceneExMgr_SceneEx()
	: m_bCachable(false)
	, m_pExtraObject(NULL)
	, m_strClassName("")
	, m_bLoaded(false)
	, m_nLoadResourcesAsyncCount(0)
	, m_nLoadResourcesAsyncIdx(0)
	, m_bAutoRemoveUnusedTexture(false)
{
	setAnchorPoint(CCPointZero);
	setContentSize(CCDirector::sharedDirector()->getWinSize());
}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneEx::~FKCW_SceneExMgr_SceneEx()
{
	CC_SAFE_RELEASE(m_pExtraObject);
#if COCOS2D_DEBUG >= 1
	if( !m_strClassName.empty() )
	{
		CCLOG("[DEBUG] : destroy Scene %s", m_strClassName.c_str());
	}
#endif
	if( m_bAutoRemoveUnusedTexture )
	{
		removeAllChildrenWithCleanup(true);
		CCLOG("[DEBUG] : -------------------Begin FreeImage-------------------");
		CCTextureCache::sharedTextureCache()->removeUnusedTextures();
		CCLOG("[DEBUG] : -------------------Cached Image-------------------");
		CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
		CCLOG("[DEBUG] : --------------------End FreeImage--------------------");
	}
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::onEnter()
{
#if USING_FKCW_WIDGET
	FKCW_SceneExMgr_MsgManager::sharedManager()->RegisterMessageDelegate(this);
	onEnterScene();
	FKCW_UI_WidgetWindow::onEnter();
#else
	FKCW_SceneExMgr_MsgManager::sharedManager()->RegisterMsgDelegate(this);
	onEnterScene();
	CCNode::onEnter();
#endif
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::onExit()
{
	FKCW_SceneExMgr_MsgManager::sharedManager()->UnregisterMsgDelegate(this);
#if USING_FKCW_WIDGET
	onExitScene();
	FKCW_UI_WidgetWindow::onExit();
#else
	onExitScene();
	CCNode::onExit();
#endif
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneEx::isAutoRemoveUnusedTexture() const
{
	return m_bAutoRemoveUnusedTexture;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::setAutoRemoveUnusedTexture(bool bAuto)
{
	m_bAutoRemoveUnusedTexture = bAuto;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneEx::isCachable() const
{
	return m_bCachable;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::setCachable(bool bCachable)
{
	m_bCachable = bCachable;
}
//-------------------------------------------------------------------------
CCObject* FKCW_SceneExMgr_SceneEx::getExtraObject() const
{
	return m_pExtraObject;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::setExtraObject(CCObject* pExtraObject)
{
	CC_SAFE_RETAIN(pExtraObject);
	CC_SAFE_RELEASE(m_pExtraObject);
	m_pExtraObject = pExtraObject;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::setClassName(const char* pClassName)
{
	m_strClassName = pClassName;
}
//-------------------------------------------------------------------------
const char* FKCW_SceneExMgr_SceneEx::getClassName()
{
	return m_strClassName.c_str();
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneEx::isLoaded() const
{
	return m_bLoaded;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::setLoaded(bool bLoaded)
{
	m_bLoaded = bLoaded;
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::addImage(const char* pFile)
{
	CCAssert(pFile && strlen(pFile), "file path invalidate");
	m_vLoadImageFilesSync.push_back(pFile);
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::addImageAsync(const char* pFile)
{
	CCAssert(pFile && strlen(pFile), "file path invalidate");
	m_vLoadImageFilesAsync.push_back(pFile);
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneEx::isLoadingResourcesAsync()
{
	return !m_vLoadImageFilesAsync.empty();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::loadResourcesSync()
{
	if( m_vLoadImageFilesSync.empty() )
		return;

	vector<string>::iterator itr = m_vLoadImageFilesSync.begin();
	vector<string>::iterator end = m_vLoadImageFilesSync.end();

	for(; itr != end; ++itr)
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(itr->c_str());
		CCAssert(pTexture, "load image failed");
#if COCOS2D_DEBUG >= 1
		CCLOG("[DEBUG] : Scene [%s] load image sync %s", m_strClassName.c_str(), itr->c_str());
#endif
	}

	m_vLoadImageFilesSync.clear();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::loadResourcesAsync()
{
	CCAssert(!m_vLoadImageFilesAsync.empty(), "empty");

	CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);

	m_nLoadResourcesAsyncCount = m_vLoadImageFilesAsync.size();
	m_nLoadResourcesAsyncIdx = 0;

	vector<string>::iterator itr = m_vLoadImageFilesAsync.begin();
	vector<string>::iterator end = m_vLoadImageFilesAsync.end();

	for(; itr != end; ++itr)
	{
		CCTextureCache::sharedTextureCache()->addImageAsync(
			itr->c_str(), 
			this, 
			callfuncO_selector(FKCW_SceneExMgr_SceneEx::loadingResourcesCallBack));
#if COCOS2D_DEBUG >= 1
		CCLOG("[DEBUG] : Scene [%s] load image async %s", m_strClassName.c_str(), itr->c_str());
#endif
	}

	m_vLoadImageFilesAsync.clear();
}
//-------------------------------------------------------------------------
void FKCW_SceneExMgr_SceneEx::loadingResourcesCallBack(CCObject* pTextureObj)
{
	CCAssert(pTextureObj, "load resources async failed");

	m_nLoadResourcesAsyncIdx ++;

	if( m_nLoadResourcesAsyncIdx == m_nLoadResourcesAsyncCount )
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);

		m_nLoadResourcesAsyncCount = 0;
		m_nLoadResourcesAsyncIdx = 0;
		onLoadResourcesCompleted();
		onLoadScene();

		FKCW_SceneExMgr_SceneExMgr::sharedManager()->unlockSceneSwitch(m_strClassName.c_str());
		FKCW_SceneExMgr_SceneExMgr::sharedManager()->unlockUISceneSwitch(m_strClassName.c_str());
	}
}
//-------------------------------------------------------------------------