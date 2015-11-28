//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_Geometry.h"
#include "../Include/FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
CCSize ccpDiff(const CCPoint& tPoint1, const CCPoint& tPoint2)
{
	return ccz(tPoint1.x - tPoint2.x, tPoint1.y - tPoint2.y);
}
//-------------------------------------------------------------------------
CCPoint ccpShift(const CCPoint& tPoint, const CCSize& tSize)
{
	return ccp(tPoint.x + tSize.width, tPoint.y + tSize.height);
}
//-------------------------------------------------------------------------
CCPoint ccpNormalizeTo(const CCPoint& tPoint, const CCSize& tNorm)
{
	return ccp(tPoint.x / tNorm.width, tPoint.y / tNorm.height);
}
//-------------------------------------------------------------------------
CCPoint ccpDenormalizeTo(const CCPoint& tPoint, const CCSize& tNorm)
{
	return ccp(tPoint.x * tNorm.width, tPoint.y * tNorm.height);
}
//-------------------------------------------------------------------------
CCSize cczFromPoint(const CCPoint& tPoint)
{
	return ccz(tPoint.x, tPoint.y);
}
//-------------------------------------------------------------------------
float cczLength(const CCSize& tSize)
{
	return sqrtf(tSize.width * tSize.width + tSize.height * tSize.height);
}
//-------------------------------------------------------------------------
CCSize cczAdd(const CCSize& tSize1, const CCSize& tSize2)
{
	return ccz(tSize1.width + tSize2.width, tSize1.height + tSize2.height);
}
//-------------------------------------------------------------------------
CCSize cczSub(const CCSize& tSize1, const CCSize& tSize2)
{
	return ccz(tSize1.width - tSize2.width, tSize1.height - tSize2.height);
}
//-------------------------------------------------------------------------
CCSize cczNormalizeTo(const CCSize& tSize, const CCSize& tNorm)
{
	return ccz(tSize.width / tNorm.width, tSize.height / tNorm.height);
}
//-------------------------------------------------------------------------
CCSize cczDenormalizeTo(const CCSize& tSize, const CCSize& tNorm)
{
	return ccz(tSize.width * tNorm.width, tSize.height * tNorm.height);
}
//-------------------------------------------------------------------------
CCSize cczClamp(const CCSize& tSize, const CCSize& tLimit1, const CCSize& tLimit2)
{
	return ccz(clampf(tSize.width, tLimit1.width, tLimit2.width), clampf(tSize.height, tLimit1.height, tLimit2.height));
}
//-------------------------------------------------------------------------
CCSize cczMin(const CCSize& tSize1, const CCSize& tSize2)
{
	return ccz(MIN(tSize1.width, tSize2.width), MIN(tSize1.height, tSize2.height));
}
//-------------------------------------------------------------------------
CCSize cczMax(const CCSize& tSize1, const CCSize& tSize2)
{
	return ccz(MAX(tSize1.width, tSize2.width), MAX(tSize1.height, tSize2.height));
}
//-------------------------------------------------------------------------
CCRect ccRectMakeWithCenterAndSize(const CCPoint& tCenter, const CCSize& tSize)
{
	return CCRectMake(tCenter.x - 0.5f * tSize.width, tCenter.y - 0.5f * tSize.height, tSize.width, tSize.height);
}
//-------------------------------------------------------------------------
CCRect ccRectMakeWithOriginAndSize(const CCPoint& tOrigin, const CCSize& tSize)
{
	return CCRectMake(tOrigin.x, tOrigin.y, tSize.width, tSize.height);
}
//-------------------------------------------------------------------------
CCPoint ccRectGetCenter(const CCRect& tRect)
{
	return ccp(tRect.getMidX(), tRect.getMidY());
}
//-------------------------------------------------------------------------
CCSize ccRectGetSize(const CCRect& tRect)
{
	return CCSizeMake(tRect.getMaxX()-tRect.getMinX(),tRect.getMaxY()-tRect.getMinY());
}
//-------------------------------------------------------------------------
bool ccIsZeroRect(const CCRect& tRect)
{
	if(tRect.getMinX()==0 && tRect.getMaxX()==0
		&& tRect.getMinY()==0 && tRect .getMaxY()==0)
		return true;
	else 
		return false;
}
//-------------------------------------------------------------------------