//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_DynamicNumLabel.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel::FKCW_UI_DynamicNumLabel()
{

}
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel::~FKCW_UI_DynamicNumLabel()
{

}
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel * FKCW_UI_DynamicNumLabel::create(const char *string, const char *fontName, float fontSize)
{
	return FKCW_UI_DynamicNumLabel::create(string, fontName, fontSize,
		CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel * FKCW_UI_DynamicNumLabel::create(const char *string, const char *fontName, float fontSize,
										const CCSize& dimensions, CCTextAlignment hAlignment)
{
	return FKCW_UI_DynamicNumLabel::create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop);
}
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel* FKCW_UI_DynamicNumLabel::create(const char *string, const char *fontName, float fontSize,
									   const CCSize &dimensions, CCTextAlignment hAlignment,
									   CCVerticalTextAlignment vAlignment)
{
	FKCW_UI_DynamicNumLabel *pRet = new FKCW_UI_DynamicNumLabel();
	if(pRet && pRet->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
	{
		pRet->__InitData( string );
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UI_DynamicNumLabel * FKCW_UI_DynamicNumLabel::createWithFontDefinition(const char *string, ccFontDefinition &textDefinition)
{
	FKCW_UI_DynamicNumLabel *pRet = new FKCW_UI_DynamicNumLabel();
	if(pRet && pRet->initWithStringAndTextDefinition(string, textDefinition))
	{
		pRet->__InitData( string );
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UI_DynamicNumLabel::__InitData( const char* p_szString )
{
	m_nCurValue = 0;
	m_bIsInUpdate = false;
	int a = atoi( p_szString );
	setString( FKCW_Base_Utils::ItoA(a).c_str() );
}
//-------------------------------------------------------------------------
void FKCW_UI_DynamicNumLabel::__UpdateDynamicValue(float dt)
{
	int temScore = atoi(getString());
	int addScore = m_nCurValue - temScore;

	if(abs(addScore)>10)
	{   
		temScore += addScore / 10;
		setString(CCString::createWithFormat("%d", temScore)->getCString());
	}
	else if(abs(addScore) > 2 && abs(addScore) <= 10)  
	{
		temScore += addScore / abs(addScore);
		setString(CCString::createWithFormat("%d", temScore)->getCString());
	}
	else
	{
		unschedule(schedule_selector(FKCW_UI_DynamicNumLabel::__UpdateDynamicValue));
		m_bIsInUpdate = false;
		setString(CCString::createWithFormat("%d", m_nCurValue)->getCString());
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_DynamicNumLabel::SetDynamicValue(int value)
{
	m_nCurValue = value;
	if (!m_bIsInUpdate)
	{
		m_bIsInUpdate = true;
		schedule(schedule_selector(FKCW_UI_DynamicNumLabel::__UpdateDynamicValue));
	}
}
//-------------------------------------------------------------------------
int FKCW_UI_DynamicNumLabel::GetDynamicValue()
{
	return m_nCurValue;
}
//-------------------------------------------------------------------------