#include "AttributeEditView.h"

#include "../../Resource.h"

bool AttributeEditView::init()
{
	if(!FKCW_UIWidget_Layout::init())
		return false;

	this->setBackgroundColor(ccc4(52, 53, 57, 255));
	this->setContentSize(CCSize(250, 245));
	
	/***************************************************************************************************/
	
	FKCW_UIWidget_Label* pAttNameLabel =  FKCW_UIWidget_Label::create(A2U("变量名").c_str(), "", s_iFontSizeSmall);
	pAttNameLabel->setAnchorPoint(ccp(0 , 0.5));
	pAttNameLabel->setPosition(ccp(15, getContentSize().height - pAttNameLabel->getContentSize().height - 15));
	this->addChild(pAttNameLabel);
	
	CCScale9Sprite* pEditBoxAttNameBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxAttName = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxAttNameBK );
	m_pEditBoxAttName->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxAttName->setPosition( ccp(pAttNameLabel->getPositionX() + 55, pAttNameLabel->getPositionY()) );
	m_pEditBoxAttName->setText( "" );
	this->addChild( m_pEditBoxAttName );

	/***************************************************************************************************/

	FKCW_UIWidget_Label* pTypeNameLabel = FKCW_UIWidget_Label::create(A2U("变量类型").c_str(), "", s_iFontSizeSmall);
	pTypeNameLabel->setAnchorPoint(ccp(0, 0.5));
	pTypeNameLabel->setPosition(ccp(pAttNameLabel->getPositionX(), pAttNameLabel->getPositionY() - 50));
	this->addChild(pTypeNameLabel);

	m_pTypeNameComboBox = ComboBox::create(CCSize(170, 30), 5);
	m_pTypeNameComboBox->setText("");
	m_pTypeNameComboBox->setAnchorPoint(ccp(0, 1));
	m_pTypeNameComboBox->setZOrder(1000);
	m_pTypeNameComboBox->setPosition(ccp(pTypeNameLabel->getPositionX() + 55, pTypeNameLabel->getPositionY() + 15));
	m_pTypeNameComboBox->addString("int");
	m_pTypeNameComboBox->addString("short");
	m_pTypeNameComboBox->addString("bool");
	m_pTypeNameComboBox->addString("float");
	m_pTypeNameComboBox->addString("string");
	this->addChild(m_pTypeNameComboBox);

	/***************************************************************************************************/
	
	FKCW_UIWidget_Label* pDataLabel =	  FKCW_UIWidget_Label::create(A2U("数  据").c_str(), "", s_iFontSizeSmall);
	pDataLabel->setAnchorPoint(ccp(0, 0.5));
	pDataLabel->setPosition(ccp(pTypeNameLabel->getPositionX(), pTypeNameLabel->getPositionY() - 50));
	this->addChild(pDataLabel);

	CCScale9Sprite* pEditBoxDataBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxData = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxDataBK );
	m_pEditBoxData->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxData->setPosition( ccp(pDataLabel->getPositionX() + 55, pDataLabel->getPositionY()) );
	m_pEditBoxData->setText( "" );
	this->addChild( m_pEditBoxData );

	/***************************************************************************************************/

	m_pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(225, 40), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	m_pButton->setAnchorPoint(ccp(0.5, 0));
	m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(AttributeEditView::onButtonClick));
	m_pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	m_pButton->getLabel()->setFontSize(s_iFontSizeNormal);
	m_pButton->getLabel()->setString(A2U( "保存" ).c_str());
	this->addChild(m_pButton);

	return true;
}

AttributeEditView::AttributeEditView():
	m_pDelegate(NULL)
{
}

AttributeEditView::~AttributeEditView()
{
}

void AttributeEditView::onButtonClick( CCObject* pSender )
{
	if(m_pDelegate != NULL)
	{
		string attName = m_pEditBoxAttName->getText();
		string typeName = m_pTypeNameComboBox->getText();
		string data = m_pEditBoxData->getText();

		attName = FKCW_Base_Utils::Trim(attName);
		typeName = FKCW_Base_Utils::Trim(typeName);
		data = FKCW_Base_Utils::Trim(data);

		//保护机制
		if (typeName.empty() || attName.empty())
		{
			MessageBox(NULL, L"变量名和类型不得为空", L"", 0);
		}
		else
		{
			m_pDelegate->onSaveMessage(this, attName, typeName, data);
		}
	}
}

void AttributeEditView::setDelegate( AttributeEditViewDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

void AttributeEditView::setAttName( string attName )
{
	m_pEditBoxAttName->setText(attName.c_str());
}

void AttributeEditView::setTypeName( string typeName )
{
	m_pTypeNameComboBox->setText(typeName.c_str());
}

void AttributeEditView::setData( string data )
{
	m_pEditBoxData->setText(data.c_str());
}

void AttributeEditView::setEnabled(bool bEnabled)
{
	FKCW_UIWidget_Layout::setEnabled(bEnabled);
	m_pButton->setEnabled(bEnabled);
	m_pEditBoxData->setEnabled(bEnabled);
	m_pEditBoxAttName->setEnabled(bEnabled);
	m_pTypeNameComboBox->setEnabled(bEnabled);
}
