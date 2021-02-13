#if !defined(AFX_DLGPRINT_H__8833A321_7178_4369_8BE1_2575951E22B8__INCLUDED_)
#define AFX_DLGPRINT_H__8833A321_7178_4369_8BE1_2575951E22B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrint.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog

class CDlgPrint : public CDialog
{
// Construction
public:
    bool m_smallwin;
	bool m_centerline;
	bool m_1;
	bool m_2;
	bool m_3;
	bool m_4;
	bool m_5;

	CDlgPrint(CWnd* pParent = NULL);// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgPrint)
	enum {IDD = IDD_DLG_PRINT };
	int		m_iMaxChannelWidth;
	int		m_iMinChannelWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrint)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrint)
	afx_msg void OnButtonPre();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditMin();
	afx_msg void OnSetfocusEditMax();
	afx_msg void OnRadioAll();
	afx_msg void OnRadioPart();
	afx_msg void OnRadioSelf();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck9();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRINT_H__8833A321_7178_4369_8BE1_2575951E22B8__INCLUDED_)
