/********************************************************************
	created:	2014/10/31
	file base:	FKCW_Network_NetworkCommon
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
//--------------------------------------------------------------------
#if !defined(__GNUC__)
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
typedef float Real;
#else
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef uint32_t DWORD;
typedef float Real;
#endif
//--------------------------------------------------------------------
#define SocketMaxPacketSize					(16 * 1024)
#define SocketDefaultTimeout				30
#define SocketInputBufferDefaultSize		(64 * 1024)
#define SocketOutputBufferDefaultSize		(8 * 1024)
#define SocketError							-1
#define SocketInvalid						-1
#define PacketHeaderLength					24
//--------------------------------------------------------------------
