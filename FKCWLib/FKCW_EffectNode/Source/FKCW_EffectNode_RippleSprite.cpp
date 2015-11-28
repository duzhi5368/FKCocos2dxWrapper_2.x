//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_RippleSprite.h"
//-------------------------------------------------------------------------
FKCW_EffectNode_RippleSprite::FKCW_EffectNode_RippleSprite()
{
	m_indexVBO=NULL;
	m_mesh=NULL;
	m_srcBuffer=NULL;
	m_dstBuffer=NULL;
	m_nRow=0;
	m_nCol=0;
	m_rippleStrength=8;
	m_isDrawDebug=false;
	m_time=0;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_RippleSprite::~FKCW_EffectNode_RippleSprite()
{
	if(m_indexVBO)m_indexVBO->release();
	if(m_mesh)m_mesh->release();
	if(m_srcBuffer)m_srcBuffer->release();
	if(m_dstBuffer)m_dstBuffer->release();
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_RippleSprite::init(string texFileName,float gridSideLen)
{
	m_gridSideLen=gridSideLen;
	// 初始化精灵
	this->CCSprite::initWithFile(texFileName.c_str());
	// 计算行列数
	CCSize contentSize=this->getContentSize();
	m_nRow = static_cast<int>(floorf(contentSize.height/gridSideLen)+1);// 额外添加一行以保证缓冲区大于背景图片
	m_nCol = static_cast<int>(floorf(contentSize.width/gridSideLen)+1);	// 额外添加一列以保证缓冲区大于背景图片

	// 创建网格
	m_mesh=new FKCW_EffectNode_Mesh();
	m_mesh->autorelease();
	m_mesh->retain();
	// 填充顶点数据
	m_mesh->vlist.reserve((m_nRow+1)*(m_nCol+1));
	m_mesh->texCoordList.reserve((m_nRow+1)*(m_nCol+1));
	m_mesh->colorList.reserve((m_nRow+1)*(m_nCol+1));
	for(int i=0;i<m_nRow+1;i++)
	{
		for(int j=0;j<m_nCol+1;j++)
		{
			float x=m_gridSideLen*j;
			float y=contentSize.height-m_gridSideLen*i;
			float s=x/contentSize.width;
			float t=1-y/contentSize.height;
			Cv2 pos=Cv2(x,y);
			Cv2 texCoord=Cv2(s,t);
			Cv4 color=Cv4(1,1,1,1);
			m_mesh->vlist.push_back(pos);
			m_mesh->texCoordList.push_back(texCoord);
			m_mesh->colorList.push_back(color);
		}
	}
	// 填充索引
	const int nVertexPerRow=m_nCol+1;
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			//	当前格子是 grid(i,j)
			//	grid(i,j) 的左上顶点是 vertex(i,j)
			//	vertex(i,j) 是 vertex(i*nVertexPerRow+j)
			int vID_LU=i*nVertexPerRow+j;
			int vID_RU=vID_LU+1;
			int vID_LD=vID_LU+nVertexPerRow;
			int vID_RD=vID_LD+1;
			CIDTriangle IDtri1=CIDTriangle(vID_LU, vID_LD, vID_RD);
			CIDTriangle IDtri2=CIDTriangle(vID_LU, vID_RD, vID_RU);
			m_mesh->IDtriList.push_back(IDtri1);
			m_mesh->IDtriList.push_back(IDtri2);
		}
	}
	// 保存纹理UV列表
	m_texCoordList_store=m_mesh->texCoordList;
	// 创建IndexVBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 提交网格信息
	m_indexVBO->SubmitPos(m_mesh->vlist, GL_STATIC_DRAW);
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
	m_indexVBO->SubmitColor(m_mesh->colorList, GL_STATIC_DRAW);
	m_indexVBO->SubmitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);

	//生成原Buffer和目标buffer
	{
		m_srcBuffer=new FKCW_EffectNode_RippleBuffer();
		m_srcBuffer->autorelease();
		m_srcBuffer->retain();
		m_srcBuffer->mat.resize(m_nRow+1);
		for(int i=0;i<(int)m_srcBuffer->mat.size();i++)
		{
			m_srcBuffer->mat[i].resize(m_nCol+1);
		}

		m_dstBuffer=new FKCW_EffectNode_RippleBuffer();
		m_dstBuffer->autorelease();
		m_dstBuffer->retain();
		*m_dstBuffer=*m_srcBuffer;
	}
	return true;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_RippleSprite::update(float dt)
{
	m_time+=dt;
	if(m_time>=1.0/45)
	{
		_UpdateOnce();
		m_time=0;
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_RippleSprite::_UpdateOnce()
{
	// 更新Buffer和网格mesh
	float k=0.5f-0.5f/m_rippleStrength;
	float kTexCoord=1.0f/1048;
	const int nRow=m_nRow+1;
	const int nCol=m_nCol+1;
	for(int i=1;i<nRow-1;i++)
	{
		for(int j=1;j<nCol-1;j++)
		{
			// 更新 m_dstBuffer
			float Hup_src=m_srcBuffer->mat[i-1][j];
			float Hdn_src=m_srcBuffer->mat[i+1][j];
			float Hleft_src=m_srcBuffer->mat[i][j-1];
			float Hright_src=m_srcBuffer->mat[i][j+1];
			float Hcenter_dst=m_dstBuffer->mat[i][j];
			float H=(Hup_src+Hdn_src+Hleft_src+Hright_src-2*Hcenter_dst)*k;
			m_dstBuffer->mat[i][j]=H;
			// 更新纹理 texCoord
			float s_offset=(Hup_src-Hdn_src)*kTexCoord;
			float t_offset=(Hleft_src-Hright_src)*kTexCoord;
			Cv2&texCoord=m_mesh->texCoordList[i*nCol+j];
			Cv2&texCoord_store=m_texCoordList_store[i*nCol+j];
			texCoord.setx(texCoord_store.x()+s_offset);
			texCoord.sety(texCoord_store.y()+t_offset);
		}
	}
	FKCW_EffectNode_RippleBuffer*temp=m_dstBuffer;
	m_dstBuffer=m_srcBuffer;
	m_srcBuffer=temp;
	// 重新提交纹理UV
	m_indexVBO->SubmitTexCoord(m_mesh->texCoordList, GL_DYNAMIC_DRAW);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_RippleSprite::doTouch(const CCPoint&touchPoint,float depth,float r)
{
	// 触摸点是在父类空间位置，将其转换为本地空间位置
	CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
	CCPoint touchPoint_localSpace=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform);
	// 将本地空间位置转换为以左上角为基点的空间(OLU = origin at left up corner)坐标位置
	CCSize contentSize=this->getContentSize();
	float x_OLU=touchPoint_localSpace.x;
	float y_OLU=contentSize.height-touchPoint_localSpace.y;

	// OLU空间坐标位置
	float xmin=x_OLU-r;
	float xmax=x_OLU+r;
	float ymin=y_OLU-r;
	float ymax=y_OLU+r;

	// 计算索引范围
	int imin,imax,jmin,jmax;
	int nRow=m_nRow+1;
	int nCol=m_nCol+1;
	const int imargin=1;
	const int jmargin=1;
	imin=static_cast<int>(MAX(imargin,floorf(ymin/m_gridSideLen)));
	imax=static_cast<int>(MIN(nRow-1-imargin,ceilf(ymax/m_gridSideLen)-1));
	jmin=static_cast<int>(MAX(jmargin,floorf(xmin/m_gridSideLen)));
	jmax=static_cast<int>(MIN(nCol-1-jmargin,ceilf(xmax/m_gridSideLen)-1));

	// 遍历在 [imin,imax]x[jmin,jmax] 中的顶点，并将挤压这些顶点
	for(int i=imin;i<=imax;i++)
	{
		for(int j=jmin;j<=jmax;j++)
		{
			const Cv2&v=m_mesh->vlist[i*nCol+j];
			const Cv2&v_OLU=Cv2(v.x(),contentSize.height-v.y());
			float dis=ccpDistance(CCPoint(x_OLU,y_OLU), CCPoint(v_OLU.x(),v_OLU.y()));
			if(dis<r)
			{
				float fX = static_cast<float>(dis*M_PI/r);
				float curDepth = depth*(0.5f+0.5f*cosf(fX));
				m_srcBuffer->mat[i][j]-=curDepth;
			}
			else
			{
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_RippleSprite::draw()
{
	// 对较大的矩形，使用剪裁测试比使用ClippingNode效率更高。
	// 开启剪裁测试。
	glEnable(GL_SCISSOR_TEST);
	// 剪裁测试，计算世界矩形
	CCSize contentSize=this->getContentSize();
	CCRect rect_local=CCRect(0,0,contentSize.width,contentSize.height);
	CCRect rect_world=CCRectApplyAffineTransform(rect_local, this->nodeToWorldTransform());
	// 不要使用glScissor，使用cocos2dx的API CCEGLView::setScissorInPoints，所以要注意，其参数是世界空间坐标。
	CCEGLView::sharedOpenGLView()->setScissorInPoints(rect_world.getMinX(), rect_world.getMinY(), rect_world.size.width, rect_world.size.height);
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
	// 进行纹理绑定
	ccGLBindTexture2D( this->getTexture()->getName());
	m_indexVBO->SetPointerPosition();
	m_indexVBO->SetPointerTexCoord();
	m_indexVBO->SetPointerColor();
	m_indexVBO->Draw(GL_TRIANGLES);
	ccGLBindTexture2D(0);

	FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
	// 关闭剪裁测试
	glDisable(GL_SCISSOR_TEST);

	if(m_isDrawDebug)
	{
		drawWire();
	}
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_RippleSprite::drawWire()
{
	glLineWidth(1);
	int nIDtri=(int)m_mesh->IDtriList.size();
	for(int i=0;i<nIDtri;i++)
	{
		const CIDTriangle&IDtri=m_mesh->IDtriList[i];
		Cv2 v0=m_mesh->vlist[IDtri.getvIDByIndex(0)];
		Cv2 v1=m_mesh->vlist[IDtri.getvIDByIndex(1)];
		Cv2 v2=m_mesh->vlist[IDtri.getvIDByIndex(2)];
		CCPoint vertices[]={CCPoint(v0.x(),v0.y()),CCPoint(v1.x(),v1.y()),CCPoint(v2.x(),v2.y())};
		ccDrawPoly(vertices, 3, true);
	}
}
//-------------------------------------------------------------------------