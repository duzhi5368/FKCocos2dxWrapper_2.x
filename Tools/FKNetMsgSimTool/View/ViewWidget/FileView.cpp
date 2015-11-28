#include "FileView.h"
#include "../../FKExpand/ComboBox.h"
#include "../../FKExpand/FileDirDialog.h"

bool FileView::init()
{
	if(!FKCW_UIWidget_WidgetWindow::init())
		return false;
	
	/************************************************************************/
	/* 文件菜单	                                                            */
	/************************************************************************/
	m_pFileMenuComboBox = ComboBox::create(CCSize(70, 32), 3);
	m_pFileMenuComboBox->setTag(1111);	//标识
	m_pFileMenuComboBox->setPosition(CCSize(70, 25)/2);
	m_pFileMenuComboBox->setText(A2U("文件").c_str());
	this->addChild(m_pFileMenuComboBox);
	
	m_pFileMenuComboBox->addString(A2U("新建").c_str());
	m_pFileMenuComboBox->addString(A2U("文件..").c_str());
	m_pFileMenuComboBox->addString(A2U("文件夹..").c_str());

	m_pFileMenuComboBox->setOnComboBoxChangeListener(this, fkcw_uiwidget_combobox_change_selector(FileView::onFileComboBoxChange));

	/************************************************************************/
	/* 文件选择框                                                             */
	/************************************************************************/
	m_pFileSelectComboBox = ComboBox::create(CCSize(1080, 32), 3);
	m_pFileSelectComboBox->setTag(2222);	//标识
	m_pFileSelectComboBox->setText(A2U("请点击左侧文件菜单,新建或载入配置文件").c_str());
	m_pFileSelectComboBox->setPosition(CCSize(1080, 25)/2 + ccp(70+5, 0));
	this->addChild(m_pFileSelectComboBox);

	m_pFileSelectComboBox->setOnComboBoxChangeListener(this, fkcw_uiwidget_combobox_change_selector(FileView::onFileComboBoxChange));

	return true;
}

void FileView::onFileComboBoxChange(CCObject* pSender, int tag, const char* text)
{
	ComboBox* pComboBox = (ComboBox*)pSender;

	if (pComboBox->getTag() == 1111)	//文件菜单
	{
		if(tag == 0)		//新建
		{
			string strFilePath = OpenDir(L"请选择文件保存目录");
			if(!strFilePath.empty())
			{
				if (m_pDelegate != NULL)
				{
					m_pDelegate->onFileViewNewFileMessage(A2U(strFilePath.c_str()));
				}
			}
		}
		else if (tag == 1)	//文件
		{
			string strFilePath = OpenFile();
			if (!strFilePath.empty())
			{
				//遍历目录下所有xml文件
				vector<string> dfsFileNames = dfsFolder(FKCW_Base_Utils::DeleteLastPathOfComponment(strFilePath));
				//清空ComboBox
				m_pFileSelectComboBox->clear();
				//向ComboBox添加目录下所有文件
				for (unsigned int i=0; i<dfsFileNames.size(); i++)
				{
					m_pFileSelectComboBox->addString(A2U(dfsFileNames.at(i).c_str()).c_str());
				}
				m_pFileSelectComboBox->setText(A2U(strFilePath.c_str()).c_str());

				if (m_pDelegate != NULL)
				{
					m_pDelegate->onFileViewOpenFileMessage(A2U(strFilePath.c_str()));
				}
			}
		}
		else if(tag == 2)	//文件夹
		{
			string strFilePath = OpenDir(L"请选择配置文件文件夹");
			if (!strFilePath.empty())
			{
				//遍历目录下所有xml文件
				vector<string> dfsFileNames = dfsFolder(strFilePath);
				//清空ComboBox
				m_pFileSelectComboBox->clear();
				//向ComboBox添加目录下所有文件
				for (unsigned int i=0; i<dfsFileNames.size(); i++)
				{
					m_pFileSelectComboBox->addString(A2U(dfsFileNames.at(i).c_str()).c_str());
				}
				m_pFileSelectComboBox->setText(A2U("请点击该处选择配置文件").c_str());
			}
		}
	}

	else if(pComboBox->getTag() == 2222)	//文件选择框
	{
		if (m_pDelegate != NULL)
		{
			string strFilePath = m_pFileSelectComboBox->getText();
			m_pDelegate->onFileViewOpenFileMessage(strFilePath);
		}
	}
}

void FileView::setDelegate(FileViewDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

FileView::FileView():
	m_pDelegate(NULL)
{
}

void FileView::setText(const char* title)
{
	m_pFileSelectComboBox->setText(title);
}
