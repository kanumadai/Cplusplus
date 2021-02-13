#if !defined(AFX_PROGCTRL_H__EF7093B9_9661_43F6_8310_AE8A1237548D__INCLUDED_)
#define AFX_PROGCTRL_H__EF7093B9_9661_43F6_8310_AE8A1237548D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgCtrl dialog

class CProgCtrl : public CDialog
{
// Construction
public:
	CProgCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgCtrl)
	enum { IDD = IDD_PROGCTRL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	int	iProgCtrlData[10];		//³Ì¿ØÊý¾Ý

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgCtrl)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGCTRL_H__EF7093B9_9661_43F6_8310_AE8A1237548D__INCLUDED_)
