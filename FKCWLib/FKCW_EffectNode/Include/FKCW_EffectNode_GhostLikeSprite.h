//*************************************************************************
//	创建日期:	2014-11-17
//	文件名称:	FKCW_EffectNode_GhostLikeSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
const float s_fFilterR=16;
const float s_fFilterSrcScale=1.0;
const float s_fDownSample=1.0;
//-------------------------------------------------------------------------
class FKCW_EffectNode_GhostLikeSprite : public CCSprite
{
public:
	FKCW_EffectNode_GhostLikeSprite();
	virtual ~FKCW_EffectNode_GhostLikeSprite();

	bool			init(const string&texFileName,float filterR=s_fFilterR,float fliterSrcScale=s_fFilterSrcScale);
	bool			init(CCSpriteFrame* spriteFrame,float filterR=s_fFilterR,float fliterSrcScale=s_fFilterSrcScale);
	bool			init(CCSprite* sprite,float filterR=s_fFilterR,float fliterSrcScale=s_fFilterSrcScale);
	virtual void	draw() ;
	void			setDownSample(float value){m_downSample=value;}
	float			getDownSample()const{return m_downSample;}
	CCSprite*		getRefSprite(){return m_refSprite;}
protected:
	float m_filterR;
	FKCW_EffectNode_GLProgramWithUnifos*	m_program_filterX;
	FKCW_EffectNode_GLProgramWithUnifos*	m_program_filterY;
	CCRenderTexture*						m_biggerRT;
	CCRenderTexture*						m_filterXRT;
	CCRenderTexture*						m_filterYRT;
	CCSprite*								m_refSprite;
	float									m_fliterSrcScale;
	float									m_downSample;
};