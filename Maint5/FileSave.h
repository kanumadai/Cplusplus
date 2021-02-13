#if !defined(AFX_FILESAVE1_H__2841BCC7_6B15_48BE_9111_D1C6D6D3B04B__INCLUDED_)
#define AFX_FILESAVE1_H__2841BCC7_6B15_48BE_9111_D1C6D6D3B04B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSave1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileSave dialog

class CFileSave : public CDialog
{
// Construction
public:
	CFileSave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSave)
	enum { IDD = IDD_FILESAVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL  bNewfilepackage;
	int	  iBlockSel;
	BOOL  bEnReadOperate;


protected:

	// Generated message map functions
	//{{AFX_MSG(CFileSave)
	afx_msg void OnNewFilepack();
	afx_msg void OnEditFilepack();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESAVE1_H__2841BCC7_6B15_48BE_9111_D1C6D6D3B04B__INCLUDED_)
