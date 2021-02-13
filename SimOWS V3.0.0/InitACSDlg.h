#if !defined(AFX_INITACS_H__4388A0D8_FBAB_49A1_8214_B4EF4EC53D8E__INCLUDED_)
#define AFX_INITACS_H__4388A0D8_FBAB_49A1_8214_B4EF4EC53D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitACS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInitACSDlg dialog

class CInitACSDlg : public CDialog
{
// Construction
public:
	CInitACSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInitACSDlg)
	enum { IDD = IDD_INIT_ACS };
	int		m_nCoinFmt;
	int		m_nEcgPreAmp;
	int		m_nEcgFreq;
	BOOL	m_bEnDbgBucket;
	BOOL	m_bEnEmc;
	int		m_nTimeInstant;
	int		m_nTimeMax;
	int		m_nTimeMin;
	int		m_nRodRate1;
	int		m_nRodRate2;
	int		m_nRodRate3;
	int		m_nAcqMode;
	int		m_nAcqTime;
	BOOL	m_bEnBucketSIO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitACSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitACSDlg)
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITACS_H__4388A0D8_FBAB_49A1_8214_B4EF4EC53D8E__INCLUDED_)
