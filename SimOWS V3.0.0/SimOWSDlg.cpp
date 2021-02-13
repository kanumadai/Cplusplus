// SimOWSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "SimOWSDlg.h"
#include "TestParamDlg.h"

#include "DbgPrt.h"
#include "..\AcqCenter\OWSProtocol.h"

#include "BedCoordDlg.h"
#include "GetThDlg.h"
#include "Input1.h"
#include "RelateMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUF_SIZE 32*1024*1024
DWORD  g_dwBufLen[4];   
HANDLE g_hFHBuf[4];
HANDLE g_hFHEvt[4];
HANDLE g_hEvtExit;
BOOL   g_bFH;

DWORD g_dwEvtCnt;

HANDLE g_hEvtStopWriteFile;
CRITICAL_SECTION g_lockFile; 
CFile g_saveFile;

BOOL   g_bDbgPrtEn;
HWND   g_hWndOWS;
HWND   g_hWndFH = NULL;
HWND   g_hWndSIO = NULL;

int    g_BucketNum;

DWORD g_dwContCoinCnt;
DWORD g_dwDelayCoinCnt;

DWORD WINAPI WriteThread(LPVOID pData);
DWORD WINAPI StopWriteThread(LPVOID pData);
DWORD WINAPI ThreadAutoAcqCoinTime(LPVOID pData);
DWORD WINAPI ThreadAutoAcqBucket(LPVOID pData);

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
// CSimOWSDlg dialog

CSimOWSDlg::CSimOWSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimOWSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimOWSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimOWSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimOWSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimOWSDlg, CDialog)
	//{{AFX_MSG_MAP(CSimOWSDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_QUERY_BED_MOVESTATUS, OnQueryBedMovestatus)
	ON_COMMAND(IDM_QUERY_BED_POS, OnQueryBedPos)
	ON_COMMAND(IDM_QUERY_BED_STEP, OnQueryBedStep)
	ON_COMMAND(IDM_QUERY_BED_SWITCHSET, OnQueryBedSwitchset)
	ON_COMMAND(IDM_QUERY_ROD_RATE, OnQueryRodRate)
	ON_COMMAND(IDM_QUERY_ROD_SWITCHSET, OnQueryRodSwitchset)
	ON_COMMAND(IDM_QUERY_SEPTA_SWITCHSET, OnQuerySeptaSwitchset)
	ON_COMMAND(IDM_QUERY_TBED, OnQueryTbed)
	ON_COMMAND(IDM_QUERY_TG, OnQueryTg)
	ON_COMMAND(IDM_QUERY_TH_I, OnQueryThI)
	ON_COMMAND(IDM_QUERY_TH_TG, OnQueryThTg)
	ON_COMMAND(IDM_QUERY_TH_TW, OnQueryThTw)
	ON_COMMAND(IDM_QUERY_TH_V, OnQueryThV)
	ON_COMMAND(IDM_QUERY_TROD, OnQueryTrod)
	ON_COMMAND(IDM_QUERY_TSEPTA, OnQueryTsepta)
	ON_COMMAND(IDM_QUERY_TW, OnQueryTw)
	ON_COMMAND(IDM_SET_BEDCOORD, OnSetBedcoord)
	ON_COMMAND(IDM_SET_BEDSTEP, OnSetBedstep)
	ON_COMMAND(IDM_SET_RODRATE, OnSetRodrate)
	ON_COMMAND(IDM_SET_TH_I, OnSetThI)
	ON_COMMAND(IDM_SET_TH_TG, OnSetThTg)
	ON_COMMAND(IDM_SET_TH_TW, OnSetThTw)
	ON_COMMAND(IDM_SET_TH_V, OnSetThV)
	ON_COMMAND(IDM_CTRL_BEDMOVESTEPX, OnCtrlBedmovestepx)
	ON_COMMAND(IDM_CTRL_BEDMOVESTEPY, OnCtrlBedmovestepy)
	ON_COMMAND(IDM_CTRL_BEDMOVEX, OnCtrlBedmovex)
	ON_COMMAND(IDM_CTRL_BEDMOVEX_RELATE, OnCtrlBedmovexRelate)
	ON_COMMAND(IDM_CTRL_BEDMOVEY, OnCtrlBedmovey)
	ON_COMMAND(IDM_CTRL_BEDMOVEY_RELATE, OnCtrlBedmoveyRelate)
	ON_COMMAND(IDM_CTRL_BEDSTOPALL, OnCtrlBedstopall)
	ON_COMMAND(IDM_CTRL_BEDSTOPX, OnCtrlBedstopx)
	ON_COMMAND(IDM_CTRL_BEDSTOPY, OnCtrlBedstopy)
	ON_COMMAND(IDM_CTRL_RODESTOP, OnCtrlRodestop)
	ON_COMMAND(IDM_CTRL_RODIN, OnCtrlRodin)
	ON_COMMAND(IDM_CTRL_RODOUT, OnCtrlRodout)
	ON_COMMAND(IDM_CTRL_RODROLL, OnCtrlRodroll)
	ON_COMMAND(IDM_CTRL_RODSTOP, OnCtrlRodstop)
	ON_COMMAND(IDM_CTRL_SEPTAESTOP, OnCtrlSeptaestop)
	ON_COMMAND(IDM_CTRL_SEPTAIN, OnCtrlSeptain)
	ON_COMMAND(IDM_CTRL_SEPTAOUT, OnCtrlSeptaout)
	ON_COMMAND(IDM_CTRL_SEPTASTOP, OnCtrlSeptastop)
	ON_COMMAND(IDM_CTRL_HV_OFF, OnCtrlHvOff)
	ON_COMMAND(IDM_CTRL_HV_ON, OnCtrlHvOn)
	ON_COMMAND(IDM_CTRL_LASEROFF, OnCtrlLaseroff)
	ON_COMMAND(IDM_CTRL_LASERON, OnCtrlLaseron)
	ON_COMMAND(IDM_CTRL_MPOWER_OFF, OnCtrlMpowerOff)
	ON_COMMAND(IDM_CTRL_MPOWER_ON, OnCtrlMpowerOn)
	ON_BN_CLICKED(IDC_DISP_GATE, OnDispGate)
	ON_BN_CLICKED(IDC_DISP_ROD, OnDispRod)
	ON_BN_CLICKED(IDC_DISP_BUCK, OnDispBucket)
	ON_COMMAND(IDM_QUERY_ROD_STATUS, OnQueryRodStatus)
	ON_COMMAND(IDM_QUERY_SEPTA_STATUS, OnQuerySeptaStatus)
	ON_BN_CLICKED(IDC_DISP_LMD, OnDispLmd)
	ON_BN_CLICKED(IDC_INITACS, OnInitACS)
	ON_COMMAND(IDM_QUERY_HVSTATUS, OnQueryHVStatus)
	ON_COMMAND(IDM_TESTMODE, OnTestMode)
	ON_BN_CLICKED(IDC_AUTOACQ_COINTIME, OnAutoAcqCoinTime)
	ON_BN_CLICKED(IDC_ACQ_BUCKET, OnAcqBucket)
	ON_BN_CLICKED(IDC_ACQ_COINPOS, OnAcqCoinpos)
	ON_BN_CLICKED(IDC_AUTOACQ_BUCKET, OnAutoAcqBucket)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_ACQ_COINTIME, OnAcqCoinTime)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimOWSDlg message handlers


void CSimOWSDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( (nFlags&MK_SHIFT) == 0 ) return;

	if( m_bExpertMode ){
		m_bExpertMode = FALSE;
		GetDlgItem(IDC_START)->ShowWindow(SW_HIDE);
	}else{
		m_bExpertMode = TRUE;
		GetDlgItem(IDC_START)->ShowWindow(SW_SHOW);
	}
	
	//CDialog::OnRButtonDblClk(nFlags, point);
}

