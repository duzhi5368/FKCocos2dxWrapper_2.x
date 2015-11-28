//*************************************************************************
//	创建日期:	2014-10-25
//	文件名称:	FKCW_Base_Utils.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#include "FKCW_Base_Types.h"
//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <UIKit/UIKit.h>
#endif
//-------------------------------------------------------------------------
enum ENUM_ConsolTextColor
{
	eConsolTextColor_Blue			= 0x09,		// 蓝字
	eConsolTextColor_Green			= 0x0A,		// 绿字
	eConsolTextColor_Ching			= 0x0B,		// 青字
	eConsolTextColor_Red			= 0x0C,		// 红字
	eConsolTextColor_Pink			= 0x0D,		// 品红字
	eConsolTextColor_Yellow			= 0x0E,		// 黄字
};
enum ENUM_ConsolBKColor
{
	eConsolBKColor_Blue				= 0x10,		// 蓝底
	eConsolBKColor_Green			= 0x20,		// 绿底
	eConsolBKColor_Ching			= 0x30,		// 青底
	eConsolBKColor_Red				= 0x40,		// 红底
	eConsolBKColor_Pink				= 0x50,		// 品红底
	eConsolBKColor_Yellow			= 0x60,		// 黄底
	eConsolBKColor_Gray				= 0x80,		// 灰底
};
//-------------------------------------------------------------------------
#define FKFULLLOG(format, ...)						FKCW_Base_Utils::FKConsoleLog("%s [L]%d -"format, __FILE__,__LINE__, ##__VA_ARGS__)
#define FKFULLCOLORLOG(color,format, ...)			FKCW_Base_Utils::FKConsoleLogColor(color, "%s [L]%d -"format, __FILE__,__LINE__,##__VA_ARGS__)
#define FKLOG(format, ...)							FKCW_Base_Utils::FKConsoleLog(format, ##__VA_ARGS__)
#define FKCOLORLOG(color,format, ...)				FKCW_Base_Utils::FKConsoleLogColor(color, format, ##__VA_ARGS__)
//-------------------------------------------------------------------------
class FKCW_Base_Utils
{
private:
	static StringList		s_TmpStringList;
	static CCArray			s_TmpArray;
public:
	// 简便的consoleLog
	static void				FKConsoleLog(const char * pszFormat, ...);
	// 颜色参数如下，可 + 组合
	/*
	FOREGROUND_BLUE			字体颜色：蓝	1
	FOREGROUND_GREEN		字体颜色：绿	2
	FOREGROUND_RED			字体颜色：红	4
	FOREGROUND_INTENSITY	前景色高亮显示	8
	BACKGROUND_BLUE			背景颜色：蓝	16
	BACKGROUND_GREEN		背景颜色：绿	32
	BACKGROUND_RED			背景颜色：红	64
	BACKGROUND_INTENSITY	背景色高亮显示	128
	*/
	static void				FKConsoleLogColor( short p_sColor, const char * pszFormat, ... );
	// 写入Log
	static void				FKFileLog( const char* p_szMsg );

	// int转换为string
	static string			ItoA(int p_nNum);
	// string转换为int
	static int				AtoI(const char* str);
	static int				AtoI(string str);
	// float转换为string
	static string			FtoA(float p_fNum,int p_nPrecision = 2);
	// string转换为float
	static double			AtoF(const char* str);
	static double			AtoF(string str);
	// HexString转换为int
	static int				Hex2I(const char* str);
	// 字符转换为16进制形式 例如A转换为"41"
	static string			Char2hex( char dec );
	// 字符串转换
	static string			Bin2Hex(const unsigned char * input, size_t length);
	// 判断一个字符串是否是数字串
	static bool				IsDigital( const string& str );
	// 判断一个字符串是否是十六进制的数字串
	static bool				IsDigitalHex( const string& str );
	// 字符串转换为int
	// 参数：nBase是指数底，例如10进制用10,16进制用16
	static bool				AtoI( const string& str, int nBase, int& nOutNum );

