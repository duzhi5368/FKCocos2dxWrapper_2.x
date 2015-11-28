//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_NormalMappedSprite.h"
//-------------------------------------------------------------------------
const char* s_NormalMappedFragShader = "														\n\
#ifdef GL_ES																					\n\
	precision mediump float;																	\n\
#endif																							\n\
																								\n\
varying vec2 v_texCoord;																		\n\
uniform sampler2D CC_Texture0;																	\n\
uniform sampler2D u_normalMap;																	\n\
																								\n\
uniform vec4 u_diffuseM;																		\n\
uniform vec4 u_ambientM;																		\n\
uniform vec4 u_diffuseL;																		\n\
uniform vec4 u_ambientL;																		\n\
uniform vec2 u_contentSize;																		\n\
uniform vec4 u_lightPosInLocalSpace;															\n\
uniform float u_kBump;																			\n\
void main() 																					\n\
{																								\n\
	vec4 diffuseML=u_diffuseM*u_diffuseL;														\n\
	vec4 ambientML=u_ambientM*u_ambientL;														\n\
	vec4 texColor=texture2D(CC_Texture0, v_texCoord);											\n\
	vec3 normal=texture2D(u_normalMap, v_texCoord).rgb;											\n\
	normal=normal*2.0-1.0;																		\n\
	// 法线贴图制作需要通过Nvidia Texture Tools for Photoshop.使用向下Y轴。所以要用取反			\n\
	normal.y=-normal.y;																			\n\
	if(u_kBump!=1.0)																			\n\
	{																							\n\
		normal=vec3(normal.x,normal.y,normal.z/u_kBump);										\n\
		normal=normalize(normal);																\n\
	}																							\n\
	vec4 curPixelPosInLocalSpace=vec4(v_texCoord.x*u_contentSize.x,								\n\
		(1.0-v_texCoord.y)*u_contentSize.y,0.0,1.0);											\n\
	vec4 lightDir=normalize(curPixelPosInLocalSpace-u_lightPosInLocalSpace);					\n\
	// 环境光																					\n\
	vec4 ambient=ambientML;																		\n\
	// 漫反射光																					\n\
	vec3 posToLight=-lightDir.xyz;																\n\
	float normDotPosToLight=max(0.0,dot(normal,posToLight));									\n\
	vec4 diffuse=vec4(normDotPosToLight*vec3(diffuseML),1.0);									\n\
	gl_FragColor=texColor*vec4(vec3(diffuse+ambient),diffuse.a);								\n\
}";
//-------------------------------------------------------------------------
FKCW_EffectNode_NormalMappedLightSprite::FKCW_EffectNode_NormalMappedLightSprite()
{
	m_tagDiffuse=ccc4f(1, 1, 1, 1);
	m_tagAmbient=ccc4f(1, 1, 1, 1);
	m_fZ=0;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_NormalMappedLightSprite::~FKCW_EffectNode_NormalMappedLightSprite()
{
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_NormalMappedLightSprite::init( CCTexture2D* p_pTex )
{
	this->CCSprite::initWithTexture( p_pTex );
	return true;
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_NormalMappedLightSprite::init(const string&texFileName)
{
	this->CCSprite::initWithFile(texFileName.c_str());
	return true;
}
//-------------------------------------------------------------------------
ccColor4F FKCW_EffectNode_NormalMappedLightSprite::getDiffuse()const
{
	return m_tagDiffuse;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedLightSprite::setDiffuse(const ccColor4F&diffuse)
{
	m_tagDiffuse=diffuse;
	setColor(ccc3(static_cast<GLubyte>(m_tagDiffuse.r*255),
		static_cast<GLubyte>(m_tagDiffuse.g*255),
		static_cast<GLubyte>(m_tagDiffuse.b*255)));
}
//-------------------------------------------------------------------------
ccColor4F FKCW_EffectNode_NormalMappedLightSprite::getAmbient()const
{
	return m_tagAmbient;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedLightSprite::setAmbient(const ccColor4F&ambient)
{
	m_tagAmbient=ambient;
}
//-------------------------------------------------------------------------
float FKCW_EffectNode_NormalMappedLightSprite::getZ()const
{
	return m_fZ;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedLightSprite::setZ(float z)
{
	m_fZ=z;
}
//-------------------------------------------------------------------------
// CnormalMappedSprite
//-------------------------------------------------------------------------
FKCW_EffectNode_NormalMappedSprite::FKCW_EffectNode_NormalMappedSprite() 
{
	m_program=NULL;
	m_normalMap=NULL;
	m_lightSprite=NULL;
	m_kBump=1.0;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_NormalMappedSprite::~FKCW_EffectNode_NormalMappedSprite()
{
	if(m_program)m_program->release();
	if(m_normalMap)m_normalMap->release();
	if(m_lightSprite)m_lightSprite->release();
};
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedSprite::setLightSprite(FKCW_EffectNode_NormalMappedLightSprite*lightSprite)
{
	assert(lightSprite);
	if(m_lightSprite==NULL)
	{
		m_lightSprite=lightSprite;
		m_lightSprite->retain();
	}
	else
	{
		m_lightSprite->release();
		m_lightSprite=lightSprite;
		m_lightSprite->retain();
	}
}
//-------------------------------------------------------------------------
FKCW_EffectNode_NormalMappedLightSprite* FKCW_EffectNode_NormalMappedSprite::getLightSprite()
{
	return m_lightSprite;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedSprite::setKBump(float value)
{
	m_kBump=value;
}
//-------------------------------------------------------------------------
float FKCW_EffectNode_NormalMappedSprite::getKBump()const
{
	return m_kBump;
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_NormalMappedSprite::init(const string&texFileName,const string&normalMapFileName)
{
	// 初始化精灵
	this->CCSprite::initWithFile(texFileName.c_str());
	// 创建法线贴图
	m_normalMap=CCTextureCache::sharedTextureCache()->addImage(normalMapFileName.c_str());
	m_normalMap->retain();
	// 创建灯光精灵
	{
		FKCW_EffectNode_NormalMappedLightSprite*lightSprite_dft=new FKCW_EffectNode_NormalMappedLightSprite();
		lightSprite_dft->autorelease();
		lightSprite_dft->init( "Engine/NormalMapped_light.png" );
		this->setLightSprite(lightSprite_dft);
		lightSprite_dft->setPosition(ccp(0,0));
		lightSprite_dft->setZ(0);
	}
	// 创建并设置shader
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_NormalMappedFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		// 绑定自定义参数
		program->attachUniform("u_normalMap");
		program->attachUniform("u_diffuseM");
		program->attachUniform("u_ambientM");
		program->attachUniform("u_diffuseL");
		program->attachUniform("u_ambientL");
		program->attachUniform("u_contentSize");
		program->attachUniform("u_lightPosInLocalSpace");
		program->attachUniform("u_kBump");

		m_program=program;
		m_program->retain();

		CHECK_GL_ERROR_DEBUG();
	}
	CHECK_GL_ERROR_DEBUG();
	return true ;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_NormalMappedSprite::draw()
{
	float diffuseM_tmp[4]={m_material.m_tagDiffuse.r,m_material.m_tagDiffuse.g,
		m_material.m_tagDiffuse.b,m_material.m_tagDiffuse.a};
	float ambientM_tmp[4]={m_material.m_tagAmbient.r,m_material.m_tagAmbient.g,
		m_material.m_tagAmbient.b,m_material.m_tagAmbient.a};
	float diffuseL_tmp[4]={m_lightSprite->getDiffuse().r,m_lightSprite->getDiffuse().g,
		m_lightSprite->getDiffuse().b,m_lightSprite->getDiffuse().a};
	float ambientL_tmp[4]={m_lightSprite->getAmbient().r,m_lightSprite->getAmbient().g,
		m_lightSprite->getAmbient().b,m_lightSprite->getAmbient().a};
	float contentSize_tmp[2]={this->getContentSize().width,this->getContentSize().height};
	CCSize contentSize=this->getContentSize();
	CCPoint lightPosInLocalSpace=CCPointApplyAffineTransform(m_lightSprite->getPosition(),this->parentToNodeTransform());

	float lightPosInLocalSpace_tmp[4]={lightPosInLocalSpace.x,lightPosInLocalSpace.y,m_lightSprite->getZ(),1};
	// 更换shader
	ccGLEnable(m_eGLServerState);

	this->setShaderProgram(m_program);
	FKCW_EffectNode_GLProgramWithUnifos* program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
	program->use();
	program->setUniformsForBuiltins();
	// 设置自定义参数
	program->passUnifoValueNfv("u_diffuseM", diffuseM_tmp,4);
	program->passUnifoValueNfv("u_ambientM",ambientM_tmp,4);
	program->passUnifoValueNfv("u_diffuseL",diffuseL_tmp,4);
	program->passUnifoValueNfv("u_ambientL",ambientL_tmp,4);
	program->passUnifoValueNfv("u_contentSize", contentSize_tmp, 2);
	program->passUnifoValueNfv("u_lightPosInLocalSpace",lightPosInLocalSpace_tmp,4);
	program->passUnifoValue1f("u_kBump", m_kBump);
	program->passUnifoValue1i("u_normalMap", 1);
	// 贴入纹理
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap->getName());
	glActiveTexture(GL_TEXTURE0);
	// 父类render
	CCSprite::draw();
}
//-------------------------------------------------------------------------