BOOL CSimOWSDlg::OnInitDialog()
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
	//SetDlgItemText(IDC_TIPS_ACQ, "");
	
	m_menu.LoadMenu(IDR_MENU_MAIN);

	m_bExpertMode = FALSE;
	GetDlgItem(IDC_START)->ShowWindow(SW_HIDE);

	/////////////////////////////////////////////////////////////////////////////
	m_WndType = dat_fhtime;
	m_WndEnergy.CreateWnd(); m_WndEnergy.ShowWindow(SW_HIDE);
	m_WndTime.CreateWnd();  m_WndTime.ShowWindow(SW_HIDE);
	m_WndSD.CreateWnd();    m_WndSD.ShowWindow(SW_HIDE);
	m_WndCoinTime.CreateWnd(); m_WndCoinTime.ShowWindow(SW_HIDE);

	g_bDbgPrtEn = TRUE;
	GetDlgItem(IDC_DBGSHOW, &g_hWndOWS);

	InitializeCriticalSection(&g_lockFile);

	//init fh buf
	for(int i=0; i<4; i++)
	{
		g_hFHBuf[i] = GlobalAlloc(GMEM_FIXED, BUF_SIZE);
		g_hFHEvt[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		ASSERT( g_hFHBuf[i] != NULL );
	}
	g_hEvtExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	////////////////////////////////////////////////////
	strcpy( m_InitAcsData[0].szKey, "Coin_Prompt" );     m_InitAcsData[0].dwValue = 8;
	strcpy( m_InitAcsData[1].szKey, "Coin_DelayMin" );   m_InitAcsData[1].dwValue = 48;
	strcpy( m_InitAcsData[2].szKey, "Coin_DelayMax" );   m_InitAcsData[2].dwValue = 60;
	strcpy( m_InitAcsData[3].szKey, "Coin_DatFmt" );     m_InitAcsData[3].dwValue = 0;
	strcpy( m_InitAcsData[4].szKey, "Protocol_DbgBucket" ); m_InitAcsData[4].dwValue = 0;
	strcpy( m_InitAcsData[5].szKey, "Protocol_EnSIO" );     m_InitAcsData[5].dwValue = 1;
	strcpy( m_InitAcsData[6].szKey, "Protocol_RodRate1");   m_InitAcsData[6].dwValue = 8;
	strcpy( m_InitAcsData[7].szKey, "Protocol_RodRate2");   m_InitAcsData[7].dwValue = 12;
	strcpy( m_InitAcsData[8].szKey, "Protocol_RodRate3");   m_InitAcsData[8].dwValue = 18;
	strcpy( m_InitAcsData[9].szKey, "Adc_SampleFreq" );     m_InitAcsData[9].dwValue = 4;
	strcpy( m_InitAcsData[10].szKey, "Adc_AmpFactor" );      m_InitAcsData[10].dwValue = 2;
	strcpy( m_InitAcsData[11].szKey, "Protocol_EnSIOBucket" ); m_InitAcsData[11].dwValue = 1;

	strcpy( m_InitBuckData[0].szKey, "Acq_Time"); m_InitBuckData[0].dwValue = 2000;
	strcpy( m_InitBuckData[1].szKey, "Acq_mode"); m_InitBuckData[1].dwValue = 3;

	strcpy( m_InitEMCData[0].szKey, "set_hv_value");           m_InitEMCData[0].dwValue = 1;
	strcpy( m_InitEMCData[1].szKey, "set_lv_value");           m_InitEMCData[1].dwValue = 1;
	strcpy( m_InitEMCData[2].szKey, "tem_limen_value_up");     m_InitEMCData[2].dwValue = 400;
	strcpy( m_InitEMCData[3].szKey, "tem_limen_value_down");   m_InitEMCData[3].dwValue = 100;
	strcpy( m_InitEMCData[4].szKey, "hv_amp_limen_up");        m_InitEMCData[4].dwValue = 100;
	strcpy( m_InitEMCData[5].szKey, "hv_amp_limen_down");      m_InitEMCData[5].dwValue = 60;
	strcpy( m_InitEMCData[6].szKey, "cool_device_limen_up");   m_InitEMCData[6].dwValue = 300;
	strcpy( m_InitEMCData[7].szKey, "cool_device_limen_down"); m_InitEMCData[7].dwValue = 50;
	
	m_dlgInitAcs.m_nTimeInstant =  m_InitAcsData[0].dwValue;
	m_dlgInitAcs.m_nTimeMin     =  m_InitAcsData[1].dwValue; 
	m_dlgInitAcs.m_nTimeMax     =  m_InitAcsData[2].dwValue; 
	m_dlgInitAcs.m_nCoinFmt     =  m_InitAcsData[3].dwValue; 
	m_dlgInitAcs.m_bEnDbgBucket =  m_InitAcsData[4].dwValue; 
	m_dlgInitAcs.m_bEnEmc       =  m_InitAcsData[5].dwValue; 
	m_dlgInitAcs.m_nRodRate1    =  m_InitAcsData[6].dwValue; 
	m_dlgInitAcs.m_nRodRate2    =  m_InitAcsData[7].dwValue; 
	m_dlgInitAcs.m_nRodRate3    =  m_InitAcsData[8].dwValue; 
	m_dlgInitAcs.m_nEcgFreq     =  m_InitAcsData[9].dwValue; 
	m_dlgInitAcs.m_nEcgPreAmp   =  m_InitAcsData[10].dwValue;
	m_dlgInitAcs.m_bEnBucketSIO =  m_InitAcsData[11].dwValue;
    m_dlgInitAcs.m_nAcqTime     =  m_InitBuckData[0].dwValue;
    m_dlgInitAcs.m_nAcqMode     =  m_InitBuckData[1].dwValue;

	m_dlgInitAcs.DoModal();

	m_InitAcsData[0].dwValue = m_dlgInitAcs.m_nTimeInstant ;
	m_InitAcsData[1].dwValue = m_dlgInitAcs.m_nTimeMin     ; 
	m_InitAcsData[2].dwValue = m_dlgInitAcs.m_nTimeMax     ; 
	m_InitAcsData[3].dwValue = m_dlgInitAcs.m_nCoinFmt     ; 
	m_InitAcsData[4].dwValue = m_dlgInitAcs.m_bEnDbgBucket ; 
	m_InitAcsData[5].dwValue = m_dlgInitAcs.m_bEnEmc       ; 
	m_InitAcsData[6].dwValue = m_dlgInitAcs.m_nRodRate1    ; 
	m_InitAcsData[7].dwValue = m_dlgInitAcs.m_nRodRate2    ; 
	m_InitAcsData[8].dwValue = m_dlgInitAcs.m_nRodRate3    ; 
	m_InitAcsData[9].dwValue = m_dlgInitAcs.m_nEcgFreq     ; 
	m_InitAcsData[10].dwValue = m_dlgInitAcs.m_nEcgPreAmp    ;
	m_InitAcsData[11].dwValue = m_dlgInitAcs.m_bEnBucketSIO ;
    m_InitBuckData[0].dwValue = m_dlgInitAcs.m_nAcqTime      ;
    m_InitBuckData[1].dwValue = m_dlgInitAcs.m_nAcqMode      ;

	////////////////////////////////////////////////////

	m_bAcsInTestMode = FALSE;

	//init server socket
    if(!m_ServerSock.Create( 8080, SOCK_STREAM, NULL)){
		DbgPrt("Create Sock Error,port 8080!\r\n", g_hWndOWS);
		return TRUE;
	}
	if(!m_ServerSock.Listen(1)){
		DbgPrt("Listent sock error!\r\n", g_hWndOWS);
	    return TRUE;
	}
	DbgPrt("Wait ACS to Connet...\r\n", g_hWndOWS);

	//init comm protocal member
	m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);
	strcpy( m_protocol.acq_head.cBlockName, "E1" );
	m_protocol.acq_head.LogFlag = FALSE;
	m_protocol.acq_head.ScanClass = 2;  // 1: T scan        2:E scan
	m_protocol.acq_head.nTotalTime = 1200;  //no use
	m_protocol.acq_head.SeptaStatus = FALSE;
	m_protocol.acq_head.RodStatus = FALSE;
	m_protocol.acq_head.RodSpeed  = 3;
	m_protocol.acq_head.RingDifference = 0;
	m_protocol.acq_head.Span = 1;
	m_protocol.acq_head.BedOffset = 0.;
	m_protocol.acq_head.BedPosition = -1.;
	m_protocol.acq_head.AxialFov = 1;
	m_protocol.acq_head.ULD = 500;
	m_protocol.acq_head.LLD = 150;
	m_protocol.acq_head.ScatterEnery = 0;
	m_protocol.acq_head.DetectionMode = 0;
	m_protocol.acq_head.FrameNumber = 1;

	m_protocol.pFrame = new ACQ_FRAME_PARAM;
	m_protocol.pFrame[0].FrameClass = 2;     //stop by user
	m_protocol.pFrame[0].CountFlag = TRUE;
	m_protocol.pFrame[0].FrameTime = 0;
	m_protocol.pFrame[0].FrameCount = 0;
	m_protocol.pFrame[0].IntervalTime = 1;	

	m_protocol.gate_head.GateClass = 1;  //frame when acq
	m_protocol.gate_head.UPB = 120;
	m_protocol.gate_head.LPB = 60;
	m_protocol.gate_head.BeatSkip = 0;
	m_protocol.gate_head.PhaseOrFixed = FALSE; //gate on phase
	m_protocol.gate_head.GateStop = 0; //0:stop by user; 1:stop by event
	m_protocol.gate_head.ForwordRRNumber = 8;
	m_protocol.gate_head.ForwordRRPer = 5;
	m_protocol.gate_head.BackwordRRNumber = 8;
	m_protocol.gate_head.BackwordRRPer = 5;

	//init write thread
	DWORD dwID;
	CreateThread(NULL,0, WriteThread, NULL, 0, &dwID);

	g_hEvtStopWriteFile = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(NULL,0, StopWriteThread, NULL, 0, &dwID);

	m_bFHStart = FALSE;
	m_bAutoAcq = FALSE;
	//////////////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimOWSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimOWSDlg::OnPaint() 
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
HCURSOR CSimOWSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSimOWSDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//CDialog::OnClose();
	OnOK();

	return;
}

void CSimOWSDlg::OnOK() 
{
	// TODO: Add extra validation here
	OWS_HEAD head;

	if(m_bFHStart || m_bAutoAcq){
		AfxMessageBox("正在采集数据！");
		return;
	}

	if( m_ServerSock.IsConnected() ){

		if( AfxMessageBox("确认退出采集程序吗?", MB_OKCANCEL) != IDOK ) return;	 

		head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
		head.Cmd = PET_QUIT;
		head.cFlag[0] = 0x7e;
		head.cFlag[1] = 0x7e;
		head.cFlag[2] = 0x7e;

		m_ServerSock.SendDatToClient(&head, sizeof(head));
		
		if( m_ServerSock.IsConnected() )
			m_ServerSock.CloseClient();

		m_ServerSock.Close();
	}

	SetEvent(g_hEvtExit);
	CDialog::OnOK();
}

void CSimOWSDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_WndSD.DestroyWindow();
	m_WndEnergy.DestroyWindow();
	m_WndTime.DestroyWindow();

	for(int i=0; i<4; i++)
	{
		CloseHandle(g_hFHEvt[i]);
		if( g_hFHBuf[i] == NULL ) continue;
		GlobalFree(g_hFHBuf[i]);
	}

	SetEvent(g_hEvtExit);
	CloseHandle(g_hEvtExit);

	delete m_protocol.pFrame;
}

CWndDisp* CSimOWSDlg::GetDispWnd()
{
	switch(m_WndType){
	case dat_fhtime: return &m_WndCoinTime;
	case dat_time: return &m_WndTime;
	case dat_sd:   return &m_WndSD;
	case dat_energy: return &m_WndEnergy;
	default:  return NULL;
	}

	return NULL;
}

