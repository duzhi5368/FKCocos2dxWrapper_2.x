//*************************************************************************
//	创建日期:	2014-12-11
//	文件名称:	Test_UI_DynamicNumLabel.h
//  创 建 人:   Wilhan Tian	
//	版权所有:	MIT
//	说    明:	动态数字标签测试	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UI_DynamicNumLabelLayer : public CCLayer
{
public:
	virtual bool init()
	{
		//提示按钮
		CCLabelTTF* pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我将动态改变数字标签").c_str(), "Arial", 36 );
		pLabel->setColor( ccc3(230,155,3) );

		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(pLabel, this, menu_selector(CTest_UI_DynamicNumLabelLayer::menuCallBack));
		CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
		pMenu->setPosition( SCREEN_CENTER_POS + ccp( 0, -200 ));
		addChild(pMenu, 1);

		m_pLabel1 = FKCW_UI_DynamicNumLabel::create("test1", "Atlas", 35.0f);
		m_pLabel1->setPosition(CCPoint(SCREEN_CENTER_POS_X, 450));
		this->addChild(m_pLabel1);
		
		CCLabelTTF* lable1 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("默认参数风格").c_str(), "Arial", 26 );
		lable1->setColor( ccBLACK );
		lable1->setPosition(CCPoint(SCREEN_CENTER_POS_X + 150, 450));
		this->addChild(lable1);

		m_pLabel2 = FKCW_UI_DynamicNumLabel::create("-2.0", "Atlas", 35.0f, CCSize(500.0f, 100.0f), CCTextAlignment::kCCTextAlignmentLeft);
		m_pLabel2->setPosition(CCPoint(SCREEN_CENTER_POS_X, 350));
		this->addChild(m_pLabel2);

		CCLabelTTF* lable2 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("左对齐风格").c_str(), "Arial", 26 );
		lable2->setColor( ccBLACK );
		lable2->setPosition(CCPoint(SCREEN_CENTER_POS_X + 150, 350));
		this->addChild(lable2);

		m_pLabel3 = FKCW_UI_DynamicNumLabel::create("1000", "Atlas", 35.0f, CCSize(30.0f, 100), CCTextAlignment::kCCTextAlignmentRight, CCVerticalTextAlignment::kCCVerticalTextAlignmentBottom);
		m_pLabel3->setPosition(CCPoint(SCREEN_CENTER_POS_X, 250));
		this->addChild(m_pLabel3);

		CCLabelTTF* lable3 = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("裁切 右对齐 上对齐").c_str(), "Arial", 26 );
		lable3->setColor( ccBLACK );
		lable3->setPosition(CCPoint(SCREEN_CENTER_POS_X + 150, 250));
		this->addChild(lable3);

		return true;
	}
public:
	void menuCallBack(CCObject* pSender) 
	{
		//修改动态数值
		m_pLabel1->SetDynamicValue((m_pLabel1->GetDynamicValue()+1) * (m_pLabel1->GetDynamicValue()+1));
		m_pLabel2->SetDynamicValue( (int)FKCW_Base_Utils::RandomInRange( 0, 100000 ));
		m_pLabel3->SetDynamicValue((m_pLabel3->GetDynamicValue()+1) * (m_pLabel3->GetDynamicValue()+1));
	}
public:
	CREATE_FUNC( CTest_UI_DynamicNumLabelLayer );
private:
	FKCW_UI_DynamicNumLabel* m_pLabel1;
	FKCW_UI_DynamicNumLabel* m_pLabel2;
	FKCW_UI_DynamicNumLabel* m_pLabel3;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UI, DynamicNumLabel, 动态数字标签实例，注意，暂时只支持整数。可支持显示区切割，显示对齐方式等)
//-------------------------------------------------------------------------