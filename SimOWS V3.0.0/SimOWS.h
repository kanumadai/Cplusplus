// SimOWS.h : main header file for the SIMOWS application
//

#if !defined(AFX_SIMOWS_H__7DE52747_DEFD_4C7F_A462_41369A17FEA0__INCLUDED_)
#define AFX_SIMOWS_H__7DE52747_DEFD_4C7F_A462_41369A17FEA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimOWSApp:
// See SimOWS.cpp for the implementation of this class
//

class CSimOWSApp : public CWinApp
{
public:
	CSimOWSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimOWSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimOWSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMOWS_H__7DE52747_DEFD_4C7F_A462_41369A17FEA0__INCLUDED_)
