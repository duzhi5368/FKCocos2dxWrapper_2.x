#ifndef _ATTRIBUTE_LIST_VIEW_H_
#define _ATTRIBUTE_LIST_VIEW_H_

#include "FKCWLib/FKCocos2dxWrapper.h"

class AttributeListView;

class AttributeListViewDelegate
{
public:
	//更新数据委托
	virtual void onAttributeListViewDataSourceMessage(AttributeListView* pSender) = 0;
	//选项选中委托
	virtual void onAttributeListViewSelectedItemMessage(AttributeListView* pSender, string attributeName) = 0;
	//删除选项委托
	virtual void onAttributeListViewRemoveItemMessage(AttributeListView* pSender, string attributeName) = 0;
	//添加属性
	virtual void onAttributeListViewAddItemMessage(AttributeListView* pSender) = 0;
};

class AttributeListView : public FKCW_UIWidget_ExpandableListView
{
public:
	CREATE_FUNC(AttributeListView);
public:
	void setTitle(string title);
	void setSubTitle(string title);

	void addItem(string title);
	//更新数据源 会响应onMessageListViewDataSourceMessage事件
	void updateDataResouse();
	//设置事件委托目标
	void setDelegate(AttributeListViewDelegate* pDelegate);
	void clear();

	//增加属性按钮
	void addItemAddButton();
protected:
	AttributeListView();
	~AttributeListView();
	virtual bool init();

	void onSelectedItemClick(CCObject* pSender);
	void onRemoveItemButtonClick(CCObject* pSender);
	void onAddItemButtonClick(CCObject* pSender);

	//下一帧刷新数据
	void autoReloadData();
	virtual void update(float delta);
private:
	AttributeListViewDelegate* m_pDelegate;
	FKCW_UIWidget_Label* m_pTitleLabel;
	FKCW_UIWidget_Label* m_pSubTitleLabel;
	FKCW_UIWidget_ColorView* m_pSelectColorView;	//当前选中的
	bool m_isReloadData;							//下一帧清空标识
};

#endif