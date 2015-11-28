//*************************************************************************
//	创建日期:	2014-12-30
//	文件名称:	FKCW_Font_Manager.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Font_Config.h"
//-------------------------------------------------------------------------
typedef unsigned long utf32;
typedef unsigned short utf16;
typedef unsigned char utf8;

const utf32 c_char_invalid = '*';
const utf32 c_char_blank = ' ';
//-------------------------------------------------------------------------
enum EFontStyle
{
	e_plain,
	e_strengthen,
	e_border,
	e_shadow
};
//-------------------------------------------------------------------------
struct SPaddingRect
{
	int origin_x;
	int origin_y;
	int width;
	int height;
};
//-------------------------------------------------------------------------
struct SGlyphMetrics
{
	int left;
	int top;
	int width;
	int height;
	int advance_x;
	int advance_y;
};
//-------------------------------------------------------------------------
struct SGlyphSlot
{
	utf32 charcode;
	size_t ref_count;
	SPaddingRect padding_rect;
	SGlyphMetrics metrics;
	class FKCW_Font_Texture2D* texture;

	class IBitmap* bitmap;

	void retain();
	void release();
};
//-------------------------------------------------------------------------
class FKCW_Font_Texture2D
{
	friend struct SGlyphSlot;
public:
	FKCW_Font_Texture2D(class FKCW_Font_Info* f, int width, int height, int padding_width, int padding_height);
	~FKCW_Font_Texture2D();

	int width();
	int height();

	// 刷新数据到GPU
	void flush();
	bool has_empty_slot();

	SGlyphSlot* cache_charcode(utf32 charcode);
	unsigned char* buffer_data();

	// T == cocos2d::CCTexture2D
	template<typename T>
	T* user_texture()
	{
		return (T*)m_user_texture;
	}

	void dump_textures(const char* prefix, int index);

private:
	void _init_slot(int i);
	void _slot_nouse(SGlyphSlot* slot);
	void _slot_inuse(SGlyphSlot* slot);
	void _dump2texture(class IBitmap* bitmap, const SPaddingRect& rect, bool draw_cbox = false);

private:
	class FKCW_Font_Info*			m_font;
	SGlyphSlot*				m_slots;
	size_t					m_slot_num;
	int						m_width;
	int						m_height;
	int						m_cols;
	int						m_rows;
	int						m_padding_width;
	int						m_padding_height;
	unsigned char*			m_data;
	void*					m_user_texture;
	std::vector<SGlyphSlot*> m_dirty_slots;		// 需要被释放的位图
	std::set<SGlyphSlot*>	m_emptyslots;
};
//-------------------------------------------------------------------------
class FKCW_Font_Catalog
{
public:
	typedef std::map<utf32,  SGlyphSlot*> glyph_map_t;
	typedef std::map<SGlyphSlot*, utf32> reverse_glyph_map_t;

	void						require_text(utf16* text, size_t len, std::vector<SGlyphSlot*>* glyph_slots);
	void						require_text(utf32* text, size_t len, std::vector<SGlyphSlot*>* glyph_slots);
	SGlyphSlot*					require_char(utf32 charcode);
	std::vector<FKCW_Font_Texture2D*>*	textures();
	void						flush();
	unsigned int				char_width();
	unsigned int				char_height();
	bool						AddHackFont(const char* fontname, std::set<unsigned long>* charset,
		unsigned int shift_y = 0);
	bool						AddHackFont(const char* fontname, long face_idx,
		std::set<unsigned long>* charset, unsigned int shift_y);
	void						dump_textures(const char* prefix);

	FKCW_Font_Catalog(class FKCW_Font_Info* f, int texture_width, int texture_height, int max_textures=2);
	~FKCW_Font_Catalog();
private:
	void _add_to_map(SGlyphSlot* slot);
	void _remove_from_map(SGlyphSlot* slot);
private:
	class FKCW_Font_Info*				m_font;
	std::vector<FKCW_Font_Texture2D*>	m_textures;
	glyph_map_t					m_glyphmap;
	reverse_glyph_map_t			m_reverse_glyphmap;
	int							m_max_textures;
	int							m_texture_width;
	int							m_texture_height;
	int							m_padding_width;
	int							m_padding_height;
	utf32						m_previous_char_idx;
};
//-------------------------------------------------------------------------
class FKCW_Font_Factory
{
public:
	typedef void (*initor_t)();
public:
	static void				register_initor(initor_t initor);
	static FKCW_Font_Factory*		GetInstance();

	FKCW_Font_Catalog* find_font(const char* alias, bool no_fail = true);
	FKCW_Font_Catalog* CreateFont(
		const char* alias, const char* font_name, unsigned int color, int size_pt,
		EFontStyle style=e_plain, float strength=1.0f, unsigned int secondary_color=0xff000000, 
		int faceidx=0, int ppi=FKCW_Font_Default_FontPPI
		);
	FKCW_Font_Catalog* another_alias(const char* another_alias, const char* origin_alias);
	void		DumpTextures();

private:
	FKCW_Font_Factory();
	~FKCW_Font_Factory(); 
private:
	std::map<std::string, FKCW_Font_Catalog*> m_fonts;
};
//-------------------------------------------------------------------------