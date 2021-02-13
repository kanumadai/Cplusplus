#if !defined(AFX_AUTOACQBUCKETDLG_H__08FE6D0E_9B69_4FB7_9555_4889CFE3AC9A__INCLUDED_)
#define AFX_AUTOACQBUCKETDLG_H__08FE6D0E_9B69_4FB7_9555_4889CFE3AC9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoAcqBucketDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqBucketDlg dialog

class CAutoAcqBucketDlg : public CDialog
{
// Construction
public:
	CAutoAcqBucketDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoAcqBucketDlg)
	enum { IDD = IDD_AUTOACQ_BUCKET };
	int		m_nBucketBegin;
	int		m_nBucketEnd;
	int		m_nDatFmt;
	int		m_bRodOut;
	CString	m_strPath;
	int		m_bSeptaOut;
	int		m_nStopEvent;
	int		m_nStopTime;
	int		m_nStopType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoAcqBucketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoAcqBucketDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOACQBUCKETDLG_H__08FE6D0E_9B69_4FB7_9555_4889CFE3AC9A__INCLUDED_)
