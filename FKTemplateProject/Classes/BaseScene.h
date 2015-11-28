#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class BaseLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
	BaseLayer();
public:
	
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	// WINDOWS ÏûÏ¢
	static void KeyboardHook( UINT message,WPARAM wParam, LPARAM lParam );
#endif
public:
    CREATE_FUNC(BaseLayer);
};
//-------------------------------------------------------------------------
#endif // __HELLOWORLD_SCENE_H__
