//*************************************************************************
//	创建日期:	2015-1-26
//	文件名称:	FKMapObj.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKBackgroundLayer;
class FKFarLayer;
class FKNormalLayer;
class FKFrontLayer;
class FKCameraMaskLayer;
class FKBarrierLayer;
class FKCameraLayer;
class FKMaskLayer;
class FKEventLayer;
class FKMapLayer;
struct SMapInfo;
class FKMapObj : public IMapObj, public CCNode
{
public:
	FKMapObj( RENDER_MAP_ID id );
	~FKMapObj();
public:
	virtual void			update( float dt );
	virtual void			onEnter();
	virtual void			onExit();
public:
	// 从一个FKM资源文件创建
	virtual bool			InitFromFile( const char* szFKMFileName );
	// 写入到一个资源文件
	virtual bool			WriteToFile( const char* szFileName );

	// 添加一个Obj
	virtual unsigned int	AddObj( RENDER_MAP_LAYER_ID id, IRenderObj* pObj );
	// 动态更新Obj位置
	virtual void			SetObjPos( RENDER_MAP_CHUNK_ID id, const CCPoint& tagPos );
	// 获取渲染Obj
	virtual IRenderObj*		GetObjByID( RENDER_MAP_CHUNK_ID id );
	// 移除渲染Obj
	virtual void			RemoveObjByID( RENDER_MAP_CHUNK_ID id );

	// 获取指定层
	virtual IMapLayer*		GetLayerByID( RENDER_MAP_LAYER_ID id );

	// 转换屏幕坐标到场景坐标
	virtual void			ConvertScreenPos2ScenePos( const CCPoint& screenPos, 
		CCPoint& scenePos );

	// 碰撞选取检测
	virtual void			HitTestObj( const CCPoint& pt, list<IRenderObj*>& ObjList );
	// 获取场景ID
	virtual RENDER_MAP_ID	GetID();
	// 获取场景文件名
	virtual void			GetSceneFileName( string& strFileName );
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender );
	// 是否允许Update
	virtual void			SetEnableUpdate( bool bIsEnable );
private:
	// 获取本地图的全部渲染对象列表
	void					GetRenderObjsSet( set<IRenderObj*>& SetRenderObjs );
private:
	SMapInfo*						m_pMapInfo;					// 场景自身信息
	bool							m_bIsNeedUpdate;			// 是否需要更新
	bool							m_bIsUseDebugRender;		// 是否开启了debug渲染
	unsigned int					m_unRenderObjSceneID;		// 渲染对象场景ID

	map<RENDER_MAP_ID,FKMapLayer*>	m_AllLayers;				// 全部层

	//FKBackgroundLayer*				m_pBackgroundLayer;			// 背景层，单层，固定大小，连续
	//vector<FKFarLayer*>				m_vecFarLayers;				// 远景层，多层
	//vector<FKNormalLayer*>			m_vecNormalLayers;			// 中景层，多层
	//vector<FKFrontLayer*>				m_vecFrontLayers;			// 前景层，多层
	//FKCameraMaskLayer*				m_pCameraMaskLayer;			// 镜头层，单层，固定大小，连续

	//FKBarrierLayer*					m_pBarrierLayer;			// 障碍层，单层
	//vector<FKCameraLayer*>			m_vecCameraBarrierLayers;	// 摄像机限制层，多层
	//FKMaskLayer*						m_pMaskLayer;				// 半遮罩层，单层
	//FKEventLayer*						m_pEventLayer;				// 事件层，单层
};