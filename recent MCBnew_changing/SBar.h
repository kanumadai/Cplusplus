#if !defined(AFX_SBAR_H__A6C2F8A1_DA4D_49E2_8B45_E16CD7BD8321__INCLUDED_)
#define AFX_SBAR_H__A6C2F8A1_DA4D_49E2_8B45_E16CD7BD8321__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSBar window

class CSBar : public CScrollBar
{
// Construction
public:
	CSBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBAR_H__A6C2F8A1_DA4D_49E2_8B45_E16CD7BD8321__INCLUDED_)
