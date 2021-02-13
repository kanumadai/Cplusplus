#if !defined(AFX_RELATEMOVEDLG_H__88CE17F9_F6AF_4714_8A0C_CB07935A5A8A__INCLUDED_)
#define AFX_RELATEMOVEDLG_H__88CE17F9_F6AF_4714_8A0C_CB07935A5A8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RelateMoveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRelateMoveDlg dialog

class CRelateMoveDlg : public CDialog
{
// Construction
public:
	CRelateMoveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRelateMoveDlg)
	enum { IDD = IDD_BEDRELATE_PARAM };
	int		m_bOut;
	short	m_len;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRelateMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRelateMoveDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELATEMOVEDLG_H__88CE17F9_F6AF_4714_8A0C_CB07935A5A8A__INCLUDED_)
