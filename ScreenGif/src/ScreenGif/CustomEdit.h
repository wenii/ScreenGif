#pragma once


// CCustomEdit

class CCustomEdit : public CEdit
{
	DECLARE_DYNAMIC(CCustomEdit)

public:
	CCustomEdit();
	virtual ~CCustomEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


