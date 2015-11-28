//-------------------------------------------------------------------------
#include "ListControllerLayer.h"
#include "SceneMacro.h"
#include "Resource.h"
#include "TestScene.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"

#include "SubTestWnd/ExampleScene.h"

#include "SubTestWnd/Test_GR_Examples.h"

#include "SubTestWnd/Test_Font_Test.h"

#include "SubTestWnd/Test_Database_Database.h"

#include "SubTestWnd/Test_SimpleNet_NetObj.h"

#include "SubTestWnd/Test_Json_Json.h"

#include "SubTestWnd/Test_SceneEx_UISceneEx.h"

#include "SubTestWnd/Test_3D_FKSimpleModel.h"

#include "SubTestWnd/Test_RichHtml_HtmlLabel.h"

#include "SubTestWnd/Test_Story_StoryLayer.h"

#include "SubTestWnd/Test_Pixel_Demo.h"

#include "SubTestWnd/Test_Shader_Demo1.h"
#include "SubTestWnd/Test_Shader_Demo2.h"

#include "SubTestWnd/Test_Network_FileDownLoader.h"
#include "SubTestWnd/Test_Network_HttpGet.h"
#include "SubTestWnd/Test_Network_TCP.h"
#include "SubTestWnd/Test_Network_UDP.h"

#include "SubTestWnd/Test_UI_AutoRenderMenuItemSprite.h"
#include "SubTestWnd/Test_UI_MenuItemColor.h"
#include "SubTestWnd/Test_UI_ToastLayer.h"
#include "SubTestWnd/Test_UI_SimpleRookieGuiderLayer.h"

#include "SubTestWnd/Test_UI_DynamicNumLabel.h"
#include "SubTestWnd/Test_UI_MenuItemProgressTimer.h"
#include "SubTestWnd/Test_UI_ProgressIndicator.h"
#include "SubTestWnd/Test_UI_ScrollableLayer.h"
#include "SubTestWnd/Test_UI_Table.h"

#include "SubTestWnd/Test_Action_Blur.h"
#include "SubTestWnd/Test_Action_ClipIn.h"
#include "SubTestWnd/Test_Action_Count.h"
#include "SubTestWnd/Test_Action_Flash.h"
#include "SubTestWnd/Test_Action_Missile.h"
#include "SubTestWnd/Test_Action_Shake.h"
#include "SubTestWnd/Test_Action_Shine.h"
#include "SubTestWnd/Test_Action_TreeFadeIn.h"
#include "SubTestWnd/Test_Action_TreeFadeOut.h"
#include "SubTestWnd/Test_Action_TrailMoveTo.h"
#include "SubTestWnd/Test_Action_CallFuncT.h"
#include "SubTestWnd/Test_Action_Curl.h"
#include "SubTestWnd/Test_Action_Suck.h"

#include "SubTestWnd/Test_UIWidget_DrawZOrder.h"
#include "SubTestWnd/Test_UIWidget_Button.h"
#include "SubTestWnd/Test_UIWidget_CheckBox.h"
#include "SubTestWnd/Test_UIWidget_ColorView.h"
#include "SubTestWnd/Test_UIWidget_ControlView.h"
#include "SubTestWnd/Test_UIWidget_ExpandableListView.h"
#include "SubTestWnd/Test_UIWidget_GradientView.h"
#include "SubTestWnd/Test_UIWidget_ImageView.h"
#include "SubTestWnd/Test_UIWidget_GridPageView.h"
#include "SubTestWnd/Test_UIWidget_GridView.h"
#include "SubTestWnd/Test_UIWidget_Label.h"
#include "SubTestWnd/Test_UIWidget_Layout.h"
#include "SubTestWnd/Test_UIWidget_ProgressBar.h"
#include "SubTestWnd/Test_UIWidget_ScrollView.h"
#include "SubTestWnd/Test_UIWidget_ToggleView.h"
#include "SubTestWnd/Test_UIWidget_ListView.h"
#include "SubTestWnd/Test_UIWidget_Slider.h"
#include "SubTestWnd/Test_UIWidget_TextRich.h"
#include "SubTestWnd/Test_UIWidget_LabelAtlas.h"
#include "SubTestWnd/Test_UIWidget_PageView.h"
#include "SubTestWnd/Test_UIWidget_TableView.h"