	// 获取一个字符串的Hash值
	static unsigned int		HashString( const char* p_szSrc );
	// 拷贝一个字符串
	// 返回值：拷贝后的字符串，注意，调用者应当负责释放该字符串内存
	static const char*		Copy( const char* p_szSrc );
	// 拷贝一个字符串的指定部分
	// 返回值：拷贝后的字符串，注意，调用者应当负责释放该字符串内存
	static const char*		Copy( const char* p_szSrc, int p_nStart, size_t p_unLen );
	// 判断一个字符是否是空格字符
	static bool				IsSpace( const char a );
	// 去掉字符串左侧空白
	static void				LeftTrim( string& str );
	// 去除字符串右侧空白
	static void				RightTrim( string& str );
	// 空格分隔字符串(去空格头尾)
	static string			Trim( const string& s );
	// 将括号进行字符串分割，取得其前部，括号中部(已去除括号本身)，括号后部
	static void				TrimWithBrackets( const string& s, string& head, string& in, string& tail );
	// 转换字符串为小写
	static void				ToLowerCase( string& s );
	// 判断一个字符串(s)首部字符串是否与制定字符串(sub)匹配
	static bool				IsStartWith( const string& s, const string& sub );
	// 判断一个字符串(s)尾部字符串是否与制定字符串(sub)匹配
	static bool				IsEndWith( const string& s, const string& sub );
	// 更换一个字符串中的所有字符为指定字符
	static void				ReplaceChar( string& s, char p_cOld, char p_cNew );
	// 更换一个字符串中的所有字符为指定字符串
	static string			Replace( string& s, const string& p_strOld, const string& p_strNew );
	// 从一个字符串中移除一个字符
	static void				RemoveChar( string& s, char c );
	// 根据一个分隔符进行字符串分割( 输出类型不同，分别为string, int, float, bool )
	// 注：返回值不需要外部释放，但注意返回值数组为公共数组，获得后请立刻拷贝，否则该部分内存将被其他函数重复使用
	static CCArray&			ComponentOfString( const string& s, const char p_cSeparator );
	static CCArray&			IntComponentOfString( const string& s, const char p_cSeparator );
	static CCArray&			FloatComponentOfString( const string& s, const char p_cSeparator );
	static CCArray&			BoolComponentOfString( const string& s, const char p_cSeparator );
	// 通过分隔符和数组生成一个字符串（上一系列函数的逆向）
	static string			JoinString( const CCArray& a, const char p_cSeparator );
	static string			JoinInt( const CCArray& a, const char p_cSeparator );
	static string			JoinFloat( const CCArray& a, const char p_cSeparator );
	static string			JoinBool( const CCArray& a, const char p_cSeparator );
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
	static CCPoint			CcpFromString( const string& s );
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
	static CCSize			CcsFromString(const string& s);
	// 将字符串转换为CCRect矩形
	/* 例：
	* {x,y,w,h} ==> ccRectMake( x, y, w,h );
	*/
	// 注：若解析失败，则返回ccRectZero
	static CCRect			CcrFromString(const string& s);
	// 将字符串转换为数组
	/* 例：
	* {a1,a2,a3,...}
	*/
	// 注1：若内部元素有单引号，或者双引号包含，则返回的是string数组。否则返回的是float数组。
	// 注2：返回值不需要外部释放，但注意返回值数组为公共数组，获得后请立刻拷贝，否则该部分内存将被其他函数重复使用
	static CCArray&			ArrayFromString( const string& s );
	// 将数组转换为字符串
	// 相当于 ArrayFromString 的逆向
	static string			ArrayToString( const CCArray& arr );

	// float转换string
	static string			FloatToString( float f );
	// int转换string
	static string			IntToString( int n );
	// 计算一个UTF-8字符占用多少字节
	static int				GetUTF8Btyes( unsigned char c );
	// 计算一个UTF-8字符串长度
	static int				GetUTF8StrLen( const char* p );
	// 计算一个UTF-8字符串内有多少个UTF-8字符
	static int				GetUTF8CharCount( const char* p_szUTF8 );
	// 获取一个UTF-8字符串子字符串
	static char*			SubUTF8String( const char* p_str, unsigned int start, unsigned int end );
	// 转Url中文为实际编码
	static string			UrlEncode( const string& s );
	// 解码HTML实体字符串
	static string			DecodeHtmlEntities( const string& p_strSrc );
	// 将一个中文字符串转换为拼音字符串
	// 注：当前仅支持 utf-8 字符串，GBK 字符串暂时不支持
	static string			GetPinyin( const string& s );

