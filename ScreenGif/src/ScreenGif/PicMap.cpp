// PicMap.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "PicMap.h"
#include "afxdialogex.h"
#include "Gif.h"
#include "ScreenGifDlg.h"

// CPicMap 对话框

IMPLEMENT_DYNAMIC(CPicMap, CDialogEx)

CPicMap::CPicMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPicMap::IDD, pParent)
{

}

CPicMap::~CPicMap()
{
}

void CPicMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicMap, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPicMap 消息处理程序


void CPicMap::OnPaint()
{
	CClientDC dc(this);
	CDC m_CurMemDC;
	
	if (m_CurMemDC.m_hDC == NULL)		//创建兼容DC
	{
		m_CurMemDC.CreateCompatibleDC(&dc);
	}

	PEN penstruct = ((CScreenGifDlg*)GetParent())->m_pen;
	//CBrush brush = ((CScreenGifDlg*)GetParent())->m_rcbrush;	//支持画实心矩形
	CPen pen(penstruct.m_style, penstruct.m_width, penstruct.m_color);
	CPen* pOldPen = m_CurMemDC.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* oldBrush = m_CurMemDC.SelectObject(pBrush);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, m_curRect.Width(), m_curRect.Height());	//创建兼容位图
	m_CurMemDC.SelectObject(&bmp);
	m_CurMemDC.BitBlt(0, 0, m_curRect.Width(), m_curRect.Height(), &m_SrcMemDC, 0, 0, SRCCOPY); //拷贝原图到当前内存dc

	//向当前内存dc中绘图
	list<CRect>::iterator it;
	for (it = (m_pProcess->m_vecRect).begin(); it != (m_pProcess->m_vecRect).end(); it++)
	{
		if (m_pProcess->m_iTool == 1)		//矩形
		{
			m_CurMemDC.Rectangle(&(*it));
		}
		else if (m_pProcess->m_iTool == 2)	//椭圆
		{
			m_CurMemDC.Ellipse(&(*it));
		}
	}
	//将图形画在画布上
	dc.BitBlt(0, 0, m_curRect.Width(), m_curRect.Height(), &m_CurMemDC, 0, 0, SRCCOPY);
	m_CurMemDC.DeleteDC();
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CPicMap::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
