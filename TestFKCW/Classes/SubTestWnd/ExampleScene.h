//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	ExampleScene.h
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
class CExampleLayer : public CCLayer
{
public:
	virtual bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );

			// 这里做部分初始化工作
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCLabelTTF* pInfo = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("本程序是 FreeKnight 对cocos2dx 2.x版本的Wrapper库 的测试用例").c_str(), "", 26 );
			pInfo->setPosition( ccp( winSize.width / 2, winSize.height / 2 ));
			pInfo->setColor( ccRED );
			this->addChild(pInfo);

			return true;
		} while ( 0 );
		return false;
	}
public:
	CREATE_FUNC( CExampleLayer );
};
//-------------------------------------------------------------------------
// 下面的场景注意在ListControllerLayer中注册
// CREATE_TEST_SCENE_CLASS(MainType, SubType, Desc)
//-------------------------------------------------------------------------
class CExampleScene : public CTestScene
{
public:
	CREATE_SCENE_FUNC(CExampleScene);
public:
	CExampleScene()
	{
		SetInfo("这是一个测试场景，以便学习");
	}
public:
	// 这里做部分初始化工作,可以理解是main
	virtual void Run()
	{
		CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccGRAY )));
		layer->setColor( ccGRAY );
		addChild(layer);

		CExampleLayer* pLayer = (CExampleLayer*)CExampleLayer::create();
		if( pLayer )
		{
			this->addChild( pLayer );
		}
	}
};
//-------------------------------------------------------------------------