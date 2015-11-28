#ifndef _MESSAGE_LIST_VIEW_
#define _MESSAGE_LIST_VIEW_

#include "FKCWLib/FKCocos2dxWrapper.h"

class MessageListView;

class MainItemObject : public CCObject
{
public:
	CREATE_FUNC(MainItemObject);
	virtual bool init(){return true;}
public:
	string mainTitle;
};

class SubItemUserObject : public CCObject
{
public:
	CREATE_FUNC(SubItemUserObject);
	virtual bool init(){return true;}
public:
	string mainTitle;
	string subTitle;
	FKCW_UIWidget_Label* pMainLabel;
};

class MessageListViewDelegate
{
public:
	MessageListViewDelegate(){}
	virtual ~MessageListViewDelegate(){}
	//更新数据委托
	virtual void onMessageListViewDataSourceMessage(MessageListView* pSender) = 0;
	//选中子类型委托
	virtual void onMessageListViewSelectedSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle) = 0;
	//删除主类型委托  会调用onMessageListViewDataSourceMessage
	virtual void onMessageListViewRemoveMainItemMessage(MessageListView* pSender, string mainTitle) = 0;
	//修改主类型
	virtual void onMessageListViewReviseMainItemMessage(MessageListView* pSender, string mainTitle) = 0;
	//删除子类型委托  会调用onMessageListViewDataSourceMessage
	virtual void onMessageListViewRemoveSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle) = 0;
	//添加子类型委托
	virtual void onMessageListViewAddSubItemMessage(MessageListView* pSender, string mainTitle) = 0;
	//修改子类型委托
	virtual void onMessageListViewReviseSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle) = 0;
};

class MessageListView : public FKCW_UIWidget_ExpandableListView
{
public:
	CREATE_FUNC(MessageListView);
	MessageListView();
	virtual ~MessageListView();
public:
	//添加一个主节点
	void addMainItem(string mainTitle);
	//添加一个子项
	void addSubItem(int index, string subTitle);
	//在一个主节点下追加一个添加按钮
	void addSubItemAddButton(int index);

	//设置事件委托目标
	void setDelegate(MessageListViewDelegate* pDelegate);
	//更新数据源 会响应onMessageListViewDataSourceMessage事件
	void updateDataResouse();
	//清空
	void clear();
protected:
	virtual bool init();
	void onRemoveMainItemButtonClick(CCObject* pSender);
	void onAddSubItemButtonClick(CCObject* pSender);
	void onSelectedSubItemClick(CCObject* pSender);
	void onRemoveSubItemButtonClick(CCObject* pSender);
	//下一帧刷新数据
	void autoReloadData();
	virtual void update(float delta);
private:
	MessageListViewDelegate* m_pDelegate;
	FKCW_UIWidget_ColorView* m_pSelectColorView;	//当前选中的ColorView
	bool m_isReloadData;							//下一帧清空标识
};



#endif