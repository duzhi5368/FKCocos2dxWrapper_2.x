//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_SpotLightSprite.h"
//-------------------------------------------------------------------------
const static int SEGS = 45;
//-------------------------------------------------------------------------
FKCW_RenderEx_SpotLightSprite*	FKCW_RenderEx_SpotLightSprite::InitWithRenderTexture( CCRenderTexture* pRenderTex,
											  float fRadius, ccColor4B tagRenderColor )
{
	do 
	{
		CC_BREAK_IF( !CCSprite::init() );

		m_pRenderTex = pRenderTex;
		m_fSpotLightRadius = fRadius;
		m_tagRenderColor = tagRenderColor;

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		setPosition( ccp( winSize.width / 2, winSize.height / 2 ) );

		m_pVertices = (GLfloat *)malloc( sizeof(GLfloat)*2*(SEGS));
		m_pCoordinates = (GLfloat *)malloc( sizeof(GLfloat)*2*(SEGS));
		m_pColors = (ccColor4B *)malloc( sizeof(ccColor4B)*(SEGS));

		// ¿ªÆôÖ¡Ñ­»·
		schedule( schedule_selector( FKCW_RenderEx_SpotLightSprite::Tick ));

		return this;
	} while ( 0 );
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_SpotLightSprite* FKCW_RenderEx_SpotLightSprite::Create( CCRenderTexture* pRenderTex,
							   float fRadius, ccColor4B tagRenderColor )
{
	FKCW_RenderEx_SpotLightSprite* pSpotLight = new FKCW_RenderEx_SpotLightSprite();
	if(pSpotLight && pSpotLight->InitWithRenderTexture(pRenderTex, fRadius, tagRenderColor ))
	{
		pSpotLight->autorelease();
		return pSpotLight;
	}
	CC_SAFE_DELETE(pSpotLight);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_SpotLightSprite::Tick(float dt)
{
	CC_NODE_DRAW_SETUP();

	memset(m_pVertices,0, sizeof(GLfloat)*2*(SEGS));
	memset(m_pCoordinates,0, sizeof(GLfloat)*2*(SEGS));
	m_pRenderTex->clear( m_tagRenderColor.r / 255.0f, 
		m_tagRenderColor.g / 255.0f, m_tagRenderColor.b / 255.0f, 
		m_tagRenderColor.a / 255.0f );

	m_pColors[0] = ccc4(0, 0, 0, 255);
	for (int i = 1; i < SEGS; i++)
	{
		m_pColors[i] = ccc4(0, 0, 0, 0);
	}

	const float coef = 2.0f * (float)M_PI/(SEGS-2) ;

	m_pVertices[0] = getPosition().x;
	m_pVertices[1] = getPosition().y;
	m_pCoordinates[0] = getPosition().x;
	m_pCoordinates[1] = (getContentSize().height-getPosition().y);
	for(int i=1;i<=SEGS;i++)
	{
		float rads = i*coef;
		float j = m_fSpotLightRadius * cosf(rads) + getPosition().x;
		float k = m_fSpotLightRadius * sinf(rads) + getPosition().y;

		m_pVertices[i*2] = j;
		m_pVertices[i*2+1] = k;

		m_pCoordinates[i*2] = (j);
		m_pCoordinates[i*2+1] = (getContentSize().height-k);
	}

	// Update the render texture
	m_pRenderTex->begin();

	ccGLBindTexture2D((GLuint)m_pRenderTex);
	ccGLBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pCoordinates);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, m_pColors);

	glDrawArrays(GL_TRIANGLE_FAN, 0, SEGS);
	ccGLBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	m_pRenderTex->end();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_SpotLightSprite::RemoveSelf()
{
	schedule( schedule_selector( FKCW_RenderEx_SpotLightSprite::SpreadOutTimer ));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_SpotLightSprite::SpreadOutTimer(float dt)
{
	m_fSpotLightRadius += 3.0f;

	if( m_fSpotLightRadius > 500.0f )
	{
		unscheduleAllSelectors();
		m_pRenderTex->removeFromParentAndCleanup( true );
		removeFromParentAndCleanup( true );

		/*CC_SAFE_FREE(m_pVertices);
		CC_SAFE_FREE(m_pCoordinates);
		CC_SAFE_FREE(m_pColors);*/
	}
}
//-------------------------------------------------------------------------