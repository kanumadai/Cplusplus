// ACQCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACQCenter.h"
#include "ACQCenterDlg.h"
#include "SysInitDlg.h"

#include "DbgPrt.h"

#include "OWS.h"
#include "FH.h"
#include "SIO.h"
//#include "BuckSIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
BOOL  g_bOWSQuitNormal;
COWS  g_OWS;
#ifndef _FH_SIM
	CFH   g_FH;
#else
	CFHSim g_FH;
#endif
CSIO  g_SIO;
//CBuckSIO g_BuckSIO;

//调试输出控件
#if (DBG_LEVEL==3)
CPrintSocket g_PrtSocket;
#endif

BOOL g_bDbgPrtEn;

HWND g_hWndSIO;     extern BOOL g_bSIODbgPrtEn;
HWND g_hWndFH;		extern BOOL g_bFHDbgPrtEn;
HWND g_hWndOWS;
HWND g_hWndProtocol;

HANDLE g_hEvtSysError;
extern HANDLE g_hEvtSIO_Alarm;
extern HANDLE g_hEvtOWS_ConnectLost;

DWORD WINAPI Thread_SelfTest(LPVOID  lpData);

volatile extern MCTL_INFO g_MCtlInfo;
extern HANDLE g_hEvtSIO_EMCReset;
DWORD WINAPI ThreadMonitorExit(LPVOID  lpData);

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

/////////////////////////////////////////////////////////////////////////////
// CACQCenterDlg dialog

CACQCenterDlg::CACQCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACQCenterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACQCenterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CACQCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACQCenterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

