//-------------------------------------------------------------------------
#include "../Include/FKCW_Pixel_DynamicPixelActor.h"
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniFrame::FKCW_Pixel_DynamicPixelAniFrame()
{
	m_durTime=0;
	m_dpModel=NULL;
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniFrame::~FKCW_Pixel_DynamicPixelAniFrame()
{
	if(m_dpModel)m_dpModel->release();
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelAniFrame::setdpModel(FKCW_Pixel_DynamicPixelModel*dpModel)
{
	assert(dpModel);
	if(m_dpModel==NULL)
	{
		m_dpModel=dpModel;
		m_dpModel->retain();
	}
	else
	{
		m_dpModel->release();
		m_dpModel=dpModel;
		m_dpModel->retain();
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelAniFrame::setDurTime(float durTime)
{
	m_durTime=durTime;
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelAniFrame::init(FKCW_Pixel_DynamicPixelModel*dpModel,float durTime){
	setdpModel(dpModel);
	setDurTime(durTime);
	return true;
}
//-------------------------------------------------------------------------
// DynamicPixelAniLayer
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniLayer::~FKCW_Pixel_DynamicPixelAniLayer()
{
	int nAniFrame=(int)m_aniFrameList.size();
	for(int i=0;i<nAniFrame;i++)
	{
		m_aniFrameList[i]->release();
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelAniLayer::addAniFrame(FKCW_Pixel_DynamicPixelAniFrame*aniFrame)
{
	assert(aniFrame);
	m_aniFrameList.push_back(aniFrame);
	aniFrame->retain();
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniFrame* FKCW_Pixel_DynamicPixelAniLayer::getAniFrameByIndex(int index)
{
	assert(index>=0&&index<(int)m_aniFrameList.size());
	return m_aniFrameList[index];
}
//-------------------------------------------------------------------------
float FKCW_Pixel_DynamicPixelAniLayer::getAniDurTime()const
{
	int nAniFrame=(int)m_aniFrameList.size();
	float timeSum=0;
	for(int i=0;i<nAniFrame;i++){
		timeSum+=m_aniFrameList[i]->getDurTime();
	}
	return timeSum;
}
//-------------------------------------------------------------------------
// DynamicPixelAniStack
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniStack::~FKCW_Pixel_DynamicPixelAniStack(){
	int nAniLayer=(int)m_aniLayerList.size();
	for(int i=0;i<nAniLayer;i++)
	{
		m_aniLayerList[i]->release();
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelAniStack::addAniLayer(FKCW_Pixel_DynamicPixelAniLayer*aniLayer)
{
	assert(aniLayer);
	m_aniLayerList.push_back(aniLayer);
	aniLayer->retain();
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniLayer* FKCW_Pixel_DynamicPixelAniStack::getAniLayerByIndex(int index)
{
	assert(index>=0&&index<(int)m_aniLayerList.size());
	return m_aniLayerList[index];
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelAniLayer* FKCW_Pixel_DynamicPixelAniStack::getAniLayerByName(const string&aniName)
{
	int aniLayerIndex=getAniLayerIndexByName(aniName);
	if(aniLayerIndex==-1)
	{
		return NULL;
	}
	else
	{
		return m_aniLayerList[aniLayerIndex];
	}
}
//-------------------------------------------------------------------------
int FKCW_Pixel_DynamicPixelAniStack::getAniLayerIndexByName(const string&aniName)
{
	int nAniLayer=(int)m_aniLayerList.size();
	for(int i=0;i<nAniLayer;i++)
	{
		if(aniName==m_aniLayerList[i]->getAniName())
		{
			return i;
		}
	}
	return -1;
}
//-------------------------------------------------------------------------
// DynamicPixelActor
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelActor::FKCW_Pixel_DynamicPixelActor()
{
	m_displayNode=NULL;
	m_aniStack=NULL;
	m_dpModel=NULL;
	init_dft();
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelActor::~FKCW_Pixel_DynamicPixelActor()
{
	if(m_aniStack)m_aniStack->release();
	if(m_dpModel)m_dpModel->release();
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelActor::init(const string&texFileName,CCSize perPixelSize)
{
	FKCW_Pixel_DynamicPixelSprite *dpSprite=new FKCW_Pixel_DynamicPixelSprite();
	dpSprite->autorelease();
	dpSprite->init(texFileName,perPixelSize);
	m_dpModel->adddpSprite(dpSprite);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActor::setAniStack(FKCW_Pixel_DynamicPixelAniStack*aniStack)
{
	assert(aniStack);
	if(m_aniStack==NULL)
	{
		m_aniStack=aniStack;
		m_aniStack->retain();
	}
	else
	{
		m_aniStack->release();
		m_aniStack=aniStack;
		m_aniStack->retain();
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActor::setdpModel(FKCW_Pixel_DynamicPixelModel*dpModel)
{
	assert(dpModel);
	if(m_dpModel==NULL)
	{
		m_dpModel=dpModel;
		m_dpModel->retain();
	}
	else
	{
		m_dpModel->release();
		m_dpModel=dpModel;
		m_dpModel->retain();
	}
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActor::aniTo(float t,int aniLayerIndex)
{
	FKCW_Pixel_DynamicPixelAniLayer*aniLayer=getAniStack()->getAniLayerByIndex(aniLayerIndex);
	float aniDurTime=aniLayer->getAniDurTime();
	t=t-floor(t/aniDurTime)*aniDurTime;

	int frameIndex=0;
	float timeSum=0;
	while(1)
	{
		timeSum+=aniLayer->getAniFrameByIndex(frameIndex)->getDurTime();
		if(timeSum>t)
		{
			break;
		}
		frameIndex++;
		if(frameIndex>=aniLayer->getAniFrameCount()){
			frameIndex=aniLayer->getAniFrameCount();
			break;
		}
	}

	FKCW_Pixel_DynamicPixelModel*dpModel=aniLayer->getAniFrameByIndex(frameIndex)->getdpModel();
	m_displayNode->removeAllChildrenWithCleanup(true);
	m_displayNode->addChild(dpModel);
}
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelModel* FKCW_Pixel_DynamicPixelActor::getCurDisplayModel()
{
	CCArray*children=m_displayNode->getChildren();
	int nChild=children->count();
	assert(nChild==1);
	return (FKCW_Pixel_DynamicPixelModel*)children->objectAtIndex(0);
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getBoundingBoxInLocalSpace()
{
	return m_dpModel->getBoundingBox();
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getBoundingBox()
{
	return CCRectApplyAffineTransform(getBoundingBoxInLocalSpace(), nodeToParentTransform());
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getCurrentFrameBoundingBoxInLocalSpace()
{
	FKCW_Pixel_DynamicPixelModel*curModel=getCurDisplayModel();
	assert(curModel);
	return curModel->getBoundingBox();
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getCurrentFrameCollisionRectByIndex(int index)
{
	FKCW_Pixel_DynamicPixelModel*curModel=getCurDisplayModel();
	assert(curModel);
	return convertRectToParent(curModel->getCollisionRectByIndex(index));
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getCurrentFrameCollisionRectByName(const string&name)
{
	FKCW_Pixel_DynamicPixelModel*curModel=getCurDisplayModel();
	assert(curModel);
	return convertRectToParent(curModel->getCollisionRectByName(name));
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_DynamicPixelActor::getCurrentFrameBoundingBox()
{
	return CCRectApplyAffineTransform(getCurrentFrameBoundingBoxInLocalSpace(),
		nodeToParentTransform());
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActor::init_dft()
{
	m_displayNode=CCNode::create();
	this->addChild(m_displayNode,1);

	FKCW_Pixel_DynamicPixelModel*dpModel=new FKCW_Pixel_DynamicPixelModel();
	dpModel->autorelease();
	setdpModel(dpModel);
	m_displayNode->addChild(dpModel);
}
//-------------------------------------------------------------------------
// DynamicPixelActorAnimate
//-------------------------------------------------------------------------
FKCW_Pixel_DynamicPixelActorAnimate* FKCW_Pixel_DynamicPixelActorAnimate::create(float fDuration,int aniLayerIndex){

	FKCW_Pixel_DynamicPixelActorAnimate *p = new FKCW_Pixel_DynamicPixelActorAnimate();
	p->initWithDuration(fDuration,aniLayerIndex);
	p->autorelease();

	return p;
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_DynamicPixelActorAnimate::initWithDuration(float fDuration,int aniLayerIndex){
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_aniLayerIndex=aniLayerIndex;
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActorAnimate::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
}
//-------------------------------------------------------------------------
void FKCW_Pixel_DynamicPixelActorAnimate::update(float time)
{
	float progressPercentage=time;
	float curTime=progressPercentage*getDuration();
	((FKCW_Pixel_DynamicPixelActor*)m_pTarget)->aniTo(curTime,m_aniLayerIndex);
}
//-------------------------------------------------------------------------