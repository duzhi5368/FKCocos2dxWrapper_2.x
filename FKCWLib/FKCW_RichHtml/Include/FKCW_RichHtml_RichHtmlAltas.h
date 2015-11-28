//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlAltas.h
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
class FKCW_RichHtml_RichAtlas : public CCAtlasNode, public FKCW_RichHtml_IRichAtlas
{
public:
	static FKCW_RichHtml_RichAtlas* create(class FKCW_RichHtml_IRichNode* container, CCTexture2D* texture, size_t capacity);

	bool initWithTexture(CCTexture2D* texture, size_t capacity);

	virtual void appendRichElement(FKCW_RichHtml_IRichElement* element);
	virtual void resetRichElements();
	virtual void updateRichRenderData();

	void resizeCapacity(size_t ns);
	void reset();

	virtual void updateAtlasValues();
	virtual void draw();

	FKCW_RichHtml_RichAtlas(class FKCW_RichHtml_IRichNode* container);
	virtual ~FKCW_RichHtml_RichAtlas();

protected:
	class FKCW_RichHtml_IRichNode* m_container;
	bool m_dirty;
	std::list<FKCW_RichHtml_IRichElement*> m_elements;
};