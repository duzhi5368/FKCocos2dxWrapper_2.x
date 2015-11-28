#ifndef _MAIN_SCREEN_H_
#define _MAIN_SCREEN_H_

#include "FKCWLib/FKCocos2dxWrapper.h"
#include "View/IncludeViewWidget.h"
#include "Xml/XmlParser.h"
#include "Xml/MsgPackUnpack.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

class MainScreen : public CCLayerColor,
	public FileViewDelegate,
	public MessageListViewDelegate,
	public AttributeListViewDelegate,
	public AttributeEditViewDelegate,
	public IpPostViewDelegate,
	public NewMainMessageDialogDelegate,
	public NewSubMessageDialogDelegate,
	public NewAttributeDialogDelegate,
	public ReviseSubMessageDialogDeleget,
	public ReviseMainMessageDialogDelegate,
	public NewFileDialogDelegate
{
public:
	CREATE_FUNC(MainScreen);
	static CCScene* createScene();
public:
	//初始化界面
	void initUI();
	
	//服务器开始监听
	void serverListen(const char* ip, short post);
	void serverClose();

	//客户端连接服务器
	void clientConnect(const char* ip, short post);
	void clientClose();
protected:
	MainScreen();
	virtual ~MainScreen();
	virtual bool init();
protected:
	CCNode* getViewByID(int id);
	/***************************************************************
							文件菜单
	***************************************************************/
	virtual void onFileViewNewFileMessage(string strFilePathName);
	virtual void onFileViewOpenFileMessage(string strFilePathName);

	/***************************************************************
							消息列表
	***************************************************************/
	//数据源
	virtual void onMessageListViewDataSourceMessage(MessageListView* pSender);
	//选中消息
	virtual void onMessageListViewSelectedSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle);
	//移除主列表
	virtual void onMessageListViewRemoveMainItemMessage(MessageListView* pSender, string mainTitle);
	//移除子项
	virtual void onMessageListViewRemoveSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle);
	//添加子项
	virtual void onMessageListViewAddSubItemMessage(MessageListView* pSender, string mainTitle);
	//修改主列表
	virtual void onMessageListViewReviseMainItemMessage(MessageListView* pSender, string mainTitle);
	//修改子项
	virtual void onMessageListViewReviseSubItemMessage(MessageListView* pSender, string mainTitle, string subTitle);
	
	/***************************************************************
							属性列表
	***************************************************************/
	//数据源
	virtual void onAttributeListViewDataSourceMessage(AttributeListView* pSender);
	//选中消息
	virtual void onAttributeListViewSelectedItemMessage(AttributeListView* pSender, string attributeName);
	//移除
	virtual void onAttributeListViewRemoveItemMessage(AttributeListView* pSender, string attributeName);
	//添加
	virtual void onAttributeListViewAddItemMessage( AttributeListView* pSender );

	/***************************************************************
							编辑属性模块
	***************************************************************/
	//保存
	virtual void onSaveMessage(AttributeEditView* pAttributeEditView, string attName, string typeName, string data);

	/***************************************************************
							服务器-客户端
	***************************************************************/
	//连接按钮点击
	virtual void onIpPostButtonClickMessage(IpPostView* pSender, string ip, short post);

	/***************************************************************
							客户端事件
	***************************************************************/
	void onClientMessageReceived	(NetworkClient* client, FKCW_SimpleNet_Buffer& oBuffer);
	void onClientConnected			();
	void onClientConnectTimeout		();
	void onClientExceptionCaught	();
	void onClientDisconnected		();
	void onClientConnectedEnded		();

	/***************************************************************
							服务器事件
	***************************************************************/
	void onServerMessageReceived	(NetworkServer* pServer, FKCW_SimpleNet_Buffer& oBuffer);
	void onServerConnected			();	
	void onServerDisconnected		();
	void onServerConnectedEnded		();
	void onServerExceptionCaught	();

	/***************************************************************
							基本事件
	***************************************************************/
	//发送消息按钮
	void onSendButtonClick(CCObject* pSender);
	//添加主列表按钮
	void onAddMainMessageButtonClick(CCObject* pSender);

	/***************************************************************
							对话框
	***************************************************************/
	//添加主列表 对话框 保存
	virtual void onNewMainMessageDialogSaveMessage( NewMainMessageDialog* pSender, short mainType, string mainName );
	//修改主列表 对话框 保存
	virtual void onReviseMainMessageDialogSaveMessage(ReviseMainMessageDialog* pSender, short oldMainType, short mainType, string mainName);

	//添加子项 对话框 保存
	virtual void onNewSubMessageDialogSaveMessage( NewSubMessageDialog* pSender, short mainType, short subType, string subName, short fbm, short fbs );
	//修改子项 对话框 保存
	virtual void onReviseSubMessageDialogSaveMessage(ReviseSubMessageDialog* pSender, short mainType, short subType);

	//添加属性列表 对话框 保存
	virtual void onNewAttributeDialogSaveMessage( NewAttributeDialog* pSender, string attName, string typeName, string data );
	
	//新建文件 对话框 保存
	virtual void onNewFileDialogDelegateSaveMessage(NewFileDialog* pSender, string pathAndFileName);

private:
	NetworkServer* m_pNetworkServer;
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	short m_mainType;
	short m_subType;
	string m_attName;
	bool m_isLoadConfigFile;		//是否已加载配置文件
};

#endif