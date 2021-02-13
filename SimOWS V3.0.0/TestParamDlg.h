#if !defined(AFX_TESTPARAMDLG_H__900DD2FE_BE1F_4C19_A1E8_D432D10ADE6A__INCLUDED_)
#define AFX_TESTPARAMDLG_H__900DD2FE_BE1F_4C19_A1E8_D432D10ADE6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestParamDlg dialog

class CTestParamDlg : public CDialog
{
// Construction
public:
	CTestParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestParamDlg)
	enum { IDD = IDD_TESTPARAM };
	int		m_bucket;
	int		m_DatFmt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestParamDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPARAMDLG_H__900DD2FE_BE1F_4C19_A1E8_D432D10ADE6A__INCLUDED_)
