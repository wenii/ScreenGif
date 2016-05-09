
// ScreenGifDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "ScreenGifDlg.h"
#include "afxdialogex.h"
#include "WellcomPage.h"
#include "SgifAbout.h"
#include "PicProcess.h"
#include "PicMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScreenGifDlg 对话框




CScreenGifDlg::CScreenGifDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenGifDlg::IDD, pParent)
{
	m_index = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pRegionDlg = NULL;
	//获得显示器屏幕尺寸
	m_AllScreen.left = 0;
	m_AllScreen.top = 0;
	m_AllScreen.right = GetSystemMetrics(SM_CXSCREEN);
	m_AllScreen.bottom = GetSystemMetrics(SM_CYSCREEN);
	// 创建屏幕DC
	m_srcDc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	m_bIsReadyGif = false;
	m_bFirstGif = false;
	m_pGifFile = NULL;
	m_wGifbeginSize = 0;
	m_strCurentGif = _T("");
	m_brush.CreateSolidBrush(RGB(37, 37, 38));
	//在程序目录下创建pic目录
	m_strPath = CGlobalData::GetCurrentFilePath();
	m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));
	m_strPath += _T("\\pic");
	if (!PathFileExists(m_strPath))
	{
		CreateDirectory(m_strPath, NULL);
	}
	//初始化画笔
	m_pen.m_style = PS_SOLID;
	m_pen.m_width = 2;
	m_pen.m_color = RGB(255, 0, 0);
	m_rcbrush.CreateSolidBrush(NULL_BRUSH);
}


CScreenGifDlg::~CScreenGifDlg()
{
	NOTIFYICONDATA nd;
	nd.cbSize = sizeof (NOTIFYICONDATA);
	nd.hWnd = m_hWnd;
	nd.uID = NIF_ICON;
	Shell_NotifyIcon(NIM_DELETE, &nd);
	UnregisterHotKey(m_hWnd, HOTKEYID);
	if (m_wellcom != NULL)
	{
		delete m_wellcom;
		m_wellcom = NULL;
	}
	if (m_process != NULL)
	{
		delete m_process;
	}
}
void CScreenGifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

BEGIN_MESSAGE_MAP(CScreenGifDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_GETAREA, &CScreenGifDlg::OnBnRectangle)
	ON_BN_CLICKED(ID_START, &CScreenGifDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_SHARE, &CScreenGifDlg::OnBnClickedShare)
	ON_BN_CLICKED(ID_GETPIC, &CScreenGifDlg::OnBnClickedGetPic)
	ON_BN_CLICKED(ID_SAVE, &CScreenGifDlg::OnBnClickedSave)
	ON_MESSAGE(UM_BEGINPOINT, OnBeginPoint)
	ON_MESSAGE(UM_ENDPOINT, OnEndPoint)
	ON_MESSAGE(UK_REGION, OnScreenPic)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_COMMAND(WM_USEHELP,	OnHelp)
	ON_COMMAND(WM_ABOUT, OnAbout)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CANCEL, &CScreenGifDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CScreenGifDlg 消息处理程序
HRESULT CScreenGifDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == HOTKEYID)
	{
		GetRect();
	}
	return TRUE;
}


// 获取终点坐标
LRESULT CScreenGifDlg::OnEndPoint(WPARAM wParam, LPARAM lParam)
{
	POINT* pt = (POINT*)lParam;
	m_rc.right = pt->x;
	m_rc.bottom = pt->y;
	bool isAllScreen = false;
	if (pt->x -  m_rc.left == 0 || pt->y - m_rc.top == 0)//如果区域无效取全屏
	{
		//获取桌面全屏尺寸
		m_rc = m_AllScreen;
		isAllScreen = true;
	}
	//创建，显示框选对话框
	if (m_pRegionDlg == NULL)
	{
		m_pRegionDlg = new CRegionDlg;
		m_pRegionDlg->m_rect = m_rc;
		m_pRegionDlg->Create(IDD_DIALOG_REGION, NULL);
		
		m_pRegionDlg->ShowWindow(SW_SHOW);
		Invalidate();
	}
	RECT rect;
	GetWindowRect(&rect);
	RECT rt;
	rt.left = m_rc.right - (rect.right - rect.left);
	rt.top = m_rc.bottom;
	rt.right = m_rc.right;
	rt.bottom = m_rc.bottom + (rect.bottom - rect.top);
	if (isAllScreen)
	{
		rt.top = 0;
		rt.right = m_rc.right;
		rt.bottom = rect.bottom - rect.top;
	}
	SetWindowPos(&wndTopMost, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_SHOWWINDOW);
	MoveWindow(&rt);
	ShowWindow(SW_SHOW);
	//隐藏背景对话框
	if (m_pAllScreenDlg->IsWindowVisible())
	{
		m_pAllScreenDlg->ShowWindow(SW_HIDE);
	}
	
	return TRUE;
}

