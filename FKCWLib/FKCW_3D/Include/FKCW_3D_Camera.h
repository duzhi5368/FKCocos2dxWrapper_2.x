//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_Camera.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_Matrix4.h"
#include "FKCW_3D_GLMath.h"
#include "FKCW_3D_CommonFunc.h"
#include "FKCW_3D_MatStackInfoGetor.h"
#include "FKCW_3D_Range.h"
//-------------------------------------------------------------------------
class FKCW_3D_Camera:public CCCamera
{
public:
	FKCW_3D_Camera();
	virtual ~FKCW_3D_Camera();
public:
	FKCW_3D_Vector4 getEyePos()const;
	FKCW_3D_Vector4 getCenter()const;
	FKCW_3D_Vector4 getUp()const;
	void setEyePos(const FKCW_3D_Vector4&eyePos);
	void setCenter(const FKCW_3D_Vector4&center);
	void setUp(const FKCW_3D_Vector4&up);
	float getFovy()const {return m_fovy;}
	float getAspect()const {return m_aspect;}
	float getzNear()const {return m_zNear;}
	float getzFar()const {return m_zFar;}
	void setFovy(float fovy){m_fovy=fovy;}
	void setAspect(float aspect){m_aspect=aspect;}
	void setzNear(float zNear){m_zNear=zNear;}
	void setzFar(float zFar){m_zFar=zFar;}
	FKCW_3D_Range getRange()const {return m_range;}
	void setRange(const FKCW_3D_Range&range){m_range=range;}
	FKCW_3D_Matrix4 calculateViewMat();
	FKCW_3D_Matrix4 calculateViewMatInverse();
	FKCW_3D_Matrix4 calculateProjectionMat();
	ENUM_FKCW_3D_ProjectionMode getProjectionMode(){return m_projectionMode;}
	void setProjectionMode(ENUM_FKCW_3D_ProjectionMode projectionMode){m_projectionMode=projectionMode;}
	void applyProjection();
	void printProjectionMode()const;
protected:
	ENUM_FKCW_3D_ProjectionMode m_projectionMode;
	float			m_fovy;
	float			m_aspect;
	float			m_zNear;
	float			m_zFar;
	FKCW_3D_Range	m_range;		// 摄像机空间范围
protected:
	FKCW_3D_Matrix4	m_viewMatCache;
	bool				m_isViewMatDirty;
	FKCW_3D_Matrix4	m_viewMatInverseCache;
	bool				m_isViewMatInverseDirty;
};