#ifndef _ATTRIBUTE_EDIT_VIEW_H_
#define _ATTRIBUTE_EDIT_VIEW_H_

#include "FKCWLib/FKCocos2dxWrapper.h"

#include "../../FKExpand/ComboBox.h"

#include "cocos-ext.h"
using namespace cocos2d::extension;

class AttributeEditView;

class AttributeEditViewDelegate
{
public:
	//属性编辑保存委托
	virtual void onSaveMessage(AttributeEditView* pAttributeEditView, string attName, string typeName, string data) = 0;
};

/**
 * AttributeEditView
 * 属性编辑面板
 */
class AttributeEditView : public FKCW_UIWidget_Layout
{
public:
	CREATE_FUNC(AttributeEditView);
public:
	virtual void setEnabled(bool bEnabled);

	void setDelegate(AttributeEditViewDelegate* pDelegate);
	//设置变量名
	void setAttName(string attName);
	//设置变量类型
	void setTypeName(string typeName);
	//设置数据
	void setData(string data);
protected:
	AttributeEditView();
	virtual ~AttributeEditView();
	virtual bool init();

	void onButtonClick(CCObject* pSender);
private:
	AttributeEditViewDelegate* m_pDelegate;
	CCEditBox* m_pEditBoxAttName;
	ComboBox* m_pTypeNameComboBox;
	CCEditBox* m_pEditBoxData;
	FKCW_UIWidget_Button* m_pButton;
};

#endif