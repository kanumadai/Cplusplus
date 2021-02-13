// Maint.h : main header file for the MAINT application
//

#if !defined(AFX_MAINT_H__D7D64205_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_MAINT_H__D7D64205_BF45_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//akang 
#define _SELECT_232_PORT  1
//akang

#define WM_STATUSBAR_SHOW WM_USER+103

#define  WM_AUTOSAMPLE_RECFINISH  WM_USER+110

/////////////////////////////////////////////////////////////////////////////
// CMaintApp:
// See Maint.cpp for the implementation of this class
//

class CMaintApp : public CWinApp
{
public:
	CMaintApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMaintApp)
	afx_msg void OnAppAbout();
	afx_msg void OnMenuCommset();
	afx_msg void OnMenuEnvirmentparam();
	afx_msg void OnMenuBoardlocal();
	afx_msg void OnMenuBoardenerg();
	afx_msg void OnMenuTimemodfy();
	afx_msg void OnMenuCysnum();
	afx_msg void OnMenuTseries();
	afx_msg void OnMenuFpgaCounter();
	afx_msg void OnMenuAboutme();
	afx_msg void OnMenuEnvirmentLimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINT_H__D7D64205_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
