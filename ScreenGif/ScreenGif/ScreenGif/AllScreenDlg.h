#pragma once


// CAllScreenDlg 对话框

class CAllScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAllScreenDlg)

public:
	CAllScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAllScreenDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALLSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 鼠标移动时的坐标
	//afx_msg LRESULT OmMouseMove(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
public:
	RECT m_rc;			//屏幕尺寸
	RECT m_rcSelect;	//框选尺寸
	POINT m_beginpoint;
	POINT m_endpoint;
	bool m_bstart;
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
