// AllScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "AllScreenDlg.h"
#include "afxdialogex.h"
#include "Mouse.h"




// CAllScreenDlg 对话框

IMPLEMENT_DYNAMIC(CAllScreenDlg, CDialogEx)

CAllScreenDlg::CAllScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAllScreenDlg::IDD, pParent)
	, m_bstart(false)
{
	m_rcSelect.left = 0;
	m_rcSelect.top = 0;
	m_rcSelect.right = 0;
	m_rcSelect.bottom = 0;
}

CAllScreenDlg::~CAllScreenDlg()
{
}

void CAllScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAllScreenDlg, CDialogEx)
	//ON_MESSAGE(UM_MOUSEMOVE, OmMouseMove)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CAllScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 100, LWA_ALPHA);
	SetWindowPos(&wndTop, m_rc.left, m_rc.top,
		m_rc.right, m_rc.bottom, SWP_SHOWWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAllScreenDlg::PostNcDestroy()
{
	// TODO:  在此添加专用代码和/或调用基类
	//delete this;
	CDialogEx::PostNcDestroy();
}


void CAllScreenDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//打印到屏幕上  
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* pOlBrush = dc.SelectObject(pBrush);
	if (m_rcSelect.right > m_rcSelect.left && m_rcSelect.bottom > m_rcSelect.top)
	{
		dc.Rectangle(&m_rcSelect);

	}
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOlBrush);
	dc.DeleteDC();
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CAllScreenDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bstart = true;
	m_rcSelect.left = 0;
	m_rcSelect.top = 0;
	m_rcSelect.right = 0;
	m_rcSelect.bottom = 0;
	HWND hpWnd = GetParent()->m_hWnd;
	m_beginpoint = point;
	::SendMessage(hpWnd, UM_BEGINPOINT, 0, (LPARAM)(&m_beginpoint));
	m_rcSelect.left = point.x;
	m_rcSelect.top = point.y;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAllScreenDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bstart = false;
	HWND hpWnd = GetParent()->m_hWnd;
	m_endpoint = point;
	::SendMessage(hpWnd, UM_ENDPOINT, 0, (LPARAM)(&m_endpoint));
	m_rcSelect.right = point.x;
	m_rcSelect.bottom = point.y;
	Invalidate();

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CAllScreenDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bstart)
	{
		m_rcSelect.right = point.x;
		m_rcSelect.bottom = point.y;
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CAllScreenDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
}
