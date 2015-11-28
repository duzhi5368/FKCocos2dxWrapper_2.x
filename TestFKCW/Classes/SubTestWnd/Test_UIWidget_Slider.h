//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_Slider.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	滑块
//  测试结果:		和想象中的效果不大一样：看不到滑块的总长度
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_SliderLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		m_pScrollView = FKCW_UIWidget_ScrollView::create(CCSize(SCREEN_CENTER_POS_X, SCREEN_CENTER_POS_Y));
		m_pScrollView->setBackgroundImage(s_szBackground1Path);
		m_pScrollView->setContainerSize(CCSize(3100, 320));
		m_pScrollView->setDirection(eScrollViewDirectionHorizontal);
		m_pScrollView->setPosition(CCPoint(SCREEN_CENTER_POS_X, 350));
		m_pScrollView->setDragable(false);
		m_pWindow->addChild(m_pScrollView);

		float x = 100.0f;
		for( int i = 0; i < 30; i++ )
		{
			FKCW_UIWidget_Button* pIcon = FKCW_UIWidget_Button::createWith9Sprite(
				CCSize(60, 60), s_szSprite9Btn1Path, s_szSprite9Btn2Path );

			char buf[32] = {0};
			sprintf(buf, "%d", i + 1);
			pIcon->initText(buf, "", 25);
			pIcon->setPosition(CCPoint( x , 160 ));
			m_pScrollView->getContainer()->addChild(pIcon);
			x += 100.0f;
		}

		m_pSlider = FKCW_UIWidget_Slider::create();
		m_pSlider->setSliderImage(s_szSliderPath);
		m_pSlider->setProgressImage(s_szProgressPath);
		m_pSlider->setBackgroundImage(s_szProgressBGPath);

		m_pSlider->setPosition(CCPoint(SCREEN_CENTER_POS_X, 140));
		m_pSlider->setMaxValue( (int)(m_pScrollView->getContainer()->getContentSize().width - m_pScrollView->getContentSize().width) );
		m_pSlider->setMinValue(0);
		m_pSlider->setValue(0);
		m_pSlider->setOnValueChangedListener(this, fkcw_uiwidget_valuechanged_selector(CTest_UIWidget_SliderLayer::onValueChanged));
		m_pWindow->addChild(m_pSlider);

		m_pLabel = FKCW_UIWidget_Label::create("0","", 30.0f);
		m_pLabel->setPosition(CCPoint(SCREEN_CENTER_POS_X, 100));
		m_pWindow->addChild(m_pLabel);

		return true;
	}
public:
	void onValueChanged(CCObject* pSender, int nValue)
	{
		m_pScrollView->setContentOffset(CCPoint((float)(-nValue), 0));
		int nMaxValue = m_pSlider->getMaxValue();
		char buf[32] = {0};
		sprintf(buf, "%d/%d", nValue,nMaxValue);
		m_pLabel->setString(buf);

	}
public:
	CREATE_FUNC( CTest_UIWidget_SliderLayer );
protected:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label*	m_pLabel;
	FKCW_UIWidget_Slider* m_pSlider;
	FKCW_UIWidget_ScrollView* m_pScrollView;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, Slider, 滑块，一般用来控制游戏的音量大小)