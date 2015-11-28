#ifndef _NEW_ATTRIBUTE_DIALOG_H_
#define _NEW_ATTRIBUTE_DIALOG_H_

#include "DialogBase.h"
#include "../../FKExpand/ComboBox.h"

class NewAttributeDialog;

class NewAttributeDialogDelegate
{
public:
	//保存属性消息委托
	virtual void onNewAttributeDialogSaveMessage(NewAttributeDialog* pSender, string attName, string typeName, string data) = 0;
};


/**
 * NewAttributeDialog
 * 创建属性对话框
 */
class NewAttributeDialog : public DialogBase
{
public:
	CREATE_FUNC(NewAttributeDialog);
	//绑定事件委托
	void setDelegate(NewAttributeDialogDelegate* pDelegate);
protected:
	NewAttributeDialog();
	virtual ~NewAttributeDialog();
	virtual bool init();
	virtual CCSize GetDialogSize();
	//保存按钮回调
	void onSaveButtonClick(CCObject* pSender);
private:
	NewAttributeDialogDelegate* m_pDelegate;
	CCEditBox* m_pEditBoxAttName;
	ComboBox* m_pTypeNameComboBox;
	CCEditBox* m_pEditBoxData;
};

#endif