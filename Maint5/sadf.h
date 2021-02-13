#if !defined(AFX_SADF_H__78D16209_C31E_4B0A_A1BF_8A6319B55747__INCLUDED_)
#define AFX_SADF_H__78D16209_C31E_4B0A_A1BF_8A6319B55747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sadf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Csadf dialog

class Csadf : public CDialog
{
// Construction
public:
	Csadf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Csadf)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csadf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Csadf)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SADF_H__78D16209_C31E_4B0A_A1BF_8A6319B55747__INCLUDED_)