void CSimOWSDlg::SendInitACS()
{
	OWS_HEAD owshead;
	INITFILE_HEAD inithead;

	strcpy( inithead.cFlag, "PET_INIT");
	owshead.Cmd = PET_INITFILE;
	owshead.cFlag[0] = 0x7e;
	owshead.cFlag[1] = 0x7e;
	owshead.cFlag[2] = 0x7e;

	//PET_INIT ACS
	inithead.nInitObj = INIT_OBJ_ACS;
	owshead.dwDatLen =   sizeof(owshead) 
		               + sizeof(inithead) 
					   + sizeof(INIT_ITEM)*12;
	DbgPrt("OWS=>PET_INITFILE, obj=ACS\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&owshead, sizeof(owshead) );
	m_ServerSock.SendDatToClient(&inithead, sizeof(inithead));
	m_ServerSock.SendDatToClient(m_InitAcsData, sizeof(INIT_ITEM)*12 );

	//PET_INIT EMControl
	inithead.nInitObj = INIT_OBJ_GANTRY;
	owshead.dwDatLen =   sizeof(owshead) 
		               + sizeof(inithead) 
					   + sizeof(INIT_ITEM)*8;
	DbgPrt("OWS=>PET_INITFILE, obj=GANTRY\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&owshead, sizeof(owshead) );
	m_ServerSock.SendDatToClient(&inithead, sizeof(inithead));
	m_ServerSock.SendDatToClient(m_InitEMCData, sizeof(INIT_ITEM)*8 );

	//PET_INIT BUCKET
	inithead.nInitObj = INIT_OBJ_BUCKET_ALL;
	owshead.dwDatLen =   sizeof(owshead) 
		               + sizeof(inithead) 
					   + sizeof(INIT_ITEM)*2;
	DbgPrt("OWS=>PET_INITFILE, obj=BUCKET\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&owshead, sizeof(owshead) );
	m_ServerSock.SendDatToClient(&inithead, sizeof(inithead));
	m_ServerSock.SendDatToClient(m_InitBuckData, sizeof(INIT_ITEM)*2 );

}

void CSimOWSDlg::OnInitACS() 
{
	// TODO: Add your control notification handler code here
/*	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
*/
	if( m_bFHStart || m_bAutoAcq){
		AfxMessageBox("数据传输已经启动\r\n或前一次数据传输没有结束!");
		return;
	}

	if( m_dlgInitAcs.DoModal() != IDOK ) return;

	m_InitAcsData[0].dwValue = m_dlgInitAcs.m_nTimeInstant;
	m_InitAcsData[1].dwValue = m_dlgInitAcs.m_nTimeMin;
	m_InitAcsData[2].dwValue = m_dlgInitAcs.m_nTimeMax;
	m_InitAcsData[3].dwValue = m_dlgInitAcs.m_nCoinFmt;
	m_InitAcsData[4].dwValue = m_dlgInitAcs.m_bEnDbgBucket;
	m_InitAcsData[5].dwValue = m_dlgInitAcs.m_bEnEmc;
	m_InitAcsData[6].dwValue = m_dlgInitAcs.m_nRodRate1;
	m_InitAcsData[7].dwValue = m_dlgInitAcs.m_nRodRate2;
	m_InitAcsData[8].dwValue = m_dlgInitAcs.m_nRodRate3;
	m_InitAcsData[9].dwValue = m_dlgInitAcs.m_nEcgFreq;
	m_InitAcsData[10].dwValue = m_dlgInitAcs.m_nEcgPreAmp;
	m_InitAcsData[11].dwValue = m_dlgInitAcs.m_bEnBucketSIO;

	m_InitBuckData[0].dwValue = m_dlgInitAcs.m_nAcqTime;
	m_InitBuckData[1].dwValue = m_dlgInitAcs.m_nAcqMode;

	if( m_ServerSock.IsConnected() ) SendInitACS();
}


void CSimOWSDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	BOOL ret;
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart ){
		AfxMessageBox("数据传输已经启动\r\n或前一次数据传输没有结束!");
		return;
	}

	m_dlgFHParam.m_dlgMode = expert;
	if(m_dlgFHParam.DoModal() != IDOK) return;

	CTestParamDlg testDlg;
	if( !m_dlgFHParam.m_bFH ){
		testDlg.m_bucket = g_BucketNum%32;
		if( testDlg.DoModal() != IDOK ) return;
	}

	char szLMD[] = "LMD";
	char szLMD1[] = "PET LISTMODE文件(*.lmd)|*.lmd|All Files (*.*)|*.*||";
	char szBCK[]   = "bkt";
	char szBCK1[] = "PET BUCKET文件(*.bkt)|*.bkt|All Files (*.*)|*.*||";
	char* szDftExt, *szExt;
	char szBuckFile[512];

	if( m_dlgFHParam.m_bFH )
	{
		szDftExt = szLMD;
		szExt = szLMD1;
		szBuckFile[0] = 0;
	}
	else
	{
		szDftExt = szBCK;
		szExt = szBCK1;
		g_BucketNum = testDlg.m_bucket;
		sprintf(szBuckFile, "B%2.2d", g_BucketNum%32 );
	}

	CFileDialog fileDlg(FALSE,szDftExt,szBuckFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                szExt);
	if(fileDlg.DoModal() != IDOK ) return;

	if( !m_dlgFHParam.m_bFH ){
		g_BucketNum = testDlg.m_bucket;
		g_BucketNum++;
	}

	EnterCriticalSection(&g_lockFile);
		if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
		ret = g_saveFile.Open( fileDlg.GetPathName(), 
			                   CFile::modeCreate|CFile::modeWrite
					         );
		if( !ret )
		{
			AfxMessageBox("Create Save File Error\r\n");
			LeaveCriticalSection(&g_lockFile);
			return;
		}
		if( m_dlgFHParam.m_bFH ){
			m_WndSD.ShowWindow(SW_HIDE);
			m_WndTime.ShowWindow(SW_HIDE);
			m_WndEnergy.ShowWindow(SW_HIDE);

			if( m_dlgFHParam.m_nCoinFmt == 1){
				//coin data fmt set to Time
				DWORD dwSelectedBucket;
				dwSelectedBucket = 0xC0000000 | m_dlgFHParam.m_nCoinTimeBucket;
				g_saveFile.Write( &dwSelectedBucket, sizeof(DWORD) );
			}
		}else{
			m_WndType = (DAT_FMT)testDlg.m_DatFmt;
			switch(m_WndType){
			case dat_sd:
				 m_WndSD.ShowWindow(SW_SHOW);
				 m_WndTime.ShowWindow(SW_HIDE);
				 m_WndEnergy.ShowWindow(SW_HIDE);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndSD.ResetDat();
				 break;
			case dat_time:
				 m_WndSD.ShowWindow(SW_HIDE);
				 m_WndTime.ShowWindow(SW_SHOW);
				 m_WndEnergy.ShowWindow(SW_HIDE);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndTime.ResetDat();
				 break;
			case dat_energy:
				 m_WndSD.ShowWindow(SW_HIDE);
				 m_WndTime.ShowWindow(SW_HIDE);
				 m_WndEnergy.ShowWindow(SW_SHOW);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndEnergy.ResetDat();
				 break;
			case dat_fhtime:
				 m_WndCoinTime.ResetDat();
			default:
				m_WndSD.ShowWindow(SW_HIDE);
				m_WndTime.ShowWindow(SW_HIDE);
				m_WndEnergy.ShowWindow(SW_HIDE);
				m_WndCoinTime.ShowWindow(SW_HIDE);
				break;
			}
		}
	LeaveCriticalSection(&g_lockFile);

	if( m_dlgFHParam.m_scanmode == 3 ) //gate scan
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_GATE_HEAD);
	else
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
	head.Cmd = PET_PROTOCOL;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
	//m_protocol.acq_head.AxialFov = (float)m_dlgFHParam.m_fov;
	m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
	m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
	m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

	if( m_dlgFHParam.m_bFH ){
		if( m_dlgFHParam.m_nCoinFmt == 0)
			m_protocol.acq_head.nTotalTime = 0;	   //coin data fmt : pos
		else
			m_protocol.acq_head.nTotalTime = 0x40000000 | m_dlgFHParam.m_nCoinTimeBucket;
	}else 
		m_protocol.acq_head.nTotalTime = 0x80000000 | testDlg.m_bucket;
	g_bFH = m_dlgFHParam.m_bFH;

	m_protocol.acq_head.ScanClass = m_dlgFHParam.m_scanmode;  // 1: T scan 2:E scan
	//if(!g_bFH){
		switch(m_protocol.acq_head.ScanClass) {
		case 0:	//bank scan
		case 1:	//T scan
			m_protocol.acq_head.RodStatus = TRUE;
			break;
		case 2:	//e scan
		case 3:	//G Scan
		default:
			m_protocol.acq_head.RodStatus = FALSE;
			break;
		}  
	//}
	m_protocol.acq_head.SeptaStatus = m_dlgFHParam.m_bSeptaEnable;

	switch(m_dlgFHParam.m_stopType)
	{
	case 0:  //stop by event
		 m_protocol.pFrame[0].FrameClass = 0;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = m_dlgFHParam.m_stopEvent;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 1:  //stop by time
		 m_protocol.pFrame[0].FrameClass = 1;
		 m_protocol.pFrame[0].CountFlag = 1;  
		 m_protocol.pFrame[0].FrameTime = m_dlgFHParam.m_stopTime;  //s
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 2:  //stop by user
		 m_protocol.pFrame[0].FrameClass = 2;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = 0;
		 break;

	default: 
		 ASSERT(FALSE); 
		 return;
	}

	m_protocol.gate_head.GateClass = m_dlgFHParam.m_GateMode; //0: frame after acq; 1:frame when acq
	m_protocol.gate_head.UPB = m_dlgFHParam.m_UBP;
	m_protocol.gate_head.LPB = m_dlgFHParam.m_LBP;
	m_protocol.gate_head.BeatSkip = 10;
	m_protocol.gate_head.PhaseOrFixed = 0; //0:gate on phase; 1: gate on time;
	m_protocol.gate_head.ForwordRRNumber = m_dlgFHParam.m_Forword_RR_Num;
	m_protocol.gate_head.ForwordRRPer    = m_dlgFHParam.m_Forword_RR_Per;
	m_protocol.gate_head.BackwordRRNumber = m_dlgFHParam.m_Backword_RR_Num;
	m_protocol.gate_head.BackwordRRPer    = m_dlgFHParam.m_Backword_RR_Per;

	BYTE* pBuf, *pDat;
	pBuf = new BYTE[ head.dwDatLen ];
	memset(pBuf, 0, head.dwDatLen );
	
	pDat = pBuf/*+68*/;
	memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

	pDat += sizeof(ACQ_HEAD);
	if( m_protocol.acq_head.ScanClass == 3) //gate
		memcpy(pDat, &m_protocol.gate_head, sizeof(ACQ_GATE_HEAD) );
	else
		memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

	DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&head, sizeof(head));
	m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

	delete [] pBuf;

	m_Timer = 0;
	g_dwEvtCnt = 0;
	SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
	SetTimer(100, 1000, NULL);

	g_dwContCoinCnt = 0;
	g_dwDelayCoinCnt = 0;

	m_bFHStart = TRUE;
}


void CSimOWSDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	if( !m_bFHStart ){
		AfxMessageBox("数据传输没有启动");
		return;
	}

	m_bAutoAcq = FALSE;

	OWS_HEAD head;
	if(m_protocol.pFrame[0].FrameClass == 2 )
	{
		DbgPrt("OWS=>PET_SKIP\r\n", g_hWndOWS);
		head.Cmd = PET_SKIP;
	}
	else
	{
		DbgPrt("OWS=>PET_STOP\r\n", g_hWndOWS);
		head.Cmd = PET_STOP;
	}
	head.dwDatLen = sizeof(head);
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	m_ServerSock.SendDatToClient(&head, sizeof(head));

	char szTemp[512];
	sprintf(szTemp, "采集时间：%d 秒  事例记数：%d", m_Timer, g_dwEvtCnt);
	SetDlgItemText(IDC_TIPS_ACQ, szTemp);
	KillTimer(100);

	//m_bFHStart = FALSE;
}

DWORD WINAPI StopWriteThread(LPVOID pData)
{
	do{
		WaitForSingleObject(g_hEvtStopWriteFile, INFINITE);
		ResetEvent(g_hEvtStopWriteFile);

		EnterCriticalSection(&g_lockFile);
			if( g_saveFile.m_hFile != CFile::hFileNull)
			{
				g_saveFile.Close();
			}
		LeaveCriticalSection(&g_lockFile);
	}while(TRUE);

	return 0;
}


void CSimOWSDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_Timer++;
	char szTemp[512];
	sprintf(szTemp, "采集时间：%d 秒  事例记数：%d", m_Timer, g_dwEvtCnt);
	SetDlgItemText(IDC_TIPS_ACQ, szTemp);
}


void CSimOWSDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);

	if( m_bAcsInTestMode )
		m_menu.GetSubMenu(0)->CheckMenuItem(IDM_TESTMODE, MF_CHECKED   );
	else
		m_menu.GetSubMenu(0)->CheckMenuItem(IDM_TESTMODE, MF_UNCHECKED);
	m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CSimOWSDlg::OnTestMode() 
{
	// TODO: Add your command handler code here
	OWS_HEAD owshead;
	WORD mode;//0:acq 1:maint 2:test

	owshead.Cmd = PET_STATUS;
	owshead.cFlag[0] = 0x7e;
	owshead.cFlag[1] = 0x7e;
	owshead.cFlag[2] = 0x7e;
	owshead.dwDatLen = sizeof(owshead)+2;

	DbgPrt("OWS=>PET_STATUS\r\n", g_hWndOWS);

	if( m_bAcsInTestMode ){
		m_bAcsInTestMode = FALSE;
		mode = 0;
	}else{
		m_bAcsInTestMode = TRUE;
		mode = 1;
	}

	m_ServerSock.SendDatToClient(&owshead, sizeof(owshead) );
	m_ServerSock.SendDatToClient(&mode, sizeof(WORD));
}

//查询BED 运动状态
void CSimOWSDlg::OnQueryBedMovestatus() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryBedMoveStatus();
}

void CSimOWSDlg::OnQueryBedPos() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryBedPos();
}

void CSimOWSDlg::OnQueryBedStep() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryBedStep();
}

void CSimOWSDlg::OnQueryBedSwitchset() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryBedSwitchSet();
}

void CSimOWSDlg::OnQueryHVStatus() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryHVStatus();
}

void CSimOWSDlg::OnQueryRodStatus() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryRodStatus();
}

void CSimOWSDlg::OnQuerySeptaStatus() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QuerySeptaStatus();
}

void CSimOWSDlg::OnQueryRodRate() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryRodRate();
}

void CSimOWSDlg::OnQueryRodSwitchset() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryRodSwitchSet();
}

void CSimOWSDlg::OnQuerySeptaSwitchset() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QuerySeptaSwitchSet();
}

void CSimOWSDlg::OnQueryTbed() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryTBed();
}

void CSimOWSDlg::OnQueryTg() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryTGantry();
}

void CSimOWSDlg::OnQueryThI() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryThreshold_I();
}

void CSimOWSDlg::OnQueryThTg() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryThreshold_TG();
}

void CSimOWSDlg::OnQueryThTw() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryThreshold_TW();
}

void CSimOWSDlg::OnQueryThV() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryThreshold_HV();
}

void CSimOWSDlg::OnQueryTrod() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryTRod();
}

void CSimOWSDlg::OnQueryTsepta() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryTSepta();
}

void CSimOWSDlg::OnQueryTw() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.QueryTWaterCool();
}

//set oprate

void CSimOWSDlg::OnSetBedcoord() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CBedCoordDlg dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetBedCoord( dlg.m_coord );
	
}

void CSimOWSDlg::OnSetBedstep() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CInput1 dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetBedStep( dlg.m_wValue );
}

void CSimOWSDlg::OnSetRodrate() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CInput1 dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetRodRate( dlg.m_wValue );
}

void CSimOWSDlg::OnSetThI() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CGetThDlg dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetThreshold_I( dlg.m_uth, dlg.m_lth);
}

void CSimOWSDlg::OnSetThTg() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CGetThDlg dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetThreshold_TG( dlg.m_uth, dlg.m_lth);
}

void CSimOWSDlg::OnSetThTw() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CGetThDlg dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetThreshold_TW( dlg.m_uth, dlg.m_lth);
}

void CSimOWSDlg::OnSetThV() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CGetThDlg dlg;
	if(dlg.DoModal() != IDOK ) return;

	m_sio.SetThreshold_HV( dlg.m_uth, dlg.m_lth);
}


//bed control

void CSimOWSDlg::OnCtrlBedmovestepx() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CRelateMoveDlg dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveXStep(dlg.m_bOut);	
}

void CSimOWSDlg::OnCtrlBedmovestepy() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CRelateMoveDlg dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveXStep(dlg.m_bOut);	
}

void CSimOWSDlg::OnCtrlBedmovex() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CInput1 dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveX(dlg.m_wValue);
}

void CSimOWSDlg::OnCtrlBedmovexRelate() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CRelateMoveDlg dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveXRelate(dlg.m_len, dlg.m_bOut);
}

void CSimOWSDlg::OnCtrlBedmovey() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CInput1 dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveY(dlg.m_wValue);
}

void CSimOWSDlg::OnCtrlBedmoveyRelate() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	CRelateMoveDlg dlg;
	if( dlg.DoModal()!=IDOK ) return;

	m_sio.Control_BedMoveYRelate(dlg.m_len, dlg.m_bOut);
}

void CSimOWSDlg::OnCtrlBedstopall() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
	
	m_sio.Control_BedStopAll();
}

void CSimOWSDlg::OnCtrlBedstopx() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_BedStopX();
	
}

void CSimOWSDlg::OnCtrlBedstopy() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_BedStopY();
}


//棒源控制

void CSimOWSDlg::OnCtrlRodestop() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_RodEStop();
}

void CSimOWSDlg::OnCtrlRodin() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
	
	m_sio.Control_RodIn();
}

void CSimOWSDlg::OnCtrlRodout() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_RodOut();
	
}

void CSimOWSDlg::OnCtrlRodroll() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
	
	m_sio.Control_RodRoll();
}

void CSimOWSDlg::OnCtrlRodstop() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_RodStop();	
}


////////////////////////////////////////////////////////////////////////////////
// septa control

void CSimOWSDlg::OnCtrlSeptaestop() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_SeptaEStop();
	
}

void CSimOWSDlg::OnCtrlSeptain() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
	
	m_sio.Control_SeptaIn();
}

void CSimOWSDlg::OnCtrlSeptaout() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}
	
	m_sio.Control_SeptaOut();
}

void CSimOWSDlg::OnCtrlSeptastop() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_SeptaStop();
	
}

void CSimOWSDlg::OnCtrlHvOff() 
{
	// TODO: Add your command handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	m_sio.Control_HVON(FALSE);
}

void CSimOWSDlg::OnCtrlHvOn() 
{
	// TODO: Add your command handler code here
	m_sio.Control_HVON(TRUE);
}

void CSimOWSDlg::OnCtrlLaseroff() 
{
	// TODO: Add your command handler code here
	m_sio.Control_LaserON(FALSE);
}

void CSimOWSDlg::OnCtrlLaseron() 
{
	// TODO: Add your command handler code here
	m_sio.Control_LaserON(TRUE);
}

void CSimOWSDlg::OnCtrlMpowerOff() 
{
	// TODO: Add your command handler code here
	m_sio.Control_MainPowerON(FALSE);
}

void CSimOWSDlg::OnCtrlMpowerOn() 
{
	// TODO: Add your command handler code here
	m_sio.Control_MainPowerON(TRUE);
}

