//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlNode.h
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
class FKCW_RichHtml_RichNode: public CCNode, public FKCW_RichHtml_IRichNode
{
public:
	typedef std::map<CCTexture2D*, class FKCW_RichHtml_RichAtlas*> atlas_map_t;
	typedef std::map<unsigned int, atlas_map_t*> color_map_t;

public:
	virtual FKCW_RichHtml_IRichParser* getParser();
	virtual FKCW_RichHtml_IRichCompositor* getCompositor();
	virtual FKCW_RichHtml_Size getActualSize();

	virtual FKCW_RichHtml_Size getPreferredSize();
	virtual void setPreferredSize(FKCW_RichHtml_Size size);

	virtual void setStringUTF8(const char* utf8_str);
	virtual void appendStringUTF8(const char* utf8_str);
	virtual const char* getStringUTF8();
	virtual FKCW_RichHtml_IRichAtlas* findAtlas(class CCTexture2D* texture, unsigned int color_rgba, int zorder = ZORDER_CONTEXT);
	virtual void addOverlay(FKCW_RichHtml_IRichElement* overlay);
	virtual void addCCNode(class CCNode* node);
	virtual void removeCCNode(class CCNode* node);
	class FKCW_RichHtml_RichOverlay* getOverlay();

	virtual void draw();

	virtual void setPlainMode(bool on);
	virtual bool isPlainMode() { return getParser()->isPlainMode(); }

	virtual void setDefaultFontAlias(const char* font_alias);
	virtual const char* getDefaultFontAlias();
	virtual void setDefaultColor(unsigned int color);
	virtual unsigned int getDefaultColor();
	virtual void setDefaultAlignment(ENUM_Alignment align);
	virtual ENUM_Alignment getDefaultAlignment();
	virtual bool isDefaultWrapline();
	virtual void setDefaultWrapline(bool wrapline);
	virtual short getDefaultSpacing();
	virtual void setDefaultSpacing(short spacing);
	virtual short getDefaultPadding();
	virtual void setDefaultPadding(short padding);

	virtual bool initialize() = 0;

	FKCW_RichHtml_RichNode(class CCNode* container);
	virtual ~FKCW_RichHtml_RichNode();

private:
	void processRichString(const char* utf8_str);
	void updateAll();
	void updateContentSize();
	void clearStates();
	void clearRichElements();
	void clearAtlasMap();
	class FKCW_RichHtml_RichAtlas* findColoredTextureAtlas(CCTexture2D* texture, unsigned int color_rgba, int zorder);

protected:
	class CCNode* m_rContainer;

	FKCW_RichHtml_IRichParser* m_rParser;
	FKCW_RichHtml_IRichCompositor* m_rCompositor;

	std::string m_rRichString;
	element_list_t m_rElements;

	FKCW_RichHtml_Size m_rPreferedSize;

	color_map_t m_rAtlasMap;
	std::vector<class FKCW_RichHtml_RichAtlas*> m_rAtlasList;
	class FKCW_RichHtml_RichOverlay* m_rOverlays;
};
//-------------------------------------------------------------------------
// HTMLNode
//-------------------------------------------------------------------------
class FKCW_RichHtml_HTMLNode : public FKCW_RichHtml_RichNode
{
public:
	static FKCW_RichHtml_HTMLNode* createWithContainer(class CCNode* container);
	bool initialize();
private:
	FKCW_RichHtml_HTMLNode(class CCNode* container);
};
//-------------------------------------------------------------------------
