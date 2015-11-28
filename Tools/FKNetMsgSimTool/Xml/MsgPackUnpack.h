#ifndef _MSG_PARSER_H_
#define _MSG_PARSER_H_

//-------------------------------------------------------------------------
#include "XmlParser.h"
#include "FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------

class MsgPackUnpack : public cocos2d::CCObject
{
public:
	//打包成网络传输对象
	//buffer： 打包后的Buffer对象
	//msgPace: 被打包的MsgPack包
	static void pack(FKCW_SimpleNet_Buffer &buffer, MsgPack msgPace);
	
	//解包成MsgPack结构体
	//magPack: 解包后储存MsgPack的引用
	//xmlParser: XML解析对象指针
	//buffer： 被解包的Buffer源数据
	static void unPack(MsgPack& magPack, XmlParser* xmlParser, FKCW_SimpleNet_Buffer* buffer);
	
	//字符流解包成MsgPack结构体
	//magPack: 解包后储存MsgPack的引用
	//xmlParser: XML解析对象指针
	//stream: 字符串流
	//lenght: 字符串流长度
	static void unPackStringStream(MsgPack& magPack, XmlParser* xmlParser, char* stream, int lenght);
};

#endif	//_MSG_PARSER_H_