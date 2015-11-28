//-------------------------------------------------------------------------
#include "../Include/FKCW_Font_Utility.h"
#include "../Include/FKCW_Font_Config.h"
#include "../Include/FKCW_Font_Manager.h"
#include <fstream>
//-------------------------------------------------------------------------
#if defined(_MSC_VER)
	#define NO_WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(__GNUC__)

#else
	#error  "FKCW_Font do not support this os!"
#endif
//-------------------------------------------------------------------------

const char*		FKCW_font_default_fontpath = NULL;
const char*		FKCW_font_default_fontfile = NULL;
int				FKCW_font_default_ppi = 96;
int				FKCW_font_default_size = 18;
unsigned int	FKCW_font_default_color = 0xffffffff;

//-------------------------------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//-------------------------------------------------------------------------
	char default_fontpath_buf[256] = {0};
	const char* get_systemfont_path()
	{
		GetWindowsDirectoryA(default_fontpath_buf, 256);
		sprintf(default_fontpath_buf, "%s/%s", default_fontpath_buf, "/fonts");
		return default_fontpath_buf;
	}

	int get_system_default_ppi()
	{
		return 0;
	}

	int get_prefered_default_fontsize()
	{
		return 18;
	}

	const char* get_system_default_fontfile()
	{
		return "simsun.ttc";
	}

	const char* get_system_fallback_fontfile()
	{
		return "arial.ttf";
	}

	const char* get_system_default_hacklatin_fontfile()
	{
		return "courf.fon";
	}

	int get_system_default_hacklatin_fontshifty()
	{
		return 1;
	}
//-------------------------------------------------------------------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//-------------------------------------------------------------------------
	const char* get_systemfont_path()
	{
		return "/System/Library/Fonts/Cache";
	}

	int get_system_default_ppi()
	{
		return 0;
	}

	int get_prefered_default_fontsize()
	{
		return 18;
	}

	const char* get_system_default_fontfile()
	{
		return "STHeiti Light.ttc";
	}

	const char* get_system_fallback_fontfile()
	{
		return "_H_Helvetica.ttf";
	}

	const char* get_system_default_hacklatin_fontfile()
	{
		return NULL;
	}

	int get_system_default_hacklatin_fontshifty()
	{
		return 0;
	}
//-------------------------------------------------------------------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//-------------------------------------------------------------------------
	const char* get_systemfont_path()
	{
		return "/System/Library/Fonts";
	}

	int get_system_default_ppi()
	{
		return 0;
	}

	int get_prefered_default_fontsize()
	{
		return 18;
	}

	const char* get_system_default_fontfile()
	{
		return "STHeiti Light.ttc";
	}

	const char* get_system_fallback_fontfile()
	{
		return "Helvetica.dfont";
	}

	const char* get_system_default_hacklatin_fontfile()
	{
		return NULL;
	}

	int get_system_default_hacklatin_fontshifty()
	{
		return 0;
	}
//-------------------------------------------------------------------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//-------------------------------------------------------------------------
	const char* get_systemfont_path()
	{
		return "/system/fonts";
	}

	int get_system_default_ppi()
	{
		return 0;
	}

	int get_prefered_default_fontsize()
	{
		return 18;
	}

	const char* get_system_default_fontfile()
	{
		return "DroidSansFallback.ttf";
	}

	const char* get_system_fallback_fontfile()
	{
		return "DroidSansFallback.ttf";
	}

	const char* get_system_default_hacklatin_fontfile()
	{
		return NULL;
	}

	int get_system_default_hacklatin_fontshifty()
	{
		return 0;
	}
//-------------------------------------------------------------------------
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//-------------------------------------------------------------------------
	const char* get_systemfont_path()
	{
		return "/usr/share/fonts";
	}

	int get_system_default_ppi()
	{
		return 0;
	}

	int get_prefered_default_fontsize()
	{
		return 18;
	}

	const char* get_system_default_fontfile()
	{
		return "truetype/wqy/wqy-microhei.ttc";
	}

	const char* get_system_fallback_fontfile()
	{
		return "truetype/freefont/FreeSerif.ttf";
	}

	const char* get_system_default_hacklatin_fontfile()
	{
		return NULL;
	}

	int get_system_default_hacklatin_fontshifty()
	{
		return 0;
	}

