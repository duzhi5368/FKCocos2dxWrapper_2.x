//*************************************************************************
//	创建日期:	2014-10-28
//	文件名称:	FKCW_Base_ResourceLoader.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	一个自我维持的资源加载类
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#include "FKCW_Base_Types.h"
#include "FKCW_Base_ResourceLoaderListener.h"
//-------------------------------------------------------------------------
// 一个自我维持的资源加载类
// 它在OepnGL线程中每帧进行资源加载工作，一个资源被一个Task处理，
// 逻辑开发人员不需要关心其内部加载方式。
// 注意：因为这个任务对CPU单位时间内消耗过大，建议设置空闲时间以免线程堵塞过长。
// 注意：如果你期望在这个过程中播放动画过渡，请不要使用CCAction机制，
//		因为该线程长期占用CPU，会导致动画跳帧，建议使用SetDisplayFrame进行动画播放。
// 说明：你可以指定加密/解密函数来修改资源加载方法。
// 支持的资源类型:
//		1：XML文件
//		2：单独的图片格式文件【无论是否加密】
//		3：图片集
//		4：动画集
//		5：cocosDenshion支持的音频文件
//	说明：如果有不支持的格式，可添加一个task来支持它。
class FKCW_Base_ResourceLoader : public CCObject
{
public:
	struct SLoadTask
	{
		float		m_fIdle;		// 加载完毕后闲置时间
		SLoadTask() : m_fIdle( 0.1f ){};
		virtual ~SLoadTask(){}
		virtual void Load(){}
	};
private:
	// 平台xml文件加载任务
	struct SPlatformStringLoadTask : public SLoadTask
	{
		string		m_strLanguage;			// 语言
		string		m_strPath;				// 资源路径
		bool		m_bIsMerge;				// 是否组合。 True表示如果当前字符串已经存在，新字符串将会合并进去。
											//		false表示若当前字符串已经存在，则新字符串会直接替代原字符串。

		virtual ~SPlatformStringLoadTask(){}
		virtual void Load();
	};

	// cocosDenshion支持的音乐加载任务
	struct SMusicTask : public SLoadTask
	{
		string		m_strName;				// 音乐名

		virtual		~SMusicTask(){}
		virtual void Load();
	};

	// cocosDenshion支持的音效加载任务
	struct SEffectTask : public SLoadTask
	{
		string		m_strName;				// 音效名

		virtual		~SEffectTask(){}
		virtual void Load();
	};

	// 位图字符加载任务
	struct SBMPFontLoadTask : public SLoadTask
	{
		string		m_strName;				// fnt文件名

		virtual		~SBMPFontLoadTask(){}
		virtual void Load();
	};

	// 加密的位图字符加载任务
	struct SEncryptedBMPFontLoadTask : public SLoadTask
	{
		string			m_strName;			// 加密后的fnt文件名
		DECRYPT_FUNC	m_pDeFunc;			// 解密函数

		virtual		~SEncryptedBMPFontLoadTask(){}
		virtual void Load();
	};

	// 图片加载任务
	struct SImageLoadTask : public SLoadTask
	{
		string		m_strName;				// image文件名

		virtual		~SImageLoadTask(){}
		virtual void Load();
	};

	// 加密图片加载任务
	struct SEncryptedImageLoadTask : public SLoadTask
	{
		string			m_strName;			// 加密后的image文件名
		DECRYPT_FUNC	m_pDeFunc;			// 解密函数

		virtual		~SEncryptedImageLoadTask(){}
		virtual void Load();
	};

	// 图片集加载任务( TexturePacker 也使用这个)
	struct SZwoptexLoadTask : public SLoadTask
	{
		string		m_strName;				// plist文件名

		virtual		~SZwoptexLoadTask(){}
		virtual void Load();
	};

	// 加密图片集加载任务
	struct SEncryptedZwoptexLoadTask : public SLoadTask
	{
		string			m_strPlistName;		// plist文件名（未加密对象）
		string			m_strPicName;		// 图片集名（加密对象）
		DECRYPT_FUNC	m_pDeFunc;			// 解密函数

		virtual		~SEncryptedZwoptexLoadTask(){}
		virtual void Load();
	};

	// Zwoptex动画加载
	struct SZwoptexAnimLoadTask : public SLoadTask
	{
		typedef vector<string>	StringList;
		StringList		m_vecFrames;		// 动画帧名列表
		string			m_strName;			// 动画名
		float			m_fUnitDelay;		// 帧间隔时间
		bool			m_bRestorOriginalFrame;		// 是否在动画播放完时恢复初始帧

