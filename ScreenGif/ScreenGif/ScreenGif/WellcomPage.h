#pragma once


// CWellcomPage 对话框

class CWellcomPage : public CDialog
{
	DECLARE_DYNAMIC(CWellcomPage)

public:
	CWellcomPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWellcomPage();

// 对话框数据
	enum { IDD = IDD_DIALOG_WELLCOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
