//*************************************************************************
//	创建日期:	2014-11-6
//	文件名称:	FKCW_RenderEx_CatmullRomSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
// 笛卡尔差值扭曲精灵
// 该精灵可以将一个node的图片进行任意扭曲，扭曲方式可以通过一系列的控制点控制
// 注：因为它是一个曲线，所以其contextSize为0.但是，你可以通过获取其控制点列表来计算起大小
// 注：如果使用纹理集，应当将纹理增加一个额外像素。TexturePacker支持该拉伸(Extrude)选项。
class FKCW_RenderEx_CatmullRomSprite : public CCNodeRGBA, public CCBlendProtocol 
{
private:
	bool					m_bIsDirty;					// 脏标示
	CCTextureAtlas*			m_pAtlas;					// 纹理集
	FKCW_Base_PointList		m_tagControlPoints;			// 曲线参考点
	FKCW_Base_PointList		m_tagPoints;				// 曲线顶点
	float					m_fTension;					// 曲线张力
	float					m_fPatternLength;			// 图案长
	float					m_fPatternWidth;			// 图案宽
	vector<int>				m_vecSegmentPointIndices;	// 片段顶点索引
	vector<int>				m_vecSegmentQuadIndices;	// 片段UV值索引
	map<int,bool>			m_mapSegmentVisibilities;	// 片段可视标示列表
	bool					m_bIsAllVisible;			// 全片段可视性
	CC_SYNTHESIZE_READONLY(CCSprite*, m_pSprite, PatternSprite);	// 内部精灵组
public:
	// 更新纹理集
	void					__UpdateAtlas();
	// 通过控制点信息，计算全部点信息
	void					__PopulatePoints( FKCW_Base_PointList& controlPoints, FKCW_Base_PointList& points);
	// 计算一个矩形
	void					__PopulateQuad(const CCPoint& bl, const CCPoint& br, const CCPoint& tl, const CCPoint& tr,
		float texStartP, float texEndP, float verStartP, float verEndP);
public:
	FKCW_RenderEx_CatmullRomSprite(CCSprite* sprite);
	virtual ~FKCW_RenderEx_CatmullRomSprite();

	static FKCW_RenderEx_CatmullRomSprite* create(const char* fileImage);
	static FKCW_RenderEx_CatmullRomSprite* createWithTexture(CCTexture2D* tex);
	static FKCW_RenderEx_CatmullRomSprite* createWithSpriteFrameName(const char* name);
	static FKCW_RenderEx_CatmullRomSprite* createWithSpriteFrame(CCSpriteFrame* frame);
	static FKCW_RenderEx_CatmullRomSprite* createWithSprite(CCSprite* sprite);

	// 重载父类函数
	inline void			setBlendFunc(ccBlendFunc blendFunc) { m_pSprite->setBlendFunc(blendFunc); }
	inline ccBlendFunc	getBlendFunc(void) { return m_pSprite->getBlendFunc(); }
	virtual void		setOpacity(GLubyte opacity);
	virtual void		draw();

	// 添加一个控制点
	// 注：该操作将导致本精灵会被重绘
	void				AddControlPoint( const CCPoint& p );
	// 设置一个新的控制点队列
	// 注：该操作将导致本精灵会被重绘
	void				SetControlPoints( CCPointArray& pa );
	// 设置指定片段是否可见
	void				SetSegmentVisible(int sIndex, bool visible);
	// 判断指定片段是否可见
	bool				IsSegmentVisible(int sIndex);
	// 设置全部片段可见
	void				ResetSegmentVisibility();
	// 设置曲线张力（该值越大，曲线弧度越大）
	void				SetTension(float t);

	// 获取控制点个数
	int GetControlPointCount() { return m_tagControlPoints.GetCount(); }
	// 获取片段个数
	int GetSegmentCount() { return MAX(0, GetControlPointCount() - 1); }
};