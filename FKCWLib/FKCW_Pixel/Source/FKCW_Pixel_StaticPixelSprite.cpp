//-------------------------------------------------------------------------
#include "../Include/FKCW_Pixel_StaticPixelSprite.h"
#include "../Include/FKCW_Pixel_BaseFunc.h"
//-------------------------------------------------------------------------
FKCW_Pixel_StaticPixelSprite::FKCW_Pixel_StaticPixelSprite()
{

}
//-------------------------------------------------------------------------
FKCW_Pixel_StaticPixelSprite::~FKCW_Pixel_StaticPixelSprite()
{

}
//-------------------------------------------------------------------------
bool FKCW_Pixel_StaticPixelSprite::init(const string&texFileName,CCSize perPixelSize)
{
	m_perPixelSize=perPixelSize;
	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(texFileName.c_str());
	ccTexParams texParams = {GL_NEAREST,GL_NEAREST,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
	tex->setTexParameters(&texParams);
	CCTexture2D*newTex = ScaleTexUsingRTT(tex,m_perPixelSize.width,m_perPixelSize.height);
	CCSprite::initWithTexture(newTex);
	return true;
}
//-------------------------------------------------------------------------