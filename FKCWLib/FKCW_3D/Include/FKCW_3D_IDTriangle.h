//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_IDTriangle.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
//-------------------------------------------------------------------------
class FKCW_3D_IDTriangle FKCW_3D_USED_FOR_SUBMIT_TO_VBO
{
public:
	FKCW_3D_IDTriangle();
	~FKCW_3D_IDTriangle(){}
	FKCW_3D_IDTriangle(int ID0,int ID1,int ID2);
	const short*getArray()const {return m_array;}

	void	init(int ID0,int ID1,int ID2);
	short	vID(int i)const;
	void	setVID(int i,short vID);
protected:
	void	_InitMembers();
protected:
	short	m_array[3];
};