void CSimOWSDlg::OnDispGate() 
{
	// TODO: Add your control notification handler code here
	if( m_bFHStart ){
		AfxMessageBox("正在传输数据");
		return;
	}

	DWORD dat,pos,temp,datCnt=0;
	char buf2[512],buf3[512];

	CFileDialog fileDlg( TRUE,"LMD",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                 "PET LISTMODE文件(*.lmd)|*.lmd|All Files (*.*)|*.*||"
					   );
	if(fileDlg.DoModal() != IDOK ) return;

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	DbgPrt("\r\n------------- 开始分析 门控数据 --------------\r\n", g_hWndOWS);

	CFile ifile,ofile;
	ifile.Open(fileDlg.GetPathName(), CFile::modeRead);
	ofile.Open("c:\\GateOut.txt", CFile::modeCreate|CFile::modeReadWrite);

    sprintf(buf2, "%s\r\n", fileDlg.GetPathName());
	ofile.Write(buf2, strlen(buf2));

	pos = 0;   datCnt = 0;
	while(	ifile.Read(&dat,4) != NULL )
	{
		pos++;
		if( (dat&0x80000000) == 0 )	continue;
		datCnt++;

		sprintf(buf2, "Dat=%8.8x, Pos=%8.8x ", dat, pos);
	
		if( (dat&0x20000000) == 0) strcat(buf2,"有效");
		else strcat(buf2,"无效");
	
		if( (dat&0x10000000) == 0) strcat(buf2,"    帧数据");
		else strcat(buf2,"  周期数据");
	
		if( (dat&0x08000000) == 0) strcat(buf2,"  开始"); 
		else strcat(buf2,"  结束");

		temp = dat;
		sprintf(buf3, "    周期记数=%10u",((temp&0x07ffffe0)<<1));//>>5));
		strcat(buf2,buf3);

		temp = dat;
		sprintf(buf3, "    帧记数=%-2u\r\n",(temp&0x0000001f));
		strcat(buf2,buf3);
		ofile.Write(buf2, strlen(buf2));

		DbgPrt(buf2, g_hWndOWS);
	}

	ifile.Close();
	ofile.Close();
	
	if( datCnt <= 0 ) DbgPrt("没有门控数据\r\n", g_hWndOWS);
	else DbgPrt("结果保存在 C:\\GateOut.txt中\r\n", g_hWndOWS);
	DbgPrt("------------- 分析结束 门控数据 --------------\r\n\r\n", g_hWndOWS);

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CSimOWSDlg::OnDispLmd() 
{
	// TODO: Add your control notification handler code here
	if( m_bFHStart ){
		AfxMessageBox("正在传输数据");
		return;
	}

	DWORD dwCurr,datCnt,errCnt,coinCnt,gateCnt,rodCnt;
	DWORD* datBuf, datLen;

	char szTemp[512];

	CFileDialog fileDlg( TRUE,"LMD",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                 "PET LISTMODE文件(*.lmd)|*.lmd|All Files (*.*)|*.*||"
					   );
	if(fileDlg.DoModal() != IDOK ) return;

	datBuf = (DWORD*)GlobalAlloc(GMEM_FIXED, 16*1024*1024);

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	DbgPrt("\r\n------------- 开始分析 LMD文件 --------------\r\n", g_hWndOWS);

	CFile ifile,ofile;
	ifile.Open(fileDlg.GetPathName(), CFile::modeRead);
	ofile.Open("c:\\LmdOut.txt", CFile::modeCreate|CFile::modeReadWrite);

    sprintf(szTemp, "%s\r\n", fileDlg.GetPathName());
	ofile.Write(szTemp, strlen(szTemp));

	errCnt=0;	datCnt=0;
	coinCnt=0;  rodCnt=0;  gateCnt=0;
	do{
		datLen = ifile.Read(datBuf, 16*1024*1024);
		for(DWORD i=0; i<datLen/4; i++){
			datCnt++;
			dwCurr = datBuf[i]&0xC0000000;

			//00:coin dat; 01:rod dat; 10:gate dat; 11:invalid
			if(dwCurr == 0x0)        { coinCnt++; continue; }
			if(dwCurr == 0x40000000) { rodCnt++;  continue; }
			if(dwCurr == 0x80000000) { gateCnt++; continue; }
			errCnt++;

			if(errCnt>1000) continue;

			sprintf( szTemp, "Invalid Dat=%8.8x, order=%d, Offset in File=%8.8x\r\n", datBuf[i], datCnt, datCnt*4);

			if(errCnt==1000) {
				sprintf(szTemp, "ErrCnt>1000, skip write file\r\n");
			}
			ofile.Write( szTemp, strlen(szTemp) );
			DbgPrt(szTemp, g_hWndOWS);
		}
	}while( datLen > 0);

	sprintf(szTemp, "Coin Dat Cnt=%d,Rod Dat Cnt=%d,Gate Dat Cnt=%d\r\n", coinCnt, rodCnt, gateCnt);
	ofile.Write(szTemp, strlen(szTemp));
	DbgPrt(szTemp, g_hWndOWS);

	if( errCnt != 0)
		sprintf(szTemp, "datCnt=%d, errCnt=%d, Error rat=%f%%\r\n", datCnt, errCnt, float(errCnt)/datCnt*100.);
	else
		sprintf(szTemp, "datCnt=%d, No Error\r\n", datCnt);
	DbgPrt(szTemp, g_hWndOWS);
	ofile.Write(szTemp, strlen(szTemp));

	ifile.Close();
	ofile.Close();

	DbgPrt("结果保存在 C:\\LmdOut.txt中\r\n", g_hWndOWS);
	DbgPrt("------------- 分析结束 LMD文件 --------------\r\n\r\n", g_hWndOWS);

	GlobalFree( datBuf );
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CSimOWSDlg::OnDispRod() 
{
	// TODO: Add your control notification handler code here
	if( m_bFHStart ){
		AfxMessageBox("正在传输数据");
		return;
	}

	DWORD dwPrev,dwCurr,pos,rodCnt,errCnt;
	DWORD* datBuf, datLen;

	char szTemp[512];

	CFileDialog fileDlg( TRUE,"LMD",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                 "PET LISTMODE文件(*.lmd)|*.lmd|All Files (*.*)|*.*||"
					   );
	if(fileDlg.DoModal() != IDOK ) return;

	datBuf = (DWORD*)GlobalAlloc(GMEM_FIXED, 16*1024*1024);

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	DbgPrt("\r\n------------- 开始分析 棒源位置 --------------\r\n", g_hWndOWS);

	CFile ifile,ofile;
	ifile.Open(fileDlg.GetPathName(), CFile::modeRead);
	ofile.Open("c:\\RodOut.txt", CFile::modeCreate|CFile::modeReadWrite);

    sprintf(szTemp, "%s\r\n", fileDlg.GetPathName());
	ofile.Write(szTemp, strlen(szTemp));

	BOOL bFrame = FALSE,bWriteFrameSize=FALSE;
	int nFramePos=0,nFrameSize=0;
	pos = 0;  errCnt = 0;	rodCnt=0; 
	ifile.Read(&dwPrev, 4);	 
	do{
		datLen = ifile.Read(datBuf, 16*1024*1024);
		for(DWORD i=0; i<datLen/4; i++){
			pos++;
			dwCurr = datBuf[i];
			nFrameSize++;

			//if( (dwCurr&0xC0000000) != 0x40000000)	continue;
			if( ((dwCurr&0xC0000000)!=0x40000000) && (dwCurr!=0xC1000000) )	continue;
			if( dwCurr == 0xC1000000 ){
				bFrame = TRUE;
				nFramePos = pos;
				if( bWriteFrameSize ){
					sprintf(szTemp, "Frame Size=%d\r\n", nFrameSize-1);
					ofile.Write(szTemp, strlen(szTemp));
				}
				nFrameSize = 0;
				bWriteFrameSize = FALSE;
				continue;
			}
			rodCnt++;

			if( rodCnt==1 ) { dwPrev=dwCurr; continue; }

			if( (dwCurr-dwPrev) != 1 ){
				if( ! (dwPrev==0x400003ff && dwCurr==0x40000000) ){
					errCnt++;
					bWriteFrameSize = TRUE;
					if(errCnt==1000){
						sprintf(szTemp, "ErrCnt>1000, skip write file\r\n");
						DbgPrt(szTemp, g_hWndOWS);
						ofile.Write(szTemp, strlen(szTemp));
					}else if(errCnt<1000) {
						if( bFrame )
							sprintf(szTemp, "****Error, prev=%8.8x, curr=%8.8x, RodOrder=%d, file pos=%d, frame pos=%d\r\n", 
							                                 dwPrev,     dwCurr,         rodCnt,      pos,          nFramePos);
						else
							sprintf(szTemp, "----Error, prev=%8.8x, curr=%8.8x, RodOrder=%d, file pos=%d, frame pos=%d\r\n", 
							                                 dwPrev,     dwCurr,         rodCnt,      pos,          nFramePos);
						DbgPrt(szTemp,g_hWndOWS);
						ofile.Write(szTemp, strlen(szTemp));
					}
				}
			} 

			bFrame = FALSE;
			dwPrev = dwCurr;
		}
	}while( datLen > 0);

	if(rodCnt==0)
		sprintf(szTemp, "No Rod Dat\r\n");
	else if( errCnt != 0)
		sprintf(szTemp, "datCnt=%d, errCnt=%d, Error rat=%f%%\r\n", rodCnt, errCnt, float(errCnt)/rodCnt*100.);
	else
		sprintf(szTemp, "datCnt=%d, No Error\r\n", rodCnt);
	DbgPrt(szTemp, g_hWndOWS);
	ofile.Write(szTemp, strlen(szTemp));

	ifile.Close();
	ofile.Close();

	DbgPrt("结果保存在 C:\\RodOut.txt中\r\n", g_hWndOWS);
	DbgPrt("------------- 分析结束 棒源位置 --------------\r\n\r\n", g_hWndOWS);

	GlobalFree( datBuf );
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void CSimOWSDlg::OnDispBucket() 
{
	// TODO: Add your control notification handler code here
	if( m_bFHStart ){
		AfxMessageBox("正在传输数据");
		return;
	}
	
	CFileDialog fileDlg( TRUE,"bkt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                 "PET BUCKET文件(*.bkt)|*.bkt|PET Coin Time文件(*.tlmd)|*.tlmd|All Files (*.*)|*.*||"
					   );
	if(fileDlg.DoModal() != IDOK ) return;

	CTestParamDlg fmtDlg;
	if( fmtDlg.DoModal() != IDOK ) return;

	DAT_FMT datFmt = (DAT_FMT)fmtDlg.m_DatFmt;
	CWndDisp* pDispWnd;
	switch(datFmt){
		case dat_sd:
			 m_WndSD.ShowWindow(SW_SHOW);
			 m_WndTime.ShowWindow(SW_HIDE);
			 m_WndEnergy.ShowWindow(SW_HIDE);
			 m_WndCoinTime.ShowWindow(SW_HIDE);

			 m_WndSD.ResetDat();
			 pDispWnd = &m_WndSD;
			 break;
		case dat_time:
			 m_WndSD.ShowWindow(SW_HIDE);
			 m_WndTime.ShowWindow(SW_SHOW);
			 m_WndEnergy.ShowWindow(SW_HIDE);
			 m_WndCoinTime.ShowWindow(SW_HIDE);
		
			 m_WndTime.ResetDat();
			 pDispWnd = &m_WndTime;
			 break;
		case dat_energy:
			 m_WndSD.ShowWindow(SW_HIDE);
			 m_WndTime.ShowWindow(SW_HIDE);
			 m_WndEnergy.ShowWindow(SW_SHOW);
			 m_WndCoinTime.ShowWindow(SW_HIDE);

			 m_WndEnergy.ResetDat();
			 pDispWnd = &m_WndEnergy;
			 break;
		case dat_fhtime:
			 m_WndCoinTime.ShowWindow(SW_SHOW);
			 m_WndSD.ShowWindow(SW_HIDE);
			 m_WndTime.ShowWindow(SW_HIDE);
			 m_WndEnergy.ShowWindow(SW_HIDE);

			 m_WndCoinTime.ResetDat();
			 pDispWnd = &m_WndCoinTime;
			 break;
		default:
			m_WndSD.ShowWindow(SW_HIDE);
			m_WndTime.ShowWindow(SW_HIDE);
			m_WndEnergy.ShowWindow(SW_HIDE);
			m_WndCoinTime.ShowWindow(SW_HIDE);
			return;
	}

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	int len, vCnt=0;
	void* pBuf;

	if(datFmt == dat_fhtime){
		m_WndCoinTime.ShowDat( fileDlg.GetPathName().GetBuffer(512) );
	}else{
		CFile datFile;
		datFile.Open(fileDlg.GetPathName(), CFile::modeRead);

		len = datFile.GetLength();
		pBuf = new BYTE[len];
		datFile.Read( pBuf, len);

		FH_ROWDAT* pDat = (FH_ROWDAT*)pBuf;
		for(int i=0; i<len/4; i++){
			if(pDat[i].valid) vCnt++;
		}

		pDispWnd->ShowDat(pBuf, len);

		char szTemp[512];
		sprintf(szTemp, "总事例:%d, 有效事例:%d", len/4, vCnt);
		AfxMessageBox(szTemp);

		datFile.Close();
		delete [] pBuf;
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CSimOWSDlg::SendPetAllProtocolOver()
{
	OWS_HEAD owshead;

	owshead.Cmd = PET_ALLPROTOCOLSOVER;
	owshead.cFlag[0] = 0x7e;
	owshead.cFlag[1] = 0x7e;
	owshead.cFlag[2] = 0x7e;
	owshead.dwDatLen =   sizeof(owshead);

	DbgPrt("OWS=>PET_ALLPROTOCOLOVER\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&owshead, sizeof(owshead) );
}

void CSimOWSDlg::OnAutoAcqCoinTime() 
{
	// TODO: Add your control notification handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart || m_bAutoAcq ){
		AfxMessageBox("数据传输已经启动\r\n或前一次数据传输没有结束!");
		return;
	}

	if( !m_dlgInitAcs.m_bEnDbgBucket ){
		AfxMessageBox("采集符合时间谱必须 在PET_INIT中允许 调试BUCKET 选项!");
		return;
	}

	switch(m_dlgInitAcs.m_nAcqMode){
	case 3:
	case 4:
		 break;
	case 0: //energy
	case 1: //time
	case 2: //sd
	default:
		AfxMessageBox("采集符合时间谱时必须 在PET_INIT中将BUCKET采集方式设置为 COIN0或COIN1 方式");
		return;
	}

	if(m_dlgAutoAcqCointTime.DoModal() != IDOK) return;

	m_bFHStart = TRUE;
	m_bAutoAcq = TRUE;

	DWORD dwID;
	CreateThread(NULL,0, ThreadAutoAcqCoinTime, this, 0, &dwID);
}

DWORD WINAPI ThreadAutoAcqCoinTime(LPVOID pData)
{
	CSimOWSDlg* pMainDlg = (CSimOWSDlg*)pData;

	return pMainDlg->_ThreadAutoAcqCoinTime();
}

DWORD CSimOWSDlg::_ThreadAutoAcqCoinTime()
{
	int ret;
	char szFile[4096], szPath[4096], szTemp[4096];
	int beginBucket, endBucket;
	DWORD dwSelectedBucket;
	OWS_HEAD head;

	beginBucket = m_dlgAutoAcqCointTime.m_BucketBegin;
	endBucket = m_dlgAutoAcqCointTime.m_BucketEnd;

	strcpy( szPath, m_dlgAutoAcqCointTime.m_strPath );
	int i=strlen(szPath);
	if( i == 0 ){
		AfxMessageBox("Save Path Error!");
		m_bFHStart = FALSE;
		m_bAutoAcq = FALSE;
		return 0;
	}
	if( szPath[i-1] == '\\') szPath[i-1] = 0;

	i=beginBucket;
	while( (i<=endBucket) && m_bAutoAcq ) {
	
		sprintf(szTemp, "Auto Acq Bucket%2.2d Coin-Time\r\n", i);
		DbgPrt(szTemp, g_hWndOWS);

		sprintf( szFile, "%s\\cointime%2.2d.tlmd", szPath, i);

		EnterCriticalSection(&g_lockFile);
			if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
			ret = g_saveFile.Open( szFile, CFile::modeCreate|CFile::modeWrite);
			if( !ret )
			{
				AfxMessageBox("Create Save File Error\r\n");
				LeaveCriticalSection(&g_lockFile);
				m_bAutoAcq = FALSE;
				m_bFHStart = FALSE;
				//continue;
				return 0;
			}

			//coin data fmt set to Time
			dwSelectedBucket = 0xC0000000 | i;
			g_saveFile.Write( &dwSelectedBucket, sizeof(DWORD) );

		LeaveCriticalSection(&g_lockFile);

		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

		head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
		head.Cmd = PET_PROTOCOL;
		head.cFlag[0] = 0x7e;
		head.cFlag[1] = 0x7e;
		head.cFlag[2] = 0x7e;

		m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
		m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
		m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
		m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

		//coin data format set to coin-time
		m_protocol.acq_head.nTotalTime = 0x40000000 | i;
		g_bFH = TRUE; //m_dlgFHParam.m_bFH;

		m_protocol.acq_head.ScanClass = 2; //E SACN  m_DlgAutoAcqCointTime.m_scanmode;  // 1: T scan 2:E scan
		m_protocol.acq_head.RodStatus = m_dlgAutoAcqCointTime.m_bRodOut;
		m_protocol.acq_head.SeptaStatus = m_dlgAutoAcqCointTime.m_bSeptaOut;

		switch(m_dlgAutoAcqCointTime.m_bStopByTime)
		{
		case 0:  //stop by event
			 m_protocol.pFrame[0].FrameClass = 0;
			 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
			 m_protocol.pFrame[0].FrameTime = 0;
			 m_protocol.pFrame[0].FrameCount = m_dlgAutoAcqCointTime.m_dwEventCnt; //m_dlgFHParam.m_stopEvent;
			 m_protocol.pFrame[0].IntervalTime = 0;

			 m_protocol.gate_head.GateStop = m_dlgAutoAcqCointTime.m_dwEventCnt;
			 break;

		case 1:  //stop by time
			 m_protocol.pFrame[0].FrameClass = 1;
			 m_protocol.pFrame[0].CountFlag = 1;  
			 m_protocol.pFrame[0].FrameTime = m_dlgAutoAcqCointTime.m_dwStopTime; //m_dlgFHParam.m_stopTime;  //s
			 m_protocol.pFrame[0].FrameCount = 0;
			 m_protocol.pFrame[0].IntervalTime = 0;

			 m_protocol.gate_head.GateStop = m_dlgAutoAcqCointTime.m_dwEventCnt;
			 break;

		default: 
			 ASSERT(FALSE); 
			 return 0;
		}

		BYTE* pBuf, *pDat;
		pBuf = new BYTE[ head.dwDatLen ];
		memset(pBuf, 0, head.dwDatLen );
		
		pDat = pBuf/*+68*/;
		memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

		pDat += sizeof(ACQ_HEAD);
		memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

		DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
		m_ServerSock.SendDatToClient(&head, sizeof(head));
		m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

		delete [] pBuf;

		m_Timer = 0;
		g_dwEvtCnt = 0;
		SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
		SetTimer(100, 1000, NULL);

		g_dwContCoinCnt = 0;
		g_dwDelayCoinCnt = 0;

		m_bFHStart = TRUE;
		
		while( m_bFHStart ) Sleep(500);
		Sleep(1000);

		i++;
	} //end while( bucket )

	m_bAutoAcq = FALSE;
	return 0;
}

void CSimOWSDlg::OnAcqBucket() 
{
	// TODO: Add your control notification handler code here
	BOOL ret;
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart || m_bAutoAcq){
		AfxMessageBox("数据采集已经启动\r\n或前一次数据采集没有结束!");
		return;
	}

	if( !m_dlgInitAcs.m_bEnDbgBucket ){
		AfxMessageBox("采集BUCKET数据时必须 在PET_INIT中允许 调试BUCKET 选项!");
		return;
	}

	switch(m_dlgInitAcs.m_nAcqMode){
	case 0: //energy
		 m_dlgBucketParam.m_nDatFmt = 1; break;
	case 1: //time
		 m_dlgBucketParam.m_nDatFmt = 2; break;
	case 2: //sd
		 m_dlgBucketParam.m_nDatFmt = 0; break;
	default:
		AfxMessageBox("采集BUCKET数据时必须 在PET_INIT中将BUCKET采集方式设置为 能谱、散点或时方 方式");
		return;
	}

	m_dlgBucketParam.m_nBucket = g_BucketNum%32;

	if( m_dlgBucketParam.DoModal() != IDOK ) return;

	EnterCriticalSection(&g_lockFile);
		if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
		ret = g_saveFile.Open( m_dlgBucketParam.m_strFile, 
			                   CFile::modeCreate|CFile::modeWrite
					         );
		if( !ret )
		{
			AfxMessageBox("Create Save File Error\r\n");
			LeaveCriticalSection(&g_lockFile);
			return;
		}
		switch(m_dlgBucketParam.m_nDatFmt){
		case 0: //dat_sd:
			 m_WndType = dat_sd;
			 m_WndSD.ShowWindow(SW_SHOW);
			 m_WndTime.ShowWindow(SW_HIDE);
			 m_WndEnergy.ShowWindow(SW_HIDE);
			 m_WndCoinTime.ShowWindow(SW_HIDE);

			 m_WndSD.ResetDat();
			 break;
		case 2: //dat_time:	
			 m_WndType = dat_time;
			 m_WndSD.ShowWindow(SW_HIDE);
			 m_WndTime.ShowWindow(SW_SHOW);
			 m_WndEnergy.ShowWindow(SW_HIDE);
			 m_WndCoinTime.ShowWindow(SW_HIDE);

			 m_WndTime.ResetDat();
			 break;
		case 1: //dat_energy:
			 m_WndType = dat_energy;
			 m_WndSD.ShowWindow(SW_HIDE);
			 m_WndTime.ShowWindow(SW_HIDE);
			 m_WndEnergy.ShowWindow(SW_SHOW);
			 m_WndCoinTime.ShowWindow(SW_HIDE);

			 m_WndEnergy.ResetDat();
			 break;
		default:
			m_WndSD.ShowWindow(SW_HIDE);
			m_WndTime.ShowWindow(SW_HIDE);
			m_WndEnergy.ShowWindow(SW_HIDE);
			m_WndCoinTime.ShowWindow(SW_HIDE);
			break;
		}
	LeaveCriticalSection(&g_lockFile);

	g_BucketNum++;

	m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
	head.Cmd = PET_PROTOCOL;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
	//m_protocol.acq_head.AxialFov = (float)m_dlgFHParam.m_fov;
	m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
	m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
	m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

	//acq bucket 
	m_protocol.acq_head.nTotalTime = 0x80000000 | m_dlgBucketParam.m_nBucket;
	g_bFH = FALSE; //m_dlgFHParam.m_bFH;

	m_protocol.acq_head.ScanClass = 2;  // 1: T scan 2:E scan
	m_protocol.acq_head.RodStatus = m_dlgBucketParam.m_bRodOut;
	m_protocol.acq_head.SeptaStatus = m_dlgBucketParam.m_bSeptaOut;

	switch(m_dlgBucketParam.m_nStopType)
	{
	case 0:  //stop by event
		 m_protocol.pFrame[0].FrameClass = 0;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = m_dlgBucketParam.m_nStopEvent;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 1:  //stop by time
		 m_protocol.pFrame[0].FrameClass = 1;
		 m_protocol.pFrame[0].CountFlag = 1;  
		 m_protocol.pFrame[0].FrameTime = m_dlgBucketParam.m_nStopTime;  //s
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 2:  //stop by user
		 m_protocol.pFrame[0].FrameClass = 2;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = 0;
		 break;

	default: 
		 ASSERT(FALSE); 
		 return;
	}

	m_protocol.gate_head.GateClass = m_dlgFHParam.m_GateMode; //0: frame after acq; 1:frame when acq
	m_protocol.gate_head.UPB = m_dlgFHParam.m_UBP;
	m_protocol.gate_head.LPB = m_dlgFHParam.m_LBP;
	m_protocol.gate_head.BeatSkip = 10;
	m_protocol.gate_head.PhaseOrFixed = 0; //0:gate on phase; 1: gate on time;
	//m_protocol.gate_head.GateClass = 
	m_protocol.gate_head.ForwordRRNumber = m_dlgFHParam.m_Forword_RR_Num;
	m_protocol.gate_head.ForwordRRPer    = m_dlgFHParam.m_Forword_RR_Per;
	m_protocol.gate_head.BackwordRRNumber = m_dlgFHParam.m_Backword_RR_Num;
	m_protocol.gate_head.BackwordRRPer    = m_dlgFHParam.m_Backword_RR_Per;

	BYTE* pBuf, *pDat;
	pBuf = new BYTE[ head.dwDatLen ];
	memset(pBuf, 0, head.dwDatLen );
	
	pDat = pBuf/*+68*/;
	memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

	pDat += sizeof(ACQ_HEAD);
	memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

	DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&head, sizeof(head));
	m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

	delete [] pBuf;

	m_Timer = 0;
	g_dwEvtCnt = 0;
	SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
	SetTimer(100, 1000, NULL);

	g_dwContCoinCnt = 0;
	g_dwDelayCoinCnt = 0;

	m_bFHStart = TRUE;
}

void CSimOWSDlg::OnAutoAcqBucket() 
{
	// TODO: Add your control notification handler code here
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart || m_bAutoAcq ){
		AfxMessageBox("数据采集已经启动\r\n或前一次数据采集没有结束!");
		return;
	}

	if( !m_dlgInitAcs.m_bEnDbgBucket ){
		AfxMessageBox("采集BUCKET数据时必须 在PET_INIT中允许 调试BUCKET 选项!");
		return;
	}

	switch(m_dlgInitAcs.m_nAcqMode){
	case 0: //energy
		 m_dlgAutoAcqBucket.m_nDatFmt = 1; break;
	case 1: //time
		 m_dlgAutoAcqBucket.m_nDatFmt = 2; break;
	case 2: //sd
		 m_dlgAutoAcqBucket.m_nDatFmt = 0; break;
	default:
		AfxMessageBox("采集BUCKET数据时必须 在PET_INIT中将BUCKET采集方式设置为 能谱、散点或时方 方式");
		return;
	}

	if(m_dlgAutoAcqBucket.DoModal() != IDOK) return;

	m_bFHStart = TRUE;
	m_bAutoAcq = TRUE;

	DWORD dwID;
	CreateThread(NULL,0, ThreadAutoAcqBucket, this, 0, &dwID);
}

DWORD WINAPI ThreadAutoAcqBucket(LPVOID pData)
{
	CSimOWSDlg* pMainDlg = (CSimOWSDlg*)pData;

	return pMainDlg->_ThreadAutoAcqBucket();
}

DWORD CSimOWSDlg::_ThreadAutoAcqBucket()
{
	int ret;
	char szFile[4096], szPath[4096], szTemp[4096];
	int beginBucket, endBucket;

	beginBucket = m_dlgAutoAcqBucket.m_nBucketBegin;
	endBucket = m_dlgAutoAcqBucket.m_nBucketEnd;

	strcpy( szPath, m_dlgAutoAcqBucket.m_strPath );
	int i=strlen(szPath);
	if( i == 0 ){
		AfxMessageBox("Save Path Error!");
		m_bFHStart = FALSE;
		m_bAutoAcq = FALSE;
		return 0;
	}
	if( szPath[i-1] == '\\') szPath[i-1] = 0;

	i=beginBucket;
	while( (i<=endBucket) && m_bAutoAcq ) {
	
		sprintf(szTemp, "Auto Acq Bucket%2.2d\r\n", i);
		DbgPrt(szTemp, g_hWndOWS);

		sprintf( szFile, "%s\\B%2.2d.bkt", szPath, i);

		EnterCriticalSection(&g_lockFile);
			if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
			ret = g_saveFile.Open( szFile, CFile::modeCreate|CFile::modeWrite);
			if( !ret )
			{
				AfxMessageBox("Create Save File Error\r\n");
				LeaveCriticalSection(&g_lockFile);
				m_bAutoAcq = FALSE;
				m_bFHStart = FALSE;
				return 0;
			}
			switch(m_dlgAutoAcqBucket.m_nDatFmt){
			case 0: //dat_sd:
				 m_WndType = dat_sd;
				 m_WndSD.ShowWindow(SW_SHOW);
				 m_WndTime.ShowWindow(SW_HIDE);
				 m_WndEnergy.ShowWindow(SW_HIDE);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndSD.ResetDat();
				 break;
			case 2: //dat_time:
				 m_WndType = dat_time;
				 m_WndSD.ShowWindow(SW_HIDE);
				 m_WndTime.ShowWindow(SW_SHOW);
				 m_WndEnergy.ShowWindow(SW_HIDE);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndTime.ResetDat();
				 break;
			case 1: //dat_energy:
				 dat_energy;
				 m_WndSD.ShowWindow(SW_HIDE);
				 m_WndTime.ShowWindow(SW_HIDE);
				 m_WndEnergy.ShowWindow(SW_SHOW);
				 m_WndCoinTime.ShowWindow(SW_HIDE);

				 m_WndEnergy.ResetDat();
				 break;
			default:
				m_WndSD.ShowWindow(SW_HIDE);
				m_WndTime.ShowWindow(SW_HIDE);
				m_WndEnergy.ShowWindow(SW_HIDE);
				m_WndCoinTime.ShowWindow(SW_HIDE);
				break;
			}  //end switch
		LeaveCriticalSection(&g_lockFile);

		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

		OWS_HEAD head;
		head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
		head.Cmd = PET_PROTOCOL;
		head.cFlag[0] = 0x7e;
		head.cFlag[1] = 0x7e;
		head.cFlag[2] = 0x7e;

		m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
		//m_protocol.acq_head.AxialFov = (float)m_dlgFHParam.m_fov;
		m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
		m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
		m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

		//acq bucket 
		m_protocol.acq_head.nTotalTime = 0x80000000 | i;
		g_bFH = FALSE; //m_dlgFHParam.m_bFH;

		m_protocol.acq_head.ScanClass = 2;  // 1: T scan 2:E scan
		m_protocol.acq_head.RodStatus = m_dlgAutoAcqBucket.m_bRodOut;
		m_protocol.acq_head.SeptaStatus = m_dlgAutoAcqBucket.m_bSeptaOut;

		switch(m_dlgAutoAcqBucket.m_nStopType)
		{
		case 0:  //stop by event
			 m_protocol.pFrame[0].FrameClass = 0;
			 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
			 m_protocol.pFrame[0].FrameTime = 0;
			 m_protocol.pFrame[0].FrameCount = m_dlgAutoAcqBucket.m_nStopEvent;
			 m_protocol.pFrame[0].IntervalTime = 0;

			 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
			 break;

		case 1:  //stop by time
			 m_protocol.pFrame[0].FrameClass = 1;
			 m_protocol.pFrame[0].CountFlag = 1;  
			 m_protocol.pFrame[0].FrameTime = m_dlgAutoAcqBucket.m_nStopTime;  //s
			 m_protocol.pFrame[0].FrameCount = 0;
			 m_protocol.pFrame[0].IntervalTime = 0;

			 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
			 break;

		default: 
			 ASSERT(FALSE); 
			 m_bAutoAcq = FALSE;
			 return 0;
		}

		BYTE* pBuf, *pDat;
		pBuf = new BYTE[ head.dwDatLen ];
		memset(pBuf, 0, head.dwDatLen );
		
		pDat = pBuf/*+68*/;
		memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

		pDat += sizeof(ACQ_HEAD);
		memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

		DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
		m_ServerSock.SendDatToClient(&head, sizeof(head));
		m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

		delete [] pBuf;

		m_Timer = 0;
		g_dwEvtCnt = 0;
		SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
		SetTimer(100, 1000, NULL);

		g_dwContCoinCnt = 0;
		g_dwDelayCoinCnt = 0;

		m_bFHStart = TRUE;

		while(m_bFHStart) Sleep(500);
		Sleep(1000);

		i++;
	}//end while

	m_bAutoAcq = FALSE;
	return 0;
}

void CSimOWSDlg::OnAcqCoinpos() 
{
	// TODO: Add your control notification handler code here
	BOOL ret;
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart || m_bAutoAcq){
		AfxMessageBox("数据采集已经启动\r\n或前一次数据采集没有结束!");
		return;
	}

	switch(m_dlgInitAcs.m_nAcqMode){
	case 3:
	case 4:
		 break;
	case 0: //energy
	case 1: //time
	case 2: //sd
	default:
		AfxMessageBox("采集符合数据时必须 在PET_INIT中将BUCKET采集方式设置为 COIN0或COIN1 方式");
		return;
	}

	m_dlgFHParam.m_dlgMode = coin_pos;
	m_dlgFHParam.m_bFH = TRUE;
	m_dlgFHParam.m_nCoinFmt = 0;
	if(m_dlgFHParam.DoModal() != IDOK) return;

	CFileDialog fileDlg(FALSE,"lmd",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                "PET LISTMODE文件(*.lmd)|*.lmd|All Files (*.*)|*.*||");
	if(fileDlg.DoModal() != IDOK ) return;

	EnterCriticalSection(&g_lockFile);
		if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
		ret = g_saveFile.Open( fileDlg.GetPathName(), 
			                   CFile::modeCreate|CFile::modeWrite
					         );
		if( !ret )
		{
			AfxMessageBox("Create Save File Error\r\n");
			LeaveCriticalSection(&g_lockFile);
			return;
		}

		m_WndSD.ShowWindow(SW_HIDE);
		m_WndTime.ShowWindow(SW_HIDE);
		m_WndEnergy.ShowWindow(SW_HIDE);
		m_WndCoinTime.ShowWindow(SW_HIDE);
	LeaveCriticalSection(&g_lockFile);

	if( m_dlgFHParam.m_scanmode == 3 ) //gate scan
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_GATE_HEAD);
	else
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);
//	m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
	head.Cmd = PET_PROTOCOL;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
	//m_protocol.acq_head.AxialFov = (float)m_dlgFHParam.m_fov;
	m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
	m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
	m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

	m_protocol.acq_head.nTotalTime = 0;	   //coin data fmt : pos
	g_bFH = m_dlgFHParam.m_bFH;
	ASSERT( g_bFH == TRUE );

	m_protocol.acq_head.ScanClass = m_dlgFHParam.m_scanmode;  // 1: T scan 2:E scan
	switch(m_protocol.acq_head.ScanClass) {
	case 0:	//bank scan
	case 1:	//T scan
		m_protocol.acq_head.RodStatus = TRUE;
		break;
	case 2:	//e scan
	case 3:	//G Scan
	default:
		m_protocol.acq_head.RodStatus = m_dlgFHParam.m_bRodEnable;
		break;
	}  
	m_protocol.acq_head.SeptaStatus = m_dlgFHParam.m_bSeptaEnable;

	switch(m_dlgFHParam.m_stopType)
	{
	case 0:  //stop by event
		 m_protocol.pFrame[0].FrameClass = 0;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = m_dlgFHParam.m_stopEvent;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 1:  //stop by time
		 m_protocol.pFrame[0].FrameClass = 1;
		 m_protocol.pFrame[0].CountFlag = 1;  
		 m_protocol.pFrame[0].FrameTime = m_dlgFHParam.m_stopTime;  //s
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 2:  //stop by user
		 m_protocol.pFrame[0].FrameClass = 2;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = 0;
		 break;

	default: 
		 ASSERT(FALSE); 
		 return;
	}

	m_protocol.gate_head.GateClass = m_dlgFHParam.m_GateMode; //0: frame after acq; 1:frame when acq
	m_protocol.gate_head.UPB = m_dlgFHParam.m_UBP;
	m_protocol.gate_head.LPB = m_dlgFHParam.m_LBP;
	m_protocol.gate_head.BeatSkip = 10;
	m_protocol.gate_head.PhaseOrFixed = 0; //0:gate on phase; 1: gate on time;
	m_protocol.gate_head.ForwordRRNumber = m_dlgFHParam.m_Forword_RR_Num;
	m_protocol.gate_head.ForwordRRPer    = m_dlgFHParam.m_Forword_RR_Per;
	m_protocol.gate_head.BackwordRRNumber = m_dlgFHParam.m_Backword_RR_Num;
	m_protocol.gate_head.BackwordRRPer    = m_dlgFHParam.m_Backword_RR_Per;

	BYTE* pBuf, *pDat;
	pBuf = new BYTE[ head.dwDatLen ];
	memset(pBuf, 0, head.dwDatLen );
	
	pDat = pBuf/*+68*/;
	memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

	pDat += sizeof(ACQ_HEAD);
	if( m_protocol.acq_head.ScanClass == 3) //gate
		memcpy(pDat, &m_protocol.gate_head, sizeof(ACQ_GATE_HEAD) );
	else
		memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

	DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&head, sizeof(head));
	m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

	delete [] pBuf;

	m_Timer = 0;
	g_dwEvtCnt = 0;
	SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
	SetTimer(100, 1000, NULL);

	g_dwContCoinCnt = 0;
	g_dwDelayCoinCnt = 0;

	m_bFHStart = TRUE;
}

