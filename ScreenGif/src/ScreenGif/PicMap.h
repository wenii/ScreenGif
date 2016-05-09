#pragma once


// CPicMap 对话框
#include "PicProcess.h"
class CPicMap : public CDialogEx
{
	DECLARE_DYNAMIC(CPicMap)

public:
	CPicMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicMap();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	list<CRect> m_vecRect;		//保存绘图数据
	CRect m_curRect;			//当前一次绘图
	CDC m_SrcMemDC;				//截图区域内存dc
	CPicProcess* m_pProcess;	//前面板指针
	
};