// 获取起始点坐标
LRESULT CScreenGifDlg::OnBeginPoint(WPARAM wParam, LPARAM lParam)
{
	POINT* pt = (POINT*)lParam;
	m_rc.left = pt->x;
	m_rc.top = pt->y;

	return TRUE;
}

// 截图快捷键消息
LRESULT CScreenGifDlg::OnScreenPic(WPARAM wParam, LPARAM lParam)
{
	//指定区域
	GetRect();
	return TRUE;
}
// 添加工具栏
void CScreenGifDlg::AddToolbar()
{
	GetDlgItem(ID_GETAREA)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_SAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_START)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_GETPIC)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_SHARE)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_FULLSCREEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCEL)->ShowWindow(SW_HIDE);

	if (m_toolbar.CreateEx(this, CBRS_FLYBY | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED | CBRS_TOP | CBRS_TOOLTIPS))
	{
		
		static UINT BASED_CODE DockTool[] = { ID_GETAREA, ID_SAVE, ID_START, IDC_CANCEL, ID_GETPIC  , ID_SHARE};
		CBitmap bitmap;
		bitmap.LoadBitmapW(IDB_BITMAP1);
		m_toolbarlist.Create(16, 16, ILC_COLOR24, 0, 0);
		m_toolbarlist.Add(&bitmap, (CBitmap*)NULL);
	
		//设置工具栏按钮图片
		m_toolbar.GetToolBarCtrl().SetImageList(&m_toolbarlist);
		//设置工具栏按钮大小， 和按钮中位图大小
		SIZE sbutton, sImage;
		sbutton.cx = 33;
		sbutton.cy = 32;
		sImage.cx = 16;
		sImage.cy = 16;
		m_toolbar.SetSizes(sbutton, sImage);
		m_toolbar.SetButtons(DockTool, (UINT)6);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	}
	

}
BOOL CScreenGifDlg::OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	//  UNICODE消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR   szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	UINT tempid;
	UINT fid;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags&TTF_IDISHWND) || pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags&TTF_IDISHWND))
	{
		//idFrom为工具条的HWND
		nID = ::GetDlgCtrlID((HWND)nID);
		//tem = m_toolbar.CommandToIndex(pNMHDR->idFrom);
		  
		  HWND hWnd = ::GetFocus();
		  tempid = m_toolbar.GetToolBarCtrl().GetDlgCtrlID();// ::GetDlgCtrlID(hWnd);
		 
	}
	if (nID != 0)   //不为分隔符
	{
		tempid = m_toolbar.GetToolBarCtrl().GetHotItem();
		fid = m_toolbar.GetItemID(tempid);
		strTipText.LoadString(fid);
		
		strTipText = strTipText.Mid(strTipText.Find(_T("/n"), 0) + 1);
#ifndef   _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif
		*pResult = 0;
		// 使工具条提示窗口在最上面
		CRect rcChild;
		GetWindowRect(&rcChild);
	
		
		//ScreenToClient(rcChild);
		int x = 0, y = rcChild.top;
		if (tempid == 0)
		{
			x = rcChild.left - 50;
			y = rcChild.top + 40;
		}
		else if (tempid == 1)
		{
			x = rcChild.left +30;
			y = rcChild.top + 40;

		}
		else if (tempid == 2)
		{
			x = rcChild.left + 65;
			y = rcChild.top + 40;

		}
		else if (tempid == 3)
		{
			x = rcChild.left +100;
			y = rcChild.top + 40;

		}
		else if (tempid == 4)
		{
			x = rcChild.left +130;
			y = rcChild.top + 40;

		}
		else if (tempid == 5)
		{
			x = rcChild.left +165;
			y = rcChild.top + 40;

		}
		
		::MoveWindow(pNMHDR->hwndFrom, x, y, rcChild.Width(), rcChild.Height(), TRUE);
		return TRUE;
	}
	return TRUE;

}
	

