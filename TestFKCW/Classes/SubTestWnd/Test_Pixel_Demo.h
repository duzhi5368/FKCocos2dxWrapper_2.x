//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	Test_Pixel_Demo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
CCSize	g_pixelSize=CCSize(6,6);
float	g_gravityFactor=10*60;
bool	g_debugOn=true;
//-------------------------------------------------------------------------
// 草地对象
class Cgrass	: public FKCW_Pixel_DynamicPixelSprite{};
//-------------------------------------------------------------------------
// 地面对象
class Cground	: public FKCW_Pixel_DynamicPixelSprite{};
//-------------------------------------------------------------------------
// 熔岩对象
class Clava		: public FKCW_Pixel_DynamicPixelSprite
{
public:
	Clava(){
		m_time=0;
	}
	bool init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize)
	{
		FKCW_Pixel_DynamicPixelSprite::init(texFileName,perPixelSize);
		const float w=1.0f/5;
		int nCol=getFragMatColCount();
		for (int j=0; j<nCol; j++) {
			table_sinJW.push_back(sinf(w*j));
			table_cosJW.push_back(cosf(w*j));
		}
		return true;
	}
	void wave(float dt)
	{
		m_time+=dt;
		if(m_time>=INFINITY)m_time=0;
		const float vx=5;
		float sinTimeVx=sinf(m_time*vx);
		float cosTimeVx=cosf(m_time*vx);
		int nRow=getFragMatRowCount();
		int nCol=getFragMatColCount();
		for(int i=0;i<nRow;i++)
		{
			for(int j=0;j<nCol;j++)
			{
				FKCW_Pixel_FragSprite* fragSprite = m_fragMat[i][j];
				if(fragSprite==NULL)continue;
				float sinJWaddTimeVx=table_sinJW[j]*cosTimeVx+sinTimeVx*table_cosJW[j];//sin(j*w+m_time*vx)
				float offsety=8*(sinJWaddTimeVx-1);
				fragSprite->setPosition(fragSprite->getInitPosition()+CCPoint(0,offsety*(1-(float)i/(nRow-1))));
			}
		}
	}
