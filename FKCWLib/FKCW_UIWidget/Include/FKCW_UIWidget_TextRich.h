//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_TextRich.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Label.h"
#include "FKCW_UIWidget_Layout.h"
#include "FKCW_UIWidget_ImageView.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
#include "../../FKCW_Base/FKCW_Base.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_TextArea : public FKCW_UIWidget_Label
{
public:
	FKCW_UIWidget_TextArea();
	virtual ~FKCW_UIWidget_TextArea();
	static FKCW_UIWidget_TextArea* create(const CCSize& tSize);
};
//-------------------------------------------------------------------------
enum ENUM_AtlasType
{
	eAtlasType_Label,
	eAtlasType_Sprite
};
//-------------------------------------------------------------------------
struct STextRichElement
{
	ENUM_AtlasType eATLASTYPE;
	std::string strDESCRIPTION;
	CCNode* pATLASITEM;
};
//-------------------------------------------------------------------------
struct STextRichElementRect
{
	CCRect tRECT;
	std::string strDESCRIPTION;
};
//-------------------------------------------------------------------------
typedef std::vector<STextRichElement> CTextRichAtlasLine;
typedef std::vector<CTextRichAtlasLine*> CTextRichAtlas;
//-------------------------------------------------------------------------
class FKCW_UIWidget_TextRich
: public CCNodeRGBA
, public FKCW_UIWidget_Widget
, public FKCW_UIWidget_TextRichClickableProtocol
{
public:
	FKCW_UIWidget_TextRich();
	virtual ~FKCW_UIWidget_TextRich();
	virtual bool init();
	static FKCW_UIWidget_TextRich* create();

	const char* getFontName();
	void setFontName(const char* pFontName);
	float getFontSize() const;
	void setFontSize(float fFontSize);
	unsigned int getMaxLineLength() const;
	void setMaxLineLength(unsigned int uLen);
	float getVerticalSpacing() const;
	void setVerticalSpacing(float fSpacing);

	void insertElement(const char* pString, 
		const char* pFontName = NULL, 
		float fFontSize = 0.0f, 
		const ccColor3B& tColor = ccWHITE, 
		const char* pDescription = NULL);

	void insertElement(CCNode* pNode,
		unsigned int uLen = 0,
		const char* pDescription = NULL);

	void removeAllElements();
	void reloadData();

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

protected:
	void pushAtlasLine();
	void pushAtlasElementAtLast(const STextRichElement& tAtlas);
	void makeAtlasLabelElement(STextRichElement& tElement, const char* str, const char* fontname, float size, const ccColor3B& color, const char* pDescription);

protected:
	float m_fVerticalSpacing;
	unsigned int m_uCharCursor;
	unsigned int m_uMaxLineLength;
	std::string m_strFontName;
	float m_fFontSize;

	vector<STextRichElementRect> m_vRichTextAtlasDescriptions;
	CTextRichAtlas* m_pRichTextAtlas;
};