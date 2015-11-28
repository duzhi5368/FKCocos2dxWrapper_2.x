//-------------------------------------------------------------------------
#pragma warning( disable: 4244 )
//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_DrawingPrimitives.h"
#include "../Include/FKCW_Base_Types.h"
#include "../Include/FKCW_Base_Geometry.h"
//-------------------------------------------------------------------------
static bool			s_bInitialized = false;					// 是否初始化
static CCGLProgram* s_pShader = NULL;						// 渲染shaderProgram
static int			s_nColorLocation = -1;					//
static ccColor4F	s_tColor = {1.0f,1.0f,1.0f,1.0f};		// 绘制颜色
//-------------------------------------------------------------------------
static void LazyInit()
{
	if( ! s_bInitialized ) 
	{
		// 模拟 glColor4ub
		s_pShader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);

		s_nColorLocation = glGetUniformLocation( s_pShader->getProgram(), "u_color");
		CHECK_GL_ERROR_DEBUG();

		s_bInitialized = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_Base_DrawingPrimitives::DrawColor4BEx(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	s_tColor.r = r/255.0f;
	s_tColor.g = g/255.0f;
	s_tColor.b = b/255.0f;
	s_tColor.a = a/255.0f;
	ccDrawColor4B(r,g,b,a);
}
//-------------------------------------------------------------------------
// 绘制一个实心圆/弧
void FKCW_Base_DrawingPrimitives::DrawSolidCircle(const CCPoint& center, float radius, float angle,
					unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY)
{
	LazyInit();

	int additionalSegment = 1;
	if (drawLineToCenter)
		additionalSegment++;

	const float coef = 2.0f * (float)M_PI/segments;

	GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
	if( ! vertices )
		return;

	for(unsigned int i = 0;i <= segments; i++) {
		float rads = i*coef;
		GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
		GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;

		vertices[i*2] = j;
		vertices[i*2+1] = k;
	}
	vertices[(segments+1)*2] = center.x;
	vertices[(segments+1)*2+1] = center.y;

	s_pShader->use();
	s_pShader->setUniformsForBuiltins();
	s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);

	free( vertices );

	CC_INCREMENT_GL_DRAWS(1);
}
//-------------------------------------------------------------------------
// 绘制一个实心矩形
void FKCW_Base_DrawingPrimitives::DrawSolidRoundRect(CCPoint origin, CCPoint destination, float cornerRadius, ccColor4B color)
{
	// 保证基准点在左下角
	CCPoint bl = origin;
	CCPoint tr = destination;
	if(bl.x > tr.x) {
		bl.x = MIN(origin.x, destination.x);
		tr.x = MAX(origin.x, destination.x);
	}
	if(bl.y > tr.y) {
		bl.y = MIN(origin.y, destination.y);
		tr.y = MAX(origin.y, destination.y);
	}

	// 四角的中心
	CCPoint tlCenter = ccp(bl.x + cornerRadius, tr.y - cornerRadius);
	CCPoint trCenter = ccp(tr.x - cornerRadius, tr.y - cornerRadius);
	CCPoint blCenter = ccp(bl.x + cornerRadius, bl.y + cornerRadius);
	CCPoint brCenter = ccp(tr.x - cornerRadius, bl.y + cornerRadius);

	// 填充顶点数据
	CCPoint vertices[] = 
	{
		// 左边缘
		ccp(bl.x, bl.y + cornerRadius),
		ccp(bl.x, tr.y - cornerRadius),

		// 左上角
		ccpAdd(tlCenter, ccpDegree(170) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(160) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(150) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(140) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(130) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(120) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(110) * cornerRadius),
		ccpAdd(tlCenter, ccpDegree(100) * cornerRadius),

		// 上边缘
		ccp(bl.x + cornerRadius, tr.y),
		ccp(tr.x - cornerRadius, tr.y),

		// 右上角
		ccpAdd(trCenter, ccpDegree(80) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(70) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(60) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(50) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(40) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(30) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(20) * cornerRadius),
		ccpAdd(trCenter, ccpDegree(10) * cornerRadius),

		// 右边缘
		ccp(tr.x, tr.y - cornerRadius),
		ccp(tr.x, bl.y + cornerRadius),

		// 右下角
		ccpAdd(brCenter, ccpDegree(-10) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-20) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-30) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-40) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-50) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-60) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-70) * cornerRadius),
		ccpAdd(brCenter, ccpDegree(-80) * cornerRadius),

		// 下边缘
		ccp(tr.x - cornerRadius, bl.y),
		ccp(bl.x + cornerRadius, bl.y),

		// 左下角
		ccpAdd(blCenter, ccpDegree(-100) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-110) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-120) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-130) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-140) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-150) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-160) * cornerRadius),
		ccpAdd(blCenter, ccpDegree(-170) * cornerRadius)
	};

	ccDrawSolidPoly(vertices, 40, ccc4FFromccc4B(color));
}
//-------------------------------------------------------------------------
// RectNode
//-------------------------------------------------------------------------
CCRectNode* CCRectNode::createWithSize(const CCSize& tSize, const ccColor4F& tColor, bool bFill)
{
	return createWithRect(ccRectMakeWithOriginAndSize(CCPointZero, tSize), tColor, bFill);
}
//-------------------------------------------------------------------------
CCRectNode* CCRectNode::createWithRect(const CCRect& tRect, const ccColor4F& tColor, bool bFill)
{
	CCRectNode* pRet = new CCRectNode();
	return pRet->initWithRect(tRect, tColor, bFill) ? (CCRectNode*)pRet->autorelease() : (delete pRet, pRet = NULL);
}
//-------------------------------------------------------------------------
bool CCRectNode::initWithSize(const CCSize& tSize, const ccColor4F& tColor, bool bFill)
{
	setContentSize(tSize);
	setColor(ccc3(tColor.r * 255, tColor.b * 255, tColor.g * 255));
	setOpacity(tColor.a * 255);
	m_bFill = bFill;

	return true;
}
//-------------------------------------------------------------------------
bool CCRectNode::initWithRect(const CCRect& tRect, const ccColor4F& tColor, bool bFill)
{
	setPosition(tRect.origin);
	return initWithSize(tRect.size, tColor, bFill);
}
//-------------------------------------------------------------------------
void CCRectNode::draw()
{
	const ccColor3B& tColor(getColor());
	CCPoint tOrigin(isIgnoreAnchorPointForPosition() ? CCPointZero : getAnchorPointInPoints());
	CCPoint tDestination(ccpShift(tOrigin, getContentSize()));

	if (m_bFill)
	{
		ccColor4F tColor4F(ccc4FFromccc3B(tColor));
		tColor4F.a = getOpacity() / 255.0f;
		FKCW_Base_DrawingPrimitives::DrawSolidRoundRect(tOrigin, tDestination, 0, ccc4BFromccc4F(tColor4F));
	}
	else
	{
		FKCW_Base_DrawingPrimitives::DrawColor4BEx(tColor.r, tColor.g, tColor.b, getOpacity());
		ccDrawRect(tOrigin, tDestination);
	}
}
//-------------------------------------------------------------------------
// LineNode
//-------------------------------------------------------------------------
CCLineNode* CCLineNode::createWithPoints(const CCPoint& tBeginPoint, const CCPoint& tEndPoint, const ccColor4F& tColor)
{
	CCLineNode* pRet = new CCLineNode();
	return pRet->initWithPoints(tBeginPoint, tEndPoint, tColor) ? (CCLineNode*)pRet->autorelease() : (delete pRet, pRet = NULL);
}
//-------------------------------------------------------------------------
bool CCLineNode::initWithPoints(const CCPoint& tBeginPoint, const CCPoint& tEndPoint, const ccColor4F& tColor)
{
	setPosition(tBeginPoint);
	setContentSize(ccpDiff(tEndPoint, tBeginPoint));
	setColor(ccc3(tColor.r * 255, tColor.b * 255, tColor.g * 255));
	setOpacity(tColor.a * 255);

	return true;
}
//-------------------------------------------------------------------------
void CCLineNode::draw()
{
	const ccColor3B& tColor(getColor());
	FKCW_Base_DrawingPrimitives::DrawColor4BEx(tColor.r, tColor.g, tColor.b, getOpacity());
	CCPoint tOrigin(isIgnoreAnchorPointForPosition() ? CCPointZero : getAnchorPointInPoints());
	ccDrawLine(tOrigin, ccpShift(tOrigin, getContentSize()));
}
//-------------------------------------------------------------------------
// DotNode
//-------------------------------------------------------------------------
CCDotNode* CCDotNode::createWithPoint(const CCPoint& tPoint, const ccColor4F& tColor)
{
	CCDotNode* pRet = new CCDotNode();
	return pRet->initWithPoint(tPoint, tColor) ? (CCDotNode*)pRet->autorelease() : (delete pRet, pRet = NULL);
}
//-------------------------------------------------------------------------
bool CCDotNode::initWithPoint(const CCPoint& tPoint, const ccColor4F& tColor)
{
	setPosition(tPoint);
	setColor(ccc3(tColor.r * 255, tColor.b * 255, tColor.g * 255));
	setOpacity(tColor.a * 255);

	return true;
}
//-------------------------------------------------------------------------
void CCDotNode::draw()
{
	const ccColor3B& tColor(getColor());
	FKCW_Base_DrawingPrimitives::DrawColor4BEx(tColor.r, tColor.g, tColor.b, getOpacity());
	ccDrawPoint(CCPointZero);
}
//-------------------------------------------------------------------------