		SZwoptexAnimLoadTask():
			m_fUnitDelay( 0 ), m_bRestorOriginalFrame( false ){}
		virtual		~SZwoptexAnimLoadTask(){};
		virtual void Load();
	};

	// Zwoptex动画加载{ 每一帧有独立的间隔时间 }
	struct SZwoptexHighAnimLoadTask : public SLoadTask
	{
		typedef vector<string>	StringList;
		StringList		m_vecFrames;		// 动画帧名列表
		typedef vector<float>	TimeList;
		TimeList		m_vecDelays;		// 帧间隔时间列表
		string			m_strName;			// 动画名
		bool			m_bRestorOriginalFrame;		// 是否在动画播放完时恢复初始帧

		SZwoptexHighAnimLoadTask():
			 m_bRestorOriginalFrame( false ){}
		virtual		~SZwoptexHighAnimLoadTask(){};
		virtual void Load();
	};

	// 骨骼动画加载
	struct SArmatureLoadTask : public SLoadTask
	{
		string			m_strConfigFilePath;	// 配置文件

		virtual		~SArmatureLoadTask(){}
		virtual void Load();
	};

	// 其他自定义格式加载
	struct SCustomTask : public SLoadTask
	{
		CCCallFunc*			m_pLoadFunc;		// 加载文件的函数

		SCustomTask() : m_pLoadFunc( NULL )
		{
		}
		virtual ~SCustomTask()
		{
			if( m_pLoadFunc )
			{
				m_pLoadFunc->release();
				m_pLoadFunc = NULL;
			}
		}
		virtual void Load();
	};


private:
	typedef vector<SLoadTask*>				VecLoadTaskPtr;
	VecLoadTaskPtr							m_vecLoadTaskList;		// 需要加载的资源任务列表
	FKCW_Base_ResourceLoaderListener*		m_pListener;			// 加载事件监听器
	float									m_fRemainingIdle;		// 剩余的延迟时间
	int										m_nNextLoad;			// 下一个加载对象
	bool									m_bIsLoading;			// 标示当前是否在加载资源
	CC_SYNTHESIZE(float, m_fDelay, Delay);							// 进行加载任务前的闲置时间
private:
	// 实际加载
	void					__DoLoad( float p_fDelta );
public:
	FKCW_Base_ResourceLoader( FKCW_Base_ResourceLoaderListener* p_pListener );
	virtual ~FKCW_Base_ResourceLoader();

	// 停止全部资源加载
	static void AbortAll();
	// 加载一个资源并返回其原始数据
	// 参数：p_strName 资源路径名 p_pSize 数据长度 p_pFunc 解压函数指针
	// 注意：调用者应当负责返回值的释放
	static unsigned char*	LoadRow( const string& p_strName, unsigned long* p_pSize, DECRYPT_FUNC p_pFunc = NULL );
	// 加载一个资源并以C字符串格式返回
	// 注意：调用者应当负责返回值的释放
	static char*			LoadCString( const string& p_strName, DECRYPT_FUNC p_pFunc = NULL );
	// 加载一个资源并以C++字符串格式返回
	// 注意：调用者应当负责返回值的释放
	static string			LoadString( const string& p_strName, DECRYPT_FUNC p_pFunc = NULL );

	// 加载一个图片资源
	static void				LoadImage( const string& p_strName, DECRYPT_FUNC p_pFunc );
	// 加载一个Zwoptex打包资源
	// 注：plist文件不允许加密，pic可以被加密。解密函数仅对pic文件有效
	static void				LoadZwoptex( const string& p_strPlistName, const string& p_strPicName, DECRYPT_FUNC p_pFunc );

