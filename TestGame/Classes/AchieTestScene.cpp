//-------------------------------------------------------------------------
#include "AchieTestScene.h"
//-------------------------------------------------------------------------


CAchieTestScene::CAchieTestScene(void)
{
}
//-------------------------------------------------------------------------
// 初始异步加载资源
void CAchieTestScene::onLoadResources()
{
	//TODO-加载csv数据-2015年3月25日23:24:19

}
//-------------------------------------------------------------------------
// 其他逻辑初始化
void CAchieTestScene::onLoadScene()
{
	m_pWindow = FKCW_UIWidget_WidgetWindow::create();
	m_pWindow->setMultiTouchEnabled(true);

	m_pLoader = new FKCW_Base_CSVLoader();
	if( m_pLoader == NULL )
		return;

	//background
	m_pBackGroundImage = FKCW_UIWidget_ImageView::create(s_szLoginBgPath);
	m_pBackGroundImage->setScaleX(SCERRN_WIDTH / m_pBackGroundImage->getContentSize().width);
	m_pBackGroundImage->setScaleY(SCERRN_HEIGHT / m_pBackGroundImage->getContentSize().height);
	m_pBackGroundImage->setPosition( SCREEN_CENTER_POS);
	m_pWindow->addChild(m_pBackGroundImage);

	m_pListView = FKCW_UIWidget_ListView::create(CCSize(640, 350));
	//m_pListView->setBackgroundImage(s_szListViewBg);
	m_pListView->setPosition(SCREEN_CENTER_POS + ccp(0, 200));
	m_pListView->setDirection(eScrollViewDirectionVertical);
	m_pWindow->addChild(m_pListView);
	this->addChild(m_pWindow);

	// 退出场景按钮
	{
		CCMenu* pMenu = CCMenu::create();
		pMenu->setPosition(CCPointZero);
		addChild(pMenu);

		CCMenuItemImage* pBtn = CCMenuItemImage::create();
		CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("back_button_normal_e.png"); 
		pBtn->setNormalSpriteFrame(pFrame);
		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("back_button_touch_e.png"); 
		pBtn->setSelectedSpriteFrame(pFrame);
		pBtn->setTarget(this, menu_selector(CAchieTestScene::onClick));
		pBtn->setPosition(SCREEN_CENTER_POS + ccp(0,400));
		pMenu->addChild(pBtn);
	}

}
//-------------------------------------------------------------------------
// 当场景显示时调用
void CAchieTestScene::onEnterScene()
{
	//CAchieTestScene::onLoadCVS();
}
//-------------------------------------------------------------------------
// 当场景隐藏时调用
void CAchieTestScene::onExitScene()
{
	if( m_pLoader )
	{
		delete m_pLoader;
		m_pLoader = NULL;
	}
	removeChild(m_pWindow);
}
//-------------------------------------------------------------------------
void  CAchieTestScene::onLoadCVS()
{
	string strFilePath = FKCW_Base_StringConvert::a2u(s_szListViewCSV).c_str();
	m_pLoader->Clear();
	if( !m_pLoader->OpenAndResolveFile( strFilePath.c_str() ) )
	{
		FKLOG("需要解析的CVS文件不存在");
		return;
	}
	for( int i = m_pLoader->GetRowLength()-1; i >= 0; --i )
	{
		string str;
		for( int j = 0; j < m_pLoader->GetColLength(); ++j )
		{
			str += m_pLoader->GetData(i, j);
			str += "   ";
		}
		_AddToList( str.c_str() );
	}
}
//-------------------------------------------------------------------------
void  CAchieTestScene::_AddToList(const char* pStr)
{
	FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
	pLayout->setContentSize(CCSize(640, 30));

	FKCW_UIWidget_ColorView* pColor = FKCW_UIWidget_ColorView::create(ccc4(0, 128, 0, 255));
	pColor->setContentSize(CCSize(636, 28));
	pColor->setPosition(CCPoint(640/2, 30/2));
	pLayout->addChild(pColor);

	FKCW_UIWidget_Label* pLabel = FKCW_UIWidget_Label::create(
		FKCW_Base_StringConvert::a2u(pStr).c_str(), "", 30.0f);
	pLabel->setPosition(CCPoint(640 /2, 30 /2));
	pLayout->addChild(pLabel);
	m_pListView->insertNodeAtFront(pLayout);
	m_pListView->reloadData();
}
//-------------------------------------------------------------------------
void CAchieTestScene::onClick(CCObject* sender)
{
	FKCW_SceneExMgr_SceneExMgr::sharedManager()->popScene(this);
}
//-------------------------------------------------------------------------

