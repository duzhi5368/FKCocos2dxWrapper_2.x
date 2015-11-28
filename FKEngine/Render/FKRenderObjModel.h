//*************************************************************************
//	创建日期:	2015-1-17
//	文件名称:	FKRenderObjModel.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
// 一个RenderObjModel对应一个文件
class FKRenderObjModel : public CCObject
{
public:
	// 对象组件
	struct SObjPart
	{
		string		m_strName;		// 部件名
		string		m_strFile;		// 部件文件名
		int			m_nDepth;		// 部件深度
		SObjPart()
		{
			m_strName.clear();
			m_strFile.clear();
			m_nDepth = 0;
		}
	};
	// 绑定对象
	struct SObjLocator
	{
		string		m_strName;		// 对象名
		CCPoint		m_tagOffset;	// 偏移值
		SObjLocator()
		{
			m_strName.clear();
			m_tagOffset = CCPointZero;
		}
	};
	// 特效对象
	struct SObjEffect
	{
		string		m_strName;		// 特效名
		string		m_strFileName;	// 特效文件名
		string		m_strLocator;	// 特效绑定的节点名
		CCPoint		m_tagOffset;	// 偏移值
		CCPoint		m_tagScale;		// 缩放值
		ccColor4B	m_tagColor;		// 颜色值
		int			m_nDepth;		// 深度
		SObjEffect()
		{
			m_strName.clear();
			m_strFileName.clear();
			m_strLocator.clear();
			m_tagOffset = CCPointZero;
			m_tagScale = ccp( 1.0f, 1.0f );
			m_tagColor = ccc4(255,255,255,255);
			m_nDepth = 0;
		}
	};

	typedef map<string, SObjPart>			ObjPartMap;
	typedef map<string, SObjLocator>		ObjLocatorMap;
	typedef map<string, SObjEffect>			ObjEffectMap;
public:
	FKRenderObjModel();
	~FKRenderObjModel();
private:
	string								m_strObjName;		// 对象名
	int									m_nVersion;			// 文件版本号
public:
	CCPoint								m_pOffset;			// 偏移量
	ObjPartMap							m_mapObjParts;		// 部件列表
	ObjEffectMap						m_mapObjEffects;	// 特效列表
	ObjLocatorMap						m_mapObjLocators;	// 绑定对象列表
public:
	// 解析一个文件
	virtual bool	LoadFromFKOFile( const string& strFKOFilePathName );
	// 保存本模型到一个文件中
	virtual bool	SaveToFKOFile( const string& strFKOFilePathName );

	// 添加一个组件
	virtual bool	AddPart( const string& strName, const string& strFile, int nDepth );
	// 移除一个组件
	virtual void	RemovePart( const string& strName );

	// 添加一个挂接对象
	virtual bool	AddLocator( const string& strLocatorName, CCPoint& ptOffset );
	// 删除一个挂接对象
	virtual void	RemoveLocator( const string& strLocatorName );

	// 添加特效节点
	virtual bool	AddEffect( const string& strEffectName, const string& strLocatorName, 
		const string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth );
	// 删除特效节点
	virtual void	RemoveEffect( const string& strEffectName );

private:
	// 保存10001版本的文件
	virtual bool	__SaveToFKOFile_10001( const string& strFKOFilePathName );
};