// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
/*
首先要添加静态库：
Project-----Settings------Link
Object/Library modules  下添加：ws2_32.lib 

再添加头文件：

找到StdAfx.h文件，双击打开：
在以下语句的下方：#include <afxcmn.h>，添加：
#include <winsock2.h>
*/
#if !defined(AFX_STDAFX_H__2C103501_9A4E_4F8F_A105_59F8410ED568__INCLUDED_)
#define AFX_STDAFX_H__2C103501_9A4E_4F8F_A105_59F8410ED568__INCLUDED_

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
#include <winsock2.h>       //网络连接
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2C103501_9A4E_4F8F_A105_59F8410ED568__INCLUDED_)
