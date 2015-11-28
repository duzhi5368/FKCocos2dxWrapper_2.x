//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_Widget.h"
#include "../Include/FKCW_UIWidget_WidgetWindow.h"
#if FKCW_WIDGET_USING_LUA
#include "CCLuaEngine.h"
#endif
//-------------------------------------------------------------------------
FKCW_UIWidget_Widget::FKCW_UIWidget_Widget()
	: m_strId("")
	, m_strDescription("")
	, m_pThisObject(NULL)
	, m_bEnabled(true)
	, m_bTouchEnabled(true)
	, m_pTouchBeganListener(NULL)
	, m_pTouchMovedListener(NULL)
	, m_pTouchEndedListener(NULL)
	, m_pTouchCancelledListener(NULL)
	, m_pTouchBeganHandler(NULL)
	, m_pTouchMovedHandler(NULL)
	, m_pTouchEndedHandler(NULL)
	, m_pTouchCancelledHandler(NULL)
	, m_bTouchInterrupt(false)
	, m_nUserTag(kCCNodeTagInvalid)
#if FKCW_WIDGET_USING_LUA
	, m_nTouchBeganScriptHandler(0)
	, m_nTouchMovedScriptHandler(0)
	, m_nTouchEndedScriptHandler(0)
	, m_nTouchCancelledScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_Widget::~FKCW_UIWidget_Widget()
{
#if FKCW_WIDGET_USING_LUA
	removeOnTouchBeganScriptHandler();
	removeOnTouchMovedScriptHandler();
	removeOnTouchEndedScriptHandler();
	removeOnTouchCancelledScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setThisObject(CCObject* pThis)
{
	m_pThisObject = pThis;
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_Widget::getUserTag() const
{
	return m_nUserTag;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setUserTag(int nTag)
{
	m_nUserTag = nTag;
}
//-------------------------------------------------------------------------
const char* FKCW_UIWidget_Widget::getId()
{
	return m_strId.c_str();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setId(const char* id)
{
	m_strId = id;
}
//-------------------------------------------------------------------------
const char* FKCW_UIWidget_Widget::getDescription()
{
	return m_strDescription.c_str();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setDescription(const char* description)
{
	m_strDescription = description;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Widget::isEnabled()
{
	return m_bEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Widget::isTouchEnabled()
{
	return m_bTouchEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setTouchEnabled(bool bTouchEnabled)
{
	m_bTouchEnabled = bTouchEnabled;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_Widget::executeTouchBeganHandler(CCTouch* pTouch)
{
	m_bTouchInterrupt = false;

	if( m_pTouchBeganListener && m_pTouchBeganHandler )
	{
		ENUM_WidgetTouchModel eUserTouchModel = (m_pTouchBeganListener->*m_pTouchBeganHandler)(m_pThisObject, pTouch);
		if( eUserTouchModel == eWidgetTouchNone )
		{
			return eWidgetTouchNone;
		}
		else
		{
			this->onTouchBegan(pTouch);
			return eUserTouchModel;
		}
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchBeganScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(m_pThisObject, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nTouchBeganScriptHandler, 2, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return count = 0");

		CCDouble* pIntModel = (CCDouble*) pRetArray->objectAtIndex(0);
		ENUM_WidgetTouchModel eUserTouchModel = (ENUM_WidgetTouchModel) ( (int)pIntModel->getValue() );
		delete pRetArray;
		pStack->clean();

		if( eUserTouchModel == eWidgetTouchNone )
		{
			return eWidgetTouchNone;
		}
		else
		{
			this->onTouchBegan(pTouch);
			return eUserTouchModel;
		}
	}
#endif
	return this->onTouchBegan(pTouch);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::executeTouchMovedHandler(CCTouch* pTouch, float fDuration)
{
	if( m_pTouchMovedListener && m_pTouchMovedHandler )
	{
		if( !(m_pTouchMovedListener->*m_pTouchMovedHandler)(m_pThisObject, pTouch, fDuration) )
		{
			return;
		}
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchMovedScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(m_pThisObject, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nTouchMovedScriptHandler, 3, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return count = 0");

		CCBool* pBool = (CCBool*) pRetArray->objectAtIndex(0);
		bool bContinue = pBool->getValue();
		delete pRetArray;
		pStack->clean();

		if(!bContinue)
		{
			return;
		}
	}
#endif
	this->onTouchMoved(pTouch, fDuration);
	return;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::executeTouchEndedHandler(CCTouch* pTouch, float fDuration)
{
	if( m_pTouchEndedListener && m_pTouchEndedHandler )
	{
		if( !(m_pTouchEndedListener->*m_pTouchEndedHandler)(m_pThisObject, pTouch, fDuration) )
		{
			return;
		}
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchEndedScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(m_pThisObject, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nTouchEndedScriptHandler, 3, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return count = 0");

		CCBool* pBool = (CCBool*) pRetArray->objectAtIndex(0);
		bool bContinue = pBool->getValue();
		delete pRetArray;
		pStack->clean();

		if(!bContinue)
		{
			return;
		}
	}
#endif
	this->onTouchEnded(pTouch, fDuration);
	return;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::executeTouchCancelledHandler(CCTouch* pTouch, float fDuration)
{
	if( m_pTouchCancelledListener && m_pTouchCancelledHandler )
	{
		if( !(m_pTouchCancelledListener->*m_pTouchCancelledHandler)(m_pThisObject, pTouch, fDuration) )
		{
			return;
		}
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nTouchCancelledScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(m_pThisObject, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");
		pStack->pushFloat(fDuration);

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nTouchCancelledScriptHandler, 3, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return count = 0");

		CCBool* pBool = (CCBool*) pRetArray->objectAtIndex(0);
		bool bContinue = pBool->getValue();
		delete pRetArray;
		pStack->clean();

		if(!bContinue)
		{
			return;
		}
	}
#endif
	this->onTouchCancelled(pTouch, fDuration);
	return;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchBeganListener(CCObject* pListener, SEL_TouchBeganHandler pHandler)
{
	m_pTouchBeganListener = pListener;
	m_pTouchBeganHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchMovedListener(CCObject* pListener, SEL_TouchEventHandler pHandler)
{
	m_pTouchMovedListener = pListener;
	m_pTouchMovedHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchEndedListener(CCObject* pListener, SEL_TouchEventHandler pHandler)
{
	m_pTouchEndedListener = pListener;
	m_pTouchEndedHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchCancelledListener(CCObject* pListener, SEL_TouchEventHandler pHandler)
{
	m_pTouchCancelledListener = pListener;
	m_pTouchCancelledHandler = pHandler;
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_Widget::setOnTouchBeganScriptHandler(int nHandler)
{
	removeOnTouchBeganScriptHandler();
	m_nTouchBeganScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchMovedScriptHandler(int nHandler)
{
	removeOnTouchMovedScriptHandler();
	m_nTouchMovedScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchEndedScriptHandler(int nHandler)
{
	removeOnTouchEndedScriptHandler();
	m_nTouchEndedScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setOnTouchCancelledScriptHandler(int nHandler)
{
	removeOnTouchCancelledScriptHandler();
	m_nTouchCancelledScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::removeOnTouchBeganScriptHandler()
{
	if( m_nTouchBeganScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchBeganScriptHandler);
		m_nTouchBeganScriptHandler = 0;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::removeOnTouchMovedScriptHandler()
{
	if( m_nTouchMovedScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchMovedScriptHandler);
		m_nTouchMovedScriptHandler = 0;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::removeOnTouchEndedScriptHandler()
{
	if( m_nTouchEndedScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchEndedScriptHandler);
		m_nTouchEndedScriptHandler = 0;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::removeOnTouchCancelledScriptHandler()
{
	if( m_nTouchCancelledScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nTouchCancelledScriptHandler);
		m_nTouchCancelledScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::interruptTouch(CCTouch* pTouch, float fDuration)
{
	if( !m_bTouchInterrupt )
	{
		m_bTouchInterrupt = true;
		executeTouchCancelledHandler(pTouch, fDuration);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::interruptTouchCascade(CCTouch* pTouch, float fDuration)
{
	for(CCNode* pNode = (CCNode*)m_pThisObject; pNode != NULL; pNode = pNode->getParent())
	{
		FKCW_UIWidget_Widget* pWidgetParent = dynamic_cast<FKCW_UIWidget_Widget*>(pNode);
		if( pWidgetParent != NULL )
		{
			pWidgetParent->interruptTouch(pTouch, fDuration);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Widget::setLongClickTouchHandlerWidget(CCNode* pWidget, int id)
{
	for(CCNode* pNode = (CCNode*)m_pThisObject; pNode != NULL; pNode = pNode->getParent())
	{
		FKCW_UIWidget_WidgetWindow* pWidgetWindow = dynamic_cast<FKCW_UIWidget_WidgetWindow*>(pNode);
		if( pWidgetWindow != NULL )
		{
			pWidgetWindow->setLongClickTouchHandlerWidget(pWidget, id);
			return;
		}
	}
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Widget::isTouchInterrupted()
{
	return m_bTouchInterrupt;
}
//-------------------------------------------------------------------------