//*************************************************************************
//	创建日期:	2014-11-14
//	文件名称:	FKCW_Base_Geometry.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
// 计算两个点的差作为矢量
CCSize ccpDiff(const CCPoint& tPoint1, const CCPoint& tPoint2);
// 返回向指定方向移动后的点坐标
CCPoint ccpShift(const CCPoint& tPoint, const CCSize& tSize);
// 返回给定向量的标准化后的点
CCPoint ccpNormalizeTo(const CCPoint& tPoint, const CCSize& tNorm);
// 返回给定向量的去标准化后的点
CCPoint ccpDenormalizeTo(const CCPoint& tPoint, const CCSize& tNorm);
// 转换CCPoint -> CCSize
CCSize cczFromPoint(const CCPoint& tPoint);
// 计算向量长度
float cczLength(const CCSize& tSize);
// 计算两向量的和
CCSize cczAdd(const CCSize& tSize1, const CCSize& tSize2);
// 计算两个向量区别
CCSize cczSub(const CCSize& tSize1, const CCSize& tSize2);
// 返回指定向量标准化后值
CCSize cczNormalizeTo(const CCSize& tSize, const CCSize& tNorm);
// 返回指定向量去标准化后值
CCSize cczDenormalizeTo(const CCSize& tSize, const CCSize& tNorm);
// 计算俩个向量的夹向量
CCSize cczClamp(const CCSize& tSize, const CCSize& tLimit1, const CCSize& tLimit2);
// 返回组成相应坐标的最小值向量
CCSize cczMin(const CCSize& tSize1, const CCSize& tSize2);
// 返回组成相应坐标的最大值向量
CCSize cczMax(const CCSize& tSize1, const CCSize& tSize2);
// 根据给定的中心点和大小，计算一个矩形
CCRect ccRectMakeWithCenterAndSize(const CCPoint& tCenter, const CCSize& tSize);
// 根据给定的基准原点和大小，计算一个矩形
CCRect ccRectMakeWithOriginAndSize(const CCPoint& tOrigin, const CCSize& tSize);
// 返回一个矩形的中心点坐标
CCPoint ccRectGetCenter(const CCRect& tRect);
// 计算一个矩形的大小
CCSize ccRectGetSize(const CCRect& tRect);
// 判断一个矩形是否是空矩形
bool ccIsZeroRect(const CCRect& tRect);