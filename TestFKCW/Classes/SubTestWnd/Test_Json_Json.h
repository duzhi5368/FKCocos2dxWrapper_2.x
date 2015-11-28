//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_Json_Json.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

#pragma comment( lib, "yajl.lib")
//-------------------------------------------------------------------------
class CTest_Json_JsonLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_Json_Object* pJsonFileObject	= FKCW_Json_Object::Create( s_szTestJsonFile );
		FKCW_Json_Array* ja					= pJsonFileObject->GetArray("f");
		FKCW_Json_Object* jo2				= ja->GetObject(5);
		string stringVal					= jo2->GetString("f2");
		int intVal							= jo2->GetInt("f2");
		float floatVal						= jo2->GetFloat("f2");
		bool boolVal						= jo2->GetBool("f2");

		char buf[256];
		sprintf(buf, "JSON 解析结果,控制台可查看整个输出\n源文件: Resource/File/Test.json\n\n以下为key为f2的解析结果\nkey: f2\nstring value: \"%s\"\nbool value: %s\nint value: %d\nfloat value: %.2f",
					stringVal.c_str(), boolVal ? "true" : "false", intVal, floatVal);
		m_pOutputLabel = CCLabelTTF::create(A2U(buf).c_str(), "Helvetica", 26);
		m_pOutputLabel->setPosition( SCREEN_CENTER_POS + ccp(0,100) );
		m_pWindow->addChild(m_pOutputLabel);

		FKLOG(pJsonFileObject->ToString().c_str());



		FKCW_UIWidget_Button* pButtonFind = FKCW_UIWidget_Button::createWith9Sprite(CCSize(196, 68),
			s_szButtonNormalPath, s_szButtonPressPath );
		pButtonFind->setPosition(SCREEN_CENTER_POS + ccp(-200, -260));
		pButtonFind->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Json_JsonLayer::onClickFindByKey));
		pButtonFind->initText(FKCW_Base_StringConvert::a2u("查找").c_str(), "", 26);
		m_pWindow->addChild(pButtonFind);

		FKCW_UIWidget_Button* pButtonAdd = FKCW_UIWidget_Button::createWith9Sprite(CCSize(196, 68),
			s_szButtonNormalPath, s_szButtonPressPath );
		pButtonAdd->setPosition(SCREEN_CENTER_POS + ccp(0, -260));
		pButtonAdd->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Json_JsonLayer::onClickAddByKey));
		pButtonAdd->initText(FKCW_Base_StringConvert::a2u("添加").c_str(), "", 26);
		m_pWindow->addChild(pButtonAdd);

		CCScale9Sprite* pEditBoxBG1 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcKeyEdit = CCEditBox::create(CCSizeMake(290, 50), pEditBoxBG1 );
		m_pSrcKeyEdit->setPosition( SCREEN_CENTER_POS + ccp(-150, -200));
		m_pSrcKeyEdit->setFontSize( 26 );
		m_pSrcKeyEdit->setText(FKCW_Base_StringConvert::a2u("这里输入key").c_str());
		m_pWindow->addChild(m_pSrcKeyEdit);

		CCScale9Sprite* pEditBoxBG2 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcValueEdit = CCEditBox::create(CCSizeMake(290, 50), pEditBoxBG2 );
		m_pSrcValueEdit->setPosition( SCREEN_CENTER_POS + ccp(150, -200));
		m_pSrcValueEdit->setFontSize( 26 );
		m_pSrcValueEdit->setText(FKCW_Base_StringConvert::a2u("这里输入value").c_str());
		m_pWindow->addChild(m_pSrcValueEdit);

		return true;
	}

	void onClickFindByKey(CCObject* pSender)
	{
		char buf[256];
		FKCW_Json_Object* pJsonFileObject	= FKCW_Json_Object::Create( s_szTestJsonFile );
		if (pJsonFileObject == NULL)
		{
			
			sprintf(buf, "open %s failed!",s_szTestJsonFile);
			FKLOG(buf);
			return;
		}

		string strKey = m_pSrcKeyEdit->getText();
		string strValue = pJsonFileObject->GetString(strKey.c_str(),"null");

		if (strValue.compare("null") == 0)
		{
			sprintf(buf, "JSON 解析结果,控制台可查看整个文件输出\n源文件: Resource/File/Test.json\n\n不存在key为 %s 的的节点",strKey.c_str());
		} 
		else
		{
			sprintf(buf, "JSON 解析结果,控制台可查看整个文件输出\n源文件: Resource/File/Test.json\n\n以下为key为%s的解析结果\nkey: %s\nstring value: \"%s\"",
				strKey.c_str(),strKey.c_str(), strValue.c_str() );
		}
		

		m_pOutputLabel->setString(A2U(buf).c_str());
	}

	void onClickAddByKey(CCObject* pSender)
	{
		char buf[256];
		FKCW_Json_Object* pJsonFileObject	= FKCW_Json_Object::Create( s_szTestJsonFile );
		if (pJsonFileObject == NULL)
		{

			sprintf(buf, "open %s failed!",s_szTestJsonFile);
			FKLOG(buf);
			return;
		}

		string strKey = m_pSrcKeyEdit->getText();
		string strValue	= m_pSrcValueEdit->getText();
		string strOldValue = pJsonFileObject->GetString(strKey.c_str(),"null");

		if (strOldValue.compare("null") == 0)
		{
			pJsonFileObject->AddString(strKey.c_str(),strValue.c_str());
			sprintf(buf, "JSON 解析结果,控制台可查看整个文件输出\n源文件: Resource/File/Test.json\n\n成功添加 key为 %s ,value 为 %s 的节点",
				strKey.c_str(),strValue.c_str());
			FKLOG(pJsonFileObject->ToString().c_str());
		} 
		else
		{
			sprintf(buf, "JSON 解析结果,控制台可查看整个文件输出\n源文件: Resource/File/Test.json\n\n已经存在 key为 %s 的节点",strKey.c_str());
		}

		

		m_pOutputLabel->setString(A2U(buf).c_str());


	}

private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCLabelTTF*					m_pOutputLabel;
	CCEditBox*					m_pSrcKeyEdit;
	CCEditBox*					m_pSrcValueEdit;
	FKCW_Json_Object*			m_pJsonFileObject;
public:
	CREATE_FUNC( CTest_Json_JsonLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Json, Json, Json格式文件测试)
//-------------------------------------------------------------------------