BOOL CScreenGifDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	EnableToolTips(TRUE);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_wellcom = new CWellcomPage;
	m_wellcom->Create(CWellcomPage::IDD, NULL);
	m_wellcom->ShowWindow(SW_SHOW);
	m_Progress.ShowWindow(SW_HIDE);	//影藏进度条
	SetTimer(3, 200, NULL);
	
	::RegisterHotKey(m_hWnd, HOTKEYID, MOD_CONTROL|MOD_ALT|MOD_WIN, NULL);//注册全局快捷键 CTRL + WIN + ALT
	AddToolbar();	//添加工具条
	
	m_notify.cbSize = sizeof NOTIFYICONDATA;
	m_notify.hWnd = this->m_hWnd;
	m_notify.uID = IDR_MAINFRAME;
	m_notify.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	CString str = _T("SGif截图");
	CStringW strWide = CT2CW(str); // 将T字符转换成宽字符
	wcscpy_s(m_notify.szTip, strWide);
	m_notify.uCallbackMessage = WM_USER_NOTIFYICON;
	m_notify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;    
	Shell_NotifyIcon(NIM_ADD, &m_notify);	//将程序图标放在任务栏
	//画布
	m_map = new CPicMap;
	m_map->Create(CPicMap::IDD, NULL);
	m_map->ShowWindow(SW_HIDE);
	//画图背景
	m_process = new CPicProcess;
	m_process->m_rc = m_rc;
	m_map->m_pProcess = m_process; //画布保存前图层地址
	m_process->m_map = m_map; //保存画布地址
	m_process->Create(CPicProcess::IDD, NULL);	
	m_process->ShowWindow(SW_HIDE);
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenGifDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenGifDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//矩形工具
void CScreenGifDlg::OnBnRectangle()
{
	//当选择矩形工具的时候，先给截图区域覆盖窗口，后面要在该窗口上面绘图
	if (m_process != NULL && m_map != NULL)
	{
		GetPic();	//先截图
		m_process->m_bRect = true;
		m_process->m_rc = m_rc;
		m_map->m_curRect = m_rc;
		m_process->m_rect.SetRectEmpty();
		m_process->m_vecRect.clear();
		m_map->SetWindowPos(&wndTop, m_rc.left + 2, m_rc.top + 2,
			m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, SWP_SHOWWINDOW);
		m_process->SetWindowPos(&wndTopMost, m_rc.left + 2, m_rc.top + 2, 
			m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, SWP_SHOWWINDOW);

	}	
}

// 指定区域
void CScreenGifDlg::GetRect()
{
	ShowWindow(SW_HIDE);
	if (m_pRegionDlg != NULL)
	{
		m_pRegionDlg->DestroyWindow();
		m_pRegionDlg = NULL;
	}
	if (m_pAllScreenDlg != NULL)	//当点击选框按钮时，销毁之前的背景对话框，创建新的
	{
		m_pAllScreenDlg->DestroyWindow();
		m_pAllScreenDlg = NULL;
	}
	m_pAllScreenDlg = new CAllScreenDlg;		//创建新的全屏幕背景对话框
	m_pAllScreenDlg->m_rc = m_AllScreen;
	m_pAllScreenDlg->Create(IDD_DIALOG_ALLSCREEN, NULL);
	m_pAllScreenDlg->ShowWindow(SW_SHOW);
}

