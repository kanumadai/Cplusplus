#if !defined(AFX_SONORSETTING_H__B7E49DFE_B633_4B49_B694_C9C08CF87753__INCLUDED_)
#define AFX_SONORSETTING_H__B7E49DFE_B633_4B49_B694_C9C08CF87753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SonorSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSonorSetting dialog

class CSonorSetting : public CDialog
{
// Construction
public:
	CSonorSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSonorSetting)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here	
	UINT	m_SonorLimtedL1;
	UINT	m_SonorLimtedL2;
	UINT	m_SonorLimtedL3;
	UINT	m_SonorLimtedL4;
	UINT	m_SonorLimtedR1;
	UINT	m_SonorLimtedR2;
	UINT	m_SonorLimtedR3;
	UINT	m_SonorLimtedR4;
	UINT	m_IRLimtedL;
	UINT	m_IRLimtedR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSonorSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSonorSetting)
	afx_msg void OnChangeSonorlimtedl1();
	afx_msg void OnChangeSonorlimtedl2();
	afx_msg void OnChangeIrlimtedl();
	afx_msg void OnChangeIrlimtedr();
	afx_msg void OnChangeSonorlimtedl3();
	afx_msg void OnChangeSonorlimtedl4();
	afx_msg void OnChangeSonorlimtedr1();
	afx_msg void OnChangeSonorlimtedr2();
	afx_msg void OnChangeSonorlimtedr3();
	afx_msg void OnChangeSonorlimtedr4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SONORSETTING_H__B7E49DFE_B633_4B49_B694_C9C08CF87753__INCLUDED_)
