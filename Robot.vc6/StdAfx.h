// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
/*
����Ҫ��Ӿ�̬�⣺
Project-----Settings------Link
Object/Library modules  ����ӣ�ws2_32.lib 

�����ͷ�ļ���

�ҵ�StdAfx.h�ļ���˫���򿪣�
�����������·���#include <afxcmn.h>����ӣ�
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
#include <winsock2.h>       //��������
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2C103501_9A4E_4F8F_A105_59F8410ED568__INCLUDED_)
