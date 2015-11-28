//*************************************************************************
//	创建日期:	2015-1-26
//	文件名称:	FKEventChunk.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapChunk.h"
//-------------------------------------------------------------------------
// 事件格
class FKEventChunk : public FKMapChunk
{
public:
	FKEventChunk( RENDER_MAP_CHUNK_ID id );
	~FKEventChunk();
public:
	virtual void			draw();
protected:
	// 添加子格
	virtual bool			_SubAddGrid( IMapGridInfo* pGridInfo );
	// 移除子格
	virtual void			_SubDelGrid( CCPoint& tagPos );
private:
	typedef map< unsigned short, SEventGridInfo* >	EventGridsMap;
	EventGridsMap			m_mapEventGrids;		// 事件障碍格组
};