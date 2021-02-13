#if !defined(AFX_SFLASH_H__83BDBC36_8D8C_4FE1_BDA5_7AA928296829__INCLUDED_)
#define AFX_SFLASH_H__83BDBC36_8D8C_4FE1_BDA5_7AA928296829__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SFlash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSFlash dialog

class CSFlash : public CDialog
{
// Construction
public:
	CSFlash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSFlash)
	enum { IDD = IDD_SFLASH };
	CString	m_strUser;
	CString	m_strCompany;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSFlash)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OnEraseBkgnd(CDC* pDC);

	CBitmap m_bmSplash;
	CBrush m_brHollow;
	CSize m_sBitmap;

	// Generated message map functions
	//{{AFX_MSG(CSFlash)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFLASH_H__83BDBC36_8D8C_4FE1_BDA5_7AA928296829__INCLUDED_)
