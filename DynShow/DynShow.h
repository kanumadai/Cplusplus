// DynShow.h : main header file for the DYNSHOW application
//

#if !defined(AFX_DYNSHOW_H__1A0140C9_7800_4B9A_A2EC_BD136D3DDC8C__INCLUDED_)
#define AFX_DYNSHOW_H__1A0140C9_7800_4B9A_A2EC_BD136D3DDC8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDynShowApp:
// See DynShow.cpp for the implementation of this class
//


class CDynShowApp : public CWinApp
{
public:
	CDynShowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynShowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDynShowApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSHOW_H__1A0140C9_7800_4B9A_A2EC_BD136D3DDC8C__INCLUDED_)
