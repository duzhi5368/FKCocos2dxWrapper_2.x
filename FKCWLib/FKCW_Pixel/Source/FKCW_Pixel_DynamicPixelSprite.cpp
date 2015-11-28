//-------------------------------------------------------------------------
#include "../Include/FKCW_Pixel_DynamicPixelSprite.h"
#include "../Include/FKCW_Pixel_BaseFunc.h"
//-------------------------------------------------------------------------
static unsigned char s_1x1WhiteImage[] = 
{
	// RGBA8888
	0xFF, 0xFF,
	0xFF, 0xFF
};

#define CC_1x1_WHITE_IMAGE_KEY  "s_1x1WhiteImage"
//-------------------------------------------------------------------------
FKCW_Pixel_FragSprite::FKCW_Pixel_FragSprite()
{

}
//-------------------------------------------------------------------------
FKCW_Pixel_FragSprite::~FKCW_Pixel_FragSprite()
{

}
//-------------------------------------------------------------------------
bool FKCW_Pixel_FragSprite::init(const string&texFileName)
{
	CCSprite::initWithFile(texFileName.c_str());
	return true;
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_FragSprite::init(CCTexture2D*texture)
{
	CCSprite::initWithTexture(texture);
	return true;
}
//-------------------------------------------------------------------------
// DynamicPixelSprite
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelSprite::~FKCW_Pixel_DynamicPixelSprite()
{

}
//-------------------------------------------------------------------------
CCSpriteBatchNode* FKCW_Pixel_DynamicPixelSprite::getFragRoot()
{
	return m_fragRoot;
}
//-------------------------------------------------------------------------
int FKCW_Pixel_DynamicPixelSprite::getFragMatRowCount()const
{
	return m_fragMat.size();
}
//-------------------------------------------------------------------------
int FKCW_Pixel_DynamicPixelSprite::getFragMatColCount()const
{
	return (getFragMatRowCount()==0?0:(int)m_fragMat[0].size());
}
//-------------------------------------------------------------------------
int FKCW_Pixel_DynamicPixelSprite::getFragMatElementCount()const
{
	return getFragMatRowCount()*getFragMatColCount();
}
//-------------------------------------------------------------------------
vector<FKCW_Pixel_FragSprite*> FKCW_Pixel_DynamicPixelSprite::getShellFrags()
{
	vector<FKCW_Pixel_FragSprite*> shellFragList;
	int nRow=getFragMatRowCount();
	int nCol=getFragMatColCount();
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			if(m_fragMat[i][j]==NULL)continue;
			bool upFragIsNULL;
			if(i-1>=0)
			{
				upFragIsNULL=(m_fragMat[i-1][j]==NULL);
			}
			else
			{
				upFragIsNULL=true;
			}
			bool dnFragIsNULL;
			if(i+1<nRow)
			{
				dnFragIsNULL=(m_fragMat[i+1][j]==NULL);
			}
			else
			{
				dnFragIsNULL=true;
			}
			bool leftFragIsNULL;
			if(j-1>=0)
			{
				leftFragIsNULL=(m_fragMat[i][j-1]==NULL);
			}
			else
			{
				leftFragIsNULL=true;
			}
			bool rightFragIsNULL;
			if(j+1<nCol)
			{
				rightFragIsNULL=(m_fragMat[i][j+1]==NULL);
			}
			else
			{
				rightFragIsNULL=true;
			}
			if(upFragIsNULL==false&&dnFragIsNULL==false&&leftFragIsNULL==false&&rightFragIsNULL==false)
			{
				// 不是shellFrag，什么都不做
			}
			else
			{
				shellFragList.push_back(m_fragMat[i][j]);
			}
		}
	}
	return shellFragList;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelSprite::noise()
{
	int nRow=getFragMatRowCount();
	int nCol=getFragMatColCount();

	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			FKCW_Pixel_FragSprite*fragSprite=m_fragMat[i][j];
			if(fragSprite==NULL)continue;
			float offsetx=(rand()%200-100)*0.01f*4.0f;
			float offsety=(rand()%200-100)*0.01f*4.0f;
			fragSprite->setPosition(fragSprite->getPosition()+CCPoint(offsetx,offsety));
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelSprite::relocateToOrigin()
{
	int nRow=getFragMatRowCount();
	int nCol=getFragMatColCount();

	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			FKCW_Pixel_FragSprite*fragSprite=m_fragMat[i][j];
			if(fragSprite==NULL)continue;
			fragSprite->setPosition(fragSprite->getInitPosition());
		}
	}
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelSprite::init(const string&texFileName,CCSize perPixelSize)
{
	CCNode::init();
	m_perPixelSize = perPixelSize;

	CCTexture2D *pTexture = NULL;
	if(!pTexture || CCTextureCache::sharedTextureCache()->textureForKey(CC_1x1_WHITE_IMAGE_KEY) != pTexture) 
	{
		pTexture = CCTextureCache::sharedTextureCache()->textureForKey(CC_1x1_WHITE_IMAGE_KEY);
		if (!pTexture) 
		{
			CCImage* image = new CCImage();
			bool isOK = image->initWithImageData(s_1x1WhiteImage, sizeof(s_1x1WhiteImage), CCImage::kFmtRawData, 1, 1, 8);
			CCAssert(isOK, "The 1x1 empty texture was created unsuccessfully.");
			pTexture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_1x1_WHITE_IMAGE_KEY);
			CC_SAFE_RELEASE(image);
		}
	}

	CCTexture2D* pixelTex = ScaleTexUsingRTT(pTexture, m_perPixelSize.width,m_perPixelSize.height);
	m_fragRoot = CCSpriteBatchNode::createWithTexture(pixelTex);
	addChild(m_fragRoot);

	CCImage* image=new CCImage();
	image->autorelease();
	bool succ=image->initWithImageFile(texFileName.c_str());
	assert(succ);
	vector<vector<ccColor4B> > pixelMat = GetColorMatFromImage(image);
	int nRow=(int)pixelMat.size();
	int nCol=(nRow==0?0:(int)pixelMat[0].size());
	m_fragMat.resize(nRow);
	for(int i=0;i<nRow;i++)
	{
		m_fragMat[i].resize(nCol);
	}
	float height=nRow*m_perPixelSize.height;
	float width=nCol*m_perPixelSize.width;
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			ccColor4B color=pixelMat[i][j];
			if(color.a==0)
				continue;
			FKCW_Pixel_FragSprite*fragSprite=new FKCW_Pixel_FragSprite();
			fragSprite->autorelease();
			fragSprite->init(pixelTex);
			fragSprite->setColor(ccc3(color.r, color.g, color.b));
			fragSprite->setOpacity(color.a);
			fragSprite->setIJ(i,j);
			m_fragRoot->addChild(fragSprite);

			float x=j*m_perPixelSize.width+m_perPixelSize.width/2-width/2;
			float y=height-(i*m_perPixelSize.height+m_perPixelSize.height/2)-height/2;
			fragSprite->setPosition(ccp(x,y));
			fragSprite->storeInitPosition(ccp(x,y));
			m_fragMat[i][j]=fragSprite;
		}
	}
	m_boundingBoxInLocalSpace=CCRect(-width/2,-height/2,width,height);
	// DEBUG输出
	if(g_isPrintFragMat)
	{
		for(int i=0;i<nRow;i++)
		{
			for(int j=0;j<nCol;j++)
			{
				ccColor4B color=pixelMat[i][j];
				cout<<(color.a>0)<<" ";
			}
			cout<<endl;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelSprite::FKCW_Pixel_DynamicPixelSprite()
{
	m_fragRoot=NULL;
}
//-------------------------------------------------------------------------
FKCW_Pixel_FragSprite* FKCW_Pixel_DynamicPixelSprite::getFragByIndex(int i,int j)
{
	assert(i>=0&&i<getFragMatRowCount());
	assert(j>=0&&j<getFragMatColCount());
	return m_fragMat[i][j];
}
//-------------------------------------------------------------------------
vector<FKCW_Pixel_FragSprite*> FKCW_Pixel_DynamicPixelSprite::getFragsInRect(const CCRect& rectInParentSpace)
{
	vector<FKCW_Pixel_IJ> fragSpriteIndexList = getFragsIndexInRect(rectInParentSpace);
	vector<FKCW_Pixel_FragSprite*> fragSpriteList;
	int nIJ=(int)fragSpriteIndexList.size();
	for(int i=0;i<nIJ;i++)
	{
		const FKCW_Pixel_IJ& IJ=fragSpriteIndexList[i];
		fragSpriteList.push_back(m_fragMat[IJ.getI()][IJ.getJ()]);
	}
	return fragSpriteList;
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::getFragPosition(FKCW_Pixel_FragSprite*frag)
{
	assert(getIsHasFrag(frag));
	return CCPointApplyAffineTransform(frag->getPosition(),nodeToParentTransform());
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::getFragPosition(int i,int j)
{
	FKCW_Pixel_FragSprite*frag=getFragByIndex(i, j);
	assert(frag);
	return CCPointApplyAffineTransform(frag->getPosition(),nodeToParentTransform());
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelSprite::getFragRect(FKCW_Pixel_FragSprite*frag)
{
	assert(getIsHasFrag(frag));
	return CCRectApplyAffineTransform(frag->boundingBox(),nodeToParentTransform());
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelSprite::getMinRectForFrags(const vector<FKCW_Pixel_FragSprite*>&fragList)
{
	int nFrag=(int)fragList.size();
	if(nFrag==0)return CCRect(0,0,0,0);
	for(int i=0;i<nFrag;i++)
	{
		FKCW_Pixel_FragSprite*frag=fragList[i];
		assert(getIsHasFrag(frag));
	}
	float xmin = static_cast<float>(INFINITY);
	float xmax = static_cast<float>(-INFINITY);
	float ymin = static_cast<float>(INFINITY);
	float ymax = static_cast<float>(-INFINITY);
	for(int i=0;i<nFrag;i++)
	{
		FKCW_Pixel_FragSprite*frag=fragList[i];
		const CCRect&rect=getFragRect(frag);
		if(rect.getMinX()<xmin)xmin=rect.getMinX();
		if(rect.getMaxX()>xmax)xmax=rect.getMaxX();
		if(rect.getMinY()<ymin)ymin=rect.getMinY();
		if(rect.getMaxY()>ymax)ymax=rect.getMaxY();
	}
	return CCRect(xmin,ymin,xmax-xmin,ymax-ymin);
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelSprite::getFragRect(int i,int j)
{
	FKCW_Pixel_FragSprite*frag=getFragByIndex(i, j);
	assert(frag);
	return CCRectApplyAffineTransform(frag->boundingBox(),nodeToParentTransform());
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelSprite::getIsHasFrag(int i,int j)
{
	int nRow=getFragMatRowCount();
	int nCol=getFragMatColCount();
	if(i<0)
		return false;
	if(j<0)
		return false;
	if(i>=nRow)
		return false;
	if(j>=nCol)
		return false;

	if(m_fragMat[i][j]==NULL)return false;
	return true;
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelSprite::getIsHasFrag(FKCW_Pixel_FragSprite*frag)
{
	assert(frag);
	int I=frag->getI();
	int J=frag->getJ();
	if(getIsHasFrag(I,J)==false)
		return false;
	if(m_fragMat[I][J]!=frag)
		return false;
	return true;
}
//-------------------------------------------------------------------------
vector<FKCW_Pixel_IJ> FKCW_Pixel_DynamicPixelSprite::getFragsIndexInRect(const CCRect& rectInParentSpace)
{
	vector<FKCW_Pixel_IJ> fragIndexList;
	// 换换矩形到节点空间
	CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
	CCRect rectInNodeSpace=CCRectApplyAffineTransform(rectInParentSpace, parentToNodeTransform);
	// 矩形范围（本地空间）
	float xmin_rt=rectInNodeSpace.getMinX();
	float xmax_rt=rectInNodeSpace.getMaxX();
	float ymin_rt=rectInNodeSpace.getMinY();
	float ymax_rt=rectInNodeSpace.getMaxY();
	// 精灵范围（本地空间）
	float xmin_sp=m_boundingBoxInLocalSpace.getMinX();
	float xmax_sp=m_boundingBoxInLocalSpace.getMaxX();
	float ymin_sp=m_boundingBoxInLocalSpace.getMinY();
	float ymax_sp=m_boundingBoxInLocalSpace.getMaxY();
	// 计算坐标范围索引区域
	int imin,imax,jmin,jmax;
	imin=static_cast<int>(MAX(0,floorf((ymax_sp-ymax_rt)/m_perPixelSize.height)));
	imax=static_cast<int>(MIN(getFragMatRowCount()-1,ceilf((ymax_sp-ymin_rt)/m_perPixelSize.height)-1));
	jmin=static_cast<int>(MAX(0,floorf((xmin_rt-xmin_sp)/m_perPixelSize.width)));
	jmax=static_cast<int>(MIN(getFragMatColCount()-1,ceilf((xmax_rt-xmin_sp)/m_perPixelSize.width)-1));
	// 聚集全部片段精灵到一个区域 [imin,imax]x[jmin,jmax] 中
	for(int i=imin;i<=imax;i++)
	{
		for(int j=jmin;j<=jmax;j++)
		{
			if(m_fragMat[i][j])
			{
				fragIndexList.push_back(FKCW_Pixel_IJ(i,j));
			}
		}
	}
	return fragIndexList;
}
//-------------------------------------------------------------------------
vector<FKCW_Pixel_IJ> FKCW_Pixel_DynamicPixelSprite::getFragsIndexInRectGeneral(const CCRect& rectInParentSpace)
{
	vector<FKCW_Pixel_IJ> fragIndexList;
	// 转换矩形到节点空间
	CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
	CCRect rectInNodeSpace=CCRectApplyAffineTransform(rectInParentSpace, parentToNodeTransform);
	int nRow=getFragMatRowCount();
	int nCol=getFragMatColCount();
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			if(getIsHasFrag(i, j)==false)continue;
			FKCW_Pixel_FragSprite*frag=m_fragMat[i][j];
			CCRect fragRect=frag->boundingBox();

			if(rectInNodeSpace.intersectsRect(fragRect))
			{
				fragIndexList.push_back(FKCW_Pixel_IJ(i,j));
			}
		}
	}
	return fragIndexList;
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelSprite::getBoundingBoxInLocalSpace()const 
{
	return m_boundingBoxInLocalSpace;
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelSprite::getBoundingBox() 
{
	return CCRectApplyAffineTransform(m_boundingBoxInLocalSpace, nodeToParentTransform());
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::downDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
{
	CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayWidth/2,rayStartPoint.y-rayLen,rayWidth,rayLen);
	vector<FKCW_Pixel_FragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
	float maxY = static_cast<float>(-INFINITY);
	float x=0;
	int nCollisionFrag=(int)collisionFrags.size();
	// 计算maxY
	for(int i=0;i<nCollisionFrag;i++)
	{
		// 转换到其父空间
		CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());
		if(frageRect_parentSpace.getMaxY()>maxY)
		{
			maxY=frageRect_parentSpace.getMaxY();
			x=(frageRect_parentSpace.getMinX()+frageRect_parentSpace.getMaxX())/2;
		}
	}
	CCPoint intersectPoint=CCPoint(x,maxY);
	return intersectPoint;
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::upDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
{
	CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayWidth/2,rayStartPoint.y,rayWidth,rayLen);
	vector<FKCW_Pixel_FragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
	float minY=static_cast<float>(INFINITY);
	float x=0;
	int nCollisionFrag=(int)collisionFrags.size();
	// 计算maxY
	for(int i=0;i<nCollisionFrag;i++)
	{
		// 转换到其父空间
		CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());
		if(frageRect_parentSpace.getMaxY()<minY)
		{
			minY=frageRect_parentSpace.getMaxY();
			x=(frageRect_parentSpace.getMinX()+frageRect_parentSpace.getMaxX())/2;
		}
	}
	CCPoint intersectPoint=CCPoint(x,minY);
	return intersectPoint;
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::leftDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
{
	CCRect rect_parentSpace=CCRect(rayStartPoint.x-rayLen,rayStartPoint.y-rayWidth/2,rayLen,rayWidth);
	vector<FKCW_Pixel_FragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
	float maxX=static_cast<float>(-INFINITY);
	float y=0;
	int nCollisionFrag=(int)collisionFrags.size();
	// 计算maxX
	for(int i=0;i<nCollisionFrag;i++)
	{
		// 转换到其父空间
		CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());
		if(frageRect_parentSpace.getMaxX()<maxX)
		{
			maxX=frageRect_parentSpace.getMaxX();
			y=(frageRect_parentSpace.getMinY()+frageRect_parentSpace.getMaxY())/2;
		}
	}
	CCPoint intersectPoint=CCPoint(maxX,y);
	return intersectPoint;
}
//-------------------------------------------------------------------------
CCPoint FKCW_Pixel_DynamicPixelSprite::rightDirRayIntersectWithMe(const CCPoint&rayStartPoint,float rayLen,float rayWidth)
{
	CCRect rect_parentSpace=CCRect(rayStartPoint.x,rayStartPoint.y-rayWidth/2,rayLen,rayWidth);
	vector<FKCW_Pixel_FragSprite*> collisionFrags=this->getFragsInRect(rect_parentSpace);
	float minX=static_cast<float>(INFINITY);
	float y=0;
	int nCollisionFrag=(int)collisionFrags.size();
	for(int i=0;i<nCollisionFrag;i++)
	{
		// 转换到其父空间
		CCRect frageRect_parentSpace=CCRectApplyAffineTransform(collisionFrags[i]->boundingBox(),nodeToParentTransform());
		if(frageRect_parentSpace.getMaxX()<minX)
		{
			minX=frageRect_parentSpace.getMaxX();
			y=(frageRect_parentSpace.getMinY()+frageRect_parentSpace.getMaxY())/2;
		}
	}
	CCPoint intersectPoint=CCPoint(minX,y);
	return intersectPoint;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelSprite::draw()
{
	if(g_isShowBoundingBox)
	{
		CCRect rect=m_boundingBoxInLocalSpace;
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
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelSprite::killFrag(int i,int j)
{
	assert(getIsHasFrag(i, j));
	m_fragMat[i][j]->removeFromParentAndCleanup(true);
	m_fragMat[i][j]=NULL;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelSprite::killFrag(FKCW_Pixel_FragSprite*frag)
{
	assert(getIsHasFrag(frag));
	FKCW_Pixel_IJ IJ=frag->getIJ();
	m_fragMat[IJ.getI()][IJ.getJ()]->removeFromParentAndCleanup(true);
	m_fragMat[IJ.getI()][IJ.getJ()]=NULL;
}
//-------------------------------------------------------------------------