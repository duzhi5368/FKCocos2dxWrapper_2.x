//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneEx.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_MsgManager.h"
#include "FKCW_SceneExMgr.h"
//-------------------------------------------------------------------------
class FKCW_SceneExMgr_SceneExMgr;

class FKCW_SceneExMgr_SceneEx 
#if USING_FKCW_WIDGET
	: public cocos2d::cocoswidget::FKCW_UI_WidgetWindow
#else
	: public CCNode
#endif
	, public FKCW_SceneExMgr_MsgDelegate
{
public:
	FKCW_SceneExMgr_SceneEx();
	virtual ~FKCW_SceneExMgr_SceneEx();

	// 首先，第一步调用，加载全部资源.该函数仅会被调用一次。
	virtual void onLoadResources(){};
	// 第二部调用，加载完成。该函数仅会被调用一次。
	virtual void onLoadResourcesCompleted(){};
	// 第三部调用，加载当前场景。该函数仅会被调用一次。
	virtual void onLoadScene(){};

	// 当场景显示时调用
	virtual void onEnterScene(){};
	// 当场景隐藏时调用
	virtual void onExitScene(){};

	// 同步加载图片，将会消耗主线程loop一段时间
	void addImage(const char* pFile);
	// 异步加载图片，将不会消耗主线程loop时间
	void addImageAsync(const char* pFile);

	// 是否当场景被销毁时自动删除图片
	bool isAutoRemoveUnusedTexture() const;
	// 设置：是否当场景被销毁时自动删除图片
	void setAutoRemoveUnusedTexture(bool bAuto);

	// 是否是一个可缓存的场景
	bool isCachable() const;
	// 设置：是否是一个可缓存的场景
	void setCachable(bool bCachable);

	// 获取一些额外数据
	CCObject* getExtraObject() const;
	// 设置一些额外数据
	void setExtraObject(CCObject* pExtraObject);

	// 获取场景类名
	const char* getClassName();
	// 是否当前场景已经加载完成
	bool isLoaded() const;

public:

	// 重载函数
	virtual void onEnter();
	virtual void onExit();

private:
	friend class FKCW_SceneExMgr_SceneExMgr;

	// 是否在 onLoadResources() 函数回调之后异步加载
	bool isLoadingResourcesAsync();
	// 同步加载资源
	void loadResourcesSync();
	// 异步加载资源
	void loadResourcesAsync();
	// 异步加载资源回调函数
	void loadingResourcesCallBack(CCObject* pTextureObj);
	// 设置场景类名
	void setClassName(const char* pClassName);
	// 设置：本场景资源是否已经加载完毕
	void setLoaded(bool bLoaded);

private:
	std::vector<std::string>	m_vLoadImageFilesSync;		// 需要同步加载的图片资源文件
	std::vector<std::string>	m_vLoadImageFilesAsync;		// 需要异步加载的图片资源文件
	std::string					m_strClassName;				// 场景类名
	CCObject*					m_pExtraObject;				// 场景额外信息
	bool						m_bLoaded;					// 场景是否已经加载完毕
	bool						m_bCachable;				// 场景是否可以缓存
	bool						m_bAutoRemoveUnusedTexture;	// 是否自动移除不在使用的资源
	int							m_nLoadResourcesAsyncCount;	// 异步加载的资源个数
	int							m_nLoadResourcesAsyncIdx;	// 当前异步加载的资源编号
};
//-------------------------------------------------------------------------