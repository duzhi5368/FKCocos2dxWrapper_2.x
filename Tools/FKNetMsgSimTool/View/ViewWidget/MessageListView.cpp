#include "MessageListView.h"

#include "../../Resource.h"

MessageListView::MessageListView():
	m_pDelegate(NULL),
	m_pSelectColorView(NULL),
	m_isReloadData(false)
{
}

MessageListView::~MessageListView()
{
}

bool MessageListView::init()
{
	if (!FKCW_UIWidget_ExpandableListView::init())
		return false;
	//设置大小
	this->setContentSize(CCSize(250, 730));
	//设置背景
	this->setBackgroundColor(ccc4(52, 53, 57, 255));

	this->scheduleUpdate();

	return true; 
}

void MessageListView::addMainItem(string mainTitle)
{
	FKCW_UIWidget_ExpandableNode* pExpandableNode = FKCW_UIWidget_ExpandableNode::create();
	pExpandableNode->setContentSize(CCSize(this->getContentSize().width, 30));
	this->insertExpandableNodeAtLast(pExpandableNode);
	//标识
	MainItemObject* pMainItemObject = MainItemObject::create();
	pMainItemObject->mainTitle = mainTitle;
	pExpandableNode->setUserObject(pMainItemObject);
	
	FKCW_UIWidget_ColorView* pExpandableNodeBg = FKCW_UIWidget_ColorView::create(ccc4(25, 25, 25, 255));
	pExpandableNodeBg->setPosition(pExpandableNode->getContentSize()/2);
	pExpandableNodeBg->setContentSize(CCSize(this->getContentSize().width, 26));
	pExpandableNode->addChild(pExpandableNodeBg);

	//删除按钮
	FKCW_UIWidget_Button* pColBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(25, 25), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pColBtn->setTag(1111);
	pColBtn->setPosition(ccp(this->getContentSize().width - 20, 30 / 2));
	pColBtn->setText("-");
	pColBtn->setUserObject(CCString::create(mainTitle));
	pColBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onRemoveMainItemButtonClick));
	pExpandableNode->addChild(pColBtn);

	//修改按钮
	FKCW_UIWidget_Button* pReBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(40, 25), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pReBtn->setTag(2222);
	pReBtn->setPosition(ccp(this->getContentSize().width - 30 - pColBtn->getContentSize().width, 30 / 2));
	pReBtn->setTextFontSize(s_iFontSizeSuperSmall);
	pReBtn->setText(A2U("修改").c_str());
	pReBtn->setUserObject(CCString::create(mainTitle));
	pReBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onRemoveMainItemButtonClick));
	pExpandableNode->addChild(pReBtn);
	
	//标签
	FKCW_UIWidget_Label* pTitleLabel = FKCW_UIWidget_Label::create(mainTitle.c_str(), "", s_iFontSizeSmall);
	pTitleLabel->setPosition(CCPoint(5, 35/2));
	pTitleLabel->setAnchorPoint(CCPoint(0, 0.5));
	pTitleLabel->setTag(5050);	//标识
	pExpandableNode->addChild(pTitleLabel);
	
	this->autoReloadData();
}

