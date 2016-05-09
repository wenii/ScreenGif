// CustomEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "CustomEdit.h"


// CCustomEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

CCustomEdit::CCustomEdit()
{

}

CCustomEdit::~CCustomEdit()
{
}


BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CCustomEdit 消息处理程序




HBRUSH CCustomEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//修改背景透明
	//pDC->SetBkMode(TRANSPARENT);

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
