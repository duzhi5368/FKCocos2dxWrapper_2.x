//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_WidgetWindow.h"
#if FKCW_WIDGET_USING_LUA
#include "CCLuaEngine.h"
#endif
//-------------------------------------------------------------------------
FKCW_UIWidget_WidgetWindow::FKCW_UIWidget_WidgetWindow()
	: m_bIsTouched(false)
	, m_nTouchPriority(0)
	, m_bModalable(false)
	, m_bTouchEnabled(true)
	, m_pSelectedWidget(NULL)
	, m_fTouchedDuration(0.0f)
	, m_pTouchMovedAfterLongClickListener(NULL)
	, m_pTouchEndedAfterLongClickListener(NULL)
	, m_pTouchCancelledAfterLongClickListener(NULL)
	, m_pTouchMovedAfterLongClickHandler(NULL)
	, m_pTouchEndedAfterLongClickHandler(NULL)
	, m_pTouchCancelledAfterLongClickHandler(NULL)
	, m_pLongClickedWidgetObject(NULL)
	, m_bMultiTouchEnabled(false)
#if FKCW_WIDGET_USING_LUA
	, m_nTouchMovedAfterLongClickScriptHandler(0)
	, m_nTouchEndedAfterLongClickScriptHandler(0)
	, m_pTouchCancelledAfterLongClickScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_WidgetWindow::~FKCW_UIWidget_WidgetWindow()
{
#if FKCW_WIDGET_USING_LUA
	removeOnTouchMovedAfterLongClickScriptHandler();
	removeOnTouchEndedAfterLongClickScriptHandler();
	removeOnTouchCancelledAfterLongClickScriptHandler();
#endif
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_WidgetWindow::getTouchPriority()
{
	return m_nTouchPriority;
}
//-------------------------------------------------------------------------
CCObject* FKCW_UIWidget_WidgetWindow::findWidgetById(const char* id)
{
	CCAssert(id && strlen(id), "should not null");

	return find(m_pChildren, id);
}
//-------------------------------------------------------------------------
CCObject* FKCW_UIWidget_WidgetWindow::find(CCArray* pChidren, const char* id)
{
	if( pChidren && pChidren->count() > 0 )
	{
		unsigned int nCount = pChidren->count();
		for(unsigned int i = 0; i < nCount; ++i)
		{
			CCObject* pObject = pChidren->objectAtIndex(i);
			FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
			if( pWidget )
			{
				if( strcmp(pWidget->getId(), id) == 0 )
				{
					return pObject;
				}
				else
				{
					CCObject* pRet = find(dynamic_cast<CCNode*>(pObject)->getChildren(), id);
					if( pRet )
					{
						return pRet;
					}
				}
			}
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setLongClickTouchHandlerWidget(CCObject* pWidgetObject, int nTouchId)
{
	if( m_bMultiTouchEnabled && !m_mMultiTouchKeeper.empty() )
	{
		map<int, SMultiTouchTarget>::iterator itr = m_mMultiTouchKeeper.find(nTouchId);
		if( itr != m_mMultiTouchKeeper.end() )
		{
			itr->second.pLongClickedWidgetObject = pWidgetObject;
		}
	}
	else
	{
		m_pLongClickedWidgetObject = pWidgetObject;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setOnTouchMovedAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler)
{
	m_pTouchMovedAfterLongClickListener = pListener;
	m_pTouchMovedAfterLongClickHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setOnTouchEndedAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler)
{
	m_pTouchEndedAfterLongClickListener = pListener;
	m_pTouchEndedAfterLongClickHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setOnTouchCancelledAfterLongClickListener(CCObject* pListener, SEL_AfterLongClickHandler pHandler)
{
	m_pTouchCancelledAfterLongClickListener = pListener;
	m_pTouchCancelledAfterLongClickHandler = pHandler;
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_WidgetWindow::setOnTouchMovedAfterLongClickScriptHandler(int nHandler)
{
	removeOnTouchMovedAfterLongClickScriptHandler();
	m_nTouchMovedAfterLongClickScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setOnTouchEndedAfterLongClickScriptHandler(int nHandler)
{
	removeOnTouchEndedAfterLongClickScriptHandler();
	m_nTouchEndedAfterLongClickScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setOnTouchCancelledAfterLongClickScriptHandler(int nHandler)
{
	removeOnTouchCancelledAfterLongClickScriptHandler();
	m_pTouchCancelledAfterLongClickScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::removeOnTouchMovedAfterLongClickScriptHandler()
{
	if( m_nTouchMovedAfterLongClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchMovedAfterLongClickScriptHandler);
		m_nTouchMovedAfterLongClickScriptHandler = 0;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::removeOnTouchEndedAfterLongClickScriptHandler()
{
	if( m_nTouchEndedAfterLongClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchEndedAfterLongClickScriptHandler);
		m_nTouchEndedAfterLongClickScriptHandler = 0;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::removeOnTouchCancelledAfterLongClickScriptHandler()
{
	if( m_pTouchCancelledAfterLongClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_pTouchCancelledAfterLongClickScriptHandler);
		m_pTouchCancelledAfterLongClickScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::executeTouchMovedAfterLongClickHandler(CCObject* pSender, CCTouch *pTouch, float fDuration)
{
	if( m_pTouchMovedAfterLongClickListener && m_pTouchMovedAfterLongClickHandler )
	{
		(m_pTouchMovedAfterLongClickListener->*m_pTouchMovedAfterLongClickHandler)(pSender, pTouch, fDuration);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchMovedAfterLongClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		int nRet = pStack->executeFunctionByHandler(m_nTouchMovedAfterLongClickScriptHandler, 3);
		pStack->clean();
	}
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::executeTouchEndedAfterLongClickHandler(CCObject* pSender, CCTouch *pTouch, float fDuration)
{
	if( m_pTouchEndedAfterLongClickListener && m_pTouchEndedAfterLongClickHandler )
	{
		(m_pTouchEndedAfterLongClickListener->*m_pTouchEndedAfterLongClickHandler)(pSender, pTouch, fDuration);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchEndedAfterLongClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		int nRet = pStack->executeFunctionByHandler(m_nTouchEndedAfterLongClickScriptHandler, 3);
		pStack->clean();
	}
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::executeTouchCancelledAfterLongClickHandler(CCObject* pSender, CCTouch *pTouch, float fDuration)
{
	if( m_pTouchCancelledAfterLongClickListener && m_pTouchCancelledAfterLongClickHandler )
	{
		(m_pTouchCancelledAfterLongClickListener->*m_pTouchCancelledAfterLongClickHandler)(pSender, pTouch, fDuration);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_pTouchCancelledAfterLongClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		int nRet = pStack->executeFunctionByHandler(m_pTouchCancelledAfterLongClickScriptHandler, 3);
		pStack->clean();
	}
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setTouchPriority(int nTouchPriority)
{
	if( m_nTouchPriority != nTouchPriority )
	{
		m_nTouchPriority = nTouchPriority;
		if( m_bTouchEnabled && m_bRunning )
		{
			CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
			pDispatcher->setPriority(nTouchPriority, this);
		}
	}
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_WidgetWindow::isMultiTouchEnabled() const
{
	return m_bMultiTouchEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setMultiTouchEnabled(bool bEnabled)
{
	if( m_bMultiTouchEnabled != bEnabled )
	{
		setTouchEnabled(false);
		m_bMultiTouchEnabled = bEnabled;
		setTouchEnabled(true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setModalable(bool bModalable)
{
	m_bModalable = bModalable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_WidgetWindow::isModalable() const
{
	return m_bModalable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_WidgetWindow::isTouchEnabled()
{
	return m_bTouchEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::setTouchEnabled(bool bTouchEnabled)
{
	if( m_bTouchEnabled != bTouchEnabled )
	{
		m_bTouchEnabled = bTouchEnabled;
		if( m_bRunning )
		{
			if( bTouchEnabled )
			{
				if( m_bMultiTouchEnabled )
				{
					CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, m_nTouchPriority);
				}
				else
				{
					CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_nTouchPriority, true);
				}
			}
			else
			{
				CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
			}
		}
	}
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_WidgetWindow::init()
{
	if( CCNode::init() )
	{
		CCSize tWinSize = CCDirector::sharedDirector()->getWinSize();

		this->ignoreAnchorPointForPosition(true);
		this->setContentSize(tWinSize);
		this->setAnchorPoint(CCPointZero);
		this->setPosition(CCPointZero);

		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::visit()
{
	if( m_bIsTouched )
	{
		m_fTouchedDuration += CCDirector::sharedDirector()->getDeltaTime();
	}

	if( !m_mMultiTouchKeeper.empty() )
	{
		map<int, SMultiTouchTarget>::iterator itr = m_mMultiTouchKeeper.begin();
		for(; itr != m_mMultiTouchKeeper.end(); ++itr)
		{
			itr->second.fTouchedDuration += CCDirector::sharedDirector()->getDeltaTime();
		}
	}

	CCNode::visit();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::onEnter()
{
	if( m_bTouchEnabled )
	{
		if( m_bMultiTouchEnabled )
		{
			CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, m_nTouchPriority);
		}
		else
		{
			CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_nTouchPriority, true);
		}
	}
	CCNode::onEnter();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::onExit()
{
	if( m_bTouchEnabled )
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	}
	CCNode::onExit();
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_WidgetWindow::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if( m_bTouchEnabled && m_bVisible && m_pChildren && m_pChildren->count() > 0 )
	{
		CCPoint touchPointInView = convertToNodeSpace(pTouch->getLocation());
		if( m_pChildren && m_pChildren->count() > 0 )
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH_REVERSE( m_pChildren, pObject )
			{
				CCNode* pNode = dynamic_cast<CCNode*>(pObject);
				FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
				if( pWidget && pNode->isVisible() && pWidget->isEnabled() && pWidget->isTouchEnabled() )
				{
					if( pNode->boundingBox().containsPoint(touchPointInView) )
					{
						if( pWidget->executeTouchBeganHandler(pTouch) != eWidgetTouchNone )
						{
							m_pSelectedWidget = pWidget;
							m_bIsTouched = true;
							m_fTouchedDuration = 0.0f;
							return true;
						}
					}
				}
			}
			m_pSelectedWidget = NULL;
		}
	}
	return m_bModalable;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if( m_pSelectedWidget )
	{
		if( m_pSelectedWidget->isTouchInterrupted() )
		{
			if( m_pLongClickedWidgetObject )
			{
				executeTouchMovedAfterLongClickHandler(m_pLongClickedWidgetObject, pTouch, m_fTouchedDuration);
			}
		}
		else
		{
			m_pSelectedWidget->executeTouchMovedHandler(pTouch, m_fTouchedDuration);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if( m_pSelectedWidget )
	{
		if( m_pSelectedWidget->isTouchInterrupted() )
		{
			if( m_pLongClickedWidgetObject )
			{
				executeTouchEndedAfterLongClickHandler(m_pLongClickedWidgetObject, pTouch, m_fTouchedDuration);
			}
		}
		else
		{
			m_pSelectedWidget->executeTouchEndedHandler(pTouch, m_fTouchedDuration);
		}
	}
	m_bIsTouched = false;
	m_fTouchedDuration = 0.0f;
	m_pSelectedWidget = NULL;
	m_pLongClickedWidgetObject = NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	if( m_pSelectedWidget )
	{
		if( m_pSelectedWidget->isTouchInterrupted() )
		{
			if( m_pLongClickedWidgetObject )
			{
				executeTouchCancelledAfterLongClickHandler(m_pLongClickedWidgetObject, pTouch, m_fTouchedDuration);
			}
		}
		else
		{
			m_pSelectedWidget->interruptTouchCascade(pTouch, m_fTouchedDuration);
		}
	}
	m_bIsTouched = false;
	m_fTouchedDuration = 0.0f;
	m_pSelectedWidget = NULL;
	m_pLongClickedWidgetObject = NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator itr = pTouches->begin();
	for(; itr != pTouches->end(); ++itr)
	{
		CCTouch* pTouch = (CCTouch*)(*itr);
		if( m_bTouchEnabled && m_bMultiTouchEnabled && m_bVisible && m_pChildren && m_pChildren->count() > 0 )
		{
			CCPoint touchPointInView = convertToNodeSpace(pTouch->getLocation());
			if( m_pChildren && m_pChildren->count() > 0 )
			{
				CCObject* pObject = NULL;
				CCARRAY_FOREACH_REVERSE( m_pChildren, pObject )
				{
					CCNode* pNode = dynamic_cast<CCNode*>(pObject);
					FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
					if( pWidget && pNode->isVisible() && pWidget->isEnabled() && pWidget->isTouchEnabled() )
					{
						bool bSameWidgetBreak = false;
						if( pNode->boundingBox().containsPoint(touchPointInView) )
						{
							// 保证它不会在同一个组件上发生
							map<int, SMultiTouchTarget>::iterator mitr = m_mMultiTouchKeeper.begin();
							for(; mitr != m_mMultiTouchKeeper.end(); ++mitr)
							{
								if( mitr->second.pWidget == pWidget )
								{
									bSameWidgetBreak = true;
									break;
								}
							}

							if( bSameWidgetBreak )
							{
								break;
							}

							if( pWidget->executeTouchBeganHandler(pTouch) != eWidgetTouchNone )
							{
								SMultiTouchTarget tKeeper;
								tKeeper.fTouchedDuration = 0.0f;
								tKeeper.pWidget = pWidget;
								tKeeper.pLongClickedWidgetObject = NULL;
								m_mMultiTouchKeeper[pTouch->getID()] = tKeeper;
								return;
							}
						}
					}
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator itr = pTouches->begin();
	for(; itr != pTouches->end(); ++itr)
	{
		CCTouch* pTouch = (CCTouch*)(*itr);
		map<int, SMultiTouchTarget>::iterator mitr = m_mMultiTouchKeeper.find(pTouch->getID());
		if( mitr != m_mMultiTouchKeeper.end() )
		{
			if( mitr->second.pWidget->isTouchInterrupted() )
			{
				if( mitr->second.pLongClickedWidgetObject )
				{
					executeTouchMovedAfterLongClickHandler(mitr->second.pLongClickedWidgetObject, pTouch, mitr->second.fTouchedDuration);
				}
			}
			else
			{
				mitr->second.pWidget->executeTouchMovedHandler(pTouch, mitr->second.fTouchedDuration);
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator itr = pTouches->begin();
	for(; itr != pTouches->end(); ++itr)
	{
		CCTouch* pTouch = (CCTouch*)(*itr);
		map<int, SMultiTouchTarget>::iterator mitr = m_mMultiTouchKeeper.find(pTouch->getID());
		if( mitr != m_mMultiTouchKeeper.end() )
		{
			if( mitr->second.pWidget )
			{
				if( mitr->second.pWidget->isTouchInterrupted() )
				{
					if( mitr->second.pLongClickedWidgetObject )
					{
						executeTouchEndedAfterLongClickHandler(mitr->second.pLongClickedWidgetObject, pTouch, mitr->second.fTouchedDuration);
					}
				}
				else
				{
					mitr->second.pWidget->executeTouchEndedHandler(pTouch, mitr->second.fTouchedDuration);
				}
			}
			m_mMultiTouchKeeper.erase(mitr);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_WidgetWindow::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator itr = pTouches->begin();
	for(; itr != pTouches->end(); ++itr)
	{
		CCTouch* pTouch = (CCTouch*)(*itr);
		map<int, SMultiTouchTarget>::iterator mitr = m_mMultiTouchKeeper.find(pTouch->getID());
		if( mitr != m_mMultiTouchKeeper.end() )
		{
			if( mitr->second.pWidget )
			{
				if( mitr->second.pWidget->isTouchInterrupted() )
				{
					if( mitr->second.pLongClickedWidgetObject )
					{
						executeTouchCancelledAfterLongClickHandler(mitr->second.pLongClickedWidgetObject, pTouch, mitr->second.fTouchedDuration);
					}
				}
				else
				{
					mitr->second.pWidget->interruptTouchCascade(pTouch, mitr->second.fTouchedDuration);
				}
			}
			m_mMultiTouchKeeper.erase(mitr);
		}
	}
}
//-------------------------------------------------------------------------
FKCW_UIWidget_WidgetWindow* FKCW_UIWidget_WidgetWindow::create()
{
	FKCW_UIWidget_WidgetWindow *pRet = new FKCW_UIWidget_WidgetWindow();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------