	// 获取一个字符串中最后的文件夹分隔符'\\'位置
	// 返回值：若没有文件夹分隔符，则返回-1
	static int				LastSlashIndex( string p_strPath );
	// 获取一个字符串中最后的点号'.'位置
	// 返回值：若没有点号，则返回-1
	static int				LastDotIndex( const string& p_strPath );
	// 获取一个路径字符串最后的片段（以文件夹分隔符判断'\\'）
	static string			LastPathOfComponent( const string& p_strPath );
	// 删除一个路径字符串最后的片段
	/* 例: 输入					输出
	* "/tmp/scratch.tiff"		"/tmp"
	* "/tmp/scratch"			"/tmp"
	* "/tmp/"					"/tmp"
	* "scratch"					""
	* "/" 						"/"
	*/
	static string			DeleteLastPathOfComponment( const string& p_strPath );
	// 为一个路径添加一个尾片段
	/* 例：输入					输出
	* "/tmp", "/scratch.tiff"	"/tmp/scratch.tiff"
	* "/tmp//", "/scratch"		"/tmp/scratch"
	* "/tmp", "/"				"/tmp"
	* "/", "tmp/" 				"/tmp"
	*/
	static string			AppendLastPathOfComponment( const string& p_strPath, const string& p_strComponent );
	// 删除文件后缀名
	static string			DeletePathExtension( const string& p_strPath );
	// 获取文件后缀名
	// 注：返回值带 . 点号。例：".exe"".jpg"
	static string			GetPathExtension( const string& p_strPath );
	// 获取一个路径的父路径
	static string			GetParentPath( const string& p_strPath );
	// 获取路径完整路径
	// 若该路径是绝对路径，则什么也不做
	// 若iOS中，则在路径前面添加 ~/Documents
	// 若Android中，则在路径前面添加 内部存储文件夹路径
	// windows中，则什么也不做
	static string			Externalize(const string& path);
	// 拆分文件路径字符串
	// 将一个路径strFilePathName，拆成三部分 路径， 文件名，以及文件后缀
	static bool				ParseFilePathName( const std::string& strFilePathName,
		std::string& strPathName, std::string& strFileName, std::string& strFileExtName );
	// 判断一个路径是否是绝对路径
	static bool				IsAbsolutePath( const string& strFilePathName );

	// 获取一个数字的位数，例如0,1,2就是1位，12,38就是2位，9821就是4位
	// 注：若是负数，则无视负号
	static int				GetNumDigits( int p_nNum );
	// 对浮点数进行一定精度的四舍五入 123.54 -> 123.50
	static double			Round( double x, int p_nPrecision );
	static float			Round( float f );
	// 对浮点数进行一定精度的舍弃 123.54 -> 123.0
	static double			Floor( double x, int p_nPrecision );
	// 对浮点数进行一定精度的无条件进位 123.54 -> 124.0
	static double			Ceil( double x, int p_nPrecision );
	// 线性内插
	static float			Lerp( float a, float b, float p );
	// 求平方值
	static float			Pow2(float x);
	// 随机【0-1】之间的浮点数
	static float			Random01();
	// 随机一定范围内的浮点数
	static float			RandomInRange(float min,float max);
	// 将一个向量旋转一定角度
	static CCPoint			RotateVector2(const CCPoint&vec,float A);
	static CCPoint			RotateVector2(const CCPoint&vec,float cosA,float sinA);
	// 判断两点是否相同
	static bool				IsPointEqual(const CCPoint&point1,const CCPoint&point2,float eps);
	static bool				IsPointEqual(const CCPoint&p1,const CCPoint&p2);
	// 通过sin.cos值计算出角度值
	static float			CalculateAngleFromCosAndSin(float cosA,float sinA);
	// 计算Vec1到Vec2的角度的sin,cos值，并将结果分别设置为CCPoint的y,x返回
	static CCPoint			CalculateCosASinAOfVec1ToVec2(const CCPoint&vec1,const CCPoint&vec2);
	// 计算a的N次方
	static float			Pow(float a,int n);
	// 点 到一条线的距离
	// 注：线是从start到end连接延长线。点到该线的距离是其垂线距离
	// 若返回值为正，则意味点在线的左方。若返回值为负，则意味着点在线的右方。
	static float			PointDisToLine(const CCPoint&point,const CCPoint&start,const CCPoint&end);
	// 获取两个线的交点
	// 若平行，返回 CCPoint(INFINITY,INFINITY);
	static CCPoint			GetIntersectPointOfTwoLine(const CCPoint&p1,const CCPoint&dir1,
							const CCPoint&p2,const CCPoint&dir2);
	// 计算三角形内切圆半径平方
	static float			CalculateIncircleR2(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2);
	// 返回一个缩小的三角形
	// 说明：返回的三角形中v0v1和p0p1之间的距离为d, v1v2和p1p2之间的距离为d, v2v0和p2p0之间的距离为d
	// 返回值应当是三个点p0, p1, p2，但也可能p0p1p2收缩到一个点，则返回值为一个元素点
	static vector<CCPoint>	MakeTriangleSmaller(CCPoint v0,CCPoint v1,CCPoint v2,float d);
	// 获取右向法线或外法线
	static CCPoint			GetRightNormal(const CCPoint&dir);
	// 获取右侧向量或外向量
	// 注意：没有标准化。
	static CCPoint			GetRightVector(const CCPoint&p1,const CCPoint&p2);
	// 计算三角形面积
	static float			CalculateTriangleArea(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2);
	// 计算点到指定圆的切线
	// 返回值：首先返回左侧切点，第二个值返回右侧切点。
	// 注意：若点在圆内，则返回空向量；若点在圆上，则返回的两个点位置相同。
	static vector<CCPoint>	CalculateTangentPointsOfPointToCircle(const CCPoint &center, float r,const CCPoint &point);
	// 通过指定边长，生成正N边形
	static vector<CCPoint>	MakeRegularPolygon(int r,int n);
	// 通过两点，生成正方形顶点数组
	static vector<CCPoint>	MakeRectPolygon(float rx,float ry);

