//*************************************************************************
//	创建日期:	2014-12-18
//	文件名称:	Test_RenderEx_TurnCardSprite.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:	无法正确得到包围盒
//				多次执行OpenCard不会再次翻转
//				正在执行翻转时，立即执行OpenCard程序跳出
//				当动画时间较长时，会提前显示卡牌反面纹理，不太真实。 请参考sprite3
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_RenderEx_TurnCardSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		m_nCurIndex = 0;

		m_pSprite1 = FKCW_RenderEx_TurnCardSprite::create(s_szCardPath, s_szCard2Path, 0.5f);
		m_pSprite1->setPosition(SCREEN_CENTER_POS + ccp(-300, 0));
		this->addChild(m_pSprite1);

		m_pSprite2 = FKCW_RenderEx_TurnCardSprite::create(s_szCardPath, s_szCard2Path, 1.5f);
		m_pSprite2->setPosition(SCREEN_CENTER_POS);
		this->addChild(m_pSprite2);

		m_pSprite3 = FKCW_RenderEx_TurnCardSprite::create(s_szCard2Path, s_szCardPath, 1.0f);
		m_pSprite3->setPosition(SCREEN_CENTER_POS + ccp(300, 0));
		this->addChild(m_pSprite3);


		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我执行翻牌动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_RenderEx_TurnCardSpriteLayer::menuCallback));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
	void menuCallback(CCObject* pSender)
	{
		switch ( m_nCurIndex % 3 )
		{
		case 0:
			m_pSprite1->OpenCard();
			break;
		case 1:
			m_pSprite2->OpenCard();
			break;
		case 2:
			m_pSprite3->OpenCard();
			break;
		}
		m_nCurIndex++;
		if( m_nCurIndex >= 3 )
		{
			((CCMenuItemLabel*)pSender)->setEnabled( false );
		}
	}
public:
	CREATE_FUNC( CTest_RenderEx_TurnCardSpriteLayer );
protected:
	FKCW_RenderEx_TurnCardSprite* m_pSprite1;
	FKCW_RenderEx_TurnCardSprite* m_pSprite2;
	FKCW_RenderEx_TurnCardSprite* m_pSprite3;
	int			m_nCurIndex;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, TurnCardSprite, 卡牌（翻页）精灵)
//-------------------------------------------------------------------------