#include "SubTestWnd/Test_Base_AStar.h"
#include "SubTestWnd/Test_Base_UtilsI.h"
#include "SubTestWnd/Test_Base_DrawingPrimitives.h"
#include "SubTestWnd/Test_Base_Encode.h"
#include "SubTestWnd/Test_Base_CVSLoader.h"
#include "SubTestWnd/Test_Base_DecodeHtml.h"
#include "SubTestWnd/Test_Base_Localization.h"
#include "SubTestWnd/Test_Base_Memory.h"
#include "SubTestWnd/Test_Base_PinYin.h"
#include "SubTestWnd/Test_Base_ResourceLoader.h"
#include "SubTestWnd/Test_Base_SecureUserDefault.h"
#include "SubTestWnd/Test_Base_FileZip.h"

#include "SubTestWnd/Test_RenderEx_GradientSprite.h"
#include "SubTestWnd/Test_RenderEx_TiledSprite.h"
#include "SubTestWnd/Test_RenderEx_LaserSprite.h"
#include "SubTestWnd/Test_RenderEx_TurnCardSprite.h"
#include "SubTestWnd/Test_RenderEx_EraserSprite.h"
#include "SubTestWnd/Test_RenderEx_ScrollingBackgroundNode.h"
#include "SubTestWnd/Test_RenderEx_JoystickNode.h"
#include "SubTestWnd/Test_RenderEx_NumberCountNode.h"
#include "SubTestWnd/Test_RenderEx_SimpleShaderColorPainter.h"
#include "SubTestWnd/Test_RenderEx_CatmullRomSprite.h"
#include "SubTestWnd/Test_RenderEx_SwayNode.h"
#include "SubTestWnd/Test_RenderEx_BlurTextureFilter.h"
#include "SubTestWnd/Test_RenderEx_SpotLightSprite.h"


#include "SubTestWnd/Test_EffectNode_2DSoftShadow.h"
#include "SubTestWnd/Test_EffectNode_BreakSprite.h"
#include "SubTestWnd/Test_EffectNode_ColorAdjustSprite.h"
#include "SubTestWnd/Test_EffectNode_GhostLikeSprite.h"
#include "SubTestWnd/Test_EffectNode_Laser.h"
#include "SubTestWnd/Test_EffectNode_LightingBolt.h"
#include "SubTestWnd/Test_EffectNode_NormalMapped.h"
#include "SubTestWnd/Test_EffectNode_RippleSprite.h"
#include "SubTestWnd/Test_EffectNode_Ripple2DSprite.h"
#include "SubTestWnd/Test_EffectNode_ShatterSprite.h"
#include "SubTestWnd/Test_EffectNode_TailSprite.h"
#include "SubTestWnd/Test_EffectNode_TinyWindTerrain.h"

