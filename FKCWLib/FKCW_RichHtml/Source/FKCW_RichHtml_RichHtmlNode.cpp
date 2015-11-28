//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlNode.h"
#include "../Include/FKCW_RichHtml_RichHtmlOverlay.h"
#include "../Include/FKCW_RichHtml_RichHtmlAltas.h"
#include "../Include/FKCW_RichHtml_RichHtmlCompositor.h"
#include "../Include/FKCW_RichHtml_RichHtmlParser.h"
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichParser* FKCW_RichHtml_RichNode::getParser()
{
	return m_rParser;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichCompositor* FKCW_RichHtml_RichNode::getCompositor()
{
	return m_rCompositor;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_Size FKCW_RichHtml_RichNode::getActualSize()
{
	return getCompositor()->getRect().size;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_Size FKCW_RichHtml_RichNode::getPreferredSize()
{
	return m_rPreferedSize;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setPreferredSize(FKCW_RichHtml_Size size) 
{ 
	if ( size.w != m_rPreferedSize.w || size.h != m_rPreferedSize.h )
	{
		m_rPreferedSize = size; 
		updateAll();
	}
}
//-------------------------------------------------------------------------
const char* FKCW_RichHtml_RichNode::getDefaultFontAlias()
{
	return getCompositor()->getRenderState()->font_alias;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultFontAlias(const char* font_alias)
{
	if ( getCompositor()->getRenderState()->font_alias != font_alias )
	{
		getCompositor()->getRenderState()->font_alias = font_alias;
		updateAll();
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultColor(unsigned int color)
{
	if ( getCompositor()->getRenderState()->color != color )
	{
		getCompositor()->getRenderState()->color = color;
		updateAll();
	}
}
//-------------------------------------------------------------------------
unsigned int FKCW_RichHtml_RichNode::getDefaultColor()
{
	return getCompositor()->getRenderState()->color;
}
//-------------------------------------------------------------------------
ENUM_Alignment FKCW_RichHtml_RichNode::getDefaultAlignment()
{
	return getCompositor()->getRootCache()->getHAlign();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultAlignment(ENUM_Alignment align)
{
	if ( getCompositor()->getRootCache()->getHAlign() != align )
	{
		getCompositor()->getRootCache()->setHAlign(align);
		updateAll();
	}
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_RichNode::isDefaultWrapline()
{
	return getCompositor()->getRootCache()->isWrapline();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultWrapline(bool wrapline)
{
	if ( getCompositor()->getRootCache()->isWrapline() != wrapline )
	{
		getCompositor()->getRootCache()->setWrapline(wrapline);
		updateAll();
	}
}
//-------------------------------------------------------------------------
short FKCW_RichHtml_RichNode::getDefaultSpacing()
{
	return getCompositor()->getRootCache()->getSpacing();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultSpacing(short spacing)
{
	if ( getCompositor()->getRootCache()->getSpacing() != spacing )
	{
		getCompositor()->getRootCache()->setSpacing(spacing);
		updateAll();
	}
}
//-------------------------------------------------------------------------
short FKCW_RichHtml_RichNode::getDefaultPadding()
{
	return getCompositor()->getRootCache()->getPadding();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setDefaultPadding(short padding)
{
	if ( getCompositor()->getRootCache()->getPadding() != padding )
	{
		getCompositor()->getRootCache()->setPadding(padding);
		updateAll();
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setStringUTF8(const char* utf8_str)
{
	m_rRichString = utf8_str;
	updateAll();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::appendStringUTF8(const char* utf8_str)
{
	m_rRichString.append(utf8_str);
	processRichString(utf8_str);
}
//-------------------------------------------------------------------------
const char* FKCW_RichHtml_RichNode::getStringUTF8()
{
	return m_rRichString.c_str();
}
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichAtlas* FKCW_RichHtml_RichNode::findAtlas(class CCTexture2D* texture, unsigned int color_rgba, int zorder /*= 0*/)
{
	return findColoredTextureAtlas(texture, color_rgba, zorder);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::addOverlay(FKCW_RichHtml_IRichElement* overlay)
{
	getOverlay()->append(overlay);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::addCCNode(class CCNode* node)
{
	getOverlay()->addChild(node);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::removeCCNode(class CCNode* node)
{
	getOverlay()->removeChild(node);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichOverlay* FKCW_RichHtml_RichNode::getOverlay()
{
	if ( !m_rOverlays )
	{
		m_rOverlays = FKCW_RichHtml_RichOverlay::create();
		if (m_rOverlays)
		{
			m_rOverlays->retain();
			m_rOverlays->registerWithTouchDispatcher();
			addChild(m_rOverlays);
		}
	}

	return m_rOverlays;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::processRichString(const char* utf8_str)
{
	if ( !utf8_str )
		return;

	element_list_t* eles = getParser()->parseString(utf8_str);
	if ( !eles )
		return;

	for ( element_list_t::iterator it = eles->begin(); it != eles->end(); it++ )
	{
		getCompositor()->composit(*it);
	}

	m_rElements.insert(m_rElements.end(), eles->begin(), eles->end());
	CC_SAFE_DELETE(eles);

	updateContentSize();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::updateAll()
{
	clearStates();
	if ( !m_rRichString.empty() )
		processRichString(m_rRichString.c_str());
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::updateContentSize()
{
	if ( m_rContainer )
	{
		FKCW_RichHtml_Rect rect = getCompositor()->getRect();
		m_rContainer->setContentSize(CCSize(/*m_rPreferedSize.w*/rect.size.w, rect.size.h));
		setPositionY(rect.size.h);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::clearStates()
{
	getCompositor()->reset();
	clearRichElements();

	// Çå¿ÕAtlas
	clearAtlasMap();

	// ÇåÀíOverLay
	if (m_rOverlays)
	{
		m_rOverlays->reset();
		m_rOverlays->removeAllChildren();
	}

	updateContentSize();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::clearRichElements()
{
	for ( element_list_t::iterator it = m_rElements.begin(); it != m_rElements.end(); it++ )
	{
		delete *it;
	}
	m_rElements.clear();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::clearAtlasMap()
{
	for ( color_map_t::iterator color_it = m_rAtlasMap.begin(); color_it != m_rAtlasMap.end(); color_it++ )
	{
		for ( atlas_map_t::iterator atlas_it = color_it->second->begin(); atlas_it != color_it->second->end(); atlas_it++ )
		{
			CC_SAFE_RELEASE(atlas_it->second);
		}
		color_it->second->clear();
		delete color_it->second;
	}
	m_rAtlasMap.clear();

	for ( std::vector<FKCW_RichHtml_RichAtlas*>::iterator it = m_rAtlasList.begin(); it != m_rAtlasList.end(); it++ )
	{
		FKCW_RichHtml_RichAtlas* atlas = *it;
		CC_SAFE_RELEASE(atlas);
		getOverlay()->removeChild(atlas);
	}
	m_rAtlasList.clear();
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichAtlas* FKCW_RichHtml_RichNode::findColoredTextureAtlas(CCTexture2D* texture, unsigned int color_rgba, int zorder)
{
	if ( texture == NULL || color_rgba == 0 )
	{
		return false;
	}

	atlas_map_t* atlas_map = NULL;
	color_map_t::iterator cit = m_rAtlasMap.find(color_rgba);
	if ( cit == m_rAtlasMap.end() )
	{
		atlas_map = new atlas_map_t;
		m_rAtlasMap.insert(std::make_pair(color_rgba, atlas_map));
	}
	else
	{
		atlas_map = cit->second;
	}

	FKCW_RichHtml_RichAtlas* atlas = NULL;
	atlas_map_t::iterator ait = atlas_map->find(texture);
	if ( ait == atlas_map->end() )
	{
		atlas = FKCW_RichHtml_RichAtlas::create(this, texture, m_rElements.size());
		cocos2d::ccColor3B color = ccc3( color_rgba & 0xff, color_rgba >> 8 & 0xff, color_rgba >> 16 & 0xff);
		atlas->setColor(color);
		atlas->setOpacity(color_rgba >> 24 & 0xff);

		atlas->retain();
		atlas_map->insert(std::make_pair(texture, atlas));
		atlas->retain();
		m_rAtlasList.push_back(atlas);

		getOverlay()->addChild(atlas, zorder);
	}
	else
	{
		atlas = ait->second;
	}

	return atlas;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::draw()
{
	FKCW_RichHtml_Canvas canvas;
	canvas.root = this;
	canvas.rect/*.size*/ = getCompositor()->getRect()/*.size*/;

	for ( element_list_t::iterator it = m_rElements.begin(); it != m_rElements.end(); it++ )
	{
		(*it)->render(canvas);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichNode::setPlainMode(bool on) 
{ 
	if ( getParser()->isPlainMode() != on )
	{
		getParser()->setPlainMode(on); 

		updateAll();
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichNode::FKCW_RichHtml_RichNode(class CCNode* container)
	: m_rContainer(container)
	, m_rParser(NULL)
	, m_rCompositor(NULL)
	, m_rOverlays(NULL)
{
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichNode::~FKCW_RichHtml_RichNode()
{
	clearAtlasMap();
	clearRichElements();

	if ( m_rOverlays )
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(m_rOverlays);
	}

	CC_SAFE_RELEASE(m_rOverlays);
	CC_SAFE_DELETE(m_rParser);
	CC_SAFE_DELETE(m_rCompositor);
}
//-------------------------------------------------------------------------
// HTMLNode
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLNode* FKCW_RichHtml_HTMLNode::createWithContainer(class CCNode* container)
{
	FKCW_RichHtml_HTMLNode* node = new FKCW_RichHtml_HTMLNode(container);
	if ( node->initialize() )
	{
		node->autorelease();
		return node;
	}

	CC_SAFE_DELETE(node);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_HTMLNode::initialize()
{
	m_rParser = new FKCW_RichHtml_SimpleHTMLParser(this);
	m_rCompositor = new FKCW_RichHtml_SimpleHTMLCompositor(this);

	return m_rParser && m_rCompositor;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_HTMLNode::FKCW_RichHtml_HTMLNode(class CCNode* container)
	: FKCW_RichHtml_RichNode(container)
{

}
//-------------------------------------------------------------------------