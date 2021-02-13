#if !defined(AFX_AUTOACQCOINTIMEDLG_H__E53E11C7_4DC3_4D33_81B4_0510EF0793B3__INCLUDED_)
#define AFX_AUTOACQCOINTIMEDLG_H__E53E11C7_4DC3_4D33_81B4_0510EF0793B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcqCoinTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqCoinTimeDlg dialog

class CAutoAcqCoinTimeDlg : public CDialog
{
// Construction
public:
	CAutoAcqCoinTimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoAcqCoinTimeDlg)
	enum { IDD = IDD_AUTOACQ_COINTIME };
	int		m_BucketBegin;
	int		m_BucketEnd;
	CString	m_strPath;
	UINT	m_dwEventCnt;
	UINT	m_dwStopTime;
	int		m_bStopByTime;
	int		m_bRodOut;
	int		m_bSeptaOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoAcqCoinTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoAcqCoinTimeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOACQCOINTIMEDLG_H__E53E11C7_4DC3_4D33_81B4_0510EF0793B3__INCLUDED_)
