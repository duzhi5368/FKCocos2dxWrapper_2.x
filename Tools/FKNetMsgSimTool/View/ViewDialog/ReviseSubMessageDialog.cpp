#include "ReviseSubMessageDialog.h"
#include "../../Resource.h"


ReviseSubMessageDialog* ReviseSubMessageDialog::create(short mainType, short subType)
{
	ReviseSubMessageDialog* pBuf = new ReviseSubMessageDialog();
	if (pBuf && pBuf->init(mainType, subType))
	{
		pBuf->autorelease();
		return pBuf;
	}
	delete pBuf;
	pBuf = NULL;
	return NULL;
}


ReviseSubMessageDialog::ReviseSubMessageDialog():
	m_pDeleget(NULL)
{

}

ReviseSubMessageDialog::~ReviseSubMessageDialog()
{

}

cocos2d::CCSize ReviseSubMessageDialog::GetDialogSize()
{
	return CCSize(350, 300);
}

bool ReviseSubMessageDialog::init(short mainType, short subType)
{
	if (!DialogBase::init())
	{
		return false;
	}

	m_mainType = mainType;
	m_subType = subType;

	this->setTitle(A2U("修改子消息").c_str());
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
	pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(ReviseSubMessageDialog::onButtonClick));
	pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	pButton->getLabel()->setFontSize(18);
	pButton->getLabel()->setString(A2U( "确认" ).c_str());
	this->addDialogChild(pButton);
	/********************************************************************************************/

	return true;
}

void ReviseSubMessageDialog::onButtonClick(CCObject* pSender)
{
	if (m_pDeleget != NULL)
	{
		short subType = this->getSubType();
		if (subType <= 0)
		{
			MessageBox(NULL, L"子类型输入错误: 正确的子类型应该是大于0的short类型", L"", 0);
		}
		else
		{
			m_pDeleget->onReviseSubMessageDialogSaveMessage(this, m_mainType, m_subType);
		}
	}
	this->removeFromParentAndCleanup(true);
}

void ReviseSubMessageDialog::setDelegate(ReviseSubMessageDialogDeleget* pDeleget)
{
	m_pDeleget = pDeleget;
}

void ReviseSubMessageDialog::setEditBoxSubType(string subType)
{
	m_pEditBoxSubType->setText(subType.c_str());
}

void ReviseSubMessageDialog::setEditBoxSubName(string subName)
{
	m_pEditBoxSubName->setText(subName.c_str());
}

void ReviseSubMessageDialog::setEditBoxFbm(string fbm)
{
	m_pEditBoxFbm->setText(fbm.c_str());
}

void ReviseSubMessageDialog::setEditBoxFbs(string fbs)
{
	m_pEditBoxFbs->setText(fbs.c_str());
}

short ReviseSubMessageDialog::getSubType()
{
	return atoi(m_pEditBoxSubType->getText());
}

string ReviseSubMessageDialog::getSubName()
{
	return m_pEditBoxSubName->getText();
}

short ReviseSubMessageDialog::getFbm()
{
	return atoi(m_pEditBoxFbm->getText());
}

short ReviseSubMessageDialog::getFbs()
{
	return atoi(m_pEditBoxFbs->getText());
}
