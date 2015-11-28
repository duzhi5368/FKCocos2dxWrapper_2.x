//*************************************************************************
//	创建日期:	2014-10-25
//	文件名称:	FKCW_Base_PointList.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	两倍数自增点列表
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------
class FKCW_Base_PointList : public CCObject
{
private:
	CCPoint*		m_pBuffer;		// 缓冲区
	int				m_nCount;		// 点个数
	int				m_nCapacity;	// 容量（点的，不是char的）
public:
	FKCW_Base_PointList();
	virtual ~FKCW_Base_PointList();
	static FKCW_Base_PointList* Create();
public:
	// 添加一个点
	void		AddPoint( float x, float y );
	void		AddPoint( CCPoint p ){ AddPoint( p.x, p.y ); }
	// 添加一个点组
	void		AddPoints( FKCW_Base_PointList& p_pList );
	// 根据索引删除一个点
	void		DeletePointAt( int p_nIndex );
	// 获取缓冲区
	CCPoint*	GetBuffer(){ return m_pBuffer; }
	// 获取点数量
	int			GetCount(){ return m_nCount; }
	// 清除全部点
	void		Clear();
	//  获取指定索引位的一个点
	CCPoint		GetPointAt( int p_nIndex );
};