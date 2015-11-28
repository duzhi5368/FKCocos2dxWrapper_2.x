//*************************************************************************
//	创建日期:	2014-12-19
//	文件名称:	Test_GR_Examples.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:	不会使用。。。
//  修改记录: 更新描述与优化操作显示
//	修 改 人: 邓观福 Rect
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_GR_ExamplesLayer : public CCLayer
{
public:
	bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		//长按
		FKCW_GR_LongPressGestureRecognizer* grLongPress = FKCW_GR_LongPressGestureRecognizer::create();
		grLongPress->SetTarget(this, callfuncO_selector(CTest_GR_ExamplesLayer::onLongPress));
		m_pWindow->addChild(grLongPress);

		//滑动操作,慢速拖移
		FKCW_GR_PanGestureRecognizer* grPan = FKCW_GR_PanGestureRecognizer::create();
		grPan->SetTarget(this, callfuncO_selector(CTest_GR_ExamplesLayer::onPan));
		m_pWindow->addChild(grPan);

		//捏合
		FKCW_GR_PinchGestureRecognizer* grPinch = FKCW_GR_PinchGestureRecognizer::create();
		grPinch->SetTarget(this, callfuncO_selector(CTest_GR_ExamplesLayer::onPinch));
		m_pWindow->addChild(grPinch);

		//快速滑动，和pan不同是速度
		FKCW_GR_SwipeGestureRecognizer* grSwipe = FKCW_GR_SwipeGestureRecognizer::create();
		grSwipe->SetTarget(this, callfuncO_selector(CTest_GR_ExamplesLayer::onSwipe));
		m_pWindow->addChild(grSwipe);

		//点击
		FKCW_GR_TapGestureRecognizer* grTap = FKCW_GR_TapGestureRecognizer::create();
		grTap->SetTarget(this, callfuncO_selector(CTest_GR_ExamplesLayer::onTap));
		m_pWindow->addChild(grTap);
	
		
		std::string str = "目前支持的手势：\n";
		str += "单点长按:定义长按时间为0.5s\n";
		str += "单点滑动:依靠2dx原有的moved消息判断\n";
		str += "多点捏合:只判断前两个触控点\n";
		str += "单点快速滑动:可更改响应速度(60p/300ms)\n";
		str += "单点点击";

		CCLabelTTF* pLabel = CCLabelTTF::create(
			A2U(str.c_str()).c_str(), 
			"", 24);
		pLabel->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pLabel);
		//提示
		m_pInfoLabel = CCLabelTTF::create(A2U("当前无消息").c_str(), "", 26);
		m_pInfoLabel->setColor(ccc3(255,0,0));
		m_pInfoLabel->setPosition(SCREEN_CENTER_POS+ccp(0, 200));
		m_pWindow->addChild(m_pInfoLabel);

		return true;
	}
public:
	void onLongPress(CCObject* object)
	{
		FKCW_GR_LongPress* p = dynamic_cast<FKCW_GR_LongPress*>(object);
		if( p == NULL )
		{
			m_pInfoLabel->setString(A2U("长按消息").c_str());
		}
		else
		{
			char szInfo[64];
			memset( szInfo, 0, 64 );
			sprintf( szInfo, "长按消息: pos.x = %.2f, pos.y = %.2f", 
				p->m_tagLocation.x, p->m_tagLocation.y );
			m_pInfoLabel->setString(A2U( szInfo ).c_str());
		}
	}
	void onPan(CCObject* object)
	{
		FKCW_GR_Pan* p = dynamic_cast<FKCW_GR_Pan*>(object);
		if( p == NULL )
		{
			m_pInfoLabel->setString(A2U("滑动拖移").c_str());
		}
		else
		{
			char szInfo[128];
			memset( szInfo, 0, 128 );
			sprintf( szInfo, "滑动消息: pos.x = %.2f, pos.y = %.2f delta.x = %.2f, delta.y = %.2f", 
				p->m_tagLocation.x, p->m_tagLocation.y,
				p->m_tagDelta.x, p->m_tagDelta.y );
			m_pInfoLabel->setString(A2U( szInfo ).c_str());
		}
	}
	void onPinch(CCObject* object)
	{
		FKCW_GR_Pinch* p = dynamic_cast<FKCW_GR_Pinch*>(object);
		if( p == NULL )
		{
			m_pInfoLabel->setString(A2U("捏合").c_str());
		}
		else
		{
			char szInfo[64];
			memset( szInfo, 0, 64 );
			if( p->m_eType == eGRPinchType_Close )
			{
				sprintf(szInfo, "%s", "捏合缩小");
			}
			else if( p->m_eType == eGRPinchType_Open )
			{
				sprintf(szInfo, "%s", "捏合放大");
			}
			else
			{
				sprintf(szInfo, "%s", "捏合:WTF");
			}
			m_pInfoLabel->setString(A2U( szInfo ).c_str());
		}
	}
	void onSwipe(CCObject* object)
	{
		FKCW_GR_Swipe* p = dynamic_cast<FKCW_GR_Swipe*>(object);
		if( p == NULL )
		{
			m_pInfoLabel->setString(A2U("快速滑动").c_str());
		}
		else
		{
			char szInfo[128];
			memset( szInfo, 0, 128 );
			std::string str;
			if( (p->m_eDirection & eSwipeDirection_Right) == eSwipeDirection_Right )
				str += "向右";
			if( (p->m_eDirection & eSwipeDirection_Left) == eSwipeDirection_Left )
				str += "向左";
			if( (p->m_eDirection & eSwipeDirection_Up) == eSwipeDirection_Up )
				str += "向上";
			if( (p->m_eDirection & eSwipeDirection_Down) == eSwipeDirection_Down )
				str += "向下";
			sprintf( szInfo, "快速滑动: pos.x = %.2f, pos.y = %.2f", 
				p->m_tagLocation.x, p->m_tagLocation.y );
			str += string( szInfo );
			m_pInfoLabel->setString(A2U( str.c_str() ).c_str());
		}
	}
	void onTap(CCObject* object)
	{
		FKCW_GR_Tap* p = dynamic_cast<FKCW_GR_Tap*>(object);
		if( p == NULL )
		{
			m_pInfoLabel->setString(A2U("点击").c_str());
		}
		else
		{
			char szInfo[64];
			memset( szInfo, 0, 64 );
			sprintf( szInfo, "点击消息: pos.x = %.2f, pos.y = %.2f", 
				p->m_tagLocation.x, p->m_tagLocation.y );
			m_pInfoLabel->setString(A2U( szInfo ).c_str());
		}
	}
public:
	CREATE_FUNC( CTest_GR_ExamplesLayer );
protected:
	CCLabelTTF* m_pInfoLabel;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(GR, Examples, 手势识别示例)