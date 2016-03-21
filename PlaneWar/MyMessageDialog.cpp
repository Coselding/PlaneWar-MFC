// MyMessageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneWar.h"
#include "MyMessageDialog.h"
#include "afxdialogex.h"


// MyMessageDialog 对话框

IMPLEMENT_DYNAMIC(MyMessageDialog, CDialogEx)

MyMessageDialog::MyMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MyMessageDialog::~MyMessageDialog()
{
}

void MyMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MyMessageDialog, CDialogEx)
END_MESSAGE_MAP()


// MyMessageDialog 消息处理程序
