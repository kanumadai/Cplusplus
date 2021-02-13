#if !defined(AFX_READFLASHSEL_H__9F5B4BA5_A363_4AE0_B257_6BCCC9F62D93__INCLUDED_)
#define AFX_READFLASHSEL_H__9F5B4BA5_A363_4AE0_B257_6BCCC9F62D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadFlashSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadFlashSel dialog

class CReadFlashSel : public CDialog
{
// Construction
public:
	UINT iFlashSel;
	CReadFlashSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReadFlashSel)
	enum { IDD = IDD_READ_FLASH_SEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadFlashSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReadFlashSel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READFLASHSEL_H__9F5B4BA5_A363_4AE0_B257_6BCCC9F62D93__INCLUDED_)
