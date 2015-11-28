//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_Shatter.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
//-------------------------------------------------------------------------
FKCW_EffectNode_ShatterSprite::FKCW_EffectNode_ShatterSprite()
{
	m_gridSideLen=2.7f;
	m_initalFrageScale=4.5f;
	m_fragBatchNode=NULL;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_ShatterSprite::~FKCW_EffectNode_ShatterSprite()
{
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_ShatterSprite::init(const string& texFileName)
{
	this->CCSprite::initWithFile(texFileName.c_str());
	_CreateShatter();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_ShatterSprite::_CreateShatter()
{
	// 计算格子大小，以及碎片个数
	CCSize contentSize=this->getContentSize();
	const int nRow=(int)floorf(contentSize.height/m_gridSideLen);	// 格子行数
	const int nCol=(int)floorf(contentSize.width/m_gridSideLen);	// 格子列数
	const int fragCount=nRow*nCol;
	// 创建BatchNode
	m_fragBatchNode=CCSpriteBatchNode::createWithTexture(this->getTexture(),fragCount);
	this->addChild(m_fragBatchNode);
	m_fragBatchNode->setVisible(false);
	// 创建碎片并将其添加到 BatchNode 和格子列表中
	m_grid.resize(nRow);
	for(int i=0;i<nRow;i++)
	{
		m_grid[i].resize(nCol);
	}
	const float halfGridSideLen=0.5*m_gridSideLen;
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			FKCW_EffectNode_ShatterFragSprite*frag=new FKCW_EffectNode_ShatterFragSprite();
			frag->autorelease();
			frag->initWithTexture(this->getTexture());
			m_grid[i][j]=frag;
			m_fragBatchNode->addChild(frag);
			frag->m_randomNumber=rand();
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_ShatterSprite::_ResetShatter()
{
	CCSize contentSize=this->getContentSize();
	int nRow=(int)m_grid.size();
	int nCol=(nRow==0?0:(int)m_grid[0].size());
	const float halfGridSideLen=0.5*m_gridSideLen;
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			FKCW_EffectNode_ShatterFragSprite*frag=m_grid[i][j];
			float x=j*m_gridSideLen+halfGridSideLen;
			float y=contentSize.height-(i*m_gridSideLen+halfGridSideLen);
			frag->setTextureRect(CCRect(x-halfGridSideLen,(contentSize.height-y)-halfGridSideLen,m_gridSideLen,m_gridSideLen));
			frag->setPosition(ccp(x,y));
			frag->setScale(m_initalFrageScale);
			frag->setOpacity(255);
			frag->setVisible(true);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_ShatterSprite::_UpdateShatterAction(float time,float dt,float growSpeedOfTargetR)
{
	// 更新碎片
	CCSize contentSize=this->getContentSize();
	CCPoint center=CCPoint(contentSize.width/2,contentSize.height/2);
	float initalTargetR=ccpLength(ccp(contentSize.width,contentSize.height))/2;	//srounding圆半径
	int nRow=(int)m_grid.size();
	int nCol=nRow?(int)m_grid[0].size():0;
	for(int i=0;i<nRow;i++)
	{
		for(int j=0;j<nCol;j++)
		{
			FKCW_EffectNode_ShatterFragSprite*frag=m_grid[i][j];
			if(frag->getOpacity()==0||frag->getScale()==0)
			{
				frag->setVisible(false);
				continue;
			}
			float targetR=initalTargetR+time*growSpeedOfTargetR;
			// 更新位置
			CCPoint fragPos=frag->getPosition();
			float disToCenter=ccpLength(fragPos-center);
			CCPoint dir;
			if(disToCenter==0)
			{
				dir=CCPoint(0,0);
			}
			else
			{
				dir=fragPos-center;
				dir.x/=disToCenter;
				dir.y/=disToCenter;
			}
			float disToEdge=targetR-disToCenter;
			float disToEdgeWithRandom=disToEdge+frag->m_randomNumber%(int)initalTargetR-initalTargetR/2;
			float movLen=disToEdgeWithRandom*0.0333;
			CCPoint movVec=ccpMult(dir,movLen);
			frag->setPosition(fragPos+movVec);
			// 更新透明度
			float opacity=MAX(0,255-255*disToCenter/initalTargetR);
			frag->setOpacity(opacity);
			// 更新缩放
			frag->setScale(MAX(0,m_initalFrageScale-m_initalFrageScale*disToCenter/initalTargetR));
		}
	}
}
//-------------------------------------------------------------------------
// FKCW_Action_ShatterAction
//-------------------------------------------------------------------------
FKCW_Action_ShatterAction::FKCW_Action_ShatterAction()
{
	_InitMembers();
}
//-------------------------------------------------------------------------
FKCW_Action_ShatterAction::~FKCW_Action_ShatterAction()
{
}
//-------------------------------------------------------------------------
void FKCW_Action_ShatterAction::_InitMembers()
{
	m_timeFoe=0;
	m_timeCur=0;
	m_growSpeedOfTargetR=50;
}
//-------------------------------------------------------------------------
FKCW_Action_ShatterAction* FKCW_Action_ShatterAction::create(float fDuration)
{
	FKCW_Action_ShatterAction *p = new FKCW_Action_ShatterAction();
	p->initWithDuration(fDuration);
	p->autorelease();
	return p;
}
//-------------------------------------------------------------------------
bool FKCW_Action_ShatterAction::initWithDuration(float fDuration)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		_InitMembers();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
bool FKCW_Action_ShatterAction::isDone()
{
	return CCActionInterval::isDone();
}
//-------------------------------------------------------------------------
void FKCW_Action_ShatterAction::stop()
{
	((FKCW_EffectNode_ShatterSprite*)m_pTarget)->m_fragBatchNode->setVisible(true);
	CCActionInterval::stop();
}
//-------------------------------------------------------------------------
void FKCW_Action_ShatterAction::startWithTarget(CCNode *pTarget)
{
	((FKCW_EffectNode_ShatterSprite*)pTarget)->_ResetShatter();
	((FKCW_EffectNode_ShatterSprite*)pTarget)->m_fragBatchNode->setVisible(true);
	CCActionInterval::startWithTarget(pTarget);
}
//-------------------------------------------------------------------------
void FKCW_Action_ShatterAction::update(float time)
{
	m_timeFoe=m_timeCur;
	float progressPercentage=time;
	m_timeCur=progressPercentage*getDuration();
	((FKCW_EffectNode_ShatterSprite*)m_pTarget)->_UpdateShatterAction(m_timeCur,m_timeCur-m_timeFoe,m_growSpeedOfTargetR);
}
//-------------------------------------------------------------------------