//*************************************************************************
//	创建日期:	2014-11-17
//	文件名称:	FKCW_EffectNode_ColorAdjustSprite.h
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
class FKCW_EffectNode_ColorAdjustSprite : public CCSprite
{
public:
	FKCW_EffectNode_ColorAdjustSprite();
	virtual ~FKCW_EffectNode_ColorAdjustSprite();

	bool init(const string&texFileName) ;
	virtual void draw() ;

protected:
	FKCW_EffectNode_GLProgramWithUnifos*	m_pAdjustColorHSL;
	CC_SYNTHESIZE( float, m_dH, DH );
	CC_SYNTHESIZE( float, m_dS, DS );
	CC_SYNTHESIZE( float, m_dL, DL );
};