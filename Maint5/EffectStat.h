#if !defined(AFX_EFFECTSTAT_H__2E6B5BFF_7092_4984_AB04_49DB52828611__INCLUDED_)
#define AFX_EFFECTSTAT_H__2E6B5BFF_7092_4984_AB04_49DB52828611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEffectStat dialog

class CEffectStat : public CDialog
{
// Construction
public:
	CEffectStat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEffectStat)
	enum { IDD = IDD_EFFECT_STAT };
	CButton	m_checkPeak_AnolPara_sel;
	CButton	m_checkRelativeValue;
	CString	m_strEndFileNo;
	CString	m_strStartFileNo;
	CString	m_strInPeakPosition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectStat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_Effect_SourceFilePath;
	CString m_Effect_SourceFileName;
	CString m_Effect_DestFilePath;
	CString m_Effect_DestFileName;
	CString m_Effect_OutTextFilePath;
	CString m_Effect_OutTextFileName;
	BOOL m_bOutRelativeValue;

	
	int	m_iRefPeakPosition;
	int m_iStartFileNo;
	int m_iEndFileNo;

	BOOL m_bEncheck;
	BOOL m_bSel_Peak_Stat;

	
	
protected:	

	// Generated message map functions
	//{{AFX_MSG(CEffectStat)
	afx_msg void OnEffectResourceFile();
	afx_msg void OnEffectDestinationFile();
	afx_msg void OnEffectOuttextFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckPeak_AnolParaSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTSTAT_H__2E6B5BFF_7092_4984_AB04_49DB52828611__INCLUDED_)
