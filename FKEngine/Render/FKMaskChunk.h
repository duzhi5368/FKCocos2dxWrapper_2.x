//*************************************************************************
//	创建日期:	2015-1-27
//	文件名称:	FKMaskChunk.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapChunk.h"
//-------------------------------------------------------------------------
class FKMaskChunk : public FKMapChunk
{
public:
	FKMaskChunk( RENDER_MAP_CHUNK_ID id );
	~FKMaskChunk();
public:
	virtual void			draw();
protected:
	// 添加子格
	virtual bool			_SubAddGrid( IMapGridInfo* pGridInfo );
	// 移除子格
	virtual void			_SubDelGrid( CCPoint& tagPos );
private:
	typedef map< unsigned short, SMaskGridInfo* >	MaskGridsMap;
	MaskGridsMap		m_mapMaskGrids;		// 遮罩障碍格组
};