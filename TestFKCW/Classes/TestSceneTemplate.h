//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	TestSceneTemplate.h
//  创 建 人:   WilhanTian
//	版权所有:	MIT
//	说    明:	创建测试Scene宏模板
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 创建一个测试SCENE
#define CREATE_TEST_SCENE_CLASS(_TEST_ITEM_TYPE_, _TEST_ITEM_NAME_, _DESCRIPTION_TEXT_)\
class CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Scene : public CTestScene\
{\
public:\
	CREATE_SCENE_FUNC(CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Scene);\
public:\
	CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Scene()\
	{\
		SetInfo( #_DESCRIPTION_TEXT_ );\
	}\
	virtual void Run()\
	{\
		CCLayerColor *layer = CCLayerColor::create( ccc4BFromccc4F(ccc4FFromccc3B( ccc3(18,53,85) )));\
		addChild(layer);\
\
		CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Layer* pLayer = (CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Layer*)CTest_##_TEST_ITEM_TYPE_##_##_TEST_ITEM_NAME_##Layer::create();\
		if( pLayer )\
		{\
			this->addChild( pLayer );\
		}\
	}\
};

//-------------------------------------------------------------------------