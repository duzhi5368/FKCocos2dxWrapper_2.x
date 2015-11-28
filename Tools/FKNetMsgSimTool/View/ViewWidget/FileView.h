#ifndef _FILE_VIEW_H_
#define _FILE_VIEW_H_

#include "FKCWLib/FKCocos2dxWrapper.h"

class ComboBox;

class FileViewDelegate
{
public:
	virtual void onFileViewNewFileMessage(string strFilePathName) = 0;
	virtual void onFileViewOpenFileMessage(string strFilePathName) = 0;
};

/**
 * FileView
 * 文件面板
 */
class FileView : public FKCW_UIWidget_WidgetWindow
{
public:
	CREATE_FUNC(FileView);
	//设置事件委托目标
	void setDelegate(FileViewDelegate* pDelegate);
	//设置文件选择框文字
	void setText(const char* title);
protected:
	FileView();
	virtual bool init();
private:
	void onFileComboBoxChange(CCObject* pSender, int tag, const char* text);
private:
	FileViewDelegate* m_pDelegate;
	ComboBox* m_pFileMenuComboBox;
	ComboBox* m_pFileSelectComboBox;
};

#endif