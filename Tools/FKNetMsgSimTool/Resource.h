/**************************************************************************
***************************************************************************/
#pragma once

//-------------------------------------------------------------------------
enum ID_VIEW
{
	ID_FILE_VIEW = 3001 ,		//文件View ID
	ID_MESSAGE_LIST_VIEW,		//消息列表View
	ID_ATTRIBUTE_LIST_VIEW,		//属性列表View
	ID_ATTRIBUTE_EDIT_VIEW,		//属性编辑View

	ID_CONNECT_VIEW,			//连接View
	ID_LISTEN_VIEW,				//连接View

	ID_CLIENT_LOG_VIEW,			//客户端Log View
	ID_SERVER_LOG_VIEW,			//客户端Log View

	ID_SEND_MESSAGE_BUTTON,		//发送按钮
	ID_ADD_MAIN_MESSAGE_BUTTON, //添加主消息按钮
	ID_REVISE_SUB_MESSAGE_BUTTON, //修改子消息按钮
};

// add your res here.
static const char s_szSprite9Btn1Path[]				= "sprite9_btn1.png";
static const char s_szSprite9Btn2Path[]				= "sprite9_btn2.png";
static const char s_szCloseButton[]					= "close_but.png";

static const int s_iFontSizeBig = 28;
static const int s_iFontSizeNormal = 18;
static const int s_iFontSizeSmall = 14;
static const int s_iFontSizeSuperSmall = 12;