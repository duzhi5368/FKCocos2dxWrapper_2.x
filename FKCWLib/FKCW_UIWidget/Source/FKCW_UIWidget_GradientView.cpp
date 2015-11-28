//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_GradientView.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_GradientView::FKCW_UIWidget_GradientView()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GradientView::~FKCW_UIWidget_GradientView()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GradientView* FKCW_UIWidget_GradientView::create()
{
	FKCW_UIWidget_GradientView* pRet = new FKCW_UIWidget_GradientView();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GradientView* FKCW_UIWidget_GradientView::create(const ccColor4B& tStart, const ccColor4B& tEnd)
{
	FKCW_UIWidget_GradientView* pRet = new FKCW_UIWidget_GradientView();
	if( pRet && pRet->initWithColor(tStart, tEnd) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GradientView* FKCW_UIWidget_GradientView::create(const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& v)
{
	FKCW_UIWidget_GradientView* pRet = new FKCW_UIWidget_GradientView();
	if( pRet && pRet->initWithColor(tStart, tEnd, v) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GradientView::init()
{
	return initWithColor(ccc4(0, 0, 0, 255), ccc4(0, 0, 0, 255));
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GradientView::initWithColor(const ccColor4B& tStart, const ccColor4B& tEnd)
{
	return initWithColor(tStart, tEnd, CCPoint(0, -1));
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GradientView::initWithColor(const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& v)
{
	m_tEndColor.r   = tEnd.r;
	m_tEndColor.g   = tEnd.g;
	m_tEndColor.b   = tEnd.b;

	m_cEndOpacity   = tEnd.a;
	m_cStartOpacity = tStart.a;
	m_tAlongVector  = v;

	m_bCompressedInterpolation = true;

	return FKCW_UIWidget_ColorView::initWithColor(ccc4(tStart.r, tStart.g, tStart.b, 255));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setStartColor(const ccColor3B& tColor)
{
	m_tStartColor = tColor;
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_UIWidget_GradientView::getStartColor()
{
	return m_tStartColor;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setEndColor(const ccColor3B& tColor)
{
	m_tEndColor = tColor;
	updateColor();
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_UIWidget_GradientView::getEndColor()
{
	return m_tEndColor;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setStartOpacity(GLubyte cOpacity)
{
	m_cStartOpacity = cOpacity;
	updateColor();
}
//-------------------------------------------------------------------------
GLubyte FKCW_UIWidget_GradientView::getStartOpacity()
{
	return m_cStartOpacity;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setEndOpacity(GLubyte cOpacity)
{
	m_cEndOpacity = cOpacity;
	updateColor();
}
//-------------------------------------------------------------------------
GLubyte FKCW_UIWidget_GradientView::getEndOpacity()
{
	return m_cEndOpacity;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setVector(const CCPoint& tPoint)
{
	m_tAlongVector = tPoint;
	updateColor();
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_UIWidget_GradientView::getVector()
{
	return m_tAlongVector;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::setCompressedInterpolation(bool bCompressedInterpolation)
{
	m_bCompressedInterpolation = bCompressedInterpolation;
	updateColor();
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GradientView::isCompressedInterpolation()
{
	return m_bCompressedInterpolation;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GradientView::updateColor()
{
	FKCW_UIWidget_ColorView::updateColor();

	float h = ccpLength(m_tAlongVector);
	if( (int)h == 0 )
		return;

	float c = sqrtf(2.0f);
	CCPoint u = CCPoint(m_tAlongVector.x / h, m_tAlongVector.y / h);

	if( m_bCompressedInterpolation )
	{
		float h2 = 1 / ( fabsf(u.x) + fabsf(u.y) );
		u = ccpMult(u, h2 * (float)c);
	}

	float opacityf = (float)_displayedOpacity / 255.0f;

	ccColor4F S = {
		_displayedColor.r / 255.0f,
		_displayedColor.g / 255.0f,
		_displayedColor.b / 255.0f,
		m_cStartOpacity * opacityf / 255.0f
	};

	ccColor4F E = {
		m_tEndColor.r / 255.0f,
		m_tEndColor.g / 255.0f,
		m_tEndColor.b / 255.0f,
		m_cEndOpacity * opacityf / 255.0f
	};

	// (-1, -1)
	m_pSquareColors[0].r = E.r + (S.r - E.r) * ((c + u.x + u.y) / (2.0f * c));
	m_pSquareColors[0].g = E.g + (S.g - E.g) * ((c + u.x + u.y) / (2.0f * c));
	m_pSquareColors[0].b = E.b + (S.b - E.b) * ((c + u.x + u.y) / (2.0f * c));
	m_pSquareColors[0].a = E.a + (S.a - E.a) * ((c + u.x + u.y) / (2.0f * c));
	// (1, -1)
	m_pSquareColors[1].r = E.r + (S.r - E.r) * ((c - u.x + u.y) / (2.0f * c));
	m_pSquareColors[1].g = E.g + (S.g - E.g) * ((c - u.x + u.y) / (2.0f * c));
	m_pSquareColors[1].b = E.b + (S.b - E.b) * ((c - u.x + u.y) / (2.0f * c));
	m_pSquareColors[1].a = E.a + (S.a - E.a) * ((c - u.x + u.y) / (2.0f * c));
	// (-1, 1)
	m_pSquareColors[2].r = E.r + (S.r - E.r) * ((c + u.x - u.y) / (2.0f * c));
	m_pSquareColors[2].g = E.g + (S.g - E.g) * ((c + u.x - u.y) / (2.0f * c));
	m_pSquareColors[2].b = E.b + (S.b - E.b) * ((c + u.x - u.y) / (2.0f * c));
	m_pSquareColors[2].a = E.a + (S.a - E.a) * ((c + u.x - u.y) / (2.0f * c));
	// (1, 1)
	m_pSquareColors[3].r = E.r + (S.r - E.r) * ((c - u.x - u.y) / (2.0f * c));
	m_pSquareColors[3].g = E.g + (S.g - E.g) * ((c - u.x - u.y) / (2.0f * c));
	m_pSquareColors[3].b = E.b + (S.b - E.b) * ((c - u.x - u.y) / (2.0f * c));
	m_pSquareColors[3].a = E.a + (S.a - E.a) * ((c - u.x - u.y) / (2.0f * c));
}
//-------------------------------------------------------------------------