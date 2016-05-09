// RegionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "RegionDlg.h"
#include "afxdialogex.h"


// CRegionDlg 对话框

IMPLEMENT_DYNAMIC(CRegionDlg, CDialogEx)

CRegionDlg::CRegionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegionDlg::IDD, pParent)
{
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
}

CRegionDlg::~CRegionDlg()
{
}

void CRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRegionDlg 消息处理程序


BOOL CRegionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CRgn rgn, tmprgn;
	rgn.CreateRectRgn(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
	tmprgn.CreateRectRgn(2, 2, m_rect.right - m_rect.left - 2, m_rect.bottom - m_rect.top - 2);
	rgn.CombineRgn(&rgn, &tmprgn, RGN_XOR);
	SetWindowRgn(rgn, TRUE);
	SetWindowPos(&wndTopMost, m_rect.left, m_rect.top,m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, SWP_SHOWWINDOW);


	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CRegionDlg::PostNcDestroy()
{
	// TODO:  在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


HBRUSH CRegionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	return m_brush;
}
