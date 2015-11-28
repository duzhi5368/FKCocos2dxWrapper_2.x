#ifndef _REVISE_SUB_MESSAGE_DIALOG_H_
#define _REVISE_SUB_MESSAGE_DIALOG_H_

#include "DialogBase.h"

class ReviseSubMessageDialog;

class ReviseSubMessageDialogDeleget
{
public:
	//修改子类型保存消息委托
	virtual void onReviseSubMessageDialogSaveMessage(ReviseSubMessageDialog* pSender, short mainType, short subType) = 0;
};

class ReviseSubMessageDialog : public DialogBase
{
public:
	static ReviseSubMessageDialog* create(short mainType, short subType);

	void setDelegate(ReviseSubMessageDialogDeleget* pDeleget);
	//设置子类型编辑框
	void setEditBoxSubType(string subType);
	//设置子类型名编辑框
	void setEditBoxSubName(string subName);
	//设置反馈主类型编辑框
	void setEditBoxFbm(string fbm);
	//设置反馈子类型编辑框
	void setEditBoxFbs(string fbs);

	//获取子类型
	short  getSubType();
	//获取子类型名
	string getSubName();
	//获取反馈主类型
	short  getFbm();
	//获取反馈子类型
	short  getFbs();
protected:
	ReviseSubMessageDialog();
	virtual ~ReviseSubMessageDialog();
	virtual bool init(short mainType, short subType);
	virtual CCSize GetDialogSize();

	void onButtonClick(CCObject* pSender);
protected:
	CCEditBox* m_pEditBoxSubType;
	CCEditBox* m_pEditBoxSubName;
	CCEditBox* m_pEditBoxFbm;
	CCEditBox* m_pEditBoxFbs;
	ReviseSubMessageDialogDeleget* m_pDeleget;
	short m_mainType;
	short m_subType;
};

#endif