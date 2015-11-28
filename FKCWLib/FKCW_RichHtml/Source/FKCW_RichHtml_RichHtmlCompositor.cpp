//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlCompositor.h"
//-------------------------------------------------------------------------
bool FKCW_RichHtml_SimpleHTMLCompositor::composit(FKCW_RichHtml_IRichElement* root)
{
	root->composit(this);

	FKCW_RichHtml_Rect rect = getMetricsState()->elements_cache->flush(this);
	m_rRect.extend(rect);

	if ( m_rFontCache )
	{
		m_rFontCache->flush();
	}

	return true;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_SimpleHTMLCompositor::FKCW_RichHtml_SimpleHTMLCompositor(FKCW_RichHtml_IRichNode* container)
	: FKCW_RichHtml_BaseCompositor(container)
{
	getRootCache()->setWrapline(true);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_MetricsState* FKCW_RichHtml_BaseCompositor::getMetricsState()
{
	if ( !m_rMetricsStack.empty() )
	{
		return &m_rMetricsStack.top();
	}

	return &m_rInitMetricsState;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_MetricsState* FKCW_RichHtml_BaseCompositor::pushMetricsState()
{
	m_rMetricsStack.push(FKCW_RichHtml_MetricsState(*getMetricsState()));
	return getMetricsState();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_BaseCompositor::popMetricsState()
{
	if ( !m_rMetricsStack.empty() )
	{
		m_rMetricsStack.pop();
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_MetricsState* FKCW_RichHtml_BaseCompositor::initMetricsState(const FKCW_RichHtml_MetricsState* new_init_state /*= NULL*/)
{
	if ( new_init_state )
	{
		m_rInitMetricsState = *new_init_state;
	}
	while(!m_rMetricsStack.empty())
		m_rMetricsStack.pop();

	m_rInitMetricsState.elements_cache = getRootCache();
	m_rInitMetricsState.pen_x = 0;
	m_rInitMetricsState.pen_y = 0;

	return &m_rInitMetricsState;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RenderState* FKCW_RichHtml_BaseCompositor::getRenderState()
{
	if ( !m_rRenderStack.empty() )
	{
		return &m_rRenderStack.top();
	}

	return &m_rInitRenderState;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RenderState* FKCW_RichHtml_BaseCompositor::pushRenderState()
{
	m_rRenderStack.push(FKCW_RichHtml_RenderState(*getRenderState()));
	return getRenderState();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_BaseCompositor::popRenderState()
{
	if ( !m_rRenderStack.empty() )
	{
		m_rRenderStack.pop();
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RenderState* FKCW_RichHtml_BaseCompositor::initRenderState(const FKCW_RichHtml_RenderState* new_init_state/* = NULL*/)
{
	if ( new_init_state )
	{
		m_rInitRenderState = *new_init_state;
	}
	while(!m_rRenderStack.empty())
		m_rRenderStack.pop();

	return &m_rInitRenderState;
}
//-------------------------------------------------------------------------
class FKCW_Font_Catalog* FKCW_RichHtml_BaseCompositor::getFont()
{
	if ( m_rFontCacheAlias 
		&& strcmp(m_rFontCacheAlias, getRenderState()->font_alias ) == 0 )
	{
		return m_rFontCache;
	}

	if (m_rFontCache)
		m_rFontCache->flush();

	m_rFontCacheAlias = getRenderState()->font_alias;
	m_rFontCache = FKCW_Font_Factory::GetInstance()->find_font(m_rFontCacheAlias);

	return m_rFontCache;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_BaseCompositor::reset()
{
	initMetricsState();
	initRenderState();

	getMetricsState()->zone = FKCW_RichHtml_Rect();
	getMetricsState()->zone.size = getContainer()->getPreferredSize();

	getRootCache()->clear();

	m_rRect = FKCW_RichHtml_Rect();
	m_rFontCacheAlias = NULL;
	m_rFontCache = NULL;
}
//-------------------------------------------------------------------------
class FKCW_RichHtml_IRichNode* FKCW_RichHtml_BaseCompositor::getContainer()
{
	return m_rContainer;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_BaseCompositor::FKCW_RichHtml_BaseCompositor(FKCW_RichHtml_IRichNode* container)
	: m_rContainer(container),  m_rFontCache(NULL), m_rFontCacheAlias(NULL)
{
}
//-------------------------------------------------------------------------
FKCW_RichHtml_BaseCompositor::~FKCW_RichHtml_BaseCompositor()
{
	m_rContainer = NULL;
}
//-------------------------------------------------------------------------