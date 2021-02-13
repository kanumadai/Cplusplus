#if !defined(AFX_LOGIN_H__B80F4131_256F_4CE9_AD39_8B61374A07D6__INCLUDED_)
#define AFX_LOGIN_H__B80F4131_256F_4CE9_AD39_8B61374A07D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogIn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogIn dialog

class CLogIn : public CDialog
{
// Construction
public:
	CLogIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogIn)
	enum { IDD = IDD_LOGIN };
	CString	m_strUser;
	CString	m_strCompany;
	CString	m_strPassWord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogIn)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__B80F4131_256F_4CE9_AD39_8B61374A07D6__INCLUDED_)
