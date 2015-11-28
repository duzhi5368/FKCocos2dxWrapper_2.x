//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	Test_TMX_Flip.h
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
#pragma warning( disable : 4244 )
//-------------------------------------------------------------------------
class CTest_TMX_FlipLayer : public CCLayer
{
public:
	FKCW_TMX_Map* createMap()
	{
		return FKCW_TMX_Map::createWithXMLFile( s_szFlipMapFile );
	}
public:
	virtual bool init()
	{
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 添加Map
		m_map = createMap();
		m_map->setPosition(ccp( SCREEN_CENTER_POS_X / 2, SCREEN_CENTER_POS_Y / 2 ));
		m_map->setDebugDrawObjects(true);
		addChild(m_map);

		// 触摸标示
		m_sprite = CCSprite::create( s_szLetterAImagePath );
		m_sprite->setVisible(false);
		m_sprite->setScale( 0.1f );
		m_map->addChild(m_sprite, MAX_INT);

		setTouchEnabled(true);
		setTouchMode(kCCTouchesOneByOne);

		return true;
	}
public:
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) 
	{
		m_lastLoc = pTouch->getLocation();

		FKCW_TMX_Layer* layer = (FKCW_TMX_Layer*)m_map->getLayerAt(0);
		CCPoint loc = layer->convertToNodeSpace(m_lastLoc);
		CCPoint d = layer->getTileCoordinateAt(loc.x, loc.y);
		int gid = layer->getGidAt(d.x, d.y);
		if( gid == 0 )
			return true;
		int pureGid = gid & eTMXTileFlagFlipMask;
		if((gid & eTMXTileFlagFlipAll) == eTMXTileFlagFlipAll)
			layer->updateTileAt(pureGid, d.x, d.y);
		else if((gid & eTMXTileFlagFlipH) == eTMXTileFlagFlipH)
			layer->updateTileAt(pureGid | eTMXTileFlagFlipV, d.x, d.y);
		else if((gid & eTMXTileFlagFlipV) == eTMXTileFlagFlipV)
			layer->updateTileAt(pureGid | eTMXTileFlagFlipDiagonal, d.x, d.y);
		else if((gid & eTMXTileFlagFlipDiagonal) == eTMXTileFlagFlipDiagonal)
			layer->updateTileAt(pureGid | eTMXTileFlagFlipAll, d.x, d.y);
		else							  
			layer->updateTileAt(pureGid | eTMXTileFlagFlipH, d.x, d.y);

		return true;
	}
public:
	CREATE_FUNC( CTest_TMX_FlipLayer );
protected:
	CCSprite* m_sprite;
	FKCW_TMX_Map* m_map;
	CCPoint m_lastLoc;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(TMX, Flip, 直角地图测试，点击地图格可进行单元旋转)
//-------------------------------------------------------------------------