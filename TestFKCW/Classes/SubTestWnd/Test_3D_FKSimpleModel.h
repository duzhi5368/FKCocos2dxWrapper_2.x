//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	Test_3D_FKSimpleModel.h
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
class CTest_3D_FKSimpleModelLayer : public CCLayer
{
public:
	virtual bool init()
	{
		// 基本值初始化
		m_r = 6;
		m_A = 0;
		m_B = 0;
		m_eyePos.init(0, 0, m_r, 1);
		m_eyeTg.init(0, 0, 0, 1);

		m_mosPos = CCPoint(0,0);
		m_mosPosf = CCPoint(0,0);

		m_root3d = NULL;
		m_isDoUpdate = false;
		m_pLabel = NULL;

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		// 开启Touch许可
		setTouchEnabled( true );
		// 开启Update
		scheduleUpdate();
		// 每帧更新视野坐标
		updateEyePos();
		//-----------------------------
		// Step1: 创建Root3D
		m_root3d = new FKCW_3D_Root();
		m_root3d->autorelease();
		m_root3d->init();
		m_root3d->setNodeName("root3d");
		this->addChild(m_root3d);

		// Step2: 创建摄像机
		FKCW_3D_Camera*camera = m_root3d->getCamera3D();
		m_r=(winSize.height/2)/tanf(camera->getFovy()/2*static_cast<float>(M_PI)/180.0f);
		camera->setEyePos(FKCW_3D_Vector4(0, 0, m_r, 1));
		camera->setCenter(FKCW_3D_Vector4(0, 0, 0, 1));
		camera->setUp(FKCW_3D_Vector4(0, 1, 0, 0));
		camera->setProjectionMode(eFKCW3D_PerspectiveMode);
		// Step3:  创建光源
		FKCW_3D_LightSource*lightSource=new FKCW_3D_LightSource();
		lightSource->autorelease();
		lightSource->init();
		m_root3d->addChild(lightSource);
		lightSource->setAmbient(FKCW_3D_Vector4(0.8f, 0.8f, 0.8f, 1.0f));
		lightSource->setPosition3D(FKCW_3D_Vector4(600.0f, 900.0f, 800.0f, 1.0f));
		// Step4: 设置Program
		FKCW_3D_Program*program=GetProgramClassicLighting();
		// 设置材质
		FKCW_3D_Material*material=new FKCW_3D_Material();
		material->autorelease();
		material->init();
		material->setSpecular(FKCW_3D_Vector4(0.5, 0.5, 0.5, 1));
		material->setShininess(10);
		// 设置纹理
		CCTexture2D* pTexBrick = CCTextureCache::sharedTextureCache()->addImage( s_szBrickPath );
		CCTexture2D* pTexLuFei = CCTextureCache::sharedTextureCache()->addImage( s_szHalfPhotoPath );
		CCTexture2D* pTexLogo = CCTextureCache::sharedTextureCache()->addImage( s_szCocosLogoPath );
		// 添加网格模型
		FKCW_3D_Mesh* mesh_ball = CreateBallMesh(100, 20, 10, FKCW_3D_Vector4(1, 0.5, 0, 1), FKCW_3D_Vector4(0, 1, 0, 1));
		mesh_ball->setTexture(pTexLuFei);
		mesh_ball->setMaterial(material);
		mesh_ball->setNodeName("mesh_ball");
		FKCW_3D_Mesh*mesh_box = CreateBoxMesh(100,FKCW_3D_Vector4(1, 0, 0, 1),FKCW_3D_Vector4(0, 1, 0, 1),FKCW_3D_Vector4(0, 0, 1, 1),FKCW_3D_Vector4(1, 1, 0, 1),
			FKCW_3D_Vector4(0, 0, 1, 1),FKCW_3D_Vector4(1, 1, 0, 1),FKCW_3D_Vector4(1, 0, 0, 1),FKCW_3D_Vector4(0, 1, 0, 1));
		mesh_box->setTexture(pTexBrick);
		mesh_box->setMaterial(material);
		mesh_box->setNodeName("mesh_box");
		FKCW_3D_Mesh*mesh_cone = CreateConeMesh(100, 270, 20, 10, true, FKCW_3D_Vector4(1, 0, 0, 1), FKCW_3D_Vector4(0, 1, 0, 1));
		mesh_cone->setTexture(pTexLogo);
		mesh_cone->setMaterial(material);
		mesh_cone->setNodeName("mesh_cone");

		// Step5: 添加Actor对象
		const int actorCount = 3;
		vector<FKCW_3D_Mesh*> meshList;
		meshList.push_back(mesh_box);
		meshList.push_back(mesh_cone);
		meshList.push_back(mesh_ball);
		vector<FKCW_3D_Vector4> posList;
		posList.push_back(FKCW_3D_Vector4(-250, 0, 0, 1));
		posList.push_back(FKCW_3D_Vector4(0, -100, 180, 1));
		posList.push_back(FKCW_3D_Vector4(250, 0, 0, 1));
		for(int i=0;i<actorCount;i++)
		{
			FKCW_3D_Actor* actor3D = new FKCW_3D_Actor();
			actor3D->autorelease();
			actor3D->init();
			actor3D->setPosition3D(posList[i]);
			actor3D->addMesh(meshList[i]);
			actor3D->setLightSource(lightSource);
			actor3D->setCamera3D(camera);
			actor3D->setPassUnifoCallback(PassUnifoCallback_ClassicLighting);
			actor3D->setProgram(program);
			actor3D->setNodeName("actor3d");
			actor3D->scale3D(0.5f, 0.5f, 0.5f);
			m_root3d->addChild(actor3D,0);
			m_actor3dList.push_back(actor3D);
		}
		// Step6: 提交Actor
		for(int i=0;i<(int)m_actor3dList.size();i++)
		{
			FKCW_3D_Actor*actor3D = m_actor3dList[i];
			actor3D->submit(GL_STATIC_DRAW);
		}

		{
			// 添加新的Actor
			m_pActor3D=CreateActorByFKSimpleFile( s_sz3DAppleDir );
			m_pActor3D->setLightSource(lightSource);
			m_pActor3D->setCamera3D(camera);
			m_pActor3D->setPassUnifoCallback(PassUnifoCallback_ClassicLighting);
			m_pActor3D->setProgram(program);
			m_pActor3D->setNodeName("actor3D");
			m_root3d->addChild(m_pActor3D,0);

			m_pActor3D->scale3D(2, 2, 2);
			m_pActor3D->setPosition3D(FKCW_3D_Vector4(0, 120, 0,1));

			// 提交
			m_pActor3D->submit(GL_STATIC_DRAW);
		}

		{
			CCTexture2D*tex = CCTextureCache::sharedTextureCache()->addImage( s_szWoodPath );
			float r = 40;
			m_nRow = 3;
			m_nCol = 4;
			m_nDeep = 2;
			m_actor3dMat3d.resize(m_nRow);
			for(int i=0;i<m_nRow;i++){
				m_actor3dMat3d[i].resize(m_nCol);
				for(int j=0;j<m_nCol;j++){
					m_actor3dMat3d[i][j].resize(m_nDeep);
				}
			}
			float xspan = m_nCol*2*r;
			float yspan = m_nRow*2*r;
			float zspan = m_nDeep*2*r;
			float xmin =- xspan/2;
			float ymin =- yspan/2;
			float zmin =- zspan/2;
			for(int i=0;i<m_nRow;i++){
				for(int j=0;j<m_nCol;j++){
					for(int k=0;k<m_nDeep;k++){
						// 创建Mesh
						FKCW_3D_Mesh*mesh_box=CreateBoxMesh(r, FKCW_3D_V4(1, 1, 1, 1),FKCW_3D_V4(1, 1, 1, 1),
							FKCW_3D_V4(1, 1, 1, 1),FKCW_3D_V4(1, 1, 1, 1),
							FKCW_3D_V4(1, 1, 1, 1),FKCW_3D_V4(1, 1, 1, 1),FKCW_3D_V4(1, 1, 1, 1),FKCW_3D_V4(1, 1, 1, 1));
						mesh_box->setTexture(tex);
						mesh_box->setMaterial(material);
						mesh_box->setNodeName("mesh_box");
						// 创建Actor
						FKCW_3D_Actor* actor3D=new FKCW_3D_Actor();
						actor3D->autorelease();
						actor3D->init();
						float x = xmin+r+j*2*r;
						float y = ymin+r+(m_nRow-1-i)*2*r;
						float z = zmin+r+(m_nDeep-1-k)*2*r;
						actor3D->setPosition3D(FKCW_3D_V4(x, y, z, 1));
						actor3D->addMesh(mesh_box);
						actor3D->setLightSource(lightSource);
						actor3D->setCamera3D(camera);
						actor3D->setPassUnifoCallback(PassUnifoCallback_ClassicLighting);
						actor3D->setProgram(program);
						actor3D->setNodeName("actor3d");
						m_root3d->addChild(actor3D,0);
						m_actor3dMat3d[i][j][k]=actor3D;

					}
				}
			}
			// 提交
			for(int i=0;i<m_nRow;i++){
				for(int j=0;j<m_nCol;j++){
					for(int k=0;k<m_nDeep;k++){
						FKCW_3D_Actor*actor3D = m_actor3dMat3d[i][j][k];
						actor3D->submit(GL_STATIC_DRAW);
					}
				}
			}
		}

		{
// 			CCScale9Sprite* btnUp = CCScale9Sprite::create( s_szSprite9Btn1Path );
// 			CCScale9Sprite* btnDn = CCScale9Sprite::create( s_szSprite9Btn2Path );
// 			CCLabelTTF*title = CCLabelTTF::create(A2U("清除子块").c_str(), "Arial", 26);
// 			CCControlButton* controlButton = CCControlButton::create(title, btnUp);
// 			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
// 			controlButton->setPreferredSize(CCSize(180,50));
// 			controlButton->setPosition(ccp(SCREEN_CENTER_POS_X,50));
// 			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_3D_FKSimpleModelLayer::clearCallBack), CCControlEventTouchDown);
// 			this->addChild(controlButton);
// 			m_controlButton_Clear=controlButton;
		}

		{
			CCScale9Sprite* btnUp = CCScale9Sprite::create( s_szSprite9Btn1Path );
			CCScale9Sprite* btnDn = CCScale9Sprite::create( s_szSprite9Btn2Path );
			CCLabelTTF*title = CCLabelTTF::create(A2U("投影模式").c_str(), "Arial", 26);
			CCControlButton* controlButton = CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(180,50));
			controlButton->setPosition(ccp(SCREEN_CENTER_POS_X+200,50));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_3D_FKSimpleModelLayer::switchProjModeCallBack), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_swithProjMode = controlButton;
		}

		{
			CCScale9Sprite* btnUp = CCScale9Sprite::create(s_szSprite9Btn1Path );
			CCScale9Sprite* btnDn = CCScale9Sprite::create(s_szSprite9Btn2Path );
			CCLabelTTF*title = CCLabelTTF::create(A2U("旋转").c_str(), "Arial", 26);
			CCControlButton* controlButton = CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(180,50));
			controlButton->setPosition(ccp(SCREEN_CENTER_POS_X-200,50));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_3D_FKSimpleModelLayer::transformCallBack), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_transform = controlButton;
		}
		// 矩阵模式
		m_pLabel=CCLabelTTF::create(A2U("透视投影").c_str(), "Arial", 26);
		m_pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
			origin.y + visibleSize.height - m_pLabel->getContentSize().height-50));
		this->addChild(m_pLabel, 1);

		return true;
	}
