//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_TileSetInfo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_TileSetInfo : public CCObject
{
protected:
	FKCW_TMX_TileSetInfo();
	
public:
	virtual ~FKCW_TMX_TileSetInfo();
	static FKCW_TMX_TileSetInfo* create();
	
	// 获取TileSet的图片大小
	// 参数：TileSet起始的GridID
	CCRect getRect(int gid);										
	
	CC_SYNTHESIZE(string, m_name, Name);							// TileSet名字
	CC_SYNTHESIZE(int, m_firstGid, FirstGid);						// TileSet起始的GridID，从0开始叠加
	CC_SYNTHESIZE(float, m_tileWidth, TileWidth);					// Tile宽度（像素）	
	CC_SYNTHESIZE(float, m_tileHeight, TileHeight);					// Tile高度（像素）
	CC_SYNTHESIZE(float, m_spacing, Spacing);						// TileSet图片中的两个 Tile 之间的间隔大小（像素）
	CC_SYNTHESIZE(float, m_margin, Margin);							// TileSet图片边缘大小（像素）
	CC_SYNTHESIZE(string, m_sourceImagePath, SourceImagePath);		// 源文件路径（是绝对路径还是相对路径取决于tmx文件的路径类型）
	CC_SYNTHESIZE(float, m_imageWidth, ImageWidth);					// 纹理宽度
	CC_SYNTHESIZE(float, m_imageHeight, ImageHeight);				// 纹理高度（像素）
	CC_SYNTHESIZE(CCTexture2D*, m_texture, Texture);				// 纹理
};