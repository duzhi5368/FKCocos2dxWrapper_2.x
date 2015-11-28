//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_GhostLikeSprite.h"
//-------------------------------------------------------------------------
#pragma warning ( disable:4244 )
//-------------------------------------------------------------------------
const char* s_GhostlikeFilterXFragShader = "					\n\
	#ifdef GL_ES												\n\
		precision mediump float;								\n\
	#endif														\n\
																\n\
	varying vec2 v_texCoord;									\n\
	uniform sampler2D CC_Texture0;//m_biggerRT纹理				\n\
	uniform float u_filterR;									\n\
	uniform float u_ds;											\n\
	uniform float u_s_step;										\n\
	void main() {												\n\
	float glowAlpha;											\n\
	{															\n\
		float s_cur=v_texCoord.x;								\n\
		float t_cur=v_texCoord.y;								\n\
		float s_halfWidth=u_filterR*u_ds;						\n\
		float s_begin=s_cur-s_halfWidth;						\n\
		float s_end=s_cur+s_halfWidth;							\n\
		float alpha=0.0;										\n\
		int count=0;											\n\
		for(float s=s_begin;s<=s_end;s+=u_s_step){				\n\
			alpha+=texture2D(CC_Texture0, vec2(s,t_cur)).a;		\n\
			count+=1;											\n\
		}														\n\
		alpha/=float(count);									\n\
		glowAlpha=alpha;										\n\
	}															\n\
	vec4 finalColor=vec4(vec3(1.0),glowAlpha);					\n\
	gl_FragColor=finalColor;									\n\
}";
//-------------------------------------------------------------------------
const char* s_GhostlikeFilterYAndMergeFragShader = "													\n\
#ifdef GL_ES																							\n\
precision mediump float;																				\n\
#endif																									\n\
																										\n\
varying vec2 v_texCoord;																				\n\
uniform sampler2D CC_Texture0;//m_filterXRT纹理															\n\
uniform sampler2D u_refTex;//m_refSprite纹理															\n\
uniform float u_filterR;																				\n\
uniform float u_dt;																						\n\
uniform float u_t_step;																					\n\
void main() {																							\n\
    vec4 refTexColor=texture2D(u_refTex,v_texCoord);													\n\
    float glowAlpha;																					\n\
    {																									\n\
        float s_cur=v_texCoord.x;																		\n\
        float t_cur=v_texCoord.y;																		\n\
        float t_halfHeight=u_filterR*u_dt;																\n\
        float t_begin=t_cur-t_halfHeight;																\n\
        float t_end=t_cur+t_halfHeight;																	\n\
        float alpha=0.0;																				\n\
        int count=0;																					\n\
        for(float t=t_begin;t<=t_end;t+=u_t_step){														\n\
            alpha+=texture2D(CC_Texture0, vec2(s_cur,t)).a;												\n\
            count+=1;																					\n\
        }																								\n\
        alpha/=float(count);																			\n\
        glowAlpha=alpha;																				\n\
        glowAlpha+=refTexColor.a;																		\n\
    }																									\n\
    float ghostAlpha;																					\n\
    {																									\n\
        float grey = dot(refTexColor.rgb, vec3(0.299, 0.587, 0.114));									\n\
        ghostAlpha=grey*refTexColor.a;																	\n\
    }																									\n\
    vec4 finalColor=vec4(vec3(1.0),glowAlpha*(1.0-refTexColor.a)+ghostAlpha);							\n\
	/* 若你需要让影子有一种淡蓝色，解除下一行注释即可。													\n\
    // finalColor=vec4(finalColor.rgb*finalColor.a+(1.0-finalColor.a)*vec3(0.5,1.0,1.0),finalColor.a);	\n\
	*/																									\n\
    gl_FragColor=finalColor;																			\n\
}";
//-------------------------------------------------------------------------
FKCW_EffectNode_GhostLikeSprite::FKCW_EffectNode_GhostLikeSprite()
{
	m_program_filterX=NULL;
	m_program_filterY=NULL;
	m_biggerRT=NULL;
	m_filterXRT=NULL;
	m_filterYRT=NULL;
	m_refSprite=NULL;
	m_filterR=s_fFilterR;
	m_fliterSrcScale=s_fFilterSrcScale;
	m_downSample=s_fDownSample;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_GhostLikeSprite::~FKCW_EffectNode_GhostLikeSprite()
{
	if(m_program_filterX)m_program_filterX->release();
	if(m_program_filterY)m_program_filterY->release();
	if(m_biggerRT)m_biggerRT->release();
	if(m_filterXRT)m_filterXRT->release();
	if(m_filterYRT)m_filterYRT->release();
};
//-------------------------------------------------------------------------
bool FKCW_EffectNode_GhostLikeSprite::init(CCSpriteFrame* spriteFrame,float filterR,float fliterSrcScale)
{
	CCSprite*sprite=CCSprite::create();
	sprite->initWithSpriteFrame(spriteFrame);
	this->init(sprite);
	return true ;
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_GhostLikeSprite::init(const string&texFileName,float filterR,float fliterSrcScale)
{
	CCSprite*sprite=CCSprite::create();
	sprite->initWithFile(texFileName.c_str());
	this->init(sprite);
	return true ;
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_GhostLikeSprite::init(CCSprite* sprite,float filterR,float fliterSrcScale)
{
	// 混合函数
	ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
	this->setBlendFunc(blendFunc);
	m_filterR=filterR;
	m_refSprite=sprite;
	// 计算更大的大小
	CCSize biggerSize;
	{
		CCSize contentSize=m_refSprite->getContentSize();
		float margin=m_filterR*3;		// 乘以三倍大小
		biggerSize=CCSize(contentSize.width+margin*2, contentSize.height+margin*2);
	}
	// 开始初始化精灵
	this->CCSprite::init();
	// 使用大一号的纹理
	this->setTextureRect(CCRect(0,0,biggerSize.width,biggerSize.height));
	// 添加一个镜像子精灵
	this->addChild(m_refSprite);
	m_refSprite->setVisible(false);
	// m_biggerRT
	m_biggerRT=CCRenderTexture::create(biggerSize.width,biggerSize.height);
	m_biggerRT->retain();
	// m_filterXRT
	m_filterXRT=CCRenderTexture::create(biggerSize.width*m_fliterSrcScale,biggerSize.height*m_fliterSrcScale);
	m_filterXRT->retain();
	// m_filterYRT
	m_filterYRT=CCRenderTexture::create(biggerSize.width,biggerSize.height);
	m_filterYRT->retain();
	// FilterX shader
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_GhostlikeFilterXFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		// 绑定自定义参数
		program->attachUniform("u_filterR");
		program->attachUniform("u_ds");
		program->attachUniform("u_s_step");
		m_program_filterX=program;
		m_program_filterX->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	// FilterY shader
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_GhostlikeFilterYAndMergeFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		// 绑定自定义参数
		program->attachUniform("u_filterR");
		program->attachUniform("u_dt");
		program->attachUniform("u_t_step");
		program->attachUniform("u_refTex");
		m_program_filterY=program;
		m_program_filterY->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	CHECK_GL_ERROR_DEBUG();

	// 抗锯齿
	this->m_filterYRT->getSprite()->getTexture()->setAntiAliasTexParameters();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_GhostLikeSprite::draw()
{
	// 绘制m_originTexSprite到m_biggerRT纹理上(使用m_program_renderAlpha)
	{
		m_biggerRT->beginWithClear(0.0,0.0,0.0,0.0);
		{
			// 保存渲染数据
			CCPoint oldAnchorPoint=m_refSprite->getAnchorPoint();
			CCPoint oldPosition=m_refSprite->getPosition();
			float oldScaleX=m_refSprite->getScaleX();
			float oldScaleY=m_refSprite->getScaleY();
			bool oldIsVisible=m_refSprite->isVisible();
			// 临时修改
			m_refSprite->setVisible(true);
			m_refSprite->setScaleY(-1);
			m_refSprite->setScaleX(1);
			m_refSprite->setAnchorPoint(ccp(0.5,0.5));
			m_refSprite->setPosition(ccp(m_biggerRT->getSprite()->getContentSize().width/2,m_biggerRT->getSprite()->getContentSize().height/2));
			m_refSprite->visit();
			// 还原原数据
			m_refSprite->setAnchorPoint(oldAnchorPoint);
			m_refSprite->setPosition(oldPosition);
			m_refSprite->setScaleX(oldScaleX);
			m_refSprite->setScaleY(oldScaleY);
			m_refSprite->setVisible(oldIsVisible);
		}
		m_biggerRT->end();
	}

	// 将m_biggerRT绘制到m_filterXRT上（使用m_program_fliterX）
	{
		// m_biggerRT 更换shader
		ccGLEnable(m_eGLServerState);
		m_biggerRT->getSprite()->setShaderProgram(m_program_filterX);
		FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)m_biggerRT->getSprite()->getShaderProgram();
		program->use();
		program->setUniformsForBuiltins();
		// 预设一些自定义值
		float ds_tmp=1.0/m_biggerRT->getSprite()->getContentSize().width;
		program->passUnifoValue1f("u_filterR", m_filterR*m_fliterSrcScale);
		program->passUnifoValue1f("u_ds", ds_tmp);
		program->passUnifoValue1f("u_s_step", ds_tmp/m_downSample);
		// 将m_biggerRT绘制到m_filterXRT上
		m_filterXRT->beginWithClear(0.0,0.0,0.0,0.0);
		{
			// 保存信息
			CCPoint oldAnchorPoint=m_biggerRT->getAnchorPoint();
			CCPoint oldPosition=m_biggerRT->getPosition();
			float oldScaleX=m_biggerRT->getScaleX();
			float oldScaleY=m_biggerRT->getScaleY();
			bool oldIsVisible=m_biggerRT->isVisible();
			// 临时改变渲染
			m_biggerRT->getSprite()->setScaleX(m_fliterSrcScale);
			m_biggerRT->getSprite()->setScaleY(-m_fliterSrcScale);
			m_biggerRT->getSprite()->setAnchorPoint(ccp(0.5,0.5));
			m_biggerRT->getSprite()->setPosition(ccp(m_filterXRT->getSprite()->getContentSize().width/2,m_filterXRT->getSprite()->getContentSize().height/2));
			m_biggerRT->getSprite()->visit();
			// 复位信息
			m_biggerRT->setAnchorPoint(oldAnchorPoint);
			m_biggerRT->setPosition(oldPosition);
			m_biggerRT->setScaleX(oldScaleX);
			m_biggerRT->setScaleY(oldScaleY);
			m_biggerRT->setVisible(oldIsVisible);
		}
		m_filterXRT->end();
	}

	// 绘制m_filterXRT 到 m_filterYRT 纹理上 (使用 m_program_fliterY)
	{
		// 更变shader
		ccGLEnable(m_eGLServerState);
		m_filterXRT->getSprite()->setShaderProgram(m_program_filterY);
		FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)m_filterXRT->getSprite()->getShaderProgram();
		program->use();
		program->setUniformsForBuiltins();
		// 预设一些自定义值
		float dt_tmp=1.0/m_filterXRT->getSprite()->getContentSize().height;
		program->passUnifoValue1f("u_filterR", m_filterR*m_fliterSrcScale);
		program->passUnifoValue1f("u_dt", dt_tmp);
		program->passUnifoValue1f("u_t_step", dt_tmp/m_downSample);
		program->passUnifoValue1i("u_refTex", 1);
		// 将纹理贴上
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_biggerRT->getSprite()->getTexture()->getName());
		glActiveTexture(GL_TEXTURE0);
		// 绘制m_filterXRT 到 m_filterYRT 纹理上
		m_filterYRT->beginWithClear(0.0,0.0,0.0,0.0);
		{
			m_filterXRT->getSprite()->setScaleX(1.0/m_fliterSrcScale);
			m_filterXRT->getSprite()->setScaleY(-1.0/m_fliterSrcScale);
			m_filterXRT->getSprite()->setAnchorPoint(ccp(0.5,0.5));
			m_filterXRT->getSprite()->setPosition(ccp(m_filterYRT->getSprite()->getContentSize().width/2,m_filterYRT->getSprite()->getContentSize().height/2));
			m_filterXRT->getSprite()->visit();
		}
		m_filterYRT->end();
	}

	// 将最终纹理贴到精灵上
	this->setTexture(m_filterYRT->getSprite()->getTexture());
	this->setTextureRect(m_filterYRT->getSprite()->getTextureRect());
	// 调用父类的draw
	this->CCSprite::draw();
}
//-------------------------------------------------------------------------