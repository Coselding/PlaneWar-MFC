// RestartDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneWar.h"
#include "RestartDialog.h"
#include "afxdialogex.h"


// CRestartDialog 对话框

IMPLEMENT_DYNAMIC(CRestartDialog, CDialogEx)

CRestartDialog::CRestartDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RESTART, pParent)
{

}

CRestartDialog::~CRestartDialog()
{
}

void CRestartDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRestartDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CRestartDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRestartDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CRestartDialog 消息处理程序


void CRestartDialog::OnBnClickedCancel()
{
	//显示详细信息，并进行退出
	MyDialog  aboutDlg;

	CDialogEx::OnCancel();
	aboutDlg.DoModal();
}


void CRestartDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	
}
