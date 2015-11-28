#ifndef _REVISE_MAIN_MESSAGE_DIALOG_H_
#define _REVISE_MAIN_MESSAGE_DIALOG_H_

#include "DialogBase.h"

class ReviseMainMessageDialog;

class ReviseMainMessageDialogDelegate
{
public:
	//更改主类型事件委托
	virtual void onReviseMainMessageDialogSaveMessage(ReviseMainMessageDialog* pSender, short oldMainType, short mainType, string mainName) = 0;
};

/**
 * ReviseMainMessageDialog
 * 修改主类型对话框
 */
class ReviseMainMessageDialog : public DialogBase
{
public:
	static ReviseMainMessageDialog* create(short mainType, string mainName);
	void setDelegate(ReviseMainMessageDialogDelegate* pDelegate);
protected:
	ReviseMainMessageDialog();
	virtual ~ReviseMainMessageDialog();
	virtual bool init(short mainType, string mainName);
	virtual CCSize GetDialogSize();

	void onButtonClick(CCObject* pSender);
protected:
	short m_mainType;
	ReviseMainMessageDialogDelegate* m_pDelegate;
	CCEditBox* m_pEditBoxMainType;
	CCEditBox* m_pEditBoxMainName;
};

#endif