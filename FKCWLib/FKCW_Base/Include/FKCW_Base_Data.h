/********************************************************************
	created:	2014/10/22
	file base:	FKCW_Base_Data
	author:		FreeKnight
	
	purpose:	一个连续的数据段
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------
class FKCW_Base_Data : public CCObject
{
public:
	FKCW_Base_Data();
	FKCW_Base_Data( uint8_t* p_pBytes, size_t p_unSize );
	FKCW_Base_Data( FKCW_Base_Data* p_pData );
	virtual ~FKCW_Base_Data();

	static FKCW_Base_Data* Create();
	static FKCW_Base_Data* CreateWithBytes( uint8_t* p_pBytes, size_t p_unSize );
	static FKCW_Base_Data* CreateWithData( FKCW_Base_Data* p_pData );

	void AppendData( FKCW_Base_Data* p_pData );
	void AppendBytes( uint8_t* p_pBytes, size_t p_unSize );

	CC_SYNTHESIZE(uint8_t*, m_pBytes, Bytes);
	CC_SYNTHESIZE(size_t, m_unSize, Size);
};
//--------------------------------------------------------------------