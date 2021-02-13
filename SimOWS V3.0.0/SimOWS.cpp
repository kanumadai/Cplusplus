// SimOWS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SimOWS.h"
#include "SimOWSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimOWSApp

BEGIN_MESSAGE_MAP(CSimOWSApp, CWinApp)
	//{{AFX_MSG_MAP(CSimOWSApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimOWSApp construction

CSimOWSApp::CSimOWSApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSimOWSApp object

CSimOWSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSimOWSApp initialization

#pragma data_seg("Shared")     
int volatile g_nInstanceCnt = 0;
#pragma data_seg()
#pragma comment(linker,"/section:Shared,RWS")

BOOL CSimOWSApp::InitInstance()
{
	g_nInstanceCnt++;
	if( g_nInstanceCnt > 1){
		AfxMessageBox("程序已经启动，只允许一个实例运行！");
		g_nInstanceCnt--;
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSimOWSDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
