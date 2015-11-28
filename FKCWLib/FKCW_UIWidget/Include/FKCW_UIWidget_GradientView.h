//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_GradientView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_ColorView.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_GradientView : public FKCW_UIWidget_ColorView
{
public:
	FKCW_UIWidget_GradientView();
	virtual ~FKCW_UIWidget_GradientView();
	virtual bool init();
	virtual bool initWithColor(const ccColor4B& tStart, const ccColor4B& tEnd);
	virtual bool initWithColor(const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& v);
	static FKCW_UIWidget_GradientView* create();
	static FKCW_UIWidget_GradientView* create(const ccColor4B& tStart, const ccColor4B& tEnd);
	static FKCW_UIWidget_GradientView* create(const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& v);

	virtual void setStartColor(const ccColor3B& tColor);
	virtual const ccColor3B& getStartColor();
	virtual void setEndColor(const ccColor3B& tColor);
	virtual const ccColor3B& getEndColor();
	virtual void setStartOpacity(GLubyte cOpacity);
	virtual GLubyte getStartOpacity();
	virtual void setEndOpacity(GLubyte cOpacity);
	virtual GLubyte getEndOpacity();
	virtual void setVector(const CCPoint& tPoint);
	virtual const CCPoint& getVector();
	virtual void setCompressedInterpolation(bool bCompressedInterpolation);
	virtual bool isCompressedInterpolation();

protected:
	virtual void updateColor();

protected:
	ccColor3B m_tStartColor;
	ccColor3B m_tEndColor;
	GLubyte m_cStartOpacity;
	GLubyte m_cEndOpacity;
	CCPoint m_tAlongVector;
	bool m_bCompressedInterpolation;
};
