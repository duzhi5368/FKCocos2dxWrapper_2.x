#include "LogView.h"

LogView* LogView::create( CCSize size )
{
	LogView *pRet = new LogView();
	if (pRet && pRet->init(size))
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

LogView::LogView():
	m_isSafeMode(false)
{

}

LogView::~LogView()
{

}

bool LogView::init( CCSize size )
{
	if (!FKCW_UIWidget_Layout::init())
	{
		return false;
	}
	
	this->setContentSize(size);

	//默认灰色
	m_pListView = FKCW_UIWidget_ListView::create(size);
	m_pListView->setBackgroundColor(ccc4(52, 52, 57, 255));
	m_pListView->setDirection(eScrollViewDirectionVertical);
	m_pListView->setPosition(size/2);
	this->addChild(m_pListView);

	//标题背景
	FKCW_UIWidget_ColorView* pColorView = FKCW_UIWidget_ColorView::create(ccc4(37, 37, 37, 255));
	pColorView->setContentSize(CCSize(size.width, 25));
	pColorView->setAnchorPoint(ccp(0.5, 0));
	pColorView->setPosition(ccp(size.width/2, size.height));
	this->addChild(pColorView);

	m_pLabel = CCLabelTTF::create("", "", 14);
	ccColor3B color;
	color.r = 250;
	color.g = 250;
	color.b = 250;
	m_pLabel->setColor(color);
	m_pLabel->setPosition(pColorView->getContentSize()/2);
	pColorView->addChild(m_pLabel);

	return true;
}

void LogView::log( const char * clog )
{
	log(ccWHITE, clog);
}

void LogView::log( ccColor3B fontColor, const char * clog )
{
	FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();

	CCLabelTTF* label = CCLabelTTF::create(clog, "", 12);
	label->setColor(fontColor);
	label->setPositionX(label->getContentSize().width/2.0f + 10);
	label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	pLayout->setContentSize(label->getContentSize() + CCSize(0, 6));
	pLayout->addChild(label);

	m_pListView->insertNodeAtLast(pLayout);

	//最大日志容量
	if(m_pListView->getNodeCount() > 500)
		m_pListView->removeFrontNode();

	m_pListView->reloadData();
	m_pListView->setContentOffsetToBottom();
}

void LogView::log( ccColor3B fontColor, ccColor4B bkColor, const char * clog )
{
	FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
	pLayout->setBackgroundColor(bkColor);

	CCLabelTTF* label = CCLabelTTF::create(clog, "", 12);
	label->setColor(fontColor);
	label->setPositionX(label->getContentSize().width/2.0f + 10);
	label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	pLayout->setContentSize(label->getContentSize() + CCSize(0, 6));
	pLayout->addChild(label);

	m_pListView->insertNodeAtLast(pLayout);

	//最大日志容量
	if(m_pListView->getNodeCount() > 500)
		m_pListView->removeFrontNode();

	m_pListView->reloadData();
	m_pListView->setContentOffsetToBottom();
}

void LogView::log(string clog)
{
	log(clog.c_str());
}

void LogView::log(ccColor3B fontColor, string clog)
{
	log(fontColor, clog.c_str());
}

void LogView::log(ccColor3B fontColor, ccColor4B bkColor, string clog)
{
	log(fontColor, bkColor, clog.c_str());
}

void LogView::clearLog()
{
	m_pListView->removeAllNodes();
	m_pListView->reloadData();
}

void LogView::setTitle( const char * title )
{
	m_pLabel->setString(title);
}

void LogView::openSafeMode()
{
	this->scheduleUpdate();
}

void LogView::update(float delta)
{
	//线程锁 todo...
	for (unsigned int i=0; i<safeLogStringStack.size(); i++)
	{
		SafeModeStackItem item = safeLogStringStack.at(i);
		this->log(item.fontColor, item.bkColor, item.log);
	}
	safeLogStringStack.clear();
}

void LogView::safeModeLog(const char * clog)
{
	SafeModeStackItem item;
	item.log = clog;
	item.fontColor = ccWHITE;
	item.bkColor = ccc4(0, 0, 0, 0);

	safeLogStringStack.push_back(item);
}

void LogView::safeModeLog(ccColor3B fontColor, const char * clog)
{
	SafeModeStackItem item;
	item.log = clog;
	item.fontColor = fontColor;
	item.bkColor = ccc4(0, 0, 0, 0);

	safeLogStringStack.push_back(item);
}

void LogView::safeModeLog(ccColor3B fontColor, ccColor4B bkColor, const char * clog)
{
	SafeModeStackItem item;
	item.log = clog;
	item.fontColor = fontColor;
	item.bkColor = bkColor;

	safeLogStringStack.push_back(item);
}

void LogView::safeModeLog(string clog)
{
	safeModeLog(clog.c_str());
}

void LogView::safeModeLog(ccColor3B fontColor, string clog)
{
	safeModeLog(fontColor, clog.c_str());
}

void LogView::safeModeLog(ccColor3B fontColor, ccColor4B bkColor, string clog)
{
	safeModeLog(fontColor, bkColor, clog.c_str());
}