protected:
	float m_time;
	vector<float> table_sinJW;
	vector<float> table_cosJW;
};
//-------------------------------------------------------------------------
// 发射球对象
class CshotBall_dynamic:public FKCW_Pixel_DynamicPixelSprite
{
public:
	CshotBall_dynamic(){
		m_isLaunched=false;
		m_isToRight=true;
	}
	virtual~CshotBall_dynamic(){}
	void setIsLaunched(bool value){m_isLaunched=value;}
	bool getIsLaunched()const{return m_isLaunched;}
	void setIsToRight(bool value){m_isToRight=value;}
	bool getIsToRight()const{return m_isToRight;}
protected:
	bool m_isLaunched;
	bool m_isToRight;
};
//-------------------------------------------------------------------------
// 角色状态
enum EPlayerState{
	eRun		=0,
	eStand,
	eJump,
	eFall,
	eDied,
};
//-------------------------------------------------------------------------
// 角色朝向
enum EPlayerFacingDir{
	eRight		=0,
	eLeft,
};
//-------------------------------------------------------------------------
// 游戏主角对象
class Cplayer:public FKCW_Pixel_DynamicPixelActor
{
public:
	Cplayer(){
		m_state=eStand;
		m_facingDir=eRight;
		m_ground=NULL;
		m_layer=NULL;
		m_sceneRoot=NULL;
		m_grass=NULL;
		m_lava=NULL;
	};
	virtual~Cplayer(){
		if(m_ground)m_ground->release();
		if(m_sceneRoot)m_sceneRoot->release();
		if(m_layer)m_layer->release();
		if(m_grass)m_grass->release();
		if(m_lava)m_lava->release();
	}
	bool init(const string&texFileName,CCNode*sceneRoot,CCSize perPixelSize=g_defaultPerPixelSize)
	{
		FKCW_Pixel_DynamicPixelActor::init(texFileName,perPixelSize);
		setSceneRoot(sceneRoot);
		// 动作堆栈
		FKCW_Pixel_DynamicPixelAniStack*aniStack=new FKCW_Pixel_DynamicPixelAniStack();
		aniStack->autorelease();
		//aniLayers
		{
			// 站立动作
			{
				FKCW_Pixel_DynamicPixelAniLayer*aniLayer=new FKCW_Pixel_DynamicPixelAniLayer();
				aniLayer->autorelease();
				vector<string> picNameList;
				picNameList.push_back( s_szPixelPlayerStandFile );
				for(int i=0;i<(int)picNameList.size();i++)
				{
					string picName=picNameList[i];

					FKCW_Pixel_DynamicPixelSprite*dpSprite=new FKCW_Pixel_DynamicPixelSprite();
					dpSprite->autorelease();
					dpSprite->init(picName,g_pixelSize);
					FKCW_Pixel_DynamicPixelModel*dpModel=new FKCW_Pixel_DynamicPixelModel();
					dpModel->autorelease();
					dpModel->adddpSprite(dpSprite);
					FKCW_Pixel_DynamicPixelAniFrame*dpAniFrame=new FKCW_Pixel_DynamicPixelAniFrame();
					dpAniFrame->autorelease();
					dpAniFrame->init(dpModel, 0.12f);
					aniLayer->addAniFrame(dpAniFrame);
					aniLayer->setAniName("stand");
				}
				aniStack->addAniLayer(aniLayer);
			}
			// 移动动作
			{
				FKCW_Pixel_DynamicPixelAniLayer*aniLayer=new FKCW_Pixel_DynamicPixelAniLayer();
				aniLayer->autorelease();
				vector<string> picNameList;
				picNameList.push_back( s_szPixelPlayerRun1File );
				picNameList.push_back( s_szPixelPlayerRun2File );
				for(int i=0;i<(int)picNameList.size();i++)
				{
					string picName=picNameList[i];

					FKCW_Pixel_DynamicPixelSprite*dpSprite=new FKCW_Pixel_DynamicPixelSprite();
					dpSprite->autorelease();
					dpSprite->init(picName,g_pixelSize);
					FKCW_Pixel_DynamicPixelModel*dpModel=new FKCW_Pixel_DynamicPixelModel();
					dpModel->autorelease();
					dpModel->adddpSprite(dpSprite);
					FKCW_Pixel_DynamicPixelAniFrame*dpAniFrame=new FKCW_Pixel_DynamicPixelAniFrame();
					dpAniFrame->autorelease();
					dpAniFrame->init(dpModel, 0.12f);
					aniLayer->addAniFrame(dpAniFrame);
					aniLayer->setAniName("run");
				}
				aniStack->addAniLayer(aniLayer);
			}
			// 跳跃动作
			{
				FKCW_Pixel_DynamicPixelAniLayer*aniLayer=new FKCW_Pixel_DynamicPixelAniLayer();
				aniLayer->autorelease();
				vector<string> picNameList;
				picNameList.push_back( s_szPixelPlayerJumpFile );
				for(int i=0;i<(int)picNameList.size();i++)
				{
					string picName=picNameList[i];

					FKCW_Pixel_DynamicPixelSprite*dpSprite=new FKCW_Pixel_DynamicPixelSprite();
					dpSprite->autorelease();
					dpSprite->init(picName,g_pixelSize);
					FKCW_Pixel_DynamicPixelModel*dpModel=new FKCW_Pixel_DynamicPixelModel();
					dpModel->autorelease();
					dpModel->adddpSprite(dpSprite);
					FKCW_Pixel_DynamicPixelAniFrame*dpAniFrame=new FKCW_Pixel_DynamicPixelAniFrame();
					dpAniFrame->autorelease();
					dpAniFrame->init(dpModel, 0.12f);
					aniLayer->addAniFrame(dpAniFrame);
					aniLayer->setAniName("jump");
				}
				aniStack->addAniLayer(aniLayer);
			}
		}
		// 设置动作栈
		this->setAniStack(aniStack);
		//collision rect
		//  --C--
		//  |   |
		//  C   C
		//  |   |
		//  --C--
		float width=ccRectGetSize(this->getBoundingBox()).width;
		float height=ccRectGetSize(this->getBoundingBox()).height;

		float margin=2;
		FKCW_Pixel_CollisionRect leftRect	=FKCW_Pixel_CollisionRect(CCRect(margin-width/2,margin-height/2+height/3,width/3-2*margin,height/3-2*margin),"leftRect");
		FKCW_Pixel_CollisionRect rightRect	=FKCW_Pixel_CollisionRect(CCRect(margin-width/2+width/3*2,margin-height/2+height/3,width/3-2*margin,height/3-2*margin),"rightRect");
		FKCW_Pixel_CollisionRect dnRect		=FKCW_Pixel_CollisionRect(CCRect(margin-width/2+width/3,margin-height/2,width/3-2*margin,height/3-2*margin),"dnRect");
		FKCW_Pixel_CollisionRect upRect		=FKCW_Pixel_CollisionRect(CCRect(margin-width/2+width/3,margin-height/2+height/3*2,width/3-2*margin,height/3-2*margin),"upRect");

		this->getdpModel()->addCollisionRectInLocalSpace(leftRect);
		this->getdpModel()->addCollisionRectInLocalSpace(rightRect);
		this->getdpModel()->addCollisionRectInLocalSpace(dnRect);
		this->getdpModel()->addCollisionRectInLocalSpace(upRect);

		// 发射球堆栈
		const int nShotBall=20;
		for(int i=0;i<nShotBall;i++)
		{
			CshotBall_dynamic*shotBall=new CshotBall_dynamic();
			shotBall->autorelease();
			shotBall->init(s_szPixelShotballFile,g_pixelSize);
			shotBall->setVisible(false);
			m_sceneRoot->addChild(shotBall);
			m_shotBallList.push_back(shotBall);
		}
		return true;
	}
	void playRunAnimation()
	{
		float aniLoopDurTime=this->getAniStack()->getAniLayerByName("run")->getAniDurTime();
		FKCW_Pixel_DynamicPixelActorAnimate* ani = FKCW_Pixel_DynamicPixelActorAnimate::create(
			aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("run"));
		this->runAction(CCRepeatForever::create(ani));
	}
	void playStandAnimation()
	{
		float aniLoopDurTime=this->getAniStack()->getAniLayerByName("stand")->getAniDurTime();
		FKCW_Pixel_DynamicPixelActorAnimate*ani=FKCW_Pixel_DynamicPixelActorAnimate::create(
			aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("stand"));
		this->runAction(CCRepeatForever::create(ani));
	}
	void playJumpAnimation()
	{
		float aniLoopDurTime=this->getAniStack()->getAniLayerByName("jump")->getAniDurTime();
		FKCW_Pixel_DynamicPixelActorAnimate*ani=FKCW_Pixel_DynamicPixelActorAnimate::create(
			aniLoopDurTime,this->getAniStack()->getAniLayerIndexByName("jump"));
		this->runAction(CCRepeatForever::create(ani));
	}
	void shatter(float r)
	{
		assert(this->getCurDisplayModel()->getdpSpriteCount()>0);
		FKCW_Pixel_DynamicPixelSprite* dpSprite=this->getCurDisplayModel()->getdpSpriteByIndex(0);
		CCPoint center=dpSprite->getPosition();
		int nRow=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragMatRowCount();
		int nCol=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragMatColCount();
		for(int i=0;i<nRow;i++)
		{
			for(int j=0;j<nCol;j++)
			{
				FKCW_Pixel_FragSprite*frag=this->getCurDisplayModel()->getdpSpriteByIndex(0)->getFragByIndex(i, j);
				if(frag==NULL)continue;
				CCPoint fragPos=frag->getPosition();
				CCPoint dir=ccpNormalize(fragPos-center);
				float disToCenter=ccpLength(center-fragPos);
				float disToEdge=r-disToCenter;
				float disToEdgeWithRandom=disToEdge+rand()%(int)r-r/2;
				float movLen=disToEdgeWithRandom*0.04f;
				CCPoint movVec=ccpMult(dir, movLen);
				CCPoint newFragPos=fragPos+movVec;
				frag->setPosition(newFragPos);
				frag->setOpacity(MAX(0,255*disToEdge/r));
				frag->setScale(MAX(0, 2.5f*disToEdge/r));
			}
		}
	}
	void shotBallsMove(float dt)
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		int nShotBall=(int)m_shotBallList.size();
		for(int i=0;i<nShotBall;i++)
		{
			CshotBall_dynamic*shotBall=m_shotBallList[i];
			if(shotBall->getIsLaunched())
			{
				float sig=(shotBall->getIsToRight()?1.0f:-1.0f);
				shotBall->setPositionX(shotBall->getPositionX()+sig*7*60*dt);
				CCCamera*camera=m_sceneRoot->getCamera();
				float centerx,centery,centerz;
				float eyex,eyey,eyez;
				float upx,upy,upz;
				camera->getCenterXYZ(&centerx, &centery, &centerz);
				camera->getEyeXYZ(&eyex, &eyey, &eyez);
				camera->getUpXYZ(&upx, &upy, &upz);
				CCRect visibleRect=CCRect(eyex,eyey,winSize.width,winSize.height);
				if(visibleRect.intersectsRect(shotBall->getBoundingBox())==false)
				{
					shotBall->setIsLaunched(false);
					shotBall->setVisible(false);
				}
			}
		}
	}
	bool launchAShotBall()
	{
		int nShotBall=(int)m_shotBallList.size();
		for(int i=0;i<nShotBall;i++)
		{
			CshotBall_dynamic*shotBall=m_shotBallList[i];
			if(shotBall->getIsLaunched()==false)
			{
				shotBall->setIsLaunched(true);
				shotBall->setVisible(true);
				shotBall->setPosition(this->getPosition());
				shotBall->setIsToRight(this->getScaleX()>0);
				shotBall->setScaleX(shotBall->getIsToRight()?1.0f:-1.0f);
				return true;
			}
		}
		return false;
	}
	void updateVelocityAndPosition(float dt)
	{
		m_velocity=m_velocity+ccpMult(m_acceleration,dt);
		CCPoint oldPos=this->getPosition();
		CCPoint newPos=oldPos+ccpMult(m_velocity,dt);
		this->setPosition(newPos);
	}
	void setGround(Cground*ground)
	{
		assert(ground);
		if(m_ground==NULL){
			m_ground=ground;
			m_ground->retain();
		}else{
			m_ground->release();
			m_ground=ground;
			m_ground->retain();
		}
	}
	void setGrass(Cgrass*grass)
	{
		assert(grass);
		if(m_grass==NULL){
			m_grass=grass;
			m_grass->retain();
		}else{
			m_grass->release();
			m_grass=grass;
			m_grass->retain();
		}
	}
	void setLava(Clava*lava)
	{
		assert(lava);
		if(m_lava==NULL){
			m_lava=lava;
			m_lava->retain();
		}else{
			m_lava->release();
			m_lava=lava;
			m_lava->retain();
		}
	}
	void setSceneRoot(CCNode*sceneRoot)
	{
		assert(sceneRoot);
		if(m_sceneRoot==NULL){
			m_sceneRoot=sceneRoot;
			m_sceneRoot->retain();
		}else{
			m_sceneRoot->release();
			m_sceneRoot=sceneRoot;
			m_sceneRoot->retain();
		}
	}
	CCRect getLeftCollisionRectWithGround()
	{
		CCRect leftRect_parentSpace=this->getCollisionRectByName((getScaleX()>0)?"leftRect":"rightRect");
		vector<FKCW_Pixel_FragSprite*> collisionFrags_left=m_ground->getFragsInRect(leftRect_parentSpace);
		CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_left);
		return minRect;
	}
	CCRect getRightCollisionRectWithGround()
	{
		CCRect rightRect_parentSpace=this->getCollisionRectByName((getScaleX()>0)?"rightRect":"leftRect");
		vector<FKCW_Pixel_FragSprite*> collisionFrags_right=m_ground->getFragsInRect(rightRect_parentSpace);
		CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_right);
		return minRect;
	}
	CCRect getUpCollisionRectWithGround()
	{
		CCRect upRect_parentSpace=this->getCollisionRectByName("upRect");
		vector<FKCW_Pixel_FragSprite*> collisionFrags_up=m_ground->getFragsInRect(upRect_parentSpace);
		CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_up);
		return minRect;
	}
	CCRect getDnCollisionRectWithGround()
	{
		CCRect dnRect_parentSpace=this->getCollisionRectByName("dnRect");
		vector<FKCW_Pixel_FragSprite*> collisionFrags_dn=m_ground->getFragsInRect(dnRect_parentSpace);
		CCRect minRect=m_ground->getMinRectForFrags(collisionFrags_dn);
		return minRect;
	}
	CCRect getDnCollisionRectWithLava()
	{
		CCRect dnRect_parentSpace=this->getCollisionRectByName("dnRect");
		vector<FKCW_Pixel_FragSprite*> collisionFrags_dn=m_lava->getFragsInRect(dnRect_parentSpace);
		CCRect minRect=m_lava->getMinRectForFrags(collisionFrags_dn);
		return minRect;
	}
	void shotBallsCollisionWithGround()
	{
		assert(m_ground);
		int nShotBall=(int)m_shotBallList.size();
		for(int i=0;i<nShotBall;i++)
		{
			CshotBall_dynamic*shotBall=m_shotBallList[i];
			if(shotBall->getIsLaunched()==false)continue;
			vector<FKCW_Pixel_FragSprite*> collisionFrags=getFragsHitByShotBall(shotBall, m_ground);
			int nCollisionFrag=(int)collisionFrags.size();
			if(nCollisionFrag)
			{
				for(int k=0;k<nCollisionFrag;k++)
				{
					FKCW_Pixel_FragSprite*frag=collisionFrags[k];
					FKCW_Pixel_IJ IJ=frag->getIJ();
					for(int i=IJ.getI()-3;i<=IJ.getI()+3;i++)
					{
						for(int j=IJ.getJ()-3;j<=IJ.getJ()+3;j++)
						{
							if(i==IJ.getI()&&j==IJ.getJ())
							{
								if(m_ground->getIsHasFrag(i, j))
								{
									m_ground->killFrag(i, j);
								}
							}else
							{
								bool isKill=(rand()%2!=0);
								if(isKill)
								{
									if(m_ground->getIsHasFrag(i, j))
									{
										m_ground->killFrag(i, j);
									}
								}
							}

						}
					}
					// 子弹消失
					shotBall->setIsLaunched(false);
					shotBall->setVisible(false);
				}
			}
		}
	}
	void shotBallsCollisionWithGrass()
	{
		assert(m_grass);
		int nShotBall=(int)m_shotBallList.size();
		for(int i=0;i<nShotBall;i++)
		{
			CshotBall_dynamic*shotBall=m_shotBallList[i];
			if(shotBall->getIsLaunched()==false)continue;
			vector<FKCW_Pixel_FragSprite*> collisionFrags=getFragsHitByShotBall(shotBall, m_grass);
			int nCollisionFrag=(int)collisionFrags.size();
			if(nCollisionFrag)
			{
				for(int k=0;k<nCollisionFrag;k++)
				{
					FKCW_Pixel_FragSprite*frag=collisionFrags[k];
					FKCW_Pixel_IJ IJ=frag->getIJ();
					for(int i=IJ.getI()-3;i<=IJ.getI()+3;i++)
					{
						for(int j=IJ.getJ()-3;j<=IJ.getJ()+3;j++)
						{
							if(i==IJ.getI()&&j==IJ.getJ())
							{
								if(m_grass->getIsHasFrag(i, j))
								{
									m_grass->killFrag(i, j);
								}
							}
							else
							{
								bool isKill=(rand()%2!=0);
								if(isKill)
								{
									if(m_grass->getIsHasFrag(i, j))
									{
										m_grass->killFrag(i, j);
									}
								}
							}
						}
					}
					// 子弹消失
					shotBall->setIsLaunched(false);
					shotBall->setVisible(false);
				}
			}
		}
	}
	Cground* getdpGround(){return m_ground;}
	void setState(EPlayerState state){m_state=state;}
	EPlayerState getState()const{return m_state;}
	void setFacingDir(EPlayerFacingDir facingDir){m_facingDir=facingDir;}
	EPlayerFacingDir getFacingDir()const{return m_facingDir;}
	void setVelocity(const CCPoint&velocity){m_velocity=velocity;}
	CCPoint getVelocity()const{return m_velocity;}
	void setAcceleration(const CCPoint&acceleration){m_acceleration=acceleration;}
	CCPoint getAcceleration()const{return m_acceleration;}

