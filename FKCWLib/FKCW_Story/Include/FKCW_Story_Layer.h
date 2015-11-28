//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_Story_Layer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Story_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Story_Player;
class FKCW_Story_Layer : public CCLayer 
{
	friend class FKCW_Story_Player;

private:
	FKCW_Story_Player* m_player;		// 演员			
	bool m_playing;						// 是否正在update
	vector<string> m_loadedImageFiles;	// 已加载的图片

protected:
	FKCW_Story_Layer();

	// 故事结束毁掉
	void onStoryDone();
	// 卸载已加载的图片
	void unloadImageFiles();

public:
	virtual ~FKCW_Story_Layer();
	static FKCW_Story_Layer* create();

	// 全局参数
	static string getParameter(const string& key);
	static void setParameter(const string& key, const string& value);

	virtual bool init();
	virtual void onExit();
	virtual void onEnter();

	// 从文件中预加载一个故事脚本
	bool preloadStoryFile(const string& storyScriptFile,DECRYPT_FUNC decFunc = NULL);
	// 预加载一段故事脚本字符串
	bool preloadStoryString(const string& storyScript);

	// 开始进行故事播放
	void playStory();
	// 故事停止播放
	void stopPlay();

	// 故事完成的回调函数
	CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_doneFunc, DoneFunction);
	// 解压资源的解压函数
	CC_SYNTHESIZE(DECRYPT_FUNC, m_decFunc, DecryptFunction);
};
//-------------------------------------------------------------------------