// WellcomPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "WellcomPage.h"
#include "afxdialogex.h"


// CWellcomPage 对话框

IMPLEMENT_DYNAMIC(CWellcomPage, CDialog)

CWellcomPage::CWellcomPage(CWnd* pParent /*=NULL*/)
	: CDialog(CWellcomPage::IDD, pParent)
{

}

CWellcomPage::~CWellcomPage()
{
}

void CWellcomPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWellcomPage, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWellcomPage 消息处理程序


void CWellcomPage::OnPaint()
{
	CPaintDC dc(this); // device context for paintin
	//CDialog::OnPaint();//要禁止这个调用   
	
	CRect   rect;
	GetClientRect(&rect);
	CDC   dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap   bmpBackground;
	bmpBackground.LoadBitmap(IDB_BITMAP4);   //IDB_BITMAP是你自己的图对应的ID   ，由于我刚刚加入的位图资源 
	//被我命名成了IDB_Bg，因而我这句就是bmpBackground.LoadBitmap(IDB_Bg);  

	BITMAP   bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap   *pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0,
	bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}


BOOL CWellcomPage::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowPos(&wndTopMost, 0, 0, 600, 200, SWP_SHOWWINDOW);
	// TODO:  在此添加额外的初始化
	SetTimer(2, 5000, NULL);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(CRect(x / 2 - 300, y / 2 - 100, x / 2 + 300, y / 2 + 100));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CWellcomPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 2)
	{
		
		KillTimer(2);
		DestroyWindow();
	}
	CDialog::OnTimer(nIDEvent);
}
