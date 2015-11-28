//*************************************************************************
//	创建日期:	2014-11-22
//	文件名称:	FKCW_RenderEx_TurnCardSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
class FKCW_RenderEx_TurnCardSprite : public CCSprite
{
public:
	FKCW_RenderEx_TurnCardSprite();
	~FKCW_RenderEx_TurnCardSprite();
	static FKCW_RenderEx_TurnCardSprite* create(const char* inCardImageName, const char* outCardImageName, float duration);
	virtual bool init(const char* inCardImageName, const char* outCardImageName, float duration);

private:
	bool				m_bIsOpened;
	CCActionInterval*	m_pOpenAnimIn;
	CCActionInterval*	m_pOpenAnimOut;

	void __InitData(const char* inCardImageName, const char* outCardImageName, float duration);
public:
	void OpenCard();
};