void CSimOWSDlg::OnAcqCoinTime() 
{
	// TODO: Add your control notification handler code here
	BOOL ret;
	if( !m_ServerSock.IsConnected() )
	{
		AfxMessageBox("ACS Didn't Connect!");
		return;
	}

	if( m_bFHStart || m_bAutoAcq){
		AfxMessageBox("数据采集已经启动\r\n或前一次数据采集没有结束!");
		return;
	}

	if( !m_dlgInitAcs.m_bEnDbgBucket ){
		AfxMessageBox("采集符合时间谱时必须 在PET_INIT中允许 调试BUCKET 选项!");
		return;
	}

	switch(m_dlgInitAcs.m_nAcqMode){
	case 3:
	case 4:
		 break;
	case 0: //energy
	case 1: //time
	case 2: //sd
	default:
		AfxMessageBox("采集符合数据时必须 在PET_INIT中将BUCKET采集方式设置为 COIN0或COIN1 方式");
		return;
	}

	m_dlgFHParam.m_dlgMode = coin_time;
	m_dlgFHParam.m_bFH = TRUE;
	m_dlgFHParam.m_nCoinFmt = 1;
	m_dlgFHParam.m_scanmode = 2;  //E SCAN
	if(m_dlgFHParam.DoModal() != IDOK) return;

	CFileDialog fileDlg(FALSE,"tlmd",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                "PET LISTMODE文件(*.tlmd)|*.tlmd|All Files (*.*)|*.*||");
	if(fileDlg.DoModal() != IDOK ) return;

	EnterCriticalSection(&g_lockFile);
		if(g_saveFile.m_hFile != CFile::hFileNull) g_saveFile.Close();
		ret = g_saveFile.Open( fileDlg.GetPathName(), 
			                   CFile::modeCreate|CFile::modeWrite
					         );
		if( !ret )
		{
			AfxMessageBox("Create Save File Error\r\n");
			LeaveCriticalSection(&g_lockFile);
			return;
		}
		DWORD dwSelectedBucket;
		dwSelectedBucket = 0xC0000000 | m_dlgFHParam.m_nCoinTimeBucket;
		g_saveFile.Write( &dwSelectedBucket, sizeof(DWORD) );

		m_WndSD.ShowWindow(SW_HIDE);
		m_WndTime.ShowWindow(SW_HIDE);
		m_WndEnergy.ShowWindow(SW_HIDE);
		m_WndCoinTime.ShowWindow(SW_HIDE);
	LeaveCriticalSection(&g_lockFile);

	if( m_dlgFHParam.m_scanmode == 3 ) //gate scan
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_GATE_HEAD);
	else
		m_protocol.acq_head.nBlockSize = sizeof(ACQ_HEAD) + sizeof(ACQ_FRAME_PARAM);

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) /*+ 68*/ + m_protocol.acq_head.nBlockSize;
	head.Cmd = PET_PROTOCOL;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	m_protocol.acq_head.Span = m_dlgFHParam.m_ring;
	//m_protocol.acq_head.AxialFov = (float)m_dlgFHParam.m_fov;
	m_protocol.acq_head.RingDifference = m_dlgFHParam.m_fov;
	m_protocol.acq_head.ScatterEnery = m_dlgFHParam.m_energymode;
	m_protocol.acq_head.RodSpeed = m_dlgFHParam.m_RodSpeed+1;

	m_protocol.acq_head.nTotalTime = 0x40000000 | m_dlgFHParam.m_nCoinTimeBucket;
	g_bFH = m_dlgFHParam.m_bFH;
	ASSERT( g_bFH == TRUE );

	m_protocol.acq_head.ScanClass = m_dlgFHParam.m_scanmode;  // 1: T scan 2:E scan
	ASSERT( m_protocol.acq_head.ScanClass == 2 ); //E SCAN
	m_protocol.acq_head.RodStatus = m_dlgFHParam.m_bRodEnable;
	m_protocol.acq_head.SeptaStatus = m_dlgFHParam.m_bSeptaEnable;

	switch(m_dlgFHParam.m_stopType)
	{
	case 0:  //stop by event
		 m_protocol.pFrame[0].FrameClass = 0;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = m_dlgFHParam.m_stopEvent;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 1:  //stop by time
		 m_protocol.pFrame[0].FrameClass = 1;
		 m_protocol.pFrame[0].CountFlag = 1;  
		 m_protocol.pFrame[0].FrameTime = m_dlgFHParam.m_stopTime;  //s
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = m_dlgFHParam.m_stopEvent;
		 break;

	case 2:  //stop by user
		 m_protocol.pFrame[0].FrameClass = 2;
		 m_protocol.pFrame[0].CountFlag = 1;  //绝对记数
		 m_protocol.pFrame[0].FrameTime = 0;
		 m_protocol.pFrame[0].FrameCount = 0;
		 m_protocol.pFrame[0].IntervalTime = 0;

		 m_protocol.gate_head.GateStop = 0;
		 break;

	default: 
		 ASSERT(FALSE); 
		 return;
	}

	m_protocol.gate_head.GateClass = m_dlgFHParam.m_GateMode; //0: frame after acq; 1:frame when acq
	m_protocol.gate_head.UPB = m_dlgFHParam.m_UBP;
	m_protocol.gate_head.LPB = m_dlgFHParam.m_LBP;
	m_protocol.gate_head.BeatSkip = 10;
	m_protocol.gate_head.PhaseOrFixed = 0; //0:gate on phase; 1: gate on time;
	m_protocol.gate_head.ForwordRRNumber = m_dlgFHParam.m_Forword_RR_Num;
	m_protocol.gate_head.ForwordRRPer    = m_dlgFHParam.m_Forword_RR_Per;
	m_protocol.gate_head.BackwordRRNumber = m_dlgFHParam.m_Backword_RR_Num;
	m_protocol.gate_head.BackwordRRPer    = m_dlgFHParam.m_Backword_RR_Per;

	BYTE* pBuf, *pDat;
	pBuf = new BYTE[ head.dwDatLen ];
	memset(pBuf, 0, head.dwDatLen );
	
	pDat = pBuf/*+68*/;
	memcpy(pDat, &m_protocol.acq_head, sizeof(ACQ_HEAD) );

	pDat += sizeof(ACQ_HEAD);
	if( m_protocol.acq_head.ScanClass == 3) //gate
		memcpy(pDat, &m_protocol.gate_head, sizeof(ACQ_GATE_HEAD) );
	else
		memcpy(pDat, m_protocol.pFrame, sizeof(ACQ_FRAME_PARAM) );

	DbgPrt("OWS=>PET_PROTOCOL\r\n", g_hWndOWS);
	m_ServerSock.SendDatToClient(&head, sizeof(head));
	m_ServerSock.SendDatToClient(pBuf, head.dwDatLen-sizeof(head));

	delete [] pBuf;

	m_Timer = 0;
	g_dwEvtCnt = 0;
	SetDlgItemText(IDC_TIPS_ACQ, "采集时间：0 秒  事例记数：0");
	SetTimer(100, 1000, NULL);

	g_dwContCoinCnt = 0;
	g_dwDelayCoinCnt = 0;

	m_bFHStart = TRUE;
}


