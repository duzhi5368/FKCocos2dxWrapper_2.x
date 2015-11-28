//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_DefaultMeshes.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Mesh.h"
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_IDTriangle.h"
#include "FKCW_3D_CommonFunc.h"
//-------------------------------------------------------------------------
FKCW_3D_Mesh* CreateBoxMesh(float r);
FKCW_3D_Mesh* CreateBoxMesh(float r,
						   const FKCW_3D_Vector4&upFrontLeftVertexColor,const FKCW_3D_Vector4&upFrontRightVertexColor,
						   const FKCW_3D_Vector4&upBackLeftVertexColor,const FKCW_3D_Vector4&upBackRightVertexColor,
						   const FKCW_3D_Vector4&dnFrontLeftVertexColor,const FKCW_3D_Vector4&dnFrontRightVertexColor,
						   const FKCW_3D_Vector4&dnBackLeftVertexColor,const FKCW_3D_Vector4&dnBackRightVertexColor);
FKCW_3D_Mesh* CreateBallMesh(float r,int nSlice,int nStack);
FKCW_3D_Mesh* CreateBallMesh(float r,int nSlice,int nStack,const FKCW_3D_Vector4&northPoleColor,const FKCW_3D_Vector4&southPoleColor);
FKCW_3D_Mesh* CreateConeMesh(float r,float h,int nSlice,int nStack,bool isHaveBottom=true);
FKCW_3D_Mesh* CreateConeMesh(float r,float h,int nSlice,int nStack,bool isHaveBottom,
							const FKCW_3D_Vector4&headColor,const FKCW_3D_Vector4&bottomColor);
//-------------------------------------------------------------------------