//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlOverlay.h"
#include "../Include/FKCW_RichHtml_RichHtmlElement.h"
//-------------------------------------------------------------------------
FKCW_RichHtml_RichOverlay* FKCW_RichHtml_RichOverlay::create()
{
	FKCW_RichHtml_RichOverlay* overlay = new FKCW_RichHtml_RichOverlay();
	if ( overlay && overlay->init() )
	{
		overlay->autorelease();
		return overlay;
	}

	CC_SAFE_DELETE(overlay);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_RichOverlay::init()
{
	this->setTouchMode(kCCTouchesOneByOne);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::draw()
{
	CCLayer::draw();

	if( g_bIsFKCWRichHtmlDebug )
	{
		// »æÖÆ°üÎ§ºÐ
		const CCSize& s = this->getContentSize();
		CCPoint vertices[4]={
			ccp(0,0),ccp(s.width,0),
			ccp(s.width,s.height),ccp(0,s.height),
		};
		ccDrawColor4B(0x00, 0xff, 0x00, 0xff);
		ccDrawPoly(vertices, 4, true);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::append(FKCW_RichHtml_IRichElement* ele)
{
	FKCW_RichHtml_EleHTMLTouchable* overlay = dynamic_cast<FKCW_RichHtml_EleHTMLTouchable*>(ele);
	CCAssert(overlay, "[CCRich] not a overlay or subclass!");

	if ( overlay )
	{
		m_touchables.push_back(overlay);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::reset()
{
	removeAllChildren();
	m_elements.clear();
	m_touchables.clear();
	m_touched = NULL;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichNode* FKCW_RichHtml_RichOverlay::getContainer()
{
	CCAssert(getParent(), "");
	return dynamic_cast<FKCW_RichHtml_IRichNode*>(getParent());
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_RichOverlay::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	FKCW_RichHtml_Rect rect = getContainer()->getCompositor()->getRect();

	CCRect bbox;
	bbox.size = CCSize(rect.size.w, rect.size.h);
	bbox.origin = ccp(rect.pos.x, rect.pos.y - rect.size.h);

	CCPoint pt = convertToNodeSpace(pTouch->getLocation());

	if ( !bbox.containsPoint(convertToNodeSpace(pTouch->getLocation())) )
	{
		return false;
	}

	for( std::list<FKCW_RichHtml_EleHTMLTouchable*>::iterator it = m_touchables.begin(); it != m_touchables.end(); it++ )
	{
		FKCW_RichHtml_EleHTMLTouchable* overlay = *it;

		if ( overlay->isEnabled() && 
			overlay->isLocationInside(pt)
			/*overlay->onTouchBegan(this, pTouch, pEvent)*/)
		{
			m_touched = overlay;
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if ( m_touched && !m_eventhandlers.empty() )
	{
		CCPoint pt = convertToNodeSpace(pTouch->getLocation());
		std::map<void*, FKCW_RichHtml_IRichEventHandler*>::iterator hit = m_eventhandlers.begin();
		for ( ; hit != m_eventhandlers.end(); hit++ )
		{
			hit->second->onMoved(
				getContainer(), m_touched, m_touched->getID(), 
				pTouch->getLocation(), pTouch->getDelta());
		}

		//m_touched->onTouchMoved(this, pTouch, pEvent);
	}	
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if ( m_touched )
	{
		CCPoint pt = convertToNodeSpace(pTouch->getLocation());
		if ( m_touched->isLocationInside(pt) )
		{
			std::map<void*, FKCW_RichHtml_IRichEventHandler*>::iterator hit = m_eventhandlers.begin();
			for ( ; hit != m_eventhandlers.end(); hit++ )
			{
				hit->second->onClick(getContainer(), m_touched, m_touched->getID());
			}

		}

		//m_touched->onTouchEnded(this, pTouch, pEvent);
		m_touched = NULL;
	}	
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	if ( m_touched )
	{
		CCPoint pt = convertToNodeSpace(pTouch->getLocation());
		//m_touched->onTouchCancelled(this, pTouch, pEvent);
		m_touched = NULL;
	}	
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::registerListener(void* target, FKCW_RichHtml_IRichEventHandler* listener)
{
	CCAssert(m_eventhandlers.find(target) == m_eventhandlers.end(), "dummy target! memory leak!" );

	m_eventhandlers.insert(std::make_pair(target, listener));
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichOverlay::removeListener(void* target)
{
	std::map<void*, FKCW_RichHtml_IRichEventHandler*>::iterator it = m_eventhandlers.find(target);

	if ( it != m_eventhandlers.end() )
	{
		delete it->second;
		m_eventhandlers.erase(it);
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichOverlay::FKCW_RichHtml_RichOverlay()
	: m_touched(NULL)
{
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichOverlay::~FKCW_RichHtml_RichOverlay()
{
	std::map<void*, FKCW_RichHtml_IRichEventHandler*>::iterator hit = m_eventhandlers.begin();
	for ( ; hit != m_eventhandlers.end(); hit++ )
	{
		delete hit->second;
	}
	m_eventhandlers.clear();

	m_container = NULL;
	m_touched = NULL;
}
//-------------------------------------------------------------------------