//*************************************************************************
//	创建日期:	2015-1-6
//	文件名称:	BaseScene.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseScene.h"
#include "Resource.h"
#include "cocos2d.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
CCScene* BaseLayer::scene()
{
    CCScene *scene = CCScene::create();
    BaseLayer *layer = BaseLayer::create();
    scene->addChild(layer);
    
    CCLabelTTF* pInfo = CCLabelTTF::create("hello FKCW", "Arial", 26 );
		pInfo->setColor( ccc3(209,73,78) );
		pInfo->setPosition( ccp(600,300));
		layer->addChild( pInfo);
		
    return scene;
}
//-------------------------------------------------------------------------
BaseLayer::BaseLayer()
{

}
//-------------------------------------------------------------------------
bool BaseLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    	
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	// 添加键盘反馈
	CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook( BaseLayer::KeyboardHook ); 
#endif
	
	// add your code here
    
    return true;
}

//-------------------------------------------------------------------------
// WINDOWS 消息
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	void BaseLayer::KeyboardHook( UINT message,WPARAM wParam, LPARAM lParam )
{

}
#endif
//-------------------------------------------------------------------------