	// 判断一个区域是否在包围盒内
	static bool				TestSegmentAABB( CCPoint p_pPoint0, CCPoint p_pPoint1, SAABB p_Box );
	// 合并两个矩形
	static CCRect			Combine( const CCRect& p_r1, const CCRect& p_r2 );

	// 对一个int数组进行二分查找
	static int				BinarySearch( int* p_pA, unsigned int p_unLen, int p_nKey );
	// 快速排序
	static void				QuickSort(int* p_pA, unsigned int p_unLen );


	// 获取节点源点（相对父节点）
	static CCPoint			GetOrigin( CCNode* p_pNode );
	// 获取节点中心（相对父节点）
	static CCPoint			GetCenter( CCNode* p_pNode );
	// 获取节点本地中心
	static CCPoint			GetLocalCenter( CCNode* p_pNode );
	// 获取节点位置（相对父节点）
	static CCPoint			GetPoint( CCNode* p_pNode, float p_fXPercent, float p_fYPercent );
	// 获取节点位置（相对父节点）
	// 参数：p_tagAnchor 锚点
	static CCPoint			GetPoint( CCNode* p_pNode, CCPoint p_tagAnchor );
	// 获取节点位置（相对本node节点）
	static CCPoint			GetLocalPoint( CCNode* p_pNode, float p_fXPercent, float p_fYPercent );
	// 获取节点位置（相对本node节点）
	static CCPoint			GetLocalPoint( CCNode* p_pNode, CCPoint p_tagAnchor );
	// 获取在世界空间中的包围盒
	static CCRect			GetBoundingBoxInWorldSpace( CCNode* p_pNode );
	// 获取一个可变图片纹理的中心矩形
	static CCRect			GetCenterRect( const string& p_strFrameName );
	// 获取一个可变图片纹理的中心矩形
	static CCRect			GetCenterRect( CCSpriteFrame* p_pFrame );
	// 获取节点所在场景对象
	static CCScene*			GetScene( CCNode* p_pNode );
	// 递归设置一个节点以及其全部子节点 alpha 值
	static void				SetOpacityRecursively( CCNode* p_pNode, int p_nOpacity );


	// 从1970-1-1到现在的秒数
	static int64_t			GetStaticCurrentTimeSeconds();
	// 获取当前标准时间
	static string			GetUTCDataString();
	// 获取本日已过时间（精确到毫秒）
	static bool				GetTimeOfDay( timeval_t* tv, void* tz );
	// 获取本日已过时间（精确到秒）
	static double			GetTimeSeconds();
	// 获取从操作系统启动到当前已过时间（精确到毫秒）
	static unsigned int		GetTimeTickCount();
	// 屏幕截屏并保存文件
	// 注1：文件支持jpg,jpeg,png，其他格式将视为jpg保存。
	// 注2：文件大小一定与窗口大小一致
	// 参数1：被截图的根节点，这样就可以仅仅截获屏幕上部分节点。但最终图片大小依然是窗口大小。若改值为NULL，则截屏整个屏幕。
	// 参数2：保存截图的路径（包括文件名和文件后缀），应当是相对路径。在ios和android上应当是一个缓存目录。
	// 参数3：如果你需要使用一个模板文件，则改值设为true
	static string			ScreenShot( CCNode* p_pRoot, const string& p_strPath, bool p_bIsNeedStencil = false );
	// DNS解析
	static string			Domain2IP( const char* p_szDomain );
	// 创建windows控制台
	static void				CreateWinConsole();
	// 系统Sleep
	static void				SystemSleep( unsigned int millisec );
	// 系统释放
	static void				SystemYield();

	// 文件压缩
	static int				CompressFile( const char* p_szSrcFile, const char* p_szDstFile, int nLevel = -1 );
	// 文件解压缩
	static int				DecompressFile( const char* p_szSrcFile, const char* p_szDstFile, 
		double* p_pNow = NULL, double* p_pTotal = NULL );
	// 设置文件访问权限
	static bool				Access( const char* p_szPath, int p_nMode );
	// 创建目录
	static bool				MakeDir( const char* p_szPath );
	// 构建目录路径
	static bool				BuildDir( const char* p_szPath );
	// 文件重命名
	static bool				Rename( const char* p_szFrom, const char* p_szTo );
	// 删除文件 
	static bool				Remove( const char* p_szPath );
};
//-------------------------------------------------------------------------