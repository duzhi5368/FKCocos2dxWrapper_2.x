//--------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_BreakSprite.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//--------------------------------------------------------------------
#pragma warning( disable:4244 )
#pragma warning( disable:4305 )
//--------------------------------------------------------------------
const char* s_BreakFragShader = "																		\n\
				#ifdef GL_ES																			\n\
					precision mediump float;															\n\
				#endif																					\n\
																										\n\
				varying vec4 v_fragmentColor;															\n\
				varying vec2 v_texCoord;																\n\
				uniform sampler2D CC_Texture0;															\n\
																										\n\
				void main()																				\n\
				{																						\n\
					if(v_texCoord.x<0.0||v_texCoord.x>1.0||v_texCoord.y<0.0||v_texCoord.y>1.0)			\n\
					{																					\n\
						gl_FragColor=vec4(0.0,0.0,0.0,0.0);												\n\
					}else{																				\n\
						gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);			\n\
					}																					\n\
				}";
const char* s_BreakVertShader = "																		\n\
				attribute vec4 a_position;																\n\
				attribute vec2 a_texCoord;																\n\
				attribute vec4 a_color;																	\n\
																										\n\
				varying vec4 v_fragmentColor;															\n\
				varying vec2 v_texCoord;																\n\
																										\n\
																										\n\
				void main()																				\n\
				{																						\n\
					gl_Position = CC_MVPMatrix * a_position;											\n\
					v_fragmentColor = a_color;															\n\
					v_texCoord = a_texCoord;															\n\
				}";
