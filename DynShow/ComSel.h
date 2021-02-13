#if !defined(AFX_COMSEL_H__D1769418_DA63_444E_9D81_0D29FB8205CC__INCLUDED_)
#define AFX_COMSEL_H__D1769418_DA63_444E_9D81_0D29FB8205CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSel dialog

class CComSel : public CDialog
{
// Construction
public:
	CComSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComSel)
	enum { IDD = IDD_SELCOM };
	int		m_com;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSEL_H__D1769418_DA63_444E_9D81_0D29FB8205CC__INCLUDED_)
