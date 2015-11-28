#include "AttributeListView.h"

#include "../../Resource.h"

#define EXPANDABLE_NODE_BG 321

AttributeListView::AttributeListView():
	m_pDelegate(NULL),
	m_isReloadData(false),
	m_pSelectColorView(NULL),
	m_pTitleLabel(NULL)
{
}

AttributeListView::~AttributeListView()
{
}

bool AttributeListView::init()
{
	if(!FKCW_UIWidget_ExpandableListView::init())
		return false;

	//设置大小
	this->setContentSize(CCSize(250, 360));
	this->setBackgroundColor(ccc4(52, 53, 57, 255));

	this->scheduleUpdate();

	//扩展节点
	FKCW_UIWidget_ExpandableNode* pExpandableNode = FKCW_UIWidget_ExpandableNode::create();
	pExpandableNode->setContentSize(CCSize(this->getContentSize().width, 28*2));
	this->insertExpandableNodeAtLast(pExpandableNode);

	//背景
	FKCW_UIWidget_ColorView* pExpandableNodeBg = FKCW_UIWidget_ColorView::create(ccc4(25, 25, 25, 255));
	pExpandableNodeBg->setPosition(pExpandableNode->getContentSize()/2);
	pExpandableNodeBg->setContentSize(CCSize(this->getContentSize().width, 27*2));
	pExpandableNode->addChild(pExpandableNodeBg);
	pExpandableNodeBg->setTag(EXPANDABLE_NODE_BG);
	pExpandableNodeBg->setVisible(false);

	//标题标签
	m_pTitleLabel = FKCW_UIWidget_Label::create(A2U("标题").c_str(), "", s_iFontSizeNormal);
	m_pTitleLabel->setAnchorPoint(CCPoint(0, 0));
	m_pTitleLabel->setPosition(CCPoint(5, 27+5));
	m_pTitleLabel->setFontSize(s_iFontSizeSmall);
	pExpandableNode->addChild(m_pTitleLabel);

	//子标题标签
	m_pSubTitleLabel = FKCW_UIWidget_Label::create(A2U("子标题").c_str(), "", s_iFontSizeSuperSmall);
	m_pSubTitleLabel->setAnchorPoint(CCPoint(0, 0));
	m_pSubTitleLabel->setPosition(CCPoint(5, 10));
	m_pSubTitleLabel->setFontSize(s_iFontSizeSuperSmall);
	pExpandableNode->addChild(m_pSubTitleLabel);

	this->autoReloadData();

	return true;
}

