#ifndef _FILE_DIR_DIALOG_H_
#define _FILE_DIR_DIALOG_H_

#include "cocos2d.h"
#include "FKCWLib/FKCocos2dxWrapper.h"
#include <Shlobj.h>

static std::string Tchar2String(wchar_t* src)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
	char* chRtn =new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, src, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	return str;
}

//打开目录 
//返回绝对路径
std::string OpenDir(LPCTSTR lpszTitle)
{
	wchar_t szBuffer[MAX_PATH] = {0}; 
	BROWSEINFO bi;     
	bi.hwndOwner = NULL;     
	bi.pidlRoot = NULL;     
	bi.pszDisplayName = szBuffer;
	bi.ulFlags = 0;     
	bi.lpfn = NULL;     
	bi.lParam = 0;     
	bi.iImage = 0;
	bi.lpszTitle = lpszTitle;

	//弹出选择目录对话框  
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);     
	if(lp && SHGetPathFromIDList(lp, szBuffer))
	{
		std::string m_szSrcDir = Tchar2String(szBuffer);
		return m_szSrcDir;
	}
	return "";
}

//打开文件
//返回绝对路径
std::string OpenFile()
{
	OPENFILENAME ofn;			// 公共对话框结构。     
	wchar_t szFile[MAX_PATH];	// 保存获取文件名称的缓冲区。               
	// 初始化选择文件对话框。     
	ZeroMemory(&ofn, sizeof(OPENFILENAME));    
	ofn.lStructSize = sizeof(OPENFILENAME);    
	ofn.hwndOwner = NULL;    
	ofn.lpstrFile = szFile;    
	ofn.lpstrFile[0] = '\0';   
	ofn.nMaxFile = sizeof(szFile); 
		ofn.lpstrFilter = L"FK网络消息文件(*.xml)\0*.xml\0所有文件(*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;    
	ofn.lpstrFileTitle = NULL;    
	ofn.nMaxFileTitle = 0;    
	ofn.lpstrInitialDir = NULL;    
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// 显示打开选择文件对话框。     
	if ( GetOpenFileName(&ofn) )    
	{    
		return Tchar2String(szFile);
	}
	return "";
}

//遍历文件夹下所有文件
std::vector<std::string> dfsFolder( string folderPath )
{
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*.xml";	//todo
	long Handle = _findfirst(strfind.c_str(), &FileInfo);
	std::vector<std::string> filePaths;
	if (Handle == -1L)
	{
		CCLOG("can not match the folder path");
		return filePaths;
	}

	do
	{
		if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))
		{
			string filename = (folderPath + "\\" + FileInfo.name);
			filePaths.push_back(filename);
		}
	}while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);

	return filePaths;
}

#endif