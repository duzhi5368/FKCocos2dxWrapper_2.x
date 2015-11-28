//-------------------------------------------------------------------------
#include "../Include/FKCW_Font_Manager.h"
#include "../Include/FKCW_Font_Utility.h"
#include "../Include/FKCW_Font_Render.h"
#include <fstream>
//-------------------------------------------------------------------------
static FT_Library s_ft_library = NULL;
//-------------------------------------------------------------------------
void SGlyphSlot::retain()
{
	++ref_count;
	if ( ref_count == 1 )
	{
		texture->_slot_inuse(this);
	}
}
//-------------------------------------------------------------------------
void SGlyphSlot::release()
{
	--ref_count;
	if ( ref_count == 0 )
	{
		texture->_slot_nouse(this);
	}
}
//-------------------------------------------------------------------------
FKCW_Font_Texture2D::FKCW_Font_Texture2D(FKCW_Font_Info* f, int width, int height, int padding_width, int padding_height)
	: m_font(f), m_width(width), m_height(height), 
	m_padding_width(padding_width), m_padding_height(padding_height),
	m_data(NULL), m_user_texture(NULL)
{
	m_cols = m_width/m_padding_width;
	m_rows = m_height/m_padding_height;
	m_slot_num = m_cols * m_rows;
	m_slots = new SGlyphSlot[m_slot_num];

	for ( size_t i = 0; i < m_slot_num; i++ )
	{
		_init_slot(i);
	}

	m_data = new unsigned char[width * height * 4];
	memset(m_data, 0, width * height * 4);

	for( int i = 0; i < width * height * 4; i+=4 )
	{
		int row = (i/4) / width;
		int col = (i/4) % width;
		if (col % m_padding_width == 0 || row % m_padding_height == 0)
		{
			//int* p = &m_data[i];
			*(int*)(&m_data[i]) = 0xffff0000;
		}
		else
		{
			*(int*)(&m_data[i]) = 0x80008000;
		}
	}

	CCTexture2D* tex = new CCTexture2D;
	float fWidth = static_cast<float>(m_width);
	float fHight = static_cast<float>(m_height);
	tex->initWithData(m_data, kCCTexture2DPixelFormat_RGBA8888, m_width, m_height, CCSize(fWidth, fHight));

	if ( m_font->is_bitmap() )
	{
		tex->setAliasTexParameters();
	}
	else
	{
		tex->setAntiAliasTexParameters();
	}

	m_user_texture = tex;
}
//-------------------------------------------------------------------------
FKCW_Font_Texture2D::~FKCW_Font_Texture2D()
{
	m_dirty_slots.clear();
	if ( m_user_texture )
	{
		user_texture<CCTexture2D>()->release();
		m_user_texture = NULL;
	}
	delete[] m_slots;
	delete[] m_data;
}
//-------------------------------------------------------------------------
int FKCW_Font_Texture2D::width()
{
	return m_width;
}
int FKCW_Font_Texture2D::height()
{
	return m_height;
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::flush()
{
	//box@hulijun.cn: fix skewed bug
	//http://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
	//GL_UNPACK_ALIGNMENT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	ccGLBindTexture2D(user_texture<CCTexture2D>()->getName());

	for ( size_t i = 0; i < m_dirty_slots.size(); i++ )
	{
		SGlyphSlot* slot = m_dirty_slots[i];		

		glTexSubImage2D(GL_TEXTURE_2D, 0, 
			slot->padding_rect.origin_x, slot->padding_rect.origin_y,
			slot->bitmap->real_width(), slot->bitmap->real_height(),
			GL_RGBA, GL_UNSIGNED_BYTE, slot->bitmap->get_buffer()
			);

		slot->bitmap->release();
		slot->bitmap = NULL;
	}

	m_dirty_slots.clear();
}
//-------------------------------------------------------------------------
bool FKCW_Font_Texture2D::has_empty_slot()
{
	return m_emptyslots.empty() == false;
}
//-------------------------------------------------------------------------
SGlyphSlot* FKCW_Font_Texture2D::cache_charcode(utf32 charcode)
{
	SGlyphSlot* slot = NULL;

	if ( m_emptyslots.empty() )
	{
		return NULL;
	}
	slot = *m_emptyslots.begin();

	if ( slot )
	{
		GlyphBitmap bm;
		if ( m_font->render_charcode(charcode, &bm) )
		{
			slot->charcode = charcode;
			slot->metrics.left = bm.top_left_pixels.x;
			slot->metrics.top = bm.top_left_pixels.y;
			slot->metrics.width = bm.bitmap->width();
			slot->metrics.height = bm.bitmap->height();
			slot->metrics.advance_x = bm.advance_pixels.x;
			slot->metrics.advance_y = bm.advance_pixels.y;
			slot->bitmap = bm.bitmap;
			slot->padding_rect.width = bm.bitmap->real_width();
			slot->padding_rect.height = bm.bitmap->real_height();

			_dump2texture(bm.bitmap, slot->padding_rect, false);

			// 更新纹理后位图将被释放
			m_dirty_slots.push_back(slot);
		}
		else
		{
			// 无法被识别的字符
			slot = NULL;
		}
	}

	return slot;
}
//-------------------------------------------------------------------------
unsigned char* FKCW_Font_Texture2D::buffer_data()
{
	return m_data;
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::dump_textures(const char* prefix, int index)
{
	char path_buffer[256];
	sprintf(path_buffer, "%sFKCWFont_%s_%d.tga", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), prefix, index);

	if ( m_data )
	{
		dump2tga(path_buffer, (unsigned int*)m_data, this->width(), this->height());
	}
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::_init_slot(int i)
{
	SGlyphSlot& slot = m_slots[i];
	slot.charcode = 0;
	slot.ref_count = 0;
	slot.texture = this;
	slot.padding_rect.width = m_padding_width;
	slot.padding_rect.height = m_padding_height;
	slot.padding_rect.origin_x = i % m_cols * m_padding_width;
	slot.padding_rect.origin_y = i / m_cols * m_padding_height;

	m_emptyslots.insert(&slot);
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::_slot_nouse(SGlyphSlot* slot)
{
	m_emptyslots.insert(slot);
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::_slot_inuse(SGlyphSlot* slot)
{
	std::set<SGlyphSlot*>::iterator it = m_emptyslots.find(slot);
	if(it != m_emptyslots.end())
	{
		m_emptyslots.erase(it);
	}
}
//-------------------------------------------------------------------------
void FKCW_Font_Texture2D::_dump2texture(IBitmap* bitmap, const SPaddingRect& rect, bool draw_cbox/* = false*/)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = rect.origin_x + bitmap->width();
	FT_Int  y_max = rect.origin_y + bitmap->height();

	for ( i = rect.origin_x, p = 0; i < x_max; i++, p++ )
	{
		for ( j = rect.origin_y, q = 0; j < y_max; j++, q++ )
		{
			if ( i < 0 || j < 0 || i >= m_width || j >= m_height )
				continue;

			int pos = j * m_width * 4 + i * 4;
			ColorRGBA src = bitmap->get_unit_at(p, q);
			ColorRGBA bgc(m_data[pos + 0], m_data[pos + 1 ], m_data[pos + 2 ], m_data[pos + 3 ]);
			ReplaceBlender blender;
			ColorRGBA color = blender.blend(src, bgc);
			//if ( color.a )
			{
				m_data[pos + 0 ] = color.r;
				m_data[pos + 1 ] = color.g;
				m_data[pos + 2 ] = color.b;
				m_data[pos + 3 ] = color.a;
			}

			if (draw_cbox)
			{
				if ( p == 0 || q == 0 || p == bitmap->width() - 1 || q == bitmap->height() - 1 )
				{
					m_data[pos + 0 ] = 0x00;
					m_data[pos + 1 ] = 0xff;
					m_data[pos + 2 ] = 0x00;
					m_data[pos + 3 ] = 0xff;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::require_text(utf16* text, size_t len, std::vector<SGlyphSlot*>* glyph_slots)
{
	for ( size_t i = 0; i < len; i++ )
	{
		SGlyphSlot* slot = NULL;

		if ( cocos2d::isspace_unicode(text[i]) )
		{
			slot = require_char(c_char_blank);
		}
		else
		{
			slot = require_char(text[i]);
		}

		if ( !slot )
		{
			slot = require_char(c_char_invalid);
		}

		if ( slot )
		{
			glyph_slots->push_back(slot);
		}
	}
	this->flush();
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::require_text(utf32* text, size_t len, std::vector<SGlyphSlot*>* glyph_slots)
{
	for ( size_t i = 0; i < len; i++ )
	{
		SGlyphSlot* slot = NULL;

		if ( cocos2d::isspace_unicode((utf16)text[i]) )
		{
			slot = require_char(c_char_blank);
		}
		else
		{
			slot = require_char(text[i]);
		}

		if ( !slot )
		{
			slot = require_char(c_char_invalid);
		}

		if ( slot )
		{
			glyph_slots->push_back(slot);
		}
	}
	this->flush();
}
//-------------------------------------------------------------------------
SGlyphSlot* FKCW_Font_Catalog::require_char(utf32 charcode)
{
	SGlyphSlot* slot = NULL;
	bool invalid_char = false;

	// 检查该字符是否已在占用格中
	glyph_map_t::iterator it = m_glyphmap.find(charcode);
	if ( it != m_glyphmap.end() )
	{
		slot = it->second;
	}
	else
	{
		// 创建新字符
		for ( size_t i = 0; i < m_textures.size(); i++ )
		{
			if ( m_textures[i]->has_empty_slot() )
			{
				slot = m_textures[i]->cache_charcode(charcode);
				if ( !slot )
				{
					invalid_char = true;
				}
				break;
			}
		}

		if ( !invalid_char && !slot )
		{
			// 字符格不足，创建新纹理
			if (!slot && (int)m_textures.size() < m_max_textures)
			{
				FKCW_Font_Texture2D* newtex = new FKCW_Font_Texture2D(
					m_font, m_texture_width, m_texture_height, 
					m_padding_width + FKCW_Font_Bitmap_Padding*2, m_padding_height + FKCW_Font_Bitmap_Padding*2);
				m_textures.push_back(newtex);
				slot = newtex->cache_charcode(charcode);

				if ( !slot )
				{
					invalid_char = true;
				}
			}
		}

		// 无效字符
		if ( invalid_char )
		{
			//glyph_map_t::iterator it = m_glyphmap.find(c_char_invalid);
			//if ( it != m_glyphmap.end() )
			//{
			//	slot = it->second;
			//}
		}
		else if ( slot )
		{
			_remove_from_map(slot);
			_add_to_map(slot);
		}
	}

	if ( slot )
	{
		slot->retain();
	}

	return slot;
}
//-------------------------------------------------------------------------
std::vector<FKCW_Font_Texture2D*>* FKCW_Font_Catalog::textures()
{
	return &m_textures;
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::flush()
{
	for ( size_t i = 0; i < m_textures.size(); i++ )
	{
		m_textures[i]->flush();
	}

	m_previous_char_idx = 0;
}
//-------------------------------------------------------------------------
unsigned int FKCW_Font_Catalog::char_width()
{
	return m_font->char_width_pt();
}
//-------------------------------------------------------------------------
unsigned int FKCW_Font_Catalog::char_height()
{
	return m_font->char_height_pt();
}
//-------------------------------------------------------------------------
bool FKCW_Font_Catalog::AddHackFont(const char* fontname, std::set<unsigned long>* charset, unsigned int shift_y /*= 0*/)
{
	return AddHackFont(fontname, 0, charset, shift_y);
}
//-------------------------------------------------------------------------
bool FKCW_Font_Catalog::AddHackFont(const char* fontname, long face_idx, std::set<unsigned long>* charset, unsigned int shift_y)
{
	return m_font->add_hackfont(fontname, face_idx, charset, shift_y) != NULL;
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::dump_textures(const char* prefix)
{
	for ( size_t i = 0; i < m_textures.size(); i++ )
	{
		m_textures[i]->dump_textures(prefix, i);
	}
}
//-------------------------------------------------------------------------
FKCW_Font_Catalog::FKCW_Font_Catalog(FKCW_Font_Info* f, int texture_width, int texture_height, int max_textures/*=2*/)
	: m_font(f), 
	m_max_textures(max_textures),
	m_texture_width(texture_width), 
	m_texture_height(texture_height), 
	m_previous_char_idx(0)
{
	int font_size = (int)(f->char_width_pt() > f->char_height_pt() ? f->char_width_pt() : f->char_height_pt());
	font_size += f->extend_pt();
	m_padding_width = font_size;
	m_padding_height = font_size;
}
//-------------------------------------------------------------------------
FKCW_Font_Catalog::~FKCW_Font_Catalog()
{
	m_glyphmap.clear();
	m_reverse_glyphmap.clear();
	for ( size_t i = 0; i < m_textures.size(); i++ )
	{
		delete m_textures[i];
	}
	m_textures.clear();
	m_font->release();
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::_add_to_map(SGlyphSlot* slot)
{
	m_glyphmap[slot->charcode] = slot;
	m_reverse_glyphmap[slot] = slot->charcode;
}
//-------------------------------------------------------------------------
void FKCW_Font_Catalog::_remove_from_map(SGlyphSlot* slot)
{
	reverse_glyph_map_t::iterator it = m_reverse_glyphmap.find(slot);
	if ( it != m_reverse_glyphmap.end() )
	{
		m_glyphmap.erase(it->second);
		m_reverse_glyphmap.erase(it);
	}
}
//-------------------------------------------------------------------------
// fontFactory
//-------------------------------------------------------------------------
FKCW_Font_Catalog* FKCW_Font_Factory::find_font(const char* alias, bool no_fail /*= true*/)
{
	if ( !alias )
		alias = FKCW_Font_Default_FontAlias;

	std::map<std::string, FKCW_Font_Catalog*>::iterator it = m_fonts.find(alias);
	if ( it != m_fonts.end() )
	{
		return it->second;
	}

	if ( no_fail )
		return m_fonts[FKCW_Font_Default_FontAlias];

	return NULL;
}
//-------------------------------------------------------------------------
FKCW_Font_Catalog* FKCW_Font_Factory::another_alias(const char* another_alias, const char* origin_alias)
{
	FKCW_Font_Catalog* fontc = find_font(origin_alias);

	if ( fontc )
	{
		m_fonts[another_alias] = fontc;
	}

	return fontc;
}
//-------------------------------------------------------------------------
FKCW_Font_Catalog* FKCW_Font_Factory::CreateFont(
	const char* alias, const char* font_name, unsigned int color, int size_pt,
	EFontStyle style/*=e_plain*/, float strength/*=1.0f*/, unsigned int secondary_color/*=0xff000000*/, 
	int faceidx/*=0*/, int ppi/*=DFONT_DEFAULT_FONTPPI*/
	)
{
	if ( !alias )
	{
		return NULL;
	}
	FKCW_Font_Catalog* catalog = find_font(alias, false);
	FKCW_Font_Info* font = NULL;
	if ( catalog )
	{
		// the alias is already in used, return it.
		return catalog;
	}

	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(font_name);

	font = FKCW_Font_Info::create_font(s_ft_library, fullpath.c_str(), faceidx, size_pt, size_pt, ppi);
	if ( !font )
	{
		return find_font(FKCW_Font_Default_FontAlias);
	}

	switch (style)
	{
	case e_plain:
		font->add_pass(RenderPassParam(color, e_replace_blender, 0, 0, false, 0));
		break;
	case e_strengthen:
		font->add_pass(RenderPassParam(color, e_replace_blender, 0, 0, true, (FT_F26Dot6)strength*64));
		break;
	case e_border:
		font->add_pass(RenderPassParam(secondary_color, e_replace_blender, 0, 0, true, (FT_F26Dot6)strength*64))
			->add_pass(RenderPassParam(color, e_additive_blender, 0, 0, false, 0));
		break;
	case e_shadow:
		font->add_pass(RenderPassParam(secondary_color, e_replace_blender, (int)strength, (int)-strength, false, secondary_color))
			->add_pass(RenderPassParam(color, e_additive_blender, 0, 0, false, 0));
		break;
	}

	catalog = new FKCW_Font_Catalog(font, 
		FKCW_Font_Texture_Size_Width, 
		FKCW_Font_Texture_Size_Height, 
		FKCW_Font_Max_Texture_Num_RefFont);

	m_fonts[alias] = catalog;

	return catalog;
}
//-------------------------------------------------------------------------
void FKCW_Font_Factory::DumpTextures()
{
	std::map<std::string, FKCW_Font_Catalog*>::iterator it = m_fonts.begin();
	for ( ; it != m_fonts.end(); it++ )
	{
		it->second->dump_textures(it->first.c_str());
	}
}
//-------------------------------------------------------------------------
static FKCW_Font_Factory::initor_t s_initor = NULL;
//-------------------------------------------------------------------------
void FKCW_Font_Factory::register_initor(initor_t initor)
{
	s_initor = initor;
}
//-------------------------------------------------------------------------
FKCW_Font_Factory* FKCW_Font_Factory::GetInstance()
{
	static FKCW_Font_Factory* _factory = NULL;
	if ( _factory == NULL )
	{
		_factory = new FKCW_Font_Factory;

		if ( s_initor )
		{
			s_initor();
		}
		else
		{
			FKCW_font_default_initialize();
		}
	}
	return _factory;
}
//-------------------------------------------------------------------------
FKCW_Font_Factory::FKCW_Font_Factory()
{
	FT_Error error = FT_Init_FreeType(&s_ft_library);
	CCAssert(error==0, "");
}
//-------------------------------------------------------------------------
FKCW_Font_Factory::~FKCW_Font_Factory()
{
	std::set<FKCW_Font_Catalog*> delset;
	std::map<std::string, FKCW_Font_Catalog*>::iterator it = m_fonts.begin();
	for ( ; it != m_fonts.end(); it++ )
	{
		if ( delset.find(it->second) == delset.end() )
		{
			delete it->second;
			delset.insert(it->second);
		}
	}
	m_fonts.clear();
	FT_Done_FreeType(s_ft_library);
}
//-------------------------------------------------------------------------