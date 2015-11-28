//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ColorView.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_ColorView::FKCW_UIWidget_ColorView()
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ColorView::~FKCW_UIWidget_ColorView()
{

}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ColorView::init()
{
	return initWithColor(ccc4(0, 0, 0, 0));
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ColorView::initWithColor(const ccColor4B& color)
{
	setTouchEnabled(false);

	_displayedColor.r = _realColor.r = color.r;
	_displayedColor.g = _realColor.g = color.g;
	_displayedColor.b = _realColor.b = color.b;
	_displayedOpacity = _realOpacity = color.a;

	_cascadeOpacityEnabled = false;
	_cascadeColorEnabled = false;

	m_tBlendFunc.src = GL_SRC_ALPHA;
	m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

	for (size_t i = 0; i < sizeof(m_pSquareVertices) / sizeof( m_pSquareVertices[0]); i++ )
	{
		m_pSquareVertices[i].x = 0.0f;
		m_pSquareVertices[i].y = 0.0f;
	}

	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	updateColor();

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::draw()
{
	CC_NODE_DRAW_SETUP();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);

	ccGLBlendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CC_INCREMENT_GL_DRAWS(1);
}
//-------------------------------------------------------------------------
GLubyte FKCW_UIWidget_ColorView::getOpacity()
{
	return _realOpacity;
}
//-------------------------------------------------------------------------
GLubyte FKCW_UIWidget_ColorView::getDisplayedOpacity()
{
	return _displayedOpacity;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::setOpacity(GLubyte opacity)
{
	_displayedOpacity = _realOpacity = opacity;
	if( _cascadeOpacityEnabled )
	{
		GLubyte parentOpacity = 255;
		CCRGBAProtocol *parent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
		if (parent && parent->isCascadeOpacityEnabled())
		{
			parentOpacity = parent->getDisplayedOpacity();
		}
		updateDisplayedOpacity(parentOpacity);
	}
	updateColor();
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_UIWidget_ColorView::getColor()
{
	return _realColor;
}
//-------------------------------------------------------------------------
const ccColor3B& FKCW_UIWidget_ColorView::getDisplayedColor()
{
	return _displayedColor;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::setColor(const ccColor3B& color)
{
	_displayedColor = _realColor = color;
	if (_cascadeColorEnabled)
	{
		ccColor3B parentColor = ccWHITE;
		CCRGBAProtocol* parent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
		if (parent && parent->isCascadeColorEnabled())
		{
			parentColor = parent->getDisplayedColor();
		}

		updateDisplayedColor(parentColor);
	}
	updateColor();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::updateDisplayedOpacity(GLubyte parentOpacity)
{
	_displayedOpacity = static_cast<GLubyte>(_realOpacity * parentOpacity/255.0);

	if (_cascadeOpacityEnabled)
	{
		CCObject *obj = NULL;
		CCARRAY_FOREACH(m_pChildren, obj)
		{
			CCRGBAProtocol *item = dynamic_cast<CCRGBAProtocol*>(obj);
			if (item)
			{
				item->updateDisplayedOpacity(_displayedOpacity);
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::updateDisplayedColor(const ccColor3B& parentColor)
{
	_displayedColor.r = static_cast<GLubyte>(_realColor.r * parentColor.r / 255.0);
	_displayedColor.g = static_cast<GLubyte>(_realColor.g * parentColor.g / 255.0);
	_displayedColor.b = static_cast<GLubyte>(_realColor.b * parentColor.b / 255.0);

	if (_cascadeColorEnabled)
	{
		CCObject *obj = NULL;
		CCARRAY_FOREACH(m_pChildren, obj)
		{
			CCRGBAProtocol *item = dynamic_cast<CCRGBAProtocol*>(obj);
			if (item)
			{
				item->updateDisplayedColor(_displayedColor);
			}
		}
	}
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ColorView::isCascadeOpacityEnabled()
{
	return _cascadeOpacityEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
	_cascadeOpacityEnabled = cascadeOpacityEnabled;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ColorView::isCascadeColorEnabled()
{
	return _cascadeColorEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::setCascadeColorEnabled(bool cascadeColorEnabled)
{
	_cascadeColorEnabled = cascadeColorEnabled;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::setContentSize(const CCSize & size)
{
	m_pSquareVertices[1].x = size.width;
	m_pSquareVertices[2].y = size.height;
	m_pSquareVertices[3].x = size.width;
	m_pSquareVertices[3].y = size.height;

	CCNodeRGBA::setContentSize(size);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::updateColor()
{
	for( unsigned int i = 0; i < 4; ++i )
	{
		m_pSquareColors[i].r = _displayedColor.r / 255.0f;
		m_pSquareColors[i].g = _displayedColor.g / 255.0f;
		m_pSquareColors[i].b = _displayedColor.b / 255.0f;
		m_pSquareColors[i].a = _displayedOpacity / 255.0f;
	}
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ColorView::onTouchBegan(CCTouch* pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint tPoint = m_pParent->convertTouchToNodeSpace(pTouch);
	if( boundingBox().containsPoint(tPoint) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ColorView::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ColorView* FKCW_UIWidget_ColorView::create(const ccColor4B& color)
{
	FKCW_UIWidget_ColorView* pRet = new FKCW_UIWidget_ColorView();
	if( pRet && pRet->initWithColor(color) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ColorView* FKCW_UIWidget_ColorView::create()
{
	FKCW_UIWidget_ColorView* pRet = new FKCW_UIWidget_ColorView();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}
//-------------------------------------------------------------------------