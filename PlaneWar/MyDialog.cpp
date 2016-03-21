// MyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneWar.h"
#include "MyDialog.h"
#include "afxdialogex.h"


// MyDialog 对话框

IMPLEMENT_DYNAMIC(MyDialog, CDialogEx)

MyDialog::MyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

MyDialog::~MyDialog()
{
}

void MyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MyDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &MyDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// MyDialog 消息处理程序

//按确认键退出
void MyDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(8);
	CDialogEx::OnOK();
}
