//*************************************************************************
//	创建日期:	2015-1-26
//	文件名称:	FKCameraChunk.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapChunk.h"
//-------------------------------------------------------------------------
// 摄像机格
class FKCameraChunk : public FKMapChunk
{
public:
	FKCameraChunk( RENDER_MAP_CHUNK_ID id );
	~FKCameraChunk();
public:
	virtual void			draw();
protected:
	// 添加子格
	virtual bool			_SubAddGrid( IMapGridInfo* pGridInfo );
	// 移除子格
	virtual void			_SubDelGrid( CCPoint& tagPos );
private:
	typedef map< unsigned short, SCameraGridInfo* >	CameraGridsMap;
	CameraGridsMap		m_mapCameraGrids;		// 摄像机障碍格组
};