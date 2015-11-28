//*************************************************************************
//	创建日期:	2015-1-19
//	文件名称:	FKRenderTextMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKRenderText.h"
//-------------------------------------------------------------------------
class FKRenderTextMgr
{
public:
	FKRenderTextMgr();
	~FKRenderTextMgr();
public:
	// 创建渲染文字
	// 参数：szString 文字
	// 参数：szFontName 字体名称
	// 参数：nFontSize 字体大小
	// 参数：tagDimentions 字体尺寸
	// 参数：bIsBmpFont 是否是BMP文字
	IRenderText*	CreateRenderText( const char* szString, const char* szFontName, 
		int nFontSize,  CCSize tagDimentions, bool bIsBMPFont = false );
	// 删除渲染文字
	// 参数：pRenderText 需要删除的渲染文字
	bool			RemoveRenderText( IRenderText* pRenderText );
	// 删除渲染文字
	// 参数：id 唯一ID
	bool			RemoveRenderTextByID( RENDER_OBJ_ID id );
private:
	typedef map< RENDER_OBJ_ID, IRenderText* >		RenderTextMap;
	RenderTextMap					m_mapRenderText;
};