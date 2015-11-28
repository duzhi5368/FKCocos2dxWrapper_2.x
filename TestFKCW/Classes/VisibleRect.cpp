/*!
 * \author:	FreeKnight
 * \date:	2014-6-25
 * \comment:
 */
//--------------------------------------------------------
#include "VisibleRect.h"
//--------------------------------------------------------
CCRect CVisibleRect::s_VisibleRect;
//--------------------------------------------------------
void CVisibleRect::lazyInit()
{
    if (s_VisibleRect.size.width == 0.0f && s_VisibleRect.size.height == 0.0f)
    {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        s_VisibleRect.origin = pEGLView->getVisibleOrigin();
        s_VisibleRect.size = pEGLView->getVisibleSize();
    }
}
//--------------------------------------------------------
CCRect CVisibleRect::getVisibleRect()
{
    lazyInit();
    return CCRectMake(s_VisibleRect.origin.x, s_VisibleRect.origin.y, s_VisibleRect.size.width, s_VisibleRect.size.height);
}
//--------------------------------------------------------
CCPoint CVisibleRect::left()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x, s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//--------------------------------------------------------
CCPoint CVisibleRect::right()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//--------------------------------------------------------
CCPoint CVisibleRect::top()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//--------------------------------------------------------
CCPoint CVisibleRect::bottom()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, s_VisibleRect.origin.y);
}
//--------------------------------------------------------
CCPoint CVisibleRect::center()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width/2, s_VisibleRect.origin.y+s_VisibleRect.size.height/2);
}
//--------------------------------------------------------
CCPoint CVisibleRect::leftTop()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x, s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//--------------------------------------------------------
CCPoint CVisibleRect::rightTop()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, s_VisibleRect.origin.y+s_VisibleRect.size.height);
}
//--------------------------------------------------------
CCPoint CVisibleRect::leftBottom()
{
    lazyInit();
    return s_VisibleRect.origin;
}
//--------------------------------------------------------
CCPoint CVisibleRect::rightBottom()
{
    lazyInit();
    return ccp(s_VisibleRect.origin.x+s_VisibleRect.size.width, s_VisibleRect.origin.y);
}
//--------------------------------------------------------