// TODO:  制作Gif图
void CScreenGifDlg::OnBnClickedStart()
{
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	if (m_bIsReadyGif)
	{
		//Gif录制结束
		//恢复按钮功能
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETAREA, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SHARE, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SAVE, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETPIC, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_CANCEL, true);

		m_toolbar.SetButtonInfo(m_toolbar.CommandToIndex(ID_START), ID_START, TBBS_BUTTON, 6);
		
		SetDlgItemText(ID_START, _T("录制GIF"));
		m_pRegionDlg->DestroyWindow();	//销毁选框
		m_pRegionDlg = NULL;
		m_bIsReadyGif = false;
		//关闭定时器
		KillTimer(1);
		m_Progress.SetRange(0, m_index);
		m_Progress.SetStep(1);
		m_Progress.ShowWindow(SW_SHOW);
		MakeGif();	//制作Gif图片
		ShowWindow(SW_HIDE);
		m_pGifFile->Close();
		if (IDYES == ::MessageBox(NULL, _T("Gif图片完成！\n是否另存为"), _T("ScreenGif"), MB_ICONQUESTION|MB_YESNO))
		{
			//保存功能
			CString filter;
			filter = _T("*.gif||");
			CString strTime; //获取系统时间 　　
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y%m%d%H%M%S");
			CString strDefName = _T("SGif") + strTime + _T(".gif");
			CFileDialog dlg(FALSE, NULL, LPCTSTR(strDefName), OFN_HIDEREADONLY, filter);
			if (dlg.DoModal() == IDOK)
			{
				CString str;
				str = dlg.GetPathName();
				if (str.Find(_T(".gif")) == -1) // 确保用户输入正确
				{
					str += _T(".gif");
				}
				if (!str.IsEmpty() && !m_strCurentGif.IsEmpty())
				{
					MoveFile(m_strCurentGif, str);
				}
			}
		}
		return;
	}

	//改变图标
	m_bIsReadyGif = true;
	CImageList* imagList = (m_toolbar.GetToolBarCtrl()).GetImageList();
	int toolbarCount = imagList->GetImageCount();
	CBitmap cbmp;
	BITMAP bmp;
	cbmp.LoadBitmapW(IDB_BITMAP3);
	cbmp.GetBitmap(&bmp);
	imagList->Add(&cbmp, RGB(192, 192, 192));
	(m_toolbar.GetToolBarCtrl()).SetImageList(imagList);
	imagList->Detach();
	imagList = NULL;
	m_toolbar.SetButtonInfo(m_toolbar.CommandToIndex(ID_START), ID_START, TBBS_BUTTON, 7);
	//是按钮失去功能
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETAREA, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SHARE, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SAVE, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETPIC, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_CANCEL, false);
	SetDlgItemText(ID_START, _T("停止■"));
	SetTimer(1, 50, NULL);	//设置定时器，每隔50ms截一幅图并保存
	m_index = 0;
	m_wGifbeginSize = 0;
	m_pGifFile = new CFile;
	CString strTime; //获取系统时间 　　
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y%m%d%H%M%S");
	CString strDefName = m_strPath + _T("\\SGif") + strTime + _T(".gif");
	m_strCurentGif = strDefName;
	m_pGifFile->Open(strDefName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	
}



// TODO:  分享功能
void CScreenGifDlg::OnBnClickedShare()
{
	// TODO:  分享功能
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	CString strPath = _T("http://service.weibo.com/share/share.php?appkey=583395093&title=");
	CString strWords = _T("微博分享");
	CString strContext = strPath + strWords;
	OnBnClickedCancel();
	ShellExecute(NULL, _T("open"), (LPCTSTR)strContext, _T(""), _T(""), SW_SHOW);

}

// TODO:  截图
void CScreenGifDlg::OnBnClickedGetPic()
{

	// 截图功能

	GetPic();
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	m_mapCompatible.DeleteObject();
	m_pRegionDlg->DestroyWindow();
	m_pRegionDlg = NULL;

	ShowWindow(SW_HIDE);
}

// TODO:  另存为
void CScreenGifDlg::OnBnClickedSave()
{
	GetPic();
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	ShowWindow(SW_HIDE);
	//m_bIsPicexistInDc = false;
	if (m_pRegionDlg != NULL)
	{
		m_pRegionDlg->DestroyWindow();
		m_pRegionDlg = NULL;
	}
	CString filter;
	filter = _T("*.bmp | *.BMP ||");
	CString strTime; //获取系统时间 　　
	CTime tm; 
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y%m%d%H%M%S");
	CString strDefName = _T("SGif截图") + strTime + _T(".bmp");
	CFileDialog dlg(FALSE, NULL, LPCTSTR(strDefName), OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		if (str.Find(_T(".bmp")) == -1 || str.Find(_T(".BMP")) == -1) // 确保用户输入正确
		{
			str += _T(".bmp");
		}
		CBitmap* pbmp = m_dcCompatible.GetCurrentBitmap();
		if (pbmp != NULL)
		{
			SaveBitmapToFile(pbmp, str);
		}
	}
	m_mapCompatible.DeleteObject();
		
}