protected:
	vector<FKCW_Pixel_FragSprite*> getFragsHitByShotBall(CshotBall_dynamic*shotBall,FKCW_Pixel_DynamicPixelSprite*dpSprite)
	{
		CCRect shotBallRect=shotBall->getBoundingBox();
		vector<FKCW_Pixel_FragSprite*> collisionFrags;
		{
			vector<FKCW_Pixel_FragSprite*> t_collisionFrags=dpSprite->getFragsInRect(shotBallRect);
			int nFrag=(int)t_collisionFrags.size();
			for(int i=0;i<nFrag;i++)
			{
				FKCW_Pixel_FragSprite*frag=t_collisionFrags[i];
				vector<FKCW_Pixel_FragSprite*> tt_collisionFrags=shotBall->getFragsInRect(dpSprite->getFragRect(frag));
				if((int)tt_collisionFrags.size()!=0)
				{
					collisionFrags.push_back(frag);
				}
			}
		}
		return collisionFrags;
	}
protected:
	EPlayerState				m_state;
	EPlayerFacingDir			m_facingDir;
	CCPoint						m_velocity;
	CCPoint						m_acceleration;
	Cgrass*						m_grass;
	Clava*						m_lava;
	Cground*					m_ground;
	CCNode*						m_sceneRoot;
	CCLayer*					m_layer;
	vector<CshotBall_dynamic*>	m_shotBallList;
};
//-------------------------------------------------------------------------
// 爆炸破碎动作
class CplayerShatter : public CCActionInterval
{
public:
	static CplayerShatter* create(float fDuration,float r)
	{
		CplayerShatter *p = new CplayerShatter();
		p->initWithDuration(fDuration,r);
		p->autorelease();
		return p;
	}
	bool initWithDuration(float fDuration,float r)
	{
		if (CCActionInterval::initWithDuration(fDuration))
		{
			m_r=r;
			return true;
		}
		return false;
	}
	virtual void startWithTarget(CCNode *pTarget)
	{
		CCActionInterval::startWithTarget(pTarget);
	}
	virtual void update(float time)
	{
		float progressPercentage=time;
		float curTime=progressPercentage*getDuration();
		((Cplayer*)m_pTarget)->shatter(m_r);
	}
protected:
	float m_r;
};
//-------------------------------------------------------------------------
class CTest_Pixel_DemoLayer : public CCLayer
{
public:
	bool init()
	{
		// 开启update
		scheduleUpdate();
		// 开启触摸接受
		setTouchEnabled( true );
		// 开启包围盒查看
		g_isShowBoundingBox=g_debugOn;

		// 清理游戏
		cleanGame();
		// 重新初始化游戏
		initGame();

		return true;
	}
public:	// 触摸回调
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
			m_touchPointFoe=m_touchPoint;
			m_touchPoint=loc_GLSpace;
			break;

		}
		m_isTouchingFoe=m_isTouching;
		m_isTouching=true;
	}
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
			m_touchPointFoe=m_touchPoint;
			m_touchPoint=loc_GLSpace;
			break;
		}
		m_isTouchingFoe=m_isTouching;
		m_isTouching=true;
	}
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
			m_touchPointFoe=m_touchPoint;
			m_touchPoint=loc_GLSpace;
			break;
		}
		m_isTouchingFoe=m_isTouching;
		m_isTouching=false;
	}
