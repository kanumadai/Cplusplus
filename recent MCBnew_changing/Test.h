#if !defined(AFX_TEST_H__27AE3905_8664_4BFC_BE4A_FFAAA4894A88__INCLUDED_)
#define AFX_TEST_H__27AE3905_8664_4BFC_BE4A_FFAAA4894A88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Test.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTest dialog

class CTest : public CDialog
{
// Construction
public:
	CTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTest)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__27AE3905_8664_4BFC_BE4A_FFAAA4894A88__INCLUDED_)
