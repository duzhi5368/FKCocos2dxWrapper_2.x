//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_TinyWingsTerrainSprite.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
#pragma warning( disable:4305 )
//-------------------------------------------------------------------------
const static char* s_TinyWingsTerrainFragShader = "										\n\
#ifdef GL_ES																			\n\
	precision mediump float;															\n\
#endif																					\n\
																						\n\
varying vec2 v_texCoord;																\n\
uniform float u_texNonlinearFactor;														\n\
uniform vec4 u_colors[3];																\n\
uniform float u_cosA;																	\n\
uniform float u_sinA;																	\n\
uniform float u_ribbonRepeat;															\n\
void main()																				\n\
{																						\n\
	vec2 texCoord=vec2(v_texCoord.x,pow(v_texCoord.y,u_texNonlinearFactor));			\n\
	vec2 texCoord2;																		\n\
	texCoord2.x=texCoord.x*u_cosA-texCoord.y*u_sinA;									\n\
	texCoord2.y=texCoord.x*u_sinA+texCoord.y*u_cosA;									\n\
																						\n\
	int colorIndex = int(mod((texCoord2.y*u_ribbonRepeat),3.0));						\n\
	vec4 ribbonColor=u_colors[colorIndex];												\n\
	vec4 gradualColor=vec4(vec3((1.0-v_texCoord.y)*(1.0-v_texCoord.y)*0.7+0.3),1.0);	\n\
	gl_FragColor=ribbonColor*gradualColor*1.5;											\n\
}";
//-------------------------------------------------------------------------
FKCW_EffectNode_TinyWingsTerrainSprite::FKCW_EffectNode_TinyWingsTerrainSprite()
{
	m_dx=10;
	m_mesh=NULL;
	m_indexVBO=NULL;
	m_isDrawDebug=false;
	m_texture=NULL;
	m_program=NULL;
	m_texNonlinearFactor=0.7;
	m_ribbonRotation=75.0;
	m_ribbonRepeat=7.5;
	m_width=0;
	m_height=0;
	m_nRow=4;
	m_insertControlPointCount=4;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_TinyWingsTerrainSprite::~FKCW_EffectNode_TinyWingsTerrainSprite()
{
	if(m_mesh)m_mesh->release();
	if(m_indexVBO)m_indexVBO->release();
	if(m_texture)m_texture->release();
	if(m_program)m_program->release();
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::init(float width,float height,int insertControlPointCount)
{
	this->CCSprite::init();

	m_width=width;
	m_height=height;
	m_pointMat.resize(m_nRow);
	vector<CCPoint>&pointList=m_pointMat[0];
	int nseg=ceilf(m_width/m_dx);
	m_dx=m_width/nseg;
	int nPoint=nseg+1;
	for(int i=0;i<nPoint;i++)
	{
		float x=i*m_dx;
		CCPoint point(x,m_height);
		pointList.push_back(point);
		int nRow=(int)m_pointMat.size();
		for(int j=1;j<nRow;j++)
		{
			CCPoint _point;
			_point.x=point.x;
			_point.y=point.y*(1-(float)j/(nRow-1));// 注意一定要将j转换为float
			m_pointMat[j].push_back(_point);
		}
	}
	// 创建网格
	m_mesh=new FKCW_EffectNode_Mesh();
	m_mesh->autorelease();
	m_mesh->retain();
	// 创建IndexVBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 颜色列表
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
	// 调整点形状
	reGenerateShape(insertControlPointCount);
	// 更新网格
	_UpdateMesh();
	// 提交网格
	_Submit();
	// 创建并更新shader
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, s_TinyWingsTerrainFragShader);

		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		program->link();
		program->updateUniforms();
		// 设置自定义参数
		program->attachUniform("u_texNonlinearFactor");
		program->attachUniform("u_colors");
		program->attachUniform("u_cosA");
		program->attachUniform("u_sinA");
		program->attachUniform("u_ribbonRepeat");

		m_program=program;
		m_program->retain();
		CHECK_GL_ERROR_DEBUG();
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::reGenerateColors()
{
	m_colorList.clear();
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
	m_colorList.push_back(ccc4f(FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), 1));
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::_Submit()
{
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_STATIC_DRAW);
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
	m_indexVBO->SubmitColor(m_mesh->colorList, GL_STATIC_DRAW);
	m_indexVBO->SubmitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::reGenerateShape(int insertControlPointCount)
{
	m_insertControlPointCount=insertControlPointCount;
	vector<CCPoint>&pointList=m_pointMat[0];
	// 生成控制点
	CCPoint startPoint=pointList[0];
	CCPoint endPoint=pointList[(int)pointList.size()-1];
	float len=endPoint.x-startPoint.x;
	float stepLen=len/(insertControlPointCount+1);
	vector<CCPoint> controlPointList;
	controlPointList.push_back(CCPoint(startPoint.x,m_height)); // 起始控制点
	for(int i=0;i<insertControlPointCount;i++)
	{
		float x=(i+1)*stepLen;
		float y=m_height;
		controlPointList.push_back(CCPoint(x,y));
	}
	controlPointList.push_back(CCPoint(endPoint.x,m_height)); // 终止控制点
	// 为每个控制点随机Y值
	int nControlPoint=(int)controlPointList.size();
	for(int i=0;i<nControlPoint;i++)
	{
		CCPoint&controlPoint=controlPointList[i];
		float dy=(FKCW_Base_Utils::Random01()*2-1)*MIN(m_height*0.8,stepLen*0.7);
		if(i>0&&dy*(controlPointList[i-1].y-m_height)>0)
		{
			dy=-dy;
		}
		controlPoint.y=m_height+dy;
	}
	// 重新构造点列表
	int controlPointIndex=0;
	int nPoint=(int)pointList.size();
	for(int i=0;i<nPoint;i++)
	{
		CCPoint&point=pointList[i];
		// 找到第一个比point的X值更大的控制点
		while(controlPointIndex<(int)controlPointList.size()&&controlPointList[controlPointIndex].x<=point.x)
		{
			controlPointIndex++;
		}
		if(controlPointIndex>=(int)controlPointList.size())
		{// 该控制点是尾节点
			point=controlPointList[(int)controlPointList.size()-1];
		}else
		{
			assert(controlPointIndex>=1);
			CCPoint controlPoint=controlPointList[controlPointIndex-1];
			CCPoint controlPointn=controlPointList[controlPointIndex];

			// 找到改点，对y做sin差值
			const float dx=controlPointn.x-controlPoint.x;
			const float dy=controlPointn.y-controlPoint.y;
			point.y=controlPoint.y+dy/2*(1-cosf((M_PI/dx)*(point.x-controlPoint.x)));
		}
		int nRow=(int)m_pointMat.size();
		for(int j=1;j<nRow;j++)
		{
			CCPoint&_point=m_pointMat[j][i];
			_point.y=point.y*(1-pow((float)j/(nRow-1),1));// 注意一定要将j转换为float
		}
	}
	// 更新并提交网格
	_UpdateMesh();
	_Submit();
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::_UpdateMesh()
{
	m_mesh->clear();
	CCPoint startPoint=m_pointMat[0][0];
	CCPoint endPoint=m_pointMat[0][(int)m_pointMat[0].size()-1];
	const float len=endPoint.x-startPoint.x;
	int nRow=(int)m_pointMat.size();
	for(int i=0;i<nRow-1;i++)
	{
		int nPoint=(int)m_pointMat[i].size();
		for(int j=0;j<nPoint-1;j++)
		{
			const CCPoint&pLU=m_pointMat[i][j];
			const CCPoint&pLD=m_pointMat[i+1][j];
			const CCPoint&pRU=m_pointMat[i][j+1];
			const CCPoint&pRD=m_pointMat[i+1][j+1];
			//vLU
			Cv2 posLU=Cv2(pLU.x,pLU.y);
			Cv2 texCoordLU=Cv2((posLU.x()-startPoint.x)/len, (float)i/(nRow-1));
			Cv4 colorLU=Cv4(1,1,1,1);
			//vLD
			Cv2 posLD=Cv2(pLD.x,pLD.y);
			Cv2 texCoordLD=Cv2((posLD.x()-startPoint.x)/len, (float)(i+1)/(nRow-1));
			Cv4 colorLD=Cv4(1,1,1,1);
			//vRU
			Cv2 posRU=Cv2(pRU.x,pRU.y);
			Cv2 texCoordRU=Cv2((posRU.x()-startPoint.x)/len, (float)i/(nRow-1));
			Cv4 colorRU=Cv4(1,1,1,1);
			//vRD
			Cv2 posRD=Cv2(pRD.x,pRD.y);
			Cv2 texCoordRD=Cv2((posRD.x()-startPoint.x)/len, (float)(i+1)/(nRow-1));
			Cv4 colorRD=Cv4(1,1,1,1);

			// 添加顶点
			m_mesh->vlist.push_back(posLU);
			m_mesh->texCoordList.push_back(texCoordLU);
			m_mesh->colorList.push_back(colorLU);
			int IDLU=(int)m_mesh->vlist.size()-1;
			//
			m_mesh->vlist.push_back(posLD);
			m_mesh->texCoordList.push_back(texCoordLD);
			m_mesh->colorList.push_back(colorLD);
			int IDLD=(int)m_mesh->vlist.size()-1;
			//
			m_mesh->vlist.push_back(posRU);
			m_mesh->texCoordList.push_back(texCoordRU);
			m_mesh->colorList.push_back(colorRU);
			int IDRU=(int)m_mesh->vlist.size()-1;
			//
			m_mesh->vlist.push_back(posRD);
			m_mesh->texCoordList.push_back(texCoordRD);
			m_mesh->colorList.push_back(colorRD);
			int IDRD=(int)m_mesh->vlist.size()-1;

			CIDTriangle IDtri0,IDtri1;
			IDtri0.init(IDLU,IDLD,IDRD);
			IDtri1.init(IDLU, IDRD, IDRU);
			m_mesh->IDtriList.push_back(IDtri0);
			m_mesh->IDtriList.push_back(IDtri1);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::draw()
{
	// 更换shader
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	ccGLEnable(m_eGLServerState);

	this->setShaderProgram(m_program);
	FKCW_EffectNode_GLProgramWithUnifos* program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
	program->use();
	program->setUniformsForBuiltins();
	// 设置自定义值
	float colorList_tmp[4*3]={m_colorList[0].r,m_colorList[0].g,m_colorList[0].b,m_colorList[0].a,
		m_colorList[1].r,m_colorList[1].g,m_colorList[1].b,m_colorList[1].a,
		m_colorList[2].r,m_colorList[2].g,m_colorList[2].b,m_colorList[2].a};
	program->passUnifoValue1f("u_texNonlinearFactor", m_texNonlinearFactor);
	program->passUnifoValueNfvN("u_colors", colorList_tmp, 4, 3);
	program->passUnifoValue1f("u_cosA", cosf(m_ribbonRotation*M_PI/180));
	program->passUnifoValue1f("u_sinA", sinf(m_ribbonRotation*M_PI/180));
	program->passUnifoValue1f("u_ribbonRepeat", m_ribbonRepeat);

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
	// 解绑纹理
	ccGLBindTexture2D(0);

	FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
	// 绘制网格线
	if(m_isDrawDebug){
		drawWire();
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TinyWingsTerrainSprite::drawWire()
{
	vector<CCPoint>& pointList = m_pointMat[0];
	// 绘制片段列表
	{
		glLineWidth(2);
		ccPointSize(4);
		int nPoint=(int)pointList.size();
		int nSeg=nPoint-1;
		for(int i=0;i<nSeg;i++)
		{
			CCPoint point=pointList[i];
			CCPoint pointn=pointList[i+1];
			ccDrawLine(point, pointn);
			ccDrawPoint(point);
			ccDrawPoint(pointn);
		}
	}

	// 绘制网格
	{
		glLineWidth(2);
		int nRow=(int)m_pointMat.size();
		for(int i=0;i<nRow-1;i++)
		{
			int nPoint=(int)m_pointMat[i].size();
			for(int j=0;j<nPoint-1;j++)
			{
				const CCPoint&pLU=m_pointMat[i][j];
				const CCPoint&pLD=m_pointMat[i+1][j];
				const CCPoint&pRU=m_pointMat[i][j+1];
				const CCPoint&pRD=m_pointMat[i+1][j+1];

				ccDrawLine(pLU, pLD);
				ccDrawLine(pLD, pRD);
				ccDrawLine(pRD, pRU);
				ccDrawLine(pRU, pLU);
			}
		}
	}
}
//-------------------------------------------------------------------------