void MessageListView::addSubItem(int index, string subTitle)
{
	FKCW_UIWidget_ExpandableNode* pExpandableNode = this->getExpandableNodeAtIndex(index);

	MainItemObject* pMainItemObject = (MainItemObject*)pExpandableNode->getUserObject();

	//设置标识
	SubItemUserObject* pSubItemUserObject = SubItemUserObject::create();
	pSubItemUserObject->mainTitle = pMainItemObject->mainTitle;
	pSubItemUserObject->subTitle = subTitle;
	pSubItemUserObject->pMainLabel = (FKCW_UIWidget_Label*)pExpandableNode->getChildByTag(5050);

	//布局
	FKCW_UIWidget_Layout* pLayout =  FKCW_UIWidget_Layout::create();
	pLayout->setContentSize(CCSize(this->getContentSize().width, 25));
	pExpandableNode->insertItemNodeAtLast(pLayout);

	//颜色框
	FKCW_UIWidget_ColorView* pColorView = FKCW_UIWidget_ColorView::create(ccc4(37, 37, 37, 255));
	pColorView->setTouchEnabled(true);
	pColorView->setContentSize(pLayout->getContentSize() - CCSize(0, 2));
	pColorView->setPosition(pLayout->getContentSize()/2);
	pColorView->setUserObject(pSubItemUserObject);	//标识
	pColorView->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onSelectedSubItemClick));
	pLayout->addChild(pColorView);

	//标签
	FKCW_UIWidget_Label* pLabel = FKCW_UIWidget_Label::create(subTitle.c_str(), "", s_iFontSizeSuperSmall);
	pLabel->setAnchorPoint(ccp(0, 0.5));
	pLabel->setPosition(ccp(20, pLayout->getContentSize().height/2));
	pLayout->addChild(pLabel);

	//删除按钮
	FKCW_UIWidget_Button* pColBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(20, 20), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pColBtn->setTag(8525);
	pColBtn->setPosition(ccp(pLayout->getContentSize().width - 30, pLayout->getContentSize().height/2));
	pColBtn->setTextFontSize(s_iFontSizeSuperSmall);
	pColBtn->setText("-");
	pColBtn->setUserObject(pSubItemUserObject);	//标识
	pColBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onRemoveSubItemButtonClick));
	pLayout->addChild(pColBtn);

	//修改按钮
	FKCW_UIWidget_Button* pRevBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(40, 20), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pRevBtn->setTag(1313);
	pRevBtn->setPosition(ccp(pLayout->getContentSize().width - 60, pLayout->getContentSize().height/2));
	pRevBtn->setTextFontSize(s_iFontSizeSuperSmall);
	pRevBtn->setText(A2U("修改").c_str());
	pRevBtn->setUserObject(pSubItemUserObject);	//标识
	pRevBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onRemoveSubItemButtonClick));
	pLayout->addChild(pRevBtn);

	this->autoReloadData();
}

void MessageListView::addSubItemAddButton(int index)
{
	FKCW_UIWidget_ExpandableNode* pExpandableNode = this->getExpandableNodeAtIndex(index);
	string mainTitle = ((FKCW_UIWidget_Label*)pExpandableNode->getChildByTag(5050))->getString();

	//添加按钮
	FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSize(this->getContentSize().width, 24),
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	pButton->setUserObject(CCString::create(mainTitle));
	pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(MessageListView::onAddSubItemButtonClick));
	pButton->initText(A2U("添加子消息").c_str(), "", s_iFontSizeSuperSmall);
	pButton->setPosition(CCPoint(this->getContentSize().width/2, 28/2));

	FKCW_UIWidget_Layout* pBtnLayout = FKCW_UIWidget_Layout::create(CCSize(this->getContentSize().width, 28));
	pBtnLayout->addChild(pButton);
	pExpandableNode->insertItemNodeAtLast(pBtnLayout);

	this->autoReloadData();
}

void MessageListView::onRemoveMainItemButtonClick(CCObject* pSender)
{
	CCNode* pNode = (CCNode*)pSender;
	if (pNode->getTag() == 1111)
	{
		int mb = MessageBox(NULL, L"确定要删除吗？", L"", MB_OKCANCEL);//1-是 2-否
		if (mb != 1)
			return;

		if (m_pDelegate != NULL)
		{
			CCString* mainTitle = (CCString*)((CCNode*)pSender)->getUserObject();
			string strMainTitle = mainTitle->getCString();
		
			clear();
			m_pDelegate->onMessageListViewRemoveMainItemMessage(this, strMainTitle);
			updateDataResouse();
		}
	}
	else if (pNode->getTag() == 2222)
	{
		if (m_pDelegate != NULL)
		{
			CCString* mainTitle = (CCString*)((CCNode*)pSender)->getUserObject();
			string strMainTitle = mainTitle->getCString();

			m_pDelegate->onMessageListViewReviseMainItemMessage(this, strMainTitle);
		}
	}
}

