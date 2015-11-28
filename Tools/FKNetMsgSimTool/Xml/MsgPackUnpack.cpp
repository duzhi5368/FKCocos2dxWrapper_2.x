#include "MsgPackUnpack.h"

void MsgPackUnpack::pack( FKCW_SimpleNet_Buffer &buffer, MsgPack msgPace )
{
	vector<MsgAttObject> msgObjects = msgPace.atts;

	buffer.writeShort(msgPace.mainType);
	buffer.writeShort(msgPace.subType);

	for (unsigned int i = 0; i < msgObjects.size(); i++)
	{
		MsgAttObject maObj = msgObjects.at(i);
		string strData = maObj.data;

		if(maObj.type == EMDT_FLOAT)
		{
			float fb = XmlParser::StringToFloat(strData);
			buffer.writeFloat(fb);
		}
		else if (maObj.type == EMDT_SHORT)
		{
			short sb = XmlParser::StringToShort(strData);
			buffer.writeShort(sb);
		}
		else if (maObj.type == EMDT_INT)
		{
			int ib = XmlParser::StringToInt(strData);
			buffer.writeInt(ib);
		}
		else if (maObj.type == EMDT_BOOL)
		{
			if(strData.compare("false") == 0)
				buffer.writeChar('0');
			else
				buffer.writeChar('1');
		}
		else if (maObj.type == EMDT_STRING)
		{
			//字符串保存规则：int长度+字符串
			const char* cb = strData.c_str();
			buffer.writeInt(strlen(cb));
			buffer.writeString(cb);
		}
	}

	//反馈类型
	buffer.writeShort(msgPace.fbm);
	buffer.writeShort(msgPace.fbs);
}

void MsgPackUnpack::unPack(MsgPack &msgPack, XmlParser* xmlParser, FKCW_SimpleNet_Buffer* buffer )
{
	vector<MsgAttObject> maObjs;

	short mainType = (*buffer).readShort();
	short subType = (*buffer).readShort();

	msgPack.mainType = mainType;
	msgPack.subType = subType;

	maObjs = xmlParser->getAttributeObjects(mainType, subType);

	for (unsigned int i = 0; i < maObjs.size(); i++)
	{
		MsgAttObject maObjBuf = maObjs.at(i);

		if(maObjBuf.type == EMDT_FLOAT)
		{
			float fb = (*buffer).readFloat();

			ostringstream oss;
			oss<<fb;
			maObjs.at(i).data = oss.str();
		}
		else if (maObjBuf.type == EMDT_SHORT)
		{
			short sb = (*buffer).readShort();

			ostringstream oss;
			oss<<sb;
			maObjs.at(i).data = oss.str();
		}
		else if (maObjBuf.type == EMDT_INT)
		{
			int ib = (*buffer).readInt();

			ostringstream oss;
			oss<<ib;
			maObjs.at(i).data = oss.str();
		}
		else if (maObjBuf.type == EMDT_BOOL)
		{
			char cB = (*buffer).readChar();
			char strB[10];
			sprintf_s(strB, "%c", cB);
			
			string bb = strB;
			if (bb.compare("0") == 0)
				bb = "false";
			else
				bb = "true";

			ostringstream oss;
			oss<<bb;
			maObjs.at(i).data = oss.str();
		}
		else if (maObjBuf.type == EMDT_STRING)
		{
			int length = (*buffer).readInt();
			string sb = (*buffer).readString(length);

			maObjs.at(i).data = sb;
		}
	}

	msgPack.atts = maObjs;
	//反馈类型
	msgPack.fbm = (*buffer).readShort();
	msgPack.fbs = (*buffer).readShort();
}

void MsgPackUnpack::unPackStringStream( MsgPack& magPack, XmlParser* xmlParser, char* stream, int lenght )
{
	FKCW_SimpleNet_Buffer* buffer = new FKCW_SimpleNet_Buffer(stream, lenght);

	unPack(magPack, xmlParser, buffer);

	delete buffer;
}