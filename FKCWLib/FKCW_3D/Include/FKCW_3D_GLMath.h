/********************************************************************
	created:	2014/12/12
	file base:	FKCW_3D_GLMath
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_3D_Math.h"
//--------------------------------------------------------------------
//OpenGL transform pipline: http://user.qzone.qq.com/350479720/blog/1310060277
//Basic 3D Math: Matrices: http://www.matrix44.net/cms/notes/opengl-3d-graphics/basic-3d-math-matrices
//--------------------------------------------------------------------
FKCW_3D_Matrix4		calculateViewMatrixInverse(const FKCW_3D_Vector4&eyePos,const FKCW_3D_Vector4&center,const FKCW_3D_Vector4&up);
FKCW_3D_Matrix4		calculateViewMatrix(const FKCW_3D_Vector4&eyePos,const FKCW_3D_Vector4&center,const FKCW_3D_Vector4&up);
FKCW_3D_Matrix4		calculateRotationMatrix(const FKCW_3D_Vector4&axis,float cosA,float sinA);
FKCW_3D_Matrix4		calculateTranslateMatrix(const FKCW_3D_Vector4&translate);
FKCW_3D_Matrix4		calculateScaleMatrix(float kx,float ky,float kz);
FKCW_3D_Matrix4		calculatePerspectiveProjectionMatrix(float fovy,float aspect,float zNear,float zFar);
FKCW_3D_Matrix4		calculateOrthoProjectionMatrix(float left,float right,float bottom,float top,float near,float far);
FKCW_3D_Vector4		convertFromViewportSpaceToWorldSpace(const FKCW_3D_Vector4&winPos,const FKCW_3D_Matrix4&projMat,
									 const FKCW_3D_Matrix4&viewMat,const float viewport[4]);
bool				isLine3dIntersectWithTriangle3d(const FKCW_3D_Vector4&start,const FKCW_3D_Vector4&dir,
                                     const FKCW_3D_Vector4&p0,const FKCW_3D_Vector4&p1,const FKCW_3D_Vector4&p2);
FKCW_3D_Vector4		getIntersectPointOfLine3dIntersectWithPlane3d(const FKCW_3D_Vector4&start,const FKCW_3D_Vector4&dir,
                                     const FKCW_3D_Vector4&p,const FKCW_3D_Vector4&norm);
//--------------------------------------------------------------------