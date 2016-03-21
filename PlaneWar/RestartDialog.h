#pragma once
#include"MyDialog.h"

// CRestartDialog 对话框

class CRestartDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRestartDialog)

public:
	CRestartDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRestartDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RESTART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