public:
	CREATE_FUNC( CTest_3D_FKSimpleModelLayer );
public:
	void clearCallBack(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent)
	{
		for(int i=0;i<m_nRow;i++)
		{
			for(int j=0;j<m_nCol;j++)
			{
				for(int k=0;k<m_nDeep;k++)
				{
					FKCW_3D_Actor*actor = m_actor3dMat3d[i][j][k];
					int meshCount = actor->getModel()->getMeshCount();
					for(int u=0;u<meshCount;u++)
					{
						FKCW_3D_Mesh*mesh = actor->getModel()->getMeshByIndex(u);
						int subMeshCount = mesh->getSubMeshCount();
						for(int v=0;v<subMeshCount;v++)
						{
							FKCW_3D_SubMesh*subMesh = mesh->getSubMeshByIndex(v);
							FKCW_3D_SubMeshData*subMeshData = subMesh->getSubMeshData();
							vector<FKCW_3D_Vector4>&colorList = subMeshData->m_colorList;
							for(int k=0;k<(int)colorList.size();k++)
							{
								colorList[k] = FKCW_3D_Vector4(1,1,1,1);
							}
							subMesh->submitColor(GL_STREAM_DRAW);
						}
					}
				}
			}
		}
	}
	void switchProjModeCallBack(CCObject *senderz, CCControlEvent controlEvent)
	{
		FKCW_3D_Camera*camera=m_root3d->getCamera3D();
		switch(camera->getProjectionMode())
		{
			case eFKCW3D_PerspectiveMode:
			{
				camera->setProjectionMode(eFKCW3D_OrthographicMode);
				m_pLabel->setString(A2U("透视投影").c_str());
			}
			break;
			case eFKCW3D_OrthographicMode:
			{
				camera->setProjectionMode(eFKCW3D_PerspectiveMode);
				m_pLabel->setString(A2U("正交投影").c_str());
			}
			break;
		}
	}
	void transformCallBack(CCObject *senderz, CCControlEvent controlEvent)
	{
		if(m_isDoUpdate)
		{
			int nActor3D=(int)m_actor3dList.size();
			for(int i=0;i<nActor3D;i++){
				m_actor3dList[i]->setTransform3D(initalMatList[i]);
			}
			// 停止更新
			m_isDoUpdate=false;

		}
		else
		{
			initalMatList.clear();
			int nActor3D=(int)m_actor3dList.size();
			for(int i=0;i<nActor3D;i++){
				initalMatList.push_back(m_actor3dList[i]->getTransform3D());
			}
			// 开始位置更新
			m_isDoUpdate=true;
		}
	}
	void update(float dt)
	{
		// 位置旋转更新
		if(m_isDoUpdate==false)return;
		m_actor3dList[0]->rotateRelativeToParent3D(FKCW_3D_V4(0, 1, 0, 0), 120*dt);
		m_actor3dList[0]->rotate3D(FKCW_3D_V4(1, 0, 0, 0), 120*dt);
		m_actor3dList[2]->rotateRelativeToParent3D(FKCW_3D_V4(0, 1, 0, 0), 120*dt);
		m_actor3dList[2]->rotate3D(FKCW_3D_V4(0, 1, 0, 0), 240*dt);
	}
	void updateEyePos()
	{
		float cosA=cosf(m_A*static_cast<float>(M_PI)/180.0f);
		float sinA=sinf(m_A*static_cast<float>(M_PI)/180.0f);
		float cosB=cosf(m_B*static_cast<float>(M_PI)/180.0f);
		float sinB=sinf(m_B*static_cast<float>(M_PI)/180.0f);
		m_eyePos.setx(m_r*cosB*sinA);
		m_eyePos.sety(m_r*sinB);
		m_eyePos.setz(m_r*cosB*cosA);
		m_eyePos.setw(1);
	}
