//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_MenuItemColor.h"
#include "../Include/FKCW_UI_MenuItemColorStateListener.h"
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor::FKCW_UI_MenuItemColor()
	: m_pNormalColor( ccc4(0, 0, 0, 0))	
	, m_pSelectedColor( ccc4(0, 0, 0, 0))
	, m_pDisabledColor( ccc4(0, 0, 0, 0))
	, m_pFocusColor( ccc4(0, 0, 0, 0))
	, m_bIsFocus( false )
	, m_pStateListener( NULL )
{
	m_bEnabled				= true;
	m_tagBlendFunc.src		= CC_BLEND_SRC;
	m_tagBlendFunc.dst		= CC_BLEND_DST;
	memset( m_pSquareVertices, 0, sizeof( m_pSquareVertices ) );
	memset( m_pSquareColors, 0, sizeof( m_pSquareColors ) );
	setContentSize( CCSizeMake(32, 32) );
	ignoreAnchorPointForPosition( false );
	setAnchorPoint( ccp(0.5f, 0.5f ) );
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor::~FKCW_UI_MenuItemColor()
{

}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		NULL,
		NULL);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B focus)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		NULL,
		NULL);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B selected, ccColor4B focus)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		selected,
		ccc4(0, 0, 0, 0),
		NULL,
		NULL);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B selected, ccColor4B focus, ccColor4B disabled)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		selected,
		disabled,
		NULL,
		NULL);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		target,
		selector);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B focus, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		ccc4(0, 0, 0, 0),
		ccc4(0, 0, 0, 0),
		target,
		selector);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B selected, 
					ccColor4B focus, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		selected,
		ccc4(0, 0, 0, 0),
		target,
		selector);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
FKCW_UI_MenuItemColor* FKCW_UI_MenuItemColor::Create(ccColor4B normal, ccColor4B selected,
					ccColor4B focus, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_MenuItemColor* item = new FKCW_UI_MenuItemColor();
	item->InitWithColorAndTarget(normal,
		focus,
		selected,
		disabled,
		target,
		selector);
	return (FKCW_UI_MenuItemColor*)item->autorelease();
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::InitWithColorAndTarget(ccColor4B normal, ccColor4B focus,
					ccColor4B selected, ccColor4B disabled, CCObject* target, SEL_MenuHandler selector)
{
	m_pNormalColor		= normal;
	m_pSelectedColor	= selected;
	m_pFocusColor		= focus;
	m_pDisabledColor	= disabled;
	setTarget(target, selector);

	updateColor();
	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::draw()
{
	CC_NODE_DRAW_SETUP();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );

#ifdef EMSCRIPTEN
	setGLBufferData(m_pSquareVertices, 4 * sizeof(ccVertex2F), 0);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	setGLBufferData(m_pSquareColors, 4 * sizeof(ccColor4F), 1);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);
#else
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);
#endif

	ccGLBlendFunc(m_tagBlendFunc.src, m_tagBlendFunc.dst);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CC_INCREMENT_GL_DRAWS(1);
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::setContentSize(const CCSize& size)
{
	m_pSquareVertices[1].x = size.width;
	m_pSquareVertices[2].y = size.height;
	m_pSquareVertices[3].x = size.width;
	m_pSquareVertices[3].y = size.height;

	CCMenuItem::setContentSize(size);
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::selected()
{
	SelectedSilent();

	if(m_pStateListener) 
	{
		m_pStateListener->onMenuItemColorSelected(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::unselected()
{
	UnselectedSilent();

	if(m_pStateListener) 
	{
		m_pStateListener->onMenuItemColorDeselected(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::setFocus(bool flag)
{
	SetFocusSilent(flag);

	if(m_pStateListener) 
	{
		if(flag)
			m_pStateListener->onMenuItemColorFocused(this);
		else
			m_pStateListener->onMenuItemColorUnfocused(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::setEnabled(bool value)
{
	SetEnabledSilent(value);

	if(m_pStateListener) 
	{
		if(value)
			m_pStateListener->onMenuItemColorEnabled(this);
		else
			m_pStateListener->onMenuItemColorDisabled(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::SelectedSilent()
{
	CCMenuItem::selected();
	updateColor();
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::UnselectedSilent()
{
	CCMenuItem::unselected();
	updateColor();
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::SetEnabledSilent(bool value)
{
	CCMenuItem::setEnabled(value);
	updateColor();
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::SetFocusSilent(bool flag)
{
	m_bIsFocus = flag;
	updateColor();
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::updateColor()
{
	if(isSelected()) 
	{
		_displayedColor.r = _realColor.r = m_pSelectedColor.r;
		_displayedColor.g = _realColor.g = m_pSelectedColor.g;
		_displayedColor.b = _realColor.b = m_pSelectedColor.b;
		_displayedOpacity = _realOpacity = m_pSelectedColor.a;
	} 
	else if(isFocus()) 
	{
		_displayedColor.r = _realColor.r = m_pFocusColor.r;
		_displayedColor.g = _realColor.g = m_pFocusColor.g;
		_displayedColor.b = _realColor.b = m_pFocusColor.b;
		_displayedOpacity = _realOpacity = m_pFocusColor.a;
	} 
	else if(isEnabled()) 
	{
		_displayedColor.r = _realColor.r = m_pNormalColor.r;
		_displayedColor.g = _realColor.g = m_pNormalColor.g;
		_displayedColor.b = _realColor.b = m_pNormalColor.b;
		_displayedOpacity = _realOpacity = m_pNormalColor.a;
	} 
	else 
	{
		_displayedColor.r = _realColor.r = m_pDisabledColor.r;
		_displayedColor.g = _realColor.g = m_pDisabledColor.g;
		_displayedColor.b = _realColor.b = m_pDisabledColor.b;
		_displayedOpacity = _realOpacity = m_pDisabledColor.a;
	}

	for(unsigned int i = 0; i < 4; i++) {
		m_pSquareColors[i].r = _displayedColor.r / 255.0f;
		m_pSquareColors[i].g = _displayedColor.g / 255.0f;
		m_pSquareColors[i].b = _displayedColor.b / 255.0f;
		m_pSquareColors[i].a = _displayedOpacity / 255.0f;
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_MenuItemColor::setBlendFunc(ccBlendFunc blendFunc)
{
	m_tagBlendFunc = blendFunc;
}
//-------------------------------------------------------------------------
ccBlendFunc FKCW_UI_MenuItemColor::getBlendFunc() 
{
	return m_tagBlendFunc;
}
//-------------------------------------------------------------------------