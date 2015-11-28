//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlParser.h"
#include "../Include/FKCW_RichHtml_RichHtmlElement.h"
#include <stack>
using std::stack;
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_SimpleHTMLParser::parseFile(const char* filename)
{
	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	CCString *contents = CCString::createWithContentsOfFile(fullpath.c_str());

	if ( !contents )
	{
		CCLog("[CCRich] open file failed! %s", filename);
		return NULL;
	}

	return parseString(contents->getCString());
}
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_SimpleHTMLParser::parseString(const char* utf8_str)
{
	if ( !utf8_str )
	{
		CCLog("[CCRich] utf8_str is null!");
		return NULL;
	}

	element_list_t* eles = NULL;
	// 添加一个头部标签，开始解析HTML字符串
	if ( !m_rPlainModeON )
	{
		std::stringstream ss;
		ss << "<root>" << utf8_str << "</root>";
		eles = parseHTMLString(ss.str().c_str());
	}

	// 无效的Html字符串
	if ( !eles )
	{
		eles = new element_list_t;
		m_rCurrentElement = new FKCW_RichHtml_EleHTMLRoot;
		this->textHandler(this, utf8_str, strlen(utf8_str));

		eles->push_back(m_rCurrentElement);
		m_rCurrentElement = NULL;
	}

	return eles;
}
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_SimpleHTMLParser::parseHTMLString(const char* utf8_str)
{
	CCSAXParser parser;
	if ( !parser.init("UTF-8") )
	{
		CCLog("[CCRich] CCSAXParser.init failed!");
		return NULL;
	}

	parser.setDelegator(this);

	element_list_t* elelist = new element_list_t;
	m_rElements = elelist;
	m_rCurrentElement = NULL;

	if ( !parser.parse(utf8_str, strlen(utf8_str)) || elelist->empty() )
	{
		CC_SAFE_DELETE(elelist);
	}

	m_rCurrentElement = NULL;
	m_rElements = NULL;

	return elelist;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_SimpleHTMLParser::startElement(void *ctx, const char *name, const char **atts)
{
	FKCW_RichHtml_IRichElement* element = NULL;

	if ( 0 == strcmp(name, "br") )
	{
		element = new FKCW_RichHtml_EleHTMLBR;
	}
	else if ( 0 == strcmp(name, "u") )
	{
		element = new FKCW_RichHtml_EleHTMLU;
	}
	else if ( 0 == strcmp(name, "font") )
	{
		element = new FKCW_RichHtml_EleHTMLFont;
	}
	else if ( 0 == strcmp(name, "table") )
	{
		element = new FKCW_RichHtml_EleHTMLTable;
	}
	else if ( 0 == strcmp(name, "tr") )
	{
		if ( dynamic_cast<FKCW_RichHtml_EleHTMLTable*>(m_rCurrentElement) )
		{
			element = new FKCW_RichHtml_EleHTMLRow(dynamic_cast<FKCW_RichHtml_EleHTMLTable*>(m_rCurrentElement));
		}
	}
	else if ( 0 == strcmp(name, "td") )
	{
		if ( dynamic_cast<FKCW_RichHtml_EleHTMLRow*>(m_rCurrentElement) )
		{
			element = new FKCW_RichHtml_EleHTMLCell(dynamic_cast<FKCW_RichHtml_EleHTMLRow*>(m_rCurrentElement));
		}
	}
	else if ( 0 == strcmp(name, "a") )
	{
		element = new FKCW_RichHtml_EleHTMLAnchor;
	}
	else if ( 0 == strcmp(name, "button") )
	{
		element = new FKCW_RichHtml_EleHTMLButton;
	}
	else if ( 0 == strcmp(name, "img") )
	{
		element = new FKCW_RichHtml_EleHTMLImg;
	}
	else if ( 0 == strcmp(name, "ccb") )
	{
		element = new FKCW_RichHtml_EleCCBNode;
	}
	else if ( 0 == strcmp(name, "hr") )
	{
		element = new FKCW_RichHtml_EleHTMLHR;
	}
	else if ( 0 == strcmp(name, "p") )
	{
		element = new FKCW_RichHtml_EleHTMLP;
	}
	else if ( 0 == strcmp(name, "node")
		|| 0 == strcmp(name, "root")
		|| 0 == strcmp(name, "body") )
	{
		element = new FKCW_RichHtml_EleHTMLRoot;
	}

	if ( !element )
	{
		// 暂时无法支持的标签
		element = new FKCW_RichHtml_EleHTMLNotSupport;
	}

	CCAssert(element, "");

	element->parse(this, atts);

	if ( !m_rCurrentElement )
	{
		m_rElements->push_back(element);
	}
	else
	{
		m_rCurrentElement->addChildren(element);
	}

	m_rCurrentElement = element;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_SimpleHTMLParser::endElement(void *ctx, const char *name)
{
	CCAssert(m_rCurrentElement, "[CCRich] invalid rich string!");
	m_rCurrentElement = m_rCurrentElement->getParent();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_SimpleHTMLParser::textHandler(void *ctx, const char *s, int len)
{
	CCAssert(m_rCurrentElement, "[CCRich]: must specify a parent element!");

	unsigned short* utf16str = cc_utf8_to_utf16(s);
	size_t utf16size = cc_wcslen(utf16str);

	if ( utf16size == 0 )
		return;

	for ( size_t i = 0; i < utf16size; i++ )
	{
		FKCW_RichHtml_EleGlyph* ele = new FKCW_RichHtml_EleGlyph(utf16str[i]);
		if ( ele->parse(this) )
		{
			m_rCurrentElement->addChildren(ele);
		}
		else
		{
			CC_SAFE_DELETE(ele);
		}
	}

	CC_SAFE_DELETE_ARRAY(utf16str);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_SimpleHTMLParser::FKCW_RichHtml_SimpleHTMLParser(FKCW_RichHtml_IRichNode* container)
	: m_rContainer(container)
	, m_rPlainModeON(false)
{

}
//-------------------------------------------------------------------------