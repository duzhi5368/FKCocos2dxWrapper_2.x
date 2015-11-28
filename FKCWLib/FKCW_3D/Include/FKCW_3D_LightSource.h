//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_LightSource.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_Node.h"
//-------------------------------------------------------------------------
class FKCW_3D_LightSource:public FKCW_3D_Node
{
public:
	FKCW_3D_LightSource();
	~FKCW_3D_LightSource();

	FKCW_3D_Vector4 getAmbient()const {return m_ambient;}
	FKCW_3D_Vector4 getDiffuse()const {return m_diffuse;}
	FKCW_3D_Vector4 getSpecular()const {return m_specular;}
	void setAmbient(FKCW_3D_Vector4 ambient) {m_ambient=ambient;}
	void setDiffuse(FKCW_3D_Vector4 diffuse) {m_diffuse=diffuse;}
	void setSpecular(FKCW_3D_Vector4 specular) {m_specular=specular;}
protected:
	FKCW_3D_Vector4 m_ambient;
	FKCW_3D_Vector4 m_diffuse;
	FKCW_3D_Vector4 m_specular;
};
//-------------------------------------------------------------------------