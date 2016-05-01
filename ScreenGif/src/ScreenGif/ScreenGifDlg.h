
// ScreenGifDlg.h : 头文件
//

#pragma once
#include "GlobalData.h"
// CScreenGifDlg 对话框
#include "RegionDlg.h"
#include "AllScreenDlg.h"
#include "afxcmn.h"
class CWellcomPage;
class CScreenGifDlg : public CDialogEx
{
// 构造
public:
	CScreenGifDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CScreenGifDlg();

// 对话框数据
	enum { IDD = IDD_SCREENGIF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetarea();
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedShare();
	afx_msg void OnBnClickedGetPic();
	afx_msg void OnBnClickedSave();
	afx_msg BOOL OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg long OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHelp();


public:
	POINT m_Begin;	//选中区域的左上角坐标
	POINT m_End;	//选中区域的右下角坐标
	HHOOK m_myHook;	//鼠标钩子句柄
	HHOOK m_hkey;	// 键盘钩子句柄

public:
	// 截屏起点
	afx_msg LRESULT OnBeginPoint(WPARAM wParam, LPARAM lParam);
	// 鼠标移动时的坐标
	//afx_msg LRESULT OmMouseMove(WPARAM wParam, LPARAM lParam);
	// 截屏终点
	afx_msg LRESULT OnEndPoint(WPARAM wParam, LPARAM lParam);
	// 截图快捷键消息
	afx_msg LRESULT OnScreenPic(WPARAM wParam, LPARAM lParam);
	// 线程通知消息
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	// 线程通知结束
	afx_msg LRESULT OnThreadEnd(WPARAM wParam, LPARAM lParam);

public:
	// 截图
	bool GetPic();
	// 保存bitmap图片
	void SaveBitmapToFile(CBitmap* pBitmap, CString fileName);
	// 制作gif图片
	void MakeGif();
	// 添加工具栏
	void AddToolbar();
	static DWORD WINAPI ThreadMakeGif(LPVOID lpParam);



public:
	CGlobalData m_Data;				//数据类
	CRegionDlg* m_pRegionDlg;		//选取框类
	CAllScreenDlg* m_pAllScreenDlg; //屏幕对话框
	BOOL m_bStart;					//鼠标框选时的标志
	CDC m_srcDc;					//屏幕DC
	CDC m_dcCompatible;				//兼容DC
	CBitmap m_mapCompatible;		//兼容位图
	int m_index;				//bmp图编号
	bool m_bIsReadyScreen;			//是否可以截图（是否选择了截图区域）
	bool m_bIsReadyGif;				//是否可以录制Gif
	bool m_bFirstGif;				//第一张Gif
	CFile* m_pGifFile;				//Gif文件
	WORD m_wGifbeginSize;			//Gif文件开始到图形控制扩展块的大小
	WORD m_wGifDataSize;			//Gif文件图像数据部分的大小
	bool m_bMouseHook;				//鼠标钩子是否已经设定
	CString m_strPath;				//pic文件路径
	CString m_strCurentGif;			//当前制作的gif
	bool m_bIsPicexistInDc;			//内存中是否已经有了图片
	CBrush m_brush;					//对话框背景颜色
	CToolBar m_toolbar;				//工具栏
	CImageList m_toolbarlist;		//图标链表
	CToolBar m_toolbarchange;		//工具栏切换
	CImageList m_toolbarlistchange;	//切换图标链表
	NOTIFYICONDATA m_notify;
	CWellcomPage* m_wellcom;		//启动界面

	CToolTipCtrl m_tooltip;			//工具栏tooltip
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam);
	virtual void OnCancel();
	afx_msg void OnBnClickedCancel();
	// 进度条
	CProgressCtrl m_Progress;
};
