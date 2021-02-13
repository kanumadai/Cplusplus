#if !defined(AFX_COMSEL_H__1B209403_67DC_4DF4_A3B5_37186509788B__INCLUDED_)
#define AFX_COMSEL_H__1B209403_67DC_4DF4_A3B5_37186509788B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSel dialog

class CComSel : public CDialog
{
// Construction
public:
	CComSel(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bComOpen;

// Dialog Data
	//{{AFX_DATA(CComSel)
	enum { IDD = IDD_COM_SEL };
//	CButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSel)
	virtual void OnOK();
	afx_msg void OnSelchangeComsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSEL_H__1B209403_67DC_4DF4_A3B5_37186509788B__INCLUDED_)
