#if !defined(AFX_INPUT1_H__C6B171A7_4CF2_4C77_AB7D_365D3B5EF554__INCLUDED_)
#define AFX_INPUT1_H__C6B171A7_4CF2_4C77_AB7D_365D3B5EF554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Input1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInput1 dialog

class CInput1 : public CDialog
{
// Construction
public:
	CInput1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInput1)
	enum { IDD = IDD_INPUT1 };
	short	m_wValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInput1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInput1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUT1_H__C6B171A7_4CF2_4C77_AB7D_365D3B5EF554__INCLUDED_)
