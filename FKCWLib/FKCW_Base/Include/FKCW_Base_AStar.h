//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_Base_AStar.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
struct SCoord
{
	int _x;
	int _y;

	SCoord()
	{
	}
	
	SCoord(int x,int y):
		_x(x),
		_y(y)
	{
	}

	inline bool operator == (const SCoord& coord)const
	{
		return this->_x == coord._x && this->_y == coord._y;
	}

	inline bool operator != (const SCoord& coord)const
	{
		return this->_x != coord._x || this->_y != coord._y;
	}

	inline void operator += (const SCoord& coord)
	{
		this->_x += coord._x ;
		this->_y += coord._y;
	}
};
//-------------------------------------------------------------------------
struct SStepData
{
	int _h;//H 表示从指定的方格移动到终点 B 的预计耗费 (H 有很多计算方法, 这里我们设定只可以上下左右移动).
	int _g;//G 表示从起点 A 移动到网格上指定方格的移动耗费 (可沿斜方向移动).

	SCoord _coord;
	SCoord _parentCoord;

	SStepData()
	{
	}

	SStepData(const SCoord& coord):
		_coord(coord)
	{
	}
};
//-------------------------------------------------------------------------
/**
 * A* 寻路算法
 * 必须先调用SetMapSize设置地图大小
 */
class FKCW_Base_AStar
{
public:
	FKCW_Base_AStar();

	///设置地图的大小,必须在一开始设置
	bool SetMapSize(int row,int col);
	 
	/**
	 * 计算路径
	 * @return true - 有从起点到终点的路径,调用GetRoute方法获得路径; false - 找不到从起点到终点的路径 
	 */
	bool ComputeRoute();
	// 获得从起点到终点的路径
	bool GetRoute(std::vector<SCoord>* list);
	// 设置障碍物
	void SetObstacle(const SCoord& coord);
	// 取消障碍物
	void CancleObstacle(const SCoord& coord);
	// 清除所有障碍物
	void ClearObstacles();
	// 判断所给坐标上有没有障碍物
	bool IsObstacle(const SCoord& coord);
	// 设置可不可以走对角线
	void SetDiagonalEnable(bool bEnable){m_numSurround = bEnable?8:4;}
	// 当前是否可以走对角线
	bool IsDiagonalEnable(){ return (m_numSurround == 8); }
	int  GetRow()						{return m_row;}
	int  GetCol()						{return m_col;}
	void SetStart(const SCoord& coord)  {m_start  = coord;}
	void SetTarget(const SCoord& coord) {m_target = coord;}
	const SCoord& GetStart()            {return m_start;}
	const SCoord& GetTarget()           {return m_target;}
private:
	unsigned char*			m_map;
	int						m_row;
	int						m_col;
	SCoord					m_target;
	SCoord					m_start;

	//在计算四周方块的时候使用
	int						m_numSurround;
	int						m_gAdd[8];
	SCoord					m_surround[8];
	
	std::list<SStepData>	m_openList;
	std::list<SStepData>	m_closeList;
	
	bool		__PopBestStep(SStepData* pSD); 
	bool		__IsInList(std::list<SStepData>& list, const SCoord& coord);
	bool		__JudgeSurround(const SCoord& coord,const SCoord& parentCoord, int G);
	SStepData*	__FindFromList(std::list<SStepData>& list, const SCoord& coord);
	int			__ComputeH(const SCoord& coord) {return abs(m_target._x-coord._x)*10 + abs(m_target._y-coord._y)*10;}
};
//-------------------------------------------------------------------------