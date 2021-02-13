#if !defined(AFX_DLGPRINT1_H__75270A80_96A7_4D9D_9348_1B767F198627__INCLUDED_)
#define AFX_DLGPRINT1_H__75270A80_96A7_4D9D_9348_1B767F198627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGPRINT1.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// DLGPRINT dialog

class DLGPRINT : public CDialog
{
// Construction
public:
	bool m_1;
	bool m_2;
	bool m_3;
	bool m_4;
	bool m_5;
	int m_range;
	int m_selfvalue;
	DLGPRINT(CWnd* pParent = NULL);   // standard constructor
    int m_printmode;
	int m_rangemode;
// Dialog Data
	//{{AFX_DATA(DLGPRINT)
	enum { IDD = IDD_DIG_PR };
	int		m_min;
	int		m_max;
	CString	m_auto;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLGPRINT)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLGPRINT)
	afx_msg void OnAreaAll();
	afx_msg void OnAreaSelf();
	afx_msg void OnChangeMIN();
	afx_msg void OnChangeMAX();
	afx_msg void OnChangeAuto();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusMin();
	afx_msg void OnMax();
	afx_msg void OnRadioSelfselect();
	afx_msg void OnRadioAutoselect();
	afx_msg void OnSetfocusEdit3();
	afx_msg void OnSelchangeCombo();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnAreaInterest();
	afx_msg void OnCheckFile();
	afx_msg void OnCheckRealTime();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void RangeInit();
	
	float CaculateRange();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRINT1_H__75270A80_96A7_4D9D_9348_1B767F198627__INCLUDED_)
