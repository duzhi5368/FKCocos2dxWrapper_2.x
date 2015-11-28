#include "DialogBase.h"

#include "../../Resource.h"

DialogBase::DialogBase()
{

}

DialogBase::~DialogBase()
{

}

bool DialogBase::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
	this->setTouchEnabled(true);

	FKCW_UIWidget_WidgetWindow* pWindow = FKCW_UIWidget_WidgetWindow::create();
	this->addChild(pWindow);

	m_pLayout = FKCW_UIWidget_Layout::create(GetDialogSize());
	m_pLayout->setBackgroundColor(ccc4(27, 27, 27, 255));
	pWindow->addChild(m_pLayout);

	FKCW_UIWidget_Button* pCloseButton = FKCW_UIWidget_Button::create(s_szCloseButton);
	pCloseButton->setAnchorPoint(ccp(1, 1));
	pCloseButton->setPosition(ccp(m_pLayout->getContentSize().width - 5, m_pLayout->getContentSize().height - 5));
	pCloseButton->setOnClickListener(this, fkcw_uiwidget_click_selector(DialogBase::onCloseButtonClick));
	m_pLayout->addChild(pCloseButton);

	m_pLabel = FKCW_UIWidget_Label::create("", "", 16);
	m_pLabel->setPosition(ccp(m_pLayout->getContentSize().width/2, m_pLayout->getContentSize().height - 12));
	m_pLayout->addChild(m_pLabel);

	return true;
}

void DialogBase::setTitle( const char* title )
{
	m_pLabel->setString(title);
}

bool DialogBase::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void DialogBase::onCloseButtonClick( CCObject* pSender )
{
	this->removeFromParentAndCleanup(true);
}

void DialogBase::addDialogChild( CCNode* pChild )
{
	m_pLayout->addChild(pChild);
}

CCSize DialogBase::getContentSize()
{
	return m_pLayout->getContentSize();
}

void DialogBase::setDialogColor( ccColor4B color )
{
	m_pLayout->setBackgroundColor(color);
}
