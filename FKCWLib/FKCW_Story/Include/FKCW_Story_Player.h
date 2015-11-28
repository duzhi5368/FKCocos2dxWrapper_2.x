//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_Story_Player.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Story_Macros.h"
#include <cocos-ext.h>
//-------------------------------------------------------------------------
class FKCW_Story_Layer;
class FKCW_Story_Command;
class FKCW_Story_Player : public CCObject
{
private:
	CCPoint m_oldDesignSize;					// 之前的设计大小
	bool m_needRestoreDesignSize;				// 是否需要保存之前的设计大小

private:
	// 获取下一个命令
	void fetchNextCommand();
	// 执行一个命令
	void executeCurrentCommand();
	// 获取一个角色节点
	CCNode* getRole(const char* name);
	// 获取一个动画角色对象
	cocos2d::extension::CCArmature* getArmatureRole(const char* name);	
	// 某动作执行完毕的回调函数
	void onArmatureAnimationDone(cocos2d::extension::CCArmature* armature, 
		cocos2d::extension::MovementEventType e, const char* name);

public:
	FKCW_Story_Player();
	virtual ~FKCW_Story_Player();
	static FKCW_Story_Player* create(FKCW_Story_Layer* owner);

	/// init
	virtual bool initWithOwner(FKCW_Story_Layer* owner);

	// 解析一个文件数据
	int ParserStoryScriptString(const char* szStr );
	// 开始启动
	void start();
	// 一个消息处理完毕
	void onMessageDone();
	// 收到点击消息
	void onClickDone();
	// 是否有错误
	bool hasError() { return !m_error.empty(); }

	CC_SYNTHESIZE(FKCW_Story_Layer*, m_owner, Owner);
	CC_SYNTHESIZE(FKCW_Story_Command*, m_curCmd, CurrentCommand);
	CC_SYNTHESIZE(int, m_curCmdIndex, CurrentCommandIndex);
	CC_SYNTHESIZE_READONLY_BOOL(m_done, Done);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCDictionary, m_roleMap, RoleMap);

	// 全局设置
	CC_SYNTHESIZE(float, m_msgSize, MessageSize);
	CC_SYNTHESIZE(int, m_msgColor, MessageColor);
	CC_SYNTHESIZE(CCPoint, m_msgPos, MessagePos);
	CC_SYNTHESIZE(CCPoint, m_msgAnchor, MessageAnchor);
	CC_SYNTHESIZE_PASS_BY_REF(string, m_msgFont, MessageFont);
	CC_SYNTHESIZE_PASS_BY_REF(string, m_nameFont, NameFont);
	CC_SYNTHESIZE_PASS_BY_REF(string, m_labelFont, LabelFont);
	CC_SYNTHESIZE(float, m_nameSize, NameSize);
	CC_SYNTHESIZE(int, m_nameColor, NameColor);
	CC_SYNTHESIZE(CCPoint, m_labelAnchor, LabelAnchor);
	CC_SYNTHESIZE(float, m_labelSize, LabelSize);
	CC_SYNTHESIZE(int, m_labelColor, LabelColor);
	CC_SYNTHESIZE(CCPoint, m_namePos, NamePos);
	CC_SYNTHESIZE(CCPoint, m_nameAnchor, NameAnchor);
	CC_SYNTHESIZE(CCPoint, m_bgPos, BgPos);
	CC_SYNTHESIZE(CCPoint, m_requiredDesignSize, RequiredDesignSize);
	CC_SYNTHESIZE(ResolutionPolicy, m_requiredPolicy, RequiredPolicy);
	CC_SYNTHESIZE(CCSize, m_msgConstraints, MessageConstraints);
	CC_SYNTHESIZE(CCTextAlignment, m_msgAlignment, MessageAlignment);
	CC_SYNTHESIZE(CCSize, m_labelConstraints, LabelConstraints);
	CC_SYNTHESIZE(CCTextAlignment, m_labelAlignment, LabelAlignment);

	CC_SYNTHESIZE_PASS_BY_REF_SETTER(string, m_error, Error);
};
