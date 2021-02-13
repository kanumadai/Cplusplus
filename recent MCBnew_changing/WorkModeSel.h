#if !defined(AFX_WORKMODESEL_H__B949958E_F7A8_4B3D_B4A5_5EBBB5DC3A92__INCLUDED_)
#define AFX_WORKMODESEL_H__B949958E_F7A8_4B3D_B4A5_5EBBB5DC3A92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkModeSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkModeSel dialog

class CWorkModeSel : public CDialog
{
// Construction
public:
	CWorkModeSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWorkModeSel)
	enum { IDD = IDD_MODE_SEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkModeSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int	m_imodeSel;

protected:

	// Generated message map functions
	//{{AFX_MSG(CWorkModeSel)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioX();
	afx_msg void OnRadioY();
	afx_msg void OnRadioE();
	afx_msg void OnRadioT();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKMODESEL_H__B949958E_F7A8_4B3D_B4A5_5EBBB5DC3A92__INCLUDED_)