#define WM_SYSTEM_SELFTEST (WM_USER+100)
BEGIN_MESSAGE_MAP(CACQCenterDlg, CDialog)
	//{{AFX_MSG_MAP(CACQCenterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PAUSE_DISP_COININFO, OnPauseDispCoininfo)
	ON_BN_CLICKED(IDC_PAUSE_DISP_SIOINFO, OnPauseDispSioinfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SYSTEM_SELFTEST, OnSystemSelfTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACQCenterDlg message handlers

BOOL CACQCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

/*	
	DEVMODE devMode;
	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS, &devMode);
	if( devMode.dmPelsWidth!=1024 ){
		devMode.dmPelsWidth = 1024;
		devMode.dmPelsHeight = 768;
		ChangeDisplaySettings(&devMode, 0);
	}
*/
	// TODO: Add extra initialization here
	g_hWndFH  =	::GetDlgItem(m_hWnd, IDC_EDIT_FH);
	g_hWndSIO = ::GetDlgItem(m_hWnd, IDC_EDIT_SIO);
	g_hWndOWS = ::GetDlgItem(m_hWnd, IDC_EDIT_OWS);
	g_hWndProtocol = ::GetDlgItem(m_hWnd, IDC_EDIT_PROTOCOL);

	g_hEvtSysError = CreateEvent(NULL, TRUE, FALSE, NULL);

	CSysInitDlg dlg;

	dlg.m_bEnDbgBucket  = AfxGetApp()->GetProfileInt("INIT", "DbgBuck",0);
	dlg.m_bEnSIOEMC     = AfxGetApp()->GetProfileInt("INIT", "EMContorl",1);
	dlg.m_bEnSIOBucket  = AfxGetApp()->GetProfileInt("INIT", "BUCKETContorl",1);

	dlg.DoModal();
	((CButton*)GetDlgItem(IDC_EN_DBGBUCK))->SetCheck(dlg.m_bEnDbgBucket);
	((CButton*)GetDlgItem(IDC_EN_SIOEMC))->SetCheck(dlg.m_bEnSIOEMC);
//	((CButton*)GetDlgItem(IDC_EN_SIOBUCKET))->SetCheck(dlg.m_bEnSIOBucket);
	g_bDbgPrtEn = dlg.m_bEnDbg;
	g_bSIODbgPrtEn = TRUE;
	g_bFHDbgPrtEn  = TRUE;

	ShowTips();

// ------------Listen Print Socket-------------------
#if (DBG_LEVEL == 3)
	int nClientPort = 8090;
	BOOL bFlag = g_PrtSocket.Create(nClientPort,SOCK_STREAM,FD_ACCEPT);
	if( !bFlag )
	{
		AfxMessageBox("Print Socket Failed!");
		g_PrtSocket.Close();
	}

	if( !g_PrtSocket.Listen(1) )
		AfxMessageBox("Print Socket Listen Error!");
#endif	
//	---------------------------------------------------

	PostMessage(WM_SYSTEM_SELFTEST, 0, 0);
	  
	DWORD dwThreadID;
	CreateThread(NULL,0,
		         ThreadMonitorExit, NULL, 
				 0, &dwThreadID
				);
#ifdef _FH_SIM
	SetWindowText("AcqCenter(Sim Mode)");
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CACQCenterDlg::ShowTips()
{
	if( !g_bDbgPrtEn ) return;

	char szTemp[512];
	switch( g_MCtlInfo.Bed_stat){
	case still: 
		 sprintf(szTemp, "BED:still, x=%d, y=%d", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		 break;
	case move:
		 sprintf(szTemp, "BED:move, x=%d, y=%d", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		 break;
	case out_stop:
		 sprintf(szTemp, "BED:out limited, x=%d, y=%d", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		 break;
	case in_stop:
		 sprintf(szTemp, "BED:in limited, x=%d, y=%d", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		 break;
	default:
		 sprintf(szTemp, "BED:unknow, x=%d, y=%d", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		 break;
	}
	SetDlgItemText(IDC_TIPS_BED, szTemp);

	switch(g_MCtlInfo.Rod_stat){
	case out:
		 sprintf(szTemp, "ROD:out, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	case in:
		 sprintf(szTemp, "ROD:in, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	case outing:
		 sprintf(szTemp, "ROD:out in process, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	case ining:
		 sprintf(szTemp, "ROD:in in process, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	case stop:
		 sprintf(szTemp, "ROD:stop, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	case rolling:
		 sprintf(szTemp, "ROD:rolling, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	default:
	case unknow:
		 sprintf(szTemp, "ROD:unknow, speed=%d", g_MCtlInfo.wRod_rate);
		 break;
	}
	SetDlgItemText(IDC_TIPS_ROD, szTemp);
/*
	switch( g_MCtlInfo.septa_stat ){
	case in:  
		 strcpy(szTemp, "SEPTA:in");
		 break;
	case ining:
		 strcpy(szTemp, "SEPTA:in in process");
		 break;
	case out:
		 strcpy(szTemp, "SEPTA:out");
		 break;
	case outing:
		 strcpy(szTemp, "SEPTA:out in process");
		 break;
	case stop:
		 strcpy(szTemp, "SEPTA:stop");
		 break;
	case rolling:
		 strcpy(szTemp, "SEPTA:rolling XXX");
		 break;
	case unknow:
		 strcpy(szTemp, "SEPTA:unknow");
		 break;
	}

	SetDlgItemText(IDC_TIPS_SEPTA, szTemp);
*/
	sprintf(szTemp, "GANTRY Temp. Threshold:(%d<->%d)", g_MCtlInfo.wTH_GANTRYT_H, g_MCtlInfo.wTH_GANTRYT_L);
	SetDlgItemText(IDC_TIPS_TH_GANTRY, szTemp);

//  sprintf(szTemp, "HV I Threshold:(%d<->%d)", g_MCtlInfo.wTH_I_H, g_MCtlInfo.wTH_I_L);
//	SetDlgItemText(IDC_TIPS_TH_HVI, szTemp);

//	sprintf(szTemp, "Cooler Temp. Threshold:(%d<->%d)", g_MCtlInfo.wTH_WATERCOOLT_H, g_MCtlInfo.wTH_WATERCOOLT_L);
//	SetDlgItemText(IDC_TIPS_TH_WATERCOOL, szTemp);

/*	if( g_MCtlInfo.bMainPowerOn) strcpy(szTemp, "Bucket Power:ON");
	else strcpy(szTemp, "Bucket Power:OFF");
	if(g_MCtlInfo.bHVOn) strcat(szTemp, "  HV:ON");
	else strcat(szTemp, "  HV:OFF");
	SetDlgItemText(IDC_TIPS_POWER, szTemp);

	if( g_BuckSIO.m_WorkStatus.bWorking)
		sprintf(szTemp, "Bucket:working(%d)", g_BuckSIO.m_WorkStatus.dwWorkTime);
	else
		strcpy(szTemp, "Bucket:stop");

  switch( g_BuckSIO.m_WorkStatus.datMode){
	case mode_energy:
		 sprintf(szTemp, "%s energy", szTemp);
		 break;
	case mode_time:
		 sprintf(szTemp, "%s time", szTemp);
		 break;
	case mode_sd:
		 sprintf(szTemp, "%s sd", szTemp);
		 break;
	case mode_coin0:
		 sprintf(szTemp, "%s coin0", szTemp);
		 break;
	case mode_coin1:
		 sprintf(szTemp, "%s coin1", szTemp);
		 break;
	case mode_unknow:
	default:
		 sprintf(szTemp, "%s unknow", szTemp);
		 break;
	}
*/
	sprintf(szTemp, "%s coin0", szTemp);
//	if( g_BuckSIO.m_WorkStatus.b9600) strcat(szTemp, "  Speed:9600");
//	else strcat(szTemp, "  Speed:38400");
//	SetDlgItemText(IDC_TIPS_BUCKET, szTemp);

}

void CACQCenterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CACQCenterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CACQCenterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CACQCenterDlg::OnSystemSelfTest(WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadID;
	CreateThread(NULL, 0,
		         Thread_SelfTest, NULL,
				 0, &ThreadID);
	return 1;
}

void CACQCenterDlg::OnOK() 
{
	// TODO: Add extra validation here
	g_FH.CloseDevice();
	g_SIO.CloseSIO();
//	g_BuckSIO.CloseSIO();
	g_OWS.Close();

	CloseHandle(g_hEvtSysError);

	CDialog::OnOK();
}

void CACQCenterDlg::OnDestroy() 
{
	// TODO: Add your message handler code here
	CDialog::OnDestroy();
}


//数据采集系统初始化
//建立觼E骺丶扑慊腟OCKET连接
BOOL CACQCenterDlg::SelfTest_OWS()
{
	BYTE szBuf[128];
	char szHostIP[50];
	int nHostPort; 

	strcpy( szHostIP, 
		    AfxGetApp()->GetProfileString("OWS", "HostIP", "192.168.1.1")
		  );

	nHostPort = AfxGetApp()->GetProfileInt("OWS", "HostPort", 8080);
	
	sprintf( (char*)szBuf, "Begin Connect to OWS->%s:%d\r\n", szHostIP, nHostPort);
	DbgPrt( (char*)szBuf, g_hWndOWS);

	return g_OWS.Connect(szHostIP, nHostPort, -1);
}

//初始化机电控制电路
BOOL CACQCenterDlg::SelfTest_SIO()
{
	BYTE szBuf[128];
	char szPort[10];
	DWORD dwRodRate;  //rod 转速

	strcpy( szPort,
		    AfxGetApp()->GetProfileString("SIO", "Port", "COM1")
		  );
	dwRodRate = AfxGetApp()->GetProfileInt("SIO", "RodRate", 18);

	//连接机电控制电路
	sprintf((char*)szBuf, "Begin To Init SIO, connect port=%s, rod rate=%d\r\n", szPort, dwRodRate);
	DbgPrt((char*)szBuf, g_hWndOWS);
	if( !g_SIO.OpenSIO(szPort) ){
		g_OWS.SendAlarm(ERROR_FH_PORT_EMC);
		DbgPrt( "Connect EMC Port Error\r\n", g_hWndOWS );
		return FALSE;
	}

	g_SIO.SetSIOMode(FALSE);  //disable test mode
	//Sleep(2000);

	BOOL bEnEMC = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_SIOEMC))->GetCheck();   //协议控制机电电路选蟻E
	if(!bEnEMC){
		DbgPrt("EMControl SelfTest Skipped!\r\n", g_hWndOWS);
		return TRUE;
	}

	WaitForSingleObject(g_hEvtSIO_EMCReset, 1000);
	//   GANTRY初始化
/*	for( int i=0; i<6; i++){
		if( !g_SIO.QueryInitStatus() ){
			DbgPrt("Query GANTRY Init Status Error\r\n", g_hWndSIO);
		}
		if( g_MCtlInfo.bInitOK ) break;
		else  Sleep(15*1000);  //delay 15sec.
	}
	if( !g_MCtlInfo.bInitOK ){
		g_OWS.SendAlarm( ERROR_GANTRY_INIT );
		return FALSE;
	}
	Sleep(500);
*/



	//床信息初始化
	if(!g_SIO.QueryBedPos()){
		DbgPrt("Query Bed Pos Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_BED_QUERY_POS);
		return FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedStep() ){
		DbgPrt("Query Bed Move Step Len Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_BED_QUERY_STEP);
		return FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedMoveStatus() ){
		DbgPrt("Query Bed Move Status Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_BED_QUERY_MOVESTATUS);
		return FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedSwitchSet()){
		DbgPrt("Query Bed Switch Set Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_BED_QUERY_SWITCH);
		return FALSE;
	}
	Sleep(500);

	//ROD信息初始化
	if(!g_SIO.QueryRodStatus()){
		DbgPrt("QueryRodStatus Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_ROD_QUERY_STATUS);
		return FALSE;
	}
	Sleep(500);
/*	if(!g_SIO.SetRodRate((WORD)dwRodRate)){
		DbgPrt("SetRodRate Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_ROD_SETRATE);
		return FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryRodRate()){
		DbgPrt("QueryRodRate Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_ROD_QUERY_RATE);
		return FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryRodSwitchSet()){
		DbgPrt("QueryRodSwitchSet Error\r\n", g_hWndSIO);
		g_OWS.SendAlarm(ERROR_ROD_QUERY_SWITCH);
		return FALSE;
	}
	Sleep(500);
*/

	return TRUE;
}

 

BOOL CACQCenterDlg::SelfTest_FH()
{
	DWORD dwFreq, dwFactor;

	//default sample freq:1600
	dwFreq = AfxGetApp()->GetProfileInt("FH", "AdcSampleFreq", 4/*Default*/);   
	dwFactor = AfxGetApp()->GetProfileInt("FH", "AdcAmpFactor", 2);

	if( !g_FH.OpenDevice() ){
		DbgPrt("Open PLX9054 Device Error\r\n", g_hWndOWS);
		g_OWS.SendAlarm(ERROR_FH_OPENDEVICE);
		return FALSE;
	}

	if( !g_FH.EcgParamSet( dwFreq, dwFactor ) ){
		DbgPrt("Set ECG PARAMTER ERROR\r\n", g_hWndOWS);
		g_FH.CloseDevice();
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI Thread_SelfTest(LPVOID  lpData)
{
	CACQCenterDlg* pMainDlg = (CACQCenterDlg*)AfxGetMainWnd();

	int time_Instant,time_min,time_max, outfmt;
	time_Instant = AfxGetApp()->GetProfileInt("INIT", "Instant",12);
	time_min     = AfxGetApp()->GetProfileInt("INIT", "Min",    48);
	time_max     = AfxGetApp()->GetProfileInt("INIT", "Max",    60);
	outfmt       = AfxGetApp()->GetProfileInt("INIT", "DatFmt", 0);
	//move to OnInitDiglog bEnBuckDbg   = AfxGetApp()->GetProfileInt("INIT", "DbgBuck",0);
	//move to OnInitDialog bEnEMC       = AfxGetApp()->GetProfileInt("INIT", "EMContorl",1);

	pMainDlg->SetDlgItemInt(IDC_TIME_LJ,  time_Instant); //立即符合时紒E
	pMainDlg->SetDlgItemInt(IDC_TIME_MIN, time_min);     //臁延迟符合时紒E
	pMainDlg->SetDlgItemInt(IDC_TIME_MAX, time_max);     //畜延迟符合时紒E
	((CComboBox*)pMainDlg->GetDlgItem(IDC_OUTFMT))->SetCurSel(outfmt);    //输出符合数据格式
	//((CButton*)pMainDlg->GetDlgItem(IDC_EN_DBGBUCK))->SetCheck(bEnBuckDbg); //调试BUCKET选蟻E
	//((CButton*)pMainDlg->GetDlgItem(IDC_EN_SIOEMC))->SetCheck(bEnEMC);      //协议控制机电电路选蟻E

	if( !pMainDlg->SelfTest_OWS() ){
		DbgPrt("ACQ System Halt\r\n", g_hWndOWS);
		return 0;
	}

	if( !pMainDlg->SelfTest_FH() ){
		DbgPrt("CFH SelfTest Error\r\n", g_hWndOWS);
	}						 

	if( !pMainDlg->SelfTest_SIO() ){
		DbgPrt("SIO SelfTest Error\r\n", g_hWndOWS);
	}
/*
	if( !pMainDlg->SelfTest_Bucket() ){
		DbgPrt("Bucket SelfTest Error\r\n", g_hWndOWS);
	}
*/
	g_OWS.SendLogin();
	g_OWS.SendSynTime();

	return 0;
}

DWORD WINAPI ThreadMonitorExit(LPVOID  lpData)
{
	DWORD ret;
	BOOL bConti;
	HANDLE hEvent[4];

	WORD wAlarmCode;

	hEvent[0] = g_hEvtSysError;
	hEvent[1] = g_hEvtSIO_Alarm;
	hEvent[2] = g_hEvtOWS_ConnectLost;
	hEvent[3] = OpenEvent(SYNCHRONIZE, TRUE, "EVT_ACQ_EXIT");

	if( hEvent[3] == NULL)
		hEvent[3] = CreateEvent(NULL, FALSE, FALSE, NULL);

	bConti = TRUE;
	do{
		ret = WaitForMultipleObjects(4, hEvent, FALSE, INFINITE);
		switch(ret){
		case WAIT_OBJECT_0:	 //sys error
			 ResetEvent( hEvent[0] );
			 break;
		case WAIT_OBJECT_0+1: //sio alarm
			 ResetEvent( hEvent[1] );
			 if( g_MCtlInfo.alarm_Gantry == alarm){
				 g_MCtlInfo.alarm_Gantry = normal;
				 wAlarmCode = ERROR_ALARM_GANTRY | g_MCtlInfo.ac_Gantry.code8;
				 g_OWS.SendAlarm(wAlarmCode);
			 }
			 if( g_MCtlInfo.alarm_Bed == alarm){
				 g_MCtlInfo.alarm_Bed = normal;
				 wAlarmCode = ERROR_ALARM_BED | g_MCtlInfo.ac_Bed.code8;
				 g_OWS.SendAlarm(wAlarmCode);
			 }
			 if( g_MCtlInfo.alarm_Rod == alarm){
				 g_MCtlInfo.alarm_Rod = normal;
				 wAlarmCode = ERROR_ALARM_ROD | g_MCtlInfo.ac_Rod.code8;
				 g_OWS.SendAlarm(wAlarmCode);
			 }
			 if( g_MCtlInfo.alarm_Septa == alarm){
				 g_MCtlInfo.alarm_Septa = normal;
				 wAlarmCode = ERROR_ALARM_SEPTA | g_MCtlInfo.ac_Septa.code8;
				 g_OWS.SendAlarm(wAlarmCode);
			 }
			 if( g_MCtlInfo.alarm_ESTOP == alarm){
				 g_MCtlInfo.alarm_ESTOP = normal;
				 g_OWS.SendAlarm(ERROR_GANTRY_ESTOP);
			 }
			 if( g_MCtlInfo.alarm_HV == alarm){
				 g_MCtlInfo.alarm_HV = normal;
				 wAlarmCode = ERROR_ALARM_HV | g_MCtlInfo.ac_HV.code8;
				 g_OWS.SendAlarm(wAlarmCode);
			 }
			 SetEvent( g_hEvtSysError );
			 break;

		case WAIT_OBJECT_0+2: //ows connect lost
		case WAIT_OBJECT_0+3: //AcqTray Command: reboot
			 if( g_bOWSQuitNormal ){
				///// OWS quit

				g_OWS.Close();
				ResetEvent( g_hEvtOWS_ConnectLost );

				Sleep(10000);

				DbgPrt("OWS Quit, reconnect\r\n",g_hWndOWS);

				CACQCenterDlg* pMainDlg = (CACQCenterDlg*)AfxGetMainWnd();
				pMainDlg->SelfTest_OWS();

				g_OWS.SendLogin();
				g_OWS.SendSynTime();

			 }else{
				///// connect to OWS lost abnormal

				SetEvent( g_hEvtSysError );
				CloseHandle( hEvent[3] );
			 
				//delay 10s to reboot,
				//give others a chance to process g_hEvtSysError			 
				Sleep(10000);  
//				CACQCenterDlg::SystemReboot();
				////DbgPrt("OWS Connect Lost, reboot(Debug Temp)", g_hWndOWS);
			 
				bConti = FALSE;
			 }
			 break;
		}
	}while(bConti);

	return 1;
}

void CACQCenterDlg::SystemReboot()
{
	BOOL bREt;
	HANDLE hToken;
	TOKEN_PRIVILEGES newState; LUID luid;

	bREt = LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&luid );
	newState.PrivilegeCount = 1;
	newState.Privileges[0].Luid = luid;
	newState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	HANDLE hProc;
	DWORD dwProcId = GetCurrentProcessId();
    hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcId);	

	bREt = OpenProcessToken(hProc,TOKEN_ADJUST_PRIVILEGES/*TOKEN_ALL_ACCESS*/ ,&hToken);
	bREt = AdjustTokenPrivileges(hToken, FALSE, &newState,0, NULL, NULL);
	
	bREt = InitiateSystemShutdown(NULL, NULL, 3, TRUE, TRUE);
}


void CACQCenterDlg::OnPauseDispCoininfo() 
{
	// TODO: Add your control notification handler code here
	DbgPrt("---------------- Paused --------------\r\n", g_hWndFH);
	if( g_bFHDbgPrtEn ) g_bFHDbgPrtEn = FALSE;
	else g_bFHDbgPrtEn = TRUE;
}

void CACQCenterDlg::OnPauseDispSioinfo() 
{
	// TODO: Add your control notification handler code here
	DbgPrt("---------------- Paused --------------\r\n", g_hWndSIO);
	if( g_bSIODbgPrtEn ) g_bSIODbgPrtEn = FALSE;
	else g_bSIODbgPrtEn = TRUE;
}

