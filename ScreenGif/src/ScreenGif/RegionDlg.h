#pragma once


// CRegionDlg 对话框
class CRegionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegionDlg)

public:
	CRegionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
public:
	RECT m_rect;	//对话框的大小
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush m_brush;


};
