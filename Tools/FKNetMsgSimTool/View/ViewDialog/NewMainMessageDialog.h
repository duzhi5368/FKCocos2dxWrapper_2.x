#ifndef _NEW_MAIN_MESSAGE_DIALOG_H_
#define _NEW_MAIN_MESSAGE_DIALOG_H_

#include "DialogBase.h"

class NewMainMessageDialog;

class NewMainMessageDialogDelegate
{
public:
	//新建主类型 保存消息委托
	virtual void onNewMainMessageDialogSaveMessage(NewMainMessageDialog* pSender, short mainType, string mainName) = 0;
};

/**
 * NewMainMessageDialog
 * 新建主类型对话框
 */
class NewMainMessageDialog : public DialogBase
{
public:
	CREATE_FUNC(NewMainMessageDialog);
	//设置事件委托
	void setDelegate(NewMainMessageDialogDelegate* pDelegate);
protected:
	NewMainMessageDialog();
	~NewMainMessageDialog();
	virtual bool	init();
	virtual CCSize	GetDialogSize();

	void onButtonClick(CCObject* pSender);
private:
	CCEditBox* m_pEditBoxMainType;
	CCEditBox* m_pEditBoxMainName;

	NewMainMessageDialogDelegate* m_pDelegate;
};

#endif