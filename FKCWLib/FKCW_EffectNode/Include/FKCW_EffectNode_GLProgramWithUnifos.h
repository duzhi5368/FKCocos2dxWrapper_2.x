//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_EffectNode_GLProgramWithUnifos.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Macros.h"
//-------------------------------------------------------------------------
// 若Program1和Program2使用一个相同的名字的Uniform变量，但是这俩变量的UniformID( GLint )可能是不同的。
// 所以当前安全的方法是允许每个着色器有一个独立编号。
class FKCW_EffectNode_GLProgramWithUnifos:public CCGLProgram
{
protected:
	map<string,GLint> m_mapUnifos;
public:
	FKCW_EffectNode_GLProgramWithUnifos(){};
	virtual ~FKCW_EffectNode_GLProgramWithUnifos(){};

	void attachUniform(string unifoName);
	void passUnifoValue1f(string unifoName,GLfloat v);
	void passUnifoValue1i(string unifoName,GLint v);
	void passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen);
	void passUnifoValueNfv(string unifoName,const GLfloat *array,int arrayLen);
	void passUnifoValueNfvN(string unifoName,const GLfloat*array,int arrayLen,int arrayCount);
	void passUnifoValueNiv(string unifoName,const GLint *array,int arrayLen);
	void passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList);
	void passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList);
	void passUnifoValueNiv(string unifoName,const vector<GLint>&valueList);
};