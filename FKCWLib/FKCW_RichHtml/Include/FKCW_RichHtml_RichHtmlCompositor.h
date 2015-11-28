//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_RichHtml_RichHtmlCompositor.h
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
#include "FKCW_RichHtml_RichHtmlCache.h"
//-------------------------------------------------------------------------
class FKCW_RichHtml_BaseCompositor : public FKCW_RichHtml_IRichCompositor
{
public:
	virtual const FKCW_RichHtml_Rect& getRect() const { return m_rRect; }
	virtual FKCW_RichHtml_MetricsState* getMetricsState();
	virtual FKCW_RichHtml_MetricsState* pushMetricsState();
	virtual void popMetricsState();
	virtual FKCW_RichHtml_MetricsState* initMetricsState(const FKCW_RichHtml_MetricsState* new_init_state = NULL);

	virtual FKCW_RichHtml_RenderState* getRenderState();
	virtual FKCW_RichHtml_RenderState* pushRenderState();
	virtual void popRenderState();
	virtual FKCW_RichHtml_RenderState* initRenderState(const FKCW_RichHtml_RenderState* new_init_state = NULL);

	virtual class FKCW_Font_Catalog* getFont();

	virtual void reset();
	virtual class FKCW_RichHtml_IRichNode* getContainer();

	FKCW_RichHtml_BaseCompositor(FKCW_RichHtml_IRichNode* container);
	virtual ~FKCW_RichHtml_BaseCompositor();

protected:
	FKCW_RichHtml_IRichNode* m_rContainer;

	FKCW_RichHtml_MetricsState m_rInitMetricsState;
	std::stack<FKCW_RichHtml_MetricsState> m_rMetricsStack;
	FKCW_RichHtml_RenderState m_rInitRenderState;
	std::stack<FKCW_RichHtml_RenderState> m_rRenderStack;

	FKCW_RichHtml_Rect m_rRect;

	class FKCW_Font_Catalog* m_rFontCache;
	const char* m_rFontCacheAlias;
};
//-------------------------------------------------------------------------
// HTML元素组合器
class FKCW_RichHtml_SimpleHTMLCompositor : public FKCW_RichHtml_BaseCompositor
{
public:
	virtual bool composit(FKCW_RichHtml_IRichElement* root);
	virtual class FKCW_RichHtml_ICompositCache* getRootCache() { return &m_FKCW_RichHtml_LineCache; }

	FKCW_RichHtml_SimpleHTMLCompositor(FKCW_RichHtml_IRichNode* container);

private:
	FKCW_RichHtml_LineCache m_FKCW_RichHtml_LineCache;
};
//-------------------------------------------------------------------------