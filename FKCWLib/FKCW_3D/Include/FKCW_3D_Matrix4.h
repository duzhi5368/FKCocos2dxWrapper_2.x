/********************************************************************
	created:	2014/12/12
	file base:	FKCW_3D_Matrix4
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_Vector.h"
//--------------------------------------------------------------------
class FKCW_3D_Matrix4 FKCW_FINAL
{
public:
	FKCW_3D_Matrix4(){
		_InitMembers();
	}
	FKCW_3D_Matrix4(const float array[16]);
	FKCW_3D_Matrix4(float a0,float a1,float a2,float a3,//col 1
		float a4,float a5,float a6,float a7,//col2
		float a8,float a9,float a10,float a11,//col3
		float a12,float a13,float a14,float a15//col4
		);
	~FKCW_3D_Matrix4(){}
	void init(const float array[16]);
	void init(float a0,float a1,float a2,float a3,//col 1
		float a4,float a5,float a6,float a7,//col2
		float a8,float a9,float a10,float a11,//col3
		float a12,float a13,float a14,float a15//col4
		);
	const float*		getArray()const{return  m_array;}
	float				getAt(int index)const;
	void				setAt(int index,float value);
	int					getArrayLen()const{return 16;}
	vector<float>		convertToVector()const;
	FKCW_3D_Matrix4	operator*(const FKCW_3D_Matrix4&mat)const;
	FKCW_3D_Vector4	operator*(const FKCW_3D_Vector4&v)const;
	FKCW_3D_Matrix4	operator*(float k)const;
	FKCW_3D_Matrix4	operator+(const FKCW_3D_Matrix4&mat)const;
	void				print();
private:
	void _InitMembers();
private:
	float m_array[16];

};
//--------------------------------------------------------------------