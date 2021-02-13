#if !defined(AFX_FHPARAMDLG_H__EA9531AD_3EE3_49E0_B820_44802F08A5C4__INCLUDED_)
#define AFX_FHPARAMDLG_H__EA9531AD_3EE3_49E0_B820_44802F08A5C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHParamDlg.h : header file
//
	   
typedef enum {
	expert, coin_pos, coin_time
}DLG_MODE;

/////////////////////////////////////////////////////////////////////////////
// CFHParamDlg dialog

class CFHParamDlg : public CDialog
{
// Construction
public:
	CFHParamDlg(CWnd* pParent = NULL);   // standard constructor
	DLG_MODE m_dlgMode;

// Dialog Data
	//{{AFX_DATA(CFHParamDlg)
	enum { IDD = IDD_FHPARAM };
	int		m_fov;
	int		m_ring;
	int		m_energymode;
	int		m_stopType;
	int		m_stopTime;
	int		m_stopEvent;
	int		m_bFH;
	int		m_scanmode;
	int		m_Backword_RR_Num;
	int		m_Backword_RR_Per;
	int		m_Forword_RR_Num;
	int		m_Forword_RR_Per;
	int		m_LBP;
	int		m_UBP;
	int		m_GateMode;
	int		m_RodSpeed;
	BOOL	m_bRodEnable;
	BOOL	m_bSeptaEnable;
	int		m_nCoinFmt;
	int		m_nCoinTimeBucket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFHParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GateParamEnable( BOOL bEnable = TRUE);

	// Generated message map functions
	//{{AFX_MSG(CFHParamDlg)
	afx_msg void OnChangeStopEventcnt();
	afx_msg void OnChangeStopTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnScanmode();
	afx_msg void OnScanmode1();
	afx_msg void OnScanmode2();
	afx_msg void OnScanmode3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHPARAMDLG_H__EA9531AD_3EE3_49E0_B820_44802F08A5C4__INCLUDED_)
