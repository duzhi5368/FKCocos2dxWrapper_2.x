#include "NewSubMessageDialog.h"

#include "../../Resource.h"

NewSubMessageDialog* NewSubMessageDialog::create( short mainType )
{
	NewSubMessageDialog* pBuf = new NewSubMessageDialog();

	if(pBuf && pBuf->init(mainType))
	{
		pBuf->autorelease();
		return pBuf;
	}
	delete pBuf;
	pBuf = NULL;
	return NULL;
}

cocos2d::CCSize NewSubMessageDialog::GetDialogSize()
{
	return CCSize(350, 300);
}

NewSubMessageDialog::NewSubMessageDialog():
	m_pDelegate(NULL)
{

}

NewSubMessageDialog::~NewSubMessageDialog()
{

}

bool NewSubMessageDialog::init(short mainType)
{
	if (!DialogBase::init())
	{
		return false;
	}
	
	m_mainType = mainType;

	this->setTitle(A2U("添加子消息").c_str());
	/********************************************************************************************/
	//子类型标签
	FKCW_UIWidget_Label* pSubTypeLabel = FKCW_UIWidget_Label::create(A2U("子类型(short)：").c_str(), "", 14);
	pSubTypeLabel->setAnchorPoint(ccp(0, 0.5));
	pSubTypeLabel->setPosition(ccp(50, getContentSize().height - 50));
	this->addDialogChild(pSubTypeLabel);

	//子类型编辑框
	CCScale9Sprite* pEditBoxSubTypeBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxSubType = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxSubTypeBK );
	m_pEditBoxSubType->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxSubType->setPosition( ccp(pSubTypeLabel->getPositionX() + pSubTypeLabel->getContentSize().width, pSubTypeLabel->getPositionY()) );
	m_pEditBoxSubType->setText( "" );
	m_pEditBoxSubType->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxSubType );
	/********************************************************************************************/

	//子类描述标签
	FKCW_UIWidget_Label* pSubNameLabel = FKCW_UIWidget_Label::create(A2U("子类型描述：").c_str(), "", 14);
	pSubNameLabel->setAnchorPoint(ccp(0, 0.5));
	pSubNameLabel->setPosition(ccp(pSubTypeLabel->getPositionX(), pSubTypeLabel->getPositionY() - 50));
	this->addDialogChild(pSubNameLabel);

	//子类型描述编辑框
	CCScale9Sprite* pEditBoxSubNameBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxSubName = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxSubNameBK );
	m_pEditBoxSubName->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxSubName->setPosition( ccp(pSubNameLabel->getPositionX()+pSubNameLabel->getContentSize().width , pSubNameLabel->getPositionY()) );
	m_pEditBoxSubName->setText( "" );
	m_pEditBoxSubName->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxSubName );

	/********************************************************************************************/

	//反馈主类型标签
	FKCW_UIWidget_Label* pFbmLabel = FKCW_UIWidget_Label::create(A2U("反馈主类型：").c_str(), "", 14);
	pFbmLabel->setAnchorPoint(ccp(0, 0.5));
	pFbmLabel->setPosition(ccp(pSubNameLabel->getPositionX(), pSubNameLabel->getPositionY() - 50));
	this->addDialogChild(pFbmLabel);

	//反馈主类型编辑框
	CCScale9Sprite* pEditBoxFbmBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxFbm = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxFbmBK );
	m_pEditBoxFbm->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxFbm->setPosition( ccp(pFbmLabel->getPositionX()+pFbmLabel->getContentSize().width , pFbmLabel->getPositionY()) );
	m_pEditBoxFbm->setText( "" );
	m_pEditBoxFbm->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxFbm );

	/********************************************************************************************/

	//反馈子类型述标签
	FKCW_UIWidget_Label* pFbsLabel = FKCW_UIWidget_Label::create(A2U("反馈子类型：").c_str(), "", 14);
	pFbsLabel->setAnchorPoint(ccp(0, 0.5));
	pFbsLabel->setPosition(ccp(pFbmLabel->getPositionX(), pFbmLabel->getPositionY() - 50));
	this->addDialogChild(pFbsLabel);

	//反馈子类型编辑框
	CCScale9Sprite* pEditBoxFbsBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxFbs = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxFbsBK );
	m_pEditBoxFbs->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxFbs->setPosition( ccp(pFbsLabel->getPositionX()+pFbsLabel->getContentSize().width , pFbsLabel->getPositionY()) );
	m_pEditBoxFbs->setText( "" );
	m_pEditBoxFbs->setTouchPriority( -255 );
	this->addDialogChild( m_pEditBoxFbs );

	/********************************************************************************************/
	//确定按钮
	FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 32), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	pButton->setAnchorPoint(ccp(0.5, 0));
	pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(NewSubMessageDialog::onButtonClick));
	pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	pButton->getLabel()->setFontSize(18);
	pButton->getLabel()->setString(A2U( "确认" ).c_str());
	this->addDialogChild(pButton);
	/********************************************************************************************/
	return true;
}

void NewSubMessageDialog::setDelgate( NewSubMessageDialogDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

void NewSubMessageDialog::onButtonClick(CCObject* pSender)
{
	if(m_pDelegate != NULL)
	{
		short subType = atoi(m_pEditBoxSubType->getText());
		string subName = m_pEditBoxSubName->getText();
		subName = FKCW_Base_Utils::Trim(subName);
		short fbm = atoi(m_pEditBoxFbm->getText());
		short fbs = atoi(m_pEditBoxFbs->getText());

		if (subType <= 0)
		{
			MessageBox(NULL, L"子类型输入错误", L"", 0);
		}
		else
		{
			m_pDelegate->onNewSubMessageDialogSaveMessage(this, m_mainType, atoi(m_pEditBoxSubType->getText()), m_pEditBoxSubName->getText(), atoi(m_pEditBoxFbm->getText()), atoi(m_pEditBoxFbs->getText()));
	
		}
	}
	this->removeFromParentAndCleanup(true);
}