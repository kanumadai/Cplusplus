// MCB.h : main header file for the MCB application
//

#if !defined(AFX_MCB_H__E1829608_F98B_45CE_A909_85F81483D3BC__INCLUDED_)
#define AFX_MCB_H__E1829608_F98B_45CE_A909_85F81483D3BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMCBApp:
// See MCB.cpp for the implementation of this class
//

class CMCBApp : public CWinApp
{
public:
	CMCBApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCBApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMCBApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCB_H__E1829608_F98B_45CE_A909_85F81483D3BC__INCLUDED_)