//--------------------------------------------------------------------
// FKCW_RenderTriangle
//--------------------------------------------------------------------
void FKCW_RenderTriangle::setPositions(const Cv2&pos0,const Cv2&pos1,const Cv2&pos2)
{
	m_pos[0]=pos0;
	m_pos[1]=pos1;
	m_pos[2]=pos2;
}
//--------------------------------------------------------------------
void FKCW_RenderTriangle::setColors(const Cv4&color0,const Cv4&color1,const Cv4&color2)
{
	m_color[0]=color0;
	m_color[1]=color1;
	m_color[2]=color2;
}
//--------------------------------------------------------------------
void FKCW_RenderTriangle::setTexCoords(const Cv2&texCoord0,const Cv2&texCoord1,const Cv2&texCoord2)
{
	m_texCoord[0]=texCoord0;
	m_texCoord[1]=texCoord1;
	m_texCoord[2]=texCoord2;
}
//--------------------------------------------------------------------
FKCW_RenderTriangle::FKCW_RenderTriangle()
{
	m_fallOffDelay=0;
	m_vy=0;
	setPositions(Cv2(0,0), Cv2(0,0), Cv2(0,0));
	setTexCoords(Cv2(0,0), Cv2(0,0), Cv2(0,0));
	setColors(Cv4(1,1,1,1), Cv4(1,1,1,1), Cv4(1,1,1,1));
}
//--------------------------------------------------------------------
// FKCW_EffectNode_BreakSprite
//--------------------------------------------------------------------
FKCW_EffectNode_BreakSprite::FKCW_EffectNode_BreakSprite()
{
	m_mesh=NULL;
	m_meshWire=NULL;
	m_nRow=0;
	m_nCol=0;
	m_indexVBO=NULL;
	m_indexVBO_wire=NULL;
	m_program=NULL;

	m_gridSideLenMax=80;
	m_gridSideLenMin=10;
	m_state=eBreakSpriteState_Well;
	m_isDrawDebug=false;
}
//--------------------------------------------------------------------
FKCW_EffectNode_BreakSprite::~FKCW_EffectNode_BreakSprite()
{
	if(m_mesh)m_mesh->release();
	if(m_meshWire)m_meshWire->release();
	if(m_indexVBO)m_indexVBO->release();
	if(m_indexVBO_wire)m_indexVBO_wire->release();
	if(m_program)m_program->release();

}
//--------------------------------------------------------------------
bool FKCW_EffectNode_BreakSprite::init(const string&texFileName)
{
	// 初始化当前精灵
	this->CCSprite::initWithFile(texFileName.c_str());
	// 创建Mesh
	m_mesh=new FKCW_EffectNode_Mesh();
	m_mesh->autorelease();
	m_mesh->retain();
	// 创建线状Mesh
	m_meshWire=new FKCW_EffectNode_MeshWire();
	m_meshWire->autorelease();
	m_meshWire->retain();
	// 创建IndexVBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 创建线状IndexVBO
	m_indexVBO_wire=new FKCW_EffectNode_IndexVBO();
	m_indexVBO_wire->autorelease();
	m_indexVBO_wire->retain();
	// 设置program
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(s_BreakVertShader, s_BreakFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		m_program=program;
		m_program->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	return true ;
}
//--------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::draw()
{
	if(m_state==eBreakSpriteState_Well)
	{
		this->CCSprite::draw();
	}
	else
	{
		// 更改shader
		ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
		ccGLEnable(m_eGLServerState);

		this->setShaderProgram(m_program);
		FKCW_EffectNode_GLProgramWithUnifos*program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
		program->use();
		program->setUniformsForBuiltins();

		bool isAttribPositionOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayPosition();
		bool isAttribColorOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayColor();
		bool isAttribTexCoordOn=FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayTexCoord();
		FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(true);
		FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(true);
		FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(true);
		// 绑定纹理
		ccGLBindTexture2D( this->getTexture()->getName());
		// 绘制IndexVBO
		m_indexVBO->SetPointerPosition();
		m_indexVBO->SetPointerTexCoord();
		m_indexVBO->SetPointerColor();
		m_indexVBO->Draw(GL_TRIANGLES);
		// 绘制IndexVBOWire
		glLineWidth(1);
		m_indexVBO_wire->SetPointerPosition();
		m_indexVBO_wire->SetPointerTexCoord();
		m_indexVBO_wire->SetPointerColor();
		m_indexVBO_wire->Draw(GL_LINES);
		// 解除纹理绑定
		ccGLBindTexture2D(0);
		
		FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
		FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
		FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
	}
	if(m_isDrawDebug)
	{
		drawWire();
	}
}
//--------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::drawWire()
{
	int nTri=(int)m_triList.size();
	for(int i=0;i<nTri;i++)
	{
		const FKCW_RenderTriangle&tri=m_triList[i];
		CCPoint vertices[3]={CCPoint(tri.m_pos[0].x(),tri.m_pos[0].y()),
			CCPoint(tri.m_pos[1].x(),tri.m_pos[1].y()),
			CCPoint(tri.m_pos[2].x(),tri.m_pos[2].y())};
		ccDrawPoly(vertices, 3, true);
	}
}
//--------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::reSet()
{
	m_state=eBreakSpriteState_Well;
	this->stopAllActions();
	this->_Clear();
}
//--------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::_Clear()
{
	m_nRow=m_nCol=0;
	m_nRowPoint=m_nColPoint=0;
	m_pointList.clear();
	m_IDquadList.clear();
	m_triList.clear();
	m_mesh->clear();
	m_meshWire->clear();
}
//--------------------------------------------------------------------
int FKCW_EffectNode_BreakSprite::_GetPointInRangeEqualsToTargetPoint(const CCPoint&targetPoint,const vector<CCPoint>&pointList,int imin,int imax,float eps)
{
	int nPoint=(int)pointList.size();
	if(imin>=nPoint)
	{
		return -1;
	}
	assert(imin>=0&&imin<nPoint);
	assert(imax>=0&&imax<nPoint);
	for(int i=imin;i<=imax;i++)
	{
		const CCPoint&point=pointList[i];
		if(FKCW_Base_Utils::IsPointEqual(point, targetPoint,eps))
		{
			return i;
		}
	}
	return -1;
}
//--------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::doCrack(const CCPoint&touchPoint)
{
	m_state=eBreakSpriteState_Crack;
	// 清理
	this->_Clear();
	// 获取包围盒大小
	CCSize contentSize=this->getContentSize();
	// touchPoint是在父空间内的坐标，将其转换为本地空间坐标
	CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
	CCPoint touchPoint_localSpace=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform);
	// 计算PointList
	for(float y=contentSize.height+m_gridSideLenMax;y>-m_gridSideLenMax;y-=m_gridSideLenMax)
	{
		m_nRowPoint++;
		m_nColPoint=0;
		for(float x=-m_gridSideLenMax;x<=contentSize.width+m_gridSideLenMax;x+=m_gridSideLenMax)
		{
			m_nColPoint++;
			CCPoint point(x,y);
			m_pointList.push_back(point);
		}
	}
	// 计算行列数
	m_nRow=m_nRowPoint-1;
	m_nCol=m_nColPoint-1;
	// 构建QuadList
	m_IDquadList.reserve(m_nRow*m_nCol);
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			CIDquad IDquad;
			IDquad.m_vIDLU=i*m_nColPoint+j;//m_pointList(i,j)
			IDquad.m_vIDRU=IDquad.m_vIDLU+1;
			IDquad.m_vIDLD=IDquad.m_vIDLU+m_nColPoint;
			IDquad.m_vIDRD=IDquad.m_vIDLD+1;
			m_IDquadList.push_back(IDquad);
		}
	}
	// 进阶分割
	const float k=0.5;
	while(1)
	{
		bool isOneOrMoreDivided=false;
		int nIDquad=(int)m_IDquadList.size();
		for(int i=0;i<nIDquad;i++)
		{
			CIDquad&IDquad=m_IDquadList[i];
			if(IDquad.m_isRemoved)continue;
			const int IDLU=IDquad.m_vIDLU;
			const int IDLD=IDquad.m_vIDLD;
			const int IDRU=IDquad.m_vIDRU;
			const int IDRD=IDquad.m_vIDRD;
			CCPoint posLU=m_pointList[IDLU];
			CCPoint posLD=m_pointList[IDLD];
			CCPoint posRU=m_pointList[IDRU];
			CCPoint posRD=m_pointList[IDRD];
			CCPoint posCenter=ccpMidpoint(posLU, posRD);
			// 判断Quad是否需要分割
			float sideLenx=posRU.x-posLU.x;
			float sideLeny=posLU.y-posLD.y;
			assert(sideLenx==sideLeny);
			float sideLen=sideLenx;
			float disQuadToTouch=ccpDistance(touchPoint_localSpace, posCenter);
			if(sideLen/disQuadToTouch>k&&sideLen/2>m_gridSideLenMin)
			{
				isOneOrMoreDivided=true;
				// 已经分割，所以改变标示
				IDquad.m_isRemoved=true;
				// 添加新点
				int startSearchIndex=m_nRowPoint*m_nColPoint;// 一个优化。因为我们可以明确结果一定不会在第一个m_nRowPoint*m_nColPoint的元素中。
				//posL
				int IDL;
				CCPoint posL=ccpMidpoint(posLU, posLD);
				{
					// 首先去找该点，若找不到，添加。
					int ID=_GetPointInRangeEqualsToTargetPoint(posL,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
					if(ID==-1)
					{
						m_pointList.push_back(posL);
						IDL=(int)m_pointList.size()-1;
					}
					else
					{
						IDL=ID;
					}
				}
				//posU
				int IDU;
				CCPoint posU=ccpMidpoint(posLU, posRU);
				{
					// 首先去找该点，若找不到，添加。
					int ID=_GetPointInRangeEqualsToTargetPoint(posU,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
					if(ID==-1)
					{
						m_pointList.push_back(posU);
						IDU=(int)m_pointList.size()-1;
					}
					else
					{
						IDU=ID;
					}
				}
				//posR
				int IDR;
				CCPoint posR=ccpMidpoint(posRU, posRD);
				{
					// 首先去找该点，若找不到，添加。
					int ID=-1;// 因为将从左向右进行迭代，所以posR一定不在这个PointList中，所以返回结果一定为-1.
					if(ID==-1)
					{
						m_pointList.push_back(posR);
						IDR=(int)m_pointList.size()-1;
					}
					else
					{
						IDR=ID;
					}
				}
				//posD
				int IDD;
				CCPoint posD=ccpMidpoint(posLD, posRD);
				{
					// 首先去找该点，若找不到，添加。
					int ID=-1;// 因为将从上向下进行迭代，所以posR一定不在这个PointList中，所以返回结果一定为-1.
					if(ID==-1)
					{
						m_pointList.push_back(posD);
						IDD=(int)m_pointList.size()-1;
					}
					else
					{
						IDD=ID;
					}
				}
				//posC
				int IDC;
				CCPoint posC=ccpMidpoint(posLU, posRD);
				{
					int ID=-1;// 因为 posC 不被IDQuads共享，所以他必然不在pointList中
					if(ID==-1)
					{
						m_pointList.push_back(posC);
						IDC=(int)m_pointList.size()-1;
					}
					else
					{
						IDD=ID;
					}
				}
				// 分割 IDquad 到四个更小的 IDquad中
				CIDquad IDquadLU,IDquadLD,IDquadRU,IDquadRD;
				//IDquadLU
				IDquadLU.m_vIDLU=IDquad.m_vIDLU;
				IDquadLU.m_vIDLD=IDL;
				IDquadLU.m_vIDRU=IDU;
				IDquadLU.m_vIDRD=IDC;
				//IDquadLD
				IDquadLD.m_vIDLU=IDL;
				IDquadLD.m_vIDLD=IDquad.m_vIDLD;
				IDquadLD.m_vIDRU=IDC;
				IDquadLD.m_vIDRD=IDD;
				//IDquadRU
				IDquadRU.m_vIDLU=IDU;
				IDquadRU.m_vIDLD=IDC;
				IDquadRU.m_vIDRU=IDquad.m_vIDRU;
				IDquadRU.m_vIDRD=IDR;
				//IDquadRD
				IDquadRD.m_vIDLU=IDC;
				IDquadRD.m_vIDLD=IDD;
				IDquadRD.m_vIDRU=IDR;
				IDquadRD.m_vIDRD=IDquad.m_vIDRD;
				// 添加这四个小IDQuad到IDquadList中
				m_IDquadList.push_back(IDquadLU);
				m_IDquadList.push_back(IDquadLD);
				m_IDquadList.push_back(IDquadRU);
				m_IDquadList.push_back(IDquadRD);
			}
		}
		if(isOneOrMoreDivided==false)
			break;
	}
	// 为每个IDquad计算vIDL,vIDR,vIDU,vIDD
	{
		int nIDquad=(int)m_IDquadList.size();
		for(int i=0;i<nIDquad;i++){
			CIDquad&IDquad=m_IDquadList[i];
			if(IDquad.m_isRemoved)continue;
			// 计算IDquad的vIDL,vIDR,vIDU,vIDD
			// 计算IDquad的中心点
			const int IDLU=IDquad.m_vIDLU;
			const int IDLD=IDquad.m_vIDLD;
			const int IDRU=IDquad.m_vIDRU;
			const int IDRD=IDquad.m_vIDRD;
			CCPoint posLU=m_pointList[IDLU];
			CCPoint posLD=m_pointList[IDLD];
			CCPoint posRU=m_pointList[IDRU];
			CCPoint posRD=m_pointList[IDRD];
			// 计算IDquad的 posL,posR,posU,posD
			CCPoint posL=ccpMidpoint(posLU, posLD);
			CCPoint posR=ccpMidpoint(posRU, posRD);
			CCPoint posU=ccpMidpoint(posLU, posRU);
			CCPoint posD=ccpMidpoint(posLD, posRD);
			// 检查PointList中是否存在posL,若存在，则m_vIDL=该ID。否则保持-1
			// 同理：m_vIDR， m_vIDU， m_vIDD
			int startSearchIndex=m_nRowPoint*m_nColPoint;//a optimization, because we know the result must NOT in the first m_nRowPoint*m_nColPoint elements.
			int IDL=_GetPointInRangeEqualsToTargetPoint(posL,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
			int IDR=_GetPointInRangeEqualsToTargetPoint(posR,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
			int IDU=_GetPointInRangeEqualsToTargetPoint(posU,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
			int IDD=_GetPointInRangeEqualsToTargetPoint(posD,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
			IDquad.m_vIDL=IDL;
			IDquad.m_vIDR=IDR;
			IDquad.m_vIDU=IDU;
			IDquad.m_vIDD=IDD;
		}
	}
	// 随机偏移每个点
	{
		vector<vector<int> > findIDquadIDsByPointID;
		findIDquadIDsByPointID.resize((int)m_pointList.size());
		int nIDquad=(int)m_IDquadList.size();
		for(int i=0;i<nIDquad;i++)
		{
			const CIDquad&IDquad=m_IDquadList[i];
			if(IDquad.m_isRemoved)continue;
			const int IDquadID=i;
			const int IDLU=IDquad.m_vIDLU;
			const int IDLD=IDquad.m_vIDLD;
			const int IDRU=IDquad.m_vIDRU;
			const int IDRD=IDquad.m_vIDRD;
			findIDquadIDsByPointID[IDLU].push_back(IDquadID);
			findIDquadIDsByPointID[IDLD].push_back(IDquadID);
			findIDquadIDsByPointID[IDRU].push_back(IDquadID);
			findIDquadIDsByPointID[IDRD].push_back(IDquadID);
		}
		vector<CCPoint> pointOffsetList;
		int nPoint=(int)m_pointList.size();
		for(int i=0;i<nPoint;i++)
		{
			// 找到所有使用这些点的IDQuads
			const vector<int>&IDquadIDList=findIDquadIDsByPointID[i];
			// 找到IDquadIDList中的最小IDQuad
			int nIDquad=(int)IDquadIDList.size();
			int minIDquadID=-1;
			float minSideLen=INFINITY;
			for(int j=0;j<nIDquad;j++)
			{
				const int IDquadID=IDquadIDList[j];
				const CIDquad&IDquad=m_IDquadList[IDquadID];
				assert(IDquad.m_isRemoved==false);
				const int IDLU=IDquad.m_vIDLU;
				const int IDLD=IDquad.m_vIDLD;
				const int IDRU=IDquad.m_vIDRU;
				const int IDRD=IDquad.m_vIDRD;
				CCPoint posLU=m_pointList[IDLU];
				CCPoint posLD=m_pointList[IDLD];
				CCPoint posRU=m_pointList[IDRU];
				CCPoint posRD=m_pointList[IDRD];
				float sideLenx=posRU.x-posLU.x;
				float sideLeny=posLU.y-posLD.y;
				assert(sideLenx==sideLeny);
				float sideLen=sideLenx;
				if(sideLen<minSideLen)
				{
					minSideLen=sideLen;
					minIDquadID=IDquadID;
				}
			}
			float offsetR=minSideLen/4;
			float A=rand()%360;
			float dx=offsetR*cosf(A*M_PI/180);
			float dy=offsetR*sinf(A*M_PI/180);
			pointOffsetList.push_back(ccp(dx,dy));
		}

		// 为每个点执行偏移操作
		for(int i=0;i<nPoint;i++)
		{
			CCPoint&point=m_pointList[i];
			const CCPoint&offset=pointOffsetList[i];
			point=point+offset;
		}

	}
	// 生成三角形列表
	int nIDquad=(int)m_IDquadList.size();
	for(int i=0;i<nIDquad;i++)
	{
		const CIDquad&IDquad=m_IDquadList[i];
		if(IDquad.m_isRemoved)continue;
		const int IDLU=IDquad.m_vIDLU;
		const int IDLD=IDquad.m_vIDLD;
		const int IDRU=IDquad.m_vIDRU;
		const int IDRD=IDquad.m_vIDRD;
		CCPoint posLU=m_pointList[IDLU];
		CCPoint posLD=m_pointList[IDLD];
		CCPoint posRU=m_pointList[IDRU];
		CCPoint posRD=m_pointList[IDRD];
		CCPoint center=ccpMidpoint(posLU, posRD);
		const int IDL=IDquad.m_vIDL;
		const int IDR=IDquad.m_vIDR;
		const int IDU=IDquad.m_vIDU;
		const int IDD=IDquad.m_vIDD;
		if(IDL==-1&&IDR==-1&&IDU==-1&&IDD==-1)
		{ // 若没有邻居被分割，添加两个三角形
			FKCW_RenderTriangle tri0,tri1;
			tri0.setPositions(Cv2::CCPToV2(m_pointList[IDLU]) , Cv2::CCPToV2(m_pointList[IDLD]), Cv2::CCPToV2(m_pointList[IDRD]));
			tri1.setPositions(Cv2::CCPToV2(m_pointList[IDLU]) , Cv2::CCPToV2(m_pointList[IDRD]), Cv2::CCPToV2(m_pointList[IDRU]));
			m_triList.push_back(tri0);
			m_triList.push_back(tri1);
		}
		else
		{// 若被一个或更多邻居分割，则添加中心点到 pointList 中
			m_pointList.push_back(center);
			int IDC=(int)m_pointList.size()-1;
			// 左侧部分
			if(IDL!=-1)
			{
				// 左边被分割为两部分
				FKCW_RenderTriangle tri0,tri1;
				tri0.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDLU]), Cv2::CCPToV2(m_pointList[IDL]));
				tri1.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDL]), Cv2::CCPToV2(m_pointList[IDLD]));
				m_triList.push_back(tri0);
				m_triList.push_back(tri1);
			}
			else
			{
				// 左侧组成一个三角形
				FKCW_RenderTriangle tri;
				tri.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDLU]), Cv2::CCPToV2(m_pointList[IDLD]));
				m_triList.push_back(tri);
			}
			// 右侧
			if(IDR!=-1)
			{
				// 右边被分割为两部分
				FKCW_RenderTriangle tri0,tri1;
				tri0.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDRD]), Cv2::CCPToV2(m_pointList[IDR]));
				tri1.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDR]), Cv2::CCPToV2(m_pointList[IDRU]));
				m_triList.push_back(tri0);
				m_triList.push_back(tri1);
			}
			else
			{
				// 右侧组成一个三角形
				FKCW_RenderTriangle tri;
				tri.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDRD]), Cv2::CCPToV2(m_pointList[IDRU]));
				m_triList.push_back(tri);
			}
			// 上部
			if(IDU!=-1)
			{
				// 上部被分割为两部分
				FKCW_RenderTriangle tri0,tri1;
				tri0.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDU]), Cv2::CCPToV2(m_pointList[IDLU]));
				tri1.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDRU]), Cv2::CCPToV2(m_pointList[IDU]));
				m_triList.push_back(tri0);
				m_triList.push_back(tri1);

			}
			else
			{
				// 上部组成一个三角形
				FKCW_RenderTriangle tri;
				tri.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDRU]), Cv2::CCPToV2(m_pointList[IDLU]));
				m_triList.push_back(tri);
			}
			// 下侧
			if(IDD!=-1){
				// 下部被分割为两部分
				FKCW_RenderTriangle tri0,tri1;
				tri0.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDLD]), Cv2::CCPToV2(m_pointList[IDD]));
				tri1.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDD]), Cv2::CCPToV2(m_pointList[IDRD]));
				m_triList.push_back(tri0);
				m_triList.push_back(tri1);
			}
			else
			{
				// 下部组成一个三角形
				FKCW_RenderTriangle tri;
				tri.setPositions(Cv2::CCPToV2(m_pointList[IDC]), Cv2::CCPToV2(m_pointList[IDLD]), Cv2::CCPToV2(m_pointList[IDRD]));
				m_triList.push_back(tri);
			}
		}
	}

	// 将三角形全部调小
	{
		const float dOffset=1.0;
		int nTri=(int)m_triList.size();
		for(int i=0;i<nTri;i++)
		{
			FKCW_RenderTriangle&tri=m_triList[i];
			CCPoint p0=Cv2::V2ToCCP(tri.m_pos[0]);
			CCPoint p1=Cv2::V2ToCCP(tri.m_pos[1]);
			CCPoint p2=Cv2::V2ToCCP(tri.m_pos[2]);
			vector<CCPoint> points=FKCW_Base_Utils::MakeTriangleSmaller(p0, p1, p2, dOffset);
			tri.setPositions(Cv2::CCPToV2(points[0]), Cv2::CCPToV2(points[1]), Cv2::CCPToV2(points[2]));
		}
	}
	// 为每个三角面的顶点计算其UV
	{
		CCSize contentSize=this->getContentSize();
		int nTri=(int)m_triList.size();
		for(int i=0;i<nTri;i++)
		{
			FKCW_RenderTriangle&tri=m_triList[i];
			const Cv2&v0=tri.m_pos[0];
			const Cv2&v1=tri.m_pos[1];
			const Cv2&v2=tri.m_pos[2];
			Cv2&texCoord0=tri.m_texCoord[0];
			Cv2&texCoord1=tri.m_texCoord[1];
			Cv2&texCoord2=tri.m_texCoord[2];
			texCoord0.init(v0.x()/contentSize.width, 1-v0.y()/contentSize.height);
			texCoord1.init(v1.x()/contentSize.width, 1-v1.y()/contentSize.height);
			texCoord2.init(v2.x()/contentSize.width, 1-v2.y()/contentSize.height);
		}
	}

	// 根据三角形列表计算网格mesh
	int nTri=(int)m_triList.size();
	for(int i=0;i<nTri;i++)
	{
		const FKCW_RenderTriangle&tri=m_triList[i];
		const Cv2&v0=tri.m_pos[0];
		const Cv2&v1=tri.m_pos[1];
		const Cv2&v2=tri.m_pos[2];
		const Cv2&texCoord0=tri.m_texCoord[0];
		const Cv2&texCoord1=tri.m_texCoord[1];
		const Cv2&texCoord2=tri.m_texCoord[2];
		const Cv4&color0=tri.m_color[0];
		const Cv4&color1=tri.m_color[1];
		const Cv4&color2=tri.m_color[2];
		m_mesh->vlist.push_back(v0);
		m_mesh->texCoordList.push_back(texCoord0);
		m_mesh->colorList.push_back(color0);
		int ID0=(int)m_mesh->vlist.size()-1;
		m_mesh->vlist.push_back(v1);
		m_mesh->texCoordList.push_back(texCoord1);
		m_mesh->colorList.push_back(color1);
		int ID1=(int)m_mesh->vlist.size()-1;
		m_mesh->vlist.push_back(v2);
		m_mesh->texCoordList.push_back(texCoord2);
		m_mesh->colorList.push_back(color2);
		int ID2=(int)m_mesh->vlist.size()-1;
		m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
	}

	// 根据三角面，计算网格线
	// 为了保证每条线有独立颜色，我们需要复制网格线的顶点。所以每个三角面有6个顶点。
	for(int i=0;i<nTri;i++)
	{
		const FKCW_RenderTriangle&tri=m_triList[i];
		const Cv2&v0=tri.m_pos[0];
		const Cv2&v1=tri.m_pos[1];
		const Cv2&v2=tri.m_pos[2];
		const Cv2&texCoord0=tri.m_texCoord[0];
		const Cv2&texCoord1=tri.m_texCoord[1];
		const Cv2&texCoord2=tri.m_texCoord[2];
		const Cv4&color0=tri.m_color[0];
		const Cv4&color1=tri.m_color[1];
		const Cv4&color2=tri.m_color[2];

		//  计算三条线
		//IDline0
		m_meshWire->vlist.push_back(v0);//v0
		m_meshWire->texCoordList.push_back(texCoord0);
		m_meshWire->colorList.push_back(color0);
		int ID0=(int)m_meshWire->vlist.size()-1;
		m_meshWire->vlist.push_back(v1);//v1
		m_meshWire->texCoordList.push_back(texCoord1);
		m_meshWire->colorList.push_back(color1);
		int ID1=(int)m_meshWire->vlist.size()-1;
		//IDline1
		m_meshWire->vlist.push_back(v1);//v2
		m_meshWire->texCoordList.push_back(texCoord1);
		m_meshWire->colorList.push_back(color1);
		int ID2=(int)m_meshWire->vlist.size()-1;
		m_meshWire->vlist.push_back(v2);//v3
		m_meshWire->texCoordList.push_back(texCoord2);
		m_meshWire->colorList.push_back(color2);
		int ID3=(int)m_meshWire->vlist.size()-1;
		//IDline2
		m_meshWire->vlist.push_back(v2);//v4
		m_meshWire->texCoordList.push_back(texCoord2);
		m_meshWire->colorList.push_back(color2);
		int ID4=(int)m_meshWire->vlist.size()-1;
		m_meshWire->vlist.push_back(v0);//v5
		m_meshWire->texCoordList.push_back(texCoord0);
		m_meshWire->colorList.push_back(color0);
		int ID5=(int)m_meshWire->vlist.size()-1;
		//make three IDlines
		m_meshWire->IDlineList.push_back(CIDLine(ID0,ID1));
		m_meshWire->IDlineList.push_back(CIDLine(ID2,ID3));
		m_meshWire->IDlineList.push_back(CIDLine(ID4,ID5));
	}

	// 计算每条线的颜色
	{
		const float lightStrength0=3.0;
		const float lightStrength1=1.5;
		const CCPoint lightDir0=CCPoint(cosf(-45*M_PI/180),sinf(-45*M_PI/180));
		const CCPoint lightDir1=CCPoint(cosf(-135*M_PI/180),sinf(-135*M_PI/180));
		const float ambientStrength=0.5;
		int nIDline=(int)m_meshWire->IDlineList.size();
		for(int i=0;i<nIDline;i++)
		{
			CIDLine&IDline=m_meshWire->IDlineList[i];
			Cv4&colorA=m_meshWire->colorList[IDline.getvIDByIndex(0)];
			Cv4&colorB=m_meshWire->colorList[IDline.getvIDByIndex(1)];
			Cv2&vA=m_meshWire->vlist[IDline.getvIDByIndex(0)];
			Cv2&vB=m_meshWire->vlist[IDline.getvIDByIndex(1)];
			// 计算右侧发现
			CCPoint dir=ccpNormalize(Cv2::V2ToCCP(vB)-Cv2::V2ToCCP(vA));
			CCPoint rightNormal=FKCW_Base_Utils::GetRightNormal(dir);
			float colorStrength0=MAX(0,ccpDot(-lightDir0, rightNormal)*lightStrength0)+ambientStrength;
			float colorStrength1=MAX(0,ccpDot(-lightDir1, rightNormal)*lightStrength1)+ambientStrength;
			float colorStrength=MAX(colorStrength0,colorStrength1);
			colorA.init(colorStrength, colorStrength, colorStrength, 1);
			colorB.init(colorStrength, colorStrength, colorStrength, 1);
		}
	}
	// 提交一个网格
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_STATIC_DRAW);
	m_indexVBO->SubmitColor(m_mesh->colorList, GL_STATIC_DRAW);
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
	m_indexVBO->SubmitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
	// 提交线状网格
	m_indexVBO_wire->SubmitPos(m_meshWire->vlist, GL_STATIC_DRAW);
	m_indexVBO_wire->SubmitColor(m_meshWire->colorList, GL_STATIC_DRAW);
	m_indexVBO_wire->SubmitTexCoord(m_meshWire->texCoordList, GL_STATIC_DRAW);
	m_indexVBO_wire->SubmitIndex(m_meshWire->IDlineList, GL_STATIC_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::_GenerateDelayTimeForTris(float minDelayTime,float maxDelayTime)
{
	// 使用公式如下：
	// delayTime=(area^k-minArea^k)/(maxArea^k-minArea^k)*(maxDelayTime-minDelayTime)+minDelayTime
	const float k=1.5;
	// 为每个三角形计算 pow(area,k)
	vector<float> areaPowKList;
	areaPowKList.reserve((int)m_triList.size());
	int nTri=(int)m_triList.size();
	for(int i=0;i<nTri;i++)
	{
		FKCW_RenderTriangle&tri=m_triList[i];
		float Area=FKCW_Base_Utils::CalculateTriangleArea(Cv2::V2ToCCP(tri.m_pos[0]), Cv2::V2ToCCP(tri.m_pos[1]), Cv2::V2ToCCP(tri.m_pos[2]));
		Area+=(FKCW_Base_Utils::Random01()-0.5)*Area/3;
		float AreaPowK=powf(Area, k);
		areaPowKList.push_back(AreaPowK);
	}
	// 查找 minAreaPowK 和 maxAreaPowK
	float minAreaPowK=INFINITY;
	float maxAreaPowK=-INFINITY;
	int nAreaPowK=(int)areaPowKList.size();
	for(int i=0;i<nAreaPowK;i++)
	{
		float areaPowK=areaPowKList[i];
		if(areaPowK>maxAreaPowK)maxAreaPowK=areaPowK;
		if(areaPowK<minAreaPowK)minAreaPowK=areaPowK;
	}
	// 为每个三角形计算其延迟时间
	for(int i=0;i<nTri;i++)
	{
		FKCW_RenderTriangle&tri=m_triList[i];
		float areaPowK=areaPowKList[i];
		float delayTime=(areaPowK-minAreaPowK)/(maxAreaPowK-minAreaPowK)*(maxDelayTime-minDelayTime)+minDelayTime;
		tri.m_fallOffDelay=delayTime;
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::_UpdateMeshAndSubmitChange()
{
	// 基于三角形列表更新其网格和网格线
	int nTri=(int)m_triList.size();
	for(int i=0;i<nTri;i++)
	{
		const FKCW_RenderTriangle&tri=m_triList[i];
		const Cv2&v0=tri.m_pos[0];
		const Cv2&v1=tri.m_pos[1];
		const Cv2&v2=tri.m_pos[2];

		// 更新网格中三角形的顶点数据（vlist[3*i]~[3*i+2]是一个三角形的顶点数据）
		m_mesh->vlist[3*i]=v0;
		m_mesh->vlist[3*i+1]=v1;
		m_mesh->vlist[3*i+2]=v2;
		// 更新网格线中三角形的顶点数据（vlist[6*i]~[6*i+5]是一个三角形的顶点数据）
		m_meshWire->vlist[6*i]=v0;
		m_meshWire->vlist[6*i+1]=v1;
		m_meshWire->vlist[6*i+2]=v1;
		m_meshWire->vlist[6*i+3]=v2;
		m_meshWire->vlist[6*i+4]=v2;
		m_meshWire->vlist[6*i+5]=v0;
	}
	// 提交网格和网格线的顶点更改
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_STREAM_DRAW);
	m_indexVBO_wire->SubmitPos(m_meshWire->vlist, GL_STREAM_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_BreakSprite::_UpdateBreakAction(float time,float dt,float ay)
{
	// 三角形坠落
	int nTri=(int)m_triList.size();
	for(int i=0;i<nTri;i++)
	{
		FKCW_RenderTriangle&tri=m_triList[i];
		if(time>tri.m_fallOffDelay)
		{
			tri.m_vy+=ay*dt;
			float dy=tri.m_vy*dt;
			Cv2&v0=tri.m_pos[0];
			Cv2&v1=tri.m_pos[1];
			Cv2&v2=tri.m_pos[2];
			v0.sety(v0.y()+dy);
			v1.sety(v1.y()+dy);
			v2.sety(v2.y()+dy);
		}
	}
	// 更新网格并提交该更变
	_UpdateMeshAndSubmitChange();
}
//-------------------------------------------------------
// FKCW_Action_FallOffAction
//-------------------------------------------------------
FKCW_Action_FallOffAction::FKCW_Action_FallOffAction()
{
	m_ay=FKCW_Action_FallOff_AY;
	m_timeFoe=0;
	m_timeCur=0;
}
//-------------------------------------------------------
FKCW_Action_FallOffAction* FKCW_Action_FallOffAction::create(float fDuration,float ay)
{

	FKCW_Action_FallOffAction *p = new FKCW_Action_FallOffAction();
	p->initWithDuration(fDuration,ay);
	p->autorelease();

	return p;
}
//-------------------------------------------------------------------------
bool FKCW_Action_FallOffAction::initWithDuration(float fDuration,float ay)
{
	if (CCActionInterval::initWithDuration(fDuration))
	{
		m_timeFoe=0;
		m_timeCur=0;
		m_ay=ay;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_Action_FallOffAction::startWithTarget(CCNode *pTarget)
{
	((FKCW_EffectNode_BreakSprite*)pTarget)->m_state=eBreakSpriteState_FallOff;
	CCActionInterval::startWithTarget(pTarget);
}
//-------------------------------------------------------------------------
void FKCW_Action_FallOffAction::update(float time)
{
	// 因为动作中的update(t)和精灵中的update(t)，这俩t的概念不一样。
	// 动作中的t表示，进度百分比，即 (已播放的时间/动作总时间)
	// 而精灵中的t表示，当前帧和前一帧时间的deltaTime。
	// 所以这里要做转换
	m_timeFoe=m_timeCur;
	float progressPercentage=time;
	m_timeCur=progressPercentage*getDuration();
	((FKCW_EffectNode_BreakSprite*)m_pTarget)->_UpdateBreakAction(m_timeCur,m_timeCur-m_timeFoe,m_ay);
}
//-------------------------------------------------------------------------