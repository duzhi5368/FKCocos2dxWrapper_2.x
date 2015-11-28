//*************************************************************************
//	创建日期:	2014-11-18
//	文件名称:	FKCW_EffectNode_NormalMappedSprite.h
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
class FKCW_EffectNode_NormalMappedLightSprite : public CCSprite
{
public:
	FKCW_EffectNode_NormalMappedLightSprite();
	virtual~FKCW_EffectNode_NormalMappedLightSprite();

	bool		init( CCTexture2D* p_pTex );
	bool		init(const string&texFileName);
	ccColor4F	getDiffuse() const;
	void		setDiffuse(const ccColor4F&diffuse);
	ccColor4F	getAmbient() const;
	void		setAmbient(const ccColor4F&ambient);
	float		getZ()const;
	void		setZ(float z);
protected:
	ccColor4F	m_tagDiffuse;
	ccColor4F	m_tagAmbient;
	float		m_fZ;
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_NormalMappedMaterial
{
public:
	ccColor4F m_tagDiffuse;
	ccColor4F m_tagAmbient;
	FKCW_EffectNode_NormalMappedMaterial()
	{
		m_tagDiffuse=ccc4f(1, 1, 1, 1);
		m_tagAmbient=ccc4f(0.5, 0.5, 0.5, 1);
	}
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_NormalMappedSprite : public CCSprite
{
public:
	FKCW_EffectNode_NormalMappedSprite();
	virtual ~FKCW_EffectNode_NormalMappedSprite();

	bool				init(const string&texFileName,const string&normalMapFileName) ;
	virtual void		draw();

	void				setLightSprite(FKCW_EffectNode_NormalMappedLightSprite*lightSprite);
	FKCW_EffectNode_NormalMappedLightSprite* getLightSprite();
	void				setKBump(float value);
	float				getKBump()const;
protected:
	FKCW_EffectNode_GLProgramWithUnifos*		m_program;
	CCTexture2D*								m_normalMap;
	FKCW_EffectNode_NormalMappedLightSprite*	m_lightSprite;
	FKCW_EffectNode_NormalMappedMaterial		m_material;
	float										m_kBump;
};
//-------------------------------------------------------------------------