//-------------------------------------------------------------------------
#include "MsgBoxMgr.h"
#include "AudioBridge.h"
#include "GameCommon.h"
//-------------------------------------------------------------------------

#define MESSAGE_BOX_TIMEID 1000

CMsgBoxMgr::CMsgBoxMgr(void)
	:m_bIsShowing(false)
	,m_pHandle(NULL)
	,m_pRootWindow(NULL)
	,m_pMsgNode(NULL)
	,m_pAwardBtn(NULL)
	,m_nMsgKey(-1)
{
}


CMsgBoxMgr::~CMsgBoxMgr(void)
{
}
//-------------------------------------------------------------------------
bool CMsgBoxMgr::ShowAwardBox(
	IMsgBoxHandle* pHandle,FKCW_UIWidget_WidgetWindow* pRootWindow,int nAwardType,float nDuration)
{
	if (m_bIsShowing || nAwardType <= eAwardType_Min || eAwardType_Max <= nAwardType)
	{
		return !m_bIsShowing;
	}
	if (pHandle == NULL || pRootWindow == NULL)
	{
		return false;
	}
	m_pRootWindow = pRootWindow;
	m_pHandle = pHandle;

	__ShowAwardBox(nAwardType,nDuration);

	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->DestroyTimer(MESSAGE_BOX_TIMEID,this);
		pTimer->CreateTimer(MESSAGE_BOX_TIMEID,nDuration,this,1);
	}

	m_bIsShowing = true;
	return m_bIsShowing;
}
//-------------------------------------------------------------------------
bool CMsgBoxMgr::Show(
	IMsgBoxHandle* pHandle,FKCW_UIWidget_WidgetWindow* pRootWindow,int nType,float nDuration)
{
	if (m_bIsShowing)
	{
		return !m_bIsShowing;
	}
	if (pHandle == NULL || pRootWindow == NULL)
	{
		return false;
	}
	m_pRootWindow = pRootWindow;
	m_pHandle = pHandle;

	__ShowNormalBox(nType,nDuration);

	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->DestroyTimer(MESSAGE_BOX_TIMEID,this);
		pTimer->CreateTimer(MESSAGE_BOX_TIMEID,nDuration,this,1);
	}

	m_bIsShowing = true;
	return m_bIsShowing;
}
//-------------------------------------------------------------------------
void CMsgBoxMgr::Hide()
{
	m_bIsShowing = false;

	__Clear();

	if (m_pHandle)
	{
		m_pHandle->OnMessageBoxCallBack(m_nMsgKey);
	}
	m_pHandle = NULL;
}
//-------------------------------------------------------------------------
void CMsgBoxMgr::__Clear()
{
	if (m_pMsgNode != NULL)
	{
		m_pMsgNode->removeFromParentAndCleanup(true);
		m_pMsgNode = NULL;
	}

	if (m_pAwardBtn != NULL)
	{
		m_pAwardBtn->removeFromParentAndCleanup(true);
		m_pAwardBtn = NULL;
	}

	m_pRootWindow = NULL;

	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->DestroyTimer(MESSAGE_BOX_TIMEID,this);
	}
}
//-------------------------------------------------------------------------
void CMsgBoxMgr::__ShowNormalBox(int nType,float nDuration)
{
	if (m_pRootWindow == NULL)
	{
		return;
	}

	if (m_pMsgNode != NULL)
	{
		m_pMsgNode->removeFromParentAndCleanup(true);
		m_pMsgNode = NULL;
	}

	string strTitleImageName;
	string strItemImageName;
	m_nMsgKey = nType;

	switch (nType)
	{
	case eNormalType_GameSuccess:
		strTitleImageName = s_szSuccessTitle;
		break;

	default:
		break;
	}

	m_pMsgNode = CCNode::create();
	m_pMsgNode->setContentSize(m_pRootWindow->getContentSize());

	FKCW_UIWidget_ColorView* colorView = FKCW_UIWidget_ColorView::create(cc4BLACK);
	colorView->setPosition(SCREEN_CENTER_POS);
	colorView->setContentSize(CCSize(SCERRN_WIDTH, SCERRN_HEIGHT));
	colorView->setOpacity(160);
	m_pMsgNode->addChild(colorView);

	FKCW_UIWidget_ImageView* pBgImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szMsgBoxBg);
	pBgImage->setPosition(SCREEN_CENTER_POS);
	m_pMsgNode->addChild(pBgImage);

	FKCW_UIWidget_ImageView* pTitleImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(strTitleImageName.c_str());
	pTitleImage->setPosition(SCREEN_CENTER_POS + ccp(0,130));
	m_pMsgNode->addChild(pTitleImage);

	m_pAwardBtn = FKCW_UIWidget_Button::create();
	m_pAwardBtn->setNormalSpriteFrameName(s_szGoOnBtnBg);
	m_pAwardBtn->setSelectedSpriteFrameName(s_szGoOnBtnBg);
	m_pAwardBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(CMsgBoxMgr::OnBtnHandle));
	m_pAwardBtn->setPosition(SCREEN_CENTER_POS + ccp(0,-130));
	m_pAwardBtn->setTouchEnabled(true);

	m_pRootWindow->addChild(m_pMsgNode);
	m_pRootWindow->addChild(m_pAwardBtn);
}
//-------------------------------------------------------------------------
void CMsgBoxMgr::__ShowAwardBox(int nAwardType,float nDuration)
{
	if (m_pRootWindow == NULL)
	{
		return;
	}

	if (m_pMsgNode != NULL)
	{
		m_pMsgNode->removeFromParentAndCleanup(true);
		m_pMsgNode = NULL;
	}

	if (nAwardType <= eAwardType_Min || eAwardType_Max <= nAwardType)
	{
		return;
	}

	string strTitleImageName;
	string strItemImageName;
	int nNum = 0;

	m_nMsgKey = nAwardType;

	switch (nAwardType)
	{
	case eAwardType_LoginGift:
		strTitleImageName = s_szTitleLogin;
		strItemImageName = s_szLoginItem;
		nNum = 2;
		break;

	case eAwardType_NewGuyGift:
		strTitleImageName = s_szTitleNew;
		strItemImageName = s_szNewItem;
		nNum = 2;
		break;

	case eAwardType_PetGift:
		strTitleImageName = s_szTitleGift;
		strItemImageName = s_szGiftItem;
		nNum = 5000;
		break;

	default:
		break;
	}

	m_pMsgNode = CCNode::create();
	m_pMsgNode->setContentSize(m_pRootWindow->getContentSize());

	FKCW_UIWidget_ColorView* colorView = FKCW_UIWidget_ColorView::create(cc4BLACK);
	colorView->setPosition(SCREEN_CENTER_POS);
	colorView->setContentSize(CCSize(SCERRN_WIDTH, SCERRN_HEIGHT));
	colorView->setOpacity(160);
	m_pMsgNode->addChild(colorView);

	FKCW_UIWidget_ImageView* pBgImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szMsgBoxBg);
	pBgImage->setPosition(SCREEN_CENTER_POS);
	m_pMsgNode->addChild(pBgImage);

	FKCW_UIWidget_ImageView* pTitleImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(strTitleImageName.c_str());
	pTitleImage->setPosition(SCREEN_CENTER_POS + ccp(0,130));
	m_pMsgNode->addChild(pTitleImage);

	FKCW_UIWidget_ImageView* pItemImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(strItemImageName.c_str());
	pItemImage->setPosition(SCREEN_CENTER_POS + ccp(-110,0));
	m_pMsgNode->addChild(pItemImage);

	FKCW_UIWidget_ImageView* pXImage = FKCW_UIWidget_ImageView::createWithSpriteFrameName(s_szXimg);
	pXImage->setPosition(SCREEN_CENTER_POS);
	m_pMsgNode->addChild(pXImage);

	CCLabelAtlas* pAtlasLabel = CCLabelAtlas::create("", s_szYellowLabels, 30, 36, '0');
	pAtlasLabel->setPosition(SCREEN_CENTER_POS + ccp(110,5));
	m_pMsgNode->addChild(pAtlasLabel);

	pAtlasLabel->setString(CCString::createWithFormat("%d",nNum)->getCString());
	pAtlasLabel->setScale(1.5f);
	pAtlasLabel->setAnchorPoint(ccp(0.5,0.5));

	m_pAwardBtn = FKCW_UIWidget_Button::create();
	m_pAwardBtn->setNormalSpriteFrameName(s_szAwardBtn);
	m_pAwardBtn->setSelectedSpriteFrameName(s_szAwardBtn);
	m_pAwardBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(CMsgBoxMgr::OnBtnHandle));
	m_pAwardBtn->setPosition(SCREEN_CENTER_POS + ccp(0,-130));
	m_pAwardBtn->setTouchEnabled(true);

	m_pRootWindow->addChild(m_pMsgNode);
	m_pRootWindow->addChild(m_pAwardBtn);

}
//-------------------------------------------------------------------------
void CMsgBoxMgr::OnBtnHandle(CCObject* pSender)
{
	if (pSender == NULL)
	{
		return;
	}
	FKLOG("MsgBoxMgr::OnBtnHandle ing");

	CAudioBridge::GetInstance().PlayEffect(eSound_Button);

	Hide();
}
//-------------------------------------------------------------------------
// 定时器时间触发回调
void CMsgBoxMgr::OnTimer( unsigned int unTimerID )
{
	Hide();
}