// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CE92B9D5_DE5A_4B91_BA78_E0D567919817__INCLUDED_)
#define AFX_STDAFX_H__CE92B9D5_DE5A_4B91_BA78_E0D567919817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#include "MySocket.h"

//#include "OWSProtocol.h"
#include "SIOProtocol.h"
#include "ErrCode.h"

#ifdef _FH_SIM
#pragma message("------------------------------------CFH Runing in Sim Mode\r\n")
#else
#pragma message("------------------------------------CFH Runing in Real Mode\r\n")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CE92B9D5_DE5A_4B91_BA78_E0D567919817__INCLUDED_)
