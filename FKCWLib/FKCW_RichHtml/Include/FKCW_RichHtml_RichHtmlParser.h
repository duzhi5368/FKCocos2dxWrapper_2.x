//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlParser.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RichHtml_Macros.h"
#include "FKCW_RichHtml_BaseStruct.h"
#include "FKCW_RichHtml_HelpFunc.h"
#include "FKCW_RichHtml_Interface.h"
//-------------------------------------------------------------------------
class FKCW_RichHtml_SimpleHTMLParser : public FKCW_RichHtml_IRichParser, public CCSAXDelegator
{
public:
	virtual element_list_t* parseString(const char* utf8_str);
	virtual element_list_t* parseFile(const char* filename);
	virtual class FKCW_RichHtml_IRichNode* getContainer() { return m_rContainer; }

	virtual bool isPlainMode() { return m_rPlainModeON; }
	virtual void setPlainMode(bool on) { m_rPlainModeON = on; }

	FKCW_RichHtml_SimpleHTMLParser(FKCW_RichHtml_IRichNode* container);

protected:
	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);

private:
	element_list_t* parseHTMLString(const char* utf8_str);

	FKCW_RichHtml_IRichNode*			m_rContainer;
	element_list_t*		m_rElements;
	FKCW_RichHtml_IRichElement*		m_rCurrentElement;
	bool				m_rPlainModeON;
};
//-------------------------------------------------------------------------