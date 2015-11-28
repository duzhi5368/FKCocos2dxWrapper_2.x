//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_RenderEx_CatmullRomSprite.h
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
class CTest_RenderEx_CatmullRomSpriteLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pCurve = FKCW_RenderEx_CatmullRomSprite::create( s_szLinePattern );
		m_pCurve->setPosition(CCPointZero);
		m_pCurve->AddControlPoint( ccp( CCDirector::sharedDirector()->getWinSize().width / 20,
		CCDirector::sharedDirector()->getWinSize().height / 10) );
		m_pCurve->AddControlPoint(ccp( CCDirector::sharedDirector()->getWinSize().width / 5,
			CCDirector::sharedDirector()->getWinSize().height / 2 ));
		addChild(m_pCurve);

		setTouchEnabled(true);
		setTouchMode(kCCTouchesOneByOne);

		FKCW_UI_ToastLayer* t = FKCW_UI_ToastLayer::Create(this, 
			CCLabelTTF::create( A2U("点击屏幕添加节点").c_str(), "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()));
		t->setPosition( SCREEN_CENTER_POS + ccp(0, -200));

		return true;
	}
public:
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
		m_pCurve->AddControlPoint(pTouch->getLocation());
		return false;
	}

	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {

	}

	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {

	}

	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {

	}
public:
	CREATE_FUNC( CTest_RenderEx_CatmullRomSpriteLayer );
private:
	FKCW_RenderEx_CatmullRomSprite* m_pCurve;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, CatmullRomSprite, 笛卡尔差值扭曲精灵)
//-------------------------------------------------------------------------