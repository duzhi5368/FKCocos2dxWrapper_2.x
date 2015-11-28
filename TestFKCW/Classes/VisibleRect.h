/*!
 * \file:	CVisibleRect.h
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#pragma once
//--------------------------------------------------------
#include "cocos2d.h"
//--------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------
class CVisibleRect
{
public:
    static CCRect getVisibleRect();

    static CCPoint left();
    static CCPoint right();
    static CCPoint top();
    static CCPoint bottom();
    static CCPoint center();
    static CCPoint leftTop();
    static CCPoint rightTop();
    static CCPoint leftBottom();
    static CCPoint rightBottom();
private:
    static void lazyInit();
    static CCRect s_VisibleRect;
};
//--------------------------------------------------------