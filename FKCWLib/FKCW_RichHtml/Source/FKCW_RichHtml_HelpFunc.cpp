//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_HelpFunc.h"
//-------------------------------------------------------------------------
int FKCW_RichHtml_TransferOCTValue(unsigned short c)
{
	if ( c >= '0' && c <= '9' )
	{
		return c - '0';
	}

	return -1;
}
//-------------------------------------------------------------------------
int FKCW_RichHtml_TransferHEXValue(unsigned short c)
{
	if ( c >= '0' && c <= '9' )
	{
		return c - '0';
	}
	else if ( c >= 'A' && c <= 'F' )
	{
		return c - 'A' + 10;
	}
	else if ( c >= 'a' && c <= 'f' )
	{
		return c - 'a' + 10;
	}

	return -1;
}
//-------------------------------------------------------------------------
int FKCW_RichHtml_TransferInteger(unsigned short* start, unsigned short* end, int& integer)
{
	int consumed = 0;
	int temp = 0;
	bool minus = false;

	// 若找到尾部
	if ( start + consumed == end )
	{
		return 0;
	}

	// 是否是负号
	if ( start[consumed] == '-' )
	{
		minus = true;
		consumed++;

		// 若找到尾部
		if ( start + consumed == end )
		{
			return 0;
		}
	}

	// HEX值
	if ( start + consumed + 2 < end && start[consumed] == '0' && start[consumed+1] == 'x' )
	{
		consumed += 2;
		int hexv = 0;
		int cnt = 0;
		for ( cnt = 0; cnt < 8; cnt++ )
		{
			if ( start + consumed == end )
			{
				break;
			}
			hexv = FKCW_RichHtml_TransferHEXValue(start[consumed]);
			if ( hexv == -1 )
			{
				break;
			}
			temp = (temp << 4) + hexv;
			consumed++;
		}
		if ( cnt == 0 )
		{
			return 0;
		}

	}
	// OCT值
	else
	{
		int octv = 0;
		int cnt = 0;
		for ( cnt = 0; cnt < 10; cnt++ )
		{
			if ( start + consumed == end )
			{
				break;
			}
			octv = FKCW_RichHtml_TransferOCTValue(start[consumed]);
			if ( octv == -1 )
			{
				break;
			}
			temp = (temp * 10) + octv;
			consumed++;
		}
		if ( cnt == 0 )
		{
			return 0;
		}
	}

	if ( minus )
	{
		temp = -temp;
	}

	integer = temp;

	return consumed;
}
//-------------------------------------------------------------------------
int FKCW_RichHtml_TransferAngleBrackets(unsigned short* start, unsigned short* end, std::string& content)
{
	if ( start >= end || start[0] != '[' )
	{
		return 0;
	}

	int consumed = 1;
	int content_size = 0;

	while ( true )
	{
		if ( start + consumed == end )
		{
			return 0;
		}
		if ( start[consumed] == ']')
		{
			consumed++;
			break;
		}
		consumed++;
		content_size++;
	}

	content.assign(start + 1, start + 1 + content_size);

	return consumed;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_TransferRect(std::string& str, FKCW_RichHtml_Rect& rect)
{
	int pos = 0;
	// 左
	pos = str.find_first_of(',');
	if ( pos == std::string::npos )
	{
		return false;
	}
	std::string temp = str.substr(0, pos);
	int x = atoi(temp.c_str());

	// 上
	str = str.substr(pos + 1, str.size());
	pos = str.find_first_of(',');
	if ( pos == std::string::npos )
	{
		return false;
	}
	temp = str.substr(0, pos);
	int y = atoi(temp.c_str());

	// 宽
	str = str.substr(pos + 1, str.size());
	pos = str.find_first_of(',');
	if ( pos == std::string::npos )
	{
		return false;
	}
	temp = str.substr(0, pos);
	int width = atoi(temp.c_str());

	// 高
	str = str.substr(pos + 1, str.size());
	pos = str.find_first_of("|");
	int height = 0;
	if ( pos == std::string::npos )
	{
		height = atoi(str.c_str());
	}
	else
	{
		temp = str.substr(0, pos);
		height = atoi(temp.c_str());
		str = str.substr(pos + 1, str.size());
	}

	rect.pos.x = (short)x;
	rect.pos.y = (short)y;
	rect.size.w = (short)width;
	rect.size.h = (short)height;

	return true;
}
//-------------------------------------------------------------------------
bool FKCW_RichHtml_TransferImage(const std::string& content, std::string& filename, FKCW_RichHtml_Rect& tex_rect, FKCW_RichHtml_Rect& composit_rect)
{
	std::string left = content;

	size_t pos = left.find_first_of('|');

	// 没有rect信息
	if ( pos == std::string::npos )
	{
		filename = content;
		return filename.size() > 0;
	}

	filename = left.substr(0, pos);
	left = left.substr(pos + 1, left.size());

	if ( !FKCW_RichHtml_TransferRect(left, tex_rect) )
	{
		return false;
	}

	if ( !FKCW_RichHtml_TransferRect(left, composit_rect) )
	{
		return true;
	}

	return true;
}
//-------------------------------------------------------------------------