//*************************************************************************
//	创建日期:	2014-8-19   19:22
//	文件名称:	Cocos2dxDBTextureAtlas.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "CommonHead.h"
#include <map>
using std::map;
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class CCocos2dxDBTextureAtlas : public ITextureAtlas
{
protected:
	CTextureAtlasData*			m_pTextureAtlasData;
	map<string, uint>			m_mapTextureNames;
	CCTextureAtlas*				m_pTextureAtlas;
	CCTexture2D*				m_pTexture2D;
public:
	CCocos2dxDBTextureAtlas( CTextureAtlasData* p_pData );
	virtual ~CCocos2dxDBTextureAtlas();
public:
	// TextureAtlas的名字
	virtual const string&	GetName();
	// 获取指定子资源所占面积区域
	virtual CRect			GetRegion( const string& p_szName );
	// 资源清理
	virtual void			Clear();
public:
	uint					GetQuadIndex( const string& p_szName );
	CCTextureAtlas*			GetTextureAtlas();
	CCTexture2D*			GetTexture2D();
};
//-------------------------------------------------------------------------