//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_MatStackInfoGetor.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Matrix4.h"
#include "FKCW_3D_Macro.h"
//-------------------------------------------------------------------------
class FKCW_3D_MatStackInfoGetor
{
public:
	FKCW_3D_MatStackInfoGetor();
	virtual~FKCW_3D_MatStackInfoGetor();
public:
	static FKCW_3D_MatStackInfoGetor* sharedMatStackInfoGetor();
public:
	bool init();
	FKCW_3D_Matrix4 getTopMat(ENUM_FKCW_3D_MatStackName eMatStackName);
};
//-------------------------------------------------------------------------