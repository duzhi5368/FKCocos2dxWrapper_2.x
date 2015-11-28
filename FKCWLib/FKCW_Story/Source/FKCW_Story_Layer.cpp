//*************************************************************************
//	创建日期:	2015-1-4
//	文件名称:	FKCW_Story_Layer.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../Include/FKCW_Story_Layer.h"
#include "../Include/FKCW_Story_CommandSet.h"
#include "../include/FKCW_Story_Player.h"
//-------------------------------------------------------------------------
static CCDictionary sGlobalParams;
//-------------------------------------------------------------------------
FKCW_Story_Layer::FKCW_Story_Layer()
	:m_player(NULL),
	m_doneFunc(NULL),
	m_decFunc(NULL),
	m_playing(false)
{

}
//-------------------------------------------------------------------------
FKCW_Story_Layer::~FKCW_Story_Layer()
{
	CC_SAFE_RELEASE(m_player);
	CC_SAFE_RELEASE(m_doneFunc);
	unloadImageFiles();
	gStoryCommandSet.removeAllObjects();
}
//-------------------------------------------------------------------------
FKCW_Story_Layer* FKCW_Story_Layer::create() 
{
	FKCW_Story_Layer* l = new FKCW_Story_Layer();
	if(l->init()) {
		return (FKCW_Story_Layer*)l->autorelease();
	}
	CC_SAFE_RELEASE(l);
	return NULL;
}
//-------------------------------------------------------------------------
string FKCW_Story_Layer::getParameter(const string& key) 
{
	CCString* v = (CCString*)sGlobalParams.objectForKey(key);
	if(v)
		return v->getCString();
	else
		return "";
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::setParameter(const string& key, const string& value) 
{
	sGlobalParams.setObject(CCString::create(value), key);
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::onExit() 
{
	CCLayer::onExit();
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::onEnter() 
{
	CCLayer::onEnter();
}
//-------------------------------------------------------------------------
bool FKCW_Story_Layer::init() 
{
	if(!CCLayer::init())
		return false;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::unloadImageFiles() 
{
	for(vector<string>::iterator iter = m_loadedImageFiles.begin(); iter != m_loadedImageFiles.end(); iter++) {
		CCTextureCache::sharedTextureCache()->removeTextureForKey(iter->c_str());
	}
	m_loadedImageFiles.clear();
}
//-------------------------------------------------------------------------
bool FKCW_Story_Layer::preloadStoryFile(const string& storyScriptFile, DECRYPT_FUNC decFunc) 
{
	string script = FKCW_Base_ResourceLoader::LoadString(storyScriptFile, decFunc);
	return preloadStoryString(script);
}
//-------------------------------------------------------------------------
bool FKCW_Story_Layer::preloadStoryString(const string& storyScript)
{
	gStoryCommandSet.removeAllObjects();
	unloadImageFiles();

	// 加载资源
	FKCW_Base_ByteBuffer bb(storyScript.c_str(), storyScript.length(), storyScript.length());
	string line;
	bb.ReadLine(line);
	while(!line.empty())
	{
		if(line.find("#--$") == 0) 
		{
			string resCmd = line.substr(4);
			int comma = (int)resCmd.find(",");
			string type = resCmd.substr(0, comma);
			if(type == "atlas") 
			{
				int secondComma = (int)resCmd.find(",", comma + 1);
				string plist = resCmd.substr(comma + 1, secondComma - comma - 1);
				string atlas = resCmd.substr(secondComma + 1);
				FKCW_Base_ResourceLoader::LoadZwoptex(plist, atlas, m_decFunc);
			} 
			else if(type == "arm") 
			{
				string json = resCmd.substr(comma + 1);
				cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(json.c_str());
			} 
			else if(type == "image")
			{
				string filename = resCmd.substr(comma + 1);
				string ext = FKCW_Base_Utils::GetPathExtension(filename);
				string path;
				if(ext != ".png" && ext != ".jpg" && ext != ".jpeg") 
				{
					path = filename + ".png";
				} 
				else 
				{
					path = filename;
				}
				m_loadedImageFiles.push_back(path);
				FKCW_Base_ResourceLoader::LoadImage(path, m_decFunc);
			}
		}
		else
		{
			// 注释
		}

		bb.ReadLine(line);
	}

	// 开始加载
	if(!m_player) 
	{
		m_player = FKCW_Story_Player::create(this);
		CC_SAFE_RETAIN(m_player);
	}
	int ret = m_player->ParserStoryScriptString(storyScript.c_str());
	return ret == 0;
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::playStory() 
{
	if(m_playing)
		return;
	m_playing = true;

	if(!m_player) 
	{
		m_player = FKCW_Story_Player::create(this);
		CC_SAFE_RETAIN(m_player);
	}

	m_player->start();
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::stopPlay() 
{
	if(!m_playing)
		return;
	m_playing = false;

	stopAllActions();
	CC_SAFE_RELEASE_NULL(m_player);
	removeAllChildren();
}
//-------------------------------------------------------------------------
void FKCW_Story_Layer::onStoryDone() 
{
	if(m_doneFunc)
	{
		m_doneFunc->execute();
	}
}
//-------------------------------------------------------------------------