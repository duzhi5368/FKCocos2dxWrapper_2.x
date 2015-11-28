//*************************************************************************
//	创建日期:	2014-12-20
//	文件名称:	Test_RenderEx_JoystickNode.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_RenderEx_JoystickNodeLayer : public CCLayer
{
public:
	bool init()
	{
		FKCW_RenderEx_JoystickNode* joystickNode = FKCW_RenderEx_JoystickNode::create(s_szBaseBoardPath, 128/2, s_szJoystickPath, 64/2);
		joystickNode->setPosition(SCREEN_CENTER_POS);
		joystickNode->setHandleEventListener(this, joystickEvent_selector(CTest_RenderEx_JoystickNodeLayer::onJoystickListener));
		this->addChild(joystickNode);

		m_pInfoLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("请拖拽摇杆").c_str(), "", 26);
		m_pInfoLabel->setPosition(SCREEN_CENTER_POS + ccp(0, -200));
		this->addChild(m_pInfoLabel);

		return true;
	}
public:
	void onJoystickListener(float interval,float x, float y, ENUM_JoystickEventType type)
	{
		char c_type[28];
		if (type == JET_TOUCH_BEGIN)
		{
			sprintf(c_type, "触摸开始");
		}
		else if (type == JET_TOUCH_MOVE)
		{
			sprintf(c_type, "触摸移动");
		}
		if (type == JET_TOUCH_END)
		{
			sprintf(c_type, "触摸结束");
		}

		char buff[128];
		sprintf(buff, "时间间隔:%f, x:%f, y:%f \n事件:%s", interval, x, y, c_type);
		
		m_pInfoLabel->setString(FKCW_Base_StringConvert::a2u(buff).c_str());
	}
public:
	CREATE_FUNC( CTest_RenderEx_JoystickNodeLayer );
protected:
	CCLabelTTF* m_pInfoLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, JoystickNode, 虚拟摇杆节点)
//-------------------------------------------------------------------------