// Fast57600.h : main header file for the FAST57600 DLL
//

#if !defined(AFX_FAST57600_H__99BB70EE_47EA_494A_869A_B8CFFCC66B2B__INCLUDED_)
#define AFX_FAST57600_H__99BB70EE_47EA_494A_869A_B8CFFCC66B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//-----------------------------------------------------------------//
_declspec(dllexport) int WINAPI OpenShmMaint232(int portNo);
_declspec(dllexport) int WINAPI WriteCommandMaint232(int size,BYTE *command);
_declspec(dllexport) int WINAPI ReadResultMaint232(BYTE *command);
_declspec(dllexport) int WINAPI CloseShmMaint232(void);

//----------------------------------------------------------------//


#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFast57600App
// See Fast57600.cpp for the implementation of this class
//

class CFast57600App : public CWinApp
{
public:
	CFast57600App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFast57600App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFast57600App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAST57600_H__99BB70EE_47EA_494A_869A_B8CFFCC66B2B__INCLUDED_)
