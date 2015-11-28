//-------------------------------------------------------------------------
#include "../Include/FKCW_Pixel_DynamicPixelModel.h"
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelModel::adddpSprite(FKCW_Pixel_DynamicPixelSprite*dpSprite)
{
	assert(dpSprite);
	m_dpSpriteList.push_back(dpSprite);
	addChild(dpSprite);
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelSprite*FKCW_Pixel_DynamicPixelModel::getdpSpriteByIndex(int index)
{
	assert(index>=0&&index<(int)m_dpSpriteList.size());
	return m_dpSpriteList[index];
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelModel::addCollisionRectInLocalSpace(const FKCW_Pixel_CollisionRect&localSpaceCollisionRect)
{
	m_collisionRectListInLocalSpace.push_back(localSpaceCollisionRect);
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelModel::getCollisionRectByIndex(int index) 
{
	assert(index>=0&&index<(int)m_collisionRectListInLocalSpace.size());
	return CCRectApplyAffineTransform(m_collisionRectListInLocalSpace[index].getRect(), nodeToParentTransform());
}
//-------------------------------------------------------------------------
int FKCW_Pixel_DynamicPixelModel::getCollisionRectIndexByName(const string&name)
{
	int nRect=(int)m_collisionRectListInLocalSpace.size();
	int index=-1;
	for(int i=0;i<nRect;i++)
	{
		if(m_collisionRectListInLocalSpace[i].getName()==name)
		{
			index=i;
			break;
		}
	}
	return index;
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelModel::getCollisionRectByName(const string&name)
{
	int index=getCollisionRectIndexByName(name);
	if(index==-1)
	{
		cout<<"error: not find!"<<endl;
		assert(false);
		return CCRectZero;
	}
	else
	{
		return getCollisionRectByIndex(index);
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelModel::draw()
{
	if(g_isShowBoundingBox)
	{
		int nCollisionRect=(int)m_collisionRectListInLocalSpace.size();
		for(int i=0;i<nCollisionRect;i++)
		{
			CCRect rect=m_collisionRectListInLocalSpace[i].getRect();
			// v3 ---- v2
			//    |            |
			// v0 ---- v1
			CCPoint v0=CCPoint(rect.getMinX(),rect.getMinY());
			CCPoint v1=CCPoint(rect.getMaxX(),rect.getMinY());
			CCPoint v2=CCPoint(rect.getMaxX(),rect.getMaxY());
			CCPoint v3=CCPoint(rect.getMinX(),rect.getMaxY());
			ccDrawLine(v0, v1);
			ccDrawLine(v1, v2);
			ccDrawLine(v2, v3);
			ccDrawLine(v3, v0);
		}
	}
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelModel::getBoundingBoxInLocalSpace()const
{
	float xmin=static_cast<float>(INFINITY);
	float xmax=static_cast<float>(-INFINITY);
	float ymin=static_cast<float>(INFINITY);
	float ymax=static_cast<float>(-INFINITY);
	int ndpSprite=(int)m_dpSpriteList.size();
	if(ndpSprite==0)
	{
		return CCRect(0,0,0,0);
	}
	for(int i=0;i<ndpSprite;i++)
	{
		FKCW_Pixel_DynamicPixelSprite*dpSprite=m_dpSpriteList[i];
		CCRect rect=dpSprite->getBoundingBox();
		if(rect.getMinX()<xmin)xmin=rect.getMinX();
		if(rect.getMaxX()>xmax)xmax=rect.getMaxX();
		if(rect.getMinY()<ymin)ymin=rect.getMinY();
		if(rect.getMaxY()>ymax)ymax=rect.getMaxY();
	}
	return CCRect(xmin,ymin,xmax-xmin,ymax-ymin);
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelModel::getBoundingBox() 
{
	return CCRectApplyAffineTransform(getBoundingBoxInLocalSpace(), nodeToParentTransform());
}
//-------------------------------------------------------------------------