//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_Utils.h"
#include "../Include/FKCW_Base_DecodeHtmlEntitiesUTF8.h"
#include "../Include/FKCW_Base_PinYinUtils.h"
#include "../Include/FKCW_Base_FileZip.h"
#include "cocos-ext.h"
#include <string>

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <WinSock2.h>
#pragma comment( lib, "ws2_32.lib" )
#else
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#endif
//-------------------------------------------------------------------------
#pragma warning( disable : 4244 )
//-------------------------------------------------------------------------
CCArray	FKCW_Base_Utils::s_TmpArray;
//-------------------------------------------------------------------------
#ifndef MAX_LOG_LEN
#define MAX_LOG_LEN         (cocos2d::kMaxLogLen + 1)
#endif
// 简便的consoleLog
void FKCW_Base_Utils::FKConsoleLog(const char * pszFormat, ...)
{
	char szBuf[MAX_LOG_LEN - 20] = {0};
	char szShowBuf[MAX_LOG_LEN] = { 0 };

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	struct tm *tm;  
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int minute=tm->tm_min;  
	int second=tm->tm_sec;
	char szTime[10] = { 0 };
	sprintf( szTime, "%02d:%02d:%02d",  hour, minute, second );
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LOG_LEN - 20, MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);

	sprintf_s( szShowBuf, "%s - %s", szTime, szBuf );

	OutputDebugStringA(szShowBuf);
	OutputDebugStringA("\n");

	puts(szShowBuf);
	FKFileLog( szShowBuf );
#else
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf(szBuf,  MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);
	CCLOG(szBuf);
#endif
	
}
//-------------------------------------------------------------------------
void FKCW_Base_Utils::FKConsoleLogColor( short p_sColor, const char * pszFormat, ... )
{

	char szBuf[MAX_LOG_LEN - 20] = {0};
	char szShowBuf[MAX_LOG_LEN] = { 0 };


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	struct tm *tm;  
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int minute=tm->tm_min;  
	int second=tm->tm_sec;
	char szTime[10] = { 0 };
	sprintf_s( szTime, "%02d:%02d:%02d",  hour, minute, second );

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LOG_LEN - 20, MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);

	sprintf_s( szShowBuf, "%s - %s", szTime, szBuf );

	OutputDebugStringA(szShowBuf);
	OutputDebugStringA("\n");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, p_sColor);
	}

	puts(szShowBuf);
	FKFileLog( szShowBuf );

	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#else
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf(szBuf,  MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);
	CCLOG(szBuf);
#endif
	
}
//-------------------------------------------------------------------------
// 写入Log
void FKCW_Base_Utils::FKFileLog( const char* p_szMsg )
{
	std::string strPath = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();

	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	char szData[9];
	memset( szData, 0, 9 );
	sprintf( szData, "%02d月%02d日", t->tm_mon+1,t->tm_mday );
	std::string strFile = strPath +  std::string(szData) + ".txt";
	FILE *fp = fopen(strFile.c_str(), "a+");
	fwrite(p_szMsg,1, strlen(p_szMsg),fp);
	fwrite( "\n", 1, 1, fp);
	fclose(fp);
	
}
//-------------------------------------------------------------------------
// int转换为string
string FKCW_Base_Utils::ItoA(int num)
{
	std::stringstream str;
	str << num;
	std::string res(str.str());
	return res;
}
//-------------------------------------------------------------------------
// string转换为int
int FKCW_Base_Utils::AtoI(const char* str)
{
	return str == NULL ? 0 : ::atoi(str);
}
//-------------------------------------------------------------------------
int FKCW_Base_Utils::AtoI(string str)
{
	return AtoI(str.c_str());
}
//-------------------------------------------------------------------------
// float转换为string
string FKCW_Base_Utils::FtoA(float p_fNum,int p_nPrecision)
{
	if(p_fNum==floorf(p_fNum))
	{
		return ItoA(p_fNum);
	}
	if(p_fNum==0)
		return string("0");

	int sig;
	if(p_fNum==0)
	{
		sig=0;
	}
	else if(p_fNum>0)
	{
		sig=1;
	}
	else
	{
		sig=-1;
	}
	float abs = fabsf(p_fNum);
	int A = floor(abs);
	int B = floor((abs-A)*pow(10,p_nPrecision)) ;
	string Astr = ItoA(A);
	string Bstr = ItoA(B);
	string zerosStr;
	int makeUpZeroCount = p_nPrecision-(int)Bstr.size();
	assert(makeUpZeroCount>=0);
	// 补0
	for(int i=0;i<makeUpZeroCount;i++)
	{
		zerosStr+="0";
	}

	// 补符号
	string valueStr;
	if(sig<0)
	{
		valueStr+="-";
	}

	valueStr+=Astr+"."+zerosStr+Bstr;
	return valueStr;
}
//-------------------------------------------------------------------------
// string转换为float
double FKCW_Base_Utils::AtoF(const char* sptr)
{
	double temp=10;
	bool ispnum=true;
	double ans=0;
	if(*sptr=='-')//判断是否是负数
	{
		ispnum=false;
		sptr++;
	}
	else if(*sptr=='+')//判断是否为正数
	{
		sptr++;
	}

	while(*sptr!='\0')//寻找小数点之前的数
	{
		if(*sptr=='.'){ sptr++;break;}
		ans=ans*10+(*sptr-'0');
		sptr++;
	}
	while(*sptr!='\0')//寻找小数点之后的数
	{
		ans=ans+(*sptr-'0')/temp;
		temp*=10;
		sptr++;
	}
	if(ispnum) return ans;
	else return ans*(-1);
}
//-------------------------------------------------------------------------
double FKCW_Base_Utils::AtoF(string str)
{
	return AtoI(str.c_str());
}
//-------------------------------------------------------------------------
static int CharHexToInt(char ch)  
{  
        // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2  
        if(isdigit(ch))  
                return ch - 48;  
  
        // 如果是字母，但不是A~F,a~f则返回  
        if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )  
                return -1;  
  
        // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10  
        // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10  
        if(isalpha(ch))  
                return isupper(ch) ? ch - 55 : ch - 87;  
  
        return -1;  
}  
  
/* 
 * 功能：将十六进制字符串转换为整型(int)数值 
 * */  
static int StringHexToInt(char *hex)  
{  
        int len;  
        int num = 0;  
        int temp;  
        int bits;  
        int i;  
          
        // 此例中 hex = "1de" 长度为3, hex是main函数传递的  
        len = strlen(hex);  
  
        for (i=0, temp=0; i<len; i++, temp=0)  
        {  
                // 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1  
                // 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13  
                // 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14  
                temp = CharHexToInt( *(hex + i) );  
                // 总共3位，一个16进制位用 4 bit保存  
                // 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位  
                // 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位  
                // 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位  
                bits = (len - i - 1) * 4;  
                temp = temp << bits;  
  
                // 此处也可以用 num += temp;进行累加  
                num = num | temp;  
        }  
  
        // 返回结果  
        return num;  
}  

// HexString转换为int
int FKCW_Base_Utils::Hex2I(const char* str)
{
	int value = 0;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	value = std::stoul(str, nullptr, 16);
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	value = StringHexToInt( const_cast<char*>(str) );
#endif

	return value;
}
//-------------------------------------------------------------------------
string FKCW_Base_Utils::Char2hex( char dec )
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48 in ascii
	if (10<= dig1 && dig1<=15) dig1+=65-10; //A,65 in ascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=65-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}
