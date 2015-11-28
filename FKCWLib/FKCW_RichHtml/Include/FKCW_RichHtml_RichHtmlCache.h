//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlCache.h
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
class FKCW_RichHtml_CacheBase : public FKCW_RichHtml_ICompositCache
{
public:
	virtual ENUM_Alignment getHAlign() { return m_rHAlign; }
	virtual ENUM_Alignment getVAlign() { return m_rVAlign; }
	virtual void setHAlign(ENUM_Alignment align) { m_rHAlign = align; }
	virtual void setVAlign(ENUM_Alignment align) { m_rVAlign = align; }
	
	virtual short getLineHeight() { return m_rLineHeight; }
	virtual void setLineHeight(short v) { m_rLineHeight = v; }

	virtual void setSpacing(short v) { m_rSpacing = v; }
	virtual void setPadding(short v) { m_rPadding = v; }
	virtual short getSpacing() { return m_rSpacing; }
	virtual short getPadding() { return m_rPadding; }

	virtual void setWrapline(bool wrap) { m_rWrapLine = wrap; }
	virtual bool isWrapline() { return m_rWrapLine; }

	FKCW_RichHtml_CacheBase();

protected:
	ENUM_Alignment m_rHAlign;
	ENUM_Alignment m_rVAlign;
	short m_rLineHeight;
	short m_rSpacing;
	short m_rPadding;
	bool m_rWrapLine;
};
//-------------------------------------------------------------------------
// 线性缓冲组合器
class FKCW_RichHtml_LineCache : public FKCW_RichHtml_CacheBase
{
public:
	virtual void appendElement(FKCW_RichHtml_IRichElement* ele);
	virtual element_list_t* getCachedElements();
	virtual FKCW_RichHtml_Rect flush(class FKCW_RichHtml_IRichCompositor* compositor);
	virtual void clear();

	FKCW_RichHtml_LineCache();

protected:
	element_list_t m_rCachedLine;
	short m_rBaselinePos;
};
//-------------------------------------------------------------------------
// 网格表单缓冲组合器
class FKCW_RichHtml_TableCache : public FKCW_RichHtml_CacheBase
{
public:
	virtual void appendElement(FKCW_RichHtml_IRichElement* ele);
	virtual element_list_t* getCachedElements();
	virtual void clear();
	virtual FKCW_RichHtml_Rect flush(class FKCW_RichHtml_IRichCompositor* compositor);
	void setTable(class FKCW_RichHtml_EleHTMLTable* table);

	FKCW_RichHtml_TableCache();

private:
	void recompositCell(class FKCW_RichHtml_EleHTMLCell* cell);
	void travesalRecompositChildren(element_list_t* eles, short x_fixed, short y_fixed);

protected:
	element_list_t m_rCached;
	class FKCW_RichHtml_EleHTMLTable* m_rTable;
};
//-------------------------------------------------------------------------