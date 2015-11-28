//--------------------------------------------------------------------
#include "../Include/FKCW_Base_PlatformStringParser.h"
//--------------------------------------------------------------------
FKCW_Base_PlatformStringParser::FKCW_Base_PlatformStringParser()
{

}
//--------------------------------------------------------------------
FKCW_Base_PlatformStringParser::~FKCW_Base_PlatformStringParser()
{

}
//--------------------------------------------------------------------
FKCW_Base_PlatformStringParser* FKCW_Base_PlatformStringParser::Create()
{
	FKCW_Base_PlatformStringParser* p = new FKCW_Base_PlatformStringParser();
	return ( FKCW_Base_PlatformStringParser* )p->autorelease();
}
//--------------------------------------------------------------------
// 解析平台的 xml 并保存在字符串字典中
// 参数：p_strPath xml文件的路径。该路径是平台无关的，它将被映射到对应的平台格式
// 例如：Android上的"/sdcard/strings.xml" 将被映射到iOS系统上的"~/Documents/sdcard/strings.xml" 中
void FKCW_Base_PlatformStringParser::Parse( const string& p_strPath, const CCDictionary& p_Dict )
{
	// 获取xml数据。因为tinyxml不能从apk中读取文件
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(p_strPath.c_str(), "rb", &size);

	// 加载文件并读取
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	if(doc->Parse((const char*)data, size) == XML_NO_ERROR) {
		m_pDict = (CCDictionary*)&p_Dict;
		doc->Accept(this);
	}

	free(data);
	delete doc;
}
//--------------------------------------------------------------------
// 重载XMLVisitor函数
bool FKCW_Base_PlatformStringParser::VisitEnter(const XMLElement& element, const XMLAttribute* firstAttribute)
{
	if(!strcmp(element.Name(), "string")) {
		CCString* s = CCString::create(element.GetText() ? element.GetText() : "");
		const char* key = element.Attribute("name");
		if(key) 
		{
			m_pDict->setObject(s, key);
		}
	}
	return true;
}
//--------------------------------------------------------------------