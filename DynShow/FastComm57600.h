// MaintDLL.h : main header file for the MAINTDLL DLL
//

#if !defined(AFX_MAINTDLL_H__475A76E6_CB0E_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_MAINTDLL_H__475A76E6_CB0E_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//-----------------------------------------------------------------//
_declspec(dllexport) int WINAPI OpenShmMaint232(int portNo);
_declspec(dllexport) int WINAPI WriteCommandMaint232(int size,BYTE *command);
_declspec(dllexport) int WINAPI ReadResultMaint232(BYTE *command);
_declspec(dllexport) int WINAPI CloseShmMaint232(void);

//----------------------------------------------------------------//


/////////////////////////////////////////////////////////////////////////////
// CMaintDLLApp
// See MaintDLL.cpp for the implementation of this class
//

class CMaintDLLApp : public CWinApp
{
public:
	CMaintDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintDLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMaintDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINTDLL_H__475A76E6_CB0E_11D6_9644_00E04C50B69B__INCLUDED_)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


