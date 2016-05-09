// PicProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "PicProcess.h"
#include "afxdialogex.h"
#include "Gif.h"
#include "ScreenGifDlg.h"
#include "PicMap.h"
// CPicProcess 对话框

IMPLEMENT_DYNAMIC(CPicProcess, CDialog)

CPicProcess::CPicProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CPicProcess::IDD, pParent)
	, m_bDrawStart(false)
	, m_bRect(false)
{

}

CPicProcess::~CPicProcess()
{
}

void CPicProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicProcess, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPicProcess 消息处理程序


BOOL CPicProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置窗口背景透明
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 1, LWA_ALPHA);
	SetWindowPos(&wndTop, m_rc.left+2, m_rc.top+2, m_rc.right-m_rc.left-4, m_rc.bottom-m_rc.top-4, SWP_SHOWWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CPicProcess::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_rect.SetRectEmpty();
	m_rect.left = point.x;
	m_rect.top = point.y;
	m_bDrawStart = true;
	CDialog::OnLButtonDown(nFlags, point);
}


void CPicProcess::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_rect.right = point.x;
	m_rect.bottom = point.y;
	m_bDrawStart = false;
	m_vecRect.push_back(m_rect);
	m_map->OnPaint();
	CDialog::OnLButtonUp(nFlags, point);
}


void CPicProcess::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bDrawStart)
	{
		
		m_rect.right = point.x;
		m_rect.bottom = point.y;
		InvalidateRect(m_rect);

	}
	CDialog::OnMouseMove(nFlags, point);
}


void CPicProcess::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	//画矩形
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	PEN penstruct = ((CScreenGifDlg*)GetParent())->m_pen;
	//CBrush brush = ((CScreenGifDlg*)GetParent())->m_rcbrush;	//支持画实心矩形
	CPen pen(penstruct.m_style, penstruct.m_width, penstruct.m_color);
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* oldBrush = dc.SelectObject(pBrush);
	CRect rc = m_rect;
	ClientToScreen(&rc);
	if ((rc.left != rc.right) && (rc.top != rc.bottom) && (m_rect.right != 0) && (m_rect.bottom != 0))
	{
		dc.Rectangle(&rc);
	}
}


BOOL CPicProcess::OnEraseBkgnd(CDC* pDC)
{
	
	return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CPicProcess::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
