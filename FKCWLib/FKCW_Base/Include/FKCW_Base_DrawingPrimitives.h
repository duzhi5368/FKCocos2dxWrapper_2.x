//*************************************************************************
//	创建日期:	2014-11-3
//	文件名称:	FKCW_Base_DrawingPrimitives.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
// 绘制基本元素方法类。是对CCDrawingPrimitives的一个优化。
class FKCW_Base_DrawingPrimitives
{
public:
	// 设置颜色
	static void DrawColor4BEx(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	// 绘制一个实心圆/弧
	static void DrawSolidCircle(const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);
	// 绘制一个实心矩形
	static void DrawSolidRoundRect(CCPoint origin, CCPoint destination, float cornerRadius, ccColor4B color);
};
//-------------------------------------------------------------------------
// 基本元素 矩形，线，点 的绘制节点
class CCRectNode : public CCNodeRGBA
{
public:
	static CCRectNode* createWithSize(const CCSize& tSize, const ccColor4F& tColor, bool bFill = false);
	static CCRectNode* createWithRect(const CCRect& tRect, const ccColor4F& tColor, bool bFill = false);
	bool initWithSize(const CCSize& tSize, const ccColor4F& tColor, bool bFill = false);
	bool initWithRect(const CCRect& tRect, const ccColor4F& tColor, bool bFill = false);
	virtual void draw();

private:
	bool m_bFill;		// 是否填实
};
//-------------------------------------------------------------------------
class CCLineNode : public CCNodeRGBA
{
public:
	static CCLineNode* createWithPoints(const CCPoint& tBeginPoint, const CCPoint& tEndPoint, const ccColor4F& tColor);
	bool initWithPoints(const CCPoint& tBeginPoint, const CCPoint& tEndPoint, const ccColor4F& tColor);
	virtual void draw();
};
//-------------------------------------------------------------------------
class CCDotNode : public CCNodeRGBA
{
public:
	static CCDotNode* createWithPoint(const CCPoint& tPoint, const ccColor4F& tColor);
	bool initWithPoint(const CCPoint& tPoint, const ccColor4F& tColor);
	virtual void draw();
};
//-------------------------------------------------------------------------