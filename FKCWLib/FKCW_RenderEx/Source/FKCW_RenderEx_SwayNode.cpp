//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_SwayNode.h"
//-------------------------------------------------------------------------
static const GLchar* szSwayShader = 
	"																									\n\
	#ifdef GL_ES																						\n\
	precision mediump float;																			\n\
	#endif																								\n\
	\n\
	varying vec2 v_texCoord;																			\n\
	uniform sampler2D u_texture;																		\n\
	uniform float u_time;																				\n\
	\n\
	// 1																								\n\
	const float speed = 2.0;																			\n\
	const float bendFactor = 0.2;																		\n\
	void main()																							\n\
	{																									\n\
	// 获得高度，texCoord从下到上为0到1																	\n\
	float height = 1.0 - v_texCoord.y;																	\n\
	// 获得偏移量，一个幂函数，值愈大，导数越大，偏移量愈大												\n\
	float offset = pow(height, 2.5);																	\n\
	// 偏移量随时间变化，并乘以幅度，设置频率															\n\
	offset *= (sin(u_time * speed) * bendFactor);														\n\
	// 使x坐标偏移，fract取区间值（0，1）																\n\
	vec3 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb;		\n\
	gl_FragColor = vec4(normalColor, 1);																\n\
	}";
//--------------------------------------------------------
FKCW_RenderEx_SwayNode::FKCW_RenderEx_SwayNode()
	: m_pSprite( NULL )
	, m_fTime( 0.0f )
	, m_nTimeUniformLocation( 0 )
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_SwayNode::~FKCW_RenderEx_SwayNode()
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_SwayNode* FKCW_RenderEx_SwayNode::create( const char* p_szSpriteFile )
{
	FKCW_RenderEx_SwayNode *pNode = new FKCW_RenderEx_SwayNode();
	if (pNode && pNode->init( p_szSpriteFile ))
	{
		pNode->autorelease();
		return pNode;
	}

	CC_SAFE_DELETE(pNode);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_SwayNode::init( const char* p_szSpriteFile )
{
	do{
		m_fTime = 0.0f;

		m_pSprite = CCSprite::create( p_szSpriteFile );
		if( m_pSprite == NULL )
		{
			break;
		}
		this->addChild( m_pSprite );

		// 加载顶点着色器和片元着色器
		CCGLProgram* pShader = new  CCGLProgram();
		pShader->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, szSwayShader );
		m_pSprite->setShaderProgram( pShader );
		pShader->release();
		CHECK_GL_ERROR_DEBUG();

		// 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
		m_pSprite->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		m_pSprite->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();

		// 自定义着色器链接
		m_pSprite->getShaderProgram()->link();
		CHECK_GL_ERROR_DEBUG();

		// 设置移动、缩放、旋转矩阵
		m_pSprite->getShaderProgram()->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		m_nTimeUniformLocation = glGetUniformLocation( m_pSprite->getShaderProgram()->getProgram(), "u_time" );

		m_pSprite->getShaderProgram()->use();

		// 开启帧更新
		this->schedule(schedule_selector(FKCW_RenderEx_SwayNode::UpdatePos));

		return true;
	}while(0);
	return false;
}
//-------------------------------------------------------------------------

// 该函数每帧都会被调用
void FKCW_RenderEx_SwayNode::UpdatePos( float delta )
{
	m_fTime += delta;
	if( m_pSprite )
	{
		m_pSprite->getShaderProgram()->use();
	}
	glUniform1f( m_nTimeUniformLocation, m_fTime);
}
//-------------------------------------------------------------------------