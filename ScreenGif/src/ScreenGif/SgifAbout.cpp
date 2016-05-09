// SgifAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "SgifAbout.h"
#include "afxdialogex.h"


// CSgifAbout 对话框

IMPLEMENT_DYNAMIC(CSgifAbout, CDialogEx)

CSgifAbout::CSgifAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSgifAbout::IDD, pParent)
{

}

CSgifAbout::~CSgifAbout()
{
}

void CSgifAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSgifAbout, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_GITHUB, OnNmClickgithub)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_OSCHINA, OnNmClickoschina)
END_MESSAGE_MAP()


BOOL CSgifAbout::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CLinkCtrl*)(GetDlgItem(IDC_SYSLINK_GITHUB)))->SetItemUrl(0, _T("https://github.com/wenii/ScreenGif.git"));
	((CLinkCtrl*)(GetDlgItem(IDC_SYSLINK_OSCHINA)))->SetItemUrl(0, _T("https://git.oschina.net/wentx/ScreenGif.git"));
	SetDlgItemText(IDC_STATIC_VERSION1, _T("版本 2.2.0.1"));
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// CSgifAbout 消息处理程序
void CSgifAbout::OnNmClickgithub(NMHDR* pNMHDR, LRESULT* pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
}
void CSgifAbout::OnNmClickoschina(NMHDR* pNMHDR, LRESULT* pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
}


