//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_Laser.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
#pragma warning( disable:4305 )
//-------------------------------------------------------------------------
const char* s_LaserFragShader = "																				\n\
#ifdef GL_ES																									\n\
	precision highp float;																						\n\
#endif																											\n\
																												\n\
varying vec4 v_fragmentColor;																					\n\
varying vec2 v_texCoord;																						\n\
uniform sampler2D CC_Texture0;																					\n\
uniform sampler2D u_maskTex;																					\n\
uniform sampler2D u_maskTex2;																					\n\
uniform sampler2D u_noiseTex;																					\n\
uniform sampler2D u_noiseTex2;																					\n\
uniform vec2 u_noiseSize;																						\n\
uniform vec2 u_spriteSize;																						\n\
uniform vec2 u_LUPos;																							\n\
uniform float u_highlight;																						\n\
uniform float u_time;																							\n\
uniform float u_PorN;																							\n\
uniform vec2 u_scale;																							\n\
uniform float u_kLightColor;																					\n\
uniform float u_kGlowColor;																						\n\
uniform float u_noiseScale;																						\n\
																												\n\
void main()																										\n\
{																												\n\
	vec2 texCoord_noise;																						\n\
																												\n\
	texCoord_noise.x = (v_texCoord.x*u_spriteSize.x+u_LUPos.x)/u_noiseSize.x;									\n\
	texCoord_noise.y = (v_texCoord.y*u_spriteSize.y+u_noiseSize.y-u_LUPos.y)/u_noiseSize.y;						\n\
																												\n\
	vec2 texCoord_mask;																							\n\
	texCoord_mask.x=(v_texCoord.x+u_time*0.05)*u_scale.x;														\n\
	texCoord_mask.y=v_texCoord.y;																				\n\
																												\n\
	vec2 texCoord_mask2;																						\n\
	texCoord_mask2.x=(v_texCoord.x+u_time*0.1)*u_scale.x;														\n\
	texCoord_mask2.y=v_texCoord.y;																				\n\
																												\n\
	vec4 maskTexColor=texture2D(u_maskTex, texCoord_mask);														\n\
	vec4 maskTexColor2=texture2D(u_maskTex2, texCoord_mask2);													\n\
																												\n\
	vec4 lightTexColor= texture2D(CC_Texture0, v_texCoord+vec2(u_time*10.0,0.0));								\n\
																												\n\
	vec2 texCoord_noise1=texCoord_noise;																		\n\
	texCoord_noise1*=u_noiseScale;																				\n\
	texCoord_noise1+=vec2(0.0,u_time*0.2*u_PorN);																\n\
																												\n\
	vec2 texCoord_noise2=texCoord_noise;																		\n\
	texCoord_noise2*=u_noiseScale;																				\n\
	texCoord_noise2+=vec2(0.0, u_time*0.4*u_PorN);																\n\
																												\n\
	vec4 noiseTexColor1=texture2D(u_noiseTex, texCoord_noise1);													\n\
	vec4 noiseTexColor2=texture2D(u_noiseTex2, texCoord_noise2);												\n\
																												\n\
	float deep;																									\n\
	if(v_texCoord.y<0.5+0.001)																					\n\
	{																											\n\
		deep=(u_PorN==-1.0)?0.0:v_texCoord.y*v_texCoord.y*1.0/0.25;												\n\
	}																											\n\
	if(v_texCoord.y>=0.5-0.001)																					\n\
	{																											\n\
		deep=(u_PorN==-1.0)?(1.0-v_texCoord.y)*(1.0-v_texCoord.y)*1.0/0.25:0.0;									\n\
	}																											\n\
	float deep2;																								\n\
	vec2 curPoint=v_texCoord*u_spriteSize;																		\n\
	float curveX=u_spriteSize.x-0.05*(curPoint.y-0.5*u_spriteSize.y)*(curPoint.y-0.5*u_spriteSize.y)-10.0;		\n\
	if(curPoint.x<curveX)																						\n\
	{																											\n\
		deep2=1.0;																								\n\
	}else																										\n\
	{																											\n\
		curPoint.x-curveX;																						\n\
		deep2=1.0-1.0/(u_spriteSize.x-curveX)*(curPoint.x-curveX);												\n\
	}																											\n\
	vec4 glowColor =u_kGlowColor*(noiseTexColor1+noiseTexColor2)*(maskTexColor+maskTexColor2)/2.0*u_highlight;	\n\
	vec4 lightColor =lightTexColor*u_kLightColor;																\n\
	vec4 finalColor = glowColor*vec4(1.0,0.5,0.0,1.0)+lightColor;												\n\
																												\n\
	float Gray = finalColor.r*0.299 + finalColor.g*0.587 + finalColor.b*0.114;									\n\
																												\n\
	finalColor.a=Gray*2.0;																						\n\
	finalColor*=deep*deep2;																						\n\
	gl_FragColor=finalColor;																					\n\
}";
//-------------------------------------------------------------------------
FKCW_EffecrNode_HalfLaser::FKCW_EffecrNode_HalfLaser()
{
	m_program=NULL;
	m_maskTex=NULL;
	m_maskTex2=NULL;
	m_noiseTex=NULL;
	m_noiseTex2=NULL;
	m_highlight=1.0;
	m_time=0;
	m_PorN=1;
	m_kLightColor=1.0;
	m_kGlowColor=1.0;
	m_noiseScale=1.0;
}
//-------------------------------------------------------------------------
FKCW_EffecrNode_HalfLaser::~FKCW_EffecrNode_HalfLaser()
{
	if(m_program)m_program->release();
	if(m_maskTex)m_maskTex->release();
	if(m_maskTex2)m_maskTex2->release();
	if(m_noiseTex)m_noiseTex->release();
	if(m_noiseTex2)m_noiseTex2->release();
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::updateStartAndEnd()
{
	CCPoint mid=ccpMult(m_start+m_end, 0.5) ;
	CCPoint dir=ccpNormalize(m_end-m_start);
	float A;
	if(dir.y<=0)
	{
		A=acosf(dir.x)*180/M_PI;
	}
	else
	{
		A=(2*M_PI-acosf(dir.x))*180/M_PI;
	}
	this->setPosition(mid);
	this->setRotation(A);

	float targetLen=ccpDistance(m_start, m_end);
	float contentLen=this->getContentSize().width;
	float scaleX=targetLen/contentLen;
	this->setScaleX(scaleX);
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::setStart(const CCPoint&start)
{
	m_start=start;
	updateStartAndEnd();
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::setEnd(const CCPoint&end)
{
	m_end=end;
	updateStartAndEnd();
}
//-------------------------------------------------------------------------
bool FKCW_EffecrNode_HalfLaser::init()
{
	this->CCSprite::initWithFile( "Engine/Laser_light2.png" );
	{
		ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		this->getTexture()->setTexParameters(&texParams);
	}
	//maskTex
	m_maskTex=CCTextureCache::sharedTextureCache()->addImage( "Engine/Laser_mask.png" );
	m_maskTex->retain();
	{
		ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		m_maskTex->setTexParameters(&texParams);
	}
	//maskTex2
	m_maskTex2=CCTextureCache::sharedTextureCache()->addImage( "Engine/Laser_mask2.png" );
	m_maskTex2->retain();
	{
		ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		m_maskTex2->setTexParameters(&texParams);
	}

	//noiseTex
	m_noiseTex=CCTextureCache::sharedTextureCache()->addImage( "Engine/Laser_noise.png" );
	m_noiseTex->retain();
	{
		ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		m_noiseTex->setTexParameters(&texParams);
	}
	//noiseTex2
	m_noiseTex2=CCTextureCache::sharedTextureCache()->addImage( "Engine/Laser_noise2.png" );
	m_noiseTex2->retain();
	{
		ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		m_noiseTex2->setTexParameters(&texParams);
	}

	// 创建并设置shaderProgram
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_LaserFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		// 设置自定义属性
		program->attachUniform("u_maskTex");
		program->attachUniform("u_maskTex2");
		program->attachUniform("u_noiseTex");
		program->attachUniform("u_noiseTex2");
		program->attachUniform("u_spriteSize");
		program->attachUniform("u_LUPos");
		program->attachUniform("u_noiseSize");
		program->attachUniform("u_highlight");
		program->attachUniform("u_time");
		program->attachUniform("u_PorN");
		program->attachUniform("u_scale");
		program->attachUniform("u_kLightColor");
		program->attachUniform("u_kGlowColor");
		program->attachUniform("u_noiseScale");
		m_program=program;
		m_program->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	// 开启帧更新
	this->scheduleUpdate();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::setBlendFunc(GLenum src,GLenum dst)
{
	ccBlendFunc blendFunc={src,dst};
	this->CCSprite::setBlendFunc(blendFunc);
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::update(float dt)
{
	m_time+=dt;
	if(m_time>=10000)m_time=0;
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_HalfLaser::draw()
{
	float spriteSize_tmp[2]={this->getContentSize().width*this->getScaleX(),
		this->getContentSize().height*this->getScaleY()};// 因为精灵可能被缩放，所以这里使用contentSize*scale替代boundingBox().size
	float LUPos_tmp[2]={0,0};
	float noiseSize_tmp[2]={m_noiseTex->getContentSize().width,m_noiseTex->getContentSize().height};
	float scale_tmp[2]={this->getScaleX(),this->getScaleY()};
	// 更换shader
	ccGLEnable(m_eGLServerState);
	this->setShaderProgram(m_program);
	FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
	program->use();
	program->setUniformsForBuiltins();
	// 设置自定义值
	program->passUnifoValue1i("u_maskTex", 1);
	program->passUnifoValue1i("u_noiseTex", 2);
	program->passUnifoValue1i("u_noiseTex2", 4);
	program->passUnifoValue1i("u_maskTex2", 3);
	program->passUnifoValueNfv("u_spriteSize", spriteSize_tmp, 2);
	program->passUnifoValueNfv("u_LUPos", LUPos_tmp, 2);
	program->passUnifoValueNfv("u_noiseSize", noiseSize_tmp, 2);
	program->passUnifoValue1f("u_highlight", m_highlight);
	program->passUnifoValue1f("u_time", m_time);
	program->passUnifoValue1f("u_PorN", m_PorN);
	program->passUnifoValueNfv("u_scale", scale_tmp, 2);
	program->passUnifoValue1f("u_kLightColor", m_kLightColor);
	program->passUnifoValue1f("u_kGlowColor", m_kGlowColor);
	program->passUnifoValue1f("u_noiseScale", m_noiseScale);
	// 粘贴纹理
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_maskTex->getName());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_noiseTex->getName());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_maskTex2->getName());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_noiseTex2->getName());
	glActiveTexture(GL_TEXTURE0);
	// 调用父类draw
	CCSprite::draw();
}
//-------------------------------------------------------------------------
// FKCW_EffecrNode_Laser
//-------------------------------------------------------------------------
FKCW_EffecrNode_Laser::FKCW_EffecrNode_Laser()
{
	m_halfLaser1=NULL;
	m_halfLaser2=NULL;
	m_slimHalfLaser1=NULL;
	m_slimHalfLaser2=NULL;
}
//-------------------------------------------------------------------------
bool FKCW_EffecrNode_Laser::init()
{
	//halfLaser1
	m_halfLaser1=new FKCW_EffecrNode_HalfLaser();
	m_halfLaser1->autorelease();
	m_halfLaser1->init();
	m_halfLaser1->setPorN(1);
	m_halfLaser1->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_halfLaser1->setScaleY(1.3);
	m_halfLaser1->m_kGlowColor=1.6;
	m_halfLaser1->m_kLightColor=0.0;
	m_halfLaser1->m_noiseScale=2.2;
	addChild(m_halfLaser1);
	//halfLaser2
	m_halfLaser2=new FKCW_EffecrNode_HalfLaser();
	m_halfLaser2->autorelease();
	m_halfLaser2->init();
	m_halfLaser2->setPorN(-1);
	m_halfLaser2->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_halfLaser2->setScaleY(1.3);
	m_halfLaser2->m_kGlowColor=1.6;
	m_halfLaser2->m_kLightColor=0.0;
	m_halfLaser2->m_noiseScale=2.2;
	addChild(m_halfLaser2);
	//slimHalfLaser1
	m_slimHalfLaser1=new FKCW_EffecrNode_HalfLaser();
	m_slimHalfLaser1->autorelease();
	m_slimHalfLaser1->init();
	m_slimHalfLaser1->setPorN(1);
	m_slimHalfLaser1->setBlendFunc(GL_ONE, GL_ONE);
	m_slimHalfLaser1->setScaleY(0.4);
	m_slimHalfLaser1->m_kGlowColor=2.5;
	m_slimHalfLaser1->m_kLightColor=0.5;
	m_slimHalfLaser1->m_noiseScale=1.5;
	addChild(m_slimHalfLaser1);
	//slimHalfLaser2
	m_slimHalfLaser2=new FKCW_EffecrNode_HalfLaser();
	m_slimHalfLaser2->autorelease();
	m_slimHalfLaser2->init();
	m_slimHalfLaser2->setPorN(-1);
	m_slimHalfLaser2->setBlendFunc(GL_ONE, GL_ONE);
	m_slimHalfLaser2->setScaleY(0.4);
	m_slimHalfLaser2->m_kGlowColor=2.5;
	m_slimHalfLaser2->m_kLightColor=0.5;
	m_slimHalfLaser2->m_noiseScale=1.5;
	addChild(m_slimHalfLaser2);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_Laser::setStart(const CCPoint&start)
{
	m_start=start;
	m_halfLaser1->setStart(m_start);
	m_halfLaser2->setStart(m_start);
	m_slimHalfLaser1->setStart(m_start);
	m_slimHalfLaser2->setStart(m_start);
}
//-------------------------------------------------------------------------
void FKCW_EffecrNode_Laser::setEnd(const CCPoint&end)
{
	m_end=end;
	m_halfLaser1->setEnd(m_end);
	m_halfLaser2->setEnd(m_end);
	m_slimHalfLaser1->setEnd(m_end);
	m_slimHalfLaser2->setEnd(m_end);
}
//-------------------------------------------------------------------------