#include "SubTestWnd/Test_TMX_Isometric.h"
#include "SubTestWnd/Test_TMX_Hexagonal.h"
#include "SubTestWnd/Test_TMX_Orthogonal.h"
#include "SubTestWnd/Test_TMX_Flip.h"
//todo ...
//--------------------------------------------------------
//便捷返回场景名
#define CASE_TEST_ITEM(_TYPE_, _ITEM_)\
case eTestSceneType_##_TYPE_##_##_ITEM_:\
	return string("CTest_"+string(#_TYPE_)+"_"+string(#_ITEM_)+"Scene");\
	break

//便捷注册场景
#define QUICK_REGISTER_SCENE(_TYPE_, _ITEM_)\
REGISTER_SCENE_FUNC(CTest_##_TYPE_##_##_ITEM_##Scene)
//--------------------------------------------------------
static CCPoint s_tCurPos = CCPointZero;
//--------------------------------------------------------
static string GetSceneClassName( ENUM_TEST_SCENE_TYPE p_eType )
{
	if( p_eType >= eTestSceneType_Max )
		return NULL;

	string str;
	switch (p_eType)
	{
	case eTestSceneType_Default:
		str = "CExampleScene";
		return str;
		break;

	CASE_TEST_ITEM(GR, Examples);

	CASE_TEST_ITEM(Font, Test);

	CASE_TEST_ITEM(Database, Database);

	CASE_TEST_ITEM(SimpleNet, NetObj);

	CASE_TEST_ITEM(Json, Json);

	CASE_TEST_ITEM(SceneEx, UISceneEx);

	CASE_TEST_ITEM(3D, FKSimpleModel);

	CASE_TEST_ITEM(RichHtml, HtmlLabel);

	CASE_TEST_ITEM(Story, StoryLayer);

	CASE_TEST_ITEM(Pixel, Demo);

	CASE_TEST_ITEM(Shader, Demo1);
	CASE_TEST_ITEM(Shader, Demo2);

	CASE_TEST_ITEM(Network, FileDownLoader);
	CASE_TEST_ITEM(Network, HttpGet);
	CASE_TEST_ITEM(Network, TCP);
	CASE_TEST_ITEM(Network, UDP);

	CASE_TEST_ITEM(UI, AutoRenderMenuItemSprite);
	CASE_TEST_ITEM(UI, MenuItemColor);
	CASE_TEST_ITEM(UI, ToastLayer);
	CASE_TEST_ITEM(UI, SimpleRookieGuider);
	CASE_TEST_ITEM(UI, DynamicNumLabel);
	CASE_TEST_ITEM(UI, MenuItemProgressTimer);
	CASE_TEST_ITEM(UI, ProgressIndicator);
	CASE_TEST_ITEM(UI, ScrollableLayer);
	CASE_TEST_ITEM(UI, Table);

	CASE_TEST_ITEM(Action, Blur);
	CASE_TEST_ITEM(Action, ClipIn);
	CASE_TEST_ITEM(Action, Count);
	CASE_TEST_ITEM(Action, Flash);
	CASE_TEST_ITEM(Action, Missile);
	CASE_TEST_ITEM(Action, Shake);
	CASE_TEST_ITEM(Action, Shine);
	CASE_TEST_ITEM(Action, TreeFadeIn);
	CASE_TEST_ITEM(Action, TreeFadeOut);
	CASE_TEST_ITEM(Action, TrailMoveTo);
	CASE_TEST_ITEM(Action, CallFuncT);
	CASE_TEST_ITEM(Action, Curl);
	CASE_TEST_ITEM(Action, Suck);

	CASE_TEST_ITEM(UIWidget, DrawZOrder);
	CASE_TEST_ITEM(UIWidget, Button);
	CASE_TEST_ITEM(UIWidget, CheckBox);
	CASE_TEST_ITEM(UIWidget, ColorView);
	CASE_TEST_ITEM(UIWidget, ControlView);
	CASE_TEST_ITEM(UIWidget, ExpandableListView);
	CASE_TEST_ITEM(UIWidget, GradientView);
	CASE_TEST_ITEM(UIWidget, ImageView);
	CASE_TEST_ITEM(UIWidget, GridPageView);
	CASE_TEST_ITEM(UIWidget, GridView);
	CASE_TEST_ITEM(UIWidget, Label);
	CASE_TEST_ITEM(UIWidget, Layout);
	CASE_TEST_ITEM(UIWidget, ProgressBar);
	CASE_TEST_ITEM(UIWidget, ScrollView);
	CASE_TEST_ITEM(UIWidget, ToggleView);
	CASE_TEST_ITEM(UIWidget, ListView);
	CASE_TEST_ITEM(UIWidget, Slider);
	CASE_TEST_ITEM(UIWidget, TextRich);
	CASE_TEST_ITEM(UIWidget, LabelAtlas);
	CASE_TEST_ITEM(UIWidget, PageView);
	CASE_TEST_ITEM(UIWidget, TableView);

	CASE_TEST_ITEM(Base, AStar);
	CASE_TEST_ITEM(Base, UtilsI);
	CASE_TEST_ITEM(Base, Encode);
	CASE_TEST_ITEM(Base, DrawingPrimitives);
	CASE_TEST_ITEM(Base, CVSLoader);
	CASE_TEST_ITEM(Base, DecodeHtml);
	CASE_TEST_ITEM(Base, Localization);
	CASE_TEST_ITEM(Base, Memory);
	CASE_TEST_ITEM(Base, PinYin);
	CASE_TEST_ITEM(Base, ResourceLoader);
	CASE_TEST_ITEM(Base, SecureUserDefault);
	CASE_TEST_ITEM(Base, FileZip);

	CASE_TEST_ITEM(RenderEx, GradientSprite);
	CASE_TEST_ITEM(RenderEx, TiledSprite);
	CASE_TEST_ITEM(RenderEx, LaserSprite);
	CASE_TEST_ITEM(RenderEx, TurnCardSprite);
	CASE_TEST_ITEM(RenderEx, EraserSprite);
	CASE_TEST_ITEM(RenderEx, ScrollingBackgroundNode);
	CASE_TEST_ITEM(RenderEx, JoystickNode);
	CASE_TEST_ITEM(RenderEx, NumberCountNode);
	CASE_TEST_ITEM(RenderEx, SimpleShaderColorPainter);
	CASE_TEST_ITEM(RenderEx, CatmullRomSprite);
	CASE_TEST_ITEM(RenderEx, SwayNode);
	CASE_TEST_ITEM(RenderEx, BlurTextureFilter);
	CASE_TEST_ITEM(RenderEx, SpotLightSprite);

	CASE_TEST_ITEM(EffectNode, 2DSoftShadow);
	CASE_TEST_ITEM(EffectNode, BreakSprite);
	CASE_TEST_ITEM(EffectNode, ColorAdjustSprite);
	CASE_TEST_ITEM(EffectNode, GhostLikeSprite);
	CASE_TEST_ITEM(EffectNode, Laser);
	CASE_TEST_ITEM(EffectNode, LightingBolt);
	CASE_TEST_ITEM(EffectNode, NormalMapped);
	CASE_TEST_ITEM(EffectNode, RippleSprite);
	CASE_TEST_ITEM(EffectNode, Ripple2DSprite);
	CASE_TEST_ITEM(EffectNode, ShatterSprite);
	CASE_TEST_ITEM(EffectNode, TailSprite);
	CASE_TEST_ITEM(EffectNode, TinyWindTerrain);

	CASE_TEST_ITEM(TMX, Isometric);
	CASE_TEST_ITEM(TMX, Hexagonal);
	CASE_TEST_ITEM(TMX, Orthogonal);
	CASE_TEST_ITEM(TMX, Flip);
		// to do add...
	default:
		break;
	}

	return NULL;
}
//--------------------------------------------------------
CListControllerLayer::CListControllerLayer()
	: m_tBeginPos( CCPointZero )
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_szCloseBtnImagePath, s_szCloseBtnImagePath, this, menu_selector(CListControllerLayer::closeCallback) );
	CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

	pMenu->setPosition( CCPointZero );
	pCloseItem->setPosition(ccp( CVisibleRect::right().x - 50, CVisibleRect::bottom().y + 50));

	m_pItemMenu = CCMenu::create();
	for (int i = 0; i < static_cast<int>(eTestSceneType_Max); ++i)
	{
		CCLabelTTF* label = CCLabelTTF::create( FKCW_Base_StringConvert::a2u( g_aTestNames[i].c_str() ).c_str(), "Arial", 32);
		label->setColor( _GetColorByName(g_aTestNames[i]) );
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(CListControllerLayer::menuCallback));

		m_pItemMenu->addChild(pMenuItem, i + 10000);
		pMenuItem->setPosition( ccp( CVisibleRect::center().x, (CVisibleRect::top().y - (i + 1) * TEST_MENU_LINE_SPACE) ));
	}

	m_pItemMenu->setContentSize(CCSizeMake(CVisibleRect::getVisibleRect().size.width, (static_cast<int>(eTestSceneType_Max) + 1) * (TEST_MENU_LINE_SPACE)));
	m_pItemMenu->setPosition(s_tCurPos);
	addChild(m_pItemMenu);

	setTouchEnabled(true);

	addChild(pMenu, 1);

	FKCOLORLOG( eConsolTextColor_Blue | eConsolBKColor_Yellow, "创建ListControllerLayer成功" );
}
//--------------------------------------------------------
CListControllerLayer::~CListControllerLayer()
{

}
//--------------------------------------------------------
void CListControllerLayer::onEnter()
{
	CCLayer::onEnter();

	// 注册全部场景
	REGISTER_SCENE_FUNC(CExampleScene);

	QUICK_REGISTER_SCENE(GR, Examples);

	QUICK_REGISTER_SCENE(Font, Test);

	QUICK_REGISTER_SCENE(Database, Database);

	QUICK_REGISTER_SCENE(SimpleNet, NetObj);

	QUICK_REGISTER_SCENE(Json, Json);

	QUICK_REGISTER_SCENE(SceneEx, UISceneEx);

	QUICK_REGISTER_SCENE(3D, FKSimpleModel);

	QUICK_REGISTER_SCENE(RichHtml, HtmlLabel);

	QUICK_REGISTER_SCENE(Story, StoryLayer);

	QUICK_REGISTER_SCENE(Pixel, Demo);

	QUICK_REGISTER_SCENE(Shader, Demo1);
	QUICK_REGISTER_SCENE(Shader, Demo2);

	QUICK_REGISTER_SCENE(Network, FileDownLoader);
	QUICK_REGISTER_SCENE(Network, HttpGet);
	QUICK_REGISTER_SCENE(Network, TCP);
	QUICK_REGISTER_SCENE(Network, UDP);

	QUICK_REGISTER_SCENE(UI, AutoRenderMenuItemSprite);
	QUICK_REGISTER_SCENE(UI, MenuItemColor);
	QUICK_REGISTER_SCENE(UI, ToastLayer);
	QUICK_REGISTER_SCENE(UI, SimpleRookieGuider);
	QUICK_REGISTER_SCENE(UI, DynamicNumLabel);
	QUICK_REGISTER_SCENE(UI, MenuItemProgressTimer);
	QUICK_REGISTER_SCENE(UI, ProgressIndicator);
	QUICK_REGISTER_SCENE(UI, ScrollableLayer);
	QUICK_REGISTER_SCENE(UI, Table);

	QUICK_REGISTER_SCENE(Action, Blur);
	QUICK_REGISTER_SCENE(Action, ClipIn);
	QUICK_REGISTER_SCENE(Action, Count);
	QUICK_REGISTER_SCENE(Action, Flash);
	QUICK_REGISTER_SCENE(Action, Missile);
	QUICK_REGISTER_SCENE(Action, Shake);
	QUICK_REGISTER_SCENE(Action, Shine);
	QUICK_REGISTER_SCENE(Action, TreeFadeIn);
	QUICK_REGISTER_SCENE(Action, TreeFadeOut);
	QUICK_REGISTER_SCENE(Action, TrailMoveTo);
	QUICK_REGISTER_SCENE(Action, CallFuncT);
	QUICK_REGISTER_SCENE(Action, Curl);
	QUICK_REGISTER_SCENE(Action, Suck);

	QUICK_REGISTER_SCENE(UIWidget, DrawZOrder);
	QUICK_REGISTER_SCENE(UIWidget, Button);
	QUICK_REGISTER_SCENE(UIWidget, CheckBox);
	QUICK_REGISTER_SCENE(UIWidget, ColorView);
	QUICK_REGISTER_SCENE(UIWidget, ControlView);
	QUICK_REGISTER_SCENE(UIWidget, ExpandableListView);
	QUICK_REGISTER_SCENE(UIWidget, GradientView);
	QUICK_REGISTER_SCENE(UIWidget, ImageView);
	QUICK_REGISTER_SCENE(UIWidget, GridPageView);
	QUICK_REGISTER_SCENE(UIWidget, GridView);
	QUICK_REGISTER_SCENE(UIWidget, Label);
	QUICK_REGISTER_SCENE(UIWidget, Layout);
	QUICK_REGISTER_SCENE(UIWidget, ProgressBar);
	QUICK_REGISTER_SCENE(UIWidget, ScrollView);
	QUICK_REGISTER_SCENE(UIWidget, ToggleView);
	QUICK_REGISTER_SCENE(UIWidget, ListView);
	QUICK_REGISTER_SCENE(UIWidget, Slider);
	QUICK_REGISTER_SCENE(UIWidget, TextRich);
	QUICK_REGISTER_SCENE(UIWidget, LabelAtlas);
	QUICK_REGISTER_SCENE(UIWidget, PageView);
	QUICK_REGISTER_SCENE(UIWidget, TableView);

	QUICK_REGISTER_SCENE(Base, AStar);
	QUICK_REGISTER_SCENE(Base, UtilsI);
	QUICK_REGISTER_SCENE(Base, Encode);
	QUICK_REGISTER_SCENE(Base, DrawingPrimitives);
	QUICK_REGISTER_SCENE(Base, CVSLoader);
	QUICK_REGISTER_SCENE(Base, DecodeHtml);
	QUICK_REGISTER_SCENE(Base, Localization);
	QUICK_REGISTER_SCENE(Base, Memory);
	QUICK_REGISTER_SCENE(Base, PinYin);
	QUICK_REGISTER_SCENE(Base, ResourceLoader);
	QUICK_REGISTER_SCENE(Base, SecureUserDefault);
	QUICK_REGISTER_SCENE(Base, FileZip);

	QUICK_REGISTER_SCENE(RenderEx, GradientSprite);
	QUICK_REGISTER_SCENE(RenderEx, TiledSprite);
	QUICK_REGISTER_SCENE(RenderEx, LaserSprite);
	QUICK_REGISTER_SCENE(RenderEx, TurnCardSprite);
	QUICK_REGISTER_SCENE(RenderEx, EraserSprite);
	QUICK_REGISTER_SCENE(RenderEx, ScrollingBackgroundNode);
	QUICK_REGISTER_SCENE(RenderEx, JoystickNode);
	QUICK_REGISTER_SCENE(RenderEx, NumberCountNode);
	QUICK_REGISTER_SCENE(RenderEx, SimpleShaderColorPainter);
	QUICK_REGISTER_SCENE(RenderEx, CatmullRomSprite);
	QUICK_REGISTER_SCENE(RenderEx, SwayNode);
	QUICK_REGISTER_SCENE(RenderEx, BlurTextureFilter);
	QUICK_REGISTER_SCENE(RenderEx, SpotLightSprite);

	QUICK_REGISTER_SCENE(EffectNode, 2DSoftShadow);
	QUICK_REGISTER_SCENE(EffectNode, BreakSprite);
	QUICK_REGISTER_SCENE(EffectNode, ColorAdjustSprite);
	QUICK_REGISTER_SCENE(EffectNode, GhostLikeSprite);
	QUICK_REGISTER_SCENE(EffectNode, Laser);
	QUICK_REGISTER_SCENE(EffectNode, LightingBolt);
	QUICK_REGISTER_SCENE(EffectNode, NormalMapped);
	QUICK_REGISTER_SCENE(EffectNode, RippleSprite);
	QUICK_REGISTER_SCENE(EffectNode, Ripple2DSprite);
	QUICK_REGISTER_SCENE(EffectNode, ShatterSprite);
	QUICK_REGISTER_SCENE(EffectNode, TailSprite);
	QUICK_REGISTER_SCENE(EffectNode, TinyWindTerrain);

	QUICK_REGISTER_SCENE(TMX, Isometric);
	QUICK_REGISTER_SCENE(TMX, Hexagonal);
	QUICK_REGISTER_SCENE(TMX, Orthogonal);
	QUICK_REGISTER_SCENE(TMX, Flip);
	// todo...
}
//--------------------------------------------------------
void CListControllerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	m_tBeginPos = touch->getLocation();    
}
//--------------------------------------------------------
void CListControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->getLocation();    
	float nMoveY = touchLocation.y - m_tBeginPos.y;

	CCPoint curPos  = m_pItemMenu->getPosition();
	CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);

	if (nextPos.y < 0.0f)
	{
		m_pItemMenu->setPosition(CCPointZero);
		return;
	}

	if (nextPos.y > ((static_cast<int>(eTestSceneType_Max) + 1)* TEST_MENU_LINE_SPACE - CVisibleRect::getVisibleRect().size.height))
	{
		m_pItemMenu->setPosition(ccp(0, ((static_cast<int>(eTestSceneType_Max) + 1)* TEST_MENU_LINE_SPACE - CVisibleRect::getVisibleRect().size.height)));
		return;
	}

	m_pItemMenu->setPosition(nextPos);
	m_tBeginPos = touchLocation;
	s_tCurPos   = nextPos;
}
//--------------------------------------------------------
void CListControllerLayer::menuCallback(CCObject * pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder() - 10000;

	// 创建并执行子场景
	string szSceneClassName = GetSceneClassName(static_cast<ENUM_TEST_SCENE_TYPE>(nIdx));
	if ( szSceneClassName != "" )
	{
		FKCOLORLOG( eConsolBKColor_Gray | eConsolTextColor_Red , "尝试创建 : %s", szSceneClassName.c_str() );
		FKCW_SceneExMgr_SceneExMgr::sharedManager()->pushScene(LoadScene(szSceneClassName.c_str()));
	}
}
//--------------------------------------------------------
void CListControllerLayer::closeCallback(CCObject * pSender)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#else
		FKCW_SceneExMgr_SceneExMgr::sharedManager()->end();
	#endif
}
//--------------------------------------------------------
ccColor3B CListControllerLayer::_GetColorByName(std::string p_szName)
{
	CCArray a;
	a.addObjectsFromArray( &FKCW_Base_Utils::ComponentOfString(p_szName, '_') );
	if( a.count() >= 3 )
	{
		CCString* FKCWHead = (CCString*)a.objectAtIndex(0);
		CCString* SmallType = (CCString*)a.objectAtIndex(1);
		if(! FKCWHead->isEqual(ccs("FKCW")) )
			return ccc3(225,238,210);

		if(SmallType->isEqual(ccs("UI")))
			return ccc3(230, 155, 3);
		else if(SmallType->isEqual(ccs("Action")))
			return ccc3(209, 73, 78);
		else if(SmallType->isEqual(ccs("UIWidget")))
			return ccc3(131, 175, 155);
		else if(SmallType->isEqual(ccs("Base")))
			return ccc3(249, 205, 173);
		else if(SmallType->isEqual(ccs("EffectNode")))
			return ccc3(175, 215, 237);
		else if(SmallType->isEqual(ccs("RenderEx")))
			return ccc3(175, 18, 88);
		else if(SmallType->isEqual(ccs("GR")))
			return ccc3(130, 57, 53);
		else if(SmallType->isEqual(ccs("Database")))
			return ccc3(160, 191, 124);
		else if(SmallType->isEqual(ccs("Network")))
			return ccc3(255, 252, 153);
		else if(SmallType->isEqual(ccs("Json")))
			return ccc3(87, 105, 60);
		else if(SmallType->isEqual(ccs("Font")))
			return ccc3(230, 155, 3);
		else if(SmallType->isEqual(ccs("SimpleNet")))
			return ccc3(209, 73, 78);
		else if(SmallType->isEqual(ccs("SceneEx")))
			return ccc3(175, 215, 237);
		else if(SmallType->isEqual(ccs("3D")))
			return ccc3(64, 116, 52);
		else if(SmallType->isEqual(ccs("RichHtml")))
			return ccc3(249, 205, 173);
		else if(SmallType->isEqual(ccs("Story")))
			return ccc3(131, 175, 155);
		else if(SmallType->isEqual(ccs("Pixel")))
			return ccc3(175, 18, 88);
	}
	return ccc3(225,238,210);
}
//--------------------------------------------------------