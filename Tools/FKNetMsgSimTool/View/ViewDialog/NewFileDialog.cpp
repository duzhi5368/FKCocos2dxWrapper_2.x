#include "NewFileDialog.h"
#include "../../Resource.h"

NewFileDialog* NewFileDialog::create(string path)
{
	NewFileDialog* pBuf = new NewFileDialog();
	if (pBuf && pBuf->init(path))
	{
		pBuf->autorelease();
		return pBuf;
	}
	delete pBuf;
	pBuf = NULL;
	return NULL;
}

NewFileDialog::NewFileDialog():
	m_pDelegate(NULL)
{
}

NewFileDialog::~NewFileDialog()
{

}

bool NewFileDialog::init(string path)
{
	if (!DialogBase::init())
	{
		return false;
	}
	
	m_path = path;

	this->setTitle(A2U("新建文件名").c_str());

	FKCW_UIWidget_Label* pAttNameLabel =  FKCW_UIWidget_Label::create(A2U("文件名").c_str(), "", 14);
	pAttNameLabel->setAnchorPoint(ccp(0 , 0.5));
	pAttNameLabel->setPosition(ccp(35, getContentSize().height - pAttNameLabel->getContentSize().height - 50));
	this->addDialogChild(pAttNameLabel);

	CCScale9Sprite* pEditBoxAttNameBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxFileName = CCEditBox::create(
		CCSizeMake(170, 30), 
		pEditBoxAttNameBK );
	m_pEditBoxFileName->setTouchPriority(-255);
	m_pEditBoxFileName->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxFileName->setPosition( ccp(pAttNameLabel->getPositionX() + 55, pAttNameLabel->getPositionY()) );
	m_pEditBoxFileName->setText( "*.xml" );
	this->addDialogChild( m_pEditBoxFileName );

	FKCW_UIWidget_Button* m_pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(225, 40), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	m_pButton->setAnchorPoint(ccp(0.5, 0));
	m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(NewFileDialog::onSaveButtonClick));
	m_pButton->setPosition(ccp(this->getContentSize().width/2, 10));
	m_pButton->getLabel()->setFontSize(25.0f);
	m_pButton->getLabel()->setString(A2U( "保存" ).c_str());
	this->addDialogChild(m_pButton);

	return true;
}

cocos2d::CCSize NewFileDialog::GetDialogSize()
{
	return CCSize(330, 180);
}

void NewFileDialog::onSaveButtonClick(CCObject* pSender)
{
	if (m_pDelegate != NULL)
	{
		string pathAndFileName = m_path+"\\"+m_pEditBoxFileName->getText();
		m_pDelegate->onNewFileDialogDelegateSaveMessage(this, pathAndFileName);
	}

	this->removeFromParentAndCleanup(true);
}

void NewFileDialog::setDelegate(NewFileDialogDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}