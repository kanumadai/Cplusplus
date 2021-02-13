#if !defined(AFX_GETTHDLG_H__BE74536A_A799_4C0F_9AC6_C35BC6FB736E__INCLUDED_)
#define AFX_GETTHDLG_H__BE74536A_A799_4C0F_9AC6_C35BC6FB736E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetThDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetThDlg dialog

class CGetThDlg : public CDialog
{
// Construction
public:
	CGetThDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetThDlg)
	enum { IDD = IDD_INPUTTH };
	short	m_lth;
	short	m_uth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetThDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetThDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETTHDLG_H__BE74536A_A799_4C0F_9AC6_C35BC6FB736E__INCLUDED_)