bool CScreenGifDlg::GetPic()
{
	// 创建兼容DC
	if (m_dcCompatible.m_hDC == NULL)
	{
		m_dcCompatible.CreateCompatibleDC(&m_srcDc);
	}

	if (m_process->m_SrcMemDC.m_hDC == NULL)
	{
		m_process->m_SrcMemDC.CreateCompatibleDC(&m_srcDc);
	}
	
	if (m_map->m_SrcMemDC.m_hDC == NULL)
	{
		m_map->m_SrcMemDC.CreateCompatibleDC(&m_srcDc);
	}


	// 创建位图兼容DC
	if (m_mapCompatible.m_hObject == NULL)
	{
		m_mapCompatible.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);

	}
	CBitmap bmp1, bmp2;
	bmp1.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);
	bmp2.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);


	// 将兼容位图选入兼容DC中
	m_dcCompatible.SelectObject(&m_mapCompatible);
	m_process->m_SrcMemDC.SelectObject(&bmp1);
	m_map->m_SrcMemDC.SelectObject(&bmp2);
	// 将原始设备颜色表及像素数据块复制到兼容DC中
	m_dcCompatible.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	m_process->m_SrcMemDC.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	m_map->m_SrcMemDC.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	CClientDC dc(this);
	//RECT displayRect;
	// 将位图保存在剪切板中
	if (OpenClipboard())
	{
		//清空剪切板
		EmptyClipboard();
		//存入位图
		SetClipboardData(CF_BITMAP, m_dcCompatible.GetCurrentBitmap()->m_hObject);
		CloseClipboard();
	}
	return true;
	
}

void CScreenGifDlg::MakeGif()
{
	int n = m_index;
	m_index = 0;
	m_bFirstGif = true;
	while (n--)
	{
		CString strbmpName;
		CString strgifName;
		strgifName.Format(_T("\\SGif%d.gif"), m_index);
		strgifName = m_strPath + strgifName;
		m_index++;
		m_Progress.StepIt();
		UpdateData(FALSE);
		m_Progress.UpdateWindow();
		m_Progress.RedrawWindow();
		if (m_bFirstGif)
		{
			CFile gif;
			gif.Open(strgifName, CFile::modeRead | CFile::typeBinary);
			BYTE byte = 0x00;
			while (1)
			{
				gif.Read(&byte, 1);
				m_wGifbeginSize++;
				if (byte == 0x21)
				{
					m_pGifFile->Write(&byte, 1);
					GIFCONTROL gifcontrol;
					gif.Read(&gifcontrol, sizeof(GIFCONTROL));
					gifcontrol.m_wDelayTime = DELAYTIME;
					m_pGifFile->Write(&gifcontrol, sizeof(GIFCONTROL));
					while (gif.Read(&byte, 1) > 0)
					{
						m_pGifFile->Write(&byte, 1);
					}
					break;
				}
				else
				{
					m_pGifFile->Write(&byte, 1);

				}

			}
			//第一个gif文件读取结束
			gif.Close();
			::DeleteFile(strgifName);
			m_bFirstGif = false;
		}
		else   //非第一个gif文件
		{
			CGif::CombineGif(m_pGifFile, strgifName, m_wGifbeginSize);	//多幅gif图片拼接成一个
			::DeleteFile(strgifName);

		}
	}
	BYTE byte = 0x3B;
	m_pGifFile->Write(&byte, 1);
	m_Progress.ShowWindow(SW_HIDE);
}