void MessageListView::onAddSubItemButtonClick(CCObject* pSender)
{
	if (m_pDelegate != NULL)
	{
		FKCW_UIWidget_Button* pButton = (FKCW_UIWidget_Button*)pSender;
		CCString* mainTitle = (CCString*)pButton->getUserObject();
		m_pDelegate->onMessageListViewAddSubItemMessage(this, mainTitle->getCString());
	}
}

void MessageListView::onSelectedSubItemClick(CCObject* pSender)
{
	if (m_pSelectColorView != NULL)
	{
		ccColor3B color = ccc3(37, 37, 37);
		m_pSelectColorView->setColor(color);
		//更改父标题
		SubItemUserObject* pSubItemUserObject = (SubItemUserObject*)m_pSelectColorView->getUserObject();
		pSubItemUserObject->pMainLabel->setColor(ccc3(240, 240, 240));
	}
	//更新全局变量
	m_pSelectColorView = (FKCW_UIWidget_ColorView*)pSender;
	//更改子标题颜色
	m_pSelectColorView->setColor(ccc3(217, 95, 95));
	//更改父标题颜色
	SubItemUserObject* pSubItemUserObject = (SubItemUserObject*)m_pSelectColorView->getUserObject();
	pSubItemUserObject->pMainLabel->setColor(ccc3(217, 95, 95));

	if (m_pDelegate != NULL)
	{
		SubItemUserObject* pUserObject = (SubItemUserObject*)((CCNode*)pSender)->getUserObject();
		m_pDelegate->onMessageListViewSelectedSubItemMessage(this, pUserObject->mainTitle, pUserObject->subTitle);
	}
}

void MessageListView::onRemoveSubItemButtonClick(CCObject* pSender)
{
	CCNode* pNode = (CCNode*)pSender;
	if (pNode->getTag() == 8525)
	{
		int mb = MessageBox(NULL, L"确定要删除吗？", L"", MB_OKCANCEL);//1-是 2-否
		if (mb != 1)
			return;

		if (m_pDelegate != NULL)
		{
			SubItemUserObject* pUserObject = (SubItemUserObject*)((CCNode*)pSender)->getUserObject();
			string strMainTitle = pUserObject->mainTitle;
			string strSubTitle = pUserObject->subTitle;

			this->clear();
			m_pDelegate->onMessageListViewRemoveSubItemMessage(this, strMainTitle, strSubTitle);
			this->updateDataResouse();
		}
	}
	else if(pNode->getTag() == 1313)
	{
		if (m_pDelegate != NULL)
		{
			//todo
			SubItemUserObject* pUserObject = (SubItemUserObject*)((CCNode*)pSender)->getUserObject();
			string strMainTitle = pUserObject->mainTitle;
			string strSubTitle = pUserObject->subTitle;

			m_pDelegate->onMessageListViewReviseSubItemMessage(this, strMainTitle, strSubTitle);
		}
	}
	
}

void MessageListView::updateDataResouse()
{
	if (m_pDelegate != NULL)
	{
		this->clear();
		m_pDelegate->onMessageListViewDataSourceMessage(this);
		autoReloadData();
	}
}

void MessageListView::setDelegate(MessageListViewDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

void MessageListView::clear()
{
	m_pSelectColorView = NULL;
	this->removeAllExpandableNodes();
	autoReloadData();
}

void MessageListView::update(float delta)
{
	if (m_isReloadData)
	{
		this->reloadData();
		m_isReloadData = false;
	}
}

void MessageListView::autoReloadData()
{
	m_isReloadData = true;
}
