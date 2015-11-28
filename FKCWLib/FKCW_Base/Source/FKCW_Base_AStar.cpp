//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_AStar.h"
//-------------------------------------------------------------------------
FKCW_Base_AStar::FKCW_Base_AStar():
	m_numSurround(8),
	m_map(0),
	m_row(0),
	m_col(0)
{
	// | (-1,1) | (0,1) | (1,1) |
	// | (-1,0) | (0,0) | (1,0) |
	// | (-1,-1)| (0,-1)| (1,-1)|

	// 横竖行走
	m_surround[0] = SCoord( 0,-1);
	m_surround[1] = SCoord( 0, 1);
	m_surround[2] = SCoord(-1, 0);
	m_surround[3] = SCoord( 1, 0);

	// 斜向行走
	m_surround[4] = SCoord(-1,-1);
	m_surround[5] = SCoord( 1,-1);
	m_surround[6] = SCoord(-1, 1);
	m_surround[7] = SCoord( 1, 1);

	// G(10) H(14) 参数说明
	// 横向移动一个格子的耗费为10, 为了便于计算, 沿斜方向移动一个格子耗费是14.
	for(int i = 0 ; i<4 ; ++i)
		m_gAdd[i] = 10;

	for(int i = 4 ; i<8 ; ++i)
		m_gAdd[i] = 14;

	ClearObstacles();
}
//-------------------------------------------------------------------------
void FKCW_Base_AStar::ClearObstacles()
{
	if(m_map)
	{
		int size = m_row*m_col/8+1;
		memset(m_map,0,size);
	}

	m_closeList.clear();
	m_openList.clear();
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::ComputeRoute()
{
	m_openList.clear();
	m_closeList.clear();

	if(m_row<=0 || m_col<=0 )
		return false;

	SStepData sd(m_target);
	sd._g = 0;
	sd._h = __ComputeH(m_target);

	m_openList.push_back(sd);

	while (!m_openList.empty())
	{
		__PopBestStep(&sd);
		m_closeList.push_back(sd);

		SCoord coord;

		for(int i = 0 ; i<m_numSurround ; ++i)
		{
			coord  = sd._coord;
			coord += m_surround[i];

			if(__JudgeSurround(coord,sd._coord,sd._g+m_gAdd[i]))
				return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::__JudgeSurround(const SCoord& coord,const SCoord& parentCoord,int G)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row && !__IsInList(m_closeList,coord) && !IsObstacle(coord))
	{
		SStepData* pSD = __FindFromList(m_openList,coord);
		if (pSD && pSD->_g > G)
		{
			pSD->_g = G;
			pSD->_parentCoord = parentCoord;
		}
		else if(!pSD)
		{
			SStepData newSD(coord);
			newSD._g = G;
			newSD._h = __ComputeH(coord);
			newSD._parentCoord = parentCoord;

			m_openList.push_back(newSD);

			if(newSD._coord == m_start)
				return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::__PopBestStep(SStepData* pSD)
{
	std::list<SStepData>::iterator       i   = m_openList.begin();
	std::list<SStepData>::const_iterator end = m_openList.end();

	if(i!=end)
	{
		pSD->_g       = i->_g;
		pSD->_h       = i->_h;
		pSD->_coord   = i->_coord;
		pSD->_parentCoord = i->_parentCoord;

		i++;

		while(i!=end)
		{
			if(pSD->_g+pSD->_h > i->_g+i->_h)
			{
				pSD->_g       = i->_g;
				pSD->_h       = i->_h;
				pSD->_coord   = i->_coord;
				pSD->_parentCoord = i->_parentCoord;
			}

			i++;
		}

		i = m_openList.begin();
		while(i->_coord != pSD->_coord)
		{
			i++;
		}

		m_openList.erase(i);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::__IsInList(std::list<SStepData>& list,const SCoord& coord)
{
	std::list<SStepData>::iterator       i   = list.begin();
	std::list<SStepData>::const_iterator end = list.end();

	while(i!=end)
	{
		if(coord == i->_coord)
			return true;

		i++;
	}
	return false;
}
//-------------------------------------------------------------------------
SStepData* FKCW_Base_AStar::__FindFromList(std::list<SStepData>& list, const SCoord& coord)
{
	std::list<SStepData>::iterator       i   = list.begin();
	std::list<SStepData>::const_iterator end = list.end();

	while(i!=end)
	{
		if(coord == i->_coord)
			return &(*i);

		i++;
	}

	return 0;
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::GetRoute(std::vector<SCoord>* list)
{
	list->clear();

	SStepData* sd = __FindFromList(m_openList,m_start);
	int size = m_openList.size();
	if(sd!=NULL)
	{
		list->push_back(sd->_coord);

		sd = __FindFromList(m_closeList,sd->_parentCoord);
		while(sd->_coord!=m_target)
		{
			list->push_back(sd->_coord);
			sd = __FindFromList(m_closeList,sd->_parentCoord);
		}
		list->push_back(sd->_coord);
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::SetMapSize(int row,int col)
{
	if(row<= 0 || col<=0)
		return false;

	if(m_map)
		delete[] m_map;

	int size = row*col/8+1;

	m_map = new unsigned char[size];

	memset(m_map,0,size);

	m_row = row;
	m_col = col;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_Base_AStar::SetObstacle(const SCoord& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		m_map[index/8] |= 1<<(index%8);
	}
}
//-------------------------------------------------------------------------
void FKCW_Base_AStar::CancleObstacle(const SCoord& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		m_map[index/8] &= ~(1<<index%8);
	}
}
//-------------------------------------------------------------------------
bool FKCW_Base_AStar::IsObstacle(const SCoord& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		return (m_map[index/8] & (1<<(index%8))) != 0;
	}
	return true;
}
//-------------------------------------------------------------------------