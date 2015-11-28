//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_Programs.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_CommonFunc.h"
//-------------------------------------------------------------------------
class FKCW_3D_Program:public CCGLProgram
{
public:
	FKCW_3D_Program();
	virtual ~FKCW_3D_Program();
public:
	bool	init(){return true;}
	bool	init(string vertFileName,string fragFileName);
	bool	init(const char* vertShader,const char* fragShader);
	void	attachUniform(string unifoName);
	void	passUnifoValue1f(string unifoName,GLfloat v);
	void	passUnifoValue1i(string unifoName,GLint v);
	void	passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen);
	void	passUnifoValueNfv(string unifoName,const GLfloat *array,int arrayLen);
	void	passUnifoValueNiv(string unifoName,const GLint *array,int arrayLen);
	void	passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList);
	void	passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList);
	void	passUnifoValueNiv(string unifoName,const vector<GLint>&valueList);
	void	setName(const string&name){m_name=name;}
	string	getName()const{return m_name;};
protected:
	map<string,GLint> m_unifoMap;
	string m_name;
};
//-------------------------------------------------------------------------