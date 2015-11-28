//*************************************************************************
//	创建日期:	2015-1-17
//	文件名称:	IRenderText.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
// 文字对齐方式
enum ENUM_TextHorizontalAlignment
{
	eTHA_Left,				// 左对齐
	eTHA_Center,			// 中心对齐
	eTHA_Right,				// 右对齐
};
enum ENUM_TextVerticalAlignment
{
	eTVA_Top,				// 顶端对齐
	eTVA_Center,			// 中心对齐
	eTVA_Bottom,			// 底部对齐
};
//-------------------------------------------------------------------------
class IRenderText
{
public:
	// 释放
	virtual void		Release() = 0;
	// 绑定到场景中
	virtual bool		Attach( CCNode* pParentNode, int nZOrder, int nTag = 0 ) = 0;
	// 从场景中解除
	virtual void		Detach() = 0;
	// 设置位置
	virtual void		SetPos( const CCPoint& tagPos ) = 0;
	// 设置字符串
	virtual void		SetString( const char* szText ) = 0;
	// 设置颜色
	virtual void		SetColor( const ccColor3B& color ) = 0;
	// 设置透明度
	virtual	void		SetOpacity( unsigned char byAlpha ) = 0;
	// 设置字体格式
	virtual void		SetFont( const char* szFont ) = 0;
	// 设置显示区域
	virtual void		SetDeimensions( const CCSize& tagDeimensions ) = 0;
	// 设置文字水平对齐方式
	virtual void		SetHorizontalAlignment( ENUM_TextHorizontalAlignment eType ) = 0;
	// 设置文字垂直对齐方式
	virtual void		SetVerticalAlignment( ENUM_TextVerticalAlignment eType ) = 0;
};