	// 卸载图片
	static void				UnloadImages( const string& p_strPic );
	// 批量卸载图片，要求指定printf格式
	static void				UnloadImages( const string& p_strPicPattern, int p_nStart, int p_nEnd );
	// 卸载帧动画
	static void				UnloadSpriteFrames( const string& p_strPlistPattern, const string& p_PicPattern,
		int p_nStart, int p_nEnd );
	// 卸载骨架
	static void				UnloadArmatures( string p_strPlistPattern, string p_strPicPattern, 
		int p_nStart, int p_nEnd, string p_strConfig );
public:
	// 开始加载
	void					Run();
	// 分模块分段加载
	void					RunInBlockMode();
	// 终止加载
	// 注意：如果在分段分模块加载，则无法终止
	void					Abort();
	// 添加一个加载任务
	void					AddLoadTask( SLoadTask* p_pTask );
	// 添加一个自定义格式的加载任务
	void					AddCustomLoadTask( CCCallFunc* p_pFunc );
	// 添加一个图片字体加载任务
	// 参数：p_fIdle 加载完当前任务后，闲置多久加载下一任务
	void					AddBMPFontTask( const string& p_strFntFile, float p_fIdle = 0.0f );
	// 添加一个被加密的图片字体加载任务
	void					AddEncrypeBMPFontTask( const string& p_strFntFile, DECRYPT_FUNC p_pDeFunc, float p_fIdle = 0.0f );
	// 添加一个平台字符串strings.xml文件加载任务
	void					AddPlatformStringTask( const string& p_strLanguage, const string& p_strPath, bool p_bIsMerge = false );
	// 添加一个图片文件加载任务
	void					AddImageTask( const string& p_strName, float p_fIdle = 0.0f );
	// 添加一个加密的图片文件加载任务
	void					AddEncrypeImageTask( const string& p_strName, DECRYPT_FUNC p_pDeFunc, float p_fIdle = 0.0f );
	// 添加一个zwoptex图片加载任务
	void					AddZwoptexTask( const string& p_strName, float p_fIdle = 0.0f );
	// 添加一组被加密的zwoptex图片加载任务（指定sprintf格式）
	void					AddZwoptexTask( const string& p_strPattern, int p_nStart, int p_nEnd, float p_fIdle = 0.0f );
	// 添加一个zwoptex图片打包集加载任务
	// 注：plist文件不允许加密，pic可以被加密。解密函数仅对pic文件有效
	void					AddZwoptexTask( const string& p_strPlistName, const string& p_strPicName, DECRYPT_FUNC p_pFunc, float p_fIdle = 0.0f );
	// 添加一组zwoptex图片打包集加载任务
	// 注：plist文件不允许加密，pic可以被加密。解密函数仅对pic文件有效
	void					AddZwoptexTask( const string& p_strPlistPattern, const string& p_PicPattern,
		int p_nStart, int p_nEnd, DECRYPT_FUNC p_pFunc, float p_fIdle = 0.0f );
	// 添加一个音效加载任务
	void					AddCocosDenshionEffectTask( const string& p_strName, float p_fIdle = 0.0f );
	// 添加一个音乐加载任务
	void					AddCocosDenshionMusicTask( const string& p_strName, float p_fIdle = 0.0f );
	// 添加一个zwoptex动画
	// 注：p_nEndIndex 被包含
	// 参数：p_fUnitDalay 延迟时间
	// 参数：p_bIsRestoreOriginalFrame 是否在动画播放完时恢复初始帧
	void					AddZwoptexAnimTask( const string& p_strName, float p_fUnitDalay, const string& p_strPattern,
		int p_nStartIndex, int p_nEndIndex, bool p_bIsRestoreOriginalFrame = false, float p_fIdle = 0.0f );
	// 添加一个zwoptex动画
	// 注：该动画有两个起始/结束时间，所以动画可以有两个阶段
	void					AddZwoptexAnimTask( const string& p_strName, float p_fUnitDalay, const string& p_strPattern,
		int p_nStartIndex, int p_nEndIndex, int p_nStartIndex2, int p_nEndIndex2, bool p_bIsRestoreOriginalFrame = false, float p_fIdle = 0.0f );
	// 添加一个zwoptex动画
	// 注：该动画每帧均有独立间隔时间
	void					AddZwoptexAnimTask( const string& p_strName,  const string& p_strPattern, int p_nStartIndex, int p_nEndIndex,
		const CCArray& p_vecDelays, bool p_bIsRestoreOriginalFrame = false, float p_fIdle = 0.0f );
	// 添加一个加载骨骼动画配置文件的任务
	// 注：本任务并没有自动添加对应图片，需要用户额外创建加载对应图片的任务
	void					AddArmatureTask( string p_strConfig, float p_fIdle = 0.0f );
	// 添加一个加载骨骼动画配置文件的任务
	// 注：plist文件和config文件不允许被加密
	void					AddArmatureTask( string p_strPlist, string p_strPic, string p_strConfig, DECRYPT_FUNC p_pDeFunc = NULL, float p_fIdle = 0.0f );
	// 添加一个加载骨骼动画配置文件的任务
	void					AddArmatureTask( string p_strPlistPattern, string p_strPicPattern, int p_nStartIndex, int p_nEndIndex,
		string p_strConfig, DECRYPT_FUNC p_pDeFunc = NULL, float p_fIdle = 0.0f );
};