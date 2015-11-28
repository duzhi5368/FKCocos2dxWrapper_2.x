//*************************************************************************
//	创建日期:	2015-3-30
//	文件名称:	FKDialog.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "resource.h"


// CFKDialog 对话框

class CFKDialog : public CDialog
{
	DECLARE_DYNAMIC(CFKDialog)
	CString m_value;
	CString m_WindowText;
public:
	CFKDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFKDialog();
	CString GetValue() const {return m_value;}
	void SetValue(const CString& value) {m_value = value;}
	CString GetWindowTextString() const {return m_WindowText;}
	void SetWindowTextString(const CString& value) {m_WindowText = value;}

	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow( );
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