//保存bitmap图
void CScreenGifDlg::SaveBitmapToFile(CBitmap* pBitmap, CString fileName)
{
	//计算位图文件每个像素所占字节数    
	WORD wBitCount = 0;//每个像素所占位数
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	int iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 16)
		wBitCount = 16;
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits <= 32)
		wBitCount = 32;
	//计算调色板大小    
	DWORD dwPaletteSize = 0;
	if (wBitCount <= 16)
	{
		dwPaletteSize = (1 << wBitCount) * sizeof (RGBQUAD);
	}
	//设置位图信息头结构  
	BITMAPINFOHEADER bi; //位图信息头结构   
	BITMAP Bitmap;	//位图属性结构
	GetObject(pBitmap->m_hObject, sizeof (BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof (BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	//位图像素字节大小
	DWORD dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//为位图内容分配内存  
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构   
	HANDLE hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板         
	HANDLE hPal = GetStockObject(DEFAULT_PALETTE);
	HANDLE hOldPal = NULL;
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//   获取该调色板下新的像素值    
	GetDIBits(hDC, (HBITMAP)(pBitmap->m_hObject), 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	//恢复调色板       
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}    
	BITMAPFILEHEADER bmfHdr; //位图文件头结构   
	bmfHdr.bfType = 0x4D42;     //   "BM"    
	DWORD dwDIBSize = sizeof (BITMAPFILEHEADER)
		+sizeof (BITMAPINFOHEADER)
		+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof (BITMAPFILEHEADER)
		+(DWORD)sizeof (BITMAPINFOHEADER)
		+dwPaletteSize;
	//创建位图文件
	CFile bmpfile;
	bmpfile.Open(fileName, CFile::modeCreate |CFile::modeWrite | CFile::typeBinary);
	//   写入位图文件头    
	bmpfile.Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
	//   写入位图文件其余内容    
	bmpfile.Write(lpbi,dwDIBSize);
	//清除            
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	bmpfile.Close();

}

// 每隔50ms截取一张图
void CScreenGifDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//保存成bmp文件
	if (nIDEvent == 1)
	{
		CString strbmpName;
		CString strgifName;
		strbmpName.Format(_T("\\Bitmap%d.bmp"), m_index);
		strbmpName = m_strPath + strbmpName;
		strgifName.Format(_T("\\SGif%d.gif"), m_index);
		strgifName = m_strPath + strgifName;
		m_index++;
		if (m_bIsReadyGif)	//是否正在录制
		{
			GetPic();
			CBitmap* pbmp = m_dcCompatible.GetCurrentBitmap();
			if (pbmp != NULL)
			{
				SaveBitmapToFile(pbmp, strbmpName);
			}
			m_mapCompatible.DeleteObject();
		}
		//将bmp图片转换成gif图片
		CGif::GetGifPic(strbmpName, strgifName);
		::DeleteFile(strbmpName);
	}
	if (nIDEvent == 3)
	{
		this->ShowWindow(SW_HIDE);
		KillTimer(3);
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CScreenGifDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	
		return m_brush;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



LRESULT  CScreenGifDlg::OnNotifyMsg(WPARAM wparam, LPARAM lparam)
{
	if (wparam != IDR_MAINFRAME)
		return    1;
	switch (lparam)
	{
		case  WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”     
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);//得到鼠标位置     
			CMenu menu;
		
			menu.CreatePopupMenu();//声明一个弹出式菜单     
			menu.AppendMenu(MF_STRING, WM_USEHELP, _T("帮助"));
			menu.AppendMenu(MF_STRING, WM_ABOUT, _T("关于"));
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("退出"));
			//确定弹出式菜单的位置
			SetForegroundWindow();
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
			//资源回收     
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
		case    WM_LBUTTONDBLCLK://双击左键的处理     
		{
			 NULL;//简单的显示主窗口完事儿     
		}
		break;
	}
	return 0;
}

//取消
void CScreenGifDlg::OnBnClickedCancel()
{
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	if (m_pRegionDlg != NULL)
	{
		m_pRegionDlg->ShowWindow(SW_HIDE);
	}
	ShowWindow(SW_HIDE);
	// TODO:  在此添加控件通知处理程序代码
}

void CScreenGifDlg::OnHelp()
{
	CString strHelp = _T("该软件可以实现截图和gif图片制作的功能，具体的使用方式为：启动软件后，软件停靠在任务栏中。");
	strHelp += _T("当需要截图或要录制一段gif图的时候使用快捷键 CTRL + WIN + ALT ，即可选取屏幕区域截图，");
	strHelp += _T("√按钮用来完成截图，保存按钮可以保存图片，×取消截图，△开始录制gif，再次按下结束gif录制。");

	::MessageBox(NULL, strHelp, _T("帮助"), MB_YESNO | MB_ICONINFORMATION);

}

void CScreenGifDlg::OnAbout()
{
	CSgifAbout aboutdlg;
	aboutdlg.DoModal();
}