public:	// 动作回调
	void playerShatterFinish()
	{
		m_controlButton_shot->setVisible(false);
		m_controlButton_debug->setVisible(false);
		m_maskPanel->setVisible(true);
		ccColor3B color={0,0,0};
		m_maskPanel->setColor(color);
		m_maskPanel->setOpacity(0);
		CCFadeTo*fadeTo=CCFadeTo::create(1, 160);
		CCCallFunc*callfunc=CCCallFunc::create(this, 
			callfunc_selector(CTest_Pixel_DemoLayer::maskPanelFadeShowFinish));
		m_maskPanel->runAction(CCSequence::create(fadeTo,callfunc,NULL));
	}
	void maskPanelFadeShowFinish()
	{
		m_gameOver->setVisible(true);
		m_controlButton_restart->setVisible(true);
	}
	void restartClickDelayFinish()
	{
		cleanGame();
		initGame();
	}
public:
	void initGame()
	{
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		// 场景根节点
		m_sceneRoot=new CCNode();
		m_sceneRoot->autorelease();
		m_sceneRoot->init();
		addChild(m_sceneRoot);

		// UI根节点
		m_uiRoot=new CCNode();
		m_uiRoot->autorelease();
		m_uiRoot->init();
		addChild(m_uiRoot);

		{// 各种对象
			// 草
			m_grass=new Cgrass();
			m_grass->autorelease();
			m_grass->init(s_szPixelGrassFile,g_pixelSize);
			m_sceneRoot->addChild(m_grass);
			m_grass->setPosition(ccp(winSize.width/2-9.5*g_pixelSize.width,winSize.height/2-24.5*g_pixelSize.height));
			// 熔岩
			m_lava=new Clava();
			m_lava->autorelease();
			m_lava->init(s_szPixelLavaFile,g_pixelSize);
			m_sceneRoot->addChild(m_lava);
			m_lava->setPosition(ccp(winSize.width/2,winSize.height/2-46.5*g_pixelSize.height));
			// 地表
			m_ground=new Cground();
			m_ground->autorelease();
			m_ground->init(s_szPixelGroundFile,g_pixelSize);
			m_sceneRoot->addChild(m_ground);
			m_ground->setPosition(ccp(winSize.width/2-9.5*g_pixelSize.width,winSize.height/2-29.5*g_pixelSize.height));
			// 玩家主角
			m_player=new Cplayer();
			m_player->autorelease();
			m_player->init(s_szPixelPlayerStandFile,m_sceneRoot,g_pixelSize);
			m_sceneRoot->addChild(m_player);
			m_player->setPosition(ccp(winSize.width/2-450,winSize.height/2));
			m_player->setState(eJump);
			m_player->setAcceleration(ccp(0,-g_gravityFactor));
			m_player->setGround(m_ground);
			m_player->setGrass(m_grass);
			m_player->setLava(m_lava);
		}

		{   // 开火按钮
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create(A2U("发射").c_str(), "Helvetica", 25);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(SCREEN_CENTER_POS+ccp(0,180));
			controlButton->addTargetWithActionForControlEvents(this, 
				(SEL_CCControlHandler)(&CTest_Pixel_DemoLayer::controlButtonEvent_shot), CCControlEventTouchDown);
			m_uiRoot->addChild(controlButton);
			m_controlButton_shot=controlButton;
		}
		{   // DEBUG按钮
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 25);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(100,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this, 
				(SEL_CCControlHandler)(&CTest_Pixel_DemoLayer::controlButtonEvent_debug), CCControlEventTouchDown);
			m_uiRoot->addChild(controlButton);
			m_controlButton_debug=controlButton;
		}
		{   // 重新开始按钮
			FKCW_Pixel_StaticPixelSprite*spSprite=new FKCW_Pixel_StaticPixelSprite();
			spSprite->autorelease();
			spSprite->init( s_szPixelRestartFile,g_pixelSize);
			CCSpriteFrame*spriteFrame=CCSpriteFrame::createWithTexture(spSprite->getTexture(), spSprite->getTextureRect());
			CCScale9Sprite* btnUp=CCScale9Sprite::createWithSpriteFrame(spriteFrame);
			CCScale9Sprite* btnDn=CCScale9Sprite::createWithSpriteFrame(spriteFrame);
			CCLabelTTF*title=CCLabelTTF::create("", "Helvetica", 25);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(spSprite->getContentSize());
			controlButton->setPosition(ccp(winSize.width/2,winSize.height/2-40));
			controlButton->addTargetWithActionForControlEvents(this, 
				(SEL_CCControlHandler)(&CTest_Pixel_DemoLayer::controlButtonEvent_restart), CCControlEventTouchDown);
			m_uiRoot->addChild(controlButton);
			m_controlButton_restart=controlButton;
			m_controlButton_restart->setVisible(false);
		}
		{   // 介绍
			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("跳跃: 拖动角色 \r\n 开火:点击发射按钮").c_str(), "Arial", 25);
			pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
				origin.y + visibleSize.height - pLabel->getContentSize().height-150));
			m_uiRoot->addChild(pLabel);
		}

		// 蒙版面板
		m_maskPanel=new FKCW_Pixel_StaticPixelSprite();
		m_maskPanel->autorelease();
		m_maskPanel->init(s_sz1X1WhiteImagePath,winSize);
		m_uiRoot->addChild(m_maskPanel);
		m_maskPanel->setPosition(ccp(winSize.width/2,winSize.height/2));
		m_maskPanel->setVisible(false);

		// 游戏结束
		m_gameOver=new FKCW_Pixel_StaticPixelSprite();
		m_gameOver->autorelease();
		m_gameOver->init(s_szPixelGameOverFile,g_pixelSize);
		m_uiRoot->addChild(m_gameOver);
		m_gameOver->setPosition(ccp(winSize.width/2,winSize.height/2+150));
		m_gameOver->setVisible(false);
	}
	void cleanGame()
	{
		m_ground=NULL;
		m_lava=NULL;
		m_grass=NULL;
		m_player=NULL;
		m_isTouchMove=false;
		m_isTouching=false;
		m_isTouchingFoe=false;
		m_uiRoot=NULL;
		m_sceneRoot=NULL;
		m_gameOver=NULL;
		m_maskPanel=NULL;
		m_controlButton_shot=NULL;
		m_controlButton_debug=NULL;
		m_controlButton_restart=NULL;
		this->removeAllChildrenWithCleanup(true);
	}
