/********************************************************************
	created:	2014/12/12
	file base:	FKCW_3D_Vector
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
//--------------------------------------------------------------------
class FKCW_3D_Vector2 FKCW_3D_USED_FOR_SUBMIT_TO_VBO
{
public:
	FKCW_3D_Vector2(){
		_InitMembers();
	}
	FKCW_3D_Vector2(float a0,float a1);
	FKCW_3D_Vector2(const float array[2]);
	~FKCW_3D_Vector2(){};
	const float* getArray()const{return m_fArray;}
	int getArrayLen()const{return 2;}
	vector<float> convertToVector()const;
	FKCW_3D_Vector2 operator +(const FKCW_3D_Vector2&right)const;
	FKCW_3D_Vector2 operator -(const FKCW_3D_Vector2&right)const;
	FKCW_3D_Vector2 operator *(float k)const;
	void init(const float array[2]);
	void init(float a0,float a1);
	float x()const {return m_fArray[0];}
	float y()const {return m_fArray[1];}
	void setx(float x) { m_fArray[0]=x;}
	void sety(float y) { m_fArray[1]=y;}
	void print();
protected:
	void _InitMembers();
protected:
	float m_fArray[2];

};
//--------------------------------------------------------------------
class FKCW_3D_Vector4 FKCW_3D_USED_FOR_SUBMIT_TO_VBO
{
public:
	FKCW_3D_Vector4(){
		_InitMembers();
	}
	FKCW_3D_Vector4(float a0,float a1,float a2,float a3);
	FKCW_3D_Vector4(const float array[4]);
	~FKCW_3D_Vector4(){};
	const float* getArray()const{return m_fArray;}
	float getAt(int index)const;
	int getArrayLen()const{return 4;}
	vector<float> convertToVector()const;
	FKCW_3D_Vector4 operator +(const FKCW_3D_Vector4&right)const;
	FKCW_3D_Vector4 operator -(const FKCW_3D_Vector4&right)const;
	FKCW_3D_Vector4 operator *(float k)const;
	void init(const float array[4]);
	void init(float a0,float a1,float a2,float a3);
	float x()const {return m_fArray[0];}
	float y()const {return m_fArray[1];}
	float z()const {return m_fArray[2];}
	float w()const {return m_fArray[3];}
	float r()const {return m_fArray[0];}
	float g()const {return m_fArray[1];}
	float b()const {return m_fArray[2];}
	float a()const {return m_fArray[3];}
	void setx(float x) { m_fArray[0]=x;}
	void sety(float y) { m_fArray[1]=y;}
	void setz(float z) { m_fArray[2]=z;}
	void setw(float w) { m_fArray[3]=w;}
	void setr(float r) { m_fArray[0]=r;}
	void setg(float g) { m_fArray[1]=g;}
	void setb(float b) { m_fArray[2]=b;}
	void seta(float a) { m_fArray[3]=a;}
	void print();
protected:
	void _InitMembers(){
		memset(&m_fArray, 0, sizeof(m_fArray));
	}
protected:
	float m_fArray[4];

};
//--------------------------------------------------------------------