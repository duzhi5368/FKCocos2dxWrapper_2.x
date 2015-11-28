//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_TailSprite.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
#pragma warning( disable:4244 )
#pragma warning( disable:4305 )
//-------------------------------------------------------------------------
const char* s_TailFragShader = "						\n\
#ifdef GL_ES											\n\
	precision mediump float;							\n\
#endif													\n\
														\n\
varying vec2 v_texCoord;								\n\
varying vec4 v_color;									\n\
uniform sampler2D CC_Texture0;							\n\
														\n\
void main() 											\n\
{														\n\
	vec4 texColor=texture2D(CC_Texture0, v_texCoord);	\n\
	gl_FragColor=texColor*v_color;						\n\
}";

const char* s_TailVertexShader = "						  \n\
attribute vec4 a_position;								  \n\
attribute vec2 a_texCoord;								  \n\
attribute vec4 a_color;									  \n\
varying vec2 v_texCoord;								  \n\
varying vec4 v_color;									  \n\
														  \n\
void main()												  \n\
{														  \n\
	vec4 pos=vec4(a_position.xy,0,1);					  \n\
	gl_Position = CC_MVPMatrix * a_position;			  \n\
	v_texCoord = a_texCoord;							  \n\
	v_color = a_color;									  \n\
}";
//-------------------------------------------------------------------------
// FKCW_EffectNode_TailQuad
//-------------------------------------------------------------------------
FKCW_EffectNode_TailQuad::FKCW_EffectNode_TailQuad()
{
	m_opacity=INFINITY;
	m_opacityLU=INFINITY;
	m_opacityLD=INFINITY;
	m_opacityRU=INFINITY;
	m_opacityRD=INFINITY;
	m_isTriangle=false;
}
//-------------------------------------------------------------------------
// FKCW_EffectNode_TailFootPrint
//-------------------------------------------------------------------------
FKCW_EffectNode_TailFootPrint::FKCW_EffectNode_TailFootPrint()
{
	_InitMembers();
}
//-------------------------------------------------------------------------
FKCW_EffectNode_TailFootPrint::FKCW_EffectNode_TailFootPrint(const CCPoint&pos,float opacity)
{
	_InitMembers();
	m_tagPos=pos;
	m_fOpacity=opacity;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TailFootPrint::_InitMembers()
{
	m_fOpacity=1.0;
}
//-------------------------------------------------------------------------
// FKCW_EffectNode_TailSprite
//-------------------------------------------------------------------------
FKCW_EffectNode_TailSprite::FKCW_EffectNode_TailSprite() 
{
	m_dt=0.1;
	m_indexVBO=NULL;
	m_program=NULL;
	m_opacityDamping=0.95;
	m_initialOpacity=10;
	m_minDis=14;
	m_isShowWire=false;
}
//-------------------------------------------------------------------------
FKCW_EffectNode_TailSprite::~FKCW_EffectNode_TailSprite()
{
	if(m_indexVBO)m_indexVBO->release();
	if(m_program)m_program->release();
};
//-------------------------------------------------------------------------
bool FKCW_EffectNode_TailSprite::init(const string&texFileName)
{
	// 初始化精灵
	this->CCSprite::initWithFile(texFileName.c_str());
	// 创建IndexVBO
	m_indexVBO=new FKCW_EffectNode_IndexVBO();
	m_indexVBO->autorelease();
	m_indexVBO->retain();
	// 创建并设置shader
	{
		FKCW_EffectNode_GLProgramWithUnifos* program = new FKCW_EffectNode_GLProgramWithUnifos();
		program->autorelease();
		program->initWithVertexShaderByteArray(s_TailVertexShader, s_TailFragShader);
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		program->link();
		program->updateUniforms();
		m_program=program;
		m_program->retain();
		CHECK_GL_ERROR_DEBUG();
	}
	CHECK_GL_ERROR_DEBUG();
	ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE};
	this->setBlendFunc(blendFunc);
	return true ;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TailSprite::draw()
{
	// 生成网格
	if(m_footPrintList.empty())return;
	vector<FKCW_EffectNode_TailFootPrint> footPrintList=m_footPrintList;
	if( !FKCW_Base_Utils::IsPointEqual(this->getPosition(), footPrintList[(int)footPrintList.size()-1].m_tagPos) )
	{
		footPrintList.push_back(FKCW_EffectNode_TailFootPrint(this->getPosition(),m_initialOpacity));
	}
	// 转换 m_vlist 到本地矩阵
	int nFootPrint=(int)footPrintList.size();
	for(int i=0;i<nFootPrint;i++)
	{
		FKCW_EffectNode_TailFootPrint&footPrint=footPrintList[i];
		footPrint.m_tagPos=convertToNodeSpace(footPrint.m_tagPos);
	}
	FKCW_EffectNode_Mesh* mesh=_GenerateMesh(footPrintList);
	// 提交网格
	m_indexVBO->SubmitPos(mesh->vlist, GL_STREAM_DRAW);
	m_indexVBO->SubmitIndex(mesh->IDtriList, GL_STREAM_DRAW);
	m_indexVBO->SubmitTexCoord(mesh->texCoordList, GL_STREAM_DRAW);
	m_indexVBO->SubmitColor(mesh->colorList, GL_STREAM_DRAW);
	// 更换shader
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
	m_indexVBO->SetPointerPosition();
	m_indexVBO->SetPointerTexCoord();
	m_indexVBO->SetPointerColor();
	m_indexVBO->Draw(GL_TRIANGLES);
	ccGLBindTexture2D(0);

	FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(isAttribPositionOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(isAttribColorOn);
	FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(isAttribTexCoordOn);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_TailSprite::update(float dt)
{
	// 添加一个点
	CCPoint posCur=this->getPosition();
	if(m_footPrintList.empty())
	{
		m_footPrintList.push_back(FKCW_EffectNode_TailFootPrint(posCur,m_initialOpacity));
	}
	else
	{
		CCPoint posFoe=m_footPrintList[(int)m_footPrintList.size()-1].m_tagPos;
		if(ccpDistance(posCur, posFoe)>m_minDis)m_footPrintList.push_back(FKCW_EffectNode_TailFootPrint(posCur,m_initialOpacity));
	}
	// 消失
	int nFootPrint=(int)m_footPrintList.size();
	for(int i=0;i<nFootPrint;i++)
	{
		FKCW_EffectNode_TailFootPrint&footPrint=m_footPrintList[i];
		footPrint.m_fOpacity*=m_opacityDamping;
		if(footPrint.m_fOpacity<0.1)
		{
			m_footPrintList.erase(m_footPrintList.begin()+i);
			i--;
			nFootPrint--;
		}
	}
}
//-------------------------------------------------------------------------
FKCW_EffectNode_Mesh* FKCW_EffectNode_TailSprite::_GenerateMesh(const vector<FKCW_EffectNode_TailFootPrint>&footPrintList)
{
	FKCW_EffectNode_Mesh*mesh=new FKCW_EffectNode_Mesh();
	mesh->autorelease();
	int nFootPrint=(int)footPrintList.size();
	if(nFootPrint<2)
		return mesh;
	// 计算足迹列表中的每个片段的长度
	vector<float> lengthList;
	float totalLength=0;
	for(int i=0;i<nFootPrint-1;i++)
	{
		const CCPoint&start=footPrintList[i].m_tagPos;
		const CCPoint&end=footPrintList[i+1].m_tagPos;
		float length=ccpDistance(start, end);
		lengthList.push_back(length);
		totalLength+=length;
	}
	// 计算距离start点的距离
	vector<float> lengthFromStartList;
	float lengthFromStart=0;
	int nLen=(int)lengthList.size();
	for(int i=0;i<nLen;i++)
	{
		lengthFromStartList.push_back(lengthFromStart);
		lengthFromStart+=lengthList[i];
	}
	lengthFromStartList.push_back(lengthFromStart);

	// 计算 turnTypeList
	vector<eTurnType> turnTypeList;
	for(int i=0;i<nFootPrint;i++){
		if(i==0)
		{
			turnTypeList.push_back(eTurnType_unKnown);
		}
		else if(i==nFootPrint-1)
		{
			turnTypeList.push_back(eTurnType_unKnown);
		}
		else
		{
			CCPoint posf=footPrintList[i-1].m_tagPos;
			CCPoint pos=footPrintList[i].m_tagPos;
			CCPoint posn=footPrintList[i+1].m_tagPos;
			CCPoint vec=pos-posf;
			CCPoint vecn=posn-pos;
			float crossProduct=ccpCross(vec, vecn);
			if(crossProduct==0)
			{
				float dotProduct=ccpDot(vec, vecn);
				if(dotProduct>0)
				{// 并行并且同方向
					turnTypeList.push_back(eTurnType_straightForward);
				}
				else if(dotProduct<0)
				{// 并行并且反方向
					turnTypeList.push_back(eTurnType_straightBack);
				}
				else
				{
					assert(false);
				}
			}
			else if(crossProduct>0)
			{
				turnTypeList.push_back(eTurnType_left);
			}
			else
			{
				turnTypeList.push_back(eTurnType_right);
			}
		}
	}
	// 生成方格列表
	vector<FKCW_EffectNode_TailQuad> upQuadList;
	vector<FKCW_EffectNode_TailQuad> dnQuadList;
	float R=35;
	float curLength=0;
	for(int i=0;i<nFootPrint-1;i++)
	{
		float curLengthFoe=curLength;
		curLength+=lengthList[i];

		CCPoint start=footPrintList[i].m_tagPos;
		CCPoint end=footPrintList[i+1].m_tagPos;
		CCPoint dir=ccpNormalize(end-start);

		CCPoint normLeft=CCPoint(-dir.y,dir.x);
		CCPoint normRight=CCPoint(0,0)-normLeft;
		CCPoint LU=start+ccpMult(normLeft, R);
		CCPoint LD=start+ccpMult(normRight, R);
		CCPoint RU=end+ccpMult(normLeft, R);
		CCPoint RD=end+ccpMult(normRight, R);
		//  LU----RU
		//   |               |
		// start--end
		//   |               |
		//  LD----RD
		FKCW_EffectNode_TailQuad upQuad;
		upQuad.m_posLU=LU;
		upQuad.m_posLD=start;
		upQuad.m_posRU=RU;
		upQuad.m_posRD=end;
		upQuad.m_texCoordLU=CCPoint(curLengthFoe/totalLength,0);
		upQuad.m_texCoordLD=CCPoint(curLengthFoe/totalLength,0.5);
		upQuad.m_texCoordRU=CCPoint(curLength/totalLength,0);
		upQuad.m_texCoordRD=CCPoint(curLength/totalLength,0.5);
		upQuad.m_opacity=footPrintList[i].m_fOpacity;
		FKCW_EffectNode_TailQuad dnQuad;
		dnQuad.m_posLU=start;
		dnQuad.m_posLD=LD;
		dnQuad.m_posRU=end;
		dnQuad.m_posRD=RD;
		dnQuad.m_texCoordLU=CCPoint(curLengthFoe/totalLength,0.5);
		dnQuad.m_texCoordLD=CCPoint(curLengthFoe/totalLength,1);
		dnQuad.m_texCoordRU=CCPoint(curLength/totalLength,0.5);
		dnQuad.m_texCoordRD=CCPoint(curLength/totalLength,1);
		dnQuad.m_opacity=footPrintList[i].m_fOpacity;
		upQuadList.push_back(upQuad);
		dnQuadList.push_back(dnQuad);
	}
	// 调整 upQuadList 和 dnQuadList 中的矩形
	vector<FKCW_EffectNode_TailQuad> upQuadList_makeup;
	vector<int> insertIndexList_up;
	vector<FKCW_EffectNode_TailQuad> dnQuadList_makeup;
	vector<int> insertIndexList_dn;
	int nUpQuad=(int)upQuadList.size();
	int nDnQuad=(int)dnQuadList.size();
	assert(nUpQuad==nDnQuad);
	int nQuad=nUpQuad;
	for(int i=0;i<nQuad-1;i++)
	{
		FKCW_EffectNode_TailQuad&upQuad=upQuadList[i];
		FKCW_EffectNode_TailQuad&upQuadn=upQuadList[i+1];
		FKCW_EffectNode_TailQuad&dnQuad=dnQuadList[i];
		FKCW_EffectNode_TailQuad&dnQuadn=dnQuadList[i+1];
		eTurnType turnType=turnTypeList[i+1];
		CCPoint leftVec=ccpNormalize(upQuad.m_posLD-upQuad.m_posRD);
		CCPoint rightVec=ccpNormalize(upQuadn.m_posRD-upQuadn.m_posLD);
		bool isSharp=(ccpDot(leftVec, rightVec)>cosf(135*M_PI/180));
		if(turnType==eTurnType_left||turnType==eTurnType_straightBack)
		{
			// 需要在 dnQuad 和 dnQuadn 中插入
			vector<FKCW_EffectNode_TailQuad> quadList_makeup;
			CCPoint L1=dnQuad.m_posRD-dnQuad.m_posRU;
			CCPoint L2=dnQuadn.m_posLD-dnQuadn.m_posLU;
			vector<CCPoint> LList;//L1,...,L2
			LList.push_back(L1);
			const float dA=30;
			float A=0;
			while (1) 
			{
				A+=dA;
				CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
					L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
				if(ccpCross(L, L2)>0)
				{
					LList.push_back(L);
				}
				else
				{
					break;
				}
			}

			LList.push_back(L2);
			// 生成三角形表
			int nL=(int)LList.size();
			for(int j=0;j<nL-1;j++)
			{
				CCPoint L=LList[j];
				CCPoint Ln=LList[j+1];
				CCPoint P=dnQuad.m_posRU;
				FKCW_EffectNode_TailQuad quad;
				quad.m_posLU=P;
				quad.m_posLD=P+L;
				quad.m_posRU=P;
				quad.m_posRD=P+Ln;
				quad.m_opacity=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)*((j+1)/nL);
				quad.m_isTriangle=true;
				quad.m_opacityLU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
				quad.m_opacityRU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
				quadList_makeup.push_back(quad);
			}

			// 计算矩形列表中每个矩形的长度
			vector<float> lenList;
			int nQuadMakeUp=(int)quadList_makeup.size();
			for(int j=0;j<nQuadMakeUp;j++)
			{
				const FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				float len=ccpLength(quad.m_posLD-quad.m_posRD);
				lenList.push_back(len);
			}
			// 计算长度列表的长度和
			float d=0;
			int nLen=lenList.size();
			for(int j=0;j<nLen;j++)
			{
				d+=lenList[j];
			}
			float d1=ccpLength(dnQuad.m_posLU-dnQuad.m_posRU)/2;
			float d2=ccpLength(dnQuadn.m_posLU-dnQuadn.m_posRD)/2;
			//       /-------/\-------\
			//      /           d1/d\d2           \
			//     /---*---/--\---*---\
			//            sa  _sa  _sb   sb
			float sa=(dnQuad.m_texCoordLU.x+dnQuad.m_texCoordRU.x)/2;
			float sb=(dnQuadn.m_texCoordLU.x+dnQuadn.m_texCoordRU.x)/2;
			float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
			float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);

			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_texCoordLU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
				quad.m_texCoordRU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
				quad.m_texCoordLD=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,1);
				quad.m_texCoordRD=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,1);
			}
			// 计算透明度
			float opacityFirst=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)/(d1+d)*d1;
			float opacityLast=dnQuadn.m_opacity;
			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
			}
			for(int j=0;j<nQuadMakeUp;j++)
			{
				dnQuadList_makeup.push_back(quadList_makeup[j]);
				insertIndexList_dn.push_back(i);
			}
		}
		if(turnType==eTurnType_right||turnType==eTurnType_straightBack)
		{
			// 需要在 dnQuad 和 dnQuadn 中插入
			vector<FKCW_EffectNode_TailQuad> quadList_makeup;
			CCPoint L1=upQuad.m_posRU-upQuad.m_posRD;
			CCPoint L2=upQuadn.m_posLU-upQuadn.m_posLD;
			vector<CCPoint> LList;//L1,...,L2
			LList.push_back(L1);
			const float dA=-30;
			float A=0;
			while (1) 
			{
				A+=dA;
				CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
					L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
				if(ccpCross(L, L2)<0)
				{
					LList.push_back(L);
				}
				else
				{
					break;
				}
			}
			LList.push_back(L2);
			
			int nL=(int)LList.size();
			for(int j=0;j<nL-1;j++){
				CCPoint L=LList[j];
				CCPoint Ln=LList[j+1];
				CCPoint P=upQuad.m_posRD;
				FKCW_EffectNode_TailQuad quad;
				quad.m_posLU=P+L;
				quad.m_posLD=P;
				quad.m_posRU=P+Ln;
				quad.m_posRD=P;
				quad.m_opacity=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)*((j+1)/nL);
				quad.m_isTriangle=true;
				quad.m_opacityLD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
				quad.m_opacityRD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
				quadList_makeup.push_back(quad);
			}
			vector<float> lenList;
			int nQuadMakeUp=(int)quadList_makeup.size();
			for(int j=0;j<nQuadMakeUp;j++)
			{
				const FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				float len=ccpLength(quad.m_posLU-quad.m_posRU);
				lenList.push_back(len);
			}

			float d=0;
			int nLen=lenList.size();
			for(int j=0;j<nLen;j++)
			{
				d+=lenList[j];
			}
			float d1=ccpLength(upQuad.m_posLD-upQuad.m_posRD)/2;
			float d2=ccpLength(upQuadn.m_posLD-upQuadn.m_posRD)/2;
			//           sa   _sa  _sb  sb
			//   \---*---\-- /---*---/
			//    \          d1  \  / d2           /
			//     \-------\/-------/
			float sa=(upQuad.m_texCoordLD.x+upQuad.m_texCoordRD.x)/2;
			float sb=(upQuadn.m_texCoordLD.x+upQuadn.m_texCoordRD.x)/2;
			float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
			float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);

			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_texCoordLU=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,0);
				quad.m_texCoordRU=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,0);
				quad.m_texCoordLD=CCPoint(upQuad.m_texCoordRD.x,0.5);
				quad.m_texCoordRD=CCPoint(upQuad.m_texCoordRD.x,0.5);
			}
			// 计算透明度
			float opacityFirst=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)/(d1+d)*d1;
			float opacityLast=upQuadn.m_opacity;
			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
			}
			for(int j=0;j<nQuadMakeUp;j++)
			{
				upQuadList_makeup.push_back(quadList_makeup[j]);
				insertIndexList_up.push_back(i);
			}
		}
		if(turnType==eTurnType_left&&isSharp==false)
		{
			CCPoint upMid=ccpMidpoint(upQuad.m_posRU, upQuadn.m_posLU);
			upQuad.m_posRU=upMid;
			upQuadn.m_posLU=upMid;
		}
		if(turnType==eTurnType_right&&isSharp==false){
			CCPoint dnMid=ccpMidpoint(dnQuad.m_posRD, dnQuadn.m_posLD);
			dnQuad.m_posRD=dnMid;
			dnQuadn.m_posLD=dnMid;
		}

		// 若转角大约135度，为尖锐转角
		if(turnType==eTurnType_right&&isSharp)
		{
			// 需要在 dnQuad 和 dnQuadn 中插入
			vector<FKCW_EffectNode_TailQuad> quadList_makeup;
			CCPoint L1=dnQuad.m_posRD-dnQuad.m_posRU;
			CCPoint L2=dnQuadn.m_posLD-dnQuadn.m_posLU;
			vector<CCPoint> LList;//L1,...,L2
			LList.push_back(L1);
			const float dA=30;
			float A=0;
			while (1)
			{
				A+=dA;
				CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
					L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
				if(A<=180||ccpCross(L, L2)>0)
				{
						LList.push_back(L);
				}
				else
				{
					break;
				}
			}

			LList.push_back(L2);

			int nL=(int)LList.size();
			for(int j=0;j<nL-1;j++){
				CCPoint L=LList[j];
				CCPoint Ln=LList[j+1];
				CCPoint P=dnQuad.m_posRU;
				FKCW_EffectNode_TailQuad quad;
				quad.m_posLU=P;
				quad.m_posLD=P+L;
				quad.m_posRU=P;
				quad.m_posRD=P+Ln;
				quad.m_opacity=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)*((j+1)/nL);
				quad.m_isTriangle=true;
				quad.m_opacityLU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
				quad.m_opacityRU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
				quadList_makeup.push_back(quad);
			}
			vector<float> lenList;
			int nQuadMakeUp=(int)quadList_makeup.size();
			for(int j=0;j<nQuadMakeUp;j++){
				const FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				float len=ccpLength(quad.m_posLD-quad.m_posRD);
				lenList.push_back(len);
			}
			float d=0;
			int nLen=lenList.size();
			for(int j=0;j<nLen;j++)
			{
				d+=lenList[j];
			}
			float d1=ccpLength(dnQuad.m_posLU-dnQuad.m_posRU)/2;
			float d2=ccpLength(dnQuadn.m_posLU-dnQuadn.m_posRD)/2;
			//       /-------/\-------\
			//      /           d1/d\d2           \
			//     /---*---/--\---*---\
			//            sa  _sa  _sb   sb
			float sa=(dnQuad.m_texCoordLU.x+dnQuad.m_texCoordRU.x)/2;
			float sb=(dnQuadn.m_texCoordLU.x+dnQuadn.m_texCoordRU.x)/2;
			float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
			float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);

			for(int j=0;j<nQuadMakeUp;j++){
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_texCoordLU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
				quad.m_texCoordRU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
				quad.m_texCoordLD=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,1);
				quad.m_texCoordRD=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,1);

			}
			// 计算alpha 
			float opacityFirst=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)/(d1+d)*d1;
			float opacityLast=dnQuadn.m_opacity;
			for(int j=0;j<nQuadMakeUp;j++){
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
			}

			for(int j=0;j<nQuadMakeUp;j++)
			{
				dnQuadList_makeup.push_back(quadList_makeup[j]);
				insertIndexList_dn.push_back(i);
			}
		}


		if(turnType==eTurnType_left&&isSharp)
		{
			vector<FKCW_EffectNode_TailQuad> quadList_makeup;
			CCPoint L1=upQuad.m_posRU-upQuad.m_posRD;
			CCPoint L2=upQuadn.m_posLU-upQuadn.m_posLD;
			vector<CCPoint> LList;//L1,...,L2
			LList.push_back(L1);
			const float dA=-30;
			float A=0;
			while (1) 
			{
				A+=dA;
				CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
					L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
				if(A>=-180||ccpCross(L, L2)<0)
				{
					LList.push_back(L);
				}
				else
				{
					break;
				}
			}
			LList.push_back(L2);

			int nL=(int)LList.size();
			for(int j=0;j<nL-1;j++)
			{
				CCPoint L=LList[j];
				CCPoint Ln=LList[j+1];
				CCPoint P=upQuad.m_posRD;
				FKCW_EffectNode_TailQuad quad;
				quad.m_posLU=P+L;
				quad.m_posLD=P;
				quad.m_posRU=P+Ln;
				quad.m_posRD=P;
				quad.m_opacity=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)*((j+1)/nL);
				quad.m_isTriangle=true;
				quad.m_opacityLD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
				quad.m_opacityRD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
				quadList_makeup.push_back(quad);
			}
			vector<float> lenList;
			int nQuadMakeUp=(int)quadList_makeup.size();
			for(int j=0;j<nQuadMakeUp;j++)
			{
				const FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				float len=ccpLength(quad.m_posLU-quad.m_posRU);
				lenList.push_back(len);
			}
			float d=0;
			int nLen=lenList.size();
			for(int j=0;j<nLen;j++)
			{
				d+=lenList[j];
			}
			float d1=ccpLength(upQuad.m_posLD-upQuad.m_posRD)/2;
			float d2=ccpLength(upQuadn.m_posLD-upQuadn.m_posRD)/2;
			//           sa   _sa  _sb  sb
			//   \---*---\-- /---*---/
			//    \          d1  \  / d2           /
			//     \-------\/-------/
			float sa=(upQuad.m_texCoordLD.x+upQuad.m_texCoordRD.x)/2;
			float sb=(upQuadn.m_texCoordLD.x+upQuadn.m_texCoordRD.x)/2;
			float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
			float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);

			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_texCoordLU=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,0);
				quad.m_texCoordRU=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,0);
				quad.m_texCoordLD=CCPoint(upQuad.m_texCoordRD.x,0.5);
				quad.m_texCoordRD=CCPoint(upQuad.m_texCoordRD.x,0.5);

			}

			float opacityFirst=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)/(d1+d)*d1;
			float opacityLast=upQuadn.m_opacity;
			for(int j=0;j<nQuadMakeUp;j++)
			{
				FKCW_EffectNode_TailQuad&quad=quadList_makeup[j];
				quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
			}
			for(int j=0;j<nQuadMakeUp;j++)
			{
				upQuadList_makeup.push_back(quadList_makeup[j]);
				insertIndexList_up.push_back(i);
			}
		}
	}

	// 计算全部方格矩阵
	vector<vector<FKCW_EffectNode_TailQuad> > allQuadMat;
	{
		allQuadMat.resize(2);
		vector<FKCW_EffectNode_TailQuad>&allUpQuadList=allQuadMat[0];
		vector<FKCW_EffectNode_TailQuad>&allDnQuadList=allQuadMat[1];
		// 全部上方方格
		{
			int i_insertIndexList=0;
			int nUpQuad=(int)upQuadList.size();
			for(int i=0;i<nUpQuad;i++)
			{
				const FKCW_EffectNode_TailQuad&quad=upQuadList[i];
				allUpQuadList.push_back(quad);
				while(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]<i)
				{
					i_insertIndexList++;
					if(i_insertIndexList>=(int)insertIndexList_up.size())break;
				}
				if(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]==i)
				{
					while(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]==i)
					{
						int iUpQuad_makeUp=i_insertIndexList;
						allUpQuadList.push_back(upQuadList_makeup[iUpQuad_makeUp]);
						i_insertIndexList++;
						if(i_insertIndexList>=(int)insertIndexList_up.size())break;
					}
				}else if(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]>i)
				{
				}
			}
		}
		// 全部下方方格
		{
			int i_insertIndexList=0;
			int nDnQuad=(int)dnQuadList.size();
			for(int i=0;i<nDnQuad;i++)
			{
				const FKCW_EffectNode_TailQuad&quad=dnQuadList[i];
				allDnQuadList.push_back(quad);
				while(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]<i)
				{
					i_insertIndexList++;
					if(i_insertIndexList>=(int)insertIndexList_dn.size())break;
				}
				if(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]==i)
				{
					while(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]==i)
					{
						int iDnQuad_makeUp=i_insertIndexList;
						allDnQuadList.push_back(dnQuadList_makeup[iDnQuad_makeUp]);
						i_insertIndexList++;
						if(i_insertIndexList>=(int)insertIndexList_dn.size())break;
					}
				}else if(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]>i)
				{
				}
			}
		}
	}
	// 为全部方格计算 opacityLU,opacityLD,opacityRU,opacityRD
	{
		int nRow=(int)allQuadMat.size();
		for(int i=0;i<nRow;i++)
		{
			vector<FKCW_EffectNode_TailQuad>&quadList=allQuadMat[i];
			int nQuad=(int)quadList.size();
			if(nQuad==0)
			{
			}else if(nQuad==1)
			{
				FKCW_EffectNode_TailQuad&quad=quadList[0];
				if(quad.m_opacityLU==INFINITY)
					quad.m_opacityLU=quad.m_opacity;
				if(quad.m_opacityLD==INFINITY)
					quad.m_opacityLD=quad.m_opacity;
				if(quad.m_opacityRU==INFINITY)
					quad.m_opacityRU=quad.m_opacity;
				if(quad.m_opacityRD==INFINITY)
					quad.m_opacityRD=quad.m_opacity;
			}
			else
			{//nQuad>=2
				for(int j=0;j<nQuad;j++)
				{
					FKCW_EffectNode_TailQuad&quad=quadList[j];
					if(j==nQuad-1)
					{// 最后那个方格
						if(quad.m_opacityLU==INFINITY)
							quad.m_opacityLU=quad.m_opacity;
						if(quad.m_opacityLD==INFINITY)
							quad.m_opacityLD=quad.m_opacity;
						if(quad.m_opacityRU==INFINITY)
							quad.m_opacityRU=quad.m_opacity;
						if(quad.m_opacityRD==INFINITY)
							quad.m_opacityRD=quad.m_opacity;
					}
					else
					{
						const FKCW_EffectNode_TailQuad&quadn=quadList[j+1];
						if(quad.m_opacityLU==INFINITY)
							quad.m_opacityLU=quad.m_opacity;
						if(quad.m_opacityLD==INFINITY)
							quad.m_opacityLD=quad.m_opacity;
						if(quad.m_opacityRU==INFINITY)
							quad.m_opacityRU=quadn.m_opacity;
						if(quad.m_opacityRD==INFINITY)
							quad.m_opacityRD=quadn.m_opacity;
					}
				}
			}
		}
	}
	// 将全部方格转换为网格
	vector<Cv2> vlist;
	vector<CIDTriangle> IDtriList;
	vector<Cv2> texCoordList;
	vector<Cv4> colorList;
	int nRow=(int)allQuadMat.size();
	for(int i=0;i<nRow;i++)
	{
		const vector<FKCW_EffectNode_TailQuad>&quadList=allQuadMat[i];
		int nQuad=(int)quadList.size();
		for(int j=0;j<nQuad;j++)
		{
			const FKCW_EffectNode_TailQuad&quad=quadList[j];
			// 四边形是否自相交
			float PND1=FKCW_Base_Utils::PointDisToLine(quad.m_posRU, quad.m_posLU, quad.m_posLD);
			float PND2=FKCW_Base_Utils::PointDisToLine(quad.m_posRD, quad.m_posLU, quad.m_posLD);
			if(PND1*PND2<0&&quad.m_isTriangle==false)
			{   // 自相交
				//    LU --- RU
				//       |/
				//       * P
				//      /|
				//  RD --- LD

				// 将矩形分割为三角形(P,RD,LD)(P,RU,LU) 
				// 计算P的位置，P的纹理UV，P的透明度
				CCPoint P,texCoordP;
				float opacityP;
				{
					P=FKCW_Base_Utils::GetIntersectPointOfTwoLine(quad.m_posLU,quad.m_posLD-quad.m_posLU,
						quad.m_posRU,quad.m_posRD-quad.m_posRU);
					float dis_PtoUp=fabsf(FKCW_Base_Utils::PointDisToLine(P, quad.m_posLU, quad.m_posRU));
					float dis_PtoDn=fabsf(FKCW_Base_Utils::PointDisToLine(P, quad.m_posRD, quad.m_posLD));
					texCoordP=CCPoint((quad.m_texCoordLU.x+quad.m_texCoordRU.x)/2,quad.m_texCoordLU.y+0.5*dis_PtoUp/(dis_PtoUp+dis_PtoDn));
					opacityP=quad.m_opacity;
				}

				//转换为网格
				vlist.push_back(Cv2(quad.m_posLU.x,quad.m_posLU.y));
				texCoordList.push_back(Cv2(quad.m_texCoordLU.x,quad.m_texCoordLU.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityLU));
				int vID_LU=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posLD.x,quad.m_posLD.y));
				texCoordList.push_back(Cv2(quad.m_texCoordLD.x,quad.m_texCoordLD.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityLD));
				int vID_LD=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posRD.x,quad.m_posRD.y));
				texCoordList.push_back(Cv2(quad.m_texCoordRD.x,quad.m_texCoordRD.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityRD));
				int vID_RD=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posRU.x,quad.m_posRU.y));
				texCoordList.push_back(Cv2(quad.m_texCoordRU.x,quad.m_texCoordRU.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityRU));
				int vID_RU=(int)vlist.size()-1;
				vlist.push_back(Cv2(P.x,P.y));
				texCoordList.push_back(Cv2(texCoordP.x,texCoordP.y));
				colorList.push_back(Cv4(1,1,1,opacityP));
				int vID_P=(int)vlist.size()-1;

				if(i==1)
				{
					// 若方格是下部方格，则仅保留上部三角形
					IDtriList.push_back(CIDTriangle(vID_P,vID_RU,vID_LU));
				}
				if(i==0)
				{
					// 若方格是上部方格，则仅保留下部三角形
					IDtriList.push_back(CIDTriangle(vID_P,vID_RD,vID_LD));
				}
			}
			else
			{ // 若不自交
				// 转换为网格
				vlist.push_back(Cv2(quad.m_posLU.x,quad.m_posLU.y));
				texCoordList.push_back(Cv2(quad.m_texCoordLU.x,quad.m_texCoordLU.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityLU));
				int vID_LU=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posLD.x,quad.m_posLD.y));
				texCoordList.push_back(Cv2(quad.m_texCoordLD.x,quad.m_texCoordLD.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityLD));
				int vID_LD=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posRD.x,quad.m_posRD.y));
				texCoordList.push_back(Cv2(quad.m_texCoordRD.x,quad.m_texCoordRD.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityRD));
				int vID_RD=(int)vlist.size()-1;
				vlist.push_back(Cv2(quad.m_posRU.x,quad.m_posRU.y));
				texCoordList.push_back(Cv2(quad.m_texCoordRU.x,quad.m_texCoordRU.y));
				colorList.push_back(Cv4(1,1,1,quad.m_opacityRU));
				int vID_RU=(int)vlist.size()-1;

				IDtriList.push_back(CIDTriangle(vID_LU,vID_LD,vID_RD));
				IDtriList.push_back(CIDTriangle(vID_RD,vID_RU,vID_LU));
			}
		}
	}
	// 填充网格
	mesh->vlist=vlist;
	mesh->IDtriList=IDtriList;
	mesh->texCoordList=texCoordList;
	mesh->colorList=colorList;
	// 绘制线
	if(m_isShowWire)
	{
		glLineWidth(1);
		int nRow=(int)allQuadMat.size();
		for(int i=0;i<nRow;i++)
		{
			const vector<FKCW_EffectNode_TailQuad>&quadList=allQuadMat[i];
			int nQuad=(int)quadList.size();
			for(int j=0;j<nQuad;j++)
			{
				const FKCW_EffectNode_TailQuad&quad=quadList[j];
				ccDrawLine(quad.m_posLU,quad.m_posLD);
				ccDrawLine(quad.m_posLD,quad.m_posRD);
				ccDrawLine(quad.m_posRD,quad.m_posRU);
				ccDrawLine(quad.m_posRU,quad.m_posLU);
			}
		}

	}
	return mesh;
}
//-------------------------------------------------------------------------