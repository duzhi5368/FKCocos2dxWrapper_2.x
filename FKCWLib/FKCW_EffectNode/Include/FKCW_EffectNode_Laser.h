//*************************************************************************
//	创建日期:	2014-11-17
//	文件名称:	FKCW_EffectNode_Laser.h
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
class FKCW_EffecrNode_HalfLaser : public CCSprite
{
public:
	FKCW_EffecrNode_HalfLaser();
	virtual~FKCW_EffecrNode_HalfLaser();

	void setBlendFunc(GLenum src,GLenum dst);
	void updateStartAndEnd();
	void setStart(const CCPoint&start);
	void setEnd(const CCPoint&end);
	bool init();
	void draw();
	void update(float dt);
	void setPorN(float value){m_PorN=value;}
public:
	FKCW_EffectNode_GLProgramWithUnifos*	m_program;
	CCTexture2D*							m_maskTex;
	CCTexture2D*							m_maskTex2;
	CCTexture2D*							m_noiseTex;
	CCTexture2D*							m_noiseTex2;
	float									m_highlight;
	float									m_time;
	float									m_PorN;
	CCPoint									m_start;
	CCPoint									m_end;
	float									m_kLightColor;
	float									m_kGlowColor;
	float									m_noiseScale;
};
//-------------------------------------------------------------------------
class FKCW_EffecrNode_Laser:public CCNode
{
public:
	FKCW_EffecrNode_Laser();

	bool init();
	void setStart(const CCPoint&start);
	void setEnd(const CCPoint&end);
private:
	FKCW_EffecrNode_HalfLaser*	m_halfLaser1;
	FKCW_EffecrNode_HalfLaser*	m_halfLaser2;
	FKCW_EffecrNode_HalfLaser*	m_slimHalfLaser1;
	FKCW_EffecrNode_HalfLaser*	m_slimHalfLaser2;
	CCPoint						m_start;
	CCPoint						m_end;
};