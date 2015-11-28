#include "XmlParser.h"

//去掉标点符号
void removePpunctuation(string &inputString)
{
	char filter[] = ("\",.;:?!)(\\/");
	for (unsigned int i=0; i<strlen(filter); i++)
	{
		FKCW_Base_Utils::ReplaceChar(inputString, filter[i], '-');
	}
}

//输出调试信息
#define MLOG(text) MessageBox(NULL, text, L"提示", 0)

XmlParser* XmlParser::m_pInstance = NULL;

XmlParser::XmlParser()
{
	m_pDoc = new tinyxml2::XMLDocument();
}

XmlParser::~XmlParser()
{
	if (m_pDoc != NULL)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}
}

bool XmlParser::loadFile(const char* pFilePath)
{
	m_pFilePath = pFilePath;

	//载入文件
	tinyxml2::XMLError loadError = m_pDoc->LoadFile(m_pFilePath.c_str());

	//验证文件 如果有其他错误那么放弃操作
	if(loadError != tinyxml2::XMLError::XML_SUCCESS)
	{
		vector<string> mainElements;
		return false;
	}

	//如果打开的不是FK消息配置表  返回false
	if (m_pDoc->RootElement() == NULL)
	{
		return false;
	}
	if (string(m_pDoc->RootElement()->Name()).compare("fkcw") != 0)
	{
		return false;
	}

	return true;
}

void XmlParser::newFile(const char* pFilePath)
{
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();

	//xml声明
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDel);

	//节点fkcw
	tinyxml2::XMLElement *plistElement = pDoc->NewElement("fkcw");
	pDoc->LinkEndChild(plistElement);

	pDoc->SaveFile(pFilePath);
	pDoc->Print();

	delete pDoc;
	pDoc = NULL;
}

//【主类型】描述
vector<string> XmlParser::getMainElementCStrings()
{
	vector<string> mainElements;

	//根节点
	tinyxml2::XMLElement* pRootEle = m_pDoc->RootElement();
	//第一个主节点
	tinyxml2::XMLElement* pMainEleBuf = pRootEle->FirstChildElement();
	while (pMainEleBuf != NULL)
	{
		string name = pMainEleBuf->Attribute("name");		//描述
		string mainType = pMainEleBuf->Name();			//主类型

		string text = "【";

		text.append(mainType);
		text.append("】 ");
		text.append(name);

		mainElements.push_back(text);
		pMainEleBuf = pMainEleBuf->NextSiblingElement();
	}

	return mainElements;
}
//"【子类型】 描述 [反馈主类型](反馈子类型)"
vector<string> XmlParser::getSubElementCstrings(short mainType)
{
	vector<string> subElementsCStrings;

	tinyxml2::XMLElement* subElementBuf = _getMainElement(mainType);
	if (subElementBuf != NULL)
	{
		subElementBuf = subElementBuf->FirstChildElement();
	}

	while(subElementBuf != NULL)
	{
		string name = subElementBuf->Attribute("name");	//描述
		string csType = subElementBuf->Attribute("type");	//客户端/服务器类型
		string subType = subElementBuf->Name();			//子类型
		
		string fbm = subElementBuf->Attribute("fbm");
		string fbs = subElementBuf->Attribute("fbs");

		string text;	//连接三个字符串后的值 csType: name #mainTyp

		text.append("【");
		text.append(subType);
		text.append("】 ");
		text.append(name);
		text.append(" [");
		text.append(fbm);
		text.append("](");
		text.append(fbs);
		text.append(")");

		subElementsCStrings.push_back(text);
		subElementBuf = subElementBuf->NextSiblingElement();
	}

	return subElementsCStrings;
}

