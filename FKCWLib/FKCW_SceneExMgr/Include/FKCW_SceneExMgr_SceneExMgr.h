//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneEx.h"
#include "FKCW_SceneExMgr.h"
//-------------------------------------------------------------------------
typedef FKCW_SceneExMgr_SceneEx* (*Fn_CreateSceneExtension)();
//-------------------------------------------------------------------------
// 便捷宏
#ifndef CREATE_SCENE_FUNC
#define CREATE_SCENE_FUNC(_CLASS_) \
	static FKCW_SceneExMgr_SceneEx* createThisScene() { return new _CLASS_; }
#endif

// 注册场景到场景管理器中
#ifndef REGISTER_SCENE_FUNC
#define REGISTER_SCENE_FUNC(_CLASS_) \
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->registerSceneClass(#_CLASS_, &_CLASS_::createThisScene)
#endif

// 如果场景不在缓冲场景池内，则加载场景
#ifndef LoadScene
#define LoadScene(_CLASS_) \
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->loadScene(_CLASS_)
#endif

// 查找指定场景
#ifndef SeekScene
#define SeekScene(_CLASS_) \
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->seekScene(_CLASS_)
#endif
//-------------------------------------------------------------------------
// 场景替换操作消息方式
enum ENUM_SceneSwitchType
{
	eSceneSwitchPushScene,
	eSceneSwitchReplaceScene,
	eSceneSwitchPopScene,
	eSceneSwitchPopToSceneStackLevel
};
//-------------------------------------------------------------------------
// UI场景替换操作消息方式
enum ENUM_UISceneSwitchType					
{
	eUISceneSwitchRunScene,	
	eUISceneSwitchPopScene
};
//-------------------------------------------------------------------------
// 场景替换数据
struct SSceneSwitchInfo
{
	FKCW_SceneExMgr_SceneEx* pNextScene;	// 下一个需要显示的场景
	CCObject* pExtra;						// 下一个场景的额外数据
	ENUM_SceneSwitchType eType;				// 场景替换方式
	int nPopSceneStackLevel;				// 场景堆栈的弹出层级
	bool bLockedSwitch;						// 是否锁定本次替换过程
};
//-------------------------------------------------------------------------
// UI 场景替换数据
struct SUISceneSwitchInfo
{
	FKCW_SceneExMgr_SceneEx* pScene;		// 下一个需要显示的场景，若为NULL，则表示直接隐藏
	ENUM_UISceneSwitchType eType;			// 场景替换方式
	bool bLockedSwitch;						// 是否锁定本次替换过程
};
//-------------------------------------------------------------------------
// 场景管理器
class FKCW_SceneExMgr_SceneExMgr : public CCScene
{
public:
	virtual ~FKCW_SceneExMgr_SceneExMgr();
	static FKCW_SceneExMgr_SceneExMgr* sharedManager();

	// 获取当前运行中的场景（处于运行场景堆的堆顶）
	FKCW_SceneExMgr_SceneEx* getRunningScene() const;

	// 是否要清理当前运行场景
	inline bool isSendCleanupToScene(){ return m_bSendCleanupToScene; }
	// 当游戏开始时运行一个场景，应当只被调用一次
	void runWithScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra = NULL);
	// 将指定场景推到堆顶
	void pushScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra = NULL);
	// 替换运行场景（参数中的场景将运行）
	void replaceScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra = NULL);
	// 弹出场景堆顶部场景，如果场景堆栈空了，则结束游戏。
	void popScene(CCObject* pExtra = NULL);

	// 清除场景，只保留根场景
	void popToRootScene(CCObject* pExtra = NULL);
	// 清除场景指定层级之上的全部场景，若nlevel小于1，将结束游戏
	void popToSceneStackLevel(int nLevel, CCObject* pExtra = NULL);
	// 开启一个UI场景
	void runUIScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra = NULL);
	// 关闭一个UI场景
	void popUIScene(FKCW_SceneExMgr_SceneEx* pScene);
	// 关闭全部UI场景
	void popAllUIScene();
	// 是否一个UI场景正在运行中
	bool isSceneRunning(const char* pSceneName);
	// 结束游戏
	void end();

	// 从缓存场景池中删除指定的缓存的场景
	void removeCachedScene(const char* pSceneName);
	// 删除缓存场景池中全部缓存场景
	void removeAllCachedScenes();
	// 删除缓存场景池中的的全部未使用场景
	void removeUnusedCachedScenes();

	// 当游戏启动时，进行场景类的注册，该函数应当只被调用一次。
	void registerSceneClass(const char* pSceneName, Fn_CreateSceneExtension pFn);

	// 如果场景不在缓冲场景池内，则加载场景
	FKCW_SceneExMgr_SceneEx* loadScene(const char* pSceneName);
	// 查找指定场景
	FKCW_SceneExMgr_SceneEx* seekScene(const char* pSceneName);
	// 获取触摸优先级，该函数每调用一次，该值将减少一次。
	int getTouchPriority();


public:
	// 主循环调用
	virtual void visit();
	
protected:
	friend class FKCW_SceneExMgr_SceneEx;
	

	// 每帧调用，场景管理器的逻辑主循环
	void mainLoop();
	// 设置下一个运行的场景
	void setNextScene();
	// 处理场景切换事件
	void handleSceneSwitch(SSceneSwitchInfo& tSceneSwitch);
	// 解锁一个场景切换事件
	void unlockSceneSwitch(const char* pClassName);
	// 处理UI场景切换事件
	void handleUISceneSwitch(SUISceneSwitchInfo& tSceneSwitch);
	// 解锁一个UI场景切换事件
	void unlockUISceneSwitch(const char* pClassName);
	// 便利访问全部UI场景
	void visitUIScenes();
	// 加载指定场景所需的资源
	bool loadSceneResources(FKCW_SceneExMgr_SceneEx* pScene);
	// 设置场景额外信息
	void setExtraToScene(FKCW_SceneExMgr_SceneEx* pScene, CCObject* pExtra);
	// 如果场景需要缓存，则将指定场景添加到场景缓冲池
	void addCachableScene(FKCW_SceneExMgr_SceneEx* pScene);
	// 获取场景类名称
	const char* getSceneClassName(FKCW_SceneExMgr_SceneEx* pScene);

protected:
	bool											m_bSendCleanupToScene;		// 是否要清理当前运行场景
	FKCW_SceneExMgr_MsgManager*						m_pMsgManager;				// 游戏消息管理器
	FKCW_SceneExMgr_SceneEx*						m_pRunningScene;			// 当前正在运行的场景
	FKCW_SceneExMgr_SceneEx*						m_pNextScene;				// 下一个准备运行的场景
	std::vector<FKCW_SceneExMgr_SceneEx*>			m_vRunningSceneStack;		// 运行场景队列	
	std::list<SSceneSwitchInfo>						m_lSceneSwitchQueue;		// 场景切换事件队列
	std::list<SUISceneSwitchInfo>					m_lUISceneSwitchQueue;		// UI场景切换事件队列
	std::vector<FKCW_SceneExMgr_SceneEx*>			m_vRunningUIScenes;			// UI场景队列
	std::map<std::string, Fn_CreateSceneExtension>	m_mFnSceneCreatePointers;	// 函数静态指针队列，协助创建新场景
	std::map<std::string, FKCW_SceneExMgr_SceneEx*> m_mSceneCachePool;			// 缓存场景池

#if(COCOS2D_DEBUG >= 1)
	void debugSceneSwitchInfo();
#endif

private:
	FKCW_SceneExMgr_SceneExMgr();

};