public:
		virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
		{
			CCSetIterator it;
			CCTouch* touch;
			for( it = touches->begin(); it != touches->end(); it++)
			{
				touch = (CCTouch*)(*it);
				if(!touch)
					break;

				CCPoint pointInWinSpace = touch->getLocationInView();
				m_mosPosf=m_mosPos;
				m_mosPos=pointInWinSpace;

				//----------------------------------------------------------
				CCRect viewportRect=cocos2d::CCEGLView::sharedOpenGLView()->getViewPortRect();
				float viewport[4]={viewportRect.getMinX(),viewportRect.getMinY(),viewportRect.size.width,viewportRect.size.height};

				FKCW_3D_Camera*camera=m_root3d->getCamera3D();
				FKCW_3D_Vector4 eyePos=camera->getEyePos();
				FKCW_3D_Vector4 winPos=FKCW_3D_Vector4(m_mosPos.x,viewport[3]-m_mosPos.y,0,1);
				FKCW_3D_Matrix4 projMat=camera->calculateProjectionMat();
				FKCW_3D_Matrix4 viewMat=camera->calculateViewMat();
				FKCW_3D_Matrix4 projViewInverse=inverse(projMat*viewMat);
				FKCW_3D_Vector4 worldPos= convertFromViewportSpaceToWorldSpace(winPos, projMat, viewMat, viewport);


				int i_firstIntersectActor=-1;
				int j_firstIntersectActor=-1;
				int k_firstIntersectActor=-1;
				float minDisIntersectPointToEyePos=static_cast<float>(INFINITY);

				for(int i=0;i<m_nRow;i++)
				{
					for(int j=0;j<m_nCol;j++)
					{
						for(int k=0;k<m_nDeep;k++)
						{
							FKCW_3D_Actor*actor=m_actor3dMat3d[i][j][k];
							int meshCount=actor->getModel()->getMeshCount();
							for(int u=0;u<meshCount;u++)
							{
								FKCW_3D_Mesh*mesh=actor->getModel()->getMeshByIndex(u);
								int subMeshCount=mesh->getSubMeshCount();
								for(int v=0;v<subMeshCount;v++)
								{
									FKCW_3D_SubMesh*subMesh=mesh->getSubMeshByIndex(v);
									FKCW_3D_SubMeshData*subMeshData=subMesh->getSubMeshData();
									const vector<FKCW_3D_Vector4>&positionList=subMeshData->m_positionList;
									const vector<FKCW_3D_IDTriangle>&IDtriList=subMeshData->m_IDtriList;
									int nIDtri=(int)IDtriList.size();
									for(int w=0;w<nIDtri;w++)
									{
										const FKCW_3D_IDTriangle&IDtri=IDtriList[w];
										int ID0=IDtri.vID(0);
										int ID1=IDtri.vID(1);
										int ID2=IDtri.vID(2);
										FKCW_3D_Vector4 p0=subMesh->convertToWorldSpace3D(positionList[ID0]);
										FKCW_3D_Vector4 p1=subMesh->convertToWorldSpace3D(positionList[ID1]);
										FKCW_3D_Vector4 p2=subMesh->convertToWorldSpace3D(positionList[ID2]);
										if(isLine3dIntersectWithTriangle3d(eyePos, worldPos-eyePos, p0, p1, p2))
										{
											FKCW_3D_Vector4 intersectPos=getIntersectPointOfLine3dIntersectWithPlane3d(eyePos, worldPos-eyePos, p0, cross(p1-p0, p2-p1));
											float disIntersectPointToEyePos=getLength(intersectPos-eyePos);
											if(disIntersectPointToEyePos<minDisIntersectPointToEyePos){
												minDisIntersectPointToEyePos=disIntersectPointToEyePos;
												i_firstIntersectActor=i;
												j_firstIntersectActor=j;
												k_firstIntersectActor=k;
											}
										}
									}
								}
							}
						}
					}
				}
				if(i_firstIntersectActor!=-1)
				{
					FKCW_3D_Actor*firstIntersectActor=m_actor3dMat3d[i_firstIntersectActor][j_firstIntersectActor][k_firstIntersectActor];
					int meshCount=firstIntersectActor->getModel()->getMeshCount();
					for(int i=0;i<meshCount;i++)
					{
						FKCW_3D_Mesh*mesh=firstIntersectActor->getModel()->getMeshByIndex(i);
						int subMeshCount=mesh->getSubMeshCount();
						for(int j=0;j<subMeshCount;j++)
						{
							FKCW_3D_SubMesh*subMesh=mesh->getSubMeshByIndex(j);
							FKCW_3D_SubMeshData*subMeshData=subMesh->getSubMeshData();
							vector<FKCW_3D_Vector4>&colorList=subMeshData->m_colorList;
							for(int k=0;k<(int)colorList.size();k++)
							{
								colorList[k]=FKCW_3D_Vector4(0,0,1,1);
							}
							subMesh->submitColor(GL_STREAM_DRAW);
						}
					}
				}
			}
		}
		virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
		{
			CCSetIterator it;
			CCTouch* touch;
			for( it = touches->begin(); it != touches->end(); it++)
			{
				touch = (CCTouch*)(*it);
				if(!touch)
					break;

				CCPoint pointInWinSpace = touch->getLocationInView();
				m_mosPosf=m_mosPos;
				m_mosPos=pointInWinSpace;
				m_A+=-(m_mosPos.x-m_mosPosf.x)*0.4f;
				m_B+=(m_mosPos.y-m_mosPosf.y)*0.4f;
				if(m_B>89.9f)m_B=89.9f;
				if(m_B<-89.9f)m_B=-89.9f;
				updateEyePos();
				m_root3d->getCamera3D()->setEyePos(m_eyePos);
			}
		}
		virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
		{
			CCSetIterator it;
			CCTouch* touch;
			for( it = touches->begin(); it != touches->end(); it++)
			{
				touch = (CCTouch*)(*it);

				if(!touch)
					break;

				CCPoint pointInWinSpace = touch->getLocationInView();
				m_mosPosf=m_mosPos;
				m_mosPos=pointInWinSpace;
			}
		}
