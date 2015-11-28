//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_ColorAdjustSprite.h"
//-------------------------------------------------------------------------
const char* s_ColorAdjustHSLFragShader = "															\n\
										 #ifdef GL_ES												\n\
										 precision mediump float;									\n\
										 #endif														\n\
										 \n\
										 varying vec2 v_texCoord;									\n\
										 \n\
										 uniform sampler2D CC_Texture0;								\n\
										 uniform float u_dH;										\n\
										 uniform float u_dS;										\n\
										 uniform float u_dL;										\n\
										\n\
										 void main() 												\n\
										 {															\n\
											vec4 texColor=texture2D(CC_Texture0, v_texCoord);		\n\
											float r=texColor.r;										\n\
											float g=texColor.g;										\n\
											float b=texColor.b;										\n\
											float a=texColor.a;										\n\
											// 转换RGB为HSL											\n\
											float h;												\n\
											float s;												\n\
											float l;												\n\
											{														\n\
												float max=max(max(r,g),b);							\n\
												float min=min(min(r,g),b);							\n\
																									\n\
												if(max==min){										\n\
													h=0.0;											\n\
												}else if(max==r&&g>=b){								\n\
													h=60.0*(g-b)/(max-min)+0.0;						\n\
												}else if(max==r&&g<b){								\n\
													h=60.0*(g-b)/(max-min)+360.0;					\n\
												}else if(max==g){									\n\
													h=60.0*(b-r)/(max-min)+120.0;					\n\
												}else if(max==b){									\n\
													h=60.0*(r-g)/(max-min)+240.0;					\n\
												}													\n\
																									\n\
												l=0.5*(max+min);									\n\
																									\n\
												if(l==0.0||max==min){								\n\
													s=0.0;											\n\
												 }else if(0.0<=l&&l<=0.5){							\n\
													s=(max-min)/(2.0*l);							\n\
												}else if(l>0.5){									\n\
													s=(max-min)/(2.0-2.0*l);						\n\
												}													\n\
											}														\n\
											//加入自定义调整色(h,s,l)+(dH,dS,dL) -> (h,s,l)			\n\
											h=h+u_dH;												\n\
											s=min(1.0,max(0.0,s+u_dS));								\n\
											l=l;													\n\
											// 不要使用HSL中的L去调整光亮度，效果很差。				\n\
											// 直接将HSL转换为RGB的最终颜色							\n\
											vec4 finalColor;										\n\
											{														\n\
												float q;											\n\
												if(l<0.5){											\n\
													q=l*(1.0+s);									\n\
												}else if(l>=0.5){									\n\
													q=l+s-l*s;										\n\
												}													\n\
												float p=2.0*l-q;									\n\
												float hk=h/360.0;									\n\
												float t[3];											\n\
												t[0]=hk+1.0/3.0;t[1]=hk;t[2]=hk-1.0/3.0;			\n\
												for(int i=0;i<3;i++){								\n\
													if(t[i]<0.0)t[i]+=1.0;							\n\
													if(t[i]>1.0)t[i]-=1.0;							\n\
												}													\n\
												float c[3];											\n\
												for(int i=0;i<3;i++){								\n\
													if(t[i]<1.0/6.0){								\n\
														c[i]=p+((q-p)*6.0*t[i]);					\n\
													}else if(1.0/6.0<=t[i]&&t[i]<0.5){				\n\
														c[i]=q;										\n\
													}else if(0.5<=t[i]&&t[i]<2.0/3.0){				\n\
														c[i]=p+((q-p)*6.0*(2.0/3.0-t[i]));			\n\
													}else{											\n\
														c[i]=p;										\n\
													}												\n\
												}													\n\
												finalColor=vec4(c[0],c[1],c[2],a);					\n\
											}														\n\
											// 最终我们在这里进行光亮度的调整。						\n\
											finalColor+=vec4(u_dL,u_dL,u_dL,0.0);					\n\
																									\n\
											gl_FragColor=finalColor;								\n\
										}";
//-------------------------------------------------------------------------
FKCW_EffectNode_ColorAdjustSprite::FKCW_EffectNode_ColorAdjustSprite() 
{
	m_pAdjustColorHSL=NULL;
	m_dH=0;
	m_dS=0;
	m_dL=0;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_ColorAdjustSprite::~FKCW_EffectNode_ColorAdjustSprite()
{
	if(m_pAdjustColorHSL)
		m_pAdjustColorHSL->release();
};
//-------------------------------------------------------------------------
bool FKCW_EffectNode_ColorAdjustSprite::init(const string&texFileName)
{
	// 初始化本精灵
	this->CCSprite::initWithFile(texFileName.c_str());
	// 混合函数
	ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
	this->setBlendFunc(blendFunc);

	// 创建并设置shader Program
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_ColorAdjustHSLFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		// 设置自己的参数
		program->attachUniform("u_dH");
		program->attachUniform("u_dS");
		program->attachUniform("u_dL");
		m_pAdjustColorHSL=program;
		m_pAdjustColorHSL->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	CHECK_GL_ERROR_DEBUG();
	return true ;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_ColorAdjustSprite::draw()
{
	// 更换shader
	ccGLEnable(m_eGLServerState);
	this->setShaderProgram(m_pAdjustColorHSL);
	FKCW_EffectNode_GLProgramWithUnifos* program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
	program->use();
	program->setUniformsForBuiltins();
	// 设置自己的参数
	program->passUnifoValue1f("u_dH", m_dH);
	program->passUnifoValue1f("u_dS", m_dS);
	program->passUnifoValue1f("u_dL", m_dL);

	// 调用父类CCSprite::draw
	CCSprite::draw();
}
//-------------------------------------------------------------------------