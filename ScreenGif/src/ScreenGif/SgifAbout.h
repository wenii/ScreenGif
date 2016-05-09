#pragma once

// CSgifAbout 对话框

class CSgifAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CSgifAbout)

public:
	CSgifAbout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSgifAbout();

// 对话框数据
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnNmClickgithub(NMHDR* pNMHDR, LRESULT* pResult);
	void OnNmClickoschina(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

};
