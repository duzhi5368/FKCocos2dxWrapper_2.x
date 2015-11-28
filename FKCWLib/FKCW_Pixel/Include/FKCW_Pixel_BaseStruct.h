//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	FKCW_Pixel_BaseStruct.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Pixel_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Pixel_IJ
{
public:
	FKCW_Pixel_IJ();
	virtual~FKCW_Pixel_IJ();
	FKCW_Pixel_IJ(int i,int j);
public:
	void	setI(int i);
	void	setJ(int j);
	void	setIJ(int i,int j);
	int		getI()const;
	int		getJ()const;
protected:
	void	initMemebers();
protected:
	int m_i,m_j;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_CollisionRect
{
public:
	FKCW_Pixel_CollisionRect();
	virtual~FKCW_Pixel_CollisionRect();
	FKCW_Pixel_CollisionRect(const CCRect&rect,const string&name);
public:
	bool	init(const CCRect&rect,const string&name);
	void	setRect(const CCRect&rect);
	void	setName(const string&name);
	CCRect	getRect()const;
	string	getName()const;
protected:
	CCRect m_rect;
	string m_name;
};
//-------------------------------------------------------------------------