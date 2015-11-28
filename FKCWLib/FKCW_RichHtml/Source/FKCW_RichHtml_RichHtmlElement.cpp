//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlElement.h"
#include <cocos-ext.h>
//-------------------------------------------------------------------------
// EleBase
FKCW_RichHtml_EleBase::attrs_t* FKCW_RichHtml_EleBase::parseAttributes(const char** attrs)
{
	attrs_t* attrmap = new attrs_t;

	if ( attrs )
	{
		for ( size_t i = 0; attrs[i] != NULL && attrs[i+1] != NULL; i+=2 )
		{
			attrmap->insert(std::make_pair<std::string, std::string>(attrs[i], attrs[i+1]));
		}
	}

	return attrmap;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleBase::hasAttribute(attrs_t* attrs, const char* attr)
{
	CCAssert(attr, "");

	attrs_t::iterator it = attrs->find(attr);
	if ( it != attrs->end() )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------
CCNode* FKCW_RichHtml_EleBase::createDrawSolidPolygonNode(FKCW_RichHtml_Canvas canvas)
{
	CCDrawNode* drawNode = CCDrawNode::create();

	FKCW_RichHtml_Rect rect = m_rMetrics.rect;
	FKCW_RichHtml_Pos gp = getGlobalPosition();
	short left = gp.x;
	short top = gp.y;		// + canvas.root->getActualSize().h;
	short right = left + rect.size.w;
	short bottom = top - rect.size.h;

	CCPoint vertices[4] = {
		ccp(left, bottom),	// lb
		ccp(right,bottom),	// rb
		ccp(right,top),		// rt
		ccp(left,top),		// lt
	};

	unsigned int color = getColor();
	ccColor4F color4f = ccc4FFromccc4B(ccc4(color & 0xff, color >> 8 & 0xff, color >> 16 & 0xff, color >> 24 & 0xff));

	drawNode->drawPolygon(vertices, 4, color4f, 0.0f, color4f);

	return drawNode;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleBase::parse(class FKCW_RichHtml_IRichParser* parser, const char** attr /*= NULL*/) 
{ 
	attrs_t* attrs = parseAttributes(attr);
	CCAssert(attrs, "");

	if ( hasAttribute(attrs, "id") )
	{
		m_rID = atoi( (*attrs)["id"].c_str() );
	}

	bool success = onParseAttributes(parser, attrs);

	CC_SAFE_DELETE(attrs);

	return success; 
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleBase::composit(class FKCW_RichHtml_IRichCompositor* compositor) 
{
	// 设置位置 
	FKCW_RichHtml_MetricsState* state = compositor->getMetricsState();

	m_rPos.x = state->pen_x;
	m_rPos.y = state->pen_y;

	onCompositStart(compositor);

	if ( pushMetricsState() )
	{
		compositor->pushMetricsState();

		// 计算新位置
		FKCW_RichHtml_MetricsState* mstate = compositor->getMetricsState();
		mstate->zone.pos.add(m_rPos);
		mstate->pen_x = 0;
		mstate->pen_y = 0;
	}

	if ( pushRenderState() )
	{
		compositor->pushRenderState();
	}

	onCompositStatePushed(compositor);

	FKCW_RichHtml_Rect minrect;
	element_list_t* children = getChildren();
	if ( children )
	{
		for ( element_list_t::iterator it = children->begin(); it != children->end(); it++ )
		{
			(*it)->composit(compositor);
			if ( !(*it)->isCachedComposit() )
			{
				FKCW_RichHtml_Rect subrect = (*it)->getMetrics()->rect;

				if ( pushMetricsState() )
					subrect.pos.add((*it)->getLocalPosition());
				minrect.extend(subrect);
			}
		}
	}

	// flush最后一行
	if ( getParent() == NULL )
	{
		FKCW_RichHtml_Rect subrect = compositor->getMetricsState()->elements_cache->flush(compositor);
		minrect.extend(subrect);
	}

	onCompositChildrenEnd(compositor);

	if ( pushRenderState() )
	{
		compositor->popRenderState();
	}
	if ( pushMetricsState() )
	{
		compositor->popMetricsState();
	}

	// 设置Rect区域
	m_rMetrics.rect.extend(minrect);

	if ( onCompositFinish(compositor) )
	{
		if ( isCachedComposit() )
		{
			compositor->getMetricsState()->elements_cache->appendElement(this);
		}
		else
		{
			state->pen_x += m_rMetrics.advance.x;
			state->pen_y += m_rMetrics.advance.y;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleBase::render(FKCW_RichHtml_Canvas canvas)
{
	// 计算全局位置
	m_rGlobalPos.x = m_rPos.x + canvas.rect.pos.x + m_rMetrics.rect.pos.x;
	m_rGlobalPos.y = m_rPos.y + canvas.rect.pos.y + m_rMetrics.rect.pos.y;

	onRenderPrev(canvas);

	element_list_t* children = getChildren();
	if ( children )
	{
		FKCW_RichHtml_Canvas push_canvas = canvas;

		if ( pushMetricsState() )
		{
			push_canvas.rect.pos = m_rGlobalPos;
			push_canvas.rect.size = m_rMetrics.rect.size;
		}

		for ( element_list_t::iterator it = children->begin(); it != children->end(); it++ )
		{
			(*it)->render(push_canvas);
		}
	}

	onRenderPost(canvas);

	if( g_bIsFKCWRichHtmlDebug )
	{
		// 包围盒
		FKCW_RichHtml_Rect rect = m_rMetrics.rect;
		FKCW_RichHtml_Pos gp = getGlobalPosition();
		short left = gp.x;
		short top = gp.y;// + canvas.root->getActualSize().h;
		short right = left + rect.size.w;
		short bottom = top - rect.size.h;
		CCPoint vertices[4]={
			ccp(left,bottom),ccp(right,bottom),
			ccp(right,top),ccp(left,top),
		};
		ccDrawColor4B(0xff, 0xff, 0x00, 0xff);
		ccDrawPoly(vertices, 4, true);
	}
}
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_EleBase::getChildren()
{
	return m_rChildren;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleBase::addChildren(FKCW_RichHtml_IRichElement* child)
{
	CCAssert ( child && child->getParent() == NULL, "[CCRich]invalid element!" );

	if ( m_rChildren == NULL )
	{
		m_rChildren = new element_list_t;
	}

	m_rChildren->push_back(child);
	child->setParent(this);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleBase::removeAllChildren()
{
	if ( m_rChildren )
	{
		for ( element_list_t::iterator it = m_rChildren->begin(); it != m_rChildren->end(); it++ )
		{
			delete *it;
		}
		m_rChildren->clear();
		CC_SAFE_DELETE(m_rChildren);
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichElement* FKCW_RichHtml_EleBase::getParent()
{
	return m_rParent;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleBase::setParent(FKCW_RichHtml_IRichElement* parent)
{
	m_rParent = parent;
}
//-------------------------------------------------------------------------
int	FKCW_RichHtml_EleBase::getID()
{
	return m_rID;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_IRichElement* FKCW_RichHtml_EleBase::findChildByID(int _id)
{
	if ( m_rID == _id )
		return this;

	element_list_t* children = getChildren();
	if ( getChildren() )
	{
		for ( element_list_t::iterator it = children->begin(); it != children->end(); it++ )
		{
			FKCW_RichHtml_IRichElement* child = (*it)->findChildByID(_id);
			if ( child )
				return child;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleBase::FKCW_RichHtml_EleBase()
: m_rID(0)
, m_rChildren(NULL)
, m_rParent(NULL)
, m_rPos()
, m_rGlobalPos()
, m_rMetrics()
, m_rTexture()
, m_rColor(0xffffffff)
, m_rDirty(false)
{

}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleBase::~FKCW_RichHtml_EleBase()
{
	removeAllChildren();
	m_rParent = NULL;
}
//-------------------------------------------------------------------------
CCNode* FKCW_RichHtml_EleSolidPolygon::createDrawNode(FKCW_RichHtml_Canvas canvas)
{
	this->render(canvas);
	return createDrawSolidPolygonNode(canvas);
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleBatchedDrawable::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor) 
{
	m_rDirty = true;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleBatchedDrawable::onRenderPrev(FKCW_RichHtml_Canvas canvas)
{
	if ( m_rDirty )
	{
		m_rDirty = false;

		// 添加到Bacth中
		CCTexture2D* ob_texture = NULL;
		if ( NULL != this->getTexture() 
			&& NULL != (ob_texture = this->getTexture()->getTexture()) )
		{
			FKCW_RichHtml_IRichAtlas* atlas = canvas.root->findAtlas(ob_texture, getColor());

			if (atlas)
			{
				atlas->appendRichElement(this);
			}
		}
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleBatchedDrawable::FKCW_RichHtml_EleBatchedDrawable()
{

}
//-------------------------------------------------------------------------
void FKCW_RichHtml_AtlasHelper::onRenderPrev(FKCW_RichHtml_Canvas canvas)
{
	if ( m_rDirty )
	{
		m_rDirty = false;

		// 添加到Batch中
		CCTexture2D* ob_texture = NULL;
		if ( NULL != this->getTexture() 
			&& NULL != (ob_texture = this->getTexture()->getTexture()) )
		{
			FKCW_RichHtml_IRichAtlas* atlas = canvas.root->findAtlas(ob_texture, getColor(), ZORDER_BACKGROUND);

			if (atlas)
			{
				atlas->appendRichElement(this);
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleGlyph::onCompositStart(class FKCW_RichHtml_IRichCompositor* compositor)
{
	if ( !compositor->getFont() )
		return;

	m_slot = compositor->getFont()->require_char(m_charcode);

	if ( m_slot )
	{
		m_rMetrics.rect.pos = FKCW_RichHtml_Pos((short)m_slot->metrics.left, (short)m_slot->metrics.top);
		m_rMetrics.rect.size = FKCW_RichHtml_Size((short)m_slot->metrics.width, (short)m_slot->metrics.height);
		m_rMetrics.advance.x = m_slot->metrics.advance_x;
		m_rMetrics.advance.y = 0;//m_slot->metrics.advance_y;

		m_rTexture.setTexture(m_slot->texture->user_texture<CCTexture2D>());
		m_rTexture.rect.pos = FKCW_RichHtml_Pos((short)m_slot->padding_rect.origin_x, (short)m_slot->padding_rect.origin_y);
		m_rTexture.rect.size = FKCW_RichHtml_Size((short)m_slot->padding_rect.width, (short)m_slot->padding_rect.height);

		FKCW_RichHtml_RenderState* state = compositor->getRenderState();
		m_font_alias = state->font_alias;
		m_rColor = state->color;
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleGlyph::FKCW_RichHtml_EleGlyph(unsigned int charcode)
	: m_charcode(charcode), m_slot(NULL)
{

}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleGlyph::~FKCW_RichHtml_EleGlyph()
{
	CC_SAFE_RELEASE( m_slot );
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleBase::attrs_t* FKCW_RichHtml_EleHTMLNode::parseStyle(const std::string& style_str)
{
	attrs_t* attrs = new attrs_t;
	if ( style_str.empty() )
		return attrs;

	size_t start_pos = 0;
	size_t end_pos = 0;
	size_t sep_pos = std::string::npos;
	while (  start_pos < style_str.size() )
	{
		end_pos = style_str.size();
		sep_pos = style_str.find_first_of(';', start_pos);
		if ( sep_pos != std::string::npos )
			end_pos = sep_pos;

		std::string phase = style_str.substr(start_pos, end_pos - start_pos);
		start_pos = style_str.find_first_not_of("; ", end_pos);

		size_t kv_pos = phase.find_first_of(":");
		if ( kv_pos != std::string::npos )
		{
			size_t v_start = phase.find_first_not_of(": ", kv_pos);
			size_t v_last = phase.find_last_not_of(' ');
			(*attrs)[phase.substr(0, kv_pos)] = phase.substr(v_start, v_last - v_start + 1);
		}
	}

	return attrs;
}
//-------------------------------------------------------------------------
// HtmlNode
//-------------------------------------------------------------------------
FKCW_RichHtml_Margin FKCW_RichHtml_EleHTMLNode::parseMargin(const std::string& str)
{
	FKCW_RichHtml_Margin margin;

	size_t start_pos = 0;
	size_t sep_pos = 0;

	start_pos = str.find_first_not_of(' ', sep_pos);
	sep_pos = str.find_first_of(' ', start_pos);
	std::string phase = str.substr(start_pos, sep_pos - start_pos);
	margin.top = parsePixel(phase);

	if ( sep_pos == std::string::npos )
		return margin;
	
	start_pos = str.find_first_not_of(' ', sep_pos);
	sep_pos = str.find_first_of(' ', start_pos);
	phase = str.substr(start_pos, sep_pos - start_pos);
	margin.right = parsePixel(phase);
	
	if ( sep_pos == std::string::npos )
		return margin;

	start_pos = str.find_first_not_of(' ', sep_pos);
	sep_pos = str.find_first_of(' ', start_pos);
	phase = str.substr(start_pos, sep_pos - start_pos);
	margin.bottom = parsePixel(phase);

	if ( sep_pos == std::string::npos )
		return margin;

	start_pos = str.find_first_not_of(' ', sep_pos);
	sep_pos = str.find_first_of(' ', start_pos);
	phase = str.substr(start_pos, sep_pos - start_pos);
	margin.left = parsePixel(phase);

	return margin;
}
//-------------------------------------------------------------------------
unsigned int FKCW_RichHtml_EleHTMLNode::parseColor(const std::string& color_str)
{
	unsigned int color = 0;
	if ( !color_str.empty() )
	{
		if ( color_str[0] == '#' 
			&& (color_str.size() == 7 || color_str.size() == 9) ) // RGB || RGBA
		{
			if ( color_str.size() == 7 )
			{
				color = 0xff;
			}

			for ( size_t i = color_str.size() - 1; i > 1; i-=2 )
			{
				color = (color<<4) + FKCW_RichHtml_TransferHEXValue(color_str[i-1]);
				color = (color<<4) + FKCW_RichHtml_TransferHEXValue(color_str[i]);
			}
		}
	}

	return color;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_OptSize FKCW_RichHtml_EleHTMLNode::parseOptSize(const std::string& str)
{
	FKCW_RichHtml_OptSize size;
	size.ratio = parsePercent(str);
	if ( 0 == size.ratio )
	{
		size.absolute = parsePixel(str);
	}
	return size;
}
//-------------------------------------------------------------------------
short FKCW_RichHtml_EleHTMLNode::parsePixel(const std::string& str)
{
	size_t last_pos = str.find_last_not_of(" px"); // 跳过px
	short pixels = atoi(str.substr(0, last_pos + 1).c_str());
	return pixels;
}
//-------------------------------------------------------------------------
float FKCW_RichHtml_EleHTMLNode::parsePercent(const std::string& str)
{
	float ratio = 0.0f;
	if ( !str.empty() && str[str.size()-1] == '%' )
	{
		std::string value_part = str.substr(0, str.size() - 1);
		ratio = (float)atof(value_part.c_str());
		ratio *= 0.01f;
	}
	return ratio;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLNode::parseAlignment(const std::string& str, ENUM_Alignment& align)
{
	if ( str.empty() )
		return false;

	if ( strcmp(str.c_str(), "left") == 0 )
	{
		align = e_align_left;
	}
	else if ( strcmp(str.c_str(), "right") == 0 )
	{
		align = e_align_right;
	}
	else if ( strcmp(str.c_str(), "center") == 0 )
	{
		align = e_align_center;
	}
	else if ( strcmp(str.c_str(), "top") == 0 )
	{
		align = e_align_top;
	}
	else if ( strcmp(str.c_str(), "bottom") == 0 )
	{
		align = e_align_bottom;
	}
	else if ( strcmp(str.c_str(), "middle") == 0 )
	{
		align = e_align_middle;
	}
	else
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------
// p
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLP::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	FKCW_RichHtml_EleBase::attrs_t* style_attrs = NULL;

	if ( hasAttribute(attrs, "style") )
	{
		style_attrs = parseStyle((*attrs)["style"]);

		// 解析对齐方式
		if ( hasAttribute(style_attrs, "text-align") )
		{
			ENUM_Alignment align = e_align_left;
			parseAlignment((*style_attrs)["text-align"], align);
			m_FKCW_RichHtml_LineCache.setHAlign(align);
		}
		
		// 解析包围线框
		if ( hasAttribute(style_attrs, "white-space") )
		{
			if ( strcmp( (*style_attrs)["white-space"].c_str(), "nowrap") == 0 )
			{
				m_FKCW_RichHtml_LineCache.setWrapline(false);
			}
			else
			{
				m_FKCW_RichHtml_LineCache.setWrapline(true);
			}
		}

		// 颜色
		m_rColor = parseColor((*style_attrs)["color"]);

		// 字体
		m_rFontAlias = (*style_attrs)["font"];

		// 线高
		if ( hasAttribute(style_attrs, "line-height") )
			m_FKCW_RichHtml_LineCache.setLineHeight( parsePixel((*style_attrs)["line-height"]) );

		// 边缘
		if ( hasAttribute(style_attrs, "margin") )
		{
			FKCW_RichHtml_Margin margin = parseMargin((*style_attrs)["margin"]);
			m_FKCW_RichHtml_LineCache.setSpacing(margin.top);
		}

		// 填充
		if ( hasAttribute(style_attrs, "padding") )
		{
			FKCW_RichHtml_Margin padding = parseMargin((*style_attrs)["padding"]);
			m_FKCW_RichHtml_LineCache.setPadding(padding.top);
		}

		CC_SAFE_DELETE(style_attrs);
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLP::onCompositStatePushed(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_MetricsState* mstate = compositor->getMetricsState();
	mstate->elements_cache = &m_FKCW_RichHtml_LineCache;

	if ( m_rColor )
	{
		compositor->getRenderState()->color = m_rColor;
	}

	if ( !m_rFontAlias.empty() )
	{
		compositor->getRenderState()->font_alias = m_rFontAlias.c_str();
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLP::onCompositChildrenEnd(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_Rect rect = m_FKCW_RichHtml_LineCache.flush(compositor);
	m_rMetrics.rect.extend(rect);
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLP::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor) 
{
	return true; 
}
//-------------------------------------------------------------------------
// br
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLBR::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor)
{
	m_rMetrics.rect.size.h = compositor->getFont()->char_height();
	m_rMetrics.rect.pos.y = m_rMetrics.rect.size.h;
	return true;
}
//-------------------------------------------------------------------------
// font
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLFont::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	m_rFont = (*attrs)["face"];
	m_rColor = parseColor((*attrs)["color"]);

	return true;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLFont::FKCW_RichHtml_EleHTMLFont()
	: m_rColor(0)
{

}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLFont::onCompositStatePushed(class FKCW_RichHtml_IRichCompositor* compositor)
{
	if ( !m_rFont.empty() )
		compositor->getRenderState()->font_alias = m_rFont.c_str();

	if ( m_rColor != 0 )
		compositor->getRenderState()->color = m_rColor;
}
//-------------------------------------------------------------------------
// span
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLSpans::travesalChildrenSpans(
	element_list_t* eles, const char*& font, short& start_span_x, short& span_y, short& underline_thickness, 
	short& end_span_x, short& min_span_y, short& max_span_y, unsigned int& color, bool is_root)
{
	if ( !eles )
		return;

	for ( element_list_t::iterator it = eles->begin(); it != eles->end(); it++  )
	{
		if ( !(*it)->pushMetricsState() )
		{
			travesalChildrenSpans(
				(*it)->getChildren(), font,
				start_span_x, span_y, underline_thickness,
				end_span_x, min_span_y, max_span_y, color, false);
		}

		bool process_begin_after_end = false;
		bool process_end = false;
		bool process_advance = false;
		
		FKCW_RichHtml_Metrics* metrics = (*it)->getMetrics();
		FKCW_RichHtml_Pos pos = (*it)->getLocalPosition();

		short this_thickness = metrics->rect.size.h / 20 + 1;
		unsigned int this_color = (*it)->getColor();
		const char* this_font = (*it)->getFontAlias();

		if ( this_font )
		{
			// 没有下划线之前
			if ( font == NULL ) 
			{
				start_span_x = pos.x + metrics->rect.pos.x;
				span_y = pos.y;
				underline_thickness = this_thickness;
				end_span_x = start_span_x + metrics->rect.size.w;
				min_span_y = metrics->rect.min_y();
				max_span_y = metrics->rect.max_y();
				color = (*it)->getColor();
				font = this_font;
			}
			// 高级下划线
			else if ( span_y == pos.y && strcmp(this_font, font) == 0 && this_color == color )
			{
				process_advance = true;
			}
			// 新下划线
			else
			{
				process_end = true;
				process_begin_after_end = true;
			}
		}
		else
		{
			// 下划线结束
			if ( font )
			{
				process_end = true;
			}

			// 为无字符元素添加一个单独标签
			if ( metrics->rect.size.w != 0 && metrics->rect.size.h != 0 )
			{
				FKCW_RichHtml_Rect span;
				span.pos.x = pos.x + metrics->rect.pos.x;
				span.pos.y = pos.y + metrics->rect.pos.y;
				span.size = FKCW_RichHtml_Size(metrics->rect.size.w, metrics->rect.size.h);
				m_rSpans.push_back(span);

				FKCW_RichHtml_EleSolidPolygon* background_drawable = new FKCW_RichHtml_EleSolidPolygon;
				background_drawable->setLocalPosition(span.pos);
				background_drawable->getMetrics()->rect.size = span.size;
				background_drawable->setRColor(getBGColor());
				m_rBackgroudDrawables.push_back(background_drawable);
			}
		}

		// 最后一个元素
		if ( it+1 == eles->end() && is_root)
		{
			process_end = true;
		}

		// 额外处理
		if ( process_advance )
		{
			min_span_y = MIN(min_span_y, metrics->rect.min_y());
			max_span_y = MAX(max_span_y, metrics->rect.max_y());
			end_span_x = pos.x + metrics->rect.pos.x + metrics->rect.size.w;
			underline_thickness = MAX(underline_thickness, this_thickness);
		}

		// 结束处理
		if ( process_end && font )
		{
			FKCW_RichHtml_EleSolidPolygon* underline_drawable = new FKCW_RichHtml_EleSolidPolygon;
			underline_drawable->setLocalPosition(FKCW_RichHtml_Pos(start_span_x, span_y + min_span_y));
			underline_drawable->getMetrics()->rect.size = FKCW_RichHtml_Size(end_span_x - start_span_x, underline_thickness);
			underline_drawable->setRColor(color);
			m_rUnderlineDrawables.push_back(underline_drawable);

			FKCW_RichHtml_Rect span;
			span.pos.x = start_span_x;
			span.pos.y = span_y + max_span_y;
			span.size = FKCW_RichHtml_Size(end_span_x - start_span_x, max_span_y - min_span_y);
			m_rSpans.push_back(span);

			FKCW_RichHtml_EleSolidPolygon* background_drawable = new FKCW_RichHtml_EleSolidPolygon;
			background_drawable->setLocalPosition(span.pos);
			background_drawable->getMetrics()->rect.size = span.size;
			background_drawable->setRColor(getBGColor());
			m_rBackgroudDrawables.push_back(background_drawable);

			start_span_x = 0;
			end_span_x = 0;
			underline_thickness = 0;
			max_span_y = 0;
			font = NULL;
		}

		// 开始处理
		if ( process_begin_after_end )
		{
			start_span_x = pos.x + metrics->rect.pos.x;
			span_y = pos.y;
			underline_thickness = this_thickness;
			end_span_x = start_span_x + metrics->rect.size.w;
			min_span_y = metrics->rect.min_y();
			max_span_y = metrics->rect.max_y();
			color = (*it)->getColor();
			font = this_font;
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLSpans::onRenderPost(FKCW_RichHtml_Canvas canvas) 
{
	// 创建渲染下划线
	if ( m_rDirty )
	{
		clearAllSpans();

		short start_pen_x = 0;
		short pen_y = 0;
		short thickness = 0;
		short end_draw_x = 0;
		short draw_y = 0;
		short max_height = 0;
		unsigned int color = 0;
		const char* font = 0;

		travesalChildrenSpans(
			this->getChildren(), font, start_pen_x, pen_y, thickness, end_draw_x, draw_y, max_height, color, true);

		// 绘制下划线
		if ( isDrawUnderline() )
		{
			for ( size_t i = 0; i < m_rUnderlineDrawables.size(); i++ )
			{
				CCNode* drawNode = m_rUnderlineDrawables[i]->createDrawNode(canvas);
				drawNode->setZOrder(ZORDER_OVERLAY);
				canvas.root->addCCNode(drawNode);
			}
		}

		// 绘制背景色
		if ( isDrawBackground() )
		{
			for ( size_t i = 0; i < m_rBackgroudDrawables.size(); i++ )
			{
				CCNode* drawNode = m_rBackgroudDrawables[i]->createDrawNode(canvas);
				drawNode->setZOrder(ZORDER_OVERLAY);
				canvas.root->addCCNode(drawNode);
			}
		}

		m_rDirty = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLSpans::clearAllSpans()
{
	for ( size_t i = 0; i < m_rUnderlineDrawables.size(); i++ )
	{
		CC_SAFE_DELETE(m_rUnderlineDrawables[i]);
	}
	m_rUnderlineDrawables.clear();

	for ( size_t i = 0; i < m_rBackgroudDrawables.size(); i++ )
	{
		CC_SAFE_DELETE(m_rBackgroudDrawables[i]);
	}
	m_rBackgroudDrawables.clear();

	m_rSpans.clear();
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLSpans::FKCW_RichHtml_EleHTMLSpans()
	: m_rDrawUnderline(false), m_rDrawBackground(false), m_rBGColor(0xffffffff)
{
	m_rDirty = true;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLSpans::~FKCW_RichHtml_EleHTMLSpans()
{
	clearAllSpans();
}
//-------------------------------------------------------------------------
// u
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLU::FKCW_RichHtml_EleHTMLU()
{
	setDrawUnderline(true);
}
//-------------------------------------------------------------------------
// hr
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLHR::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	unsigned int color = 0;

	m_rSize = FKCW_RichHtml_EleHTMLNode::parsePixel((*attrs)["size"]);
	m_rWidth = FKCW_RichHtml_EleHTMLNode::parseOptSize( (*attrs)["width"] );

	if ( m_rSize == 0 )
	{
		m_rSize = 1;
	}

	if ( m_rWidth.absolute == 0 && m_rWidth.ratio == 0 )
	{
		m_rWidth.ratio = 1.0f;
	}

	if ( hasAttribute(attrs, "style") )
	{
		attrs_t* style_attrs = FKCW_RichHtml_EleHTMLNode::parseStyle((*attrs)["style"]);

		if ( hasAttribute(style_attrs, "color") )
		{
			unsigned int color = FKCW_RichHtml_EleHTMLNode::parseColor( (*style_attrs)["color"] );
			m_rColor = color;
		}
		CC_SAFE_DELETE(style_attrs);
	}

	m_rDirty = true;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLHR::onCachedCompositBegin(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen)
{
	pen.y -= cache->getSpacing();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLHR::onCachedCompositEnd(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen)
{
	pen.y -= cache->getSpacing();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLHR::onCompositStart(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_MetricsState* metrics = compositor->getMetricsState();
	m_rMetrics.rect.size.w = metrics->zone.size.w - metrics->elements_cache->getPadding() * 2;
	m_rMetrics.rect.size.w = m_rWidth.getValueReal(m_rMetrics.rect.size.w);
	m_rMetrics.rect.size.h = m_rSize;
	m_rMetrics.rect.pos.y = m_rMetrics.rect.size.h;

	if ( m_rColor == 0 )
		m_rColor = compositor->getRenderState()->color;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLHR::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor)
{
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLHR::onRenderPost(FKCW_RichHtml_Canvas canvas)
{
	if (m_rDirty)
	{
		CCNode* drawNode = createDrawSolidPolygonNode(canvas);
		drawNode->setZOrder(ZORDER_CONTEXT);
		canvas.root->addCCNode(drawNode);
		m_rDirty = false;
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLHR::FKCW_RichHtml_EleHTMLHR()
	: m_rSize(1), m_rTempPadding(0)
{

}
//-------------------------------------------------------------------------
// table
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLCell::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	m_rWidth = parseOptSize((*attrs)["width"]);
	m_rHeight = parseOptSize((*attrs)["height"]);

	m_rHAlignSpecified = parseAlignment((*attrs)["align"], m_rHAlignment);
	m_rVAlignSpecified = parseAlignment((*attrs)["valign"], m_rVAlignment);

	short padding = parsePixel((*attrs)["padding"]);
	short spacing = parsePixel((*attrs)["spacing"]);
	m_FKCW_RichHtml_LineCache.setPadding(padding);
	m_FKCW_RichHtml_LineCache.setSpacing(spacing);

	if ( strcmp((*attrs)["nowrap"].c_str(), "nowrap") == 0 )
	{
		m_FKCW_RichHtml_LineCache.setWrapline(false);
	}

	// 颜色
	m_rColor = parseColor((*attrs)["bgcolor"]);

	m_rBGTexture.setDirty(false);
	if ( hasAttribute(attrs, "bg-image") )
	{
		std::string bg_filename = (*attrs)["bg-image"];
		CCTexture2D* texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(bg_filename.c_str());
		if ( texture )
		{
			m_rBGTexture.setDirty(true);
			if (m_rColor) 
			{
				m_rBGTexture.setRColor(m_rColor);
				m_rColor = 0;
			}
			FKCW_RichHtml_Texture* bg_texture = m_rBGTexture.getTexture();
			bg_texture->setTexture(texture);

			if ( hasAttribute(attrs, "bg-rect") )
			{
				FKCW_RichHtml_Margin margin = FKCW_RichHtml_EleHTMLNode::parseMargin( (*attrs)["bg-rect"] );
				bg_texture->rect.pos.x = margin.left;
				bg_texture->rect.pos.y = margin.top;
				bg_texture->rect.size.h = margin.bottom - margin.top;
				bg_texture->rect.size.w = margin.right - margin.left;
			}
			else
			{
				bg_texture->rect.size.w = texture->getPixelsWide();
				bg_texture->rect.size.h = texture->getPixelsHigh();
			}
		}
	}

	m_rDirty = true;

	return true;
}
//-------------------------------------------------------------------------
// cell
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLCell::onRenderPrev(FKCW_RichHtml_Canvas canvas)
{
	// 渲染背景
	if ( m_rDirty || m_rBGTexture.isDirty() )
	{
		FKCW_RichHtml_Rect rect = m_rMetrics.rect;
		FKCW_RichHtml_Pos gp = getGlobalPosition();
		short left = gp.x;
		short top = gp.y;
		short right = left + rect.size.w;
		short bottom = top - rect.size.h - 1;

		if (m_rDirty && m_rColor)
		{
			CCNode* drawNode = createDrawSolidPolygonNode(canvas);
			drawNode->setZOrder(ZORDER_BACKGROUND);
			canvas.root->addCCNode(drawNode);
		}

		if ( m_rBGTexture.isDirty() )
		{
			m_rBGTexture.setGlobalPosition(gp);
			m_rBGTexture.getMetrics()->rect.size = FKCW_RichHtml_Size(rect.size.w, rect.size.h);
			m_rBGTexture.onRenderPrev(canvas);
		}

		m_rDirty = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLCell::onCompositStatePushed(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_MetricsState* mstate = compositor->getMetricsState();
	mstate->elements_cache = &m_FKCW_RichHtml_LineCache;

	mstate->zone.size.w = m_rRow->getCellWidth(m_rIndexNumber, m_rWidth);
	mstate->zone.size.h = m_rHeight.getValueReal(mstate->zone.size.h);
	m_rMetrics.rect.size = mstate->zone.size;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLCell::onCompositChildrenEnd(class FKCW_RichHtml_IRichCompositor* compositor)
{
	m_rContentSize = m_FKCW_RichHtml_LineCache.flush(compositor);
	m_rMetrics.rect.extend(m_rContentSize);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLCell::FKCW_RichHtml_EleHTMLCell(class FKCW_RichHtml_EleHTMLRow* row)
	: m_rRow(row), m_rHAlignSpecified(false), m_rVAlignSpecified(false), m_rIndexNumber(0),
	m_rHAlignment(e_align_left), m_rVAlignment(e_align_bottom)
{
	m_FKCW_RichHtml_LineCache.setHAlign(e_align_left);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLCell::~FKCW_RichHtml_EleHTMLCell()
{
}
//-------------------------------------------------------------------------
// row
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLRow::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	m_rHAlignSpecified = parseAlignment((*attrs)["align"], m_rHAlignment);
	m_rVAlignSpecified = parseAlignment((*attrs)["valign"], m_rVAlignment);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLRow::onCompositStatePushed(class FKCW_RichHtml_IRichCompositor* compositor)
{
	m_rLeftWidth = m_rTable->getZoneWidth();
}
//-------------------------------------------------------------------------
std::vector<class FKCW_RichHtml_EleHTMLCell*>& FKCW_RichHtml_EleHTMLRow::getCells() 
{ 
	return m_rCells; 
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLRow::addChildren(FKCW_RichHtml_IRichElement* child)
{
	FKCW_RichHtml_EleHTMLNode::addChildren(child);

	if(dynamic_cast<class FKCW_RichHtml_EleHTMLCell*>(child))
	{
		dynamic_cast<class FKCW_RichHtml_EleHTMLCell*>(child)->setIndex((int)m_rCells.size());
		m_rCells.push_back(dynamic_cast<class FKCW_RichHtml_EleHTMLCell*>(child));
	}
}
//-------------------------------------------------------------------------
class FKCW_RichHtml_EleHTMLTable* FKCW_RichHtml_EleHTMLRow::getTable()
{
	return m_rTable;
}
//-------------------------------------------------------------------------
short FKCW_RichHtml_EleHTMLRow::getCellWidth(int index, FKCW_RichHtml_OptSize width)
{
	CCAssert(index < (int)m_rCells.size(), "Invalid Cell Index!");
	if ( (m_rLeftWidth == 0 && width.isZero()) || m_rCells.size() == 0 )
		return 0;

	short returned_width = 0;
	if ( width.isZero() )
	{
		returned_width =  m_rLeftWidth / (m_rCells.size() - index);
	}
	else
	{
		returned_width = width.getValueReal(m_rTable->getZoneWidth());
	}

	m_rLeftWidth -= returned_width;
	m_rLeftWidth = MAX(0, m_rLeftWidth);
	return returned_width;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLRow::FKCW_RichHtml_EleHTMLRow(class FKCW_RichHtml_EleHTMLTable* table)
	: m_rTable(table), m_rHAlignSpecified(false), m_rVAlignSpecified(false),
	m_rHAlignment(e_align_left), m_rVAlignment(e_align_bottom), m_rLeftWidth(0)
{

}
//-------------------------------------------------------------------------
// table
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLTable::EFrame FKCW_RichHtml_EleHTMLTable::parseFrame(const std::string& str)
{
	if ( str.empty() )
		return e_box;

	if ( strcmp(str.c_str(), "void") == 0 )
	{
		return e_void;
	}
	else if ( strcmp(str.c_str(), "above") == 0 )
	{
		return e_above;
	}
	else if ( strcmp(str.c_str(), "below") == 0 )
	{
		return e_below;
	}
	else if ( strcmp(str.c_str(), "hsides") == 0 )
	{
		return e_hsides;
	}
	else if ( strcmp(str.c_str(), "lhs") == 0 )
	{
		return e_lhs;
	}
	else if ( strcmp(str.c_str(), "rhs") == 0 )
	{
		return e_rhs;
	}
	else if ( strcmp(str.c_str(), "vsides") == 0 )
	{
		return e_vsides;
	}
	else if ( strcmp(str.c_str(), "box") == 0 )
	{
		return e_box;
	}
	else if ( strcmp(str.c_str(), "border") == 0 )
	{
		return e_border;
	}

	return e_box;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLTable::ERules FKCW_RichHtml_EleHTMLTable::parseRules(const std::string& str)
{
	if ( str.empty() )
		return e_all;

	if ( strcmp(str.c_str(), "none") == 0 )
	{
		return e_none;
	}
	else if ( strcmp(str.c_str(), "groups") == 0 )
	{
		return e_groups;
	}
	else if ( strcmp(str.c_str(), "rows") == 0 )
	{
		return e_rows;
	}
	else if ( strcmp(str.c_str(), "cols") == 0 )
	{
		return e_cols;
	}
	else if ( strcmp(str.c_str(), "all") == 0 )
	{
		return e_all;
	}

	return e_all;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLTable::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	m_rWidth = parseOptSize((*attrs)["width"]);


	m_rBorder = hasAttribute(attrs, "border") ?
		parsePixel( (*attrs)["border"] ) : 0;

	short padding =  hasAttribute(attrs, "cellpadding") ?
		parsePixel((*attrs)["cellpadding"]) : 0;
	short spacing = hasAttribute(attrs, "cellspacing") ?
		parsePixel((*attrs)["cellspacing"]) : 0;

	// 颜色
	m_rColor = parseColor((*attrs)["bgcolor"]);
	m_rBorderColor = hasAttribute(attrs, "bordercolor") ?
		parseColor((*attrs)["bordercolor"]) : m_rBorderColor;

	m_rFrame = hasAttribute(attrs, "frame") ?
		parseFrame((*attrs)["frame"]) : e_void;

	m_rRules = hasAttribute(attrs, "rules") ?
		parseRules((*attrs)["rules"]) : e_none;

	m_rHAlignSpecified = parseAlignment((*attrs)["align"], m_rHAlign); // not cell alignment!

	m_rTableCache.setPadding(padding);
	m_rTableCache.setSpacing(spacing);

	m_rDirty = true;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::onCachedCompositBegin(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen)
{
	if ( m_rHAlignSpecified )
	{
		m_rTempAlign = cache->getHAlign();
		cache->setHAlign(m_rHAlign);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::onCachedCompositEnd(class FKCW_RichHtml_ICompositCache* cache, FKCW_RichHtml_Pos& pen)
{
	if ( m_rHAlignSpecified )
		cache->setHAlign(m_rTempAlign);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::addChildren(FKCW_RichHtml_IRichElement* child)
{
	FKCW_RichHtml_EleHTMLNode::addChildren(child);

	if(dynamic_cast<class FKCW_RichHtml_EleHTMLRow*>(child))
		m_rRows.push_back(dynamic_cast<class FKCW_RichHtml_EleHTMLRow*>(child));
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::onCompositStatePushed(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_MetricsState* mstate = compositor->getMetricsState();
	mstate->elements_cache = &m_rTableCache;

	mstate->zone.size.w = m_rWidth.getValueReal(mstate->zone.size.w);
	mstate->zone.size.h = 0;

	size_t cols =  m_rRows.empty() ? 0 : m_rRows[0]->getCells().size();
	short except_width = (cols - 1) * m_rTableCache.getSpacing() + cols * m_rTableCache.getPadding() * 2 + m_rBorder * 2;
	mstate->zone.size.w -= except_width;

	mstate->zone.size.w = MAX(0, mstate->zone.size.w);
	mstate->zone.size.h = MAX(0, mstate->zone.size.h);

	m_rZoneWidth = mstate->zone.size.w;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::onCompositChildrenEnd(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_Rect rect = m_rTableCache.flush(compositor);
	m_rMetrics.rect.extend(rect);
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLTable::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor) 
{
	m_rMetrics.rect.pos.y = m_rMetrics.rect.size.h;
	m_rMetrics.advance.x = m_rMetrics.rect.size.w;
	m_rMetrics.advance.y = 0;
	return true; 
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::drawThicknessLine(short left, short top, short right, short bottom, const ccColor4F& color)
{
	CCPoint vertices[4]={
		ccp(left,bottom),ccp(right,bottom),
		ccp(right,top),ccp(left,top),
	};

	ccDrawSolidPoly(vertices, 4, color);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::createTicknessLineNode(FKCW_RichHtml_Canvas canvas, short left, short top, short right, short bottom, const ccColor4F& color)
{
	CCDrawNode* drawNode = CCDrawNode::create();

	CCPoint vertices[4]={
		ccp(left,bottom),ccp(right,bottom),
		ccp(right,top),ccp(left,top),
	};

	drawNode->drawPolygon(vertices, 4, color, 0.0f, color);
	drawNode->setZOrder(ZORDER_OVERLAY);
	canvas.root->addCCNode(drawNode);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTable::onRenderPrev(FKCW_RichHtml_Canvas canvas) 
{
	if ( !m_rDirty )
		return;

	m_rDirty = false;

	FKCW_RichHtml_Rect rect = m_rMetrics.rect;
	FKCW_RichHtml_Pos gp = getGlobalPosition();
	short left = gp.x;
	short top = gp.y;// + canvas.root->getActualSize().h;
	short right = left + rect.size.w;
	short bottom = top - rect.size.h - 1;
	short spacing = m_rTableCache.getSpacing();

	// 渲染背景
	if ( m_rColor )
	{
		ccColor4B bgcolor4b = ccc4(
			m_rColor & 0xff, 
			m_rColor >> 8 & 0xff, 
			m_rColor >> 16 & 0xff,
			m_rColor >> 24 & 0xff);

		ccColor4F bgcolor4f = ccc4FFromccc4B(bgcolor4b);

		createTicknessLineNode(canvas, left, top, right, bottom, bgcolor4f);
	}

	// 帧颜色
	ccColor4B color4b = ccc4(
		m_rBorderColor & 0xff, 
		m_rBorderColor >> 8 & 0xff, 
		m_rBorderColor >> 16 & 0xff,
		m_rBorderColor >> 24 & 0xff);

	ccColor4F color4f = ccc4FFromccc4B(color4b);

	// 渲染帧线
	if ( m_rBorder > 0 && m_rBorderColor )
	{
		bool draw_top = false;
		bool draw_bottom = false;
		bool draw_left = false;
		bool draw_right = false;

		switch(m_rFrame)
		{
		case e_void:
			break;
		case e_above:
			draw_top = true;
			break;
		case e_below:
			draw_bottom = true;
			break;
		case e_hsides:
			draw_top = true;
			draw_bottom = true;
			break;
		case e_lhs:
			draw_left = true;
			break;
		case e_rhs:
			draw_right = true;
			break;
		case e_vsides:
			draw_left = true;
			draw_right = true;
			break;
		case e_box:
		case e_border:
			draw_top = true;
			draw_bottom = true;
			draw_left = true;
			draw_right = true;
			break;
		}

		if(draw_top)
			createTicknessLineNode(canvas, left, top, right, top - m_rBorder, color4f);
		if(draw_bottom)
			createTicknessLineNode(canvas, left, bottom + m_rBorder, right, bottom, color4f);
		if(draw_left)
			createTicknessLineNode(canvas, left, top, left + m_rBorder, bottom, color4f);
		if(draw_right)
			createTicknessLineNode(canvas, right - m_rBorder, top, right, bottom, color4f);
	}

	bool draw_hline = false;
	bool draw_vline = false;
	switch(m_rRules)
	{
	case e_none:
	case e_groups:
		break;
	case e_rows:
		draw_hline = true;
		break;
	case e_cols:
		draw_vline = true;
		break;
	case e_all:
		draw_hline = true;
		draw_vline = true;
		break;
	};

	// 渲染行
	if (draw_hline)
	{
		for ( size_t i = 1; i < m_rRows.size(); i++ )
		{
			short pen_y = m_rRows[i]->getLocalPosition().y;
			short rleft = left; 
			short rright = right;
			short rtop = top + pen_y + spacing;
			short rbottom = rtop - spacing;

			CCPoint vertices[4] = {
				ccp(rleft,rbottom),ccp(rright,rbottom),
				ccp(rright,rtop),ccp(rleft,rtop)
			};

			createTicknessLineNode(canvas, rleft, rtop, rright, rbottom, color4f);
		}
	}

	// 渲染列
	if ( draw_vline && m_rRows.size() > 0 )
	{
		std::vector<class FKCW_RichHtml_EleHTMLCell*>& cells = m_rRows[0]->getCells();

		for( size_t i = 1; i < cells.size(); i++ )
		{
			short pen_x = cells[i]->getLocalPosition().x;
			short cleft = left + m_rBorder + pen_x - spacing; 
			short cright = left + m_rBorder + pen_x;
			short ctop = top;
			short cbottom = bottom;

			CCPoint vertices[4] = {
				ccp(cleft,cbottom),ccp(cright,cbottom),
				ccp(cright,ctop),ccp(cleft,ctop)
			};
			createTicknessLineNode(canvas, cleft, ctop, cright, cbottom, color4f);
		}
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLTable::FKCW_RichHtml_EleHTMLTable()
: m_rBorderColor(0xff000000) 
, m_rFrame(e_box)
, m_rRules(e_all)
, m_rHAlignSpecified(false)
, m_rHAlign(e_align_left)
, m_rTempAlign(e_align_left)
, m_rZoneWidth(0)
{
	m_rTableCache.setTable(this);
}
//-------------------------------------------------------------------------
// img
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLImg::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	m_filename = (*attrs)["src"];
	m_alt = (*attrs)["alt"];

	if ( hasAttribute(attrs, "texture-rect") )
	{
		FKCW_RichHtml_Margin margin = FKCW_RichHtml_EleHTMLNode::parseMargin( (*attrs)["texture-rect"] );

		m_rTexture.rect.pos.x = margin.left;
		m_rTexture.rect.pos.y = margin.top;
		m_rTexture.rect.size.h = margin.bottom - margin.top;
		m_rTexture.rect.size.w = margin.right - margin.left;
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLImg::onCompositStart(class FKCW_RichHtml_IRichCompositor* compositor)
{
	CCTexture2D* texture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(m_filename.c_str());

	if ( texture )
	{
		m_rTexture.setTexture(texture);

		if ( m_rTexture.rect.size.w == 0 )
		{
			m_rTexture.rect.size.w = texture->getPixelsWide();
		}
		if ( m_rTexture.rect.size.h == 0 )
		{
			m_rTexture.rect.size.h = texture->getPixelsHigh();
		}

		if ( m_rMetrics.rect.size.w == 0 )
		{
			m_rMetrics.rect.size.w = m_rTexture.rect.size.w;
		}

		if ( m_rMetrics.rect.size.h == 0 )
		{
			m_rMetrics.rect.size.h = m_rTexture.rect.size.h;
		}

		m_rMetrics.advance.x = m_rMetrics.rect.pos.x + m_rMetrics.rect.size.w;
		m_rMetrics.advance.y = 0;

		m_rMetrics.rect.pos.y = m_rTexture.rect.size.h;
	}
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLTouchable::isLocationInside(CCPoint location)
{
	FKCW_RichHtml_Pos ele_pos = getGlobalPosition();
	ele_pos.sub(getLocalPosition()); // 修正坐标

	for ( std::list<FKCW_RichHtml_Rect>::iterator it = m_rSpans.begin(); it != m_rSpans.end(); it++ )
	{
		FKCW_RichHtml_Rect local_rect = *it;
		local_rect.pos.add(ele_pos);

		CCRect rect;
		rect.origin.setPoint(local_rect.pos.x, local_rect.min_y());
		rect.size.setSize(local_rect.size.w, local_rect.size.h);

		if ( rect.containsPoint(location) )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLTouchable::onTouchBegan(CCNode* container, CCTouch *touch, CCEvent *evt)
{
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTouchable::onTouchMoved(CCNode* container, CCTouch *touch, CCEvent *evt)
{
	CCPoint pt = container->convertToNodeSpace(touch->getLocation());
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTouchable::onTouchEnded(CCNode* container, CCTouch *touch, CCEvent *evt)
{
	CCPoint pt = container->convertToNodeSpace(touch->getLocation());
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTouchable::onTouchCancelled(CCNode* container, CCTouch *touch, CCEvent *evt)
{
	CCPoint pt = container->convertToNodeSpace(touch->getLocation());
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLTouchable::onCompositStart(class FKCW_RichHtml_IRichCompositor* compositor)
{
	compositor->getContainer()->addOverlay(this);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleHTMLTouchable::FKCW_RichHtml_EleHTMLTouchable()
	: m_rEnabled(false)
{

}
//-------------------------------------------------------------------------
// button
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLButton::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	unsigned int color = 0;

	m_rName = (*attrs)["name"];
	m_rValue = (*attrs)["value"];

	color = FKCW_RichHtml_EleHTMLNode::parseColor((*attrs)["bgcolor"]);

	setDrawUnderline(true);
	setDrawBackground(false);
	if ( color )
	{
		setDrawBackground(true);
		m_rBGColor = color;
	}

	setEnabled(true);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleHTMLButton::onTouchEnded(CCNode* container, CCTouch *touch, CCEvent *evt)
{
	FKCW_RichHtml_EleHTMLTouchable::onTouchEnded(container, touch, evt);
	CCLog("[Rich HTML Button Clicked] name=%s, value=%s", m_rName.c_str(), m_rValue.c_str());
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleHTMLAnchor::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	unsigned int color = 0;

	m_rName = (*attrs)["name"];
	m_rHref = (*attrs)["href"];

	color = FKCW_RichHtml_EleHTMLNode::parseColor((*attrs)["bgcolor"]);

	setDrawUnderline(true);
	setDrawBackground(false);
	if ( color )
	{
		setDrawBackground(true);
		m_rBGColor = color;
	}

	setEnabled(true);

	return true;
}
//-------------------------------------------------------------------------
static FKCW_RichHtml_EleCCBNode::ccb_reader_t s_ccb_reader = NULL;
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleCCBNode::registerCCBReader(ccb_reader_t reader)
{
	s_ccb_reader = reader;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleCCBNode::onParseAttributes(class FKCW_RichHtml_IRichParser* parser, attrs_t* attrs )
{
	unsigned int color = 0;

	m_filename = (*attrs)["src"];
	
	if ( !m_filename.empty() )
	{
		if ( s_ccb_reader )
		{
			m_ccbNode = s_ccb_reader(m_filename.c_str());
		}
		else
		{
			cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			m_ccbNode = ccbReader->readNodeGraphFromFile(m_filename.c_str(), NULL);
			ccbReader->release();
		}

		if ( m_ccbNode )
		{
			m_ccbNode->retain();
			m_ccbNode->setAnchorPoint(ccp(0.0f, 1.0f));
			m_ccbNode->ignoreAnchorPointForPosition(true);
			m_rMetrics.rect.size.w = (short)m_ccbNode->getContentSize().width;
			m_rMetrics.rect.size.h = (short)m_ccbNode->getContentSize().height;
			m_rMetrics.advance.x = m_rMetrics.rect.size.w;
			m_rMetrics.rect.pos.y = m_rMetrics.rect.size.w;
			m_dirty = true;

			cocos2d::extension::CCBAnimationManager* anim_manager = dynamic_cast<cocos2d::extension::CCBAnimationManager*>(m_ccbNode->getUserObject());
			if ( anim_manager && strcmp((*attrs)["play"].c_str(), "auto") == 0 )
			{
				m_sequence = (*attrs)["anim"];
				if ( !m_sequence.empty() )
					anim_manager->runAnimationsForSequenceNamed(m_sequence.c_str());
			}

			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_EleCCBNode::onCompositFinish(class FKCW_RichHtml_IRichCompositor* compositor) 
{
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_EleCCBNode::onRenderPost(FKCW_RichHtml_Canvas canvas)
{
	if ( m_dirty )
	{
		FKCW_RichHtml_Pos pos = getGlobalPosition();
		m_ccbNode->setPosition(ccp(pos.x, pos.y - m_rMetrics.rect.size.h /*+ canvas.root->getActualSize().h*/));
		canvas.root->addCCNode(m_ccbNode);
		m_dirty = false;
	}

	FKCW_RichHtml_EleBase::onRenderPost(canvas);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleCCBNode::FKCW_RichHtml_EleCCBNode()
	: m_ccbNode(NULL), m_dirty(false)
{

}
//-------------------------------------------------------------------------
FKCW_RichHtml_EleCCBNode::~FKCW_RichHtml_EleCCBNode()
{
	CC_SAFE_RELEASE(m_ccbNode);
}
//-------------------------------------------------------------------------