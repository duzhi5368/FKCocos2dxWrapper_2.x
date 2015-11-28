//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_Protocol.h"
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
#include "CCLuaEngine.h"
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_DataSourceAdapterProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_DataSourceAdapterProtocol::FKCW_UIWidget_DataSourceAdapterProtocol()
	: m_pDataSourceAdapterListener(NULL)
	, m_pDataSourceAdapterHandler(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nDataSourceAdapterScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_DataSourceAdapterProtocol::~FKCW_UIWidget_DataSourceAdapterProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeDataSourceAdapterScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_DataSourceAdapterProtocol::setDataSourceAdapter(CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler)
{
	m_pDataSourceAdapterListener = pListener;
	m_pDataSourceAdapterHandler = pHandler;
}
//-------------------------------------------------------------------------
CCObject* FKCW_UIWidget_DataSourceAdapterProtocol::executeDataSourceAdapterHandler(CCObject* pConvertCell, unsigned int uIdx)
{
	if( m_pDataSourceAdapterListener && m_pDataSourceAdapterHandler )
	{
		return (m_pDataSourceAdapterListener->*m_pDataSourceAdapterHandler)(pConvertCell, uIdx);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nDataSourceAdapterScriptHandler != 0 )
	{
		return executeDataSourceAdapterScriptHandler(pConvertCell, uIdx);
	}
#endif
	return NULL;
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
CCObject* FKCW_UIWidget_DataSourceAdapterProtocol::executeDataSourceAdapterScriptHandler(CCObject* pConvertCell, unsigned int uIdx)
{
	if( m_nDataSourceAdapterScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		if( pConvertCell )
		{
			pStack->pushCCObject(pConvertCell, "CCObject");
		}
		else
		{
			pStack->pushNil();
		}
		pStack->pushInt(uIdx);

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nDataSourceAdapterScriptHandler, 2, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return num = 0");

		CCObject* pReturnObject = pRetArray->objectAtIndex(0);
		delete pRetArray;
		pStack->clean();

		return pReturnObject;
	}
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_DataSourceAdapterProtocol::setDataSourceAdapterScriptHandler(int nHandler)
{
	removeDataSourceAdapterScriptHandler();
	m_nDataSourceAdapterScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_DataSourceAdapterProtocol::removeDataSourceAdapterScriptHandler()
{
	if( m_nDataSourceAdapterScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nDataSourceAdapterScriptHandler);
		m_nDataSourceAdapterScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_TextRichClickableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_TextRichClickableProtocol::FKCW_UIWidget_TextRichClickableProtocol()
	: m_pRichTextClickListener(NULL)
	, m_pRichTextClickHandler(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nRichTextClickScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_TextRichClickableProtocol::~FKCW_UIWidget_TextRichClickableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnTextRichClickScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRichClickableProtocol::setOnTextRichClickListener(CCObject* pListener, SEL_TextRichClickHandler pHandler)
{
	m_pRichTextClickListener = pListener;
	m_pRichTextClickHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRichClickableProtocol::executeTextRichClickHandler(CCObject* pSender, const char* pDescription)
{
	if( m_pRichTextClickListener && m_pRichTextClickHandler )
	{
		(m_pRichTextClickListener->*m_pRichTextClickHandler)(pSender, pDescription);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nRichTextClickScriptHandler != 0 )
	{
		executeTextRichScriptHandler(pSender, pDescription);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_TextRichClickableProtocol::executeTextRichScriptHandler(CCObject* pSender, const char* pDescription)
{
	if( m_nRichTextClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");

		if( pDescription )
		{
			pStack->pushString(pDescription);
		}
		else
		{
			pStack->pushNil();
		}

		pStack->executeFunctionByHandler(m_nRichTextClickScriptHandler, 2);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRichClickableProtocol::setOnTextRichClickScriptHandler(int nHandler)
{
	removeOnTextRichClickScriptHandler();
	m_nRichTextClickScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRichClickableProtocol::removeOnTextRichClickScriptHandler()
{
	if( m_nRichTextClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nRichTextClickScriptHandler);
		m_nRichTextClickScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_LayoutableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_LayoutableProtocol::FKCW_UIWidget_LayoutableProtocol()
	: m_pSelectedWidget(NULL)
	, m_eSelectedWidgetTouchModel(eWidgetTouchNone)
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_LayoutableProtocol::~FKCW_UIWidget_LayoutableProtocol()
{

}
//-------------------------------------------------------------------------
// FKCW_UIWidget_PageChangeableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_PageChangeableProtocol::FKCW_UIWidget_PageChangeableProtocol()
	: m_pPageChangedListener(NULL)
	, m_pPageChangedHandler(NULL)
	, m_nPageIndex(0)
#if FKCW_WIDGET_USING_LUA
	, m_nPageChangedScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_PageChangeableProtocol::~FKCW_UIWidget_PageChangeableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnPageChangedScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageChangeableProtocol::setOnPageChangedListener(CCObject* pListener, SEL_PageChangedHandler pHandler)
{
	m_pPageChangedListener = pListener;
	m_pPageChangedHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageChangeableProtocol::executePageChangedHandler(CCObject* pSender, unsigned int uPageIdx)
{
	if( m_pPageChangedListener && m_pPageChangedHandler )
	{
		(m_pPageChangedListener->*m_pPageChangedHandler)(pSender, uPageIdx);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nPageChangedScriptHandler != 0 )
	{
		executePageChangedScriptHandler(pSender, uPageIdx);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_PageChangeableProtocol::executePageChangedScriptHandler(CCObject* pSender, unsigned int uPageIdx)
{
	if( m_nPageChangedScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushInt(uPageIdx);

		pStack->executeFunctionByHandler(m_nPageChangedScriptHandler, 2);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageChangeableProtocol::setOnPageChangedScriptHandler(int nHandler)
{
	removeOnPageChangedScriptHandler();
	m_nPageChangedScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageChangeableProtocol::removeOnPageChangedScriptHandler()
{
	if( m_nPageChangedScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nPageChangedScriptHandler);
		m_nPageChangedScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_ScrollableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollableProtocol::FKCW_UIWidget_ScrollableProtocol()
	: m_pScrollingListener(NULL)
	, m_pScrollingHandler(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nScrollingScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollableProtocol::~FKCW_UIWidget_ScrollableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnScrollingScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollableProtocol::setOnScrollingListener(CCObject* pListener, SEL_ScrollingHandler pHandler)
{
	m_pScrollingListener = pListener;
	m_pScrollingHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollableProtocol::executeScrollingHandler(CCObject* pSender)
{
	if( m_pScrollingListener && m_pScrollingHandler )
	{
		(m_pScrollingListener->*m_pScrollingHandler)(pSender);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nScrollingScriptHandler != 0 )
	{
		executeScrollingScriptHandler(pSender);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_ScrollableProtocol::executeScrollingScriptHandler(CCObject* pSender)
{
	if( m_nScrollingScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");

		pStack->executeFunctionByHandler(m_nScrollingScriptHandler, 1);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollableProtocol::setOnScrollingScriptHandler(int nHandler)
{
	removeOnScrollingScriptHandler();
	m_nScrollingScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollableProtocol::removeOnScrollingScriptHandler()
{
	if( m_nScrollingScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScrollingScriptHandler);
		m_nScrollingScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_ProgressEndedProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressEndedProtocol::FKCW_UIWidget_ProgressEndedProtocol()
	: m_pProgressEndedListener(NULL)
	, m_pProgressEndedHandler(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nProgressEndedScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressEndedProtocol::~FKCW_UIWidget_ProgressEndedProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnProgressEndedScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressEndedProtocol::setOnProgressEndedListener(CCObject* pListener, SEL_ProgressEndedHandler pHandler)
{
	m_pProgressEndedListener = pListener;
	m_pProgressEndedHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressEndedProtocol::executeProgressEndedHandler(CCObject* pSender)
{
	if( m_pProgressEndedListener && m_pProgressEndedHandler )
	{
		(m_pProgressEndedListener->*m_pProgressEndedHandler)(pSender);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nProgressEndedScriptHandler != 0 )
	{
		executeProgressEndedScriptHandler(pSender);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_ProgressEndedProtocol::executeProgressEndedScriptHandler(CCObject* pSender)
{
	if( m_nProgressEndedScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");

		pStack->executeFunctionByHandler(m_nProgressEndedScriptHandler, 1);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressEndedProtocol::setOnProgressEndedScriptHandler(int nHandler)
{
	removeOnProgressEndedScriptHandler();
	m_nProgressEndedScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressEndedProtocol::removeOnProgressEndedScriptHandler()
{
	if( m_nProgressEndedScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nProgressEndedScriptHandler);
		m_nProgressEndedScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_ValueChangeableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_ValueChangeableProtocol::FKCW_UIWidget_ValueChangeableProtocol()
	: m_pValueChangedListener(NULL)
	, m_pValueChangedHandler(NULL)
	, m_nValue(0)
#if FKCW_WIDGET_USING_LUA
	, m_nValueChangedScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ValueChangeableProtocol::~FKCW_UIWidget_ValueChangeableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnValueChangedScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ValueChangeableProtocol::setOnValueChangedListener(CCObject* pListener, SEL_ValueChangedHandler pHandler)
{
	m_pValueChangedListener = pListener;
	m_pValueChangedHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ValueChangeableProtocol::executeValueChangedHandler(CCObject* pSender, int nValue)
{
	if( m_pValueChangedListener && m_pValueChangedHandler )
	{
		(m_pValueChangedListener->*m_pValueChangedHandler)(pSender, nValue);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nValueChangedScriptHandler != 0 )
	{
		executeValueChangedScriptHandler(pSender, nValue);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_ValueChangeableProtocol::executeValueChangedScriptHandler(CCObject* pSender, int nValue)
{
	if( m_nValueChangedScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushInt(nValue);

		pStack->executeFunctionByHandler(m_nValueChangedScriptHandler, 2);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ValueChangeableProtocol::setOnValueChangedScriptHandler(int nHandler)
{
	removeOnValueChangedScriptHandler();
	m_nValueChangedScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ValueChangeableProtocol::removeOnValueChangedScriptHandler()
{
	if( m_nValueChangedScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nValueChangedScriptHandler);
		m_nValueChangedScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_ClickableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_ClickableProtocol::FKCW_UIWidget_ClickableProtocol()
	: m_pClickHandler(NULL)
	, m_pClickListener(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nClickScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ClickableProtocol::~FKCW_UIWidget_ClickableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnClickScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ClickableProtocol::setOnClickListener(CCObject* pListener, SEL_ClickHandler pHandler)
{
	m_pClickListener = pListener;
	m_pClickHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ClickableProtocol::executeClickHandler(CCObject* pSender)
{
	if( m_pClickListener && m_pClickHandler )
	{
		(m_pClickListener->*m_pClickHandler)(pSender);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nClickScriptHandler != 0 )
	{
		executeClickScriptHandler(pSender);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_ClickableProtocol::executeClickScriptHandler(CCObject* pSender)
{
	if( m_nClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");

		pStack->executeFunctionByHandler(m_nClickScriptHandler, 1);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ClickableProtocol::setOnClickScriptHandler(int nHandler)
{
	removeOnClickScriptHandler();
	m_nClickScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ClickableProtocol::removeOnClickScriptHandler()
{
	if( m_nClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nClickScriptHandler);
		m_nClickScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_LongClickableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_LongClickableProtocol::FKCW_UIWidget_LongClickableProtocol()
	: m_pLongClickListener(NULL)
	, m_pLongClickHandler(NULL)
	, m_bLongClickedUpdate(false)
	, m_fLongClickLastTouchDuration(0.0f)
	, m_pLongClickLastTouch(NULL)
	, m_bLongClickEnabled(false)
	, m_fLongClickTimeCounter(0.0f)
#if FKCW_WIDGET_USING_LUA
	, m_nLongClickScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_LongClickableProtocol::~FKCW_UIWidget_LongClickableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnLongClickScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LongClickableProtocol::setOnLongClickListener(CCObject* pListener, SEL_LongClickHandler pHandler)
{
	m_bLongClickEnabled  = pListener && pHandler;
	m_pLongClickListener = pListener;
	m_pLongClickHandler  = pHandler;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_LongClickableProtocol::executeLongClickHandler(CCObject* pSender, CCTouch* pTouch)
{
	if( m_pLongClickListener && m_pLongClickHandler )
	{
		return (m_pLongClickListener->*m_pLongClickHandler)(pSender, pTouch);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nLongClickScriptHandler != 0 )
	{
		return executeLongClickScriptHandler(pSender, pTouch);
	}
#endif
	return false;
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
bool FKCW_UIWidget_LongClickableProtocol::executeLongClickScriptHandler(CCObject* pSender, CCTouch* pTouch)
{
	if( m_nLongClickScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushCCObject(pTouch, "CCTouch");

		CCArray* pRetArray = new CCArray();
		pRetArray->initWithCapacity(1);

		int nRet = pStack->executeFunctionReturnArray(m_nLongClickScriptHandler, 2, 1, pRetArray);
		CCAssert(pRetArray->count() > 0, "return count = 0");

		CCBool* pBool = (CCBool*) pRetArray->objectAtIndex(0);
		bool bReturnBool = pBool->getValue();
		pStack->clean();
		return bReturnBool;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LongClickableProtocol::setOnLongClickScriptHandler(int nHandler)
{
	removeOnLongClickScriptHandler();
	m_nLongClickScriptHandler = nHandler;
	m_bLongClickEnabled = true;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LongClickableProtocol::removeOnLongClickScriptHandler()
{
	if( m_nLongClickScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nLongClickScriptHandler);
		m_nLongClickScriptHandler = 0;
		m_bLongClickEnabled = false;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_CheckableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_CheckableProtocol::FKCW_UIWidget_CheckableProtocol()
	: m_bChecked(false)
	, m_pCheckListener(NULL)
	, m_pCheckHandler(NULL)
	, m_nExclusion( -1 )
#if FKCW_WIDGET_USING_LUA
	, m_nCheckScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_CheckableProtocol::~FKCW_UIWidget_CheckableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeCheckScriptHandler();
#endif
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_CheckableProtocol::isChecked() const
{
	return m_bChecked;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::setChecked(bool bChecked)
{
	m_bChecked = bChecked;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::setOnCheckListener(CCObject* pListener, SEL_CheckHandler pHandler)
{
	m_pCheckListener = pListener;
	m_pCheckHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::setExclusion(int nExclusion)
{
	m_nExclusion = nExclusion;
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_CheckableProtocol::getExclusion() const
{
	return m_nExclusion;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::executeCheckHandler(CCObject* pSender, bool bChecked)
{
	if( m_pCheckListener && m_pCheckHandler  )
	{
		(m_pCheckListener->*m_pCheckHandler)(pSender, bChecked);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nCheckScriptHandler != 0 )
	{
		executeCheckScriptHandler(pSender, bChecked);
	}
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::setUnCheckedForOtherWidgetsByExclusion(CCNode* pParent)
{
	if( pParent && m_nExclusion != -1 )
	{
		CCArray* pChildren = pParent->getChildren();
		CCObject* pChildObject = NULL;
		CCARRAY_FOREACH(pChildren, pChildObject)
		{
			FKCW_UIWidget_CheckableProtocol* pCheckableWidget = dynamic_cast<FKCW_UIWidget_CheckableProtocol*>(pChildObject);
			if( pCheckableWidget && pCheckableWidget != this )
			{
				if( pCheckableWidget->getExclusion() == m_nExclusion )
				{
					pCheckableWidget->setChecked(false);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_CheckableProtocol::executeCheckScriptHandler(CCObject* pSender, bool bChecked)
{
	if( m_nCheckScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushBoolean(bChecked);

		pStack->executeFunctionByHandler(m_nCheckScriptHandler, 2);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::setOnCheckScriptHandler(int nHandler)
{
	removeCheckScriptHandler();
	m_nCheckScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckableProtocol::removeCheckScriptHandler()
{
	if( m_nCheckScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nCheckScriptHandler);
		m_nCheckScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------
// FKCW_UIWidget_ControlableProtocol
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlableProtocol::FKCW_UIWidget_ControlableProtocol()
	: m_pControlListener(NULL)
	, m_pControlHandler(NULL)
#if FKCW_WIDGET_USING_LUA
	, m_nControlScriptHandler(0)
#endif
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlableProtocol::~FKCW_UIWidget_ControlableProtocol()
{
#if FKCW_WIDGET_USING_LUA
	removeOnControlScriptHandler();
#endif
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlableProtocol::setOnControlListener(CCObject* pListener, SEL_ControlHandler pHandler)
{
	m_pControlListener = pListener;
	m_pControlHandler = pHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlableProtocol::executeControlHandler(CCObject* pSender, float cx, float cy)
{
	if( m_pControlListener && m_pControlHandler )
	{
		(m_pControlListener->*m_pControlHandler)(pSender, cx, cy);
	}
#if FKCW_WIDGET_USING_LUA
	else if( m_nControlScriptHandler != 0 )
	{
		executeControlScriptHandler(pSender, cx, cy);
	}
#endif
}
//-------------------------------------------------------------------------
#if FKCW_WIDGET_USING_LUA
void FKCW_UIWidget_ControlableProtocol::executeControlScriptHandler(CCObject* pSender, float cx, float cy)
{
	if( m_nControlScriptHandler != 0 )
	{
		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		CCLuaStack* pStack = pEngine->getLuaStack();

		pStack->pushCCObject(pSender, "CCObject");
		pStack->pushFloat(cx);
		pStack->pushFloat(cy);

		pStack->executeFunctionByHandler(m_nControlScriptHandler, 3);
		pStack->clean();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlableProtocol::setOnControlScriptHandler(int nHandler)
{
	removeOnControlScriptHandler();
	m_nControlScriptHandler = nHandler;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlableProtocol::removeOnControlScriptHandler()
{
	if( m_nControlScriptHandler != 0 )
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nControlScriptHandler);
		m_nControlScriptHandler = 0;
	}
}
#endif
//-------------------------------------------------------------------------