#pragma once

// CPicProcess 对话框

//在画图是在CPicProcess面板上，图片的显示实在CPicMap上面显示

class CPicMap;
class CCustomEdit;
class CPicProcess : public CDialog
{
	DECLARE_DYNAMIC(CPicProcess)

public:
	CPicProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicProcess();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void DrawShape(CDC* dc, CPoint pointb, CPoint pointe);	//绘制图形


public:
	RECT m_rc;						//窗口大小
	CRect m_rect;					//矩形区坐标
	list<CRect> m_vecRect;			//图形链表
	bool m_bDrawStart;				//是否开始画图
	CDC m_SrcMemDC;					//原始DC
	CPicMap* m_map;					//画布
	CCustomEdit* m_pEdit;			//文字工具
	int m_iTool;					//绘图工具 1：矩形工具，2：椭圆工具，3：箭头，4：文字，5：撤销
	CPoint m_pointEnd;				//终点
	CPoint m_pointBegin;			//起点
	
};
