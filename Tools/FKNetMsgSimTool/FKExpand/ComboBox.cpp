#include "ComboBox.h"

#include "../resource.h"

bool ComboBox::init( CCSize &size , int spreadSize)
{
	if (!FKCW_UIWidget_Layout::init())
	{
		return false;
	}

	m_select = -1;
	m_pListener = NULL;
	m_listViewIsVisible = false;
	m_everySize = size;

	setContentSize(CCSize(size.width, size.height * (spreadSize+1)));

	m_pButton = FKCW_UIWidget_Button::createWith9Sprite(size, s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	m_pButton->setZOrder(1000);
	m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(ComboBox::onButtonClickListener));
	m_pButton->setTextFontSize(size.height * 0.6f);
	m_pButton->setText("ComboBox");
	m_pButton->setAnchorPoint(ccp(0, 1));
	m_pButton->setPosition(0, size.height * (spreadSize+1));
	this->addChild(m_pButton);
	
	m_listView = FKCW_UIWidget_ListView::create(CCSize(size.width, size.height * spreadSize));
	m_listView->setDirection(eScrollViewDirectionVertical);
	m_listView->setAnchorPoint(ccp(0, 1));
	m_listView->setPosition(0, size.height * spreadSize + 5);
	m_listView->setVisible(m_listViewIsVisible);
	this->addChild(m_listView);

	return true;
}

void ComboBox::onButtonClickListener( CCObject* pSender )
{
	m_listViewIsVisible = !m_listViewIsVisible;
	m_listView->setVisible(m_listViewIsVisible);

	//ÉèÖÃÊÇ·ñÏÔÊ¾±³¾°
	if (m_listViewIsVisible)
	{
		this->setBackgroundColor(ccc4(255, 255, 255, 100));
	}
	else
	{
		this->setBackgroundColor(ccc4(255, 255, 255, 0));
	}
	
}

void ComboBox::onListClickListener( CCObject* pSender )
{
	m_listViewIsVisible = false;
	m_listView->setVisible(m_listViewIsVisible);

	FKCW_UIWidget_Button* pButton = (FKCW_UIWidget_Button*)pSender;

	m_select = pButton->getTag();
	m_pButton->setText(pButton->getText());

	//Òþ²Ø±³¾°
	this->setBackgroundColor(ccc4(255, 255, 255, 0));

	if(m_pListener != NULL)
	{
		(m_pListener->*m_handler)(this, m_select, pButton->getText());
	}
}

void ComboBox::addString( const char* text )
{
	int count = m_listView->getNodeCount();

	FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(m_everySize, s_szSprite9Btn1Path, s_szSprite9Btn2Path);
	pButton->setTextFontSize(s_iFontSizeNormal);
	pButton->setText(text);
	pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(ComboBox::onListClickListener));
	pButton->setTag(count);
	m_listView->insertNodeAtLast(pButton);
	m_listView->reloadData();
}

void ComboBox::setOnComboBoxChangeListener( CCObject* pListener, SEL_ComboBoxChangeHandler handler )
{
	m_pListener = pListener;
	m_handler = handler;
}

void ComboBox::clear()
{
	m_select = -1;
	m_pButton->setText("");
	m_listView->removeAllNodes();
	m_listView->reloadData();
	m_listViewIsVisible = false;
	m_listView->setVisible(false);
}

int ComboBox::getSize()
{
	return m_listView->getNodeCount();
}

const char* ComboBox::getText()
{
	return m_pButton->getText();
}

void ComboBox::setText( const char* text )
{
	m_pButton->setText(text);
}

ComboBox* ComboBox::create(CCSize& size, int spreadSize)
{
	ComboBox *pRet = new ComboBox();
	if (pRet && pRet->init(size, spreadSize))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
