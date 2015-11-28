//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_CatmullRomSprite.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
#pragma warning( disable: 4244 )
//-------------------------------------------------------------------------
// 更新纹理集
void FKCW_RenderEx_CatmullRomSprite::__UpdateAtlas()
{
	// 计算各点信息
	__PopulatePoints(m_tagControlPoints, m_tagPoints);

	// 清理纹理数据
	m_pAtlas->removeAllQuads();
	m_vecSegmentQuadIndices.clear();

	// 最少有俩个点
	int pc = m_tagPoints.GetCount();
	if(pc < 2)
		return;

	// 添加一个点，不然将丢失最后一个片段
	CCPoint pLast0 = m_tagPoints.GetPointAt(pc - 1);
	CCPoint pLast1 = m_tagPoints.GetPointAt(pc - 2);
	CCPoint pAppend = ccpAdd(pLast0, ccpSub(pLast0, pLast1));
	m_tagPoints.AddPoint(pAppend);
	pc++;

	// 前两个点
	CCPoint p0 = m_tagPoints.GetPointAt(0);
	CCPoint p1 = m_tagPoints.GetPointAt(1);

	float halfWidth = m_fPatternWidth / 2;
	CCPoint bl, tl;
	{
		CCPoint v01 = ccpSub(p1, p0);
		float r01 = ccpToAngle(v01);
		bl.x = p0.x + halfWidth * sinf(r01);
		bl.y = p0.y - halfWidth * cosf(r01);
		tl.x = p0.x - halfWidth * sinf(r01);
		tl.y = p0.y + halfWidth * cosf(r01);
	}

	// 当前长度
	float texStartP = 0;
	float texEndP;
	float headPos = 0;

	// 计算矩阵
	int segIndex = 0;
	for(int i = 2; i < pc; i++) 
	{
		// 保存矩阵索引
		if(m_vecSegmentPointIndices[segIndex] == i - 2) 
		{
			m_vecSegmentQuadIndices.push_back(m_pAtlas->getTotalQuads());
			segIndex++;
		}

		// 第三个点
		CCPoint p2 = m_tagPoints.GetPointAt(i);

		// 3 vectors, 从0到1，从1到2，从2到0
		CCPoint v01 = ccpSub(p1, p0);
		CCPoint v12 = ccpSub(p2, p1);
		CCPoint v10 = ccpSub(p0, p1);

		// 在 v01 和 v12 中心点，再旋转90度
		float r = (ccpToAngle(v01) + ccpToAngle(v12)) / 2 - M_PI_2;

		// 中心分割点向量
		CCPoint m = ccp(cosf(r), sinf(r));

		// 中心点 到 v10 的角度
		float rm01 = ccpToAngle(m) - ccpToAngle(v10);

		// 关节点的实际基本宽度，但是我们要注意防止该宽度过大
		float s = sinf(rm01);
		float w = MAX_FLOAT;
		if(s != 0)
			w = fabsf(halfWidth / s);
		w = MIN(halfWidth * 2, w);

		// 一种特殊情况是当v01在第三象限，但v12在第四象限
		if(v01.x < 0 && SIGN(v01.x) == SIGN(v12.x) && SIGN(v01.y) != SIGN(v12.y)) 
		{
			r = M_PI + r;
		}

		// 计算tl和tr
		CCPoint br, tr;
		br.x = p1.x + w * cosf(r);
		br.y = p1.y + w * sinf(r);
		tr.x = p1.x - w * cosf(r);
		tr.y = p1.y - w * sinf(r);

		// 计算纹理百分比值
		float segLen = ccpLength(v01);
		float remainLen = segLen;
		float initVerP = 0;
		float stepVerP = m_fPatternLength / segLen;
		while(remainLen > m_fPatternLength) 
		{
			texEndP = texStartP;
			float p = (1 - texStartP) / (1 - texStartP + texEndP);
			__PopulateQuad(bl, br, tl, tr, texStartP, 1, initVerP, initVerP + stepVerP * p);
			__PopulateQuad(bl, br, tl, tr, 0, texEndP, initVerP + stepVerP * p, initVerP + stepVerP);
			initVerP += stepVerP;

			// 减去使用的纹理长度
			remainLen -= m_fPatternLength;
		}

		// 依然保留的纹理长度
		headPos += segLen;
		headPos = fmodf(headPos, m_fPatternLength);
		texEndP = headPos / m_fPatternLength;
		stepVerP = remainLen / segLen;
		if(texEndP <= texStartP) 
		{
			float p = (1 - texStartP) / (1 - texStartP + texEndP);
			__PopulateQuad(bl, br, tl, tr, texStartP, 1, initVerP, initVerP + stepVerP * p);
			__PopulateQuad(bl, br, tl, tr, 0, texEndP, initVerP + stepVerP * p, initVerP + stepVerP);
		} 
		else 
		{
			__PopulateQuad(bl, br, tl, tr, texStartP, texEndP, initVerP, 1);
		}

		// 向前移
		p0 = p1;
		p1 = p2;
		bl = br;
		tl = tr;
		texStartP = texEndP;
	}
}
//-------------------------------------------------------------------------
// 通过控制点信息，计算全部点信息
void FKCW_RenderEx_CatmullRomSprite::__PopulatePoints( FKCW_Base_PointList& controlPoints, FKCW_Base_PointList& points)
{
	// 清理数据
	points.Clear();
	m_vecSegmentPointIndices.clear();

	// 逐片段计算顶点
	int totalSeg = controlPoints.GetCount() - 1;
	for(int curSeg = 0; curSeg < totalSeg; curSeg++)
	{
		// 添加片段起始索引
		m_vecSegmentPointIndices.push_back(points.GetCount());

		// 四个控制点
		CCPoint cp0 = controlPoints.GetPointAt(curSeg - 1);
		CCPoint cp1 = controlPoints.GetPointAt(curSeg);
		CCPoint cp2 = controlPoints.GetPointAt(curSeg + 1);
		CCPoint cp3 = controlPoints.GetPointAt(curSeg + 2);

		// 片段长度 ，期望点
		float curSegLen = ccpLength(ccpSub(cp2, cp1));
		int curSegPoints = (int)ceilf(curSegLen / m_fPatternLength) + 1;
		float step = 1.0f / (curSegPoints - 1);

		// 为这个片段计算顶点
		float lt = 0;
		while(lt < 1) 
		{
			CCPoint pos = ccCardinalSplineAt(cp0, cp1, cp2, cp3, m_fTension, lt);
			points.AddPoint(pos);
			lt += step;
		}
	}

	// 最后一个点
	points.AddPoint(controlPoints.GetPointAt(controlPoints.GetCount() - 1));

	// 最后一个占位符
	m_vecSegmentPointIndices.push_back(points.GetCount());
}
//-------------------------------------------------------------------------
// 计算一个矩形
void FKCW_RenderEx_CatmullRomSprite::__PopulateQuad(const CCPoint& bl, const CCPoint& br, const CCPoint& tl, const CCPoint& tr,
									   float texStartP, float texEndP, float verStartP, float verEndP)
{
	ccV3F_C4B_T2F_Quad quad = m_pSprite->getQuad();

	// 差值这些顶点
	quad.bl.vertices.x = FKCW_Base_Utils::Lerp(bl.x, br.x, verStartP);
	quad.bl.vertices.y = FKCW_Base_Utils::Lerp(bl.y, br.y, verStartP);
	quad.br.vertices.x = FKCW_Base_Utils::Lerp(bl.x, br.x, verEndP);
	quad.br.vertices.y = FKCW_Base_Utils::Lerp(bl.y, br.y, verEndP);
	quad.tl.vertices.x = FKCW_Base_Utils::Lerp(tl.x, tr.x, verStartP);
	quad.tl.vertices.y = FKCW_Base_Utils::Lerp(tl.y, tr.y, verStartP);
	quad.tr.vertices.x = FKCW_Base_Utils::Lerp(tl.x, tr.x, verEndP);
	quad.tr.vertices.y = FKCW_Base_Utils::Lerp(tl.y, tr.y, verEndP);

	// 保存纹理， bl 和 tl
	if(m_pSprite->isTextureRectRotated()) 
	{
		float blv = quad.bl.texCoords.v;
		float brv = quad.br.texCoords.v;
		float tlv = quad.tl.texCoords.v;
		float trv = quad.tr.texCoords.v;
		quad.bl.texCoords.v = FKCW_Base_Utils::Lerp(blv, brv, texStartP);
		quad.tl.texCoords.v = FKCW_Base_Utils::Lerp(tlv, trv, texStartP);
		quad.br.texCoords.v = FKCW_Base_Utils::Lerp(blv, brv, texEndP);
		quad.tr.texCoords.v = FKCW_Base_Utils::Lerp(tlv, trv, texEndP);
	} 
	else 
	{
		float blu = quad.bl.texCoords.u;
		float bru = quad.br.texCoords.u;
		float tlu = quad.tl.texCoords.u;
		float tru = quad.tr.texCoords.u;
		quad.bl.texCoords.u = FKCW_Base_Utils::Lerp(blu, bru, texStartP);
		quad.tl.texCoords.u = FKCW_Base_Utils::Lerp(tlu, tru, texStartP);
		quad.br.texCoords.u = FKCW_Base_Utils::Lerp(blu, bru, texEndP);
		quad.tr.texCoords.u = FKCW_Base_Utils::Lerp(tlu, tru, texEndP);
	}

	// 添加矩形
	while(m_pAtlas->getTotalQuads() >= m_pAtlas->getCapacity())
		m_pAtlas->resizeCapacity(m_pAtlas->getCapacity() * 2);

	m_pAtlas->insertQuad(&quad, m_pAtlas->getTotalQuads());
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite::FKCW_RenderEx_CatmullRomSprite(CCSprite* sprite)
	: m_pSprite( NULL )
	, m_bIsDirty( false )
	, m_bIsAllVisible( true )
	, m_fTension( 0.1f )
	, m_pAtlas( NULL )
{
	CCAssert(sprite != NULL, "FKCW_RenderEx_CatmullRomSprite doesn't accept NULL sprite");

	// 保存该精灵
	m_pSprite = sprite;
	CC_SAFE_RETAIN(m_pSprite);

	// 获取纹理集
	m_pAtlas = CCTextureAtlas::createWithTexture(m_pSprite->getTexture(), kDefaultSpriteBatchCapacity);
	CC_SAFE_RETAIN(m_pAtlas);

	// shader program
	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));

	// 修改大小
	setContentSize(CCSizeZero);

	// 更新其他一些简要信息
	m_fPatternLength = m_pSprite->getContentSize().width;
	m_fPatternWidth = m_pSprite->getContentSize().height;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite::~FKCW_RenderEx_CatmullRomSprite()
{
	CC_SAFE_RELEASE(m_pSprite);
	CC_SAFE_RELEASE(m_pAtlas);
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite* FKCW_RenderEx_CatmullRomSprite::create(const char* fileImage)
{
	return FKCW_RenderEx_CatmullRomSprite::createWithSprite(CCSprite::create(fileImage));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite* FKCW_RenderEx_CatmullRomSprite::createWithTexture(CCTexture2D* tex)
{
	return FKCW_RenderEx_CatmullRomSprite::createWithSprite(CCSprite::createWithTexture(tex));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite* FKCW_RenderEx_CatmullRomSprite::createWithSpriteFrameName(const char* name)
{
	return createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite* FKCW_RenderEx_CatmullRomSprite::createWithSpriteFrame(CCSpriteFrame* frame)
{
	return FKCW_RenderEx_CatmullRomSprite::createWithSprite(CCSprite::createWithSpriteFrame(frame));
}
//-------------------------------------------------------------------------
FKCW_RenderEx_CatmullRomSprite* FKCW_RenderEx_CatmullRomSprite::createWithSprite(CCSprite* sprite)
{
	FKCW_RenderEx_CatmullRomSprite* s = new FKCW_RenderEx_CatmullRomSprite(sprite);
	return (FKCW_RenderEx_CatmullRomSprite*)s->autorelease();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_CatmullRomSprite::setOpacity(GLubyte opacity)
{
	m_pSprite->setOpacity(opacity);
	m_bIsDirty = true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_CatmullRomSprite::draw()
{
	// 最少需要两个控制点
	if(m_tagControlPoints.GetCount() < 2)
		return;

	// 处理纹理
	if(m_bIsDirty)
	{
		__UpdateAtlas();
		m_bIsDirty = false;
	}

	// 开始处理
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "FKCW_RenderEx_CatmullRomSprite - draw");

	CC_NODE_DRAW_SETUP();

	// 混合模式
	ccBlendFunc bf = m_pSprite->getBlendFunc();
	ccGLBlendFunc(bf.src, bf.dst);

	// 绘制
	if(m_pAtlas) 
	{
		if(m_bIsAllVisible) 
		{
			m_pAtlas->drawQuads();
		} 
		else 
		{
			int startIndex = 0;
			int sc = GetSegmentCount();
			for(int i = 0; i < sc; i++)
			{
				bool visible = IsSegmentVisible(i);
				if(!visible) 
				{
					int endIndex = m_vecSegmentQuadIndices[i];
					m_pAtlas->drawNumberOfQuads(endIndex - startIndex, startIndex);
					startIndex = m_vecSegmentQuadIndices[i + 1];
				}
			}

			// 最后
			if( m_pAtlas->getTotalQuads() > static_cast<unsigned int>(startIndex)) 
			{
				m_pAtlas->drawNumberOfQuads(m_pAtlas->getTotalQuads() - startIndex, startIndex);
			}
		}
	}

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "FKCW_RenderEx_CatmullRomSprite - draw");
}
//-------------------------------------------------------------------------
// 添加一个控制点
// 注：该操作将导致本精灵会被重绘
void FKCW_RenderEx_CatmullRomSprite::AddControlPoint( const CCPoint& p )
{
	m_tagControlPoints.AddPoint(p.x, p.y);
	m_bIsDirty = true;
}
//-------------------------------------------------------------------------
// 设置一个新的控制点队列
// 注：该操作将导致本精灵会被重绘
void FKCW_RenderEx_CatmullRomSprite::SetControlPoints( CCPointArray& pa )
{
	m_tagControlPoints.Clear();
	const vector<CCPoint*>* plist = pa.getControlPoints();
	for(vector<CCPoint*>::const_iterator iter = plist->begin(); iter != plist->end(); iter++) 
	{
		m_tagControlPoints.AddPoint((*iter)->x, (*iter)->y);
	}
	m_bIsDirty = true;
}
//-------------------------------------------------------------------------
// 设置指定片段是否可见
void FKCW_RenderEx_CatmullRomSprite::SetSegmentVisible(int sIndex, bool visible)
{
	if(!visible) 
	{
		m_mapSegmentVisibilities[sIndex] = visible;
		m_bIsAllVisible = false;
	} 
	else 
	{
		map<int, bool>::iterator iter = m_mapSegmentVisibilities.find(sIndex);
		if(iter != m_mapSegmentVisibilities.end())
		{
			m_mapSegmentVisibilities.erase(iter);
		}
		m_bIsAllVisible = m_mapSegmentVisibilities.empty();
	}
}
//-------------------------------------------------------------------------
// 判断指定片段是否可见
bool FKCW_RenderEx_CatmullRomSprite::IsSegmentVisible(int sIndex)
{
	map<int, bool>::iterator iter = m_mapSegmentVisibilities.find(sIndex);
	if(iter != m_mapSegmentVisibilities.end())
		return iter->second;
	else
		return true;
}
//-------------------------------------------------------------------------
// 设置全部片段可见
void FKCW_RenderEx_CatmullRomSprite::ResetSegmentVisibility()
{
	m_mapSegmentVisibilities.clear();
	m_bIsAllVisible = true;
}
//-------------------------------------------------------------------------
// 设置曲线张力（该值越大，曲线弧度越大）
void FKCW_RenderEx_CatmullRomSprite::SetTension(float t)
{
	m_fTension = t;
	m_bIsDirty = true;
}
//-------------------------------------------------------------------------