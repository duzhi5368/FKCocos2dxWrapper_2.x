
//--------------------------------------------------------------------
#include "Stdafx.h"
#include "CodeMgrMain.h"
#include "Resource.h"
#include "FKDialog.h"


static char s_cmdline[2048];

CMyWinApp theApp;   

//--------------------------------------------------------------------
BOOL CMyWinApp::InitInstance()
{
	m_pMainWnd = new CMyFrameWnd();
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
//--------------------------------------------------------------------
BOOL CMyWinApp::OnIdle(LONG lCount)
{
	CMyFrameWnd* pWnd = (CMyFrameWnd*)m_pMainWnd;

	if (pWnd && pWnd->GetThrendDone())
	{
		pWnd->DoCMD();
	}
	return TRUE;
}
//--------------------------------------------------------------------
CMyFrameWnd::CMyFrameWnd()
{
	Create(NULL, "FKCodeMgrTool", WS_OVERLAPPEDWINDOW, rectDefault,NULL, "MainMenu");   
	Init();
}
//-------------------------------------------------------------------------
void CMyFrameWnd::Init()
{
	m_nButtonClickKey = 0;
	m_bThreadDone = false;
	__CreateMainUI();
}
//--------------------------------------------------------------------
void CMyFrameWnd::__CreateMainUI()
{
	m_Font.CreateFont(18, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet ANSI_CHARSET
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("微软雅黑")); // lpszFac   

	
	CRect rect_pro(40,10,170,60);    //描述控件位置以及大小的结构
	m_CreateNewProBtn.Create("创建测试工程", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_CREATE_BTN);
	m_CreateNewProBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(40,80,170,130);  //描述控件位置以及大小的结构
	m_DeleteSvnBtn.Create("删除SVN文件", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_DELSVN_BTN);
	m_DeleteSvnBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(40,150,170,200);    //描述控件位置以及大小的结构
	m_DeleteFileBtn.Create("删除无效文件", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_DELFILE_BTN);
	m_DeleteFileBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(40,220,170,270);    //描述控件位置以及大小的结构
	m_Update2dxFileBtn.Create("更新Cocos2d文件", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_UPDATE_BTN);
	m_Update2dxFileBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(40,290,170,340);   //描述控件位置以及大小的结构
	m_CutFileBtn.Create("剪切源代码", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_CUTFILE_BTN);
	m_CutFileBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(40,360,170,410);    //描述控件位置以及大小的结构
	m_CopyFileBtn.Create("还原源代码", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_COPYFILE_BTN);
	m_CopyFileBtn.ShowWindow(SW_SHOWNORMAL);

	rect_pro = CRect(220,250,350,290);    //描述控件位置以及大小的结构
	m_DoneBtn.Create("执行上述命令", WS_CHILD | WS_VISIBLE | WS_BORDER, rect_pro, this, IDB_DONE_BTN);
	m_DoneBtn.ShowWindow(SW_SHOWNORMAL);

	CRect rect;
	GetClientRect(rect);

	m_EditCommonWR.Create(
		ES_WANTRETURN | ES_MULTILINE | WS_CHILD | WS_VISIBLE |WS_VSCROLL/*|ES_AUTOVSCROLL*/, 
		CRect (220,30, rect.Width()  - 10, 230), this, EDIT_COMMON_WR); 
	m_EditCommonWR.ShowWindow(SW_SHOWNORMAL);
	//m_EditCommonWR.ReplaceSel("echo 命令输入框");

	m_EditCommonR.Create(
		ES_WANTRETURN | ES_MULTILINE | WS_CHILD | WS_VISIBLE |WS_VSCROLL/*|ES_AUTOVSCROLL*/|ES_READONLY, 
		CRect (220,300, rect.Width()  - 10, rect.Height() - 10), this, EDIT_COMMON_R); 
	m_EditCommonR.ShowWindow(SW_SHOWNORMAL);
	
	m_hRead = NULL;

}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND_EX(IDB_CREATE_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_DELSVN_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_DELFILE_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_UPDATE_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_CUTFILE_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_COPYFILE_BTN, OnButtonClick)
	ON_COMMAND_EX(IDB_DONE_BTN,OnButtonClick)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
void CMyFrameWnd::OnPaint()
{
	CPaintDC dc(this);  
	CRect rect;

	GetClientRect(rect);

	dc.SetTextAlign(TA_BOTTOM | TA_CENTER);

	m_EditCommonWR.SetFont(&m_Font);
	m_EditCommonR.SetFont(&m_Font);
	m_CreateNewProBtn.SetFont(&m_Font);
	m_DeleteSvnBtn.SetFont(&m_Font);
	m_DeleteFileBtn.SetFont(&m_Font);
	m_Update2dxFileBtn.SetFont(&m_Font);
	m_CutFileBtn.SetFont(&m_Font);
	m_CopyFileBtn.SetFont(&m_Font);
	m_DoneBtn.SetFont(&m_Font);
}
//--------------------------------------------------------------------
void CMyFrameWnd::OnAbout()
{
	CDialog about("AboutBox", this);  
	about.DoModal();
}
//-------------------------------------------------------------------------
BOOL CMyFrameWnd::OnButtonClick(UINT nID)
{
	m_nButtonClickKey = nID;
	//m_EditCommonR.Clear();
	CFKDialog about; 
	int nMessageBoxID = IDCANCEL;

	if (m_nButtonClickKey != IDB_DONE_BTN)
	{
		SetDlgItemText(EDIT_COMMON_WR,"");
		UpdateWindow();
	}

	switch (m_nButtonClickKey)
	{
	case IDB_CREATE_BTN:
		{
			about.SetWindowTextString("请输入创建的项目名");
			nMessageBoxID = about.DoModal();
			if (nMessageBoxID == IDOK)
			{
				CString str = about.GetValue();
				__MakeCreateNewProCommon(str);

			}
		}
		break;

	case IDB_DELSVN_BTN:
		__MakeDeleteSvnCommon();
		break;

	case IDB_DELFILE_BTN:
		__MakeDeleteFileCommon();
		break;

	case IDB_UPDATE_BTN:
		__MakeUpdate2DXFileCommon();
		break;

	case IDB_CUTFILE_BTN:
		__MakeCutCPPFileCommon();
		break;

	case IDB_COPYFILE_BTN:
		__MakeCopyBackCPPFileCommon();
		break;

	case IDB_DONE_BTN:
		{
			nMessageBoxID = MessageBox("确定执行当前选定动作?点击[确定]立即执行!!","最后的提示框",IDOK);
			if (nMessageBoxID == IDOK)
			{
				__DoCommon();
			}
		}
		
		break;

	default:
		
		break;
	}
	
	
	return TRUE;
}
//-------------------------------------------------------------------------
void CMyFrameWnd::__DoCommon()
{
	CString stredit2;  
	GetDlgItemText(EDIT_COMMON_WR,stredit2);  
	if (stredit2.GetLength() == 0)
	{
		MessageBox("尚无可执行命令,请点击左侧功能按钮");
		return;
	}

	int nLineCount = m_EditCommonWR.GetLineCount();
	
	CString strTemp;
	for (int nLine = 0; nLine < nLineCount; ++nLine)
	{
		int nlen = m_EditCommonWR.LineLength(m_EditCommonWR.LineIndex(nLine)) ;
		m_EditCommonWR.GetLine(nLine,strTemp.GetBuffer(nlen), nlen) ; 
		strTemp.ReleaseBuffer(nlen);
		m_CStringArray.Add(strTemp);
	}

	if (m_CStringArray.GetCount() == 0)
	{
		return;
	}

	DoCMD();
}
//-------------------------------------------------------------------------
void CMyFrameWnd::DoCMD()
{
	SetThreadDone(false);

	if (m_CStringArray.GetCount() == 0)
	{
		return;
	}

	if (m_hRead != NULL)
	{
		CloseHandle(m_hRead);
		m_hRead = NULL;
	}

	m_CStrCommonNow  = m_CStringArray.GetAt(0);
	m_CStringArray.RemoveAt(0);

	if (m_CStrCommonNow.GetLength() == 0)
	{
		return;
	}

	if (m_CStrCommonNow.Find("ShellExecute",0) != -1)
	{
		CString strToolName = "XCopy.exe";
		m_CStrCommonNow.Replace("ShellExecute","");

		m_EditCommonR.SetSel(-1);
		m_EditCommonR.ReplaceSel("部分命令使用ShellExecute执行:");
		m_EditCommonR.ReplaceSel(m_CStrCommonNow);
		m_EditCommonR.ReplaceSel("\r\n");
		m_EditCommonR.ReplaceSel("若文件过大,可能需要一定时间,请耐心等待....\r\n");

		SHELLEXECUTEINFO sei; 
		memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
		sei.cbSize = sizeof(SHELLEXECUTEINFO); 
		sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
		sei.lpVerb = _T("open"); 
		sei.lpFile = _T(strToolName); 
		sei.lpParameters = m_CStrCommonNow;
		sei.nShow = SW_HIDE;//; SW_SHOWNORMAL
		ShellExecuteEx(&sei); 
		WaitForSingleObject(sei.hProcess, INFINITE); 
		CloseHandle(sei.hProcess); 
		SetThreadDone(true);
	}
	else if (m_CStrCommonNow.Find("System",0) != -1)
	{
		m_CStrCommonNow.Replace("System","");

		m_EditCommonR.SetSel(-1);
		m_EditCommonR.ReplaceSel("部分命令使用System执行:");
		m_EditCommonR.ReplaceSel(m_CStrCommonNow);
		m_EditCommonR.ReplaceSel("\r\n");
		m_EditCommonR.ReplaceSel("若文件过大,可能需要一定时间,请耐心等待....\r\n");
		system(m_CStrCommonNow);
		//WinExec(m_CStrCommonNow, SW_HIDE);
		SetThreadDone(true);
	}
	else
	{
		
		HANDLE thread2 = CreateThread(NULL,0,ThreadSendProc,this,0,NULL);
		CloseHandle(thread2);
	}

	
	

}
//-------------------------------------------------------------------------
void CMyFrameWnd::ShowMsgBox(char* pMsg)
{
	MessageBox(pMsg);  
}
//-------------------------------------------------------------------------
DWORD WINAPI CMyFrameWnd::ThreadSendProc(LPVOID lpThreadParameter)
{
	CMyFrameWnd *pThis = (CMyFrameWnd *) lpThreadParameter;

	if (pThis == NULL)
	{
		pThis->SetThreadDone(true);
		return true;
	}

	// 新建进程
	{
		SECURITY_ATTRIBUTES sa; 
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
		sa.lpSecurityDescriptor = NULL;  
		sa.bInheritHandle = TRUE;  

		if(!CreatePipe(&pThis->m_hRead,&pThis->m_hWrite,&sa,0))  
		{  
			pThis->ShowMsgBox("CreatePipe Failed");  
			return true;  
		}  

		STARTUPINFO si;  
		PROCESS_INFORMATION pi;  

		ZeroMemory(&si,sizeof(STARTUPINFO));  
		si.cb = sizeof(STARTUPINFO);  
		GetStartupInfo(&si);  
		si.hStdError = pThis->m_hWrite;  
		si.hStdOutput = pThis->m_hWrite;  
		si.wShowWindow = SW_HIDE;  
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;  

		CString tmp;  
		tmp.Format(_T("Cmd.exe /C %s"),pThis->m_CStrCommonNow);  
		char cmdline[2000];  
		memset(cmdline,0,2000);
		sprintf(cmdline,"%s",tmp);  

		pThis->m_EditCommonR.SetSel(-1);
		pThis->m_EditCommonR.ReplaceSel("正在执行..." + pThis->m_CStrCommonNow);
		pThis->m_EditCommonR.ReplaceSel("\r\n");

		// DEBUG_PROCESS CREATE_NEW_CONSOLE
		if(!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))  
		{  
			pThis->ShowMsgBox("CreateProcess failed!");  
			return true;  
		}  
		WaitForSingleObject(pi.hProcess,INFINITE);
		CloseHandle(pThis->m_hWrite); 

	}

	if (pThis->m_hRead == NULL)
	{
		pThis->SetThreadDone(true);
		return true;
	}

	// 读取输出数据
	{
		char buffer[4096] = {0};  
		CString strOutput;  
		DWORD bytesRead;  

		while(TRUE)  

		{  
			Sleep(1000); 
			memset(buffer,0,4096);
			if(NULL == ReadFile(pThis->m_hRead,buffer,4095,&bytesRead,NULL))  
			{ 
				break;  
			}  
			pThis->m_EditCommonR.SetSel(-1);
			pThis->m_EditCommonR.ReplaceSel(buffer);
			pThis->m_EditCommonR.ReplaceSel("\r\n");
			
		}  
	}

	pThis->UpdateWindow();  

	pThis->SetThreadDone(true);

	return true;
}
//-------------------------------------------------------------------------
void CMyFrameWnd::__MakeCreateNewProCommon(CString& pStr)
{
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);

