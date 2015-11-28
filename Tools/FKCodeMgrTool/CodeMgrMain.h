//*************************************************************************
//	创建日期:	2015-3-30
//	文件名称:	CodeMgrMain.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once
#include <iostream>
#include <vector>
//--------------------------------------------------------------------

static const CString s_szCreateProCommon[] = 
{
	"@echo off",
	"if not exist %s md %s",
	"xcopy FKTemplateProject\\*.* %s /s/h",
	"cd %s",
	"cd proj.win32",
	"rename FKTest.vcxproj %s.vcxproj",
	"cd ..",
	"echo 拷贝完成",
	"pause"
};
//--------------------------------------------------------------------
class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();       
	virtual BOOL OnIdle(LONG lCount);  
};

//--------------------------------------------------------------------
class CMyFrameWnd : public CFrameWnd
{
public:
	CMyFrameWnd();      
	void				Init();
	afx_msg void		OnPaint();  
	afx_msg void		OnAbout();   
	afx_msg BOOL		OnButtonClick(UINT nID); 
	

	static DWORD WINAPI ThreadSendProc(LPVOID lpThreadParameter);
	void				DoCMD();
	void				ShowMsgBox(char* pMsg);
	void				SetThreadDone(bool done){ m_bThreadDone = done;};
	bool				GetThrendDone(){return m_bThreadDone;};

private:
	DECLARE_MESSAGE_MAP()     
	void				__CreateMainUI();
	void				__DoCommon();
	void				__MakeCreateNewProCommon(CString&pStr);
	void				__MakeDeleteSvnCommon();
	void				__MakeDeleteFileCommon();
	void				__MakeUpdate2DXFileCommon();
	void				__MakeCutCPPFileCommon();
	void				__MakeCopyBackCPPFileCommon();

private:
	CButton m_CreateNewProBtn;
	CButton m_DeleteSvnBtn;
	CButton m_DeleteFileBtn;
	CButton m_Update2dxFileBtn;
	CButton m_CutFileBtn;
	CButton m_CopyFileBtn;
	CButton m_DoneBtn;
	CEdit	m_EditCommonWR;
	
	CFont	m_Font;
	HANDLE	m_hRead;
	HANDLE	m_hWrite;
	int		m_nButtonClickKey;
	bool	m_bThreadDone;
	CString	m_CStrCommonNow;
	CStringArray	m_CStringArray;

public:
	CEdit	m_EditCommonR;
};
//--------------------------------------------------------------------

