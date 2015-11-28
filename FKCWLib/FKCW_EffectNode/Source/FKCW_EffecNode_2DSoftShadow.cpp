//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_2DSoftShadow.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
//-------------------------------------------------------------------------
const char* s_2DSoftShadowFragShader = "																									\n\
							  #ifdef GL_ES																									\n\
							  precision highp float;																						\n\
							  #endif																										\n\
							  varying vec4 v_fragmentColor;																					\n\
							  varying vec2 v_texCoord;																						\n\
							  uniform sampler2D CC_Texture0;																				\n\
							  uniform float u_oneDivObjCount;																				\n\
							  void main()																									\n\
							  {																												\n\
								float texLightStrength=texture2D(CC_Texture0, v_texCoord).r;												\n\
								float lightStrength_start=v_fragmentColor.r;																\n\
								float lightStrength_end=v_fragmentColor.g;																	\n\
								float finalLightStrength=texLightStrength*(lightStrength_start-lightStrength_end)+lightStrength_end;		\n\
								finalLightStrength=finalLightStrength*u_oneDivObjCount;														\n\
								gl_FragColor=vec4(vec3(1,1,1),finalLightStrength);															\n\
							  }";
const GLchar* s_2DSoftShadowRenderShadowFragShader = "\n\
											 #ifdef GL_ES													\n\
											 precision mediump float;										\n\
											 #endif															\n\
											 varying vec4 v_fragmentColor;									\n\
											 varying vec2 v_texCoord;										\n\
											 uniform sampler2D CC_Texture0;									\n\
											 uniform int u_objCount;										\n\
											 uniform float u_shadowDarkness;//值从0到1.0(1.0是全黑)			\n\
											 void main()													\n\
											 {																\n\
												vec2 texCoord=vec2(v_texCoord.x,v_texCoord.y);				\n\
												float grey=texture2D(CC_Texture0, texCoord).r;				\n\
																											\n\
												float alpha=1.0-grey;										\n\
												alpha=min(1.0,alpha*float(u_objCount));						\n\
												const float brightFactor=0.5;								\n\
												alpha*=u_shadowDarkness;									\n\
												gl_FragColor=vec4(vec3(0,0,0),alpha)  ;						\n\
											 }";															 
