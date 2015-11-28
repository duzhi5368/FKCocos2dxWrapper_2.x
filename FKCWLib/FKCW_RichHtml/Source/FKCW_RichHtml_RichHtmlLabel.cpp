//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlLabel.h"
#include "../Include/FKCW_RichHtml_RichHtmlNode.h"
#include "../Include/FKCW_RichHtml_RichHtmlOverlay.h"
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLLabel* FKCW_RichHtml_HTMLLabel::create()
{
	FKCW_RichHtml_HTMLLabel* label = new FKCW_RichHtml_HTMLLabel;
	if ( label && label->initWithString(NULL, CCSize(.0f, .0f)) )
	{
		label->autorelease();
		return label;
	}

	CC_SAFE_DELETE(label);

	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLLabel* FKCW_RichHtml_HTMLLabel::createWithString(const char* utf8_str, const CCSize& preferred_size, const char* font_alias)
{
	FKCW_RichHtml_HTMLLabel* label = new FKCW_RichHtml_HTMLLabel;
	if ( label && label->initWithString(utf8_str, preferred_size, font_alias) )
	{
		label->autorelease();
		return label;
	}

	CC_SAFE_DELETE(label);

	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_HTMLLabel::initWithString(const char* utf8_str, const CCSize& preferred_size, const char* font_alias)
{
	CCAssert(m_rRichNode == NULL, "");

	m_rRichNode = FKCW_RichHtml_HTMLNode::createWithContainer(this);
	if( m_rRichNode == NULL )
		return false;

	m_rRichNode->retain();
	this->addChild(m_rRichNode);

	m_rRichNode->ignoreAnchorPointForPosition(true);
	m_rRichNode->setAnchorPoint(ccp(.0f, .0f));

	m_rRichNode->setDefaultWrapline(true);
	m_rRichNode->setDefaultSpacing(1);
	m_rRichNode->setDefaultPadding(0);
	m_rRichNode->setDefaultAlignment(e_align_left);
	m_rRichNode->setDefaultFontAlias(font_alias);
	m_rRichNode->setPreferredSize(FKCW_RichHtml_Size((short)preferred_size.width, (short)preferred_size.height));

	if ( utf8_str )
	{
		setString(utf8_str);
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_HTMLLabel::setString(const char *utf8_str) 
{
	m_rRichNode->setStringUTF8(utf8_str);
}
//-------------------------------------------------------------------------
const char* FKCW_RichHtml_HTMLLabel::getString(void) 
{ 
	return m_rRichNode->getStringUTF8();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_HTMLLabel::appendString(const char *utf8_str)
{
	m_rRichNode->appendStringUTF8(utf8_str);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_HTMLLabel::draw()
{
	CCNode::draw();

	if( g_bIsFKCWRichHtmlDebug )
	{
		// »æÖÆ°üÎ§ºÐ
		const CCSize& s = this->getContentSize();
		CCPoint vertices[4]={
			ccp(0-1,0-1),ccp(s.width+1,0-1),
			ccp(s.width+1,s.height+1),ccp(0-1,s.height+1),
		};
		ccDrawColor4B(0xff, 0xff, 0xff, 0xff);
		ccDrawPoly(vertices, 4, true);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_HTMLLabel::registerListener(void* target, FKCW_RichHtml_IRichEventHandler* listener)
{
	m_rRichNode->getOverlay()->registerListener(target, listener);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_HTMLLabel::removeListener(void* target)
{
	m_rRichNode->getOverlay()->removeListener(target);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLLabel::FKCW_RichHtml_HTMLLabel()
	: m_rRichNode(NULL)
{
}
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLLabel::~FKCW_RichHtml_HTMLLabel()
{
	CC_SAFE_RELEASE(m_rRichNode);
}
//-------------------------------------------------------------------------