//【属性名】 (数据类型) 默认值
vector<string> XmlParser::getAttributeCstrings(short mainType, short subType)
{
	vector<string> attElementsCStrings;

	tinyxml2::XMLElement* attElementBuf = _getSubElement(mainType, subType);
	if (attElementBuf != NULL)
	{
		attElementBuf = attElementBuf->FirstChildElement();
	}

	while(attElementBuf != NULL)
	{
		string dataType = attElementBuf->Attribute("type");	//数据类型
		string att = attElementBuf->Name();				//属性名
		string sValue = attElementBuf->GetText();		//值
		string text;	
		
		text.append("【");
		text.append(att);
		text.append("】 (");
		text.append(dataType);
		text.append(") ");
		text.append(sValue);

		attElementsCStrings.push_back(text);
		attElementBuf = attElementBuf->NextSiblingElement();
	}

	return attElementsCStrings;
}

std::string XmlParser::getSubElementName( short mainType, short subType )
{
	tinyxml2::XMLElement* ele = _getSubElement(mainType, subType);
	string name = ele->Attribute("name");

	return name;
}

short XmlParser::getSubElementFBM( short mainType, short subType )
{
	tinyxml2::XMLElement* ele = _getSubElement(mainType, subType);
	string name = ele->Attribute("fbm");
	return atoi(name.c_str());
}

short XmlParser::getSubElementFBS( short mainType, short subType )
{
	tinyxml2::XMLElement* ele = _getSubElement(mainType, subType);
	string name = ele->Attribute("fbs");
	return atoi(name.c_str());
}

std::vector<MsgAttObject> XmlParser::getAttributeObjects( short mainType, short subType )
{
	vector<MsgAttObject> msObjects;
	
	tinyxml2::XMLElement* attElementBuf = _getSubElement(mainType, subType);
	if (attElementBuf != NULL)
	{
		attElementBuf = attElementBuf->FirstChildElement();
	}
	
	while(attElementBuf != NULL)
	{
		string dataType = attElementBuf->Attribute("type");	//数据类型
		string att = attElementBuf->Name();				//属性名
		string sValue = attElementBuf->GetText();		//值
		
		MsgAttObject msObjectBuf;
		msObjectBuf.name = att;
		/*
		EMDT_SHORT,
		EMDT_INT,
		EMDT_FLOAT,
		EMDT_LONG,
		EMDT_CHARA,
		EMDT_UNKNOWN
		*/
		if (dataType.compare("short") == 0)
		{
			msObjectBuf.type = EMDT_SHORT;
		}
		else if (dataType.compare("int") == 0)
		{
			msObjectBuf.type = EMDT_INT;
		}
		else if (dataType.compare("float") == 0)
		{
			msObjectBuf.type = EMDT_FLOAT;
		}
		else if (dataType.compare("bool") == 0)
		{
			msObjectBuf.type = EMDT_BOOL;
		}
		else if (dataType.compare("string") == 0)
		{
			msObjectBuf.type = EMDT_STRING;
		}

		msObjectBuf.data = sValue;

		msObjects.push_back(msObjectBuf);
		attElementBuf = attElementBuf->NextSiblingElement();
	}

	return msObjects;

}

std::string XmlParser::getAttributeCstring( short mainType, short subType, std::string attName )
{
	//【属性名】 (数据类型) 默认值
	string attStr;
	tinyxml2::XMLElement* attEle = _getAttributeElement(mainType, subType, attName);

	attStr.append("【");
	attStr.append(attEle->Name());

	attStr.append("】 (");
	attStr.append(attEle->Attribute("type"));
	attStr.append(") ");
	attStr.append(attEle->GetText());

	return attStr;
}

MsgAttObject XmlParser::getAttributeObject( short mainType, short subType, std::string attName )
{
	MsgAttObject obj;
	tinyxml2::XMLElement* attEle = _getAttributeElement(mainType, subType, attName);
	obj.name = attEle->Name();
	obj.data = attEle->GetText();
	obj.type = getDataType(mainType, subType, attName);

	return obj;
}

