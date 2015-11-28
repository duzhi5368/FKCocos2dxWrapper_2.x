//*************************************************************************
//	创建日期:	2014-8-18   14:48
//	文件名称:	BaseFactory.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "BaseMath.h"
#include <map>
#include <string>
using std::string;
using std::map;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CSkeletonData;
class ITextureAtlas;
class CTextureAtlasData;
class CSkeletonData;
class CArmature;
class IObject;
class CSlot;
class CBaseFactory
{
private:
	map<string, CSkeletonData*>			m_mapDataDic;
	map<string, ITextureAtlas*>			m_mapTextureAtlasDic;
	map<string, CTextureAtlasData*>		m_mapTextureAtlasLoadingDic;
public:
	CBaseFactory();
public:
	bool			IsExistSkeletonDataInDic( const string& p_szName );
	bool			IsExistTextureDataInDic( const string& p_szName );
	CSkeletonData*	GetSkeletonData( const string& p_szName );
	void			AddSkeletonData( CSkeletonData* p_szData, const string& p_szName = "" );
	void			RemoveSkeletonData( const string& p_szName );
	ITextureAtlas*	GetTextureAtlas( const string& p_szName );
	void			AddTextureAtlas( ITextureAtlas* p_pTextureAtlas, const string& p_szName = "" );
	void			RemoveTextureAtlas( const string& p_szName );
	void			Clear( bool p_bIsClearData = true );
public:
	// 创建一个骨架
	CArmature*		BuildArmature( const string& p_szArmatureName, const string& p_szAnimationName = "",  const string& p_szPicName = "",
		const string& p_szSkeletonName = "", const string& p_szTextureAltasName = "", const string& p_szSkinName = "" );
	IObject*		GetTextureDisplay( const string& p_szTextureName, const string& p_szTextureAtlasName = "",
		f32 p_fPivotX = NaN, f32 p_fPivotY = NaN );
public:
	virtual ITextureAtlas*	GenerateTextureAtlas( IObject* p_pContent, CTextureAtlasData* p_pTextureAtlasData ) = 0;
	virtual CArmature*		GenerateArmature( const string& p_szPicName ) = 0;
	virtual CSlot*			GenerateSlot() = 0;
	virtual IObject*		GenerateDisplay( ITextureAtlas* p_pTextureAtlas, const string& p_szFullName, f32 p_fPivotX, f32 p_fPivotY ) = 0;
	virtual void			LoadSkeletonFile( const string& p_szSkeletonFile, const string& p_szName = "" ) = 0;
	virtual void			LoadTextureAtlasFile( const string& p_szTextureAtlasFile, const string& p_szName = "") = 0;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------