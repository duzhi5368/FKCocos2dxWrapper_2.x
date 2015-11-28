#include "NewAttributeDialog.h"
#include "../../Resource.h"

NewAttributeDialog::NewAttributeDialog():
	m_pDelegate(NULL)
{

}

NewAttributeDialog::~NewAttributeDialog()
{

}

cocos2d::CCSize NewAttributeDialog::GetDialogSize()
{
	return CCSize(320, 320);
}

bool NewAttributeDialog::init()
{
	if (!DialogBase::init())
	{
		return false;
	}

	this->setTitle(A2U("添加属性").c_str());

	/***************************************************************************************************/

	FKCW_UIWidget_Label* pAttNameLabel =  FKCW_UIWidget_Label::create(A2U("变量名").c_str(), "", 14);
	pAttNameLabel->setAnchorPoint(ccp(0 , 0.5));
	pAttNameLabel->setPosition(ccp(35, getContentSize().height - pAttNameLabel->getContentSize().height - 50));
	this->addDialogChild(pAttNameLabel);

	CCScale9Sprite* pEditBoxAttNameBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxAttName = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxAttNameBK );
	m_pEditBoxAttName->setTouchPriority(-255);
	m_pEditBoxAttName->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxAttName->setPosition( ccp(pAttNameLabel->getPositionX() + 55, pAttNameLabel->getPositionY()) );
	m_pEditBoxAttName->setText( "" );
	this->addDialogChild( m_pEditBoxAttName );

	/***************************************************************************************************/

	FKCW_UIWidget_Label* pDataLabel =	  FKCW_UIWidget_Label::create(A2U("数  据").c_str(), "", 14);
	pDataLabel->setAnchorPoint(ccp(0, 0.5));
	pDataLabel->setPosition(ccp(pAttNameLabel->getPositionX(), pAttNameLabel->getPositionY() - 50));
	this->addDialogChild(pDataLabel);

	CCScale9Sprite* pEditBoxDataBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxData = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxDataBK );
	m_pEditBoxData->setTouchPriority(-255);
	m_pEditBoxData->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxData->setPosition( ccp(pDataLabel->getPositionX() + 55, pDataLabel->getPositionY()) );
	m_pEditBoxData->setText( "" );
	this->addDialogChild( m_pEditBoxData );

	/***************************************************************************************************/

	FKCW_UIWidget_Label* pTypeNameLabel = FKCW_UIWidget_Label::create(A2U("变量类型").c_str(), "", 14);
	pTypeNameLabel->setAnchorPoint(ccp(0, 0.5));
	pTypeNameLabel->setPosition(ccp(pDataLabel->getPositionX(), pDataLabel->getPositionY() - 50));
	this->addDialogChild(pTypeNameLabel);

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
	this->addDialogChild(m_pTypeNameComboBox);

	/***************************************************************************************************/

	FKCW_UIWidget_Button* m_pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(225, 40), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	m_pButton->setAnchorPoint(ccp(0.5, 0));
	m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(NewAttributeDialog::onSaveButtonClick));
	m_pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	m_pButton->getLabel()->setFontSize(25.0f);
	m_pButton->getLabel()->setString(A2U( "保存" ).c_str());
	this->addDialogChild(m_pButton);

	return true;
}

void NewAttributeDialog::setDelegate( NewAttributeDialogDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

void NewAttributeDialog::onSaveButtonClick(CCObject* pSender)
{
	if (m_pDelegate != NULL)
	{
		string attName = m_pEditBoxAttName->getText();
		string typeName = m_pTypeNameComboBox->getText();
		string data = m_pEditBoxData->getText();

		attName = FKCW_Base_Utils::Trim(attName);
		typeName = FKCW_Base_Utils::Trim(typeName);
		data = FKCW_Base_Utils::Trim(data);

		if (attName.empty() || typeName.empty() || data.empty())
		{
			MessageBox(NULL, L"不能填入空内容!", L"", 0);
		}
		else
		{
			m_pDelegate->onNewAttributeDialogSaveMessage(this, attName, typeName, data);
		}
	}
	this->removeFromParentAndCleanup(true);
}