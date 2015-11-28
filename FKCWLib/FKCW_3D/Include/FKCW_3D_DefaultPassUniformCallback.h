//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_DefaultPassUniformCallback.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_Programs.h"
#include "FKCW_3D_MatStackInfoGetor.h"
//-------------------------------------------------------------------------
class FKCW_3D_Node;
//-------------------------------------------------------------------------
void PassUnifoCallback_TexOnly(FKCW_3D_Node* node,FKCW_3D_Program* program);
void PassUnifoCallback_ClassicLighting(FKCW_3D_Node* node,FKCW_3D_Program* program);
//-------------------------------------------------------------------------