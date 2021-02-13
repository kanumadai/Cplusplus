#if !defined(AFX_SETTING_H__A7165BE7_8341_41E3_9611_45A734EF1D9D__INCLUDED_)
#define AFX_SETTING_H__A7165BE7_8341_41E3_9611_45A734EF1D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetting dialog

class CSetting : public CDialog
{
// Construction
public:
	CSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting)
	enum { IDD = IDD_SETTING };
	int		m_intvelocity2;
	long	m_Slongposition1;
	int		m_intvelocity4;
	long	m_Slongposition3;
	int		m_intvelocity6;
	long	m_Slongposition5;
	int		m_intvelocity8;
	long	m_Slongposition7;
	int		m_intvelocity1;
	int		m_intvelocity3;
	int		m_intvelocity5;
	int		m_intvelocity7;
	long	m_Slongposition2;
	long	m_Slongposition4;
	long	m_Slongposition6;
	long	m_Slongposition8;
	int		m_intHoldPos;
	float	m_floPetLimtedPosition;
	float	m_floPetCtLimtedPosition;
	float	m_floCtLimtedPosition;
	int		m_intPetHoldPosition;
	int		m_intTopHoldPosition;
	int		m_intCTHoldPosition;
	float	m_CTFisrtPosition;
	float	m_CTSecondPosition;
	float	m_PETFisrtPosition;
	float	m_PETSecondPosition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting)
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_H__A7165BE7_8341_41E3_9611_45A734EF1D9D__INCLUDED_)
