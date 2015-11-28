//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_Range.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
//-------------------------------------------------------------------------
class FKCW_3D_Range
{
public:
	FKCW_3D_Range();
	FKCW_3D_Range(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax);
	virtual~FKCW_3D_Range();
public:
	bool init(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax);
	float getMinX()const{return m_xmin;}
	float getMaxX()const{return m_xmax;}
	float getMinY()const{return m_ymin;}
	float getMaxY()const{return m_ymax;}
	float getMinZ()const{return m_zmin;}
	float getMaxZ()const{return m_zmax;}
	float getSpanX()const{return m_xmax-m_xmin;}
	float getSpanY()const{return m_ymax-m_ymin;}
	float getSpanZ()const{return m_zmax-m_zmin;}
protected:
	void _InitMembers();
protected:
	float m_xmin,m_xmax;
	float m_ymin,m_ymax;
	float m_zmin,m_zmax;
};