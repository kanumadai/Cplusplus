#if !defined(AFX_ENVIRMENTLIMIT_H__4DDA319F_B5DB_47D9_BA83_B56799A9FF41__INCLUDED_)
#define AFX_ENVIRMENTLIMIT_H__4DDA319F_B5DB_47D9_BA83_B56799A9FF41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvirmentLimit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentLimit dialog

class CEnvirmentLimit : public CDialog
{
// Construction
public:
	CEnvirmentLimit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnvirmentLimit)
	enum { IDD = IDD_ENVIRMENT_LIMIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvirmentLimit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvirmentLimit)
	afx_msg void OnOperate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVIRMENTLIMIT_H__4DDA319F_B5DB_47D9_BA83_B56799A9FF41__INCLUDED_)