//-------------------------------------------------------------------------
// FKCW_EffectNode_2DSoftShadow_Edge
//-------------------------------------------------------------------------
FKCW_EffectNode_2DSoftShadow_Edge::FKCW_EffectNode_2DSoftShadow_Edge(const CCPoint&start,const CCPoint&end)
{
	m_start=start;
	m_end=end;
}
//-------------------------------------------------------------------------
// FKCW_EffectNode_2DSoftShadow_LightNode
//-------------------------------------------------------------------------
FKCW_EffectNode_2DSoftShadow_LightNode::FKCW_EffectNode_2DSoftShadow_LightNode()
{
	m_fR=0;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_2DSoftShadow_LightNode::FKCW_EffectNode_2DSoftShadow_LightNode(float r)
{
	init( r );
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_2DSoftShadow_LightNode::init(float r)
{
	m_fR=r;
	this->CCNode::init();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_2DSoftShadow_LightNode::draw()
{
	if(m_bIsDebugDraw)
	{
		glLineWidth(2);
		ccDrawColor4F(0, 0, 1, 1);
		ccDrawCircle(CCPoint(0,0), m_fR, 360, 30, false, 1, 1);
	}
}
//-------------------------------------------------------------------------
// FKCW_Effect_2DSoftShadow_PointType
//-------------------------------------------------------------------------
FKCW_Effect_2DSoftShadow_PointType::FKCW_Effect_2DSoftShadow_PointType(){
	m_isLeftUmbraPoint=false;
	m_isRightUmbraPoint=false;
	m_isLeftPenumbraPoint=false;
	m_isRightPenumbraPoint=false;
}
//-------------------------------------------------------------------------
// FKCW_Effect_2DSoftShadow_ShadowObj
//-------------------------------------------------------------------------
FKCW_Effect_2DSoftShadow_ShadowObj::FKCW_Effect_2DSoftShadow_ShadowObj()
{
	m_light=NULL;
	m_mesh=NULL;
	m_finTexture=NULL;
	m_indexVBO=NULL;
	m_program=NULL;
	m_shadowLength=1500;
	m_leftPenumbraLine.resize(3);
	m_rightPenumbraLine.resize(3);
	m_leftUmbraLine.resize(3);
	m_rightUmbraLine.resize(3);
	m_leftPenumbraPointID=-1;
	m_rightPenumbraPointID=-1;
	m_leftUmbraPointID=-1;
	m_rightUmbraPointID=-1;
	m_oneDivObjCount=1.0;
	m_isDrawDebug=false;
	m_isDrawNonDebug=true;
	m_isUpdateShadowSucc=false;
}
//-------------------------------------------------------------------------
FKCW_Effect_2DSoftShadow_ShadowObj::~FKCW_Effect_2DSoftShadow_ShadowObj()
{
	CC_SAFE_RELEASE( m_light );
	if(m_mesh)m_mesh->release();
	if(m_finTexture)m_finTexture->release();
	if(m_indexVBO)m_indexVBO->release();
	if(m_program)m_program->release();
}
//-------------------------------------------------------------------------
bool FKCW_Effect_2DSoftShadow_ShadowObj::init(const vector<CCPoint>& polygon)
{
	m_polygon=polygon;
	// 初始化精灵
	this->CCSprite::init();
	// 开启帧Update
	this->scheduleUpdate();
	// 创建Mesh
	m_mesh=new FKCW_EffectNode_Mesh();
	m_mesh->autorelease();
	m_mesh->retain();
	// 创建VBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 创建纹理
	m_finTexture = CCTextureCache::sharedTextureCache()->addImage( "Engine/2DSoftShadow_lightFin.png" );
	m_finTexture->retain();
	// 创建并设置GLProgram
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_2DSoftShadowFragShader);
		// 绑定归属
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		// 连接
		program->link();
		program->updateUniforms();
		// 绑定我们自己的Uniform
		program->attachUniform("u_oneDivObjCount");
		// 设置GLProgram
		m_program=program;
		m_program->retain();
		// 检查GL错误
		CHECK_GL_ERROR_DEBUG();
	}
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::update(float dt)
{
	m_isUpdateShadowSucc=updateShadow();
	if(m_isUpdateShadowSucc==false){
		_MakeFullWindowRectMesh();
		_Submit(GL_DYNAMIC_DRAW);
		return;
	}
	_UpdateMesh();
	_Submit(GL_DYNAMIC_DRAW);
}
//-------------------------------------------------------------------------
bool FKCW_Effect_2DSoftShadow_ShadowObj::updateShadow(){
	if(m_light==NULL)return false;
	CCPoint lightPosLocal=_GetLightPosLocal();
	m_pointTypeList.clear();
	int nPoint=(int)m_polygon.size();
	m_pointTypeList.resize(nPoint);
	for(int i=0;i<nPoint;i++)
	{
		const CCPoint&P=m_polygon[i];
		FKCW_Effect_2DSoftShadow_PointType&pointType=m_pointTypeList[i];
		vector<CCPoint> LRT=FKCW_Base_Utils::CalculateTangentPointsOfPointToCircle(lightPosLocal, m_light->getR(), P);
		if(LRT.empty())
		{
			return false;
		}
		CCPoint LT=LRT[0];
		CCPoint RT=LRT[1];
		CCPoint PLT=LT-P;
		CCPoint PRT=RT-P;

		// 检查每个点，判断它在PLT和PRT哪边，计算获得 sideTypeList_PLT 和 sideTypeList_PRT
		vector<ENUM_SideType> sideTypeList_PLT;
		vector<ENUM_SideType> sideTypeList_PRT;
		for(int j=0;j<nPoint;j++)
		{
			const CCPoint&p=m_polygon[j];
			CCPoint Pp=p-P;
			if(j==i)
			{
				sideTypeList_PLT.push_back(eOn);
				sideTypeList_PRT.push_back(eOn);
			}
			else
			{
				// sideTypeList_PLT
				{
					float cross=ccpCross(PLT, Pp);
					if(cross==0){
						sideTypeList_PLT.push_back(eOn);
					}else if(cross>0){
						sideTypeList_PLT.push_back(eLeftSide);
					}else{
						sideTypeList_PLT.push_back(eRightSide);
					}
				}
				// sideTypeList_PRT
				{
					float cross=ccpCross(PRT, Pp);
					if(cross==0){
						sideTypeList_PRT.push_back(eOn);
					}else if(cross>0){
						sideTypeList_PRT.push_back(eLeftSide);
					}else{
						sideTypeList_PRT.push_back(eRightSide);
					}
				}
			}
		}

		// 检查是否所有点都在PRT左边或在PRT上。
		{
			bool allOnLeftOrOn=true;
			for(int j=0;j<nPoint;j++)
			{
				ENUM_SideType sideType=sideTypeList_PRT[j];
				if(sideType==eLeftSide||sideType==eOn)
				{
				}else
				{
					allOnLeftOrOn=false;
					break;
				}
			}
			// 若是
			if(allOnLeftOrOn){
				pointType.m_isLeftUmbraPoint=true;
				pointType.m_leftUmbraT=RT;
			}
		}

		// 检查是否所有点都在PLT右边或在PLT上。
		{
			bool allOnRightOrOn=true;
			for(int j=0;j<nPoint;j++){
				ENUM_SideType sideType=sideTypeList_PLT[j];
				if(sideType==eRightSide||sideType==eOn){
				}else{
					allOnRightOrOn=false;
					break;
				}
			}
			// 若是
			if(allOnRightOrOn){
				pointType.m_isRightUmbraPoint=true;
				pointType.m_rightUmbraT=LT;
			}
		}
		
		// 检查是否所有点都在PLT左边或在PLT上。
		{
			bool allOnLeftOrOn=true;
			for(int j=0;j<nPoint;j++){
				ENUM_SideType sideType=sideTypeList_PLT[j];
				if(sideType==eLeftSide||sideType==eOn){
				}else{
					allOnLeftOrOn=false;
					break;
				}
			}
			// 若是
			if(allOnLeftOrOn){
				pointType.m_isLeftPenumbraPoint=true;
				pointType.m_leftPenumbraT=LT;
			}
		}

		// 检查是否所有点都在PRT右边或在PRT上。
		{
			bool allOnRightOrOn=true;
			for(int j=0;j<nPoint;j++){
				ENUM_SideType sideType=sideTypeList_PRT[j];
				if(sideType==eRightSide||sideType==eOn){
				}else{
					allOnRightOrOn=false;
					break;
				}
			}
			// 若是
			if(allOnRightOrOn){
				pointType.m_isRightPenumbraPoint=true;
				pointType.m_rightPenumbraT=RT;
			}
		}

	}
	// 得到了全部m_pointTypeList
	int nPointType=(int)m_pointTypeList.size();
	for(int i=0;i<nPointType;i++)
	{
		FKCW_Effect_2DSoftShadow_PointType&pointType=m_pointTypeList[i];
		FKCW_Effect_2DSoftShadow_PointType&pointTypen=m_pointTypeList[(i+1)%nPointType];
		if(pointType.m_isLeftPenumbraPoint&&pointTypen.m_isLeftPenumbraPoint){
			pointType.m_isLeftPenumbraPoint=false;
		}
		if(pointType.m_isRightPenumbraPoint&&pointTypen.m_isRightPenumbraPoint){
			pointType.m_isRightPenumbraPoint=false;
		}
		if(pointType.m_isLeftUmbraPoint&&pointTypen.m_isLeftUmbraPoint){
			pointType.m_isLeftUmbraPoint=false;
		}
		if(pointType.m_isRightUmbraPoint&&pointTypen.m_isRightUmbraPoint){
			pointType.m_isRightUmbraPoint=false;
		}
	}

	m_leftPenumbraPointID=-1;
	m_rightPenumbraPointID=-1;
	m_leftUmbraPointID=-1;
	m_rightUmbraPointID=-1;
	for(int i=0;i<nPointType;i++)
	{
		const CCPoint&point=m_polygon[i];
		const FKCW_Effect_2DSoftShadow_PointType&pointType=m_pointTypeList[i];
		if(pointType.m_isLeftPenumbraPoint)
		{
			m_leftPenumbraLine[1]=point;
			m_leftPenumbraLine[0]=pointType.m_leftPenumbraT;
			m_leftPenumbraLine[2]=m_leftPenumbraLine[1]+ccpMult(ccpNormalize(m_leftPenumbraLine[1]-m_leftPenumbraLine[0]), m_shadowLength);
			m_leftPenumbraPointID=i;
		}
		if(pointType.m_isRightPenumbraPoint)
		{
			m_rightPenumbraLine[1]=point;
			m_rightPenumbraLine[0]=pointType.m_rightPenumbraT;
			m_rightPenumbraLine[2]=m_rightPenumbraLine[1]+ccpMult(ccpNormalize(m_rightPenumbraLine[1]-m_rightPenumbraLine[0]), m_shadowLength);
			m_rightPenumbraPointID=i;
		}
		if(pointType.m_isLeftUmbraPoint)
		{
			m_leftUmbraLine[1]=point;
			m_leftUmbraLine[0]=pointType.m_leftUmbraT;
			m_leftUmbraLine[2]=m_leftUmbraLine[1]+ccpMult(ccpNormalize(m_leftUmbraLine[1]-m_leftUmbraLine[0]), m_shadowLength);
			m_leftUmbraPointID=i;
		}
		if(pointType.m_isRightUmbraPoint)
		{
			m_rightUmbraLine[1]=point;
			m_rightUmbraLine[0]=pointType.m_rightUmbraT;
			m_rightUmbraLine[2]=m_rightUmbraLine[1]+ccpMult(ccpNormalize(m_rightUmbraLine[1]-m_rightUmbraLine[0]), m_shadowLength);
			m_rightUmbraPointID=i;
		}
	}

	if(m_leftPenumbraPointID==-1)return false;
	if(m_rightPenumbraPointID==-1)return false;
	if(m_leftUmbraPointID==-1)return false;
	if(m_rightUmbraPointID==-1)return false;

	// 计算m_rightUmbraLine 和 m_leftUmbraLine的交点
	m_intersectPoint=FKCW_Base_Utils::GetIntersectPointOfTwoLine(m_leftUmbraLine[0], ccpNormalize(m_leftUmbraLine[1]-m_leftUmbraLine[0]),
		m_rightUmbraLine[0], ccpNormalize(m_rightUmbraLine[1]-m_rightUmbraLine[0]));

	return true;
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::draw()
{
	// 绘制mesh
	if(m_isDrawNonDebug)
	{
		{
			// 更改shader
			ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE};
			this->setBlendFunc(blendFunc);
			ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
			ccGLEnable(m_eGLServerState);
			this->setShaderProgram(m_program);
			FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
			program->use();
			program->setUniformsForBuiltins();
			program->passUnifoValue1f("u_oneDivObjCount", m_oneDivObjCount);
			bool isAttribPositionOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayPosition();
			bool isAttribColorOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayColor();
			bool isAttribTexCoordOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayTexCoord();
			FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(true);
			FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(true);
			FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(true);
			// 绑定纹理
			ccGLBindTexture2D( m_finTexture->getName() );
			// 绘制 m_indexVBO
			m_indexVBO->SetPointerPosition();
			m_indexVBO->SetPointerTexCoord();
			m_indexVBO->SetPointerColor();
			m_indexVBO->Draw(GL_TRIANGLES);

			// 卸载纹理
			ccGLBindTexture2D(0);

			FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
			FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
			FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
		}
	}
	if(m_isDrawDebug)
	{
		if(m_isUpdateShadowSucc)
		{
			ccBlendFunc blendFunc={GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
			this->setBlendFunc(blendFunc);
			ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
			// 绘线
			glLineWidth(2);
			ccDrawColor4F(0, 0, 1, 1);
			ccDrawPoly(&m_polygon.front(), (int)m_polygon.size(), true);

			// 光线节点位置
			ccPointSize(4);
			ccDrawColor4F(0, 0, 1, 1);
			ccDrawPoint(_GetLightPosLocal());

			// 交点
			if(FKCW_Base_Utils::IsPointEqual(m_intersectPoint, CCPoint(INFINITY,INFINITY), 0)==false)
			{
				ccPointSize(4);
				ccDrawColor4F(0, 0, 1, 1);
				ccDrawPoint(m_intersectPoint);

				ccDrawLine(m_intersectPoint, m_leftUmbraLine[0]);
				ccDrawLine(m_intersectPoint, m_rightUmbraLine[0]);
			}

			//m_leftPenumbraLine
			{
				ccDrawColor4F(1, 0, 0, 1);
				glLineWidth(1);
				ccDrawLine(m_leftPenumbraLine[0], m_leftPenumbraLine[2]);
				ccDrawCircle(m_leftPenumbraLine[1], 5, 360, 10, false, 1, 1);
			}
			//m_rightPenumbraLine
			{
				ccDrawColor4F(0, 1, 0, 1);
				glLineWidth(1);
				ccDrawLine(m_rightPenumbraLine[0], m_rightPenumbraLine[2]);
				ccDrawCircle(m_rightPenumbraLine[1], 5, 360, 10, false, 1, 1);
			}
			//m_leftUmbraLine
			{
				ccDrawColor4F(1, 0, 0, 1);
				glLineWidth(1);
				ccDrawLine(m_leftUmbraLine[0], m_leftUmbraLine[2]);
				ccPointSize(5);
				ccDrawPoint(m_leftUmbraLine[1]);
			}
			//m_rightUmbraLine
			{
				ccDrawColor4F(0, 1, 0, 1);
				glLineWidth(1);
				ccDrawLine(m_rightUmbraLine[0], m_rightUmbraLine[2]);
				ccPointSize(5);
				ccDrawPoint(m_rightUmbraLine[1]);
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::setLight(FKCW_EffectNode_2DSoftShadow_LightNode*light)
{
	if(m_light==NULL){
		m_light=light;
		m_light->retain();
	}else{
		m_light->release();
		m_light=light;
		m_light->retain();
	}
}
//-------------------------------------------------------------------------
CCPoint FKCW_Effect_2DSoftShadow_ShadowObj::_GetLightPosLocal()
{
	CCPoint lightPosWorld=m_light->convertToWorldSpaceAR(ccp(0,0));
	CCPoint lightPosLocal=this->convertToNodeSpace(lightPosWorld);
	return lightPosLocal;
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::_MakeFullWindowRectMesh()
{
	m_mesh->clear();
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	const CCPoint p0World=CCPoint(winSize.width,winSize.height);//RU
	const CCPoint p1World=CCPoint(0,winSize.height);//LU
	const CCPoint p2World=CCPoint(0,0);//LD
	const CCPoint p3World=CCPoint(winSize.width,0);//RD
	const CCPoint p0=convertToNodeSpace(p0World);
	const CCPoint p1=convertToNodeSpace(p1World);
	const CCPoint p2=convertToNodeSpace(p2World);
	const CCPoint p3=convertToNodeSpace(p3World);
	//v0
	Cv2 pos0=Cv2::CCPToV2(p0);
	Cv2 texCoord0=Cv2(0,0);
	Cv4 color0=Cv4(1,1,0,1);
	//v1
	Cv2 pos1=Cv2::CCPToV2(p1);
	Cv2 texCoord1=Cv2(0,0);
	Cv4 color1=Cv4(1,1,0,1);
	//v2
	Cv2 pos2=Cv2::CCPToV2(p2);
	Cv2 texCoord2=Cv2(0,0);
	Cv4 color2=Cv4(1,1,0,1);
	//v3
	Cv2 pos3=Cv2::CCPToV2(p3);
	Cv2 texCoord3=Cv2(0,0);
	Cv4 color3=Cv4(1,1,0,1);
	//add v0
	m_mesh->vlist.push_back(pos0);
	m_mesh->texCoordList.push_back(texCoord0);
	m_mesh->colorList.push_back(color0);
	int ID0=(int)m_mesh->vlist.size()-1;
	//add v1
	m_mesh->vlist.push_back(pos1);
	m_mesh->texCoordList.push_back(texCoord1);
	m_mesh->colorList.push_back(color1);
	int ID1=(int)m_mesh->vlist.size()-1;
	//add v2
	m_mesh->vlist.push_back(pos2);
	m_mesh->texCoordList.push_back(texCoord2);
	m_mesh->colorList.push_back(color2);
	int ID2=(int)m_mesh->vlist.size()-1;
	//add v3
	m_mesh->vlist.push_back(pos3);
	m_mesh->texCoordList.push_back(texCoord3);
	m_mesh->colorList.push_back(color3);
	int ID3=(int)m_mesh->vlist.size()-1;
	//add IDtri
	m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
	m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID2,ID3));
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::_UpdateMesh()
{
	m_mesh->clear();
	CCPoint lightPosLocal=_GetLightPosLocal();
	// 光影网格，获取边缘线
	{
		vector<FKCW_EffectNode_2DSoftShadow_Edge> edgeList;
		int nPoint=(int)m_polygon.size();
		if(m_rightPenumbraPointID==m_rightUmbraPointID)
		{
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightPenumbraLine[1],m_rightPenumbraLine[2]));
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
		}
		else
		{
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightPenumbraLine[1],m_rightPenumbraLine[2]));
			int index=(m_rightPenumbraPointID+1)%nPoint;
			while(1)
			{
				const CCPoint&p=m_polygon[index];
				const CCPoint&pf=m_polygon[index-1>=0?index-1:index-1+nPoint];
				FKCW_EffectNode_2DSoftShadow_Edge edge;
				edge.m_start=pf;
				edge.m_end=edge.m_start+ccpMult(ccpNormalize(p-pf), m_shadowLength);
				edgeList.push_back(edge);
				if(index==m_rightUmbraPointID)break;
				index=(index+1)%nPoint;
			}
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
		}

		vector<float> brightList;
		int nEdge=(int)edgeList.size();
		for(int i=0;i<nEdge;i++)
		{
			const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
			// 计算在边缘右侧的光线区域
			float PND=FKCW_Base_Utils::PointDisToLine(lightPosLocal, edge.m_start, edge.m_end);
			float area=m_light->getR()-PND;
			brightList.push_back(area/(2*m_light->getR()));
		}
		// 转换边缘线为mesh网格
		for(int i=0;i<nEdge-1;i++)
		{
			const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
			const FKCW_EffectNode_2DSoftShadow_Edge&edgen=edgeList[i+1];
			const CCPoint&p0=edge.m_end;
			const CCPoint&p1=edgen.m_end;
			const CCPoint&p2=edgen.m_start;
			float bright=brightList[i];
			float brightn=brightList[i+1];
			//v0
			Cv2 pos0=Cv2::CCPToV2(p0);
			Cv2 texCoord0=Cv2(0,0);
			Cv4 color0=Cv4(bright,brightn,0,1);
			//v1
			Cv2 pos1=Cv2::CCPToV2(p1);
			Cv2 texCoord1=Cv2(1,0);
			Cv4 color1=Cv4(bright,brightn,0,1);
			//v2
			Cv2 pos2=Cv2::CCPToV2(p2);
			Cv2 texCoord2=Cv2(0,1);
			Cv4 color2=Cv4(bright,brightn,0,1);
			//add v0
			m_mesh->vlist.push_back(pos0);
			m_mesh->texCoordList.push_back(texCoord0);
			m_mesh->colorList.push_back(color0);
			int ID0=(int)m_mesh->vlist.size()-1;
			//add v1
			m_mesh->vlist.push_back(pos1);
			m_mesh->texCoordList.push_back(texCoord1);
			m_mesh->colorList.push_back(color1);
			int ID1=(int)m_mesh->vlist.size()-1;
			//add v2
			m_mesh->vlist.push_back(pos2);
			m_mesh->texCoordList.push_back(texCoord2);
			m_mesh->colorList.push_back(color2);
			int ID2=(int)m_mesh->vlist.size()-1;
			//add IDtri
			m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
		}
	}

	// 左边影子网格
	{
		vector<FKCW_EffectNode_2DSoftShadow_Edge> edgeList;
		int nPoint=(int)m_polygon.size();
		// 获取边缘列表
		if(m_leftPenumbraPointID==m_leftUmbraPointID)
		{
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftPenumbraLine[1],m_leftPenumbraLine[2]));
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
		}
		else
		{
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftPenumbraLine[1],m_leftPenumbraLine[2]));
			int index=(m_leftPenumbraPointID-1>=0?m_leftPenumbraPointID-1:m_leftPenumbraPointID-1+nPoint);
			while(1)
			{
				const CCPoint&p=m_polygon[index];
				const CCPoint&pf=m_polygon[(index+1)%nPoint];
				FKCW_EffectNode_2DSoftShadow_Edge edge;
				edge.m_start=pf;
				edge.m_end=edge.m_start+ccpMult(ccpNormalize(p-pf), m_shadowLength);
				edgeList.push_back(edge);
				if(index==m_leftUmbraPointID)break;
				index=(index-1>=0?index-1:index-1+nPoint);

			}
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
		}
		vector<float> brightList;
		int nEdge=(int)edgeList.size();
		for(int i=0;i<nEdge;i++)
		{
			const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
			// 计算在边缘右侧的光线区域
			float PND=FKCW_Base_Utils::PointDisToLine(lightPosLocal, edge.m_start, edge.m_end);
			float area=m_light->getR()+PND;
			brightList.push_back(area/(2*m_light->getR()));
		}

		// 转换边缘线为mesh网格
		for(int i=0;i<nEdge-1;i++)
		{
			const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
			const FKCW_EffectNode_2DSoftShadow_Edge&edgen=edgeList[i+1];
			const CCPoint&p0=edge.m_end;
			const CCPoint&p1=edgen.m_start;
			const CCPoint&p2=edgen.m_end;
			float bright=brightList[i];
			float brightn=brightList[i+1];
			//v0
			Cv2 pos0=Cv2::CCPToV2(p0);
			Cv2 texCoord0=Cv2(0,0);
			Cv4 color0=Cv4(bright,brightn,0,1);
			//v1
			Cv2 pos1=Cv2::CCPToV2(p1);
			Cv2 texCoord1=Cv2(0,1);
			Cv4 color1=Cv4(bright,brightn,0,1);
			//v2
			Cv2 pos2=Cv2::CCPToV2(p2);
			Cv2 texCoord2=Cv2(1,0);
			Cv4 color2=Cv4(bright,brightn,0,1);
			//add v0
			m_mesh->vlist.push_back(pos0);
			m_mesh->texCoordList.push_back(texCoord0);
			m_mesh->colorList.push_back(color0);
			int ID0=(int)m_mesh->vlist.size()-1;
			//add v1
			m_mesh->vlist.push_back(pos1);
			m_mesh->texCoordList.push_back(texCoord1);
			m_mesh->colorList.push_back(color1);
			int ID1=(int)m_mesh->vlist.size()-1;
			//add v2
			m_mesh->vlist.push_back(pos2);
			m_mesh->texCoordList.push_back(texCoord2);
			m_mesh->colorList.push_back(color2);
			int ID2=(int)m_mesh->vlist.size()-1;
			//add IDtri
			m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
		}
	}

	// 影子 mesh 网格
	{
		if(FKCW_Base_Utils::IsPointEqual(m_intersectPoint, CCPoint(INFINITY,INFINITY), 0))
		{
			// 平行
			vector<FKCW_EffectNode_2DSoftShadow_Edge> edgeList;
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
			CCPoint dir=ccpNormalize(m_rightUmbraLine[2]-m_rightUmbraLine[1]);
			int nPoint=(int)m_polygon.size();
			int index=(m_rightUmbraPointID+1)%nPoint;
			assert(m_rightUmbraPointID!=m_leftUmbraPointID);
			while(1)
			{
				if(index==m_leftUmbraPointID)break;
				const CCPoint&point=m_polygon[index];
				FKCW_EffectNode_2DSoftShadow_Edge edge;
				edge.m_start=point;
				edge.m_end=edge.m_start+ccpMult(dir, m_shadowLength);
				edgeList.push_back(edge);
				index=(index+1)%nPoint;
			}
			edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
			// 转换边缘列表为网格
			int nEdge=(int)edgeList.size();
			for(int i=0;i<nEdge-1;i++)
			{
				const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
				const FKCW_EffectNode_2DSoftShadow_Edge&edgen=edgeList[i+1];
				const CCPoint&p0=edge.m_start;//RU
				const CCPoint&p1=edge.m_end;//LU
				const CCPoint&p2=edgen.m_end;//LD
				const CCPoint&p3=edgen.m_start;//RD
				//v0
				Cv2 pos0=Cv2::CCPToV2(p0);
				Cv2 texCoord0=Cv2(1,1);
				Cv4 color0=Cv4(0,0,0,1);
				//v1
				Cv2 pos1=Cv2::CCPToV2(p1);
				Cv2 texCoord1=Cv2(1,1);
				Cv4 color1=Cv4(0,0,0,1);
				//v2
				Cv2 pos2=Cv2::CCPToV2(p2);
				Cv2 texCoord2=Cv2(1,1);
				Cv4 color2=Cv4(0,0,0,1);
				//v3
				Cv2 pos3=Cv2::CCPToV2(p3);
				Cv2 texCoord3=Cv2(1,1);
				Cv4 color3=Cv4(0,0,0,1);
				//add v0
				m_mesh->vlist.push_back(pos0);
				m_mesh->texCoordList.push_back(texCoord0);
				m_mesh->colorList.push_back(color0);
				int ID0=(int)m_mesh->vlist.size()-1;
				//add v1
				m_mesh->vlist.push_back(pos1);
				m_mesh->texCoordList.push_back(texCoord1);
				m_mesh->colorList.push_back(color1);
				int ID1=(int)m_mesh->vlist.size()-1;
				//add v2
				m_mesh->vlist.push_back(pos2);
				m_mesh->texCoordList.push_back(texCoord2);
				m_mesh->colorList.push_back(color2);
				int ID2=(int)m_mesh->vlist.size()-1;
				//add v3
				m_mesh->vlist.push_back(pos3);
				m_mesh->texCoordList.push_back(texCoord3);
				m_mesh->colorList.push_back(color3);
				int ID3=(int)m_mesh->vlist.size()-1;
				//add IDtri
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID2,ID3));
			}
		}
		else
		{ // 不平行
			// 检查交点P在灯光哪边
			if(ccpDot(m_leftUmbraLine[1]-m_leftUmbraLine[0],m_leftUmbraLine[1]-m_intersectPoint)>0)
			{
				// 交点P在灯光后方
				vector<FKCW_EffectNode_2DSoftShadow_Edge> edgeList;
				edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
				int nPoint=(int)m_polygon.size();
				int index=(m_rightUmbraPointID+1)%nPoint;
				assert(m_rightUmbraPointID!=m_leftUmbraPointID);
				while(1)
				{
					if(index==m_leftUmbraPointID)break;
					const CCPoint&point=m_polygon[index];
					FKCW_EffectNode_2DSoftShadow_Edge edge;
					edge.m_start=point;
					edge.m_end=edge.m_start+ccpMult(ccpNormalize(point-m_intersectPoint), m_shadowLength);
					edgeList.push_back(edge);
					index=(index+1)%nPoint;
				}
				edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
				// 转换边缘列表为网格
				int nEdge=(int)edgeList.size();
				for(int i=0;i<nEdge-1;i++)
				{
					const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
					const FKCW_EffectNode_2DSoftShadow_Edge&edgen=edgeList[i+1];
					const CCPoint&p0=edge.m_start;//RU
					const CCPoint&p1=edge.m_end;//LU
					const CCPoint&p2=edgen.m_end;//LD
					const CCPoint&p3=edgen.m_start;//RD
					//v0
					Cv2 pos0=Cv2::CCPToV2(p0);
					Cv2 texCoord0=Cv2(1,1);
					Cv4 color0=Cv4(0,0,0,1);
					//v1
					Cv2 pos1=Cv2::CCPToV2(p1);
					Cv2 texCoord1=Cv2(1,1);
					Cv4 color1=Cv4(0,0,0,1);
					//v2
					Cv2 pos2=Cv2::CCPToV2(p2);
					Cv2 texCoord2=Cv2(1,1);
					Cv4 color2=Cv4(0,0,0,1);
					//v3
					Cv2 pos3=Cv2::CCPToV2(p3);
					Cv2 texCoord3=Cv2(1,1);
					Cv4 color3=Cv4(0,0,0,1);
					//add v0
					m_mesh->vlist.push_back(pos0);
					m_mesh->texCoordList.push_back(texCoord0);
					m_mesh->colorList.push_back(color0);
					int ID0=(int)m_mesh->vlist.size()-1;
					//add v1
					m_mesh->vlist.push_back(pos1);
					m_mesh->texCoordList.push_back(texCoord1);
					m_mesh->colorList.push_back(color1);
					int ID1=(int)m_mesh->vlist.size()-1;
					//add v2
					m_mesh->vlist.push_back(pos2);
					m_mesh->texCoordList.push_back(texCoord2);
					m_mesh->colorList.push_back(color2);
					int ID2=(int)m_mesh->vlist.size()-1;
					//add v3
					m_mesh->vlist.push_back(pos3);
					m_mesh->texCoordList.push_back(texCoord3);
					m_mesh->colorList.push_back(color3);
					int ID3=(int)m_mesh->vlist.size()-1;
					//add IDtri
					m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
					m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID2,ID3));
				}
			}
			else
			{
				// 交点P在灯光前方
				vector<FKCW_EffectNode_2DSoftShadow_Edge> edgeList;
				edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_rightUmbraLine[1],m_intersectPoint));
				int nPoint=(int)m_polygon.size();
				int index=(m_rightUmbraPointID+1)%nPoint;
				assert(m_rightUmbraPointID!=m_leftUmbraPointID);
				while(1)
				{
					if(index==m_leftUmbraPointID)break;
					const CCPoint&point=m_polygon[index];
					FKCW_EffectNode_2DSoftShadow_Edge edge;
					edge.m_start=point;
					edge.m_end=m_intersectPoint;
					edgeList.push_back(edge);
					index=(index+1)%nPoint;
				}
				edgeList.push_back(FKCW_EffectNode_2DSoftShadow_Edge(m_leftUmbraLine[1],m_intersectPoint));
				// 转换边缘列表为网格
				int nEdge=(int)edgeList.size();
				for(int i=0;i<nEdge-1;i++){
					const FKCW_EffectNode_2DSoftShadow_Edge&edge=edgeList[i];
					const FKCW_EffectNode_2DSoftShadow_Edge&edgen=edgeList[i+1];
					const CCPoint&p0=edge.m_start;
					const CCPoint&p1=edge.m_end;
					const CCPoint&p2=edgen.m_start;
					//v0
					Cv2 pos0=Cv2::CCPToV2(p0);
					Cv2 texCoord0=Cv2(1,1);
					Cv4 color0=Cv4(0,0,0,1);
					//v1
					Cv2 pos1=Cv2::CCPToV2(p1);
					Cv2 texCoord1=Cv2(1,1);
					Cv4 color1=Cv4(0,0,0,1);
					//v2
					Cv2 pos2=Cv2::CCPToV2(p2);
					Cv2 texCoord2=Cv2(1,1);
					Cv4 color2=Cv4(0,0,0,1);
					//add v0
					m_mesh->vlist.push_back(pos0);
					m_mesh->texCoordList.push_back(texCoord0);
					m_mesh->colorList.push_back(color0);
					int ID0=(int)m_mesh->vlist.size()-1;
					//add v1
					m_mesh->vlist.push_back(pos1);
					m_mesh->texCoordList.push_back(texCoord1);
					m_mesh->colorList.push_back(color1);
					int ID1=(int)m_mesh->vlist.size()-1;
					//add v2
					m_mesh->vlist.push_back(pos2);
					m_mesh->texCoordList.push_back(texCoord2);
					m_mesh->colorList.push_back(color2);
					int ID2=(int)m_mesh->vlist.size()-1;
					//add IDtri
					m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				}
			}
		}
	}

	// 计算圈光照空间mesh
	{
		CCPoint intersectOfPenumbraLines=FKCW_Base_Utils::GetIntersectPointOfTwoLine(m_leftPenumbraLine[0], ccpNormalize(m_leftPenumbraLine[1]-m_leftPenumbraLine[0]),
			m_rightPenumbraLine[0], ccpNormalize(m_rightPenumbraLine[1]-m_rightPenumbraLine[0]));

		// m_leftPenumbraLine， m_rightPenumbraLine 和这个三角形之间的空间
		if(m_leftPenumbraPointID==m_rightPenumbraPointID)
		{
		}
		else
		{
			int nPoint=(int)m_polygon.size();
			int index=m_leftPenumbraPointID;
			while(1)
			{
				const CCPoint&p=m_polygon[index];
				const CCPoint&pn=m_polygon[(index+1)%nPoint];
				const CCPoint&p0=intersectOfPenumbraLines;
				const CCPoint&p1=pn;
				const CCPoint&p2=p;
				//v0
				Cv2 pos0=Cv2::CCPToV2(p0);
				Cv2 texCoord0=Cv2(0,0);
				Cv4 color0=Cv4(1,1,0,1);
				//v1
				Cv2 pos1=Cv2::CCPToV2(p1);
				Cv2 texCoord1=Cv2(0,0);
				Cv4 color1=Cv4(1,1,0,1);
				//v2
				Cv2 pos2=Cv2::CCPToV2(p2);
				Cv2 texCoord2=Cv2(0,0);
				Cv4 color2=Cv4(1,1,0,1);
				//add v0
				m_mesh->vlist.push_back(pos0);
				m_mesh->texCoordList.push_back(texCoord0);
				m_mesh->colorList.push_back(color0);
				int ID0=(int)m_mesh->vlist.size()-1;
				//add v1
				m_mesh->vlist.push_back(pos1);
				m_mesh->texCoordList.push_back(texCoord1);
				m_mesh->colorList.push_back(color1);
				int ID1=(int)m_mesh->vlist.size()-1;
				//add v2
				m_mesh->vlist.push_back(pos2);
				m_mesh->texCoordList.push_back(texCoord2);
				m_mesh->colorList.push_back(color2);
				int ID2=(int)m_mesh->vlist.size()-1;
				//add IDtri
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				index=(index+1)%nPoint;
				if(index==m_rightPenumbraPointID)break;
			}
		}
		// 下扇形
		{
			const float dA=45;
			const CCPoint startDir=ccpNormalize(m_leftPenumbraLine[2]-intersectOfPenumbraLines);
			const CCPoint endDir=ccpNormalize(m_rightPenumbraLine[0]-m_rightPenumbraLine[1]);
			CCPoint dir=startDir;
			CCPoint dirfoe;
			while(1)
			{
				dirfoe=dir;
				dir=FKCW_Base_Utils::RotateVector2(dir, dA);
				bool isFinishCurLoopAndStop=false;
				if(ccpCross(dir, endDir)<0)
				{
					dir=endDir;
					isFinishCurLoopAndStop=true;
				}

				// 计算 dirfoe 和 dir 之间的三角形
				const CCPoint&p0=intersectOfPenumbraLines;
				const CCPoint&p1=intersectOfPenumbraLines+ccpMult(dirfoe, m_shadowLength);
				const CCPoint&p2=intersectOfPenumbraLines+ccpMult(dir, m_shadowLength);
				//v0
				Cv2 pos0=Cv2::CCPToV2(p0);
				Cv2 texCoord0=Cv2(0,0);
				Cv4 color0=Cv4(1,1,0,1);
				//v1
				Cv2 pos1=Cv2::CCPToV2(p1);
				Cv2 texCoord1=Cv2(0,0);
				Cv4 color1=Cv4(1,1,0,1);
				//v2
				Cv2 pos2=Cv2::CCPToV2(p2);
				Cv2 texCoord2=Cv2(0,0);
				Cv4 color2=Cv4(1,1,0,1);
				//add v0
				m_mesh->vlist.push_back(pos0);
				m_mesh->texCoordList.push_back(texCoord0);
				m_mesh->colorList.push_back(color0);
				int ID0=(int)m_mesh->vlist.size()-1;
				//add v1
				m_mesh->vlist.push_back(pos1);
				m_mesh->texCoordList.push_back(texCoord1);
				m_mesh->colorList.push_back(color1);
				int ID1=(int)m_mesh->vlist.size()-1;
				//add v2
				m_mesh->vlist.push_back(pos2);
				m_mesh->texCoordList.push_back(texCoord2);
				m_mesh->colorList.push_back(color2);
				int ID2=(int)m_mesh->vlist.size()-1;
				//add IDtri
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				//stop
				if(isFinishCurLoopAndStop)
					break;
			}
		}
		// 上扇形
		{
			const float dA=45;
			const CCPoint startDir=ccpNormalize(m_leftPenumbraLine[0]-m_leftPenumbraLine[1]);
			const CCPoint endDir=ccpNormalize(m_rightPenumbraLine[2]-m_rightPenumbraLine[1]);
			CCPoint dir=startDir;
			CCPoint dirfoe;
			while(1)
			{
				dirfoe=dir;
				dir=FKCW_Base_Utils::RotateVector2(dir, dA);
				bool isFinishCurLoopAndStop=false;
				if(ccpCross(dir, endDir)<0)
				{
					dir=endDir;
					isFinishCurLoopAndStop=true;
				}
				// 计算 dirfoe 和 dir 之间的三角形
				const CCPoint&p0=intersectOfPenumbraLines;
				const CCPoint&p1=intersectOfPenumbraLines+ccpMult(dirfoe, m_shadowLength);
				const CCPoint&p2=intersectOfPenumbraLines+ccpMult(dir, m_shadowLength);
				//v0
				Cv2 pos0=Cv2::CCPToV2(p0);
				Cv2 texCoord0=Cv2(0,0);
				Cv4 color0=Cv4(1,1,0,1);
				//v1
				Cv2 pos1=Cv2::CCPToV2(p1);
				Cv2 texCoord1=Cv2(0,0);
				Cv4 color1=Cv4(1,1,0,1);
				//v2
				Cv2 pos2=Cv2::CCPToV2(p2);
				Cv2 texCoord2=Cv2(0,0);
				Cv4 color2=Cv4(1,1,0,1);
				//add v0
				m_mesh->vlist.push_back(pos0);
				m_mesh->texCoordList.push_back(texCoord0);
				m_mesh->colorList.push_back(color0);
				int ID0=(int)m_mesh->vlist.size()-1;
				//add v1
				m_mesh->vlist.push_back(pos1);
				m_mesh->texCoordList.push_back(texCoord1);
				m_mesh->colorList.push_back(color1);
				int ID1=(int)m_mesh->vlist.size()-1;
				//add v2
				m_mesh->vlist.push_back(pos2);
				m_mesh->texCoordList.push_back(texCoord2);
				m_mesh->colorList.push_back(color2);
				int ID2=(int)m_mesh->vlist.size()-1;
				//add IDtri
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				//stop
				if(isFinishCurLoopAndStop)
					break;
			}
		}
		// 右扇形
		{
			const float dA=45;
			const CCPoint startDir=ccpNormalize(m_rightPenumbraLine[0]-m_rightPenumbraLine[1]);
			const CCPoint endDir=ccpNormalize(m_leftPenumbraLine[0]-m_leftPenumbraLine[1]);
			CCPoint dir=startDir;
			CCPoint dirfoe;
			while(1)
			{
				dirfoe=dir;
				dir=FKCW_Base_Utils::RotateVector2(dir, dA);
				bool isFinishCurLoopAndStop=false;
				if(ccpCross(dir, endDir)<0)
				{
						dir=endDir;
						isFinishCurLoopAndStop=true;
				}
				// 计算 dirfoe 和 dir 之间的三角形
				const CCPoint&p0=intersectOfPenumbraLines;
				const CCPoint&p1=intersectOfPenumbraLines+ccpMult(dirfoe, m_shadowLength);
				const CCPoint&p2=intersectOfPenumbraLines+ccpMult(dir, m_shadowLength);
				//v0
				Cv2 pos0=Cv2::CCPToV2(p0);
				Cv2 texCoord0=Cv2(0,0);
				Cv4 color0=Cv4(1,1,0,1);
				//v1
				Cv2 pos1=Cv2::CCPToV2(p1);
				Cv2 texCoord1=Cv2(0,0);
				Cv4 color1=Cv4(1,1,0,1);
				//v2
				Cv2 pos2=Cv2::CCPToV2(p2);
				Cv2 texCoord2=Cv2(0,0);
				Cv4 color2=Cv4(1,1,0,1);
				//add v0
				m_mesh->vlist.push_back(pos0);
				m_mesh->texCoordList.push_back(texCoord0);
				m_mesh->colorList.push_back(color0);
				int ID0=(int)m_mesh->vlist.size()-1;
				//add v1
				m_mesh->vlist.push_back(pos1);
				m_mesh->texCoordList.push_back(texCoord1);
				m_mesh->colorList.push_back(color1);
				int ID1=(int)m_mesh->vlist.size()-1;
				//add v2
				m_mesh->vlist.push_back(pos2);
				m_mesh->texCoordList.push_back(texCoord2);
				m_mesh->colorList.push_back(color2);
				int ID2=(int)m_mesh->vlist.size()-1;
				//add IDtri
				m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
				//stop?
				if(isFinishCurLoopAndStop)
					break;
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_Effect_2DSoftShadow_ShadowObj::_Submit(GLenum usage)
{
	// 提交Mesh
	m_indexVBO->SubmitPos(m_mesh->vlist, usage);
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, usage);
	m_indexVBO->SubmitColor(m_mesh->colorList, usage);
	m_indexVBO->SubmitIndex(m_mesh->IDtriList, usage);
}
//---------------------------------------------
bool C2DSoftShadowRoot::init()
{
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	m_shadowRT=CCRenderTexture::create(winSize.width, winSize.height);
	m_shadowRT->retain();
	m_shadowRT->setPosition(ccp(winSize.width/2,winSize.height/2));
	// 创建设置shader Program
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_2DSoftShadowRenderShadowFragShader );
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		program->attachUniform("u_objCount");
		program->attachUniform("u_shadowDarkness");
		m_program=program;
		m_program->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	// 更改shader和BlendFunc
	m_shadowRT->getSprite()->setShaderProgram(m_program);
	ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
	m_shadowRT->getSprite()->setBlendFunc(blendFunc);
	return true;
}
//---------------------------------------------
void C2DSoftShadowRoot::setLight(FKCW_EffectNode_2DSoftShadow_LightNode*light)
{
	assert(light);
	if(m_light==NULL)
	{
		m_light=light;
		addChild(m_light);
	}
	else
	{
		m_light->removeFromParentAndCleanup(true);
		m_light=light;
		addChild(m_light);
	}
}
//---------------------------------------------
void C2DSoftShadowRoot::addObj(FKCW_Effect_2DSoftShadow_ShadowObj*obj)
{
	assert(obj);
	m_objList.push_back(obj);
	addChild(obj);
}
//---------------------------------------------
void C2DSoftShadowRoot::visit()
{
	// push矩阵
	kmGLPushMatrix();
	// 矩阵转换
	this->transform();
	{
		// 设置obj的 isDrawDebug
		int nObj=(int)m_objList.size();
		for(int i=0;i<nObj;i++)
		{
			FKCW_Effect_2DSoftShadow_ShadowObj*obj=m_objList[i];
			obj->setIsDrawDebug(m_isDrawDebug);
		}
		// 设置灯光 isDrawDebug
		m_light->setIsDrawDebug(m_isDrawDebug);
		// 渲染到shadowRT
		m_shadowRT->beginWithClear(0, 0, 0, 1);
		{
			int nObj=(int)m_objList.size();
			float oneDivObjCount=1.0/nObj;
			for(int i=0;i<nObj;i++)
			{
				FKCW_Effect_2DSoftShadow_ShadowObj*obj=m_objList[i];
				obj->setOneDivObjCount(oneDivObjCount);
				bool isDrawDebugOld=obj->getIsDrawDebug();
				obj->setIsDrawDebug(false);
				obj->visit();
				obj->setIsDrawDebug(isDrawDebugOld);
			}
		}
		m_shadowRT->end();

		FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)m_shadowRT->getSprite()->getShaderProgram();
		program->use();
		program->passUnifoValue1i("u_objCount", nObj);
		program->passUnifoValue1f("u_shadowDarkness", m_shadowDarkness);
		// 绘制shadowRT
		m_shadowRT->visit();
		// 绘制Obj的DebugDraw
		for(int i=0;i<nObj;i++)
		{
			FKCW_Effect_2DSoftShadow_ShadowObj*obj=m_objList[i];
			bool isDrawNonDebugOld=obj->getIsDrawNonDebug();
			obj->setIsDrawNonDebug(false);
			obj->visit();
			obj->setIsDrawNonDebug(isDrawNonDebugOld);
		}
		// 绘制灯光debugDraw
		m_light->visit();
	}
	// pop矩阵
	kmGLPopMatrix();
}
//---------------------------------------------