//--------------------------------------------------------------------
#include "../Include/FKCW_Base_Localization.h"
#include "../Include/FKCW_Base_PlatformStringParser.h"
//--------------------------------------------------------------------
FKCW_Base_Localization* FKCW_Base_Localization::s_Instance = NULL;
//--------------------------------------------------------------------
// 用 ascii 码替换 \n \t \r
string FKCW_Base_Localization::__UnEscape( const string& s )
{
	string unescaped;
	char c;
	const char* buf = s.c_str();
	int len = s.length();
	for(int i = 0; i < len; i++) {
		if(buf[i] == '\\') {
			switch (buf[i + 1]) {
			case 'n':
				c = '\n';
				unescaped.append(&c, 1);
				i++;
				break;
			case 'r':
				c = '\r';
				unescaped.append(&c, 1);
				i++;
				break;
			case 't':
				c = '\t';
				unescaped.append(&c, 1);
				i++;
				break;
			default:
				unescaped.append(&buf[i], 1);
				break;
			}
		} else {
			unescaped.append(&buf[i], 1);
		}
	}
	return unescaped;
}
//--------------------------------------------------------------------
FKCW_Base_Localization::FKCW_Base_Localization()
{

}
//--------------------------------------------------------------------
FKCW_Base_Localization::~FKCW_Base_Localization()
{
	if(s_Instance) 
	{
		s_Instance->release();
		s_Instance = NULL;
	}
}
//--------------------------------------------------------------------
FKCW_Base_Localization* FKCW_Base_Localization::GetInstance()
{
	if(!s_Instance) 
	{
		s_Instance = new FKCW_Base_Localization();
	}
	return s_Instance;
}
//--------------------------------------------------------------------
// 注册添加一些字符串
// 注：文件必须是平台xml格式文件
// 参数：p_strLanguage ISO639-1双字母代码 
// 参数：p_strPath xml文件的路径。该路径是平台无关的，它将被映射到对应的平台格式
// 例如：Android上的"/sdcard/strings.xml" 将被映射到iOS系统上的"~/Documents/sdcard/strings.xml" 中
// 参数：p_bMerge。True表示如果当前字符串已经存在，新字符串将会合并进去。
//		false表示若当前字符串已经存在，则新字符串会直接替代原字符串。
void FKCW_Base_Localization::AddPlatformString( const string& p_strLanguage, const string& p_strPath, bool p_bMerge )
{
	if( p_strPath.empty() ) 
	{
		CCLOGWARN("CCLocalization::addAndroidStrings: string file path is empty");
		return;
	}
	if( p_strLanguage.length() != 2 ) 
	{
		CCLOGWARN("CCLocalization::addAndroidStrings: language code is not in ISO 639-1 format");
		return;
	}

	// 注册语言词典
	CCDictionary* d = (CCDictionary*)m_LanguageDicMap.objectForKey(p_strLanguage);
	if(!d) 
	{
		d = CCDictionary::create();
		m_LanguageDicMap.setObject(d, p_strLanguage);
	}

	// 若不组合，则先删除节点
	if(!p_bMerge) 
	{
		d->removeAllObjects();
	}

	// parse it
	FKCW_Base_PlatformStringParser::Create()->Parse(p_strPath, *d);
}
//--------------------------------------------------------------------
// 通过指定的语言种类，查找key对应的value，如果该语言找不到，则会恢复为默认的 English 语言进行查找。
string FKCW_Base_Localization::GetString( const string& p_strKey, const string& p_strLanguage )
{
	// 找指定的语言，若找不到，则找英语
	bool fallback = false;
	string lan = p_strLanguage;
	CCDictionary* strings = (CCDictionary*)m_LanguageDicMap.objectForKey(lan);
	if(!strings) 
	{
		fallback = true;
		strings = (CCDictionary*)m_LanguageDicMap.objectForKey("en");
	}

	if(strings) 
	{
		CCString* s = (CCString*)strings->objectForKey(p_strKey);
		if(s) 
		{
			string ret = s->getCString();
			ret = __UnEscape(ret);
			return ret;
		} 
		else if(!fallback)
		{
			strings = (CCDictionary*)m_LanguageDicMap.objectForKey("en");
			CCString* s = (CCString*)strings->objectForKey(p_strKey);
			if(s) 
			{
				string ret = s->getCString();
				ret = __UnEscape(ret);
				return ret;
			}
		}
	}

	return "!" + p_strKey + "!";
}
//--------------------------------------------------------------------