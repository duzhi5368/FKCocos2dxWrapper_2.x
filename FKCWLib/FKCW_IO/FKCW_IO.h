/********************************************************************
	created:	2014/10/28
	file base:	FKCW_IO
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
/* 
====================================
模块说明：	
====================================
	一些简单的数据流操作类
====================================
内部文件功能说明：
====================================
	FKCW_IO_AssetInputStream
		输入数据流类。本身是虚基类，需要被继承实现。
	FKCW_IO_AssetOutputStream
		输出数据流类。本身是虚基类，需要被继承实现。
	FKCW_IO_MemoryInputStream
		内存中的输入数据流类。继承自 FKCW_IO_AssetInputStream
	FKCW_IO_MemoryOutputStream
		内存中的输出数据流类。继承自 FKCW_IO_AssetOutputStream
====================================
依赖项目/库：
====================================
	FKCW_Base
====================================
todo:
====================================

*/
//--------------------------------------------------------------------
#include "Include/FKCW_IO_AssetInputStream.h"
#include "Include/FKCW_IO_AssetOutputStream.h"
#include "Include/FKCW_IO_MemoryInputStream.h"
#include "Include/FKCW_IO_MemoryOutputStream.h"