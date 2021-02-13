#if !defined(AFX_PROGCONTRL_H__B5A98586_E82B_4284_99E1_3AD7394FB340__INCLUDED_)
#define AFX_PROGCONTRL_H__B5A98586_E82B_4284_99E1_3AD7394FB340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgContrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgContrl dialog

class CProgContrl : public CDialog
{
// Construction
public:
	CProgContrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgContrl)
	enum { IDD = IDD_PROGCONTRL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgContrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_iWorkMode;
	int	iProgCtrlData[10];		//³Ì¿ØÊý¾Ý

protected:

	// Generated message map functions
	//{{AFX_MSG(CProgContrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnSetpara();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGCONTRL_H__B5A98586_E82B_4284_99E1_3AD7394FB340__INCLUDED_)
