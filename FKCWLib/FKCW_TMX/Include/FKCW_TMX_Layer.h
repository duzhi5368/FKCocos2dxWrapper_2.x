//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_Layer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_MapInfo;
class FKCW_TMX_LayerInfo;
class FKCW_TMX_TileSetInfo;
class FKCW_TMX_SpriteBatchNode;
//-------------------------------------------------------------------------
class FKCW_TMX_Layer : public CCNodeRGBA
{
private:
	typedef struct {
		int atlasIndex;
		int tilesetIndex;
	} STileSetAtlasInfo;
	
private:

	int* m_tiles;					// GridID列表
	CCSprite* m_reusedTile;
	
private:
	// 当Index大于指定值，则减少它
	void decreaseIndexIfMoreThan(int tilesetIndex, int index);
	//  当Index小于指定值，则增加他
	void increaseIndexIfEqualOrMoreThan(int tilesetIndex, int index);
	// 添加一个CCSprite到Tile中
	void setupTileSprite(CCSprite* sprite, CCPoint pos, int gid);
	// 获取复用的Tile
	CCSprite* reusedTile(CCRect rect, FKCW_TMX_SpriteBatchNode* bn);
	
protected:
	FKCW_TMX_Layer(int layerIndex, FKCW_TMX_MapInfo* mapInfo);

	// 在直角地图中获取一个Tile的左下角像素坐标
	CCPoint getPositionForOrthoAt(int posX, int posY);
	// 在斜45地图中获取一个Tile的左下角像素坐标
	CCPoint getPositionForIsoAt(int posX, int posY);
	// 在六边形地图中获取一个Tile的左下角像素坐标
	CCPoint getPositionForHexAt(int posX, int posY);
	// 获取一个Tile的Z值
	float getVertexZAt(int x, int y);
	// 添加一个Tile
	void appendTileForGid(int tilesetIndex, int gid, int x, int y);
	// 设置一个Tile属性
    void setTileAt(int tilesetIndex, int gid, int x, int y, int z);
	// 解析属性
	void parseInternalProperties();
	// 计算Tile相对于所在层的像素偏移值
	CCPoint calculateLayerOffset(float x, float y);
	// 初始化GridIndex
	void setupTiles();
	 
public:
	virtual ~FKCW_TMX_Layer();
	// 创建Layer，Index从0开始叠加
	static FKCW_TMX_Layer* create(int layerIndex, FKCW_TMX_MapInfo* mapInfo);
	// 获取层属性
	string getProperty(const string& key);

	// 根据相对于Layer的偏移像素位置，返回Tile的位置编号值
	CCPoint getTileCoordinateAt(float x, float y);
	// 根据给定的Tile的位置编号值，获取Tile
	// 返回时会同时将返回的CCSprite，该Sprite已经添加到Layer中
	// 可以调用 layer->removeChild(sprite, true); 或者layer->removeTileAt(x, y);来进行删除
	CCSprite* tileAt(int x, int y);
	// 从Layer删除一个Tile
	void removeTile(CCSprite* sprite);
	// 根据Tile的编号值获取其GridID
	int getGidAt(int x, int y);
	// 获取一份GridID数组
	// 这个数组大小为 layerWidth * layerHeight
	const int* getGids();
	// 拷贝一份GridID数组
	// 调用者需要释放这个返回的指针
	const int* copyGids();

	//  删除指定位置的Tile
	void removeTileAt(int x, int y);
	// 设置指定Tile位置，若Tile存在，则相等于updateTileAt
	void setTileAt(int gid, int x, int y);
	// 更新指定Tile位置，若Tile不存在，则添加新Tile
	void updateTileAt(int gid, int x, int y);
	// 获取指定Tile相对于所在Layer的左下角位置
	CCPoint getPositionAt(int x, int y);
	// 更变指定Tile的颜色
	void setTileColorAt(ccColor4B c, int x, int y);
	
public:
	CC_SYNTHESIZE(int, m_layerWidth, LayerWidth);							// Layer宽度（Tiles个数)
	CC_SYNTHESIZE(int, m_layerHeight, LayerHeight);							// Layer高度（Tiles个数)
	CC_SYNTHESIZE(int, m_tileWidth, TileWidth);								// Layer宽度（像素值)
	CC_SYNTHESIZE(int, m_tileHeight, TileHeight);							// Layer高度（像素值)
	CC_SYNTHESIZE(int, m_minGid, MinGid);									// 最小GridID
	CC_SYNTHESIZE(int, m_maxGid, MaxGid);									// 最大GridID
	CC_SYNTHESIZE(FKCW_TMX_MapInfo*, m_mapInfo, MapInfo);
	CC_SYNTHESIZE(FKCW_TMX_LayerInfo*, m_layerInfo, LayerInfo);
	CC_SYNTHESIZE(STileSetAtlasInfo*, m_atlasInfos, AtlasInfos);
	CC_SYNTHESIZE(FKCW_TMX_SpriteBatchNode**, m_batchNodes, BatchNodes);	// BatchNode组
	CC_SYNTHESIZE_BOOL(m_useAutomaticVertexZ, UseAutomaticVertexZ);			// 自定义计算Z值
	CC_SYNTHESIZE(float, m_vertexZ, VertexZ);								// 调整后的顶点Z值
	CC_SYNTHESIZE(float, m_alphaFuncValue, AlphaFuncValue);					// Alpha值
};
//-------------------------------------------------------------------------
class FKCW_TMX_SpriteBatchNode : public CCSpriteBatchNode
{
	friend class FKCW_TMX_Layer;
	
public:
    FKCW_TMX_SpriteBatchNode();
    virtual ~FKCW_TMX_SpriteBatchNode();

    static FKCW_TMX_SpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity);
    static FKCW_TMX_SpriteBatchNode* createWithTexture(CCTexture2D* tex) {
        return FKCW_TMX_SpriteBatchNode::createWithTexture(tex, kDefaultSpriteBatchCapacity);
    }

    static FKCW_TMX_SpriteBatchNode* create(const char* fileImage, unsigned int capacity);
    static FKCW_TMX_SpriteBatchNode* create(const char* fileImage) {
        return FKCW_TMX_SpriteBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
    }
};
//-------------------------------------------------------------------------