int g_nPetMonitorCnt = 0;
int g_nPetSysInfoCnt = 0;
void CSimOWSDlg::ShowPetMonitor(ACQ_MONITOR *pMonitorInfo, ACQ_SYSINFO *pSysInfo)
{
	const char* szCnt = "-\\|/";
	char szTemp[512];

	if( pMonitorInfo != NULL){
		char* pFmt1 = "%c  PET_MONITOR: FrameStart=%d CurrFrame=%d FrameClass=%d FrameStartTime=%d\r\nEvtCntAll=%d EvtCntCur=%d BedMove=%d SeptaStat=%d RodStat=%d RodSpeed=%d BedPos=%.2fcm";
		sprintf(szTemp, pFmt1,
						szCnt[g_nPetMonitorCnt%4],
			            pMonitorInfo->bAcqStart, pMonitorInfo->cCurrFrameNumber, pMonitorInfo->cCurrFrameClass,	pMonitorInfo->dwCurrFrameStartTime,
						pMonitorInfo->dwMultiples, pMonitorInfo->dwNetTrues,
						pMonitorInfo->bBedMove, pMonitorInfo->Septa_Stat, pMonitorInfo->Rod_Stat, pMonitorInfo->fWobbleTilt, pMonitorInfo->fCurrBedPosition
			   );
		SetDlgItemText(IDC_TIPS_MONITOR, szTemp);
		g_nPetMonitorCnt++;
	}

	if( pSysInfo != NULL){
		char* pFmt2 = "%c  PET_SYSINFO: Bed Pos(x=%.2f y=%.2f cm)";
		sprintf(szTemp, pFmt2, szCnt[g_nPetSysInfoCnt%4], pSysInfo->fInitBed_Position, pSysInfo->fBed_Elevation);
		SetDlgItemText(IDC_TIPS_SYSINFO, szTemp);
		g_nPetSysInfoCnt++;
	}

}

