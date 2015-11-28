//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_TextRich.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_TextArea::FKCW_UIWidget_TextArea()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_TextArea::~FKCW_UIWidget_TextArea()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_TextArea* FKCW_UIWidget_TextArea::create(const CCSize& tSize)
{
	FKCW_UIWidget_TextArea* pRet = new FKCW_UIWidget_TextArea();
	if( pRet && pRet->init() )
	{
		pRet->setDimensions(tSize);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
// TextRich
//-------------------------------------------------------------------------
FKCW_UIWidget_TextRich::FKCW_UIWidget_TextRich()
	: m_strFontName("")
	, m_fFontSize(30.0f)
	, m_uMaxLineLength(0)
	, m_uCharCursor(0)
	, m_fVerticalSpacing(0.0f)
{
	setThisObject(this);
	m_pRichTextAtlas = new CTextRichAtlas();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_TextRich::~FKCW_UIWidget_TextRich()
{
	removeAllElements();
	CC_SAFE_DELETE(m_pRichTextAtlas);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_TextRich* FKCW_UIWidget_TextRich::create()
{
	FKCW_UIWidget_TextRich* pRet = new FKCW_UIWidget_TextRich();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_TextRich::init()
{
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::insertElement(const char* pString
							  , const char* pFontName
							  , float fFontSize
							  , const ccColor3B& tColor
							  , const char* pDescription)
{
	CCAssert(pString && strlen(pString), "should not null");

	if( m_uMaxLineLength != 0 )
	{
		if( m_uCharCursor == m_uMaxLineLength )
		{
			m_uCharCursor = 0;
			pushAtlasLine();
		}

		string str_buffer;
		char* pstr = const_cast<char*>(pString);

		while( *pstr != 0 )
		{
			int nCharLength;

			int nlen = FKCW_Base_Utils::GetUTF8Btyes(*pstr);
			if( nlen > 1 )
			{
				nCharLength = 2; //default 2 (maybe a word of chinese, can be else)
			}
			else
			{
				nCharLength = 1; //English char
			}

			if( m_uCharCursor + nCharLength > m_uMaxLineLength )
			{
				STextRichElement tElement;
				makeAtlasLabelElement(tElement, str_buffer.c_str(), pFontName, fFontSize, tColor, pDescription);

				pushAtlasElementAtLast(tElement);
				pushAtlasLine();

				str_buffer.clear();
				str_buffer.append(pstr,(unsigned int)nlen);
				m_uCharCursor = 0;
			}
			else
			{
				str_buffer.append(pstr,(unsigned int)nlen);
			}

			m_uCharCursor += nCharLength;
			pstr += nlen;
		}

		if( str_buffer.empty() )
			return;

		STextRichElement tElement;
		makeAtlasLabelElement(tElement, str_buffer.c_str(), pFontName, fFontSize, tColor, pDescription);

		pushAtlasElementAtLast(tElement);
	}
	else
	{
		STextRichElement tElement;
		makeAtlasLabelElement(tElement, pString, pFontName, fFontSize, tColor, pDescription);

		pushAtlasElementAtLast(tElement);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::insertElement(CCNode* pNode
							  , unsigned int uLen
							  , const char* pDescription)
{
	CCAssert(pNode, "should not null");

	if( m_uMaxLineLength != 0 )
	{
		CCAssert(uLen <= m_uMaxLineLength, "len should be less than m_uMaxLineLength");

		m_uCharCursor += uLen;

		if( m_uCharCursor > m_uMaxLineLength )
		{
			pushAtlasLine();
			m_uCharCursor = 0;
		}

		STextRichElement tElement;
		tElement.eATLASTYPE = eAtlasType_Sprite;
		tElement.pATLASITEM = pNode;
		tElement.strDESCRIPTION = pDescription ? pDescription : "";
		pNode->setAnchorPoint(CCPointZero);

		pushAtlasElementAtLast(tElement);
	}
	else
	{
		STextRichElement tElement;
		tElement.eATLASTYPE = eAtlasType_Sprite;
		tElement.pATLASITEM = pNode;
		tElement.strDESCRIPTION = pDescription ? pDescription : "";
		pNode->setAnchorPoint(CCPointZero);

		pushAtlasElementAtLast(tElement);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::reloadData()
{
	CCAssert(!m_pRichTextAtlas->empty(), "reload data was empty");
	removeAllChildrenWithCleanup(true);
	m_vRichTextAtlasDescriptions.clear();
	m_uCharCursor = 0;

	CCSize tRealContentSize = CCSizeZero;

	int nRowBegin = m_pRichTextAtlas->size() - 1;
	unsigned int uRowEnd = 0;
	unsigned int uColumn = 0;
	unsigned int uColumnSize = 0;

	for(; nRowBegin >= (int)uRowEnd; --nRowBegin )
	{
		CTextRichAtlasLine* vAtlasLine = m_pRichTextAtlas->at(nRowBegin);
		uColumnSize = vAtlasLine->size();
		uColumn = 0;

		float fWidgetOfLine = 0.0f;
		float fMaxHeightOfLine = 0.0f;

		for(; uColumn < uColumnSize; ++uColumn )
		{
			STextRichElement& tElement = vAtlasLine->at(uColumn);

			tElement.pATLASITEM->setPosition(CCPoint(fWidgetOfLine, tRealContentSize.height));
			addChild(tElement.pATLASITEM);
#if 0
			CCLayerColor* pColor = CCLayerColor::create(ccc4(255, 0,0, 129));
			pColor->setPosition(CCPoint(fWidgetOfLine, tRealContentSize.height) + ccp(1, 1));
			pColor->setContentSize(tElement.pATLASITEM->getContentSize() - CCSize(2,2));
			addChild(pColor);
#endif

			STextRichElementRect tDES;
			tDES.tRECT = tElement.pATLASITEM->boundingBox();
			tDES.strDESCRIPTION = tElement.strDESCRIPTION;
			m_vRichTextAtlasDescriptions.push_back(tDES);

			fWidgetOfLine += tElement.pATLASITEM->getContentSize().width;
			fMaxHeightOfLine = MAX(fMaxHeightOfLine, tElement.pATLASITEM->getContentSize().height);
		}

		tRealContentSize.width = MAX(tRealContentSize.width, fWidgetOfLine);
		tRealContentSize.height += fMaxHeightOfLine + m_fVerticalSpacing;

		delete vAtlasLine;
	}

	m_pRichTextAtlas->clear();

	tRealContentSize.height -= m_fVerticalSpacing;
	setContentSize(tRealContentSize);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::removeAllElements()
{
	CTextRichAtlas::iterator itr = m_pRichTextAtlas->begin();
	CTextRichAtlas::iterator end = m_pRichTextAtlas->end();

	for(; itr != end; ++itr )
	{
		delete (*itr);
	}

	m_pRichTextAtlas->clear();
	m_vRichTextAtlasDescriptions.clear();
	m_uCharCursor = 0;

	removeAllChildrenWithCleanup(true);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::makeAtlasLabelElement(STextRichElement& tElement
									  , const char* str
									  , const char* fontname
									  , float size
									  , const ccColor3B& color
									  , const char* pDescription)
{
	CCLabelTTF* pLabel = CCLabelTTF::create(str, 
		fontname && strlen(fontname) ? fontname : m_strFontName.c_str(),
		(int) size == 0 ? m_fFontSize : size);

	pLabel->setAnchorPoint(CCPointZero);
	pLabel->setColor(color);

	tElement.strDESCRIPTION = pDescription ? pDescription : "";
	tElement.eATLASTYPE = eAtlasType_Label;
	tElement.pATLASITEM = pLabel;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::pushAtlasLine()
{
	m_pRichTextAtlas->push_back(new CTextRichAtlasLine());
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::pushAtlasElementAtLast(const STextRichElement& tAtlasElement)
{
	if( m_pRichTextAtlas->empty() )
	{
		pushAtlasLine();
	}

	m_pRichTextAtlas->back()->push_back(tAtlasElement);
}
//-------------------------------------------------------------------------
const char* FKCW_UIWidget_TextRich::getFontName()
{
	return m_strFontName.c_str();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::setFontName(const char* pFontName)
{
	m_strFontName = pFontName;
}
//-------------------------------------------------------------------------
float FKCW_UIWidget_TextRich::getFontSize() const
{
	return m_fFontSize;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::setFontSize(float fFontSize)
{
	m_fFontSize = fFontSize;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_TextRich::getMaxLineLength() const
{
	return m_uMaxLineLength;
}
//-------------------------------------------------------------------------
float FKCW_UIWidget_TextRich::getVerticalSpacing() const
{
	return m_fVerticalSpacing;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::setVerticalSpacing(float fSpacing)
{
	m_fVerticalSpacing = fSpacing;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::setMaxLineLength(unsigned int uLen)
{
	CCAssert(uLen == 0 || uLen >= 2, "len should bigger than 2");
	m_uMaxLineLength = uLen;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_TextRich::onTouchBegan(CCTouch* pTouch)
{
	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::onTouchMoved(CCTouch* pTouch, float fDuration)
{

}
//-------------------------------------------------------------------------
void FKCW_UIWidget_TextRich::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	CCPoint tPoint = m_pParent->convertTouchToNodeSpace(pTouch);
	if( boundingBox().containsPoint(tPoint) )
	{
		CCPoint tInsidePoint = convertTouchToNodeSpace(pTouch);
		vector<STextRichElementRect>::iterator itr = m_vRichTextAtlasDescriptions.begin();
		vector<STextRichElementRect>::iterator end = m_vRichTextAtlasDescriptions.end();

		for(; itr != end; ++itr )
		{
			if( itr->tRECT.containsPoint(tInsidePoint) )
			{
				executeTextRichClickHandler(this, 
					itr->strDESCRIPTION.empty() ? NULL : itr->strDESCRIPTION.c_str());
				return;
			}
		}
		executeTextRichClickHandler(this, NULL);
	}
}

void FKCW_UIWidget_TextRich::onTouchCancelled(CCTouch* pTouch, float fDuration)
{

}