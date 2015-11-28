//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlAltas.h"
//-------------------------------------------------------------------------
FKCW_RichHtml_RichAtlas* FKCW_RichHtml_RichAtlas::create(class FKCW_RichHtml_IRichNode* container, CCTexture2D* texture, size_t capacity)
{
	FKCW_RichHtml_RichAtlas* atlas = new FKCW_RichHtml_RichAtlas(container);
	if ( atlas && atlas->initWithTexture(texture, capacity) )
	{
		atlas->autorelease();
		return atlas;
	}

	CC_SAFE_DELETE(atlas);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_RichAtlas::initWithTexture(CCTexture2D* texture, size_t capacity)
{
	CCAtlasNode::initWithTexture(texture, 0, 0, capacity);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::appendRichElement(FKCW_RichHtml_IRichElement* element)
{
	m_elements.push_back(element);
	setQuadsToDraw(getQuadsToDraw()+1);
	m_dirty = true;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::resetRichElements()
{
	reset();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::updateRichRenderData()
{
	updateAtlasValues();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::resizeCapacity(size_t ns)
{
	m_pTextureAtlas->resizeCapacity(ns);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::reset()
{
	m_elements.clear();
	setQuadsToDraw(0);
	m_dirty = true;

	m_pTextureAtlas->removeAllQuads();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::updateAtlasValues()
{
	if ( !m_dirty )
	{
		return;
	}
	m_dirty = false;

	if ( m_pTextureAtlas->getCapacity() < getQuadsToDraw() )
	{
		m_pTextureAtlas->resizeCapacity( getQuadsToDraw() );
	}

	ccV3F_C4B_T2F_Quad quad;

	CCTexture2D *texture = m_pTextureAtlas->getTexture();
	float textureWide = (float) texture->getPixelsWide();
	float textureHigh = (float) texture->getPixelsHigh();

	int i = 0;
	for(std::list<FKCW_RichHtml_IRichElement*>::iterator it = m_elements.begin(); 
		it != m_elements.end(); it++, i++) {
			FKCW_RichHtml_IRichElement* ele = *it;
			FKCW_RichHtml_Texture* rtex = ele->getTexture();

#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
			float left   = (rtex->rect.pos.x + 0.5f)/textureWide;
			float right  = left + (rtex->rect.size.w - 1.0f)/textureWide;
			float top    = (rtex->rect.pos.y + 0.5f)/textureHigh;
			float bottom = top + (rtex->rect.size.h - 1.0f)/textureHigh;

			float ele_pos_left = ele->getGlobalPosition().x;
			float ele_pos_top = ele->getGlobalPosition().y;
			float ele_width = ele->scaleToElementSize() ? 
				ele->getMetrics()->rect.size.w : rtex->rect.size.w;
			float ele_height = ele->scaleToElementSize() ? 
				ele->getMetrics()->rect.size.h : rtex->rect.size.h;
#else
			float left   = rtex->rect.pos.x/textureWide;
			float right  = left + rtex->rect.size.w/textureWide;
			float top    = rtex->rect.pos.y/textureHigh;
			float bottom = top + rtex->rect.size.h/textureHigh;

			float ele_pos_left = ele->getGlobalPosition().x;
			float ele_pos_top = ele->getGlobalPosition().y;
			float ele_width = ele->scaleToElementSize() ? 
				ele->getMetrics()->rect.size.w : rtex->rect.size.w;
			float ele_height = ele->scaleToElementSize() ? 
				ele->getMetrics()->rect.size.h : rtex->rect.size.h;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

			quad.tl.texCoords.u = left;
			quad.tl.texCoords.v = top;
			quad.tr.texCoords.u = right;
			quad.tr.texCoords.v = top;
			quad.bl.texCoords.u = left;
			quad.bl.texCoords.v = bottom;
			quad.br.texCoords.u = right;
			quad.br.texCoords.v = bottom;

			quad.bl.vertices.x = (float) (ele_pos_left);
			quad.bl.vertices.y = ele_pos_top - ele_height;
			quad.bl.vertices.z = 0.0f;
			quad.br.vertices.x = (float)(ele_pos_left + ele_width);
			quad.br.vertices.y = ele_pos_top - ele_height;
			quad.br.vertices.z = 0.0f;
			quad.tl.vertices.x = (float)(ele_pos_left);
			quad.tl.vertices.y = ele_pos_top;
			quad.tl.vertices.z = 0.0f;
			quad.tr.vertices.x = (float)(ele_pos_left + ele_width);
			quad.tr.vertices.y = ele_pos_top;
			quad.tr.vertices.z = 0.0f;

			ccColor4B c = { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity };
			quad.tl.colors = c;
			quad.tr.colors = c;
			quad.bl.colors = c;
			quad.br.colors = c;
			m_pTextureAtlas->updateQuad(&quad, i);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_RichAtlas::draw()
{
	if ( m_dirty )
	{
		this->updateRichRenderData();
	}

	CCAtlasNode::draw();

	if( g_bIsFKCWRichHtmlDebug )
	{
		// АќЮЇКа
		const CCSize& s = this->getContentSize();
		CCPoint vertices[4]={
			ccp(0,0),ccp(s.width,0),
			ccp(s.width,s.height),ccp(0,s.height),
		};
		ccDrawColor4B(0x00, 0xff, 0x00, 0xff);
		ccDrawPoly(vertices, 4, true);
	}
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichAtlas::FKCW_RichHtml_RichAtlas(class FKCW_RichHtml_IRichNode* container)
	: m_container(container)
	, m_dirty(true)
{
}
//-------------------------------------------------------------------------
FKCW_RichHtml_RichAtlas::~FKCW_RichHtml_RichAtlas()
{ 
	m_elements.clear();
}
//-------------------------------------------------------------------------