//-------------------------------------------------------------------------
string FKCW_Base_Utils::Bin2Hex(const unsigned char * input, size_t length)
{
	static const char HEX[16] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	std::string str;
	str.reserve(length << 1);
	for(size_t i = 0; i < length; i++) {
		int t = input[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, HEX[a]);
		str.append(1, HEX[b]);
	}
	return str;
}
//-------------------------------------------------------------------------
// 判断一个字符串是否是数字串
bool FKCW_Base_Utils::IsDigital( const string& str )
{
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			return false;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// 判断一个字符串是否是十六进制的数字串
bool FKCW_Base_Utils::IsDigitalHex( const string& str )
{
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			if( ch < 'a' || ch > 'f' )
			{
				if( ch < 'A' || ch > 'F' )
				{
					return false;
				}
			}
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// 字符串转换为int
// 参数：nBase是指数底，例如10进制用10,16进制用16
bool FKCW_Base_Utils::AtoI( const string& str, int nBase, int& nOutNum )
{
	nOutNum = 0;
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			if( ch < 'a' || ch > 'f' )
			{
				if( ch < 'A' || ch > 'F' )
				{
					return false;
				}
				else
				{
					nOutNum += ( ch - 'A' + 10 ) * static_cast<int>( pow(static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
				}
			}
			else
			{
				nOutNum += ( ch - 'a' + 10 ) * static_cast<int>( pow( static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
			}
		}
		else
		{
			nOutNum += ( ch - '0' ) * static_cast<int>( pow( static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// 获取一个字符串的Hash值
unsigned int FKCW_Base_Utils::HashString( const char* p_szSrc )
{
	int nHash = 0;
	int nX    = 0;

	int nLen = static_cast<int>( strlen( p_szSrc ) );
	for(int i = 0; i < nLen; ++i )
	{
		nHash = (nHash << 4) + p_szSrc[i];
		if((nX = (int)(nHash & 0xF0000000L)) != 0)
		{
			nHash ^= (nX >> 24);
			nHash &= ~nX;
		}
	}

	return (nHash & 0x7FFFFFFF);
}
//-------------------------------------------------------------------------
// 拷贝一个字符串
// 返回值：拷贝后的字符串，注意，调用者应当负责释放该字符串内存
const char*	FKCW_Base_Utils::Copy( const char* p_szSrc )
{
	if( p_szSrc == NULL )
		return NULL;

	unsigned int unLen = strlen( p_szSrc );
	char* c = ( char*  )calloc( unLen + 1, sizeof(char) );
	memcpy( c, p_szSrc, unLen );
	return c;
}
//-------------------------------------------------------------------------
// 拷贝一个字符串的指定部分
// 返回值：拷贝后的字符串，注意，调用者应当负责释放该字符串内存
const char*	FKCW_Base_Utils::Copy( const char* p_szSrc, int p_nStart, size_t p_unLen )
{
	if( p_szSrc == NULL )
		return NULL;

	unsigned int unLen = strlen( p_szSrc );
	char* c = ( char*  )calloc( unLen + 1, sizeof(char) );
	memcpy( c, p_szSrc + p_nStart, unLen );
	return c;
}
//-------------------------------------------------------------------------
// 判断一个字符是否是空格字符
bool FKCW_Base_Utils::IsSpace( const char a )
{
	return ((int)a==32||a=='\t'||a=='\n'||a=='\v'||a=='\f'||a=='\r');
}
//-------------------------------------------------------------------------
// 去掉字符串左侧空白
void FKCW_Base_Utils::LeftTrim( string& str )
{
	if (str.empty())
	{
		return;
	}

	std::basic_string<char>::iterator it = str.begin();
	size_t nPos = 0;
	while( it != str.end() )
	{
		if( (*it) != ' ' )
		{
			break;
		}

		++it;
		++nPos;
	}

	if( str.length() == nPos )
	{
		str.clear();
	}
	else
	{
		str = str.substr( nPos, str.length() - nPos );
	}
}
//-------------------------------------------------------------------------
// 去除字符串右侧空白
void FKCW_Base_Utils::RightTrim( string& str )
{
	if (str.empty())
	{
		return;
	}

	std::basic_string<char>::reverse_iterator it = str.rbegin();
	size_t nLenght = str.size();
	for( ;it != str.rend(); ++it )
	{
		if( (*it) != ' ' )
		{
			break;
		}

		--nLenght;
	}

	if( nLenght == 0 )
	{
		str.clear();
	}
	else
	{
		str = str.substr( 0, nLenght );
	}
}
//-------------------------------------------------------------------------
// 空格分隔字符串(去空格头尾)
string FKCW_Base_Utils::Trim( const string& s )
{
	if(s.empty())
		return s;

	// 删除头尾
	size_t start = 0;
	size_t end = s.length();
	
	while(end > 0 && IsSpace(s.at(end - 1)))
		end--;
	while(start < end && IsSpace(s.at(start)))
		start++;

	return s.substr(start, end - start);
}
//-------------------------------------------------------------------------
// 将括号进行字符串分割，取得其前部，括号中部(已去除括号本身)，括号后部
void FKCW_Base_Utils::TrimWithBrackets( const string& s, string& head, string& in, string& tail )
{
	if( s.empty() )
		return;
	size_t start = 0;
	size_t end = s.length();
	while(end > 0 && (s.at(end - 1) != ')'))
	{
		end--;
	}
	while(start < end && (s.at(start) != '('))
	{
		start++;
	}
	if( start > end-2 )
		return;

	head = s.substr( 0, start );
	in = s.substr( start+1, end - start - 2 );
	tail = s.substr( end, s.length() );
}
//-------------------------------------------------------------------------
// 转换字符串为小写
void FKCW_Base_Utils::ToLowerCase( string& s )
{
	if( s.empty() )
		return;

	unsigned int unLen = s.length();
	char* szBuf = new char[unLen + 1];
	strcpy(szBuf, s.c_str());
	for(unsigned int i = 0; i < unLen; i++)
	{
		if(szBuf[i] >= 0x41 && szBuf[i] <= 0x5A)
		{
			szBuf[i] += 0x20;
		}
	}

	s = szBuf;
	delete szBuf;
}
//-------------------------------------------------------------------------
// 判断一个字符串(s)首部字符串是否与制定字符串(sub)匹配
bool FKCW_Base_Utils::IsStartWith( const string& s, const string& sub )
{
	return s.find( sub ) == 0;
}
//-------------------------------------------------------------------------
// 判断一个字符串(s)尾部字符串是否与制定字符串(sub)匹配
bool FKCW_Base_Utils::IsEndWith( const string& s, const string& sub )
{
	return s.rfind( sub ) == ( s.length() - sub.length() );
}
//-------------------------------------------------------------------------
// 更换一个字符串中的所有字符为指定字符
void FKCW_Base_Utils::ReplaceChar( string& s, char p_cOld, char p_cNew )
{
	size_t len = s.length();
	char* buf = new char[len + 1];
	strcpy(buf, s.c_str());

	for(unsigned int i = 0; i < len; i++) 
	{
		if(buf[i] == p_cOld) {
			buf[i] = p_cNew;
		}
	}

	s = buf;
	delete buf;
}
//-------------------------------------------------------------------------
// 更换一个字符串中的所有字符为指定字符串
string FKCW_Base_Utils::Replace( string& s, const string& p_strOld, const string& p_strNew )
{
	string ret = s;
	size_t pos = ret.length();
	while(true)   {
		if((pos = ret.rfind(p_strOld, pos)) != string::npos)
			ret = ret.replace(pos, p_strOld.length(), p_strNew);
		else
			break;
	}

	return ret;
}
//-------------------------------------------------------------------------
// 从一个字符串中移除一个字符
void FKCW_Base_Utils::RemoveChar( string& s, char c )
{
	for(string::iterator it = s.begin(); it != s.end(); it++) 
	{
		if(*it == c) {
			s.erase(it);
		}
	}
}
//-------------------------------------------------------------------------
// 根据一个分隔符进行字符串分割( 输出类型不同，分别为string, int, float, bool )
// 注：返回值不需要外部释放，但注意返回值数组为公共数组，获得后请立刻拷贝，否则该部分内存将被其他函数重复使用
CCArray& FKCW_Base_Utils::ComponentOfString( const string& s, const char p_cSeparator )
{
	s_TmpArray.removeAllObjects();

	if(s.empty()) {
		s_TmpArray.addObject(CCString::create(""));
		return s_TmpArray;
	}

	size_t start = 0;
	size_t end = s.length() - 1;
	char c = s[start];
	while(c == '{' || c == '[' || c == '(') {
		start++;
		c = s[start];
	}
	c = s[end];
	while(c == '}' || c == ']' || c == ')') {
		end--;
		c = s[end];
	}

	size_t compStart = start;
	for(size_t i = start; i <= end; i++) {
		c = s[i];
		if(c == p_cSeparator) {
			s_TmpArray.addObject(CCString::create(s.substr(compStart, i - compStart)));
			compStart = i + 1;
		} else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			if(compStart == i) {
				compStart++;
			}
		}
	}

	if(compStart <= end) {
		s_TmpArray.addObject(CCString::create(s.substr(compStart, end - compStart + 1)));
	} else if(s[end] == p_cSeparator) {
		s_TmpArray.addObject(CCString::create(""));
	}

	return s_TmpArray;
}
//-------------------------------------------------------------------------
CCArray& FKCW_Base_Utils::IntComponentOfString( const string& s, const char p_cSeparator )
{
	s_TmpArray.removeAllObjects();

	// 检查
	if(s.empty())
	{
		s_TmpArray.addObject(CCInteger::create(0));
		return s_TmpArray;
	}

	// 删除头，尾，括号
	size_t start = 0;
	size_t end = s.length() - 1;
	char c = s[start];
	while(c == '{' || c == '[' || c == '(') {
		start++;
		c = s[start];
	}
	c = s[end];
	while(c == '}' || c == ']' || c == ')') {
		end--;
		c = s[end];
	}

	// 迭代处理
	size_t compStart = start;
	for(size_t i = start; i <= end; i++) {
		c = s[i];
		if(c == p_cSeparator) 
		{
			s_TmpArray.addObject(CCInteger::create(atoi(s.substr(compStart, i - compStart).c_str())));
			compStart = i + 1;
		} else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') 
		{
			if(compStart == i) 
			{
				compStart++;
			}
		}
	}

	// 最后矫正
	if(compStart <= end) 
	{
		s_TmpArray.addObject(CCInteger::create(atoi(s.substr(compStart, end - compStart + 1).c_str())));
	} else if(s[end] == p_cSeparator) 
	{
		s_TmpArray.addObject(CCInteger::create(0));
	}

	return s_TmpArray;
}
//-------------------------------------------------------------------------
CCArray& FKCW_Base_Utils::FloatComponentOfString( const string& s, const char p_cSeparator )
{
	s_TmpArray.removeAllObjects();

	if(s.empty()) {
		s_TmpArray.addObject(CCFloat::create(0));
		return s_TmpArray;
	}

	size_t start = 0;
	size_t end = s.length() - 1;
	char c = s[start];
	while(c == '{' || c == '[' || c == '(') {
		start++;
		c = s[start];
	}
	c = s[end];
	while(c == '}' || c == ']' || c == ')') {
		end--;
		c = s[end];
	}

	size_t compStart = start;
	for(size_t i = start; i <= end; i++) {
		c = s[i];
		if(c == p_cSeparator) {
			s_TmpArray.addObject(CCFloat::create(atof(s.substr(compStart, i - compStart).c_str())));
			compStart = i + 1;
		} else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			if(compStart == i) {
				compStart++;
			}
		}
	}

	if(compStart <= end) {
		s_TmpArray.addObject(CCFloat::create(atof(s.substr(compStart, end - compStart + 1).c_str())));
	} else if(s[end] == p_cSeparator) {
		s_TmpArray.addObject(CCFloat::create(0));
	}

	return s_TmpArray;
}
//-------------------------------------------------------------------------
CCArray& FKCW_Base_Utils::BoolComponentOfString( const string& s, const char p_cSeparator )
{
	s_TmpArray.removeAllObjects();

	if(s.empty()) {
		s_TmpArray.addObject(CCBool::create(false));
		return s_TmpArray;
	}

	size_t start = 0;
	size_t end = s.length() - 1;
	char c = s[start];
	while(c == '{' || c == '[' || c == '(') {
		start++;
		c = s[start];
	}
	c = s[end];
	while(c == '}' || c == ']' || c == ')') {
		end--;
		c = s[end];
	}

	size_t compStart = start;
	for(size_t i = start; i <= end; i++) {
		c = s[i];
		if(c == p_cSeparator) {
			string sub = s.substr(compStart, i - compStart);
			ToLowerCase(sub);
			s_TmpArray.addObject(CCBool::create(sub == "y" || sub == "yes" || sub == "true" || atoi(sub.c_str()) > 0));
			compStart = i + 1;
		} else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			if(compStart == i) {
				compStart++;
			}
		}
	}

	if(compStart <= end) {
		string sub = s.substr(compStart, end - compStart + 1);
		ToLowerCase(sub);
		s_TmpArray.addObject(CCBool::create(sub == "y" || sub == "yes" || sub == "true" || atoi(sub.c_str()) > 0));
	} else if(s[end] == p_cSeparator) {
		s_TmpArray.addObject(CCBool::create(false));
	}

	return s_TmpArray;
}
//-------------------------------------------------------------------------
// 通过分隔符和数组生成一个字符串（上一系列函数的逆向）
string FKCW_Base_Utils::JoinString( const CCArray& a, const char p_cSeparator )
{
	string ret;
	CCObject* obj;
	CCARRAY_FOREACH(&a, obj) {
		CCString* ccs = (CCString*)obj;
		if(ret.length() > 0)
			ret += ',';
		ret += ccs->getCString();
	}
	return ret;
}
//-------------------------------------------------------------------------
string FKCW_Base_Utils::JoinInt( const CCArray& a, const char p_cSeparator )
{
	string ret;
	CCObject* obj;
	char buf[64];
	CCARRAY_FOREACH(&a, obj) {
		CCInteger* cci = (CCInteger*)obj;
		if(ret.length() > 0)
			ret += ',';
		sprintf(buf, "%d", cci->getValue());
		ret += buf;
	}
	return ret;
}
//-------------------------------------------------------------------------
string FKCW_Base_Utils::JoinFloat( const CCArray& a, const char p_cSeparator )
{
	string ret;
	CCObject* obj;
	char buf[64];
	CCARRAY_FOREACH(&a, obj) {
		CCFloat* ccf = (CCFloat*)obj;
		if(ret.length() > 0)
			ret += ',';
		sprintf(buf, "%f", ccf->getValue());
		ret += buf;
	}
	return ret;
}
//-------------------------------------------------------------------------
string FKCW_Base_Utils::JoinBool( const CCArray& a, const char p_cSeparator )
{
	string ret;
	CCObject* obj;
	CCARRAY_FOREACH(&a, obj) {
		CCBool* ccb = (CCBool*)obj;
		if(ret.length() > 0)
			ret += ',';
		ret += ccb->getValue() ? "true" : "false";
	}
	return ret;
}
//-------------------------------------------------------------------------
// 将字符串转换为坐标值
/* 例：
* {3.2} ==> ccp(3.2, 0)
* {,3.4} ==> ccp(0, 3.4)
* {3.2 ==> ccp(3.2, 0)
* {,3.4 ==> ccp(0, 3.4)
* {2,3,4} ==> ccp(2, 3)
* {[2,3}]) ==> ccp(2,3)
* {,} ==> ccp(0, 0)
*/
// 注：若解析失败，则返回ccPointZero
CCPoint FKCW_Base_Utils::CcpFromString( const string& s )
{
	CCArray& comp = ComponentOfString(s, ',');
	float x = 0, y = 0;
	if(comp.count() > 0) {
		x = atof(((CCString*)comp.objectAtIndex(0))->getCString());
	}
	if(comp.count() > 1) {
		y = atof(((CCString*)comp.objectAtIndex(1))->getCString());
	}
	return ccp(x, y);
}
//-------------------------------------------------------------------------
// 将字符串转换为CCSzie大小
/* 例：
* {3.2} ==> ccp(3.2, 0)
* {,3.4} ==> ccp(0, 3.4)
* {3.2 ==> ccp(3.2, 0)
* {,3.4 ==> ccp(0, 3.4)
* {2,3,4} ==> ccp(2, 3)
* {[2,3}]) ==> ccp(2,3)
* {,} ==> ccp(0, 0)
*/
// 注：若解析失败，则返回ccSizeZero
CCSize FKCW_Base_Utils::CcsFromString(const string& s)
{
	CCArray& comp = ComponentOfString(s, ',');
	float x = 0, y = 0;
	if(comp.count() > 0) {
		x = atof(((CCString*)comp.objectAtIndex(0))->getCString());
	}
	if(comp.count() > 1) {
		y = atof(((CCString*)comp.objectAtIndex(1))->getCString());
	}
	return CCSizeMake(x, y);
}
//-------------------------------------------------------------------------
// 将字符串转换为CCRect矩形
/* 例：
* {x,y,w,h} ==> ccRectMake( x, y, w,h );
*/
// 注：若解析失败，则返回ccRectZero
CCRect FKCW_Base_Utils::CcrFromString(const string& s)
{
	CCArray& comp = ComponentOfString(s, ',');
	float x = 0, y = 0, w = 0, h = 0;
	if(comp.count() > 0) {
		x = atof(((CCString*)comp.objectAtIndex(0))->getCString());
	}
	if(comp.count() > 1) {
		y = atof(((CCString*)comp.objectAtIndex(1))->getCString());
	}
	if(comp.count() > 2) {
		w = atof(((CCString*)comp.objectAtIndex(2))->getCString());
	}
	if(comp.count() > 3) {
		h = atof(((CCString*)comp.objectAtIndex(3))->getCString());
	}
	return CCRectMake(x, y, w, h);
}
//-------------------------------------------------------------------------
// 将字符串转换为数组
/* 例：
* {a1,a2,a3,...}
*/
// 注1：若内部元素有单引号，或者双引号包含，则返回的是string数组。否则返回的是float数组。
// 注2：返回值不需要外部释放，但注意返回值数组为公共数组，获得后请立刻拷贝，否则该部分内存将被其他函数重复使用
CCArray& FKCW_Base_Utils::ArrayFromString( const string& s )
{
	CCArray& comp = ComponentOfString(s, ',');
	s_TmpArray.removeAllObjects();

	// 逐个迭代组件
	CCObject* obj;
	CCARRAY_FOREACH(&comp, obj) {
		CCString* ccs = (CCString*)obj;
		string cs = ccs->getCString();
		if(cs.length() > 0) {
			if(cs[0] == '\'' || cs[0] == '"') {
				size_t start = 1;
				size_t end = cs.length() - 1;
				if(cs[end] == '\'' || cs[end] == '"') {
					end--;
				}
				if(end >= start) {
					s_TmpArray.addObject(CCString::create(cs.substr(start, end - start + 1)));
				} else {
					s_TmpArray.addObject(CCString::create(""));
				}
			} else {                
				float f = atof(cs.c_str());
				s_TmpArray.addObject(CCFloat::create(f));
			}
		} else {
			s_TmpArray.addObject(CCFloat::create(0));
		}
	}

	return s_TmpArray;
}
//-------------------------------------------------------------------------
// 将数组转换为字符串
// 相当于 ArrayFromString 的逆向
string FKCW_Base_Utils::ArrayToString( const CCArray& arr )
{
	string ret = "[";
	CCObject* obj;
	char buf[128]; 
	CCARRAY_FOREACH(&arr, obj) {
		CCString* s = dynamic_cast<CCString*>(obj);
		if(s) {
			if(ret.length() > 1)
				ret.append(",");
			ret.append("\"");
			ret.append(s->getCString());
			ret.append("\"");
			continue;
		}

		CCInteger* i = dynamic_cast<CCInteger*>(obj);
		if(i) {
			if(ret.length() > 1)
				ret.append(",");
			sprintf(buf, "%d", i->getValue());
			ret.append(buf);
			continue;
		}

		CCFloat* f = dynamic_cast<CCFloat*>(obj);
		if(f) {
			if(ret.length() > 1)
				ret.append(",");
			sprintf(buf, "%f", f->getValue());
			ret.append(buf);
			continue;
		}

		CCDouble* d = dynamic_cast<CCDouble*>(obj);
		if(d) {
			if(ret.length() > 1)
				ret.append(",");
			sprintf(buf, "%lf", d->getValue());
			ret.append(buf);
			continue;
		}
	}

	ret.append("]");
	return ret;
}
//-------------------------------------------------------------------------
// float转换string
string FKCW_Base_Utils::FloatToString( float f )
{
	ostringstream buffer;
	buffer << f;
	return( buffer.str() );
}
//-------------------------------------------------------------------------
// int转换string
string FKCW_Base_Utils::IntToString( int n )
{
	stringstream stream;  
	stream << n;  
	return( stream.str() );
}
//-------------------------------------------------------------------------
// 计算一个UTF-8字符占用多少字节
int FKCW_Base_Utils::GetUTF8Btyes( unsigned char c )
{
	int count = 1;
	if(c >= 128) {
		c <<= 1;
		do {
			count++;
			c <<= 1;
		} while(c > 128);
	}

	return count;
}
//-------------------------------------------------------------------------
// 计算一个UTF-8字符串长度
int FKCW_Base_Utils::GetUTF8StrLen( const char* p_str )
{
	char* str = (char*) p_str;
	int clen = strlen(p_str);
	int len = 0;

	for(char *ptr = str; 
		*ptr != 0 && len < clen; 
		len++, 
		ptr += GetUTF8Btyes(((unsigned char)*ptr))
		);

	return len;
}
//-------------------------------------------------------------------------
// 计算一个UTF-8字符串内有多少个UTF-8字符
int FKCW_Base_Utils::GetUTF8CharCount( const char* p_szUTF8 )
{
	if(!p_szUTF8)
		return 0;

	int count = 0;
	char* p = (char*)p_szUTF8;
	while(*p) {
		count++;
		int b = GetUTF8Btyes(*p);
		p += b;
	}

	return count;
}
//-------------------------------------------------------------------------
// 获取一个UTF-8字符串子字符串
char* FKCW_Base_Utils::SubUTF8String( const char* p_str, unsigned int start, unsigned int end )
{
	int len = GetUTF8StrLen(p_str);
	char* str = (char*) p_str;

	if( (int)start >= len ) return NULL;
	if( (int)end > len ) end = len;

	char *sptr = str;
	for(int i = 0; i < (int)start; ++i,sptr += GetUTF8Btyes((unsigned char)*sptr));

	char *eptr = sptr;
	for(int i = start; i < (int)end; ++i,eptr += GetUTF8Btyes((unsigned char)*eptr));

	int retLen = eptr - sptr;
	char *retStr = new char[(retLen + 1)];
	memcpy(retStr, sptr, retLen);
	retStr[retLen] = 0;

	return retStr;
}
//-------------------------------------------------------------------------
// 文件压缩
int FKCW_Base_Utils::CompressFile( const char* file_in, const char* file_out, int level )
{
	FILE* fin = fopen(file_in, "rb");
	if ( !fin )
	{
		FKLOG("FKCW_Base_Utils::CompressFile: open input file failed %s\n", file_in);
		return -1;
	}
	FILE* fout = fopen(file_out, "wb+");
	if ( !fout )
	{
		FKLOG("FKCW_Base_Utils::CompressFile: open output file failed %s\n", file_out);
		fclose(fin);
		return -1;
	}

	int ret = FKCW_Base_FileZip::Compress(fin, fout, level);

	fclose(fout);
	fclose(fin);
	return ret;
}
//-------------------------------------------------------------------------
// 文件解压缩
int FKCW_Base_Utils::DecompressFile( const char* file_in, const char* file_out, 
									double* pnow /*= NULL*/, double* ptotal /*= NULL*/ )
{
	FILE* fin = fopen(file_in, "rb");
	if ( !fin )
	{
		FKLOG("FKCW_Base_Utils::DecompressFile: open input file failed %s\n", file_in);
		return -1;
	}

	if ( ptotal )
	{
		fseek(fin, 0, SEEK_END); 
		*ptotal = ftell(fin);
		fseek(fin, 0, SEEK_SET);
	}

	FILE* fout = fopen(file_out, "wb+");
	if ( !fout )
	{
		FKLOG("FKCW_Base_Utils::DecompressFile: open output file failed %s\n", file_out);
		fclose(fin);
		return -1;
	}

	int ret = FKCW_Base_FileZip::Decompress(fin, fout, pnow);

	fclose(fout);
	fclose(fin);
	return ret;
}
//-------------------------------------------------------------------------
// 设置文件访问权限
bool FKCW_Base_Utils::Access( const char* p_szPath, int p_nMode )
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return ::_access(p_szPath, p_nMode) == 0;
#else
	CCLOG("FKCW_Base_Utils::Access Android Platform have no method _access");
	return true;
#endif
}
//-------------------------------------------------------------------------
// 创建目录
bool FKCW_Base_Utils::MakeDir( const char* p_szPath )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return ::_mkdir(p_szPath) == 0;
#else
	return ::mkdir(p_szPath, S_IRWXU | S_IRWXG | S_IRWXO) == 0;
#endif
}
//-------------------------------------------------------------------------
// 构建目录路径
bool FKCW_Base_Utils::BuildDir( const char* path )
{
	std::string str(path);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	std::replace(str.begin(), str.end(), '\\', '/');
#else
	std::string strOld = "\\";
	std::string strNew = "/";
	Replace(str,strOld, strNew);
#endif
	
	

	size_t end = str.find_last_not_of('/');
	bool dummy = false;
	for ( size_t i = 1; // ignore root
		i < str.size(); i++ )
	{
		if ( str[i] == '/' && !dummy )
		{
			std::string bpath = str.substr(0, i);
			if ( !FKCW_Base_Utils::Access(bpath.c_str(), 0) )
			{
				if ( !FKCW_Base_Utils::MakeDir(bpath.c_str()) )
				{
					// FIXED: if return here, multi-thread mkdir will cause error.
					return false;
				}
			}
			dummy = true;
		}
		else
		{
			dummy = false;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// 文件重命名
bool FKCW_Base_Utils::Rename( const char* p_szFrom, const char* p_szTo )
{
	return 0 == ::rename(p_szFrom, p_szTo);
}
//-------------------------------------------------------------------------
// 删除文件 
bool FKCW_Base_Utils::Remove( const char* p_szPath )
{
	return 0 == ::remove(p_szPath);
}
//-------------------------------------------------------------------------
// 转Url中文为实际编码
string FKCW_Base_Utils::UrlEncode( const string& c )
{
	std::string escaped;
	int max = c.length();
	for(int i=0; i<max; i++)
	{
		if ( (48 <= c[i] && c[i] <= 57) ||						//0-9
			(65 <= c[i] && c[i] <= 90) ||						//ABC...XYZ
			(97 <= c[i] && c[i] <= 122) ||						//abc...xyz
			(c[i]=='~' || c[i]=='-' || c[i]=='_' || c[i]=='.')
			)
		{
			escaped.append( &c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append( Char2hex(c[i]) );					//converts char 255 to string "FF"
		}
	}
	return escaped;
}
//-------------------------------------------------------------------------
// 解码HTML实体字符串
string FKCW_Base_Utils::DecodeHtmlEntities( const string& p_strSrc )
{
	char* dest = (char*)calloc(p_strSrc.length() + 1, sizeof(char));
	decode_html_entities_utf8(dest, p_strSrc.c_str());
	string decoded = string(dest);
	free(dest);
	return decoded;
}
//-------------------------------------------------------------------------
// 将一个中文字符串转换为拼音字符串
// 注：当前仅支持 utf-8 字符串，GBK 字符串暂时不支持
string FKCW_Base_Utils::GetPinyin( const string& s )
{
	return FKCW_Base_PinYinUtils::Chs2Pinyin(s);
}
//-------------------------------------------------------------------------
// 获取一个数字的位数，例如0,1,2就是1位，12,38就是2位，9821就是4位
// 注：若是负数，则无视负号
int FKCW_Base_Utils::GetNumDigits( int p_nNum )
{
	int d = 1;
	p_nNum = abs(p_nNum);
	p_nNum /= 10;
	while(p_nNum > 0) {
		d++;
		p_nNum /= 10;
	}
	return d;
}
//-------------------------------------------------------------------------
// 系统Sleep
void FKCW_Base_Utils::SystemSleep( unsigned int millisec )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	Sleep( millisec );
#else
	usleep(millisec * 1000);
#endif
}
//-------------------------------------------------------------------------
// 系统释放
void FKCW_Base_Utils::SystemYield()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	Sleep(0);
#else
	usleep(0);
#endif
}
//-------------------------------------------------------------------------
// 获取一个字符串中最后的文件夹分隔符'\\'位置
// 返回值：若没有文件夹分隔符，则返回-1
int FKCW_Base_Utils::LastSlashIndex( string p_strPath )
{
	if(p_strPath.empty())
		return -1;

	// 将分隔符更改为windows风格
	if(CC_PATH_SEPARATOR != '/')
		ReplaceChar(p_strPath, '/', CC_PATH_SEPARATOR);
	return p_strPath.rfind( CC_PATH_SEPARATOR, std::string::npos);
	/*
	// 下面代码不能正确处理尾部为 \\ 的字符串。例如 "123\\" 会返回-1错误结果
	// 查找斜杠索引位置
	size_t len = p_strPath.length();
	int end = len;
	int slash = -1;
	for(int i = len - 1; i >= 0; i--) 
	{
		if(p_strPath[i] == CC_PATH_SEPARATOR) 
		{
			if(i == end - 1)
			{
				end--;
				if(i == 0) 
				{
					slash = 0;
					break;
				}
			} 
			else 
			{
				slash = i;
				break;
			}
		}
	}

	// 跳过多余的斜杠
	if(slash != -1) {
		while(slash >= 1 && p_strPath[slash - 1] == CC_PATH_SEPARATOR)
			slash--;
	}

	// 分配完毕
	end = slash;
	if(end == 0)
		end = 1;

	return end;
	*/
}
//-------------------------------------------------------------------------
// 获取一个字符串中最后的点号'.'位置
// 返回值：若没有点号，则返回-1
int FKCW_Base_Utils::LastDotIndex( const string& p_strPath )
{
	if(p_strPath.empty())
		return -1;

	int len = p_strPath.length();
	for(int i = len - 1; i >= 0; i--) {
		if(p_strPath[i] == '.')
			return i;
	}

	return -1;
}
//-------------------------------------------------------------------------
// 获取一个路径字符串最后的片段（以文件夹分隔符判断'\\'）
string FKCW_Base_Utils::LastPathOfComponent( const string& p_strPath )
{
	// 更换分隔符为windows的分隔符
	if(CC_PATH_SEPARATOR != '/')
		ReplaceChar((string&)p_strPath, '/', CC_PATH_SEPARATOR);

	size_t len = p_strPath.length();
	int start = 0;
	int end = len;
	for(int i = len - 1; i >= 0; i--) {
		if(p_strPath[i] == CC_PATH_SEPARATOR) {
			if(i == end - 1)
				end--;
			else {
				start = i + 1;
				break;
			}
		}
	}

	if(end < start)
		return p_strPath;
	else
		return p_strPath.substr(start, end);
}
//-------------------------------------------------------------------------
// 删除一个路径字符串最后的片段
/* 例: 输入					输出
* "/tmp/scratch.tiff"		"/tmp"
* "/tmp/scratch"			"/tmp"
* "/tmp/"					"/tmp"
* "scratch"					""
* "/" 						"/"
*/
string FKCW_Base_Utils::DeleteLastPathOfComponment( const string& p_strPath )
{
	int end = LastSlashIndex(p_strPath);
	if(end < 0)
		return "";
	else
		return p_strPath.substr(0, end);
}
//-------------------------------------------------------------------------
// 为一个路径添加一个尾片段
/* 例：输入					输出
* "/tmp", "/scratch.tiff"	"/tmp/scratch.tiff"
* "/tmp//", "/scratch"		"/tmp/scratch"
* "/tmp", "/"				"/tmp"
* "/", "tmp/" 				"/tmp"
*/
string FKCW_Base_Utils::AppendLastPathOfComponment( const string& p_strPath, const string& p_strComponent )
{
	// 更换为windows格式分隔符
	if(CC_PATH_SEPARATOR != '/')
		ReplaceChar((string&)p_strPath, '/', CC_PATH_SEPARATOR);

	if(CC_PATH_SEPARATOR != '/')
		ReplaceChar((string&)p_strComponent, '/', CC_PATH_SEPARATOR);

	// 检查合法性
	if(p_strPath.empty()) {
		if(p_strComponent.empty())
			return "";
		else
			return p_strComponent;
	} else if(p_strComponent.empty()) {
		return p_strPath;
	}

	// 分配一块内存，+2是其中之一放置分隔符，一个补充结尾'\0'终止符
	size_t len = p_strPath.length();
	int cLen = p_strComponent.length();
	char* buf = new char[len + cLen + 2];
	memset(buf, 0, len + cLen + 2);

	// 复制路径
	memcpy(buf, p_strPath.c_str(), len);

	int start = len;
	if(start > 0) {
		if(buf[start - 1] != CC_PATH_SEPARATOR) {
			buf[start++] = CC_PATH_SEPARATOR;
		} else {
			while(start >= 2 && buf[start - 2] == CC_PATH_SEPARATOR)
				start--;
		}
	}

	// 复制尾片段
	int cStart = 0;
	while(cStart < cLen && p_strComponent[cStart] == CC_PATH_SEPARATOR)
		cStart++;
	if(cStart > 0 && start == 0)
		cStart--;
	memcpy(buf + start, p_strComponent.c_str() + cStart, cLen - cStart);

	// 删除尾部分隔符
	int end = start + cLen - cStart - 1;
	while(buf[end] == CC_PATH_SEPARATOR)
		buf[end--] = 0;

	string ret = buf;
	delete buf;
	return ret;
}
//-------------------------------------------------------------------------
// 删除文件后缀名
string FKCW_Base_Utils::DeletePathExtension( const string& p_strPath )
{
	int end = LastDotIndex(p_strPath);
	int slash = LastSlashIndex((string&)p_strPath);
	if(end >= 0) {
		if(end > slash)
			return p_strPath.substr(0, end);
		else
			return p_strPath;
	} else {
		return p_strPath;
	}
}
//-------------------------------------------------------------------------
// 获取文件后缀名
// 注：返回值带 . 点号。例：".exe"".jpg"
string FKCW_Base_Utils::GetPathExtension( const string& p_strPath )
{
	int end = LastDotIndex(p_strPath);
	if(end >= 0) {
		return p_strPath.substr(end);
	} else {
		return "";
	}
}
//-------------------------------------------------------------------------
// 获取一个路径的父路径
string FKCW_Base_Utils::GetParentPath( const string& p_strPath )
{
	if(p_strPath.empty())
		return "";

	size_t slash = p_strPath.rfind('/');
	if(slash == string::npos)
		return "";
	else if(slash == 0)
		return p_strPath.substr(0, 1);
	else
		return p_strPath.substr(0, slash);
}
//-------------------------------------------------------------------------
// 获取路径完成路径
// 若该路径是绝对路径，则什么也不做
// 若iOS中，则在路径前面添加 ~/Documents
// 若Android中，则在路径前面添加 内部存储文件夹路径
// windows中，则什么也不做
string FKCW_Base_Utils::Externalize(const string& path)
{
	if(CCFileUtils::sharedFileUtils()->isAbsolutePath(path)) 
	{
		return path;
	} 
	else 
	{
		return CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
	}
}
//-------------------------------------------------------------------------
// 拆分文件路径字符串
bool FKCW_Base_Utils::ParseFilePathName( const std::string& strFilePathName,
										  std::string& strPathName, std::string& strFileName, std::string& strFileExtName )
{
	if( strFilePathName.empty() )
	{
		return false;
	}

	strPathName.clear();
	strFileName.clear();
	strFileExtName.clear();

	std::size_t point_pos = strFilePathName.rfind( '.' ); 

	std::size_t pos = strFilePathName.rfind('\\');
	if( pos == std::string::npos )
	{
		pos = strFilePathName.rfind('/');
		if( pos == std::string::npos )
		{
			if( point_pos == std::string::npos )
			{
				return false;
			}
		}
	}

	if( point_pos == std::string::npos )
	{
		strPathName = strFilePathName;
	}
	else
	{
		/// 以斜杠结尾
		if( pos == strFilePathName.length() - 1 )
		{
			strPathName = strFilePathName.substr( 0, pos );
		}
		else
		{
			if( pos != std::string::npos )
			{
				strPathName = strFilePathName.substr( 0, pos );
			}

			strFileName = strFilePathName.substr( pos+1, point_pos - pos - 1 );
			strFileExtName = strFilePathName.substr( point_pos + 1, strFilePathName.length() - point_pos - 1 );
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// 判断一个路径是否是绝对路径
bool FKCW_Base_Utils::IsAbsolutePath( const string& strFilePathName )
{
	if( strFilePathName.empty() )
	{
		return false;
	}

	std::string strTemp = strFilePathName;
	Replace( strTemp, "\\", "/" );

	return strFilePathName.find( ":/") == 1;
}
//-------------------------------------------------------------------------
// 对浮点数进行一定精度的四舍五入 123.54 -> 123.50
double FKCW_Base_Utils::Round( double x, int p_nPrecision )
{
	double div = pow(10, -p_nPrecision);
	return floor(x / div + 0.5) * div;
}
//-------------------------------------------------------------------------
float FKCW_Base_Utils::Round( float f )
{
	return (f > 0.0) ? floor(f + 0.5) : ceil(f - 0.5);
}
//-------------------------------------------------------------------------
// 对浮点数进行一定精度的舍弃 123.54 -> 123.0
double FKCW_Base_Utils::Floor( double x, int p_nPrecision )
{
	double div = pow(10, -p_nPrecision);
	return (int)floor(x / div) * div;
}
//-------------------------------------------------------------------------
// 对浮点数进行一定精度的无条件进位 123.54 -> 124.0
double FKCW_Base_Utils::Ceil( double x, int p_nPrecision )
{
	double div = pow(10, -p_nPrecision);
	return (int)ceil(x / div) * div;
}
//-------------------------------------------------------------------------
// 线性内插
float FKCW_Base_Utils::Lerp( float a, float b, float p )
{
	return a * (1 - p) + b * p;
}
//-------------------------------------------------------------------------
// 求平方值
float FKCW_Base_Utils::Pow2(float x)
{
	return x*x;
}
//-------------------------------------------------------------------------
// 随机【0-1】之间的浮点数
float FKCW_Base_Utils::Random01()
{
	return rand()*1.0/RAND_MAX;
}
//-------------------------------------------------------------------------
// 随机一定范围内的浮点数
float FKCW_Base_Utils::RandomInRange(float min,float max)
{
	return min+Random01()*(max-min);
}
//-------------------------------------------------------------------------
// 将一个向量旋转一定角度
CCPoint FKCW_Base_Utils::RotateVector2(const CCPoint&vec,float A)
{
	CCPoint rs;
	float A_radian=A*M_PI/180;
	float cosA=cosf(A_radian);
	float sinA=sinf(A_radian);
	rs.x=vec.x*cosA-vec.y*sinA;
	rs.y=vec.x*sinA+vec.y*cosA;
	return rs;
}
//-------------------------------------------------------------------------
// 将一个向量旋转一定角度
CCPoint FKCW_Base_Utils::RotateVector2(const CCPoint&vec,float cosA,float sinA)
{
	CCPoint rs;
	rs.x=vec.x*cosA-vec.y*sinA;
	rs.y=vec.x*sinA+vec.y*cosA;
	return rs;
}
//-------------------------------------------------------------------------
// 判断两点是否相同
bool FKCW_Base_Utils::IsPointEqual(const CCPoint&point1,const CCPoint&point2,float eps)
{
	if(fabsf(point1.x-point2.x)<eps&&fabsf(point1.y-point2.y)<eps)
	{
		return true;
	}else{
		return false;
	}
}
//-------------------------------------------------------------------------
// 判断两点是否相同
bool FKCW_Base_Utils::IsPointEqual(const CCPoint&p1,const CCPoint&p2)
{
	if(p1.x==p2.x&&p1.y==p2.y)
		return true;
	else 
		return false;
}
//-------------------------------------------------------------------------
// 通过sin.cos值计算出角度值
float FKCW_Base_Utils::CalculateAngleFromCosAndSin(float cosA,float sinA)
{
	float angle=acosf(MAX(-1,MIN(1,cosA)))/M_PI*180;
	if(sinA>0)
	{
		return angle;
	}
	else
	{
		return 360-angle;
	}
}
//-------------------------------------------------------------------------
void __cross(const float v1[3],const float v2[3], float rs[3])
{
	rs[0] = v1[1]*v2[2] - v1[2]*v2[1];
	rs[1] = v1[2]*v2[0] - v1[0]*v2[2];
	rs[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
// 计算Vec1到Vec2的角度的sin,cos值，并将结果分别设置为CCPoint的x,y返回
CCPoint FKCW_Base_Utils::CalculateCosASinAOfVec1ToVec2(const CCPoint&vec1,const CCPoint&vec2)
{
	float cosA=ccpDot(vec1, vec2)/(ccpLength(vec1)*ccpLength(vec2));
	float signalOfSinA;
	{
		float _vec1[3]={vec1.x,vec1.y,0};
		float _vec2[3]={vec2.x,vec2.y,0};
		float _rs[3];
		__cross(_vec1, _vec2, _rs);
		if (_rs[2]==0) {
			signalOfSinA=0;
		}else if(_rs[2]>0){
			signalOfSinA=1;
		}else{
			signalOfSinA=-1;
		}
	}
	float sinA=signalOfSinA*sqrtf(MAX(0,1-cosA*cosA));
	return CCPoint(cosA,sinA);
}
//-------------------------------------------------------------------------
struct __node
{
	__node* foe;
	bool value;
	__node()
	{
		foe = NULL;
		value = false;
	}
};
// 计算a的N次方
float FKCW_Base_Utils::Pow(float a,int n)
{
	assert(!(a==0&&n==0));// 0的0次方无意义
	if(n==0&&a!=0)return 1;
	if(a==0&&n!=0)return 0;
	if(n==1)return a;
	if(a==1)return 1;
	__node*tail=NULL;
	while(n!=1)
	{
		__node* node = new __node();
		node->value=(n%2 == 0)?false:true;
		node->foe=tail;
		tail=node;
		n=n>>1;
	}
	float rs=a;
	while(tail)
	{
		if(tail->value)
		{
			rs=rs*rs*a;
		}
		else
		{
			rs=rs*rs;
		}
		__node*temp=tail;
		tail=tail->foe;
		delete temp;
	}
	return rs;
}
//-------------------------------------------------------------------------
// 点 到一条线的距离
// 注：线是从start到end连接延长线。点到该线的距离是其垂线距离
// 若返回值为正，则意味点在线的左方。若返回值为负，则意味着点在线的右方。
float FKCW_Base_Utils::PointDisToLine(const CCPoint&point,const CCPoint&start,const CCPoint&end)
{
	// start ----------end
	//        \ | PNDis
	//         \|
	//        point
	//
	CCPoint sp=point-start;
	CCPoint se=end-start;
	CCPoint se_e=ccpNormalize(se);
	float fDis=ccpCross(sp, se_e);
	fDis=-fDis;
	return fDis;
}
//-------------------------------------------------------------------------
// 获取两个线的交点
CCPoint FKCW_Base_Utils::GetIntersectPointOfTwoLine(const CCPoint&p1,const CCPoint&dir1,
												   const CCPoint&p2,const CCPoint&dir2)
{
	float D=-dir1.x*dir2.y+dir2.x*dir1.y;
	if(D==0)
		return CCPoint(INFINITY,INFINITY);

	float Py=(p1.x*dir1.y*dir2.y-p1.y*dir1.x*dir2.y-p2.x*dir2.y*dir1.y+p2.y*dir2.x*dir1.y)/D;
	float Px;
	if(fabsf(dir2.y)>fabsf(dir1.y))
	{
		Px=dir2.x*(Py-p2.y)/dir2.y+p2.x;
	}
	else
	{
		Px=dir1.x*(Py-p1.y)/dir1.y+p1.x;
	}
	return CCPoint(Px,Py);
}
//-------------------------------------------------------------------------
// 计算三角形内切圆半径平方
float FKCW_Base_Utils::CalculateIncircleR2(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2)
{
	float Len_v0v1=ccpDistance(v0, v1);
	float Len_v1v2=ccpDistance(v1, v2);
	float Len_v2v0=ccpDistance(v2, v0);
	float a=Len_v0v1;
	float b=Len_v1v2;
	float c=Len_v2v0;
	float temp=2*(a+b+c);
	float r2=((a+b+c)*(b+c-a)*(a+c-b)*(a+b-c))/(temp*temp);
	return MAX(0,r2);	// 这个数应该非负数
}
//-------------------------------------------------------------------------
// 返回一个缩小的三角形
// 说明：返回的三角形中v0v1和p0p1之间的距离为d, v1v2和p1p2之间的距离为d, v2v0和p2p0之间的距离为d
// 返回值应当是三个点p0, p1, p2，但也可能p0p1p2收缩到一个点，则返回值为一个元素点
vector<CCPoint>	FKCW_Base_Utils::MakeTriangleSmaller(CCPoint v0,CCPoint v1,CCPoint v2,float d)
{
	float incircleR2=CalculateIncircleR2(v0, v1, v2);
	bool shrinkToPoint=(d*d>=incircleR2);
	if(shrinkToPoint)
	{
		d=sqrtf(incircleR2);
	}
	vector<CCPoint> plist;
	CCPoint vlist[3]={v0,v1,v2};
	for(int i=0;i<3;i++)
	{
		CCPoint v=vlist[i];			
		CCPoint vn=vlist[(i+1)%3];	
		CCPoint vf=vlist[(i-1+3)%3];
		CCPoint dir_vvn=ccpNormalize(vn-v);
		CCPoint dir_vvf=ccpNormalize(vf-v);
		CCPoint dir_vp=ccpNormalize(dir_vvn+dir_vvf);
		float cos_vnvvf=ccpDot(dir_vvn, dir_vvf);
		float sin_halfvnvvf=sqrtf(MAX(0,0.5*(1-cos_vnvvf)));
		float len_vp=d/sin_halfvnvvf;
		CCPoint vp=ccpMult(dir_vp, len_vp);
		CCPoint p=v+vp;
		plist.push_back(p);
		if(shrinkToPoint)
		{
			//收缩到一个点了，则直接返回该点
			break;
		}
	}
	return plist;
}
//-------------------------------------------------------------------------
// 获取右向法线或外法线
CCPoint FKCW_Base_Utils::GetRightNormal(const CCPoint&dir)
{
	CCPoint normal(dir.y,-dir.x);
	return normal;
}
//-------------------------------------------------------------------------
// 获取右侧向量或外向量
// 注意：没有标准化。
CCPoint FKCW_Base_Utils::GetRightVector(const CCPoint&p1,const CCPoint&p2)
{
	CCPoint vec=p2-p1;
	CCPoint rightVec(vec.y,-vec.x);
	return rightVec;
}
//-------------------------------------------------------------------------
// 计算三角形面积
float FKCW_Base_Utils::CalculateTriangleArea(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2)
{
	const float x1=v0.x;
	const float y1=v0.y;
	const float x2=v1.x;
	const float y2=v1.y;
	const float x3=v2.x;
	const float y3=v2.y;
	float Area=0.5*fabsf(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2);
	return Area;
}
//-------------------------------------------------------------------------
// 计算点到指定圆的切线
// 返回值：首先返回左侧切点，第二个值返回右侧切点。
// 注意：若点在圆内，则返回空向量
vector<CCPoint>	FKCW_Base_Utils::CalculateTangentPointsOfPointToCircle(const CCPoint &center, float r,const CCPoint &point)
{
	CCPoint tangentPointLeft,tangentPointRight;
	CCPoint PC=center-point;
	float disPC=ccpLength(PC);
	if(disPC<r)
	{
		// 点在圆内
		vector<CCPoint> rs;
		return rs;
	}
	if(disPC==r)
	{
		// 点在圆上
		vector<CCPoint> rs;
		rs.push_back(point);
		rs.push_back(point);
		return rs;
	}
	if(disPC==0)
	{
		vector<CCPoint> rs;
		rs.push_back(tangentPointLeft);
		rs.push_back(tangentPointRight);
		return rs;
	}

	float sinA=r/disPC;
	float cosA=sqrtf(1-sinA*sinA);

	float disPT=disPC*cosA;

	CCPoint dirPC=CCPoint(PC.x/disPC,PC.y/disPC);

	CCPoint dirPTLeft=RotateVector2(dirPC, cosA, sinA);
	CCPoint dirPTRight=RotateVector2(dirPC, cosA, -sinA);

	tangentPointLeft=point+ccpMult(dirPTLeft, disPT);

	tangentPointRight=point+ccpMult(dirPTRight, disPT);
	vector<CCPoint> rs;
	rs.push_back(tangentPointLeft);
	rs.push_back(tangentPointRight);
	return rs;
}
//-------------------------------------------------------------------------
// 通过指定边长，生成正N边形
vector<CCPoint> FKCW_Base_Utils::MakeRegularPolygon(int r,int n)
{
	vector<CCPoint> pointList;
	float A=0;
	float dA=360.0/n;
	for(int i=0;i<n;i++)
	{
		float x=r*cosf(A*M_PI/180);
		float y=r*sinf(A*M_PI/180);
		CCPoint point(x,y);
		pointList.push_back(point);
		A+=dA;
	}
	return pointList;
}
//-------------------------------------------------------------------------
// 通过两点，生成正方形顶点数组
vector<CCPoint> FKCW_Base_Utils::MakeRectPolygon(float rx,float ry)
{
	CCPoint LU=CCPoint(-rx,ry);
	CCPoint LD=CCPoint(-rx,-ry);
	CCPoint RU=CCPoint(rx,ry);
	CCPoint RD=CCPoint(rx,-ry);
	vector<CCPoint> pointList;
	pointList.push_back(LU);
	pointList.push_back(LD);
	pointList.push_back(RD);
	pointList.push_back(RU);
	return pointList;
}
//-------------------------------------------------------------------------
// 判断一个区域是否在包围盒内
bool FKCW_Base_Utils::TestSegmentAABB( CCPoint p_pPoint0, CCPoint p_pPoint1, SAABB p_Box )
{
	CCPoint c = ccpMult(ccpAdd(p_Box.min, p_Box.max), 0.5f);
	CCPoint e = ccpSub(p_Box.max, c);
	CCPoint m = ccpMult(ccpAdd(p_pPoint0, p_pPoint1), 0.5f);
	CCPoint d = ccpSub(p_pPoint1, m);
	m = ccpSub(m, c);

	// 通过世界坐标轴进行坐标分离
	float adx = fabs(d.x);
	if (fabs(m.x) > e.x + adx)
		return false;
	float ady = fabs(d.y);
	if (fabs(m.y) > e.y + ady)
		return false;

	// 如果坐标轴和区域接近平行时，为避免有错误，做出该微调
	adx += FLT_EPSILON;
	ady += FLT_EPSILON;

	// 尝试区域的方向向量和坐标轴做交叉
	if (fabs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx)
		return false;

	// 没有发现分离轴，区域一定和AABB重叠
	return true;
}
//-------------------------------------------------------------------------
// 合并两个矩形
CCRect FKCW_Base_Utils::Combine( const CCRect& r1, const CCRect& r2 )
{
	float left = MIN(r1.origin.x, r2.origin.x);
	float right = MAX(r1.origin.x + r1.size.width, r2.origin.x + r2.size.width);
	float bottom = MIN(r1.origin.y, r2.origin.y);
	float top = MAX(r1.origin.y + r1.size.height, r2.origin.y + r2.size.height);
	return CCRectMake(left, bottom, right - left, top - bottom);
}
//-------------------------------------------------------------------------
// 对一个int数组进行二分查找
int FKCW_Base_Utils::BinarySearch( int* p_pA, unsigned int p_unLen, int p_nKey )
{
	int low = 0;
	int high = p_unLen -1;
	while ( low <= high )
	{
		int middle = low + ( (high-low)>>1 );
		if ( p_pA[middle] == p_nKey )
		{
			return middle;
		}
		else if ( p_pA[middle] > p_nKey )
		{
			high = middle - 1;
		}
		else if ( p_pA[middle] < p_nKey )
		{
			low = middle + 1;
		}
	}
	return -1;
	/*
	int indexRet = -1;  
	int mid = p_unLen / 2;  
 
	if ( p_pA[mid] == p_nKey )  
	{  
		indexRet = mid;  
	}  
	else if( p_unLen != 1 )
	{  
		if( ( p_pA[mid] < p_nKey ) && (p_unLen != 1))
		{
			//中间元素小于 key，表示元素在右边  
			indexRet = BinarySearch( p_pA + mid, p_unLen - mid, p_nKey );  
			if (indexRet != -1)  
			{  
				indexRet += mid;  
			}  
		}
		else
		{
			indexRet = BinarySearch( p_pA, mid, p_nKey );
		}
	}  

	return indexRet; 
	*/
}
//-------------------------------------------------------------------------
// 快速排序
void FKCW_Base_Utils::QuickSort(int* a, unsigned int p_unLen )
{
	int i=0;
	int j= p_unLen-1;
	int val=a[0];/*指定参考值val大小*/
	if(p_unLen>1)/*确保数组长度至少为2，否则无需排序*/
	{
		while(i<j)/*循环结束条件*/
		{
			/*从后向前搜索比val小的元素，找到后填到a[i]中并跳出循环*/
			for(;j>i;j--)
			{
				if(a[j]<val)
				{
					a[i++]=a[j];
					break;
				}
			}
			/*从前往后搜索比val大的元素，找到后填到a[j]中并跳出循环*/
			for(;i<j;i++)
			{
				if(a[i]>val)
				{
					a[j--]=a[i];
					break;
				}
			}
		}
		a[i]=val;/*将保存在val中的数放到a[i]中*/
		QuickSort(a,i);/*递归，对前i个数排序*/
		QuickSort(a+i+1,p_unLen-i-1);/*对i+2到numsize这numsize-1-i个数排序*/
	}
}
//-------------------------------------------------------------------------
// 获取节点源点（相对父节点）
CCPoint FKCW_Base_Utils::GetOrigin( CCNode* p_pNode )
{
	if( p_pNode->isIgnoreAnchorPointForPosition() ) 
	{
		return p_pNode->getPosition();
	} 
	else 
	{
		return ccp(p_pNode->getPositionX() - p_pNode->getAnchorPointInPoints().x,
			p_pNode->getPositionY() - p_pNode->getAnchorPointInPoints().y);
	}
}
//-------------------------------------------------------------------------
// 获取节点中心（相对父节点）
CCPoint FKCW_Base_Utils::GetCenter( CCNode* p_pNode )
{
	if( p_pNode->isIgnoreAnchorPointForPosition() ) 
	{
		return ccpAdd( p_pNode->getPosition(), ccp( p_pNode->getContentSize().width / 2, p_pNode->getContentSize().height / 2));
	} 
	else 
	{
		return ccpAdd(ccp( p_pNode->getPositionX() - p_pNode->getAnchorPointInPoints().x,
			p_pNode->getPositionY() - p_pNode->getAnchorPointInPoints().y),
			ccp( p_pNode->getContentSize().width / 2, p_pNode->getContentSize().height / 2 ));
	}
}
//-------------------------------------------------------------------------
// 获取节点本地中心
CCPoint FKCW_Base_Utils::GetLocalCenter( CCNode* p_pNode )
{
	return ccp( p_pNode->getContentSize().width / 2,
		p_pNode->getContentSize().height / 2 );
}
//-------------------------------------------------------------------------
// 获取节点位置（相对父节点）
CCPoint FKCW_Base_Utils::GetPoint( CCNode* p_pNode, float p_fXPercent, float p_fYPercent )
{
	CCPoint origin = GetOrigin(p_pNode);
	CCSize size = p_pNode->getContentSize();
	return ccp(origin.x + size.width * p_fXPercent, origin.y + size.height * p_fYPercent);
}
//-------------------------------------------------------------------------
// 获取节点位置（相对父节点）
// 参数：p_tagAnchor 锚点
CCPoint FKCW_Base_Utils::GetPoint( CCNode* p_pNode, CCPoint p_tagAnchor )
{
	return GetPoint( p_pNode, p_tagAnchor.x, p_tagAnchor.y );
}
//-------------------------------------------------------------------------
// 获取节点位置（相对本node节点）
CCPoint FKCW_Base_Utils::GetLocalPoint( CCNode* p_pNode, float p_fXPercent, float p_fYPercent )
{
	CCSize s = p_pNode->getContentSize();
	return ccp(s.width * p_fXPercent, s.height * p_fYPercent);
}
//-------------------------------------------------------------------------
// 获取节点位置（相对本node节点）
CCPoint FKCW_Base_Utils::GetLocalPoint( CCNode* p_pNode, CCPoint p_tagAnchor )
{
	CCSize s = p_pNode->getContentSize();
	return ccp(s.width * p_tagAnchor.x, s.height * p_tagAnchor.y);
}
//-------------------------------------------------------------------------
// 获取在世界空间中的包围盒
CCRect FKCW_Base_Utils::GetBoundingBoxInWorldSpace( CCNode* p_pNode )
{
	CCRect r;
	r.origin = CCPointZero;
	r.size = p_pNode->getContentSize();
	CCAffineTransform t = p_pNode->nodeToWorldTransform();
	r = CCRectApplyAffineTransform(r, t);
	return r;
}
//-------------------------------------------------------------------------
// 获取一个可变图片纹理的中心矩形
CCRect FKCW_Base_Utils::GetCenterRect( const string& p_strFrameName )
{
	CCSpriteFrame* f = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(p_strFrameName.c_str());
	const CCSize& size = f->getOriginalSize();
	return CCRectMake(size.width / 2, size.height / 2, 1, 1);
}
//-------------------------------------------------------------------------
// 获取一个可变图片纹理的中心矩形
CCRect FKCW_Base_Utils::GetCenterRect( CCSpriteFrame* p_pFrame )
{
	const CCSize& size = p_pFrame->getOriginalSize();
	return CCRectMake(size.width / 2, size.height / 2, 1, 1);
}
//-------------------------------------------------------------------------
// 获取节点所在场景对象
CCScene* FKCW_Base_Utils::GetScene( CCNode* p_pNode )
{
	while( p_pNode ) 
	{
		CCScene* s = dynamic_cast<CCScene*>(p_pNode);
		if(s) 
		{
			return s;
		} 
		else 
		{
			p_pNode = p_pNode->getParent();
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
// 递归设置一个节点以及其全部子节点 alpha 值
void FKCW_Base_Utils::SetOpacityRecursively( CCNode* p_pNode, int p_nOpacity )
{
	CCRGBAProtocol* p = dynamic_cast<CCRGBAProtocol*>(p_pNode);
	if(p) {
		p->setOpacity(p_nOpacity);
	}

	CCArray* children = p_pNode->getChildren();
	int cc = p_pNode->getChildrenCount();
	for(int i = 0; i < cc; i++) {
		CCNode* child = (CCNode*)children->objectAtIndex(i);        
		SetOpacityRecursively(child, p_nOpacity);
	}

	cocos2d::gui::Widget* w = dynamic_cast<cocos2d::gui::Widget*>(p_pNode);
	if(w) {
		if(w->getVirtualRenderer()) {
			CCRGBAProtocol* p = dynamic_cast<CCRGBAProtocol*>(w->getVirtualRenderer());
			if(p) {
				p->setOpacity(p_nOpacity);
			}
		}

		CCArray* children = w->getNodes();
		int cc = children->count();
		for(int i = 0; i < cc; i++) {
			CCNode* child = (CCNode*)children->objectAtIndex(i);
			SetOpacityRecursively(child, p_nOpacity);
		}
	}
}
//-------------------------------------------------------------------------
// 从1970-1-1到现在的毫秒数
int64_t FKCW_Base_Utils::GetStaticCurrentTimeSeconds()
{
	long long time_last;  
	time_last = time(NULL);
	return time_last;
}
//-------------------------------------------------------------------------
// 获取当前标准时间
string FKCW_Base_Utils::GetUTCDataString()
{
	time_t localTime;
	struct tm *utcTime_tm;

	localTime = time(NULL);
	utcTime_tm = gmtime(&localTime);

	std::string date(ItoA(1900+utcTime_tm->tm_year));
	date.append("-");
	if((utcTime_tm->tm_mon+1) < 10)
	{
		date.append("0");
		date.append(ItoA(utcTime_tm->tm_mon+1));
	}
	else
		date.append(ItoA(utcTime_tm->tm_mon+1));
	date.append("-");
	if((utcTime_tm->tm_mday) < 10)
	{
		date.append("0");
		date.append(ItoA(utcTime_tm->tm_mday));
	}
	else
		date.append(ItoA(utcTime_tm->tm_mday));
	date.append("T");
	date.append(ItoA(utcTime_tm->tm_hour));
	date.append(":");
	date.append(ItoA(utcTime_tm->tm_min));
	date.append(":");
	date.append(ItoA(utcTime_tm->tm_sec));
	date.append(".000Z");

	return date;
}
//-------------------------------------------------------------------------
// 获取时间
bool FKCW_Base_Utils::GetTimeOfDay( timeval_t* tv, void* tz )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (tv)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency( &liFreq );
		QueryPerformanceCounter( &liTime );
		tv->tv_sec     = (long)( liTime.QuadPart / liFreq.QuadPart );
		tv->tv_usec    = (long)( liTime.QuadPart * 1000000.0 / liFreq.QuadPart - tv->tv_sec * 1000000.0 );
		return true;
	}
	return false;
#else
	return ::gettimeofday((timeval*)tv, NULL/*(__timezone_ptr_t)tz*/) == 0;
#endif
}
//-------------------------------------------------------------------------
// 获取本日已过时间（精确到秒）
double FKCW_Base_Utils::GetTimeSeconds()
{
	timeval_t tv;
	if ( FKCW_Base_Utils::GetTimeOfDay(&tv, NULL) )
	{
		return tv.tv_sec + tv.tv_usec / 1000000.0f;
	}

	return .0f;
}
//-------------------------------------------------------------------------
// 获取从操作系统启动到当前已过时间（精确到毫秒）
unsigned int FKCW_Base_Utils::GetTimeTickCount()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return ::GetTickCount();
#else
	struct timeval tv;
	if(gettimeofday(&tv,NULL)!=0)
		return 0;
	return (tv.tv_sec*1000) + (tv.tv_usec/1000);
#endif
}
//-------------------------------------------------------------------------
// 屏幕截屏并保存文件
// 注1：文件支持jpg,jpeg,png，其他格式将视为jpg保存。
// 注2：文件大小一定与窗口大小一致
// 参数1：被截图的根节点，这样就可以仅仅截获屏幕上部分节点。但最终图片大小依然是窗口大小。若改值为NULL，则截屏整个屏幕。
// 参数2：保存截图的路径，应当是相对路径。在ios和android上应当是一个缓存目录。
// 参数3：如果你需要使用一个模板文件，则改值设为true
string FKCW_Base_Utils::ScreenShot( CCNode* p_pRoot, const string& p_strPath, bool p_bIsNeedStencil )
{
	string ext = GetPathExtension(p_strPath);
	bool png = ext == ".png";
	bool jpg = ext == ".jpg" || ext == ".jpeg";
	if(!png && !jpg)
		jpg = true;

	// 获取渲染根节点
	CCNode* renderNode = p_pRoot;
	CCRect bound;
	if(renderNode)
		bound = GetBoundingBoxInWorldSpace(renderNode);
	else
		renderNode = CCDirector::sharedDirector()->getRunningScene();

	// 渲染到纹理中
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCRenderTexture* screen = CCRenderTexture::create(winSize.width,
		winSize.height,
		kCCTexture2DPixelFormat_RGBA8888,
		p_bIsNeedStencil ? GL_DEPTH24_STENCIL8 : 0);
	screen->begin();
	renderNode->visit();
	screen->end();

	// 保存缓冲到文件中
	screen->saveToFile(p_strPath.c_str(), png ? kCCImageFormatPNG : kCCImageFormatJPEG);

	return CCFileUtils::sharedFileUtils()->getWritablePath() + p_strPath;
}
//-------------------------------------------------------------------------
// DNS解析
string FKCW_Base_Utils::Domain2IP( const char* p_szDomain )
{
	struct hostent* h = gethostbyname(p_szDomain);
	if( h != NULL ) 
	{
		unsigned char* p = (unsigned char *)(h->h_addr_list)[0];
		if( p != NULL )
		{
			char ip[16] = {0};
			sprintf(ip, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
			return ip;
		}
	}
	return "";
}
//-------------------------------------------------------------------------
// 创建windows控制台
void FKCW_Base_Utils::CreateWinConsole()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	AllocConsole(); 
	freopen("CONIN$", "r", stdin); 
	freopen("CONOUT$", "w", stdout); 
	freopen("CONOUT$", "w", stderr);

	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD pos = {160, 80};  
	SMALL_RECT rc = {0, 0, 160-1, 80-1};
	SetConsoleWindowInfo(hOut, true, &rc);  
	SetConsoleScreenBufferSize(hOut, pos); 
#endif
}
//-------------------------------------------------------------------------