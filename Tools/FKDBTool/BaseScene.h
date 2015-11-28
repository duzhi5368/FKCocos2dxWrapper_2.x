#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "../../FKCoreLib/FKPicLib/FKPicLib.h"
#include "../../FKCoreLib/FKDBCocos2dxLib/FKDBCocos2dxLib.h"
//-------------------------------------------------------------------------
class BaseLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
	BaseLayer();
public:
	// WINDOWS 消息
	static void KeyboardHook( UINT message,WPARAM wParam, LPARAM lParam );
private:
	// 显示精灵骨骼数
	void UpdateBoneNum();
	// 显示精灵动画名
	void UpdateFileName();
	// 显示精灵动作列表
	void UpdateActionList();
public:
	// 创建UI
	bool CreateUI();
protected:
	// 按下播放动作标签
	void OnSelActionBtn(CCObject* pSender );
	// 选择了DB文件
	void OnSelDBFile(CCObject* sender);
	// 更换背景颜色
	void OnChangeBGColor(CCObject* sender);
public:
    CREATE_FUNC(BaseLayer);
public:
	CCocos2dxDBSprite*		m_pDBSprite;			// DragonBones精灵
	CCLabelTTF*				m_pBonesArmasNumLabel;	// 骨骼Arma数量信息
	CCLabelTTF*				m_pDBFileLabel;			// DB精灵文件名
	CCMenu*					m_pActionListMenu;		// 动作按钮列表菜单
	CCLayerColor*			m_pBackgroundLayer;		// 背景色层
};
//-------------------------------------------------------------------------
class CTestAnimCallbackHandler : public IAnimationCallback
{
	void OnAnimaitonCallback( const SAnimationCallbackInfo& p_tagInfo, void* p_pContext );
};
//-------------------------------------------------------------------------
#endif // __HELLOWORLD_SCENE_H__