public:
	void controlButtonEvent_shot(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_player->launchAShotBall();
	}
	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		g_debugOn=!g_debugOn;
		g_isShowBoundingBox=g_debugOn;
	}
	void controlButtonEvent_restart(CCObject *senderz, CCControlEvent controlEvent)
	{
		CCDelayTime*delay=CCDelayTime::create(0.4f);
		CCCallFunc*callfunc=CCCallFunc::create(this, callfunc_selector(CTest_Pixel_DemoLayer::restartClickDelayFinish));
		this->runAction(CCSequence::create(delay,callfunc,NULL));
	}
public:
	void update(float dt)
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		// 更新面部朝向
		EPlayerFacingDir oldFacingDir=m_player->getFacingDir();
		EPlayerFacingDir newFacingDir=oldFacingDir;
		if(m_isTouching){
			if(m_touchPoint.x>winSize.width/2){
				newFacingDir=eRight;
			}else{
				newFacingDir=eLeft;
			}
		}
		m_player->setFacingDir(newFacingDir);

		if(m_player->getFacingDir()==eRight){
			m_player->setScaleX(1);
		}
		if(m_player->getFacingDir()==eLeft){
			m_player->setScaleX(-1);
		}

		// 更新状态位置
		m_player->updateVelocityAndPosition(dt);

		// 计算与地面的碰撞
		CCRect upCollisionRect=m_player->getUpCollisionRectWithGround();
		CCRect dnCollisionRect=m_player->getDnCollisionRectWithGround();
		CCRect leftCollisionRect=m_player->getLeftCollisionRectWithGround();
		CCRect rightCollsionRect=m_player->getRightCollisionRectWithGround();

		if(ccIsZeroRect(rightCollsionRect)==false){
			m_player->setVelocity(ccp(MIN(0,m_player->getVelocity().x),m_player->getVelocity().y));
			float targetX=rightCollsionRect.getMinX()-ccRectGetSize(m_player->getBoundingBox()).width/2;
			m_player->setPositionX(m_player->getPositionX()+(targetX-m_player->getPositionX())*0.2f);

		}
		if(ccIsZeroRect(leftCollisionRect)==false){
			m_player->setVelocity(ccp(MAX(0,m_player->getVelocity().x),m_player->getVelocity().y));
			float targetX=leftCollisionRect.getMaxX()+ccRectGetSize(m_player->getBoundingBox()).width/2;
			m_player->setPositionX(m_player->getPositionX()+(targetX-m_player->getPositionX())*0.2f);
		}

		if(ccIsZeroRect(upCollisionRect)==false){
			m_player->setVelocity(ccp(m_player->getVelocity().x,MIN(0,m_player->getVelocity().y)));
			float targetY=upCollisionRect.getMinY()-ccRectGetSize(m_player->getBoundingBox()).height/2;
			m_player->setPositionY(m_player->getPositionY()+(targetY-m_player->getPositionY())*0.2f);
		}

		if(ccIsZeroRect(dnCollisionRect)==false){
			m_player->setVelocity(ccp(m_player->getVelocity().x,MAX(0,m_player->getVelocity().y)));
			float targetY=dnCollisionRect.getMaxY()+ccRectGetSize(m_player->getBoundingBox()).height/2;
			m_player->setPositionY(m_player->getPositionY()+(targetY-m_player->getPositionY())*0.8f);
			m_player->setAcceleration(ccp(m_player->getAcceleration().x, MAX(-0.3,m_player->getAcceleration().y)));
		}

		// 获取脚部与地面的距离
		float groundHeightBelowFoot;
		{
			float rayStartX=(m_player->getCollisionRectByName("dnRect").getMinX()+m_player->getCollisionRectByName("dnRect").getMaxX())/2;
			float rayStartY=m_player->getPositionY();
			float rayLen=10*g_pixelSize.height;
			float rayWidth=m_player->getCollisionRectByName("dnRect").getMaxX()-m_player->getCollisionRectByName("dnRect").getMinX();
			CCPoint foot=m_ground->downDirRayIntersectWithMe(ccp(rayStartX,rayStartY), rayLen, rayWidth);
			groundHeightBelowFoot=foot.y;
		}

		// 计算与熔岩的碰撞
		CCRect dnCollisionRectWithLava=m_player->getDnCollisionRectWithLava();

		EPlayerState oldState=m_player->getState();
		if(ccIsZeroRect(dnCollisionRectWithLava)==false)
		{
			// 死亡
			m_player->setState(eDied);
			m_player->setVelocity(ccp(0,0));
			m_player->setAcceleration(ccp(0, 0));
		}
		else
		{
			// 跑动中，设置状态速度和加速度
			if(m_isTouching)
			{
				if(m_player->getState()!=eJump&&m_player->getState()!=eFall)
				{
					m_player->setState(eRun);
	
					if(m_player->getFacingDir()==eRight){
						m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
					}
					if(m_player->getFacingDir()==eLeft){
						m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
					}
				}
			}
			// 站立中，设置状态速度和加速度
			if(m_isTouchingFoe&&m_isTouching==false)
			{
				if(m_player->getState()!=eJump&&m_player->getState()!=eFall)
				{
					m_player->setState(eStand);
					m_player->setVelocity(ccp(0,m_player->getVelocity().y));
				}
			}
			// 开始跳跃，设置状态速度和加速度
			if(m_isTouching&&m_isTouchingFoe)
			{
				if(m_touchPoint.y-m_touchPointFoe.y>5)
				{
					if(m_player->getState()!=eJump&&m_player->getState()!=eFall)
					{
						m_player->setState(eJump);
						m_player->setAcceleration(ccp(m_player->getAcceleration().x, m_player->getAcceleration().y-g_gravityFactor));
						m_player->setVelocity(ccp(m_player->getVelocity().x, m_player->getVelocity().y+350));
						m_player->setPositionY(m_player->getPositionY()+5);
					}
				}
			}
			// 跳跃过程中，设置状态速度和加速度
			if(m_isTouching)
			{
				if(m_player->getState()==eJump)
				{
					if(m_player->getFacingDir()==eRight)
					{
						m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
					}
					if(m_player->getFacingDir()==eLeft)
					{
						m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
					}
				}
			}
			// 开始向下跌落，设置状态速度和加速度
			if(m_player->getState()!=eFall&&m_player->getState()!=eJump
				&&m_player->getCollisionRectByName("dnRect").getMinY()-groundHeightBelowFoot>0.8*g_pixelSize.height
				)
			{
					m_player->setState(eFall);
					m_player->setAcceleration(ccp(m_player->getAcceleration().x, m_player->getAcceleration().y-g_gravityFactor));
			}
			// 掉落过程中，设置状态速度和加速度
			if(m_isTouching)
			{
				if(m_player->getState()==eFall)
				{
					if(m_player->getFacingDir()==eRight)
					{
						m_player->setVelocity(ccp(1.6*60,m_player->getVelocity().y));
					}
					if(m_player->getFacingDir()==eLeft)
					{
						m_player->setVelocity(ccp(-1.6*60,m_player->getVelocity().y));
					}
				}
			}
			// 跳跃完成，设置状态速度和加速度
			if(ccIsZeroRect(dnCollisionRect)==false)
			{
				if(m_player->getState()==eJump)
				{
					if(m_isTouching)
					{
						m_player->setState(eRun);
					}else
					{
						m_player->setState(eStand);
					}
					m_player->setVelocity(ccp(m_player->getVelocity().x,0));
				}
			}
			// 落地，设置状态速度和加速度
			if(ccIsZeroRect(dnCollisionRect)==false)
			{
				if(m_player->getState()==eFall)
				{
					if(m_isTouching)
					{
						m_player->setState(eRun);
					
					}else
					{
						m_player->setState(eStand);
					}
					m_player->setVelocity(ccp(m_player->getVelocity().x,0));
				}
			}
		}


		if(m_player->getState()!=oldState)
		{
			m_player->stopAllActions();
			switch (m_player->getState()) 
			{
			case eRun:
				m_player->playRunAnimation();
				break;
			case eStand:
				m_player->playStandAnimation();
				break;
			case eJump:
				m_player->playJumpAnimation();
				break;
			case eFall:
				m_player->playJumpAnimation();
				break;
			case eDied:
				{
					CplayerShatter*shatter=CplayerShatter::create(1.5,80);
					CCCallFunc*callfunc=CCCallFunc::create(this, 
						callfunc_selector(CTest_Pixel_DemoLayer::playerShatterFinish));
					m_player->runAction(CCSequence::create(shatter,callfunc,NULL));
				}
				break;
			default:
				break;
			}
		}

		// 摄像机跟随
		CCPoint playerPos=m_player->getPosition();
		CCCamera*camera=m_sceneRoot->getCamera();
		float centerx,centery,centerz;
		float eyex,eyey,eyez;
		float upx,upy,upz;
		camera->getCenterXYZ(&centerx, &centery, &centerz);
		camera->getEyeXYZ(&eyex, &eyey, &eyez);
		camera->getUpXYZ(&upx, &upy, &upz);
		CCPoint wantedEyePos=playerPos-CCPoint(winSize.width/2,winSize.height/2);
		CCPoint newEyePos=CCPoint(eyex,eyey)+ccpMult(wantedEyePos-CCPoint(eyex,eyey),0.2f);
		camera->setCenterXYZ(newEyePos.x, newEyePos.y, centerz);
		camera->setEyeXYZ(newEyePos.x, newEyePos.y, eyez);
		camera->setUpXYZ(upx, upy, upz);


		// 熔岩更新
		m_lava->wave(dt);
		// 子弹更新
		m_player->shotBallsMove(dt);
		m_player->shotBallsCollisionWithGround();
		m_player->shotBallsCollisionWithGrass();
	}
protected:
	CCNode*							m_uiRoot;
	CCNode*							m_sceneRoot;
	Cground*						m_ground;
	Clava*							m_lava;
	Cgrass*							m_grass;
	FKCW_Pixel_StaticPixelSprite*	m_gameOver;
	FKCW_Pixel_StaticPixelSprite*	m_maskPanel;
	Cplayer*						m_player;
protected:
	bool							m_isTouchMove;
	bool							m_isTouching;
	bool							m_isTouchingFoe;
	CCPoint							m_touchPoint;
	CCPoint							m_touchPointFoe;
protected:
	CCControlButton*				m_controlButton_shot;		// 开火按钮
	CCControlButton*				m_controlButton_debug;		// DEBUG按钮
	CCControlButton*				m_controlButton_restart;	// 重新开始按钮
public:
	CREATE_FUNC( CTest_Pixel_DemoLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Pixel, Demo, 使用像素支持库的小Demo)
//-------------------------------------------------------------------------