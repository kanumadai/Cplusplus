// Maint.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Maint.h"
#include "CommSet.h"
#include "DataSample.h"
#include "EnvirmentParam.h"
#include "BoardLocal.h"
#include "BoardEnerge.h"
#include "TimeModfy.h"
#include "CysNum.h"
#include "TSeries.h"
#include "Anol.h"
#include "Login.h"
#include "SFlash.h"
#include "FpgaCounter.h"
#include "MaintDLL.h"
#include "EnvirmentLimit.h"
#include "PetApi.h"
#include "G_Fuction.h"
#include <afxwin.h>


#include "About.h"
#include "MainFrm.h"
#include "MaintDoc.h"
#include "MaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int	g_Port_Sel = 1;
int  g_HostBaud = 9600;
int  g_BucketBaud = 9600;

//位置边界保存标志
BOOL g_bPLimitChangeValid;


/////////////////////////////////////////////////////////////////////////////
// CMaintApp

BEGIN_MESSAGE_MAP(CMaintApp, CWinApp)
	//{{AFX_MSG_MAP(CMaintApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_COMMSET, OnMenuCommset)
	ON_COMMAND(ID_MENU_ENVIRMENTPARAM, OnMenuEnvirmentparam)
	ON_COMMAND(ID_MENU_BOARDLOCAL, OnMenuBoardlocal)
	ON_COMMAND(ID_MENU_BOARDENERG, OnMenuBoardenerg)
	ON_COMMAND(ID_MENU_TIMEMODFY, OnMenuTimemodfy)
	ON_COMMAND(ID_MENU_CYSNUM, OnMenuCysnum)
	ON_COMMAND(ID_MENU_TSERIES, OnMenuTseries)
	ON_COMMAND(IDR_MENU_FPGACOUNT, OnMenuFpgaCounter)
	ON_COMMAND(IDC_MENU_ABOUTME, OnMenuAboutme)
	ON_COMMAND(IDR_MENU_ENVIRMENT_LIMIT, OnMenuEnvirmentLimit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintApp construction

CMaintApp::CMaintApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMaintApp object

CMaintApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMaintApp initialization

BOOL CMaintApp::InitInstance()
{
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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMaintDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMaintView));
	AddDocTemplate(pDocTemplate);


	CLogIn dlgLogin;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	bool blogin = true;

	while (blogin) {
		if ( dlgLogin.DoModal() == IDCANCEL ) {
			// They quit on us... :( 
			return FALSE;
		} // if

		if((dlgLogin.m_strPassWord != "guest")&&(dlgLogin.m_strPassWord != "zzpet")) {
			blogin = true;
			//return FALSE;
		}
		else blogin = false;
	}


	CSFlash* dlgSplash = new CSFlash;
	dlgSplash->Create(IDD_SFLASH,NULL);
	dlgSplash->m_strUser = dlgLogin.m_strUser;
	dlgSplash->m_strCompany = dlgLogin.m_strCompany;
	dlgSplash->UpdateData(FALSE);
	dlgSplash->RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ERASENOW);
	Sleep(1200); // delay a bit to show off...

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	Sleep(200);
	dlgSplash->DestroyWindow();	
	delete dlgSplash;


	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMaintApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMaintApp message handlers


void CMaintApp::OnMenuCommset() 
{
	// TODO: Add your command handler code here
	CCommSet comset;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	comset.DoModal();	
}

void CMaintApp::OnMenuEnvirmentparam() 
{
	// TODO: Add your command handler code here
	CEnvirmentParam envirparam;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	envirparam.DoModal();
	
}

/*void CMaintApp::OnMenuDataSample() 
{
	// TODO: Add your command handler code here

	CDataSample datasamp;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 
	datasamp.DoModal();
}
*/

void CMaintApp::OnMenuBoardlocal() 
{
	// TODO: Add your command handler code here
	CBoardLocal boardlocal;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	boardlocal.DoModal();

}

void CMaintApp::OnMenuBoardenerg() 
{
	// TODO: Add your command handler code here
	CBoardEnerge boarden;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 
	boarden.DoModal();	



//	CDataSample  datasample;
//	datasample.DoModal();


}

void CMaintApp::OnMenuTimemodfy() 
{
	// TODO: Add your command handler code here
	CTimeModfy timemodfy;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	timemodfy.DoModal();
	
}

void CMaintApp::OnMenuCysnum() 
{
	// TODO: Add your command handler code here
	CCysNum cysnum;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	cysnum.DoModal();
	
}

void CMaintApp::OnMenuTseries() 
{
	// TODO: Add your command handler code here
	CTseries tseries;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	tseries.DoModal();

}

void CMaintApp::OnMenuFpgaCounter() 
{
	CFpgaCounter tcounter;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	tcounter.DoModal();	
}


bool UCRC( unsigned char* buff)
{
	unsigned char ucrc;
	int num=0;
	num = (int)buff[3];
	num = (num<<8) + (int)buff[2];		
	
	if((num <= 4)&&(num>9000)) return false;

	if(buff[0] != 0xB0) return false;
	if( (buff[1] < 0x80) || (buff[1] > 0x9f) )  return false;	 
	if (buff[num-1] != 0x88) return false;

	ucrc = buff[0];
	for(int i=1; i<num-2;i++) {
		ucrc ^= buff[i];
	}
	if(ucrc == buff[num-2]) return true;
	else return false;
}

bool ClearCommRecBuff(void)
{
	int size;
	unsigned char data[10000];
	BOOL flag = true;
	while (flag) {
		#ifdef _SELECT_232_PORT
			size = ReadResultMaint232(data);
		#else
			size = ReadResultMaintainMod(data);
		#endif	
		if(size == 0) flag = false;
	}
	return true;
}


void CMaintApp::OnMenuAboutme() 
{
	CAbout about;
	about.DoModal();
}



int WriteCommandMaintainMod(int lenth, BYTE* pdata)//写维护命令  加了个对象头 32；
{
	int datalenth = lenth +1;
	BYTE * pd;
	pd = new unsigned char[1024];

	(*pd) = 32;
	memcpy(pd+1,pdata,lenth);
	int nWR = WriteCommandMaintain(datalenth,pd);
	
	delete [] pd;
	return nWR;
}
int ReadResultMaintainMod(BYTE* rddata) //读维护结果
{
	BYTE * pd;
	pd = new unsigned char[9*1024];

	int datalenth = ReadResultMaintain(pd);	

	if ( ((*pd) != 32 ) || (datalenth == 0) ) { //不等于对象 32 或者 接收数据个数为0；退出
		delete [] pd;
		return 0;
	}
	memcpy(rddata,pd+1,datalenth -1);

	delete [] pd;
	return datalenth-1;
}



void CMaintApp::OnMenuEnvirmentLimit() 
{
	CEnvirmentLimit envirmentlimit;
	SetDialogBkColor(RGB(219,206,198), RGB(185, 0,0)); 

	envirmentlimit.DoModal();	
}
