//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_Material.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_CommonFunc.h"
//-------------------------------------------------------------------------
class FKCW_3D_Material:public CCObject
{
public:
	FKCW_3D_Material();
	virtual~FKCW_3D_Material();
public:
	bool init(){return true;}
	FKCW_3D_Vector4 getAmbient()const {return m_ambient;}
	FKCW_3D_Vector4 getDiffuse()const {return m_diffuse;}
	FKCW_3D_Vector4 getSpecular()const {return m_specular;}
	float getShininess()const {return m_shininess;}
	void setAmbient(FKCW_3D_Vector4 ambient) {m_ambient=ambient;}
	void setDiffuse(FKCW_3D_Vector4 diffuse) {m_diffuse=diffuse;}
	void setSpecular(FKCW_3D_Vector4 specular) {m_specular=specular;}
	void setShininess(float shininess){m_shininess=shininess;}
protected:
	FKCW_3D_Vector4 m_ambient;
	FKCW_3D_Vector4 m_diffuse;
	FKCW_3D_Vector4 m_specular;
	float m_shininess;
};