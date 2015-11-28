//*************************************************************************
//	创建日期:	2015-1-4
//	文件名称:	Test_Story_StoryLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Story_StoryLayerLayer : public CCLayer
{
public:
	void onEnter()
	{
		CCLayer::onEnter();

		FKCW_Story_Layer* storyLayer = FKCW_Story_Layer::create();
		addChild(storyLayer);
		storyLayer->preloadStoryFile( s_szFKScriptFile );
		storyLayer->playStory();
	}
	void onExit()
	{
		CCLayer::onExit();
	}


public:
	CREATE_FUNC( CTest_Story_StoryLayerLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Story, StoryLayer, AVG故事板及脚本测试)
//-------------------------------------------------------------------------