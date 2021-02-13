// ACQ_OWS.h : main header file for the ACQ_OWS application
//

#if !defined(AFX_ACQ_OWS_H__6C59C503_DE94_43B8_BFA8_69923B1E5562__INCLUDED_)
#define AFX_ACQ_OWS_H__6C59C503_DE94_43B8_BFA8_69923B1E5562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACQ_OWSApp:
// See ACQ_OWS.cpp for the implementation of this class
//

class CACQ_OWSApp : public CWinApp
{
public:
	CACQ_OWSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQ_OWSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACQ_OWSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQ_OWS_H__6C59C503_DE94_43B8_BFA8_69923B1E5562__INCLUDED_)
