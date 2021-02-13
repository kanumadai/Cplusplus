// ACQCenter.h : main header file for the ACQCENTER application
//

#if !defined(AFX_ACQCENTER_H__F96369AE_3A97_4170_A79D_047F0DBF9E1C__INCLUDED_)
#define AFX_ACQCENTER_H__F96369AE_3A97_4170_A79D_047F0DBF9E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACQCenterApp:
// See ACQCenter.cpp for the implementation of this class
//

class CACQCenterApp : public CWinApp
{
public:
	CACQCenterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQCenterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACQCenterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQCENTER_H__F96369AE_3A97_4170_A79D_047F0DBF9E1C__INCLUDED_)
