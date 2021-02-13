#if !defined(AFX_RVSETTING_H__FB472392_3B7B_4856_B06E_5E25A907AE39__INCLUDED_)
#define AFX_RVSETTING_H__FB472392_3B7B_4856_B06E_5E25A907AE39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RVSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRVSetting dialog

class CRVSetting : public CDialog
{
// Construction
public:
	CRVSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRVSetting)
	enum { IDD = IDD_RunVelocitySetting };
	CSliderCtrl		m_FrontVelocity;
	CSliderCtrl		m_MiddleVelocity;
	CSliderCtrl		m_RearVelocity;
	CSliderCtrl		m_TurnVelocity;
	CSliderCtrl		m_SonerLimted;
	CSliderCtrl		m_IRLimted;
	int		m_intFrontVelocity;
	int		m_intMiddleVelocity;
	int		m_intRearVelocity;
	int		m_wwd;
	long    m_intTurnVelocity;
	UINT		m_intSonerLimted;
	UINT		m_intIRLimted;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRVSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRVSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RVSETTING_H__FB472392_3B7B_4856_B06E_5E25A907AE39__INCLUDED_)
