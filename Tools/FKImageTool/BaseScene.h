#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "../../FKCoreLib/FKPicLib/FKPicLib.h"
//-------------------------------------------------------------------------
class BaseLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
	BaseLayer();
public:
	bool	InitUI();
public:
	// 预览某一图片文件
	void onPreviewFile(CCObject* pSender);
	// 打开某单一文件
	void onOpenFile(CCObject* pSender);
	// 打开某文件夹
	void onOpenDir(CCObject* pSender);
	// 进行单文件转换
	void onConvertFile(CCObject* pSender);
	// 进行文件夹批量转换
	void onConvertDir(CCObject* pSender);
	// 更变了Zip压缩比率
	void onChangeZipRatio(CCObject* sender, CCControlEvent controlEvent);
	// 更变了Jpeg压缩比率
	void onChangeJpegRatio(CCObject* sender, CCControlEvent controlEvent);
protected:
	// 模拟windows提示信息，但不抢占焦点
	void _MessageInfo( string strTile, string strContext );
	void _MessageInfo( const char* szTile, const char* context );
	// 获取一个文件格式更变后的后缀名
	string _GetConvertedName( string strSrcFilePathName, string strExt );
private:
	// 递归文件夹进行格式转换
	bool __RecurrenceConvertDir( const char* szPath, int p_nSrcType, int p_nDstType, int& p_nFileCount, int& p_nFileError );
public:
    CREATE_FUNC(BaseLayer);
public:
	FKCW_UIWidget_WidgetWindow* m_pRootWindow;

	CCControlSlider*			m_pZipSlider;
	FKCW_UIWidget_Label*		m_pZipRatioValue;
	CCControlSlider*			m_pJpegSlider;
	FKCW_UIWidget_Label*		m_pJpegRatioValue;

	CCEditBox*					m_pSingleFileEdit;
	FKCW_UIWidget_Button*		m_pSelSingleFileBtn;
	FKCW_UIWidget_Button*		m_pConvertFileBtn;

	CCEditBox*					m_pSrcDirEdit;
	CCEditBox*					m_pDstDirEdit;
	FKCW_UIWidget_Button*		m_pConvertDirBtn;
	FKCW_UIWidget_Button*		m_pSelSrcDirBtn;
	FKCW_UIWidget_Button*		m_pSelDstDirBtn;

	CCEditBox*					m_pPreviewFileEdit;
	FKCW_UIWidget_Button*		m_pSelPreviewFileBtn;
	FKCW_UIWidget_Button*		m_pPreviewFileBtn;

	CCSprite*					m_pPreviewSprite;	// 等待被预览的图片精灵
public:
	int							m_nZipRatio;		// zip压缩比率
	int							m_nJpegRatio;		// jpeg压缩比率
	string						m_szSingleFile;		// 单文件转换时的文件
	CFKImage*					m_pImage;			// 需要被转换的文件
	string						m_szSrcDir;			// 需要被转换的源目录
	string						m_szDstDir;			// 需要被转换的目标目录
	bool						m_bIsOpeningDir;	// 是否当前文件夹被开启状态
	string						m_szPreviewFile;	// 等待预览的图片文件
};
//-------------------------------------------------------------------------
#endif // __HELLOWORLD_SCENE_H__
