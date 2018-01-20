// CTL.cpp : 实现文件
//

#include "stdafx.h"
#include "pixhack_control.h"
#include "CTL.h"
#include "afxdialogex.h"


// CTL 对话框

IMPLEMENT_DYNAMIC(CTL, CDialog)

CTL::CTL(CWnd* pParent /*=NULL*/)
	: CDialog(CTL::IDD, pParent)
{

}

CTL::~CTL()
{
}

void CTL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTL, CDialog)
END_MESSAGE_MAP()


// CTL 消息处理程序
