#if !defined(AFX_COLORSET_H__722FBA36_0F77_40E5_BA5D_3445EE958AC2__INCLUDED_)
#define AFX_COLORSET_H__722FBA36_0F77_40E5_BA5D_3445EE958AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorSet dialog

class CColorSet : public CDialog
{
// Construction
public:
	COLORREF m_Full_Color;
	COLORREF m_Exp_Color;
	COLORREF m_Interest_Color;
	COLORREF m_SmallWin_Color;
	
	CColorSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorSet)
	enum { IDD = IDD_COLORSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFullColor();
	afx_msg void OnExpColor();
	afx_msg void OnInterestColor();
	afx_msg void OnPaint();
	afx_msg void OnSmallWindowColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSET_H__722FBA36_0F77_40E5_BA5D_3445EE958AC2__INCLUDED_)