//-------------------------------------------------------------------------
#else
#error  "FKCW_Font do not support this os!"
#endif
//-------------------------------------------------------------------------
// 函数集
//-------------------------------------------------------------------------
void FKCW_font_default_initialize()
{
	FKCW_font_default_fontpath	= get_systemfont_path();
	FKCW_font_default_ppi		= get_system_default_ppi();
	FKCW_font_default_fontfile	= get_system_default_fontfile();
	FKCW_font_default_size		= get_prefered_default_fontsize();

	CCAssert(FKCW_font_default_fontpath, "");
	CCAssert(FKCW_font_default_fontfile, "");
	CCFileUtils::sharedFileUtils()->addSearchPath(FKCW_font_default_fontpath);

	// 添加默认字体
	FKCW_Font_Catalog* font_catalog = FKCW_Font_Factory::GetInstance()->CreateFont(
		FKCW_Font_Default_FontAlias, 
		FKCW_font_default_fontfile, 
		FKCW_font_default_color, 
		FKCW_font_default_size, 
		e_plain);

	// 回滚
	if ( !font_catalog )
	{
		FKCW_font_default_fontfile	= get_system_fallback_fontfile();
		font_catalog = FKCW_Font_Factory::GetInstance()->CreateFont(
			FKCW_Font_Default_FontAlias, 
			FKCW_font_default_fontfile, 
			FKCW_font_default_color, 
			FKCW_font_default_size, 
			e_plain);
	}
	else
	{
		if ( get_system_default_hacklatin_fontfile() )
		{
			std::string fullpath = CCFileUtils::sharedFileUtils()
				->fullPathForFilename(get_system_default_hacklatin_fontfile());

			/*FontInfo* hackfont =*/ font_catalog->AddHackFont(
				fullpath.c_str(), latin_charset(), 
				get_system_default_hacklatin_fontshifty());
		}
	}
}
//-------------------------------------------------------------------------
std::set<unsigned long>* latin_charset()
{
	static std::set<unsigned long> latinset;
	for ( unsigned long i = 0; i < 256; i++ )
	{
		latinset.insert(i);
	}
	return &latinset;
}
//-------------------------------------------------------------------------
// DUMP到png纹理中
// TGA头结构
#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, 1)
#pragma pack(1)
#endif

struct TGAHeader
{
	uint8	idLength;           // Length of optional identification sequence.
	uint8	paletteType;        // Is a palette present? (1=yes)
	uint8	imageType;          // Image data type (0=none, 1=indexed, 2=rgb, 3=grey, +8=rle packed).
	uint16	firstPaletteEntry;  // First palette index, if present.
	uint16	numPaletteEntries;  // Number of palette entries, if present.
	uint8	paletteBits;        // Number of bits per palette entry.
	uint16	x;					// Horiz. pixel coord. of lower left of image.
	uint16	y;                  // Vert. pixel coord. of lower left of image.
	uint16	width;              // Image width in pixels.
	uint16	height;             // Image height in pixels.
	uint8	depth;              // Image color depth (bits per pixel).
	uint8	descriptor;         // Image attribute flags.
};

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop)
#endif


bool dump2tga(const std::string &filename,
	const unsigned int *pxl,
	uint16 width,
	uint16 height)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	if (file)
	{
		TGAHeader header;
		memset(&header, 0, sizeof(TGAHeader));
		header.imageType  = 2;
		header.width = width;
		header.height = height;
		header.depth = 32;
		header.descriptor = 0x20;

		file.write((const char *)&header, sizeof(TGAHeader));
		file.write((const char *)pxl, sizeof(unsigned int) * width * height);

		return true;
	}
	return false;
}
//-------------------------------------------------------------------------