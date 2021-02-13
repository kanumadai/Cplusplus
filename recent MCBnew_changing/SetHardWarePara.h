#if !defined(AFX_SETHARDWAREPARA_H__A9515C90_B71D_4C6C_AF84_41355F0B3805__INCLUDED_)
#define AFX_SETHARDWAREPARA_H__A9515C90_B71D_4C6C_AF84_41355F0B3805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetHardWarePara.h : header file
//
#define WM_HARDEWAREPARA_GOODBYE	 WM_USER + 105

/////////////////////////////////////////////////////////////////////////////
// CSetHardWarePara dialog

class CSetHardWarePara : public CDialog
{
// Construction
private: 
	CView *m_pView;

public:
	CSetHardWarePara(CView *pView);
	BOOL Create();

	void OnUpdataShow();
	CSetHardWarePara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetHardWarePara)
	enum { IDD = IDD_SET_HARDWARE_PARA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetHardWarePara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetHardWarePara)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETHARDWAREPARA_H__A9515C90_B71D_4C6C_AF84_41355F0B3805__INCLUDED_)
