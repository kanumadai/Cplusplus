#if !defined(AFX_ENERGEKEDU_H__CA3EE2D9_3F34_4F3B_801F_114D421B20DD__INCLUDED_)
#define AFX_ENERGEKEDU_H__CA3EE2D9_3F34_4F3B_801F_114D421B20DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnergeKedu.h : header file
//

#define WM_ENERGE_KEDU_GOODBYE	 WM_USER + 108
#define WM_ENERGE_KEDU_IDOK	 WM_USER + 109

/////////////////////////////////////////////////////////////////////////////
// CEnergeKedu dialog

class CEnergeKedu : public CDialog
{
// Construction
private: 
	CView *m_pView;
	
public:
	int m_iEnergeUnit;
	CEnergeKedu(CView *pView);
	BOOL Create();	

	CEnergeKedu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnergeKedu)
	enum { IDD = IDD_ENGERGE_KEDU };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnergeKedu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnergeKedu)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeComboxEgunit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENERGEKEDU_H__CA3EE2D9_3F34_4F3B_801F_114D421B20DD__INCLUDED_)
