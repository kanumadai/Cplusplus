#if !defined(AFX_FASTOPEN32FILE_H__95156E76_6FCF_4AC5_9719_F8593F7E4DC1__INCLUDED_)
#define AFX_FASTOPEN32FILE_H__95156E76_6FCF_4AC5_9719_F8593F7E4DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FastOpen32File.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFastOpen32File dialog

class CFastOpen32File : public CDialog
{
// Construction
public:
	CFastOpen32File(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFastOpen32File)
	enum { IDD = IDD_FAST_OPEN_OPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastOpen32File)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastOpen32File)
	afx_msg void OnSelFilename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTOPEN32FILE_H__95156E76_6FCF_4AC5_9719_F8593F7E4DC1__INCLUDED_)
