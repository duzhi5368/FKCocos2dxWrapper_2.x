//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_Ripper2DSprite.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
//-------------------------------------------------------------------------
FKCW_EffectNode_Ripple2DSprite::FKCW_EffectNode_Ripple2DSprite()
{
	m_dx=5;
	m_mesh=NULL;
	m_indexVBO=NULL;
	m_isDrawDebug=false;
	m_reduceStep=7;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_Ripple2DSprite::~FKCW_EffectNode_Ripple2DSprite()
{
	if(m_mesh)m_mesh->release();
	if(m_indexVBO)m_indexVBO->release();
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::init(const string&texFileName)
{
	this->CCSprite::initWithFile(texFileName.c_str());

	CCSize contentSize=this->getContentSize();
	int nseg=ceilf(contentSize.width/m_dx);
	m_dx=contentSize.width/nseg;
	int nPoint=nseg+1;
	for(int i=0;i<nPoint;i++)
	{
		float x=i*m_dx;
		CCPoint point(x,contentSize.height);
		m_surfacePointList.push_back(point);
	}
	// 将前台显示缓冲拷贝到后台
	m_surfacePointList_back=m_surfacePointList;
	// 创建Mesh
	m_mesh=new FKCW_EffectNode_Mesh();
	m_mesh->autorelease();
	m_mesh->retain();
	// 创建IndexVBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 初始化网格
	_InitMesh();
	// 提交网格
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_STATIC_DRAW);
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
	m_indexVBO->SubmitColor(m_mesh->colorList, GL_STATIC_DRAW);
	m_indexVBO->SubmitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::pressAtX(float x,float h_press,float r_press)
{
	// 获取水表面起始点和终止点
	if((int)m_surfacePointList.size()==0)
		return;
	if((int)m_surfacePointList.size()==1)
		return;
	CCPoint startPoint_local=m_surfacePointList[0];
	CCPoint endPoint_local=m_surfacePointList[(int)m_surfacePointList.size()-1];

	// 将起始点和终止点转换为父空间坐标
	CCPoint startPoint=CCPointApplyAffineTransform(startPoint_local,nodeToParentTransform());
	CCPoint endPoint=CCPointApplyAffineTransform(endPoint_local,nodeToParentTransform());

	// 计算两个线的交点 （石头落下垂线(X=x) 和水平面(startPoint-endPoint)的交点）
	CCPoint leftPoint=(startPoint.x<endPoint.x)?startPoint:endPoint;
	CCPoint rightPoint=(startPoint.x<endPoint.x)?endPoint:startPoint;
	if(x>leftPoint.x&&x<rightPoint.x)
	{// 确实横切到了（确实落到了水面上）
		assert(leftPoint.x!=rightPoint.x);
		float y=(leftPoint.y-rightPoint.y)/(leftPoint.x-rightPoint.x)*(x-leftPoint.x)+leftPoint.y;
		CCPoint touchPoint(x,y);
		CCPoint touchPoint_local=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform());

		float touchx=touchPoint_local.x;
		int nPoint=(int)m_surfacePointList.size();
		for(int i=0;i<nPoint;i++)
		{
			if(i==0||i==nPoint-1)
				continue;// 若点到了起始点或者终止点，不处理
			CCPoint&point=m_surfacePointList[i];
			float r=fabsf(point.x-touchx);
			if(r>r_press)continue;
			point.y-=(cosf(r/r_press*M_PI)+1)/2*h_press;
		}
	}
	else
	{
		return;
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::_InitMesh()
{
	m_mesh->clear();
	m_surfacePointList_reduced.clear();
	for(int i=0;i<(int)m_surfacePointList.size();i+=m_reduceStep)
	{
		CCPoint pos=m_surfacePointList[i];
		m_surfacePointList_reduced.push_back(pos);
	}
	m_surfacePointList_reduced.push_back(m_surfacePointList[(int)m_surfacePointList.size()-1]);

	CCPoint startPoint=m_surfacePointList_reduced[0];
	CCPoint endPoint=m_surfacePointList_reduced[(int)m_surfacePointList_reduced.size()-1];
	const float len=endPoint.x-startPoint.x;
	int nPoint=(int)m_surfacePointList_reduced.size();
	for(int i=0;i<nPoint-1;i++)
	{
		const CCPoint&p=m_surfacePointList_reduced[i];
		const CCPoint&pn=m_surfacePointList_reduced[i+1];
		//LU
		Cv2 posLU=Cv2(p.x,p.y);
		Cv2 texCoordLU=Cv2((posLU.x()-startPoint.x)/len, 0);
		Cv4 colorLU=Cv4(1,1,1,1);

		//LD
		Cv2 posLD=Cv2(p.x,0);
		Cv2 texCoordLD=Cv2((posLD.x()-startPoint.x)/len, 1);
		Cv4 colorLD=Cv4(1,1,1,1);

		//RD
		Cv2 posRD=Cv2(pn.x,0);
		Cv2 texCoordRD=Cv2((posRD.x()-startPoint.x)/len, 1);
		Cv4 colorRD=Cv4(1,1,1,1);

		//RU
		Cv2 posRU=Cv2(pn.x,pn.y);
		Cv2 texCoordRU=Cv2((posRU.x()-startPoint.x)/len, 0);
		Cv4 colorRU=Cv4(1,1,1,1);

		// 添加顶点
		m_mesh->vlist.push_back(posLU);
		m_mesh->texCoordList.push_back(texCoordLU);
		m_mesh->colorList.push_back(colorLU);
		int IDLU=(int)m_mesh->vlist.size()-1;

		m_mesh->vlist.push_back(posLD);
		m_mesh->texCoordList.push_back(texCoordLD);
		m_mesh->colorList.push_back(colorLD);
		int IDLD=(int)m_mesh->vlist.size()-1;

		m_mesh->vlist.push_back(posRD);
		m_mesh->texCoordList.push_back(texCoordRD);
		m_mesh->colorList.push_back(colorRD);
		int IDRD=(int)m_mesh->vlist.size()-1;

		m_mesh->vlist.push_back(posRU);
		m_mesh->texCoordList.push_back(texCoordRU);
		m_mesh->colorList.push_back(colorRU);
		int IDRU=(int)m_mesh->vlist.size()-1;

		CIDTriangle IDtri0,IDtri1;
		IDtri0.init(IDLU, IDLD, IDRD);
		IDtri1.init(IDLU, IDRD, IDRU);
		m_mesh->IDtriList.push_back(IDtri0);
		m_mesh->IDtriList.push_back(IDtri1);
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::_UpdateMesh()
{
	m_surfacePointList_reduced.clear();
	for(int i=0;i<(int)m_surfacePointList.size();i+=m_reduceStep){
		CCPoint pos=m_surfacePointList[i];
		m_surfacePointList_reduced.push_back(pos);
	}
	m_surfacePointList_reduced.push_back(m_surfacePointList[(int)m_surfacePointList.size()-1]);

	int nV=(int)m_mesh->vlist.size();
	int nPoint=(int)m_surfacePointList_reduced.size();
	for(int i=0;i<nPoint;i++)
	{
		const CCPoint&p=m_surfacePointList_reduced[i];
		//  0           1            2
		//  *-----*-----*
		//    0-3   4-7
		//    |    |    |    |
		//    1-2   5-6
		int imul4=i*4;
		if(imul4<nV)
		{
			m_mesh->vlist[imul4].sety(p.y);
		}

		if(imul4-1>0)
		{
			m_mesh->vlist[imul4-1].sety(p.y);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::_UpdateRipple()
{
	CCSize contentSize=this->getContentSize();
	// 更新表面顶点列表的顶点高度
	{
		int nPoint=(int)m_surfacePointList.size();
		for(int i=0;i<nPoint;i++)
		{
			CCPoint&point=m_surfacePointList[i];
			CCPoint&pointf=m_surfacePointList[i==0?nPoint-1:i-1];
			CCPoint&pointn=m_surfacePointList[i==nPoint-1?0:i+1];
			CCPoint&point_back=m_surfacePointList_back[i];
			point_back.y=contentSize.height+((pointf.y-contentSize.height)
				+(pointn.y-contentSize.height)
				-(point_back.y-contentSize.height))*(1.0-1.0/60);//150);
		}
		// 向右移
		{
			int nPoint=(int)m_surfacePointList.size();
			CCPoint lastPoint=m_surfacePointList[nPoint-1];
			CCPoint lastPoint_back=m_surfacePointList_back[nPoint-1];
			for(int i=nPoint-1;i>=1;i--){
				m_surfacePointList[i].y=m_surfacePointList[i-1].y;
				m_surfacePointList_back[i].y=m_surfacePointList_back[i-1].y;
			}
			m_surfacePointList[0].y=lastPoint.y;
			m_surfacePointList_back[0].y=lastPoint_back.y;
		}

		// 交换前后缓冲点列表
		vector<CCPoint> temp=m_surfacePointList;
		m_surfacePointList=m_surfacePointList_back;
		m_surfacePointList_back=temp;
	}

	_UpdateMesh();
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_DYNAMIC_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::update(float dt)
{
	_UpdateRipple();
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::draw()
{
	// 更换shader
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	ccGLEnable(m_eGLServerState);

	FKCW_EffectNode_GLProgramWithUnifos* program=(FKCW_EffectNode_GLProgramWithUnifos*)this->getShaderProgram();
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
	// 设置VBO
	m_indexVBO->SetPointerPosition();
	m_indexVBO->SetPointerTexCoord();
	m_indexVBO->SetPointerColor();
	m_indexVBO->Draw(GL_TRIANGLES);
	// 解除纹理
	ccGLBindTexture2D(0);

	FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
	if(m_isDrawDebug)
	{
		drawWire();
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_Ripple2DSprite::drawWire()
{
	// 绘制段列表
	glLineWidth(2);
	ccPointSize(4);
	int nPoint=(int)m_surfacePointList_reduced.size();
	int nSeg=nPoint-1;
	for(int i=0;i<nSeg;i++)
	{
		CCPoint point=m_surfacePointList_reduced[i];
		CCPoint pointn=m_surfacePointList_reduced[i+1];
		ccDrawLine(point, pointn);
		ccDrawPoint(point);
		ccDrawPoint(pointn);
	}
}
//-------------------------------------------------------------------------