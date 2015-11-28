//*************************************************************************
//	创建日期:	2014-9-15   15:14
//	文件名称:	FKImageIterator.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKPicCommonDef.h"
#include "FKImage.h"
//-------------------------------------------------------------------------
class CFKImageIterator
{
	friend class CFKImage;
public:
	CFKImageIterator();
	CFKImageIterator( CFKImage* p_pImage );
	operator CFKImage*();
	virtual ~CFKImageIterator();
public:
	bool		ItOK();
	void		Reset();
	void		Upset();
	bool		NextRow();
	bool		PrewRow();
	void		SetY( int p_nY );
	void		SetRow( BYTE* p_byBuf, int p_nN );
	void		GetRow( BYTE* p_byBuf, int p_nN );
	BYTE*	GetRow();
	BYTE*	GetRow( int p_nN );
	bool		NextByte();
	bool		PrewByte();
	bool		NextStep();
	bool		PrewStep();
	bool		GetCol( BYTE* p_pCol, DWORD p_dwX );
	bool		SetCol( BYTE* p_pCol, DWORD p_dwX );
protected:
	int			m_nItX;
	int			m_nItY;
	int			m_nStepX;
	int			m_nStepY;
	BYTE*		m_pIterImage;
	CFKImage*	m_pImage;
};
//-------------------------------------------------------------------------