bool XmlParser::addMainElement(short mainType, string name)
{
	char mainTypeBuf[128]; 
	_itoa_s(mainType, mainTypeBuf, 10); 
	
	removePpunctuation(name);
	const char* nameBuf = name.c_str();	//转换成char*

	tinyxml2::XMLElement* newElement = m_pDoc->NewElement(mainTypeBuf);
	newElement->SetAttribute("name", nameBuf);
	m_pDoc->RootElement()->LinkEndChild(newElement);
	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::addSubElement(short mainType, string name, short subType, short fbm, short fbs)
{
	char mainTypeBuf[128]; 
	_itoa_s(subType, mainTypeBuf, 10); 

	removePpunctuation(name);
	const char* nameBuf = name.c_str();	//转换成char*

	tinyxml2::XMLElement* mainElement = _getMainElement(mainType);
	if (mainElement == NULL)	//如果主节点不存在那么返回false
	{
		cocos2d::CCLog("mainElement is null");
		return false;
	}

	tinyxml2::XMLElement* newElement = m_pDoc->NewElement(mainTypeBuf);
	newElement->SetAttribute("name", nameBuf);
	newElement->SetAttribute("fbm", fbm);
	newElement->SetAttribute("fbs", fbs);
	newElement->SetAttribute("type", "");	//type 保留

	mainElement->LinkEndChild(newElement);
	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();
	return true;
}

bool XmlParser::addAttributeElement(short mainType, short subType, string attName, string dataType, string sValue)
{
	removePpunctuation(attName);
	removePpunctuation(dataType);

	tinyxml2::XMLElement* subElement = _getSubElement(mainType, subType);
	if (subElement == NULL)	//如果主节点不存在那么返回false
	{
		return false;
	}
	
	sValue = _typeConversion(dataType, sValue);

	tinyxml2::XMLElement* newElement = m_pDoc->NewElement(attName.c_str());
	newElement->SetAttribute("type", dataType.c_str());
	newElement->LinkEndChild(m_pDoc->NewText(sValue.c_str()));

	subElement->LinkEndChild(newElement);
	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();
	return true;
}

bool XmlParser::setMainElementType(short mainType, short newMainType)
{
	tinyxml2::XMLElement* pEle = _getMainElement(mainType);
	
	if (pEle == NULL)
	{
		return false;
	}

	char string[25]; 
	_itoa_s(newMainType, string, 10); 
	
	pEle->SetName(string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setSubElementType(short mainType, short subType, short newMainType)
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	char string[25]; 
	_itoa_s(newMainType, string, 10); 

	pEle->SetName(string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setSubElementCS(short mainType, short subType, string newCS)
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	const char* string = newCS.c_str();	//转换成char*

	pEle->SetAttribute("type", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setAttributeElementName(short mainType, short subType, string attName, string newAttName)
{
	tinyxml2::XMLElement* pEle = _getAttributeElement(mainType, subType, attName);

	if (pEle == NULL)
	{
		return false;
	}

	removePpunctuation(newAttName);
	const char* string = newAttName.c_str();	//转换成char* todo

	pEle->SetName(string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setMainElementName(short mainType, string newName)
{
	tinyxml2::XMLElement* pEle = _getMainElement(mainType);

	if (pEle == NULL)
	{
		return false;
	}

	removePpunctuation(newName);
	const char* string = newName.c_str();	//转换成char*

	pEle->SetAttribute("name", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setSubElementName(short mainType, short subType, string newName)
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	removePpunctuation(newName);
	const char* string = newName.c_str();	//转换成char*

	pEle->SetAttribute("name", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setSubElementFBM( short mainType, short subType, short fbm )
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	char string[25]; 
	_itoa_s(fbm, string, 10); 

	pEle->SetAttribute("fbm", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setSubElementFBS( short mainType, short subType, short fbs )
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	char string[25]; 
	_itoa_s(fbs, string, 10); 

	pEle->SetAttribute("fbs", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}

bool XmlParser::setAttributeElementDataType(short mainType, short subType, string attName, string newType)
{
	tinyxml2::XMLElement* pEle = _getAttributeElement(mainType, subType, attName);

	if (pEle == NULL)
	{
		return false;
	}

	removePpunctuation(newType);
	const char* string = newType.c_str();	//转换成char*

	pEle->SetAttribute("type", string);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}


bool XmlParser::setAttributeElementValue( short mainType, short subType, std::string attName, std::string newValue )
{
	tinyxml2::XMLElement* pEle = _getAttributeElement(mainType, subType, attName);

	if (pEle == NULL)
	{
		return false;
	}

	string attString = getAttributeCstring(mainType, subType, attName);
	string dataType = GetAttType(attString);
	newValue = _typeConversion(dataType, newValue);

	const char* string = newValue.c_str();	//转换成char*

	pEle->DeleteChildren();
	pEle->LinkEndChild(m_pDoc->NewText(string));

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();

	return true;
}


bool XmlParser::removeMainElement(short mainType)
{
	tinyxml2::XMLElement* pEle = _getMainElement(mainType);

	if (pEle == NULL)
	{
		return false;
	}

	m_pDoc->RootElement()->DeleteChild(pEle);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();
	return true;
}

bool XmlParser::removeSubElement(short mainType, short subType)
{
	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);

	if (pEle == NULL)
	{
		return false;
	}

	_getMainElement(mainType)->DeleteChild(pEle);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();
	return true;
}

bool XmlParser::removeAttributeElement(short mainType, short subType, string attName)
{
	tinyxml2::XMLElement* pEle = _getAttributeElement(mainType, subType, attName);

	if (pEle == NULL)
	{
		return false;
	}

	_getSubElement(mainType, subType)->DeleteChild(pEle);

	m_pDoc->SaveFile(m_pFilePath.c_str());
	m_pDoc->Print();
	return true;
}

MsgPack XmlParser::getMsgPack( short mainType, short subType )
{
	MsgPack pack;
	pack.mainType = mainType;
	pack.subType = subType;
	pack.atts = getAttributeObjects(mainType, subType);

	tinyxml2::XMLElement* pEle = _getSubElement(mainType, subType);
	//反馈消息类型
	pack.fbm = atoi(pEle->Attribute("fbm"));
	pack.fbs = atoi(pEle->Attribute("fbs"));

	return pack;
}


//--------------------------------------------------------------------------------------------
//									private
//--------------------------------------------------------------------------------------------

tinyxml2::XMLElement* XmlParser::_getMainElement(short mainType)
{
	//根节点
	tinyxml2::XMLElement* pRootEle = m_pDoc->RootElement();
	if( pRootEle == nullptr )
		return NULL;

	//将short类型转换成char*类型
	char string[25]; 
	_itoa_s(mainType, string, 10); 

	return pRootEle->FirstChildElement(string);
}

tinyxml2::XMLElement* XmlParser::_getSubElement(short mainType, short subType)
{
	tinyxml2::XMLElement* mainEle = _getMainElement(mainType);
	
	if(mainEle == NULL)
		return NULL;

	//将short类型转换成char*类型
	char string[25]; 
	_itoa_s(subType, string, 10); 

	return mainEle->FirstChildElement(string);
}

tinyxml2::XMLElement* XmlParser::_getAttributeElement(short mainType, short subType, string attName)
{
	tinyxml2::XMLElement* subEle = _getSubElement(mainType, subType);

	if(subEle == NULL)
		return NULL;

	const char* attBuf = attName.c_str();	//转换成char*

	return subEle->FirstChildElement(attBuf);
}

//------------------------------------------------------------------------------------------------
//【属性名】 (数据类型) 默认值
ENUM_MSG_DATA_TYPE XmlParser::GetDataType(string text)
{
	int startPos = text.find("(");
	int endPos = text.find(")");
	int startToEndLen = endPos - startPos;
	//截取
	string attType = text.substr(startPos + 1, startToEndLen - 1);

	//解析出被"()"包含起来的字符
	if(attType.compare("short") == 0)
		return EMDT_SHORT;
	else if(attType.compare("int") == 0)
		return EMDT_INT;
	else if(attType.compare("float") == 0)
		return EMDT_FLOAT;
	else if(attType.compare("bool") == 0)
		return EMDT_BOOL;
	else if(attType.compare("string") == 0)
		return EMDT_STRING;
	return EMDT_STRING;
}

ENUM_MSG_DATA_TYPE XmlParser::getDataType(short mainType, short subType, std::string attName)
{
	tinyxml2::XMLElement* attEle = _getAttributeElement(mainType, subType, attName);

	string attType = attEle->Attribute("type");

	//解析出被"()"包含起来的字符
	if(attType.compare("short") == 0)
		return EMDT_SHORT;
	else if(attType.compare("int") == 0)
		return EMDT_INT;
	else if(attType.compare("float") == 0)
		return EMDT_FLOAT;
	else if(attType.compare("bool") == 0)
		return EMDT_BOOL;
	else if(attType.compare("string") == 0)
		return EMDT_STRING;
	return EMDT_STRING;
}

std::string XmlParser::GetAttName( std::string text )
{
	//格式如: 【属性名】 (数据类型) 默认值
	int startPos = text.find("【");
	int endPos = text.find("】")-startPos;
	string attName = text.substr(startPos+2, endPos-2);

	return attName;
}

std::string XmlParser::GetAttData(std::string text)
{
	//格式如: 【属性名】 (数据类型) 默认值
	int startPos = text.find(")");
	string attData = text.substr(startPos+2);
	return attData;
}

std::string XmlParser::GetAttType( std::string text )
{
	//格式如: 【属性名】 (数据类型) 默认值
	int startPos = text.find("(");
	int endPos = text.find(")")-startPos-1;
	string attData = text.substr(startPos+1, endPos);
	return attData;
}

std::string XmlParser::GetSubString(std::string text)
{
	//【子类型】 描述 [反馈主类型](反馈子类型)
	int endPos = text.find("[");
	return text.substr(0, endPos-1);
}

short XmlParser::GetMainType(string text)
{
	//【主类型】描述
	int startPos = text.find("【");
	int endPos = text.find("】") - startPos;
	string mainTypeCS = text.substr(startPos+2, endPos-2);
	short sType = atoi(mainTypeCS.c_str());

	return sType;
}

string XmlParser::GetMainName(string text)
{
	//【主类型】描述
	int endPos = text.find(" ");
	string mainTypeCS = text.substr(endPos+1);

	return mainTypeCS;
}

short XmlParser::GetSubType(string text)
{
	//【子类型】 描述 [反馈主类型](反馈子类型)
	int startPos = text.find("【");
	int startEnd = text.find("】") - startPos;
	string mainTypeCS = text.substr(startPos+2, startEnd-2);
	short sType = atoi(mainTypeCS.c_str());

	return sType;
}

std::string XmlParser::GetNoFBTypeToSubName(std::string text)
{
	int endPos = text.find(" ");
	return text.substr(endPos+1);
}

short XmlParser::GetSubFBM( std::string text )
{
	int startPos = text.find("[");
	int startEnd = text.find("]") - startPos;
	string mainTypeCS = text.substr(startPos+1, startEnd);
	short sType = atoi(mainTypeCS.c_str());

	return sType;
}

short XmlParser::GetSubFBS( std::string text )
{
	int startPos = text.find("(");
	int startEnd = text.find(")") - startPos;
	string mainTypeCS = text.substr(startPos+1, startEnd);
	short sType = atoi(mainTypeCS.c_str());

	return sType;
}

bool XmlParser::isHaveMainElement( short mainType )
{
	if(_getMainElement(mainType) == NULL)
	{
		return false;
	}
	return true;
}

bool XmlParser::isHaveSubElement( short mainType, short subType )
{
	if(_getSubElement(mainType, subType) == NULL)
	{
		return false;
	}
	return true;
}

string XmlParser::_typeConversion(string type, string value)
{
	//强转
	if (type.compare("short") == 0)
	{
		short buf = atoi(value.c_str());
		char cBuf[128];
		value = _itoa(buf, cBuf, 10);
	}
	else if(type.compare("int") == 0)
	{
		int buf = atoi(value.c_str());
		char cBuf[128];
		value = _itoa(buf, cBuf, 10);
	}
	else if(type.compare("float") == 0)
	{
		float buf = atof(value.c_str());
		char cBuf[128];
		sprintf(cBuf, "%f", buf);
		value = cBuf;
	}
	else if(type.compare("bool") == 0)
	{
		string buf = StringToBool(value);
		value = buf;
	}
	return value;
}
