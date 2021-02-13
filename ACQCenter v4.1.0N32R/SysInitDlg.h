#if !defined(AFX_SYSINITDLG_H__991BCBC8_6D4C_4524_BC9E_DD0F2F7A0FA6__INCLUDED_)
#define AFX_SYSINITDLG_H__991BCBC8_6D4C_4524_BC9E_DD0F2F7A0FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysInitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysInitDlg dialog

class CSysInitDlg : public CDialog
{
// Construction
public:
	CSysInitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysInitDlg)
	enum { IDD = IDD_SYSINIT_OPTION };
	int		m_bEnDbg;
	BOOL	m_bEnSIOBucket;
	BOOL	m_bEnSIOEMC;
	BOOL	m_bEnDbgBucket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysInitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD m_dwTimeOut;
	// Generated message map functions
	//{{AFX_MSG(CSysInitDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSINITDLG_H__991BCBC8_6D4C_4524_BC9E_DD0F2F7A0FA6__INCLUDED_)
