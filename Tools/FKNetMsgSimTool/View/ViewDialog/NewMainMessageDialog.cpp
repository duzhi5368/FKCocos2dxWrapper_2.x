#include "NewMainMessageDialog.h"
#include "../../Resource.h"

NewMainMessageDialog::NewMainMessageDialog():
	m_pDelegate(NULL)
{

}

NewMainMessageDialog::~NewMainMessageDialog()
{

}

bool NewMainMessageDialog::init()
{
	if (!DialogBase::init())
	{
		return false;
	}

	this->setTitle(A2U("添加主类型").c_str());

	/********************************************************************************************/

	//主类型标签
	FKCW_UIWidget_Label* pMainTypeLabel = FKCW_UIWidget_Label::create(A2U("主类型(short)：").c_str(), "", 14);
	pMainTypeLabel->setAnchorPoint(ccp(0, 0.5));
	pMainTypeLabel->setPosition(ccp(20, getContentSize().height - 50));
	this->addDialogChild(pMainTypeLabel);

	//主类型编辑框
	CCScale9Sprite* pEditBoxMainTypeBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxMainType = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxMainTypeBK );
	m_pEditBoxMainType->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxMainType->setPosition( ccp(pMainTypeLabel->getPositionX() + pMainTypeLabel->getContentSize().width, pMainTypeLabel->getPositionY()) );
	m_pEditBoxMainType->setText( "" );
	m_pEditBoxMainType->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxMainType );

	/********************************************************************************************/

	//主类型描述
	FKCW_UIWidget_Label* pMainNameLabel = FKCW_UIWidget_Label::create(A2U("主类型描述：").c_str(), "", 14);
	pMainNameLabel->setAnchorPoint(ccp(0, 0.5));
	pMainNameLabel->setPosition(ccp(pMainTypeLabel->getPositionX(), m_pEditBoxMainType->getPositionY() - 50));
	this->addDialogChild(pMainNameLabel);

	//主类型描述编辑框
	CCScale9Sprite* pEditBoxMainNameBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxMainName = CCEditBox::create(
		CCSizeMake(230, 30), 
		pEditBoxMainNameBK );
	m_pEditBoxMainName->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxMainName->setPosition( ccp(pMainNameLabel->getPositionX() + pMainNameLabel->getContentSize().width, pMainNameLabel->getPositionY()) );
	m_pEditBoxMainName->setText( "" );
	m_pEditBoxMainName->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxMainName );

	/********************************************************************************************/

	//确定按钮
	FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 32), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	pButton->setAnchorPoint(ccp(0.5, 0));
	pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(NewMainMessageDialog::onButtonClick));
	pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	pButton->getLabel()->setFontSize(18);
	pButton->getLabel()->setString(A2U( "确认" ).c_str());
	this->addDialogChild(pButton);

	return true;
}

cocos2d::CCSize NewMainMessageDialog::GetDialogSize()
{
	return CCSize(370, 220);
}

void NewMainMessageDialog::onButtonClick( CCObject* pSender )
{
	if (m_pDelegate != NULL)
	{
		short mainType = atoi(m_pEditBoxMainType->getText());
		string mainName = m_pEditBoxMainName->getText();
		mainName = FKCW_Base_Utils::Trim(mainName);

		if(mainType <= 0)
		{
			MessageBox(NULL, L"您填入的主类型数据有误: 正确的主类型应该是大于0的short类型", L"", 0);
		}
		else
		{
			m_pDelegate->onNewMainMessageDialogSaveMessage(this, mainType, mainName);
		}
	}

	this->removeFromParentAndCleanup(true);
}

void NewMainMessageDialog::setDelegate( NewMainMessageDialogDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}
