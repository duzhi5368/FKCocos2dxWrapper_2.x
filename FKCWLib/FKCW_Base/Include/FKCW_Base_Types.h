/********************************************************************
	created:	2014/10/22
	file base:	FKCWTypes
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------

// 常规函数定义
typedef const char* (*DECRYPT_FUNC)(const char* p_szEnc, int p_nEncLen, int* p_pPlainLen );
typedef const char* (*ENCRYPT_FUNC)(const char* p_pPlain, int p_nPlainLen, int* p_pEncLen );
typedef const char* (*MULTI_ENCRYPT_FUNC)(const char* enc, size_t encLen, size_t* plainLen, int algorithm);
typedef const char* (*MULTI_DECRYPT_FUNC)(const char* plain, size_t plainLen, size_t* encLen, int algorithm);

// 通过角度计算位置
static inline CCPoint ccpDegree(float d) 
{
	float r = CC_DEGREES_TO_RADIANS(d);
	return ccp(cosf(r), sinf(r));
}

// 自定义常用结构
typedef vector<string>	StringList;
//--------------------------------------------------------------------
// aabb盒
typedef struct ccAABB 
{ 
	CCPoint min;
	CCPoint max; 
} SAABB;
static inline SAABB ccaabb(const CCPoint min, const CCPoint max) 
{
	SAABB b = {
		min,
		max
	};
	return b;
}
//--------------------------------------------------------------------
class CIDTriangle
{
public:
	CIDTriangle(){}
	~CIDTriangle(){}
	CIDTriangle(short ID0,short ID1,short ID2){
		m_v[0]=m_v[1]=m_v[2]=0;
		init(ID0, ID1, ID2);
	}
	void init(short ID0,short ID1,short ID2){
		m_v[0]=ID0;
		m_v[1]=ID1;
		m_v[2]=ID2;
	}
	short getvIDByIndex(int index)const {
		assert(index>=0&&index<3);
		return m_v[index];
	}
	void setvIDByIndex(int index,short ID){
		assert(index>=0&&index<3);
		m_v[index]=ID;
	}
protected:
	short m_v[3];
};
//--------------------------------------------------------------------
class CIDLine
{
public:
	CIDLine(){}
	~CIDLine(){}
	CIDLine(short ID0,short ID1){
		m_v[0]=m_v[1]=0;
		init(ID0, ID1);
	}
	void init(short ID0,short ID1){
		m_v[0]=ID0;
		m_v[1]=ID1;
	}
	short getvIDByIndex(int index){
		assert(index>=0&&index<2);
		return m_v[index];
	}
	void setvIDByIndex(int index,short ID){
		assert(index>=0&&index<2);
		m_v[index]=ID;
	}
protected:
	short m_v[2];
};
//--------------------------------------------------------------------
class CIDquad
{
public:
	int m_vIDLU;
	int m_vIDRU;
	int m_vIDLD;
	int m_vIDRD;
	bool m_isRemoved;
	int m_vIDL;
	int m_vIDR;
	int m_vIDU;
	int m_vIDD;

	CIDquad(){
		m_vIDLU=-1;
		m_vIDRU=-1;
		m_vIDLD=-1;
		m_vIDRD=-1;
		m_isRemoved=false;
		m_vIDL=-1;
		m_vIDR=-1;
		m_vIDU=-1;
		m_vIDD=-1;
	}
};
//--------------------------------------------------------------------
class Cv2
{
public:
	Cv2(){}
	Cv2(float _x,float _y){
		m_v[0]=m_v[1]=0;
		init(_x, _y);
	}
	~Cv2(){};
	void init(float x,float y){
		m_v[0]=x;
		m_v[1]=y;
	}
	void	setx(float x){m_v[0]=x;}
	void	sety(float y){m_v[1]=y;}
	float	x()const{return m_v[0];}
	float	y()const{return m_v[1];}
	float*	getArray(){return m_v;}

	static	Cv2 CCPToV2(const CCPoint&point){ Cv2 v(point.x,point.y);return v; }
	static	CCPoint V2ToCCP(const Cv2&v){ CCPoint point(v.x(),v.y()); return point; }
protected:
	float m_v[2];
};
//--------------------------------------------------------------------
class Cv4
{
public:
	Cv4(){}
	Cv4(float x,float y,float z,float w){
		m_v[0]=0;
		m_v[1]=0;
		m_v[2]=0;
		m_v[3]=0;
		init(x, y, z, w);
	}
	~Cv4(){};
	void init(float x,float y,float z,float w){
		m_v[0]=x;
		m_v[1]=y;
		m_v[2]=z;
		m_v[3]=w;
	}
	float x()const{return m_v[0];}
	float y()const{return m_v[1];}
	float z()const{return m_v[2];}
	float w()const{return m_v[3];}
	void setx(float x){m_v[0]=x;}
	void sety(float y){m_v[1]=y;}
	void setz(float z){m_v[2]=z;}
	void setw(float w){m_v[3]=w;}
protected:
	float m_v[4];
};
//--------------------------------------------------------------------