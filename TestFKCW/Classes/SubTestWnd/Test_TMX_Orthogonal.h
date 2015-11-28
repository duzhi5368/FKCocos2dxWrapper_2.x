//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	Test_TMX_Orthogonal.h
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
#pragma warning( disable:4244 )
//-------------------------------------------------------------------------
class CTest_TMX_OrthogonalLayer : public CCLayer
{
public:
	FKCW_TMX_Map* createMap()
	{
		return FKCW_TMX_Map::createWithXMLFile( s_szOrthoMultiMapFile );
	}
public:
	virtual bool init()
	{
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 添加Map
		m_map = createMap();
		m_map->setPosition(ccp( SCREEN_CENTER_POS_X / 2, 0 ));
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

		// 贴上精灵
		CCPoint pos = layer->getPositionAt(d.x, d.y);
		if(d.x != -1) {
			m_sprite->setVisible(true);
			m_sprite->setPosition(ccp(pos.x + layer->getTileWidth() / 2,
				pos.y + layer->getTileHeight() / 2));
		} else {
			m_sprite->setVisible(false);
		}

		// 执行旋转
		CCSprite* sprite = layer->tileAt(d.x, d.y);
		if(sprite) {
			sprite->stopAllActions();
			sprite->setRotation(0);
			CCRotateBy* rotate = CCRotateBy::create(2.f, 360);
			sprite->runAction(rotate);
		}


		// 删除
		int gid = layer->getGidAt(d.x, d.y);
		if(gid <= 0) 
		{
			// 添加
			layer->setTileAt( static_cast<int>(FKCW_Base_Utils::RandomInRange(0,40)) + 1, d.x, d.y);
		} else {
			// 删除
			layer->removeTileAt(d.x, d.y);
		}


		return true;
	}
public:
	CREATE_FUNC( CTest_TMX_OrthogonalLayer );
protected:
	CCSprite* m_sprite;
	FKCW_TMX_Map* m_map;
	CCPoint m_lastLoc;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(TMX, Orthogonal, 多层直角地图测试，动态添增)
//-------------------------------------------------------------------------