#pragma warning ( disable: 4996 )

	char szBuff[256];

	memset(szBuff,0,256);
	sprintf(szBuff,_T("if not exist %s md %s \r\n"),pStr,pStr);
	m_EditCommonWR.ReplaceSel(szBuff);

	memset(szBuff,0,256);
	// 可尝试参数 /i /e /y
	sprintf(szBuff,_T("ShellExecuteFKTemplateProject\\*.* \"%s\" /s /h /y \r\n"),pStr);
	m_EditCommonWR.ReplaceSel(szBuff); 

	sprintf(szBuff,"rename %s\\proj.win32\\FKTest.vcxproj %s.vcxproj \r\n",pStr,pStr,pStr);
	m_EditCommonWR.ReplaceSel(szBuff);
	m_EditCommonWR.ReplaceSel("echo 拷贝完成\r\n");
	m_EditCommonWR.ReplaceSel("pause");

#pragma warning ( default : 4996 )
}
//-------------------------------------------------------------------------
void CMyFrameWnd::__MakeDeleteSvnCommon()
{
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);
	m_EditCommonWR.ReplaceSel("rd /q /s .svn\r\n");
	m_EditCommonWR.ReplaceSel("echo 清理SVN文件完成\r\n");

}
void CMyFrameWnd::__MakeDeleteFileCommon()
{
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);
	m_EditCommonWR.ReplaceSel(
		"del /F /S /Q mt.dep *.manifest *.tlog *.lastbuildstate BuildLog.htm\r\n");
	m_EditCommonWR.ReplaceSel(
		"del /F /S /Q *.exp *.cbp *.mdp *.ilk *.sbr *.pch *.pdb *.ncb *.idb *.tmp *.opt\r\n");
	m_EditCommonWR.ReplaceSel(
		"del /F /S /Q *.aps *.plg *.bsc *.log *.cache *.sdf *.hpj *.clw *.user\r\n");
	m_EditCommonWR.ReplaceSel(
		"del /F /S /Q /A:H l *.suo *.tlog Thumbs.db\r\n");
	m_EditCommonWR.ReplaceSel(
		"echo 删除完成\r\n");
}
//-------------------------------------------------------------------------
void CMyFrameWnd::__MakeUpdate2DXFileCommon()
{
	
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);

	m_EditCommonWR.ReplaceSel(
		"copy FKCoreLib\\FKPicLib\\cocos2dxChange\\CCImage.h ..\\cocos2dx\\platform\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKCoreLib\\FKPicLib\\cocos2dxChange\\CCImageCommon_cpp.h ..\\cocos2dx\\platform\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCApplication.h ..\\cocos2dx\\platform\\win32\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCApplicationProtocol.h ..\\cocos2dx\\platform\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCDirector.cpp ..\\cocos2dx\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCNode.h ..\\cocos2dx\\base_nodes\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCNode.cpp ..\\cocos2dx\\base_nodes\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCObject.h ..\\cocos2dx\\cocoa\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCTouchDispatcher.h ..\\cocos2dxtouch_dispatcher\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCTouchDispatcher.cpp ..\\cocos2dxtouch_dispatcher\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCTextureCache.h ..\\cocos2dx\\textures\r\n");
	m_EditCommonWR.ReplaceSel(
		"copy FKEngine\\cocos2dxChange\\CCTextureCache.cpp ..\\cocos2dx\\textures\r\n");
	m_EditCommonWR.ReplaceSel(
		"echo 更新完成，请重新编译libcocos2d库\r\n");
}
//-------------------------------------------------------------------------
void CMyFrameWnd::__MakeCutCPPFileCommon()
{
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);
	m_EditCommonWR.ReplaceSel("echo 所有CPP文件将剪切到文件夹[CutSource]\r\n");
	m_EditCommonWR.ReplaceSel("if not exist CutSource md CutSource\r\n");
	m_EditCommonWR.ReplaceSel("if not exist CutSource\\FKCoreLib md CutSource\\FKCoreLib\r\n");
	m_EditCommonWR.ReplaceSel("if not exist CutSource\\FKCWLib md CutSource\\FKCWLib\r\n");
	m_EditCommonWR.ReplaceSel("ShellExecute/s/y/i/f \"FKCoreLib\\*.cpp\" \"CutSource\\FKCoreLib\"\r\n");
	m_EditCommonWR.ReplaceSel("echo 等待时间+1000ms\r\n");
	m_EditCommonWR.ReplaceSel("ShellExecute/s/y/i/f \"FKCWLib\\*.cpp\" \"CutSource\\FKCWLib\"\r\n");
	m_EditCommonWR.ReplaceSel("echo 等待时间+1000ms\r\n");
	m_EditCommonWR.ReplaceSel("Systemfor /r FKCoreLib %a in (*.cpp) do (del %a)\r\n");
	m_EditCommonWR.ReplaceSel("echo 等待时间+1000ms\r\n");
	m_EditCommonWR.ReplaceSel("Systemfor /r FKCWLib %a in (*.cpp) do (del %a)\r\n");
	m_EditCommonWR.ReplaceSel("echo 等待时间+1000ms\r\n");
	m_EditCommonWR.ReplaceSel("echo 操作完毕 请检查文件夹[CutSource]\r\n");

}
//-------------------------------------------------------------------------
void CMyFrameWnd::__MakeCopyBackCPPFileCommon()
{
	m_EditCommonWR.Clear();
	m_EditCommonWR.SetSel(-1);
	m_EditCommonWR.ReplaceSel("echo - 复制 CutSource 文件夹下的所有.cpp文件\r\n");
	m_EditCommonWR.ReplaceSel("echo - 到指定的当前目录下的 FKCoreLib FKCWLib 中\r\n");
	m_EditCommonWR.ReplaceSel("ShellExecute/s/y/i/f \"CutSource\\*.cpp\" .\r\n");
	m_EditCommonWR.ReplaceSel("echo 操作完毕 请检查当前文件夹\r\n");
}
//--------------------------------------------------------------------
