//*************************************************************************
//	创建日期:	2014-10-25
//	文件名称:	FKCW_Base.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/* 
====================================
模块说明：	
====================================
	最基本的库，被各FKCW库调用。
====================================
内部文件功能说明：
====================================
	FKCW_Base_Macro
		基本的一些宏定义
	FKCW_Base_Types
		一些基础的类型定义
	FKCW_Base_ColorConvert
		颜色相关的一些定义和便捷函数
	FKCW_Base_Utils
		大量的基础函数，不便分类的或非常常用的，或相当简便的都丢在这里。
		暂时包括 字符串处理相关函数，路径文件后缀处理函数，部分字符串解析函数，字符串编码处理函数，
				基本数学处理函数，基本节点处理函数，一些简单的附属功能函数。
	FKCW_Base_StringConvert
		字符串 gb2312 和 utf8 的转码函数
	FKCW_Base_Geometry
		部分数学函数。包括CCPoint,CCSize,CCRect之类的简易函数。
	FKCW_Base_Data
		一个基本的数据类
	FKCW_Base_DrawingPrimitives
		绘制基本元素方法类。是对CCDrawingPrimitives的一个优化。另外还有 基本元素 矩形，线，点 的绘制节点支持。
	FKCW_Base_ResourceLoader
		一个自我维持独立线程的资源加载类。可以加载不同类型资源。
	FKCW_Base_PointList
		一个ccpoint的链表类，封装了基本的链表操作。
	FKCW_Base_MD5
		简单的封装MD5加密函数。可以将一个极大的数据计算出一个128位的消息摘要数据。可用于数字签名算法。
	FKCW_Base_Base64
		简单的封装Base64加密解密函数。
	FKCW_Base_Memory
		内存跟踪系统。可以协助进行内存监察，泄露检测。
	FKCW_Base_Localization
		多国语言基本支持。需要依赖标准的strings.xml配置表。
	FKCW_Base_SecureUserDefault
		基本配置表。类似CCUserDefault，仅额外增加了一个value的加密解密功能。
	FKCW_Base_Bundle
		Key-Value一个简便的封装管理器，管理一个表，可适配多种格式类型的Value。
	FKCW_Base_CSVLoader
		CSV格式加载器
	FKCW_Base_SHA1
	FKCW_Base_HMAC_SHA1
		SHA1算法，可以将一个极大的数据计算出一个160位的消息摘要数据。可用于数字签名算法。
	FKCW_Base_AStar
		一个简单的AStar算法
====================================
依赖项目/库：
====================================
	无（部分cocos2dx标准库不算）
====================================
todo:
====================================
	平台相异的众多功能（例如获得系统内存量，弹出系统级的messageBox等）
*/ 
//-------------------------------------------------------------------------
#include "Include/FKCW_Base_Macro.h"
#include "Include/FKCW_Base_Memory.h"
#include "Include/FKCW_Base_Types.h"
#include "Include/FKCW_Base_ColorConvert.h"

#include "Include/FKCW_Base_Utils.h"
#include "Include/FKCW_Base_Data.h"
#include "Include/FKCW_Base_Geometry.h"
#include "Include/FKCW_Base_StringConvert.h"

#include "Include/FKCW_Base_DrawingPrimitives.h"
#include "Include/FKCW_Base_ResourceLoader.h"
#include "Include/FKCW_Base_PointList.h"
#include "Include/FKCW_Base_MD5.h"
#include "Include/FKCW_Base_Base64.h"
#include "Include/FKCW_Base_SHA1.h"
#include "Include/FKCW_Base_Localization.h"
#include "Include/FKCW_Base_SecureUserDefault.h"
#include "Include/FKCW_Base_Bundle.h"
#include "Include/FKCW_Base_CSVLoader.h"
#include "Include/FKCW_Base_AStar.h"
#include "Include/FKCW_Base_DecodeHtmlEntitiesUTF8.h"
#include "Include/FKCW_Base_PinYinUtils.h"
#include "Include/FKCW_Base_FileZip.h"
#include "Include/FKCW_Base_ByteBuffer.h"