void AttributeListView::setDelegate(AttributeListViewDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

void AttributeListView::setTitle(string title)
{
	m_pTitleLabel->setString(title.c_str());
}

void AttributeListView::addItem(string title)
{
	FKCW_UIWidget_ExpandableNode* pExpandableNode = this->getExpandableNodeAtIndex(0);

	//布局
	FKCW_UIWidget_Layout* pLayout =  FKCW_UIWidget_Layout::create();
	pLayout->setContentSize(CCSize(this->getContentSize().width, 25));
	pExpandableNode->insertItemNodeAtLast(pLayout);

	//颜色框
	FKCW_UIWidget_ColorView* pColorView = FKCW_UIWidget_ColorView::create(ccc4(37, 37, 37, 255));
	pColorView->setTouchEnabled(true);
	pColorView->setContentSize(pLayout->getContentSize() - CCSize(0, 2));
	pColorView->setPosition(pLayout->getContentSize()/2);
	pColorView->setUserObject(CCString::create(title));
	pColorView->setOnClickListener(this, fkcw_uiwidget_click_selector(AttributeListView::onSelectedItemClick));
	pLayout->addChild(pColorView);

	//标签
	FKCW_UIWidget_Label* pLabel = FKCW_UIWidget_Label::create(title.c_str(), "", s_iFontSizeSuperSmall);
	pLabel->setAnchorPoint(ccp(0, 0.5));
	pLabel->setPosition(ccp(20, pLayout->getContentSize().height/2));
	pLayout->addChild(pLabel);

	//删除按钮
	FKCW_UIWidget_Button* pColBtn = FKCW_UIWidget_Button::createWith9Sprite(CCSize(20, 20), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pColBtn->setPosition(ccp(pLayout->getContentSize().width - 30, pLayout->getContentSize().height/2));
	pColBtn->setText("-");
	pColBtn->setUserObject(CCString::create(title));	//标识
	pColBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(AttributeListView::onRemoveItemButtonClick));
	pLayout->addChild(pColBtn);

	this->expand(0);
	autoReloadData();
}


void AttributeListView::onSelectedItemClick(CCObject* pSender)
{
	if (m_pSelectColorView != NULL)
	{
		ccColor3B color = ccc3(37, 37, 37);
		m_pSelectColorView->setColor(color);
	}

	//更新全局变量
	m_pSelectColorView = (FKCW_UIWidget_ColorView*)pSender;
	//更改子标题颜色
	m_pSelectColorView->setColor(ccc3(217, 95, 95));

	if (m_pDelegate != NULL)
	{
		CCString* pUserObject = (CCString*)((CCNode*)pSender)->getUserObject();
		m_pDelegate->onAttributeListViewSelectedItemMessage(this, pUserObject->getCString());
	}
}


void AttributeListView::update(float delta)
{
	if (m_isReloadData)
	{
		this->reloadData();
		m_isReloadData = false;
	} 
}

void AttributeListView::autoReloadData()
{
	m_isReloadData = true;
}

void AttributeListView::updateDataResouse()
{
	if (m_pDelegate != NULL)
	{
		this->clear();
		m_pDelegate->onAttributeListViewDataSourceMessage(this);
		autoReloadData();
	}
}

void AttributeListView::clear()
{
	m_pSelectColorView = NULL;
	this->getExpandableNodeAtIndex(0)->removeAllItemNodes();
	this->expand(0);
	
	FKCW_UIWidget_ColorView* pColorView = (FKCW_UIWidget_ColorView*)this->getExpandableNodeAtIndex(0)->getChildByTag(EXPANDABLE_NODE_BG);
	pColorView->setVisible(false);

	autoReloadData();
}

void AttributeListView::onRemoveItemButtonClick(CCObject* pSender)
{
	FKCW_UIWidget_Button* pColBtn = (FKCW_UIWidget_Button*)pSender;
	
	if (m_pDelegate != NULL)
	{
		int mb = MessageBox(NULL, L"确定要删除吗？", L"", MB_OKCANCEL);//1-是 2-否
		if (mb != 1)
			return;

		CCString* pUserObject = (CCString*)pColBtn->getUserObject();
		string attName = pUserObject->getCString();
		clear();
		m_pDelegate->onAttributeListViewRemoveItemMessage(this, attName);
		m_pDelegate->onAttributeListViewDataSourceMessage(this);
		autoReloadData();
	}
}

void AttributeListView::addItemAddButton()
{
	FKCW_UIWidget_ExpandableNode* pExpandableNode = this->getExpandableNodeAtIndex(0);
	
	FKCW_UIWidget_ColorView* pColorView = (FKCW_UIWidget_ColorView*)pExpandableNode->getChildByTag(EXPANDABLE_NODE_BG);
	pColorView->setVisible(true);

	//布局
	FKCW_UIWidget_Layout* pLayout =  FKCW_UIWidget_Layout::create();
	pLayout->setContentSize(CCSize(this->getContentSize().width, 25));
	pExpandableNode->insertItemNodeAtLast(pLayout);

	//添加按钮
	FKCW_UIWidget_Button* pColBtn = FKCW_UIWidget_Button::createWith9Sprite(pLayout->getContentSize(), s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pColBtn->setPosition(ccp(pLayout->getContentSize().width/2, pLayout->getContentSize().height/2));
	pColBtn->setTextFontSize(14);
	pColBtn->setText(A2U("添加属性").c_str());
	pColBtn->setOnClickListener(this, fkcw_uiwidget_click_selector(AttributeListView::onAddItemButtonClick));
	pLayout->addChild(pColBtn);

	this->expand(0);
	autoReloadData();
}

void AttributeListView::onAddItemButtonClick( CCObject* pSender )
{
	if (m_pDelegate != NULL)
	{
		m_pDelegate->onAttributeListViewAddItemMessage(this);
	}
}

void AttributeListView::setSubTitle( string title )
{
	m_pSubTitleLabel->setString(title.c_str());
}
