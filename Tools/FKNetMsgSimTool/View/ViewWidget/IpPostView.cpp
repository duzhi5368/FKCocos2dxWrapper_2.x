#include "IpPostView.h"

#include "../../Resource.h"

IpPostView::IpPostView():
	m_pDelegate(NULL)
{

}

IpPostView::~IpPostView()
{

}

bool IpPostView::init()
{
	if (!FKCW_UIWidget_Layout::init())
	{
		return false;
	}

	this->setContentSize(CCSize(637, 32));

	//IP输入框
	CCScale9Sprite* pEditBoxIPBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxIP = CCEditBox::create(
		CCSizeMake(300, this->getContentSize().height), 
		pEditBoxIPBK );
	m_pEditBoxIP->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxIP->setPosition( ccp(0, this->getContentSize().height/2) );
	m_pEditBoxIP->setFontSize(s_iFontSizeNormal);
	m_pEditBoxIP->setText( A2U("127.0.0.1").c_str());
	this->addChild( m_pEditBoxIP );

	//Post输入框
	CCScale9Sprite* pEditBoxPostBK = CCScale9Sprite::create(s_szSprite9Btn1Path);
	m_pEditBoxPost = CCEditBox::create(
		CCSizeMake(145, this->getContentSize().height), 
		pEditBoxPostBK );
	m_pEditBoxPost->setAnchorPoint(ccp(0, 0.5));
	m_pEditBoxPost->setPosition( ccp(m_pEditBoxIP->getContentSize().width + 1, this->getContentSize().height/2) );
	m_pEditBoxPost->setFontSize(s_iFontSizeNormal);
	m_pEditBoxPost->setText( A2U("8888").c_str());
	this->addChild( m_pEditBoxPost );

	//确认按钮
	m_pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(this->getContentSize().width - pEditBoxIPBK->getContentSize().width - pEditBoxPostBK->getContentSize().width, this->getContentSize().height), 
		s_szSprite9Btn1Path, s_szSprite9Btn2Path );
	m_pButton->setAnchorPoint(ccp(0, 0.5));
	m_pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(IpPostView::onButtonClick));
	m_pButton->setPosition(ccp(m_pEditBoxIP->getContentSize().width +  m_pEditBoxPost->getContentSize().width + 1, this->getContentSize().height/2));
	m_pButton->getLabel()->setFontSize(25.0f);
	m_pButton->getLabel()->setFontSize(s_iFontSizeSmall);
	m_pButton->getLabel()->setString(A2U( "确认" ).c_str());
	this->addChild(m_pButton);

	return true;
}

void IpPostView::setButtonText( const char* text )
{
	m_pButton->setText(text);
}

void IpPostView::setDeleagte( IpPostViewDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}


//判断IP是否合法
bool is_valid_ip(const char *ip) 
{ 
	int section = 0;  //每一节的十进制值 
	int dot = 0;       //几个点分隔符 
	int last = -1;     //每一节中上一个字符 
	while(*ip)
	{ 
		if(*ip == '.')
		{ 
			dot++; 
			if(dot > 3)
			{ 
				return 0; 
			} 
			if(section >= 0 && section <=255)
			{ 
				section = 0; 
			}else{ 
				return 0; 
			} 
		}else if(*ip >= '0' && *ip <= '9')
		{ 
			section = section * 10 + *ip - '0'; 
			if(last == '0')
			{ 
				return 0; 
			} 
		}else{ 
			return 0; 
		} 
		last = *ip; 
		ip++;        
	}

	if(section >= 0 && section <=255)
	{ 
		if(3 == dot)
		{
			section = 0; 
			return 0;
		}
	} 
	return 1; 
}

void IpPostView::onButtonClick( CCObject* pSender )
{
	if (m_pDelegate != NULL)
	{
		short post = FKCW_Base_Utils::AtoI(m_pEditBoxPost->getText());
		if (post <= 0 || post > 65535)
		{
			MessageBox(NULL, L"端口号输入错误: 正确的端口号应该在0~62235之间", L"", 0);
		}
		/* IP判断
		else if(!is_valid_ip(m_pEditBoxIP->getText()))
		{
			MessageBox(NULL, L"IP错误", L"", 0);
		}
		*/
		else
		{
			m_pDelegate->onIpPostButtonClickMessage(this, m_pEditBoxIP->getText(), post);
		}
	}
}