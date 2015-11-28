//*************************************************************************
//	创建日期:	2015-1-18
//	文件名称:	FKRenderText.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKRenderText : public IRenderText
{
public:
	FKRenderText( RENDER_OBJ_ID unID );
	~FKRenderText();
public:
	// 创建文字
	bool				Create( const char* szString, const char* szFontName, 
		int nFontSize,  CCSize tagDimentions );
	// 创建BMP字体
	bool				CreateByBMP( const char* szString, const char* szFontName, 
		int nFontSize,  CCSize tagDimentions );
public:
	// 获取ID
	virtual RENDER_OBJ_ID	GetID();
	// 释放
	virtual void		Release();
	// 绑定到场景中
	virtual bool		Attach( CCNode* pParentNode, int nZOrder, int nTag = 0 );
	// 从场景中解除
	virtual void		Detach();
	// 设置位置
	virtual void		SetPos( const CCPoint& tagPos );
	// 设置字符串
	virtual void		SetString( const char* szText );
	// 设置颜色
	virtual void		SetColor( const ccColor3B& color );
	// 设置透明度
	virtual	void		SetOpacity( unsigned char byAlpha );
	// 设置字体格式
	virtual void		SetFont( const char* szFont );
	// 设置显示区域
	virtual void		SetDeimensions( const CCSize& tagDeimensions );
	// 设置文字水平对齐方式
	virtual void		SetHorizontalAlignment( ENUM_TextHorizontalAlignment eType );
	// 设置文字垂直对齐方式
	virtual void		SetVerticalAlignment( ENUM_TextVerticalAlignment eType );
private:
	RENDER_OBJ_ID		m_unID;			// 对象ID
	CCLabelTTF*			m_pLabelTTF;	// 字体对象
	CCLabelBMFont*		m_pLabelBMP;	// 字体对象
	string				m_strContent;	// 文字内容
	CCPoint				m_tagPos;		// 相对于父节点的位置
	CCNode*				m_pParent;		// 父节点
	CCSize				m_tagDimentions;// 显示区域
	ccColor3B			m_tagColor;		// 显示颜色
	unsigned char		m_byAlpha;		// 透明度
	ENUM_TextHorizontalAlignment m_eHA;	// 水平对齐方式
	ENUM_TextVerticalAlignment	 m_eVA; // 垂直对齐方式
};