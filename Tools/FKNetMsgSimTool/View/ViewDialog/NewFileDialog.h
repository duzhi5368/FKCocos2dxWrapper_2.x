#ifndef _NEW_FILE_DIALOG_H_
#define _NEW_FILE_DIALOG_H_

#include "DialogBase.h"

class NewFileDialog;

class NewFileDialogDelegate
{
public:
	//保存消息委托
	virtual void onNewFileDialogDelegateSaveMessage(NewFileDialog* pSender, string pathAndFileName) = 0;
};

/**
 * NewFileDialog
 * 新建文件对话框
 */
class NewFileDialog : public DialogBase
{
public:
	//path: 文件夹绝对路径
	static NewFileDialog* create(string path);
	//绑定事件委托
	void setDelegate(NewFileDialogDelegate* pDelegate);
protected:
	NewFileDialog();
	virtual ~NewFileDialog();
	virtual bool init(string path);
	virtual CCSize GetDialogSize();

	//保存按钮回调
	void onSaveButtonClick(CCObject* pSender);
private:
	CCEditBox* m_pEditBoxFileName;
	NewFileDialogDelegate* m_pDelegate;
	string m_path;
};

#endif