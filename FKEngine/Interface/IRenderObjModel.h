//*************************************************************************
//	创建日期:	2015-1-17
//	文件名称:	IRenderObjModel.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
// 一个渲染对象模型对应一个文件
class IRenderObjModel
{
public:
	// 解析一个文件
	virtual bool	LoadFromFKOFile( const string& strFKOFileName ) = 0;
	// 保存本模型到一个文件中
	virtual bool	SaveToFKOFile( const string& strFKOFileName ) = 0;

	// 添加一个组件
	virtual bool	AddPart( const string& strName, const string& strFile, int nDepth ) = 0;
	// 移除一个组件
	virtual void	RemovePart( const string& strName ) = 0;

	// 添加一个挂接对象
	virtual bool	AddLocator( const string& strLocatorName, CCPoint& ptOffset ) = 0;
	// 删除一个挂接对象
	virtual void	RemoveLocator( const string& strLocatorName ) = 0;

	// 添加特效节点
	virtual bool	AddEffect( const string& strEffectName, const string& strLocatorName, 
		const string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth ) = 0;
	// 删除特效节点
	virtual void	RemoveEffect( const string& strEffectName ) = 0;
};