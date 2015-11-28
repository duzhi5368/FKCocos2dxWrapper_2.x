/********************************************************************
	created:	2014/12/12
	file base:	FKCW_3D_Math
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_Matrix4.h"
//--------------------------------------------------------------------
FKCW_3D_Matrix4	unitMat();
FKCW_3D_Matrix4	zeroMat();
FKCW_3D_Vector4	normalize(const FKCW_3D_Vector4&v);
FKCW_3D_Vector4	cross(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2);
float				dot(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2);
bool				isUnitMat(const FKCW_3D_Matrix4&mat);
bool				isEqual(const FKCW_3D_Matrix4&mat1,const FKCW_3D_Matrix4&mat2,float eps=0);
bool				isEqual(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2,float eps=0);
FKCW_3D_Matrix4	inverse(const FKCW_3D_Matrix4&mat);
FKCW_3D_Matrix4	transpose(const FKCW_3D_Matrix4&mat);
FKCW_3D_Vector4	toV4(const FKCW_3D_Vector2&v2,float z,float w);
FKCW_3D_Vector2	toV2(const FKCW_3D_Matrix4&v4);
float				getLength2(const FKCW_3D_Vector4&v);
float				getLength(const FKCW_3D_Vector4&v);
//--------------------------------------------------------------------