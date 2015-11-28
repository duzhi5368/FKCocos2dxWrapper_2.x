#ifndef _NEW_SUB_MESSAGE_DIALOG_
#define _NEW_SUB_MESSAGE_DIALOG_

#include "DialogBase.h"

class NewSubMessageDialog;

class NewSubMessageDialogDelegate
{
public:
	//保存子类型消息委托事件
	virtual void onNewSubMessageDialogSaveMessage(NewSubMessageDialog* pSender, short mainType, short subType, string subName, short fbm, short fbs) = 0;
};

class NewSubMessageDialog : public DialogBase
{
public:
	static NewSubMessageDialog* create(short mainType);
	void setDelgate(NewSubMessageDialogDelegate* pDelegate);
protected:
	NewSubMessageDialog();
	virtual ~NewSubMessageDialog();
	virtual bool init(short mainType);
	virtual CCSize GetDialogSize();

	void onButtonClick(CCObject* pSender);
protected:
	short m_mainType;
	CCEditBox* m_pEditBoxSubType;
	CCEditBox* m_pEditBoxSubName;
	CCEditBox* m_pEditBoxFbm;
	CCEditBox* m_pEditBoxFbs;
	NewSubMessageDialogDelegate* m_pDelegate;
};

#endif