public:
	CCControlButton* m_controlButton_swithProjMode;
	CCControlButton* m_controlButton_transform;
//	CCControlButton* m_controlButton_Clear;
	CCLabelTTF* m_pLabel;					// 当前投影模式提示文字
	bool m_isDoUpdate;						// 是否自动更新位置
	CCPoint m_mosPos;						// 鼠标位置记录
	CCPoint m_mosPosf;						// 鼠标上帧位置记录
	FKCW_3D_Vector4 m_eyePos;				// 摄像机位置
	FKCW_3D_Vector4 m_eyeTg;				// 摄像机朝向
	float m_r;								// 摄像机eyePos 到 eyeTg之间的距离
	float m_A;								// XZ面之间夹角
	float m_B;
	FKCW_3D_Root*m_root3d;					// 3D根节点
	vector<FKCW_3D_Actor*> m_actor3dList;	// FKCW_3D_Actor列表
	vector<FKCW_3D_Matrix4> initalMatList;	// FKCW_3D_Actor初始转换矩阵
	FKCW_3D_Actor* m_pActor3D;				// 新的AppleActor

	vector<vector<vector<FKCW_3D_Actor*> > > m_actor3dMat3d;
	int m_nRow;
	int m_nCol;
	int m_nDeep;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(3D, FKSimpleModel, 3D渲染测试以及FKSimpleModel文件支持测试)
//-------------------------------------------------------------------------