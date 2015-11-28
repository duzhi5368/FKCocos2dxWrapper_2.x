/********************************************************************
	created:	2014/12/12
	file base:	FKCW_3D_Macro
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;
#include "cocos2d.h"
using namespace cocos2d;
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//--------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#define FKCW_FINAL							final
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define FKCW_FINAL							
#endif
// 如果用于提交给vbo，则不允许有虚函数（包括其析构函数也不允许为虚），所以这里强制final
#define FKCW_3D_USED_FOR_SUBMIT_TO_VBO		FKCW_FINAL
//--------------------------------------------------------------------
#define kCCAttributeNameNormal  "a_normal"
extern int kCCVertexAttrib_Normals;
//--------------------------------------------------------------------
#define FKCW_3D_V4(x,y,z,w)	FKCW_3D_Vector4(x,y,z,w)
#define FKCW_Square(x) ((x)*(x))
//--------------------------------------------------------------------
enum ENUM_FKCW_3D_MatStackName
{
	eFKCW3D_ModelViewStack = 0,
	eFKCW3D_ProjectionStack,
	eFKCW3D_TextureStack,
};
enum ENUM_FKCW_3D_ProjectionMode
{
	eFKCW3D_OrthographicMode = 0,
	eFKCW3D_PerspectiveMode,
};
//--------------------------------------------------------------------