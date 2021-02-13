// robotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "robotDlg.h"
#include "Setting.h"
#include "winsock2.h" 
#include "CHORoad.h"
#include "math.h"
#include "ChosRoad1.h"

#pragma comment(lib, "ws2_32.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�û��Զ�����Ϣ
#define WM_ACCEPT					(WM_USER + 101)
#define WM_SOCKET_READ				(WM_USER + 102)
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
// CRobotDlg dialog

CRobotDlg::CRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRobotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRobotDlg)
	m_AD1 = 0;
	m_tempAD1 = 7000;
	m_AD2 = 0;
	m_tempAD2 = 7000;
	m_AD3 = 0;
	m_tempAD3 = 7000;
	m_AD4 = 0;
	m_tempAD4 = 7000;
	m_IO1 = 0;
	m_tempIO1 = 0;
	m_IO2 = 0;
	m_tempIO2 = 0;
	m_IO3 = 0;
	m_tempIO3 = 0;
	m_IO4 = 0;
	m_tempIO4 = 0;
	m_IO5 = 0;
	m_tempIO5 = 0;
	m_IO6 = 0;
	m_tempIO6 = 0;
	m_tempIO62 = 0;
	m_IO7 = 0;
	m_IO8 = 0;
	m_RevTurnPos1 = 0;
	m_RevTurnPos3 = 0;
	m_RevTurnPos5 = 0;
	m_RevTurnPos7 = 0;
	m_RevTurnPos2 = 0;
	m_RevTurnPos4 = 0;
	m_RevTurnPos6 = 0;
	m_RevTurnPos8 = 0;
    m_RevMovePos = 0;
	m_bSerialPortOpened1= FALSE;
	m_intHoldPosition = 0;
	m_tempintHoldPosition = 0;
	m_StartSensorData = false;
	m_EndSensorData = true;
	m_StartSensorData1 = false;
	m_EndSensorData1 = true;
	m_RevData = _T("");
	m_intRevData = 0;
	m_intRevData2 = 0;
	m_RevHoldPosition = 0;

	m_intRevData3 = 0;
	m_RevData1 = _T("");
	m_RevData2 = _T("");
	m_ControlMode = true;
	m_intStat = 0;
	m_intLeftIR = 0;
	m_intRightIR = 0;
	m_intlimited = 0;
	m_comipport = 6000;
	NUM =0;
	m_CountNumLimit = 0;
	//}}AFX_DATA_INIT
	Motionstate[0] = false;
	Motionstate[1] = false;
	Motionstate[2] = false;
	Motionstate[3] = false;
    IR1234         = 0;
	TempIR1234     = 20;
    IR1291         = 0;
	TempIR1291     = 20;
	IR3467         = 0;
    TempIR3467     = 20;
	countnub      = 0;

	m_Turnstate = 0;
	tempNum = 0;
	m_CountNumLimit=0;
	tempNum1 = 0;
	DeviceNumber = 0;
	CTPETMRI =5;
	tempstate =0;
	statereset =0;
	m_FronTturNum =0;
	tempIR2 = 0;
	tempIR3 = 0;
	m_Holdstate =0;
	m_TCPNUB = 0;
	TurningAngle=0;
	TempTurningAngle =0;
	MRIError=0;
	TempNumCount=0;
	m_Y1 =-327;
	m_X2 =327;
	m_Y2 =-327;
    JoyStikeControl=0;
	SonorPosition[RightSonor1].SonerPosition = 0;
	SonorPosition[RightSonor2].SonerPosition = 0;
	SonorPosition[RightSonor3].SonerPosition = 0;
	SonorPosition[RightSonor4].SonerPosition = 0;
	SonorPosition[LeftSonor1].SonerPosition  = 0;
	SonorPosition[LeftSonor2].SonerPosition  = 0;
	SonorPosition[LeftSonor3].SonerPosition  = 0;
	SonorPosition[LeftSonor4].SonerPosition  = 0;

	

	BoolSonarControl = false;
	BoolCommMoitor = false;
	BoolIRControl = false;
	SanorFRDirection = 0;
	SanorLRDirection = 0;
	DirVelocityPara = 500;
	VelocityPara90 = 500;
	VelocityPara57 = 500;
	VelocityTurnPara = 3000;
	m_intTempLeftIR = 0;
	m_intTempRightIR = 0;
	RightLimted = false;
	LeftLimted = false;

    templednub = 0;
	m_BoolResetPosition =true;
	m_BoolJOYControl = true;
    m_COMMNUB = 0;
	m_UINTSonorLimtedL1 = SanorFRLength;
	m_UINTSonorLimtedL2 = SanorRLLength;
	m_UINTSonorLimtedL3 = SanorRLLength;
	m_UINTSonorLimtedL4 = SanorFRLength;
	m_UINTSonorLimtedR1 = SanorFRLength;
	m_UINTSonorLimtedR2 = 45;
	m_UINTSonorLimtedR3 = SanorRLLength;
	m_UINTSonorLimtedR4 = SanorFRLength;
	m_UINTIRLimtedL = IRLimtedLength;
    m_UINTIRLimtedR = IRLimtedLength;

    DevicePosition.PET   = 19000;
    DevicePosition.PETCT = 25000;
    DevicePosition.CT    = 19000;

    DevicePosition.FisrtPET    = 3000;
    DevicePosition.SecondPET   = 6000;
    DevicePosition.FisrtCT     = 3000;
    DevicePosition.SecondCT    = 6000;

    DevicePosition.PETHoldPosition = 185; 
	DevicePosition.CTHoldPosition  = 250; 
	DevicePosition.TOPHoldPosition = 390; 


	m_boolFindLine   = 1;
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotDlg)

	DDX_Text(pDX, IDC_HOLDPOSITION, m_intHoldPosition);
	DDX_Text(pDX, IDC_RevData, m_RevData);
	DDX_Text(pDX, IDC_intRevData, m_intRevData);
	DDX_Text(pDX, IDC_intRevData2, m_intRevData2);
	DDX_Text(pDX, IDC_REVHOLDPOSITION, m_RevHoldPosition);
	DDX_Text(pDX, IDC_intRevData3, m_intRevData3);
	DDX_Text(pDX, IDC_RevData2, m_RevData1);
	DDX_Text(pDX, IDC_RevData3, m_RevData2);
	DDX_Text(pDX, IDC_EDIT5, m_intStat);
	DDX_Text(pDX, IDC_LeftIR, m_intLeftIR);
	DDX_Text(pDX, IDC_RightIR, m_intRightIR);//);//
	DDX_Text(pDX, IDC_BoardIO1, m_RevMovePos);//m_intlimited);///);//IDC_LimiteValue, m_intlimited);
	DDX_Text(pDX, IDC_BoardIO2, m_intlimited);
	DDX_Text(pDX, IDC_EDIT1, IR1291);
	DDX_Text(pDX, IDC_EDIT4, IR3467);
	DDX_Control(pDX, IDC_STATIC_ICON8, m_ctrlStaticIconS8);
	DDX_Control(pDX, IDC_STATIC_ICON7, m_ctrlStaticIconS7);
	DDX_Control(pDX, IDC_STATIC_ICON6, m_ctrlStaticIconS6);
	DDX_Control(pDX, IDC_STATIC_ICON5, m_ctrlStaticIconS5);
	DDX_Control(pDX, IDC_STATIC_ICON4, m_ctrlStaticIconS4);
	DDX_Control(pDX, IDC_STATIC_ICON3, m_ctrlStaticIconS3);
	DDX_Control(pDX, IDC_STATIC_ICON2, m_ctrlStaticIconS2);
	DDX_Control(pDX, IDC_STATIC_ICON1, m_ctrlStaticIconS1);
	DDX_Control(pDX, IDC_STATIC_ICON12, m_ctrlStaticIconS12);
	DDX_Control(pDX, IDC_STATIC_ICON11, m_ctrlStaticIconS11);
	DDX_Control(pDX, IDC_STATIC_ICON10, m_ctrlStaticIconS10);
	DDX_Control(pDX, IDC_STATIC_ICON9, m_ctrlStaticIconS9);
	DDX_Control(pDX, IDC_STATIC_NO1, m_ctrlStaticNO1);
	DDX_Control(pDX, IDC_STATIC_NO2, m_ctrlStaticNO2);
	DDX_Control(pDX, IDC_STATIC_NO3, m_ctrlStaticNO3);
	DDX_Control(pDX, IDC_STATIC_NO4, m_ctrlStaticNO4);
	DDX_Control(pDX, IDC_STATIC_NO6, m_ctrlStaticNO6);
	DDX_Control(pDX, IDC_STATIC_NO7, m_ctrlStaticNO7);
	DDX_Control(pDX, IDC_STATIC_NO8, m_ctrlStaticNO8);
	DDX_Control(pDX, IDC_STATIC_NO9, m_ctrlStaticNO9);
	DDX_Control(pDX, IDC_STATIC_NO10, m_ctrlStaticNO10);
	DDX_Control(pDX, IDC_STATIC_NO13, m_ctrlStaticNO13);

	DDX_Text(pDX, IDC_EDIT3, m_Turnstate);
	DDX_Text(pDX, IDC_SETNUB, DevicePosition.DelayTimes);//tempNum

	DDX_Text(pDX, IDC_LeftTopSonar, SonorPosition[LeftSonor2].SonerPosition);
	DDX_Text(pDX, IDC_RightTopSonar, SonorPosition[LeftSonor3].SonerPosition);
    DDX_Text(pDX, IDC_RightBottomSonar, SonorPosition[RightSonor3].SonerPosition);
	DDX_Text(pDX, IDC_LeftBottomSonar, SonorPosition[RightSonor4].SonerPosition);

	DDX_Text(pDX, IDC_LeftTopSonar2, SonorPosition[LeftSonor2].SonerPosition);
	DDX_Text(pDX, IDC_RightTopSonar2, SonorPosition[LeftSonor1].SonerPosition);
    DDX_Text(pDX, IDC_RightBottomSonar2, SonorPosition[RightSonor2].SonerPosition);
	DDX_Text(pDX, IDC_LeftBottomSonar2, SonorPosition[RightSonor1].SonerPosition);


	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRobotDlg, CDialog)
	//{{AFX_MSG_MAP(CRobotDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_BN_CLICKED(IDC_MOTION1STOP, OnMotion1stop)
	ON_BN_CLICKED(IDC_MOTION3STOP, OnMotion3stop)
	ON_BN_CLICKED(IDC_MOTION5STOP, OnMotion5stop)
	ON_BN_CLICKED(IDC_MOTION7STOP, OnMotion7stop)
	ON_BN_CLICKED(IDC_VPSetting, OnVPSetting)
	ON_BN_CLICKED(IDC_SOCKETOPEN, OnSocketopen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MOTIONSTOP, OnMotionstop)
	ON_BN_CLICKED(IDC_ResetZero, OnResetZero)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MOTION1MOVEV, OnMotion1movev)
	ON_BN_CLICKED(IDC_MOTION3MOVEV, OnMotion3movev)
	ON_BN_CLICKED(IDC_MOTION5MOVEV, OnMotion5movev)
	ON_BN_CLICKED(IDC_MOTION7MOVEV, OnMotion7movev)
	ON_BN_CLICKED(IDC_ControlHold, OnControlHold)
	ON_MESSAGE(WM_COMM_RXCHAR,OnComm)
	ON_BN_CLICKED(IDC_Choroad, OnChoroad)
	ON_BN_CLICKED(IDC_ResetZero3, OnTurn90)//OnResetZero3
	ON_BN_CLICKED(IDC_Front, OnFront)
	ON_BN_CLICKED(IDC_Rear, OnRear)
	ON_BN_CLICKED(IDC_Turn57, OnTurn57)
	ON_BN_CLICKED(IDC_Front2, OnFront2)
	ON_BN_CLICKED(IDC_Rear2, OnRear2)
	ON_BN_CLICKED(IDC_StopAll, OnStopAll)
	ON_BN_CLICKED(IDC_HoldDownBroad, OnHoldDownBroad)
	ON_BN_CLICKED(IDC_HoldUpBroad, OnHoldUpBroad)
	ON_BN_CLICKED(IDC_RetSetEncoder, OnReSetEncoder)
	ON_BN_CLICKED(IDC_MoveZero, OnMoveZero)
	ON_EN_CHANGE(IDC_HoldUp2, OnChangeHoldUp2)
	ON_BN_CLICKED(IDC_HoldStopBroad, OnHoldStopBroad)
	ON_BN_CLICKED(IDC_CloseLight, OnCloseLight)
	ON_BN_CLICKED(IDC_RedLight, OnRedLight)
	ON_BN_CLICKED(IDC_GreenLight, OnGreenLight)
	ON_BN_CLICKED(IDC_ResetZero2, OnResetZero2)
	ON_BN_CLICKED(IDC_MOTION1MOVEV2, OnMotion1movev2)
	ON_BN_CLICKED(IDC_MOTION3MOVEV2, OnMotion3movev2)
	ON_BN_CLICKED(IDC_MOTION5MOVEV2, OnMotion5movev2)
	ON_BN_CLICKED(IDC_MOTION7MOVEV2, OnMotion7movev2)
	ON_BN_CLICKED(IDC_PETHoldHeight, OnPETHoldHeight)
	ON_BN_CLICKED(IDC_CTHoldHeight, OnCTHoldHeight)
	ON_BN_CLICKED(IDC_MRIHoldHeight, OnMRIHoldHeight)
	ON_COMMAND(IDC_ReadHold, OnReadHold)
	ON_COMMAND(IDC_DownHoldPos, OnDownHoldPos)
	ON_COMMAND(IDC_ResetMotionPara, OnResetMotionPara)
	ON_COMMAND(IDC_MotionMoveState, OnMotionMoveState)
	ON_COMMAND(ID_RVSetting1, OnRVSetting1)
	ON_BN_CLICKED(IDC_ReZeroPosition, OnReZeroPosition)
	ON_COMMAND(IDC_ReadHoldPos, OnReadHoldPos)
	ON_BN_CLICKED(IDC_NormalTurnLeft, OnNormalTurnLeft)
	ON_BN_CLICKED(IDC_NormalTurnRight, OnNormalTurnRight)
	ON_BN_CLICKED(IDC_NormalFront, OnNormalFront)
	ON_BN_CLICKED(IDC_NormalRear, OnNormalRear)
	ON_BN_CLICKED(IDC_StopAll2, OnNormalStopAll)
	ON_BN_CLICKED(IDC_RestartAll, OnRestartAll)
	ON_COMMAND(ID_StopIR, OnStopIR)
	ON_COMMAND(ID_StartIR, OnStartIR)
	ON_BN_CLICKED(IDC_ResetZeroLeft, OnResetZeroLeft)
	ON_BN_CLICKED(IDC_ResetZeroRight, OnResetZeroRight)
	ON_COMMAND(IDC_SONORSETTINF, OnSonorsettinf)
	ON_BN_CLICKED(IDC_Turn91, OnTurn91)
	ON_BN_CLICKED(IDC_Turn90, OnTurn90New)
	ON_BN_CLICKED(IDC_MOTIONSTART, OnMoveMotionstart)
	ON_BN_CLICKED(IDC_MOTIONSTOP, OnMoveMotionstop)
	ON_COMMAND(ID_JoyStickControl1, OnJoyStickControl)
	ON_COMMAND(ID_ConnectJoyStick, ONConnectJoyStick)
    ON_COMMAND(ID_StartJoystick, OnStartJoystick)
    ON_COMMAND(ID_StopJoystick, OnStopJoystick)
    ON_COMMAND(ID_StartSonar, OnStartSonar)
    ON_COMMAND(ID_StopSonar, OnStopSonar)
    ON_COMMAND(ID_CloseCommMonitor, CloseCommMonitor)
    ON_COMMAND(ID_OpenCommMonitor, OpenCommMonitor)
	ON_MESSAGE(WM_ACCEPT, OnSocketAccept)
	ON_MESSAGE(WM_SOCKET_READ,OnSocketRead)
	ON_COMMAND(IDC_RaedEncoder, OnRaedEncoder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotDlg message handlers

BOOL CRobotDlg::OnInitDialog()
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

	for (int j = 0; j <10; j++)
	{

		if (j < MOTIONNUM)
		{
			m_longPosition[j] = 0;
		//	motorstation[j] = true;
		}
		else if (j <4)
		{
			LimtedDi[j]       = 0;
			bfen[j]           = 0;
			TempTurnVel[j]    = 0;
			TempMoveVel[j]    = 0;
			RevTurnpos[j]     = 0;
			TempRevTurnpos[j] = 0;
			RevFrontpos[j]    = 0;
			TempRevTurnPos[j] = 0;
		}
		IRDi[j] = 0;
        IR[j]   = 0;
    	m_IO[j] = 0;
	}

	m_longVelocity[0] = 53400;//500*33.3
	m_longVelocity[1] = 200;
	m_longVelocity[2] = 53400;
	m_longVelocity[3] = 200;
	m_longVelocity[4] = 30000;
	m_longVelocity[5] = 200;
	m_longVelocity[6] = 30000;
	m_longVelocity[7] = 200;

	IRLimted  = IRLimtedLength;	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
//��ʼ������ͨѶ
	OnSocketIni();
//��ʼ��ͼ��
	m_hIconRed  = AfxGetApp()->LoadIcon(IDI_ICON_RED);
	m_hIconOff	= AfxGetApp()->LoadIcon(IDI_ICON_OFF);		
//��ʼ�����������
	OnOpenSerialPort1();
	OnOpenSerialPort2();
	OnOpenSerialPort3(); 
	for (UINT k = 0; k < MOTIONNUM; k ++)
	{
	    m_pDriver[k] = NULL;
		motorstation[k] = false;
	}
	fileName1 = "C:\\motion\\CAN-ID8.net";
//��������Ϣ����λ��
	size = IDriver::GetSizeSComInfoCollection(fileName1);//�豸��Ŀ

	OnDownload();//ͨѶ������
//�������
	Sleep(300);
//�������±���¼����
//	ERRORREM = fopen(m_StrTimeName,"w+");	
	OnMoveMotionstart();
    OnTurnMotionstart();

	OnGreenLight();
	
	SetTimer(2,500,NULL);
	SetTimer(3,800,NULL);
	SetTimer(5,200,NULL);
	OnSocketopen();
	fileName2 = "D:\\RobotParameter.txt";
    OnFileRead(fileName2);	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRobotDlg::OnPaint() 
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
	OnPaintSonar();
}
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRobotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/************************************************************************/
/* ��������: �˶�ʹ�ܺ���                                               */
/* ��������: �����˶������ת����ʹ�ܺ�ֹͣʹ��                       */
/*           OnMoveMotionstart();OnTurnMotionstart() ;                  */
/*           OnMoveMotionstop(); OnMoveMotionstop() ;                   */
/*           OnDownload();���Ӹ����ֵ��                                */
/* �޸�ʱ��: 2010��1��5��                                               */
/************************************************************************/
void CRobotDlg::OnDownload() 
{
    CString str,sError;
	SCommunicationInfo m_SCommInfo[9];	

	for(UINT i = 0; i <	MOTIONNUM; i ++)
	{
		if(!IDriver::LoadSComInfoFromCollection(m_SCommInfo[i],fileName1,sError,i))
		{		
			break;
		}
		m_pDriver[i] = IDriver::DriverConnect(m_SCommInfo[i]);
		if(m_pDriver[i] == NULL)
		{
            * (motorstation +i) = false; 
			OnRedLight();
			return;			
		}
		else
		{ 
			* (motorstation +i) = true; 
			switch(i)
			{
			case 0:
             	m_ctrlStaticIconS1.SetIcon(m_hIconRed);
				break;
			case 1:
             	m_ctrlStaticIconS2.SetIcon(m_hIconRed);
				break;
			case 2:
             	m_ctrlStaticIconS3.SetIcon(m_hIconRed);
				break;
			case 3:
             	m_ctrlStaticIconS4.SetIcon(m_hIconRed);
				break;
			case 4:
             	m_ctrlStaticIconS5.SetIcon(m_hIconRed);
				break;		
			case 5:
             	m_ctrlStaticIconS6.SetIcon(m_hIconRed);
				break;
			case 6:
             	m_ctrlStaticIconS7.SetIcon(m_hIconRed);
				break;
			case 7:
             	m_ctrlStaticIconS8.SetIcon(m_hIconRed);
				break;	
			}
			OnGreenLight(); 			
		}		
	}
}
void CRobotDlg::OnMoveMotionstart()
{
	CString sError,str;
    for (UINT i=1;i< MOTIONNUM;i=i+2)
    {
		if(m_pDriver[i] == NULL)
		{
			* (motorstation +i) = false; 
			OnRedLight();
			CString strerrors="please initial NO  motor!!!"; 
			CString innt;
			innt.Format("%d",i+1);
			strerrors.Insert(18,innt);
			SetDlgItemText(IDC_MOTIONSTATIC, strerrors);
			return;
		}
		else
		{
			if(!m_pDriver[i]->SendCommand("MO=1",str,sError))		
			{
				* (motorstation +i) = false; 
				SetDlgItemText(IDC_MOTIONSTATIC,sError);
				return;	
			}
			else
			{
                * (motorstation +i) = true; 
			}
		}    
    } 
}
void CRobotDlg::OnTurnMotionstart()
{
	CString sError,str;
    for (UINT i=0;i< MOTIONNUM;i=i+2)
    {
		if(m_pDriver[i] == NULL)
		{
			* (motorstation +i) = false; 
			OnRedLight();
			CString strerrors="please initial NO  motor!!!"; 
			CString innt;
			innt.Format("%d",i+1);
			strerrors.Insert(18,innt);
			SetDlgItemText(IDC_MOTIONSTATIC, strerrors);
			return;
		}
		else
		{
			if(!m_pDriver[i]->SendCommand("MO=1",str,sError))		
			{	
				* (motorstation +i) = false;
				SetDlgItemText(IDC_MOTIONSTATIC, sError);
				return;	
			}
			else
			{
				* (motorstation +i) = true;
				switch(i)
				{		
				case 0:
					GetDlgItem(IDC_MOTION1MOVEV)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION1MOVEV2)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION1STOP)->EnableWindow(TRUE);
					break;
				case 2:
                 	GetDlgItem(IDC_MOTION3MOVEV)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION3MOVEV2)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION3STOP)->EnableWindow(TRUE);
					break;
				case 4:
	                GetDlgItem(IDC_MOTION5MOVEV)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION5MOVEV2)->EnableWindow(TRUE);
                    GetDlgItem(IDC_MOTION5STOP)->EnableWindow(TRUE);
					break;
				case 6:
					GetDlgItem(IDC_MOTION7MOVEV)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION7MOVEV2)->EnableWindow(TRUE);
					GetDlgItem(IDC_MOTION7STOP)->EnableWindow(TRUE);		
					break;
				}				
			}
		}    
    } 
}
void CRobotDlg::OnMotionstop() 
{
	for ( UINT i = 0; i < MOTIONNUM; i ++)
	{
		OnMotionStop(i);
	}
}
void CRobotDlg::OnMoveMotionstop() 
{
    for (UINT i = 0; i< MotionNum ;i++)
    {
		UINT k = (i * 2) + 1;
		OnMotionStop(k);
    }
}
void CRobotDlg::OnTurnMotionstop()
{
    for (UINT i = 0; i< MotionNum ;i++)
    {
		UINT k = i * 2;
		OnMotionStop(k);
    }
}
void CRobotDlg::OnMotionStop(UINT i) 
{
	if(m_pDriver[i] == NULL)
	{
		return;			
	}
	else
	{	
        CString str,sError;
        if (!m_pDriver[i]->SendCommand("ST", str,sError))
		{	
			* (motorstation + i) = false;
		    return; //AfxMessageBox(sError);
		}
		else
		{
			* (motorstation + i) = true;
		}
	}	
}
/*************************************************************************************************/
/* ��������: ת�������ƺ���                                                                    */
/* ��������: (ʵ�ֵ������)OnMotionTurningPosition(UINT i, long Velocity, long Position);����ͬ��λ������;     */
/*           (ʵ�ֵ������)OOnMotionTurningVelocity(UINT i, long Velocity);����ͬ���ٶ�����,������ת���ٱȲ�ͬ; */ 
/*                                                                                               */
/*           (ʵ���������)OTurnVelocityControl(int Velocity);����ͬ���ٶ�ת�򣬿���ת������ͬ�ٱȵ�Ӱ��;     */
/* ����ע��: ת��������(1)ת����λ�ÿ���,����ҲҪ�������е��ٶ�;                             */
/*                       (2)ת�����ٶȿ���,����ǰ��ת�������ٱȲ�ͬ,������ٶ���ͬ,ʵ��ת��ͬ*/
/*                       (3)ͬ���ٶȿ���ʵ��������ͬһ���ٶȣ��ĸ�ת������ͬ������;             */
/* �޸�ʱ��: 2010��1��5��                                                                         */
/**************************************************************************************************/

void CRobotDlg::OnMotionTurningPosition(UINT i, long Velocity, long Position) 
{
	if(MotorIsNULL(i))
	{
		return;			
	}
	else
	{
		CString str,sError;
        m_pDriver[i]     -> SendCommand("MO=0",str,sError);
		if(!m_pDriver[i] -> SendCommand("UM=5",str,sError))
		{
			* (motorstation + i) = false;
			return;	//AfxMessageBox(sError);		
		}
		else
		{  
			UpdateData(TRUE);		
			m_pDriver[i] -> SendCommand("MO=1",str,sError);
			CString poscommand1 = "PA=0";
			CString velcommand1 = "SP=0";
			if(Velocity > MaxTuringSpeed1 || Velocity < -MaxTuringSpeed1)//����λ�ÿ��Ƶ��ٶ�
			{
				return;
			}
			else
			{  
				velcommand1.Format("SP=%d" , Velocity);
				m_pDriver[i] -> SendCommand(velcommand1,str,sError);
			}
			if (Position > -MaxTuringPosition && Position < MaxTuringPosition)//λ�ÿ��Ƶ�λ���趨
			{
				long TempPosition = Position * 100;
				poscommand1.Format("PA=%d",TempPosition);	
			}
			else
			{
				return;
			}
			if(!m_pDriver[i]->SendCommand(poscommand1,str,sError))
			{					
				return;	//AfxMessageBox(sError);			
			}
			if(!m_pDriver[i]->SendCommand("BG",str,sError))
			{	
				* (motorstation + i) = false;
				OnMotionstop();
				OnRedLight();
				SetDlgItemText(IDC_MOTIONSTATIC,sError);
				return;//AfxMessageBox(sError);
			}
			else
			{
				* (motorstation + i) = true;
			}
		}
	}
}
void CRobotDlg::OnMotionTurningVelocity(UINT i, long Velocity) 
{
	if(MotorIsNULL(i))
	{
		return;			
	}	
	else
	{
		CString str,sError;    
		CString velcommand = "JV=0";
		if(Velocity > MaxTuringSpeed1 || Velocity < -MaxTuringSpeed1)
		{
			return;
		}
		else
		{
			 velcommand.Format("JV=%d", Velocity);
		}
		if(!m_pDriver[i]->SendCommand(velcommand, str, sError))
		{	      
			return;//AfxMessageBox(sError);
		}
		if(!m_pDriver[i]->SendCommand("BG",str,sError))
		{	
			* (motorstation + i) = false;
			OnMotionstop();
			OnRedLight();
			SetDlgItemText(IDC_MOTIONSTATIC,sError);
			return;//AfxMessageBox(sError);
		}
		else
		{
            * (motorstation + i) = true;
		}
	}
}
void CRobotDlg::TurnVelocityControl(int Velocity)
{   	
	for (int i = 0; i < MotionNum; i++)
	{  
		UINT tempNUM = 2*i;
		if(MotorIsNULL(tempNUM))
		{
			return;			
		}
		else
		{			
			CString str1,sError;    
			CString velcommand = "JV=0";
			RevTurnPosition();
			if (TempRevTurnPos[i] > -6000 && TempRevTurnPos[i] < 6000) //180��Ӧ8540 90��Ӧ4270//47.44//26.66
			{		
				if(i < 2)
				{
					if(Velocity > MaxTuringSpeed1 || Velocity < -MaxTuringSpeed1)
					{						
						return;
					}
					else
					{	
						long TempTurnData1 =(long)(Velocity*1.78);
						velcommand.Format("JV=%d",TempTurnData1);
					}						
					if(!m_pDriver[tempNUM]->SendCommand(velcommand,str1,sError))
					{							
						return;
					}							
					if(!m_pDriver[tempNUM]->SendCommand("BG",str1,sError))
					{	
						* (motorstation + tempNUM) = false;
						OnMotionstop();
						OnRedLight();
						SetDlgItemText(IDC_MOTIONSTATIC,sError);
						return;//AfxMessageBox(sError);
					}
					else
					{
						* (motorstation + tempNUM) = true;
					}					
				}
				else
				{
					if(Velocity > MaxTuringSpeed2 || Velocity < -MaxTuringSpeed2)
					{
						return;
					}
					else
					{
						velcommand.Format("JV=%d",Velocity);
					}
					if(!m_pDriver[tempNUM]->SendCommand(velcommand,str1,sError))
					{
						return;
					}
					if(!m_pDriver[tempNUM]->SendCommand("BG",str1,sError))
					{	
						* (motorstation + tempNUM) = false;
						OnMotionstop();
						OnRedLight();
						SetDlgItemText(IDC_MOTIONSTATIC,sError);
						return;//AfxMessageBox(sError);
					}
					else
					{
						* (motorstation + tempNUM) = true;
					}	
				}
			}
			else
			{
				return;
			}
		}
	}
}
//ת������ת
void CRobotDlg::OnMotion1movev() 
{
    OnMotionTurningVelocity(0, m_longVelocity[0]);
}

void CRobotDlg::OnMotion3movev() 
{
    OnMotionTurningVelocity(2, m_longVelocity[2]);
}

void CRobotDlg::OnMotion5movev() 
{
    OnMotionTurningVelocity(4, m_longVelocity[4]);
}
void CRobotDlg::OnMotion7movev() 
{
    OnMotionTurningVelocity(6, m_longVelocity[6]);
}
//ת������ת
void CRobotDlg::OnMotion1movev2() 
{
	long tempvelocity = -m_longVelocity[0];
    OnMotionTurningVelocity(0, tempvelocity);
}
void CRobotDlg::OnMotion3movev2() 
{
	long tempvelocity = -m_longVelocity[2];
    OnMotionTurningVelocity(2, tempvelocity);
}
void CRobotDlg::OnMotion5movev2() 
{
	long tempvelocity = -m_longVelocity[4];
    OnMotionTurningVelocity(4, tempvelocity);
}  
void CRobotDlg::OnMotion7movev2() 
{
	long tempvelocity = -m_longVelocity[6];
    OnMotionTurningVelocity(6, tempvelocity);
}
//ת����ֹͣ  
void CRobotDlg::OnMotion1stop() 
{
	OnMotionStop(0); //��һ��ת����ֹͣ    
}
void CRobotDlg::OnMotion3stop() 
{
	OnMotionStop(2); //�ڶ���ת����ֹͣ	
}
void CRobotDlg::OnMotion5stop() 
{
	OnMotionStop(4); //������ת����ֹͣ
}
void CRobotDlg::OnMotion7stop() 
{
	OnMotionStop(6); //���ĸ�ת����ֹͣ
}
/***********************************************************************************************/
/* ��������: ƽ��������ƺ���                                                                  */
/* ��������: (�����˶�����ٶȿ���)OnMotionMoveingVelocity(UINT i, long Velocity);             */
/*           (ʵ���������,����ͬ�ٶȷ���)MoveSameVelocityControl(int Velocity);               */
/*           (ʵ���������,���಻ͬ�ٶȷ���)MoveDiffVelocityControl(int Velocity);����ԭ��ת�� */
/*           (ʵ���������,���಻ͬ�ٶȷ���ʹ�С)OnMotionMoveing(long velocity1, long Velocity2)*/          
/*           ���ڻ���������Ƕ�ǰ������;                                                       */
/* ����ע��: (1) ����֮ǰ�ж����ӽǶ��Ƿ���ͬ;                                                 */
/*           (2) �κ�һ�����������⽫ֹͣ�����˶�;                                             */
/*           (3) �ֶ�����ʱʹ�ã�                                                              */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/
void CRobotDlg::OnMotionMoveing(long velocity1, long Velocity2)
{
	m_longVelocity[1] = m_longVelocity[5] = velocity1;
	m_longVelocity[3] = m_longVelocity[7] = Velocity2;
	UpdateData(false);
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = (i * 2) + 1;
		OnMotionMoveingVelocity(k, m_longVelocity[k]);
	}
    tempNum = 0;
	m_Turnstate = m_Turnstate+1;
}

void CRobotDlg::OnMotionMoveingNew(long velocity1, long Velocity2)
{
	m_longVelocity[1] = m_longVelocity[7] = velocity1;
	m_longVelocity[3] = m_longVelocity[5] = Velocity2;
	UpdateData(false);
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = (i * 2) + 1;
		OnMotionMoveingVelocity(k, m_longVelocity[k]);
	}
    tempNum = 0;
	m_Turnstate = m_Turnstate+1;
}
void CRobotDlg::OnMotionMoveingVelocity(UINT i, long Velocity) 
{
	if(MotorIsNULL(i))
	{
		return;			
	}
	else
	{
		CString str,sError;    
		CString velcommand = "JV=0";
		if(Velocity > MaxMovingSpeed || Velocity < -MaxMovingSpeed)
		{
			return;
		}
		else
		{
			long  tempvelocity = (long)(Velocity * 1.2);//����1.2?
			velcommand.Format("%d", tempvelocity);
			velcommand.Insert(0, "JV=");
		}
		if(!m_pDriver[i]->SendCommand(velcommand,str,sError))
		{
	      //AfxMessageBox(sError);
			OnMoveMotionstop();
			return;
		}
		if(!m_pDriver[i]->SendCommand("BG",str,sError))
		{	
			* (motorstation + i) = false;
			OnMotionstop();
			OnRedLight();
			SetDlgItemText(IDC_MOTIONSTATIC,sError);
			return;//AfxMessageBox(sError);
		}
		else
		{
			* (motorstation + i) = true;
		}
	}
}
void CRobotDlg::MoveSameVelocityControl(int Velocity)
{
    RevTurnPosition();
	if (m_RevTurnPos1 <= m_RevTurnPos3+6 && m_RevTurnPos1 >= m_RevTurnPos3-6)
	{	
		for (int i = 0; i < MotionNum; i++)
		{ 
			UINT tempNUM = 2*i+1;
			if(MotorIsNULL(tempNUM))
			{
				return;			
			}
			else
			{
	        	CString str1,sError;    
				CString velcommand = "JV=0";
      			if(Velocity > MaxMovingSpeed || Velocity < -MaxMovingSpeed)
				{
					OnMoveMotionstop();//�κ�һ�������⽫ֹͣ���е��
					return;
				}
				else
				{  
					if (tempNUM == 3 || tempNUM == 7)
					{
						velcommand.Format("JV=%d",-Velocity);
					}
					else
					{
						velcommand.Format("JV=%d",Velocity);
					}							
					if(!m_pDriver[tempNUM]->SendCommand(velcommand,str1,sError))
					{
						OnMoveMotionstop();
						return;//AfxMessageBox(sError);
					}
					if(!m_pDriver[tempNUM]->SendCommand("BG",str1,sError))
					{	
						* (motorstation + tempNUM) = false;
						OnMotionstop();
						OnRedLight();
						SetDlgItemText(IDC_MOTIONSTATIC,sError);
						return;//AfxMessageBox(sError);
					}
					else
					{
						* (motorstation + tempNUM) = true;
					}	
				}
			}		
		}
	}
	else
	{
		SetDlgItemText(IDC_STATICwwd,_T("��ȷ�����ֵ�ת���Ƿ���ͬ!"));
		AfxMessageBox("��ȷ��������ת��Ƕ�!��֤����ת����ͬ");
	}
}

void CRobotDlg::MoveSameVelocityControlNew(int Velocity)
{
 	for (int i = 0; i < MotionNum; i++)
	{ 
		UINT tempNUM = 2*i+1;
		if(MotorIsNULL(tempNUM))
		{
			return;			
		}
		else
		{
	        CString str1,sError;    
			CString velcommand = "JV=0";
      		if(Velocity > MaxMovingSpeed || Velocity < -MaxMovingSpeed)
			{
				OnMoveMotionstop();//�κ�һ�������⽫ֹͣ���е��
				return;
			}
			else
			{  
				if (tempNUM == 3 || tempNUM == 1)
				{
					velcommand.Format("JV=%d",-Velocity);
				}
				else
				{
					velcommand.Format("JV=%d",Velocity);
				}							
				if(!m_pDriver[tempNUM]->SendCommand(velcommand,str1,sError))
				{
					OnMoveMotionstop();
					return;//AfxMessageBox(sError);
				}
				if(!m_pDriver[tempNUM]->SendCommand("BG",str1,sError))
				{	
					* (motorstation + tempNUM) = false;
					OnMotionstop();
					OnRedLight();
					SetDlgItemText(IDC_MOTIONSTATIC,sError);
					return;//AfxMessageBox(sError);
				}
				else
				{
					* (motorstation + tempNUM) = true;
				}	
			}
		}		
	}
	if (Velocity < 0)
	{
		SanorLRDirection = 1;
	}
	else if(Velocity > 0)
	{
		SanorLRDirection = 2;
	}
	else
	{
		SanorLRDirection = 0;
	}

}

//��ǰ��ĺ����෴������ԭ��ת��
void CRobotDlg::MoveDiffVelocityControl(int Velocity)
{
	RevTurnPosition();
	if (m_RevTurnPos1 == -571 && m_RevTurnPos3 == 571 && m_RevTurnPos5 == 571&& m_RevTurnPos7 == -571)
	{  
		for (int i = 0; i < MotionNum; i++)
		{ 
			UINT tempNUM = 2*i+1;
			if(MotorIsNULL(tempNUM))
			{
				return;			
			}	
			CString str1,sError;    
			CString velcommand="JV=0";
       		if(Velocity > MaxMovingSpeed || Velocity < -MaxMovingSpeed)
			{
				OnMoveMotionstop();
				return;
			}
			else
			{  
				velcommand.Format("JV=%d",Velocity);
				if(!m_pDriver[tempNUM]->SendCommand(velcommand,str1,sError))
				{
					OnMoveMotionstop();
					return;
				}
				if(!m_pDriver[tempNUM]->SendCommand("BG",str1,sError))
				{	
					* (motorstation + tempNUM) = false;
					OnMotionstop();
					OnRedLight();
					SetDlgItemText(IDC_MOTIONSTATIC,sError);
					return;//AfxMessageBox(sError);
				}
				else
				{
					* (motorstation + tempNUM) = true;
				}
			}
			SanorLRDirection = 3;
			SanorFRDirection = 3;
		}
	}
	else
	{
		SetDlgItemText(IDC_STATICwwd, _T("��ȷ������ת���Ƿ�Ϊԭ��ת��״̬!"));
	}
}

/******************************************************************************/
/* ��������: TCP/IPͨѶ                                                       */
/* ��������: TCP/IPͨѶ                                                       */
/*             (1) ��ʼ��ͨѶOnSocketIni();                                   */
/*             (2) ������Ϣ��ӦOnSocketAccept(WPARAM wParam, LPARAM lParam);  */
/*		       (3) ��ȡ��Ϣ��ӦOnSocketRead(WPARAM wParam, LPARAM lParam);    */
/*		   	   (4) ͨѶЭ�����RecvDataTcp(char recvdata[], int length);      */  
/*             (5) �ش���������ϢSendSensorMSG();                             */
/* ����ע��: ��ɻ����������̨֮�������ͨѶ                                 */
/*                                                                            */
/* �޸�ʱ��: 2010��1��5��                                                     */
/******************************************************************************/
bool CRobotDlg::OnSocketIni()
{
	//socket��ʼ��
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData))
	return false;
	//��ȡ������Ϣ	
	gethostname(m_chServerInfo,sizeof(m_chServerInfo));
	m_hostent = gethostbyname(m_chServerInfo);
	(CString)m_chServerInfo = m_hostent->h_name;
    m_csServerIPAddr = inet_ntoa(*(struct in_addr*)m_hostent->h_addr_list[0]);
	SetDlgItemText(IDC_IPADDR,m_csServerIPAddr);
	return true;
}
void CRobotDlg::OnSocketopen() 
{
	if(OnSocketOpen())
	{
		return;
	}
}
bool CRobotDlg::OnSocketOpen()
{
	m_RobTcpServer = socket(AF_INET, SOCK_STREAM, 0);
	if (m_RobTcpServer == INVALID_SOCKET)
	{
		AfxMessageBox("tcp�׽��ִ���ʧ��!");
		return false;
	}
	m_RobTcpServerAddr.sin_family = AF_INET;
	m_RobTcpServerAddr.sin_addr.s_addr = inet_addr(m_csServerIPAddr);  
	m_RobTcpServerAddr.sin_port = htons(m_comipport);
	
	if (bind(m_RobTcpServer, (struct sockaddr *)&m_RobTcpServerAddr, sizeof(m_RobTcpServerAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox("tcp�׽��ְ�ʧ��!");
		return false;
	}

	if(listen(m_RobTcpServer,5)==SOCKET_ERROR)
	{
		AfxMessageBox("tcp�׽��ּ���ʧ��!");
		return false;
	}
 
    WSAAsyncSelect(m_RobTcpServer,m_hWnd,WM_ACCEPT,FD_ACCEPT);
	return true;
}
//��������
void CRobotDlg::OnSocketAccept(WPARAM wParam, LPARAM lParam)
{
	int iLength = sizeof(SOCKADDR);

	if (WSAGETSELECTERROR(lParam))
	{
		AfxMessageBox("OnSocketAccept����!");
		return;
	}
	if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
	{
		m_RobTcpClient = accept(m_RobTcpServer,(struct sockaddr *)&m_RobTcpClientAddr,&iLength);
		
		if(m_RobTcpClient == INVALID_SOCKET)
		{
			AfxMessageBox("��������Ч�׽���!");
			return;
		}	
		SetDlgItemText(IDC_CLIENTNOMAL,_T("ͨѶ----����"));
		WSAAsyncSelect(m_RobTcpClient,m_hWnd,WM_SOCKET_READ,FD_READ);
	}
	return ;
}
//��ȡ����
void CRobotDlg::OnSocketRead(WPARAM wParam, LPARAM lParam)
{
	for(int i = 0;i < MaxRecvLength;i ++)
	{
		m_chRobRecvBuff[i] = '\0';
	}
	if (WSAGETSELECTERROR(lParam))
	{
		AfxMessageBox("OnRead ��������!");
	}
	else if (WSAGETSELECTEVENT(lParam) == FD_READ)
	{
		if(wParam == m_RobTcpClient)
		{
			m_iByteFromTcpClient=recv(m_RobTcpClient, m_chRobRecvBuff, MaxRecvLength, 0);			    
			if (m_iByteFromTcpClient == SOCKET_ERROR)
			{
				SetDlgItemText(IDC_CLIENTNOMAL,_T("ͨѶ----����"));
				return ;
			}
			else if (lstrlen(m_chRobRecvBuff) != 0)
			{	
				m_COMMNUB++;
				if(m_COMMNUB  >= 1000)
				{
					m_COMMNUB = 0;
				}
               if (m_BoolJOYControl)
               {
				   	if(m_chRobRecvBuff[0] == '$')  //����ͷ�Ƿ���ȷ
					{
						if (m_chRobRecvBuff[1] == '$')
						{					
							OnMotionstop();//�Զ�����						
						}
						if (m_BoolResetPosition)
						{
						  RecvDataTcp(&m_chRobRecvBuff[1],9);	
						}
					}
					else if (m_chRobRecvBuff[0] == '#')
					{
						RecvDataTcp1(&m_chRobRecvBuff[1],9);				
					}
					else if (m_chRobRecvBuff[0] == '*')
					{					
						RecvDataTcpSensor(&m_chRobRecvBuff[1],21);
					}
               }
			   else
			   {
				   SetDlgItemText(IDC_STATICwwd, _T("�޼��ֱ�����!"));
			   }
			}			
		}
	}
	return;
}
//����ͨѶ��Ϣ 
void CRobotDlg::RecvDataTcp(char recvdata[], int length)
{
	m_CommCorrect = 0;
	unsigned char* tempchar = NULL;
    tempchar=(unsigned char*)recvdata;
	if (m_ControlData.SetByBytes(tempchar)==true)
	{
		if (JoyStikeControl==0)
		{
			m_TCPNUB++;	
			if (m_TCPNUB>=1000)
			{
				m_TCPNUB=0;
			}
			m_ControlMode = false;
			unsigned int m_AD[4];			
			for (int i=0;i<4;i++)
			{	
				m_ControlData.GetCtlAD(&m_AD[i],i);
			}
			int tempAD22=0;       
			m_AD1=(m_AD[0]-15)*(-70);   //ǰ������64*16
			tempAD22=(m_AD[1]-15)*800;  //ת�����
			m_AD2=-tempAD22;
			m_AD3=m_AD[2]/40;  
			m_AD4=(m_AD[3]-15)*(-800); 

			m_ControlData.GetCtlIO(&m_IO1,0);//����������
			m_ControlData.GetCtlIO(&m_IO2,1);//�˶��ֱ�����
			m_ControlData.GetCtlIO(&m_IO3,2);//�ֶ��Զ�
			m_ControlData.GetCtlIO(&m_IO4,3);//������ת��λ
			m_ControlData.GetCtlIO(&m_IO5,4);//�����˼�ͣ
			m_ControlData.GetCtlIO(&m_IO6,5);//ԭ��ת��
			m_ControlData.GetCtlIO(&m_IO7,6);//��弱ͣ��ť
			m_ControlData.GetCtlIO(&m_IO8,7);//����ֹͣ��ť		
			UpdateData(FALSE);	
			
			if (m_tempIO4 != m_IO4)//���Ƹ�λ//������ת��λ
			{
				m_tempIO4 = m_IO4;
				if (m_IO4 ==0)
				{
	
					if (m_IO6 == 1)
					{
					//	OnRedLight();
						if (m_AD1 == 0)
						{
							OnMotionstop();
							OnReZeroPosition();
						    TurningAngle = 0;//���ƾ�ȷת��뾶
						}
					}
				}  					 
			} 
			if (m_IO5 == 0|| m_IO8 == 1)	//�����˽���ֹͣ
			{
				OnMotionstop();
			}
			if (m_tempIO2 != m_IO2 || m_tempIO1 != m_IO1)
			{
				OnMotionstop();
                m_tempIO2 = m_IO2;
				m_tempIO1 = m_IO1;
			}
			if (m_IO1==1&&m_IO2==1)//�����˿���
			{
				ControlRobot();
			}			
			else if (m_IO1==0&&m_IO2==1)//��������뾶ת��
			{
				 NormalTurning();
			}
			else if (m_IO1==1&&m_IO2==0)//�����ܿ���
			{
				AHand2Control();//���������ܲ���			
			}	
			if (m_TCPNUB==0)//����ִ�����
			{
		//	   OnRedLight();
			}
			else
			{
		//	   OnGreenLight();
			}
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"����ÿ���̨����,��ر��ֱ�����.");
		}
	}
}
void CRobotDlg::RecvDataTcp1(char recvdata[], int length)
{
	m_CommCorrect = 0;
	unsigned char* tempchar = NULL;
    tempchar=(unsigned char*)recvdata;
	if (m_ControlData.SetByBytes(tempchar)==true)
	{
		if (JoyStikeControl==0)
		{
			m_TCPNUB++;	
			if (m_TCPNUB>=1000)
			{
				m_TCPNUB=0;
			}
			m_ControlMode = false;
			unsigned int m_AD[4];			
			for (int i=0;i<4;i++)
			{	
				m_ControlData.GetCtlAD(&m_AD[i],i);
			}
			int tempAD22=0;       
			m_AD1=m_AD[0];//���Ƹ�λ��ʽ		
			m_AD2=m_AD[1];//����������
			m_AD3=m_AD[2];//�Զ�����ѡ��·��
			m_AD4=m_AD[3];//NC

			m_ControlData.GetCtlIO(&m_IO1,0);//��������
			m_ControlData.GetCtlIO(&m_IO2,1);//�������
			m_ControlData.GetCtlIO(&m_IO3,2);//ͨѶ������
			m_ControlData.GetCtlIO(&m_IO4,3);//ͨѶ����
			m_ControlData.GetCtlIO(&m_IO5,4);//�رջ�����
			m_ControlData.GetCtlIO(&m_IO6,5);//����������
			m_ControlData.GetCtlIO(&m_IO7,6);//��ͣ����
			m_ControlData.GetCtlIO(&m_IO8,7);//ֹͣ��ť		
			UpdateData(FALSE);	
			if (m_tempAD1 != m_AD1)
			{
				m_tempAD1 = m_AD1;
				switch(m_AD1)
				{
				case 0:
					OnResetZeroLeft();
					break;
				case 1:
					OnResetZero2();
					break;
				case 2:
					OnResetZeroRight(); 
					break;
				case 3:
					OnResetZero() ;
					break;
				default:
					break;
				}
			}		
			if (m_tempAD2 != m_AD2)
			{
				m_tempAD2 = m_AD2;
				switch(m_AD2)
				{
				case 0:
					OnHoldStopBroad();
					break;
				case 1:
					OnHoldUpBroad();
					break;
				case 2:
					OnHoldDownBroad(); 
					break;
				default:
					break;
				}
			}
			if (m_tempAD3 != m_AD3)
			{
				m_tempAD3 = m_AD3;
                OnChoroadRemote();
			}
			if (m_tempIO1 != m_IO1)
			{
				m_tempIO1 = m_IO1;
				if (m_IO1)
				{
					OnStartSonar();
				}
				else
				{
					OnStopSonar();
				}
			}
			if (m_tempIO2 != m_IO2)
			{
				m_tempIO2 = m_IO2;
				if (m_IO2)
				{
					OnStartIR();
				}
				else
				{
					OnStopIR();
				}
			}
			if (m_tempIO3 != m_IO3)
			{
				m_tempIO3 = m_IO3;
				if (m_IO3)
				{
					OpenCommMonitor();
				}
				else
				{
					CloseCommMonitor();
				}
			}
			if (m_tempIO5 != m_IO5 || m_tempIO6 != m_IO6)
			{
				if (m_IO5 == 1 && m_IO6 ==0)
				{
					OnShutdown();
				}
				else if (m_IO5 == 0 && m_IO6 ==1)
				{
                    OnRestart();
				}
			}
			if(m_IO7 == 1||m_IO8 == 1)
			{
				OnStopAll();
				//OnMotionstop();
			}	
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"����ÿ���̨����,��ر��ֱ�����.");
		}
	}
}


void CRobotDlg::SendSensorMSG()
{
	unsigned char PackedData[20];
	char SendDatas[21]; 
	int i;	
    UINT tempRevPos[4];
    for (i=0;i<4;i++)
	{
		if (RevTurnpos[i] >= -250 && RevTurnpos[i] <= 250)
		{
            tempRevPos[i] = RevTurnpos[i] + 250;
			m_SensorData.SetSenData(&tempRevPos[i],i);
		}
		else
		{
			tempRevPos[i] = 0;
			m_SensorData.SetSenData(&tempRevPos[i],i);
		}
	}

    UINT tempSensor[8];
    tempSensor[0] = SonorPosition[LeftSonor4].SonerPosition;
    tempSensor[1] = SonorPosition[LeftSonor3].SonerPosition;
    tempSensor[2] = SonorPosition[RightSonor4].SonerPosition;
    tempSensor[3] = SonorPosition[RightSonor3].SonerPosition;
    tempSensor[4] = SonorPosition[RightSonor2].SonerPosition;
    tempSensor[5] = SonorPosition[RightSonor1].SonerPosition;
    tempSensor[6] = SonorPosition[LeftSonor2].SonerPosition;
    tempSensor[7] = SonorPosition[LeftSonor1].SonerPosition;

	for (i  = 0; i < 8; i++)
	{
		if (tempSensor[i] < 290)
		{
			tempSensor[i] = tempSensor[i] - 40;		
		}
		else if(tempSensor[i] >= 290)
		{
			tempSensor[i] = 250;//290-40
		}
		else
		{
            tempSensor[i] = 0;//
		}
		m_SensorData.SetSenData(&tempSensor[i],i + 4);
	}

    UINT Holdposition;
    Holdposition = (m_RevHoldPosition - 100)/2;
	m_SensorData.SetSenData(&Holdposition,12);//		4 unsigned int FS:6;
    m_SensorData.SetSenData(&LimtedDi[0],13);
    m_SensorData.SetSenData(&LimtedDi[1],14);	
    m_SensorData.SetSenData(&LimtedDi[2],15);
    m_SensorData.SetSenData(&LimtedDi[3],16);
    UINT IRDitemp[11];
	for (i = 0; i < 11; i++)
	{
		if (IRDi[i] == true)
		{
			IRDitemp[i] = 1;
		}
		else
		{
            IRDitemp[i] = 0;
		}
		
	}
    IR[0] = IRDitemp[1];
	IR[1] = IRDitemp[0];
    IR[2] = IRDitemp[3];//3
    IR[3] = IRDitemp[4];//4
	IR[4] = IRDitemp[2];
    IR[5] = IRDitemp[7];//6
    IR[6] = IRDitemp[8];//8
	IR[7] = IRDitemp[9];//7
    IR[8] = IRDitemp[5];
	IR[9] = IRDitemp[6];
	IR[10] = IRDitemp[10];
//��Ӧ��ͼ�ϵ�ֵ1-[1];2-[0];3-[2];4-[3];5-[4];6-[5];7-[7];8-[6];11-[9];22-[8]
	IR1234 = IR[0] * 8 + IR[1] * 4 + IR[2] * 2 + IR[3];
	IR1291 = IR[9] * 8 + IR[0] * 4 + IR[8] * 2 + IR[1];
	IR3467 = IR[2] * 8 + IR[3] * 4 + IR[5] * 2 + IR[7];

    m_SensorData.SetSenData(&IRDitemp[0],17);
    m_SensorData.SetSenData(&IRDitemp[1],18);	
    m_SensorData.SetSenData(&IRDitemp[10],19);
    m_SensorData.SetSenData(&IRDitemp[3],20);
    m_SensorData.SetSenData(&IRDitemp[4],21);
    m_SensorData.SetSenData(&IRDitemp[5],22);	
    m_SensorData.SetSenData(&IRDitemp[6],23);
    m_SensorData.SetSenData(&IRDitemp[7],24);
    m_SensorData.SetSenData(&IRDitemp[8],25);
    m_SensorData.SetSenData(&IRDitemp[9],26);
	m_SensorData.GetPackedCtlData(&PackedData[0]);
	SendDatas[0] = '$';
	int j;
	for (j = 0; j < 20; j++)
	{
		SendDatas[j + 1]=PackedData[j];		
	}	
    SocketSend(SendDatas,21);	
}
void CRobotDlg::SocketSend(char* str,int n)
{
	char* pSendData = NULL;
	int iSockSend = SOCKET_ERROR;
	CString str1 = _T("");

    pSendData = (char*)str;
	if(m_RobTcpClient != INVALID_SOCKET)
	{
		iSockSend = send(m_RobTcpClient,pSendData,MaxSendLength,0);
		if(iSockSend == SOCKET_ERROR)
		{
			str1 = _T("#");
			str1 += _T("��Ϣ����ʧ��!\n");		//	AfxMessageBox(str1);	
		}
	}
}
/************************************************************************/
/* ��������: Զ�̿���̨���ƻ�����ControlRobot();                        */
/*           �����ֱ����ƻ�����ControlRobotJoy();                       */
/*           �޼������ֱ�JoyControlRobot();                             */
/* ����ע��: ���ֿ��Ʒ�ʽ����                                           */
/*                                                                      */
/* �޸�ʱ��: 2010��1��5��                                               */
/************************************************************************/
void CRobotDlg::ControlRobot()
{
	if(m_IO6 == 1)//����ԭ��ת��--��������
	{
		if (m_TCPNUB>=2)
		{
			if (m_tempAD1 != m_AD1)
			{
				MoveSameVelocityControl(m_AD1);//����ת������
				m_TCPNUB=0;
				m_tempAD1 = m_AD1;
			}
			if (m_AD1>0)
			{
				if (m_tempAD2 != m_AD2)
				{
					TurnVelocityControl(-m_AD2);
                    m_tempAD2 = m_AD2;
				}
			}
			else
			{
				if (m_tempAD2 != m_AD2)
				{
					TurnVelocityControl(m_AD2);
                    m_tempAD2 = m_AD2;
				}
			}			
		}
		m_tempIO6=0;	
	}
	else//����ԭ��ת��--ԭ��ת��
	{
		if (!m_tempIO6)
		{	
			OnMotionstop();
			OnTurn57();	
		}
		if (m_TCPNUB>=4)
		{
			if (m_tempAD1 != m_AD1)
			{
				MoveDiffVelocityControl(-m_AD1);
				m_TCPNUB=0;
				m_tempAD1 = m_AD1;
			}
		}
	}
	if (m_tempIO62 != m_IO6)
	{
		if (m_tempIO62 == 0)
		{	
			OnMotionstop();		 
		}
		m_tempIO62 = m_IO6;
	}
}
//�ֱ�����
void CRobotDlg::ControlRobotJoy()
{
	if(m_IO6 == 0)//����ԭ��ת��--��������
	{
		if (m_tempAD1 != m_AD1)
		{
			MoveSameVelocityControl(m_AD1);//����ת������
			m_TCPNUB=0;
			m_tempAD1 = m_AD1;
		}
		if (m_AD1>0)
		{
			if (m_tempAD2 != m_AD2)
			{
				TurnVelocityControl(-m_AD2);
                m_tempAD2 = m_AD2;
			}
		}
		else
		{
			if (m_tempAD2 != m_AD2)
			{
				TurnVelocityControl(m_AD2);
                m_tempAD2 = m_AD2;
			}	
		}			
		m_tempIO6=0;	
	}
	else//����ԭ��ת��--ԭ��ת��
	{
		if (!m_tempIO6)
		{
			OnTurn57();	
		}
		if (m_tempAD1 != m_AD1)
		{
			MoveDiffVelocityControl(-m_AD1);
			m_TCPNUB=0;
			m_tempAD1 = m_AD1;
		}
	}
}
void CRobotDlg::JoyControlRobot()
{
	int tempAD22 = 0;  
	m_IO1 = m_IO[0];
	m_IO2 = m_IO[1];
	m_IO3 = m_IO[2];
	m_IO4 = m_IO[3];
	m_IO5 = m_IO[4];
	m_IO6 = m_IO[5];
	m_IO7 = m_IO[6];
	m_IO8 = m_IO[7];
	UpdateData(false);
	m_AD1 = (-m_Y1-327) ;   //ǰ������64*16
	m_AD2 = (m_X2-327)*10 ;  //ת�����		
	/*m_AD3 = -m_Y2/100;//DlgJoystick.m_Y2;
	if (m_tempAD3 != m_AD3)
	{
		m_tempAD3 = m_AD3;
		if (m_AD3 >6)
		{
		//	OnHoldDownBroad();
		}
		else if (m_AD3 <1)
		{
		//	OnHoldUpBroad();
		}
		else
		{
		//	OnHoldStopBroad();
		}
	}*/
//������ת��λ		
	if (m_tempIO4 != m_IO4)//���Ƹ�λ
	{
		m_tempIO4 = m_IO4;
		if (m_IO4 == 1)
		{   
			OnMotionstop();
			OnReZeroPosition();
			TurningAngle = 0;//���ƾ�ȷת��뾶
		}  					 
	}
//�����˽���ֹͣ
	if (m_IO5 == 1)
	{
		OnMotionstop();
	}
	if (m_tempIO3 != m_IO3)
	{
		m_tempIO3 = m_IO3;
		if (m_IO3)
		{
			OnReZeroPosition();
		}		
	}
//�����˿���

	if (m_IO1 == 0 && m_IO2 == 0)
	{
		ControlRobotJoy();
	}
//��������뾶ת��	
	else if (m_IO1 == 1 && m_IO2 == 0)
	{
         NormalTurning();
	}
//�����ܿ���
	else if (m_IO1 == 0 && m_IO2 == 1)
	{
		AHand2Control();//���������ܲ���			
	}
}
/***********************************************************************************************/
/* ��������: ��ȡ�������λ��                                                                  */
/* ��������: (ת����λ��)RevMovePosition();             */
/*           (���ߵ��λ��)RevTurnPosition();               */
/*           (�������ߵ��λ��)RevMoveSinglePosition();             */
/* ����ע��: (1) ����֮ǰ�ж����ӽǶ��Ƿ���ͬ;                                                 */
/*           (2) �κ�һ�����������⽫ֹͣ�����˶�;                                             */
/*           (3) �ֶ�����ʱʹ�ã�                                                              */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/
bool CRobotDlg::RevMovePosition()
{
	bool bResult = false;
	long RevFrontPosTemp[4];
    RevFrontPosTemp[0]=RevFrontPosTemp[1]=RevFrontPosTemp[2]=RevFrontPosTemp[3]=0;
	for (int i=0; i<4;i++)
	{
		UINT tempnum=i*2+1;
		if(MotorIsNULL(tempnum))
		{
			return false;			
		}
		else
		{	
			CString tempdata;
			if(m_pDriver[tempnum]->SendCommand(_T("PX"),tempdata))
			{
				TempRevTurnPos[i]  = atoi(tempdata);
				bResult = true;
			}
			else
			{
				bResult = false;
				return bResult;
			}
		}
	}
	m_RevTurnPos2=TempRevTurnPos[0];//61*14=854     //854/18=47.4444//������Ļ���ϵ��//ʵ�ʻ���Ϊ47.4444*100=4744.44
	m_RevTurnPos4=TempRevTurnPos[1];
	m_RevTurnPos6=TempRevTurnPos[2];//4.8*100=480
	m_RevTurnPos8=TempRevTurnPos[3];
    return bResult;
}
bool CRobotDlg::RevTurnPosition()
{
	bool bResult = false;
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT tempnum = i*2;
		if(MotorIsNULL(tempnum))
		{
			return false;			
		}
		else
		{	
			CString tempdata;
			if(m_pDriver[tempnum]->SendCommand(_T("PX"),tempdata))
			{
				int wwd=0;
				wwd = atoi(tempdata);
				TempRevTurnPos[i] =(int)(wwd/100);  
				bResult = true;
			}
			else
			{
				bResult = false;
				return bResult;
			}
		}		
	}
	m_RevTurnPos1 = TempRevTurnPos[0]*180/854;//61*14=854     //854/18=47.4444//������Ļ���ϵ��//ʵ�ʻ���Ϊ47.4444*100=4744.44
	m_RevTurnPos3 = TempRevTurnPos[1]*180/854;
	m_RevTurnPos5 = TempRevTurnPos[2]*180/480;//4.8*100=480
	m_RevTurnPos7 = TempRevTurnPos[3]*180/480;
	RevTurnpos[0] = m_RevTurnPos1/10;//360*100/2000             //480/18=26.6666666666
	RevTurnpos[1] = m_RevTurnPos3/10;
	RevTurnpos[2] = m_RevTurnPos5/10;
	RevTurnpos[3] = m_RevTurnPos7/10;
    return bResult;
}

bool CRobotDlg::RevMoveSinglePosition()
{
	bool bResult = false;
	long RevFrontPosTemp0;
    RevFrontPosTemp0=0;

		if(MotorIsNULL(1))
		{
			return false;			
		}
		else
		{	
			CString tempdata;
			if(m_pDriver[1]->SendCommand(_T("PX"),tempdata))
			{
				RevFrontPosTemp0  = (long)atof(tempdata);
				bResult = true;
			}
			else
			{
				bResult = false;
				return bResult;
			}
		}
	m_RevMovePos=(float)abs(RevFrontPosTemp0);//61*14=854     //854/18=47.4444//������Ļ���ϵ��//ʵ�ʻ���Ϊ47.4444*100=4744.44
	return bResult;
}
/***********************************************************************************************/
/* ��������: ת��Ѱ��λ                                                                 */
/* ��������: (��ȸ�λ)OnResetZero();
/*           (90�ȸ�λ)OnResetZero1();
/*           (36~90�ȸ�λ)OnResetZero2();   
/*           (��ȡ��λλ�ò�������)RevLimitResetZero();����Ѱ��
/*           (��ȡ��λλ��) RevLimit2();       
/* ����ע��: �˶�֮ǰ��ת��Ѱ��λ                                                     */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/

void CRobotDlg::OnResetZero() 
{
	if (RevLimitResetZero())
	{
		return;
	}
	else
	{
		int Tempvelocity1 = 17800;//500*33.3
        int Tempvelocity2 = -17800;
		int Tempvelocity3 = -10000;//500*33.3
        int Tempvelocity4 = 10000;
		for (int i=0;i<4;i++)
		{
			int tempNum = i*2;
			if(MotorIsNULL(tempNum))
			{
				return;			
			}	
			CString str,sError;
			CString velcommand1="JV=0";
			switch(i) 
			{
			case 0:
				velcommand1.Format("JV=%d",Tempvelocity1);
				break;
			case 1:
				velcommand1.Format("JV=%d",Tempvelocity2);
				break;
			case 2:
				velcommand1.Format("JV=%d",Tempvelocity3);
				break;
			case 3:
				velcommand1.Format("JV=%d",Tempvelocity4);
				break;
			default:
				break;
			}  
			if(!m_pDriver[tempNum]->SendCommand(velcommand1,str,sError))
			{		
				OnMotionstop();//		AfxMessageBox(sError);
				return;
			}
			if(!m_pDriver[tempNum]->SendCommand("BG",str,sError))
			{	
				* (motorstation + tempNum) = false;
				OnMotionstop();
				OnRedLight();
				SetDlgItemText(IDC_MOTIONSTATIC,sError);
				return;//AfxMessageBox(sError);
			}
			else
			{
				* (motorstation + tempNum) = true;
			}
			Motionstate[i]=false;
		}
		SetTimer(1,200,NULL);
	}
}
void CRobotDlg::OnResetZeroRight() 
{
	// TODO: Add your control notification handler code here
	if (RevLimitResetZero())
	{
		return;
	}
	else
	{
		int Tempvelocity1 = 17800;//500*33.3
        int Tempvelocity2 = 17800;
		int Tempvelocity3 = 10000;//500*33.3
        int Tempvelocity4 = 10000;
		for (int i=0;i<4;i++)
		{
			int tempNum = i*2;
			if(MotorIsNULL(tempNum))
			{
				return;			
			}	
			CString str,sError;
			CString velcommand1="JV=0";
			switch(i) 
			{
			case 0:
				velcommand1.Format("JV=%d",Tempvelocity1);
				break;
			case 1:
				velcommand1.Format("JV=%d",Tempvelocity2);
				break;
			case 2:
				velcommand1.Format("JV=%d",Tempvelocity3);
				break;
			case 3:
				velcommand1.Format("JV=%d",Tempvelocity4);
				break;
			default:
				break;
			}  
			if(!m_pDriver[tempNum]->SendCommand(velcommand1,str,sError))
			{		
				OnMotionstop();//		AfxMessageBox(sError);
				return;
			}
			if(!m_pDriver[tempNum]->SendCommand("BG",str,sError))
			{	
				* (motorstation + tempNum) = false;
				OnMotionstop();
				OnRedLight();
				SetDlgItemText(IDC_MOTIONSTATIC,sError);
				return;//AfxMessageBox(sError);
			}
			else
			{
				* (motorstation + tempNum) = true;
			}
			Motionstate[i]=false;
		}
		SetTimer(1,200,NULL);
	}	
}
void CRobotDlg::OnResetZeroLeft() 
{
	// TODO: Add your control notification handler code here
	if (RevLimitResetZero())
	{
		return;
	}
	else
	{
		int Tempvelocity1 = -17800;//500*33.3
        int Tempvelocity2 = -17800;
		int Tempvelocity3 = -10000;//500*33.3
        int Tempvelocity4 = -10000;
		for (int i=0;i<4;i++)
		{
			int tempNum = i*2;
			if(MotorIsNULL(tempNum))
			{
				return;			
			}	
			CString str,sError;
			CString velcommand1="JV=0";
			switch(i) 
			{
			case 0:
				velcommand1.Format("JV=%d",Tempvelocity1);
				break;
			case 1:
				velcommand1.Format("JV=%d",Tempvelocity2);
				break;
			case 2:
				velcommand1.Format("JV=%d",Tempvelocity3);
				break;
			case 3:
				velcommand1.Format("JV=%d",Tempvelocity4);
				break;
			default:
				break;
			}  
			if(!m_pDriver[tempNum]->SendCommand(velcommand1,str,sError))
			{		
				OnMotionstop();//		AfxMessageBox(sError);
				return;
			}
			if(!m_pDriver[tempNum]->SendCommand("BG",str,sError))
			{	
				* (motorstation + tempNum) = false;
				OnMotionstop();
				OnRedLight();
				SetDlgItemText(IDC_MOTIONSTATIC,sError);
				return;//AfxMessageBox(sError);
			}
			else
			{
				* (motorstation + tempNum) = true;
			}
			Motionstate[i]=false;
		}
		SetTimer(1,200,NULL);
	}	
}

void CRobotDlg::OnResetZero2() 
{
    SetTimer(6,1000,NULL);
	int temp1=950;//-57.14*854/18;
	int temp2=530;//-57.14*480/18;
	m_longPosition[0] = temp1;	
	m_longPosition[2] = temp1;	
	m_longPosition[4] = temp2;	
	m_longPosition[6] = temp2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}
}
void CRobotDlg::OnResetZero3() //36~90��
{
    SetTimer(7,1000,NULL);
	int temp1=950;//-57.14*854/18;
	int temp2=530;//-57.14*480/18;
	m_longPosition[0] = 2810;//110---//90*854/18;	
	m_longPosition[2] = -2810;//-70---//90*854/18;	
	m_longPosition[4] = -1640;//-17.8---
	m_longPosition[6] = 1640;//57.83---
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}
}
bool CRobotDlg::RevLimitResetZero()//��ȡת����λλ�ò�����
{		
	bool bResult = false;		
	CString LimtedIn1[4];
	CString str, sError;
	for (UINT i = 0; i < MotionNum; i ++)
	{
		int tempNum = i*2;
		if(MotorIsNULL(tempNum))
		{
			return false;			
		}
		if((m_pDriver[tempNum] -> SendCommand(_T("IB[16]"), LimtedIn1[i])))
		{
			int tempDi[4];
			tempDi[i]=atoi(LimtedIn1[i]);	 
			if (tempDi[i] == 0)
			{
				CString tempdata;
				if(m_pDriver[tempNum] -> SendCommand(_T("PX"),tempdata))
				{
					int tempdata1 = 0;
					tempdata1 = atoi(tempdata);
					if ((tempdata1 / 100) == 0)
					{   
						bfen[i] = true;
						continue;						
					}
					else
					{
						if (m_pDriver[tempNum]->SendCommand(_T("MO=0"),str,sError))
						{
							if (!m_pDriver[tempNum]->SendCommand(_T("PX=0"),str,sError))
							{
			            	//	AfxMessageBox(sError);						
							}
							else
							{
								m_pDriver[tempNum]->SendCommand(_T("MO=1"),str,sError);
							//	AfxMessageBox(sError);
								bfen[i]=true;
							}
						}
					}				
				}		
	      	}
			else
			{
				bfen[i]=false;
			}
		}
		if (bfen[0] ==true && bfen[1] ==true && bfen[2] ==true && bfen[3] ==true)
		{
            bResult = true;				
		}
		else
		{
			bResult = false;
		}
	}
    return bResult;
}

void CRobotDlg::RevLimit2()//��ȡת����λλ��
{	
	CString LimtedIn1[4];
	CString str, sError;
	int tempDi[4];
	tempDi[0] = tempDi[1] = tempDi[2] = tempDi[3] = 0;
	for (int i = 0; i < MotionNum; i++)
	{	
		int tempNum = i*2;
		if(MotorIsNULL(tempNum))
		{
			return;			
		}
		else
		{
			if((m_pDriver[tempNum] -> SendCommand(_T("IB[16]"),LimtedIn1[i])))
			{				
				tempDi[i] = atoi(LimtedIn1[i]);
				if (tempDi[i] == 1)
				{
					LimtedDi[i]= 1;
				}
				else
				{
					LimtedDi[i]= 0;
				}

				if (tempDi[i]==1)
				{
					switch(i)
					{
					case 0:
						m_ctrlStaticIconS9.SetIcon(m_hIconRed);
						break;
					case 1:
						m_ctrlStaticIconS10.SetIcon(m_hIconRed);
						break;
					case 2:
						m_ctrlStaticIconS11.SetIcon(m_hIconRed);
						break;
					case 3:
						m_ctrlStaticIconS12.SetIcon(m_hIconRed);
						break;
					default:
					   break;
					}									
	      		}
				else
				{
					switch(i)
					{
					case 0:
						m_ctrlStaticIconS9.SetIcon(m_hIconOff);
						break;
					case 1:
						m_ctrlStaticIconS10.SetIcon(m_hIconOff);
						break;
					case 2:
						m_ctrlStaticIconS11.SetIcon(m_hIconOff);
						break;
					case 3:
						m_ctrlStaticIconS12.SetIcon(m_hIconOff);
						break;
					default:
						break;
					}
  				}	
			}
		}
	}
}

/***********************************************************************************************/
/* ��������: ���ڳ�ʼ��                                                                        */
/* ��������: (COM9)OnOpenSerialPort1()//��೬���ͺ��⡢���崫������ָʾ�ƿ���                 */ 
/*           (COM11)OnOpenSerialPort2()//�Ҳ೬���ͺ���                                        */
/*           (COM12)OnOpenSerialPort3()//�Ҳ������ܺʹ��崫����                                */      
/* ����ע��:                                                                                   */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/

void CRobotDlg::OnOpenSerialPort1() 
{
	if(m_serialport1.InitPort(this, 3, 9600 ,'N',8,1,EV_RXFLAG | EV_RXCHAR,512))
	{
		m_serialport1.StartMonitoring();
		m_bSerialPortOpened1=TRUE;
	}
	else
	{
		m_bSerialPortOpened1=FALSE;
	}	
}
void CRobotDlg::OnOpenSerialPort2() 
{
	if(m_serialport2.InitPort(this, 4, 9600 ,'N',8,1,EV_RXFLAG | EV_RXCHAR,512))
	{
		m_serialport2.StartMonitoring();
		m_bSerialPortOpened2=TRUE;
	}
	else
	{
		m_bSerialPortOpened2=FALSE;
	}	
}
void CRobotDlg::OnOpenSerialPort3() 
{
	if(m_serialport3.InitPort(this, 2, 9600 ,'N',8,1,EV_RXFLAG | EV_RXCHAR,512))
	{
		m_serialport3.StartMonitoring();
		m_bSerialPortOpened3=TRUE;
	}
	else
	{
		m_bSerialPortOpened3=FALSE;
	}	
}
LONG CRobotDlg::OnComm(WPARAM ch, LPARAM port)
{
	UpdateData(false);
	if(port == 4)//�����ܺʹ��崫����
	{
		CString str;
		str.Format("%02X",ch);
 		if(ch == 0x2B && m_EndSensorData)
		{
			m_RevData.Empty();
			m_StartSensorData = true;
			m_EndSensorData   = false;
		}
		else if(ch == 0x0A && m_StartSensorData)
		{
			if (m_RevData.GetLength() == 10)
			{
				if (m_RevData.Left(2) == "01")//���崫����
				{
					CString m_wwd1 = m_RevData.Mid(5,1);
					CString m_wwd2 = m_RevData.Mid(7,1);
					
					//sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData2);
				//	m_intlimited = m_intRevData2;// m_intRevData*16 + m_intRevData2;
					if (!(m_intRevData2 & 0x0002))
					{
						m_intlimited = 191;
					}
					else if (m_intRevData2 & 0x0001)
					{
                        m_intlimited = 63;
					}
					else
					{
                        m_intlimited = 0;
					}
				}
				else if (m_RevData.Left(2) == "02")//�����ܿ���
				{
					CString m_wwd1 = m_RevData.Mid(3,1);
					CString m_wwd2 = m_RevData.Mid(5,1);
					CString m_wwd3 = m_RevData.Mid(7,1);

					int m_O1st, m_O2nd, m_O3rd;
					sscanf(m_wwd1, "%x", &m_O1st); 
					sscanf(m_wwd2, "%x", &m_O2nd);
					sscanf(m_wwd3, "%x", &m_O3rd);			
					m_RevHoldPosition = (int)(m_O1st*(256) + m_O2nd*16 + m_O3rd);
					UpdateData(false);
				}
				else
				{
					m_RevData.Empty();
				}				
			}
			if (m_RevData.GetLength() >= 12)
			{
				m_RevData.Empty();
			}
			m_EndSensorData   = true;
			m_StartSensorData = false;
		}
		else if(m_StartSensorData && !m_EndSensorData)
		{		
			m_RevData += str;
		}	
	}
    if(port == 2)//�Ҳ೬���ͺ���
	{	
		CString str;
		str.Format("%02X",ch);
		
		//����2007.04.13
		if(ch == 0x2B && m_EndSensorData1)
		{
			m_RevData1.Empty();
			m_StartSensorData1 = true;
			m_EndSensorData1   = false;
		}
		else if(ch == 0x0A && m_StartSensorData1)
		{
			UINT m_intRevData,m_intRevData1,m_intRevData2;
			if (m_RevData1.GetLength() == 28)
			{
				if (m_RevData1.Left(2) == "05")//�Ҳ೬��
				{
					CString m_wwd1 = m_RevData1.Mid(3,1);
					CString m_wwd2 = m_RevData1.Mid(5,1);
					CString m_wwd3 = m_RevData1.Mid(7,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[RightSonor1].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;

					m_wwd1 = m_RevData1.Mid(9,1);
					m_wwd2 = m_RevData1.Mid(11,1);
					m_wwd3 = m_RevData1.Mid(13,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);

					SonorPosition[RightSonor2].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
		            SonorFilterNubRight++;
					if (SonorFilterNubRight == MedianFilter)
					{
						SonorFilterNubRight=0;
					}					
				//	SonorPosition[RightSonor2].SonerFilterPostion[SonorFilterNubRight] = SonorPosition[RightSonor2].SonerPosition;					
				//	SonorPosition[RightSonor2].SonerPosition = OnDataMedianValue(SonorPosition[RightSonor2].SonerFilterPostion, MedianFilter);

					m_wwd1=m_RevData1.Mid(15,1);
					m_wwd2=m_RevData1.Mid(17,1);
					m_wwd3=m_RevData1.Mid(19,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					
					SonorPosition[RightSonor3].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
		  
			//		SonorPosition[RightSonor3].SonerFilterPostion[SonorFilterNubRight] = SonorPosition[RightSonor3].SonerPosition;					
				//	SonorPosition[RightSonor3].SonerPosition = OnDataMedianValue(SonorPosition[RightSonor3].SonerFilterPostion, MedianFilter);

					m_wwd1 = m_RevData1.Mid(21,1);
					m_wwd2 = m_RevData1.Mid(23,1);
					m_wwd3 = m_RevData1.Mid(25,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[RightSonor4].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
				}   
				else
				{
					m_RevData1.Empty();
				}				
			}
			else if(m_RevData1.GetLength() == 10) 
			{
	    		if (m_RevData1.Left(2) == "06")//�Ҳ೬���ͺ���
				{							
					CString m_wwd1 = m_RevData1.Mid(5,1);
					CString m_wwd2 = m_RevData1.Mid(7,1);
				
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData2);
					IrData.IRData = m_intRevData*16 + m_intRevData2;
					
					int i =0;
                    for(i = 0;i < 7;i++)   
					{
						if (IrData.IRData >> i&1)
						{
							IRDi[i] = false;
							switch(i) 
							{
							case 0:
								m_ctrlStaticNO1.SetIcon(m_hIconRed);
								break;
							case 1:
								m_ctrlStaticNO2.SetIcon(m_hIconRed);
								break;
							case 2:
//								m_ctrlStaticNO5.SetIcon(m_hIconRed);
								break;
							case 3:
								m_ctrlStaticNO4.SetIcon(m_hIconRed);
								break;
							case 4:
								m_ctrlStaticNO3.SetIcon(m_hIconRed);
								break;
							case 5:
								m_ctrlStaticNO9.SetIcon(m_hIconRed);
								break;
							case 6:
								m_ctrlStaticNO10.SetIcon(m_hIconRed);
								break;
							default:
								break;
							}							
						}
						else
						{
							IRDi[i] = true;
							switch(i) 
							{
							case 0:
								m_ctrlStaticNO1.SetIcon(m_hIconOff);
								break;
							case 1:
								m_ctrlStaticNO2.SetIcon(m_hIconOff);
								break;
							case 2:
//								m_ctrlStaticNO5.SetIcon(m_hIconOff);
								break;
							case 3:
								m_ctrlStaticNO4.SetIcon(m_hIconOff);
								break;
							case 4:
								m_ctrlStaticNO3.SetIcon(m_hIconOff);
								break;
							case 5:
								m_ctrlStaticNO9.SetIcon(m_hIconOff);
								break;
							case 6:
								m_ctrlStaticNO10.SetIcon(m_hIconOff);
								break;
							default:
								break;
							}
						}
					}
				}
				else if (m_RevData1.Left(2) == "07")//�Ҳ������λ
				{
					CString m_wwd1 = m_RevData1.Mid(3,1);
					CString m_wwd2 = m_RevData1.Mid(5,1);
					CString m_wwd3 = m_RevData1.Mid(7,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					m_intRightIR = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
					UpdateData(false);
				}	
			}
			if (m_RevData1.GetLength() >=30)
			{
				m_RevData1.Empty();
			}
			m_EndSensorData1   = true;
			m_StartSensorData1 = false;
		}
		else if(m_StartSensorData1 && !m_EndSensorData1)
		{		
			m_RevData1 += str;
		}	
	}
    if(port == 3)//��೬���ͺ���
	{
		CString str;
		str.Format("%02X",ch);

		if(ch == 0x2B && m_EndSensorData2)
		{
			m_RevData2.Empty();
			m_StartSensorData2 = true;
			m_EndSensorData2   = false;
		}
		else if(ch == 0x0A && m_StartSensorData2)
		{
			UINT m_intRevData,m_intRevData1,m_intRevData2;
			if (m_RevData2.GetLength() == 28)
			{
				if (m_RevData2.Left(2) == "05")//��೬��
				{
					CString m_wwd1 = m_RevData2.Mid(3,1);
					CString m_wwd2 = m_RevData2.Mid(5,1);
					CString m_wwd3 = m_RevData2.Mid(7,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[LeftSonor1].SonerPosition =	m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
		            SonorFilterNubLeft++;
					if (SonorFilterNubLeft == MedianFilter)
					{
						SonorFilterNubLeft = 0;
					}  
					
             //       SonorPosition[LeftSonor1].SonerFilterPostion[SonorFilterNubLeft] = SonorPosition[LeftSonor1].SonerPosition; 
				//
					SonorPosition[LeftSonor1].SonerPosition  = OnDataMedianValue(SonorPosition[LeftSonor1].SonerFilterPostion, MedianFilter);

					m_wwd1=m_RevData2.Mid(9,1);
					m_wwd2=m_RevData2.Mid(11,1);
					m_wwd3=m_RevData2.Mid(13,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[LeftSonor2].SonerPosition=m_intRevData*256+m_intRevData1*16+m_intRevData2;

					m_wwd1=m_RevData2.Mid(15,1);
					m_wwd2=m_RevData2.Mid(17,1);
					m_wwd3=m_RevData2.Mid(19,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[LeftSonor3].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
	
//                    SonorPosition[LeftSonor3].SonerFilterPostion[SonorFilterNubLeft] = SonorPosition[LeftSonor3].SonerPosition; 
//					SonorPosition[LeftSonor3].SonerPosition  = OnDataMedianValue(SonorPosition[LeftSonor3].SonerFilterPostion, MedianFilter);
					m_wwd1 = m_RevData2.Mid(21,1);
					m_wwd2 = m_RevData2.Mid(23,1);
					m_wwd3 = m_RevData2.Mid(25,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					SonorPosition[LeftSonor4].SonerPosition = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
				} 
				else
				{
					m_RevData2.Empty();
				}				
			}
			else if(m_RevData2.GetLength() == 10) 
			{
	    		if (m_RevData2.Left(2) == "06")//������
				{
					CString m_wwd2 = m_RevData2.Mid(7,1);
				
					sscanf(m_wwd2, "%x", &m_intRevData3);
					IrData2.IRData2 = m_intRevData3;
			
                    for(int i = 0;i < 4;i++)   
					{
						
						if (IrData2.IRData2 >> i&1)
						{							
							switch(i) 
							{
							case 0:
								m_ctrlStaticNO6.SetIcon(m_hIconRed);
								IRDi[7] = false;
								break;
							case 1:
								m_ctrlStaticNO13.SetIcon(m_hIconRed);
								IRDi[10] = false;
								break;
							case 2:
								m_ctrlStaticNO7.SetIcon(m_hIconRed);
								IRDi[8]  = false;
								break;
							case 3:
								m_ctrlStaticNO8.SetIcon(m_hIconRed);
								IRDi[9]  = false;
								break;
							default:
								break;
							}							
						}
						else
						{							
							switch(i) 
							{
							case 0:
								m_ctrlStaticNO6.SetIcon(m_hIconOff);
								IRDi[7]  = true;
								break;
							case 1:
								m_ctrlStaticNO13.SetIcon(m_hIconOff);
								IRDi[10] = true;
								break;
							case 2:
								m_ctrlStaticNO7.SetIcon(m_hIconOff);
								IRDi[8]  = true;
								break;
							case 3:
								m_ctrlStaticNO8.SetIcon(m_hIconOff);
								IRDi[9]  = true;
								break;
							default:
								break;
							}
						}
					}
				}  
				else if (m_RevData2.Left(2) == "07")//��������λ
				{
					CString m_wwd1 = m_RevData2.Mid(3,1);
					CString m_wwd2 = m_RevData2.Mid(5,1);
					CString m_wwd3 = m_RevData2.Mid(7,1);
					
					sscanf(m_wwd1, "%x", &m_intRevData); 
					sscanf(m_wwd2, "%x", &m_intRevData1);				
					sscanf(m_wwd3, "%x", &m_intRevData2);
					m_intLeftIR = m_intRevData*256 + m_intRevData1*16 + m_intRevData2;
					UpdateData(false);
				}				
				else
				{
					m_RevData2.Empty();
				}
			}
			else if (m_RevData2.GetLength() >= 30)
			{
				m_RevData2.Empty();
			}		
			m_EndSensorData2   = true;
			m_StartSensorData2 = false;
		}
		else if(m_StartSensorData2 && !m_EndSensorData2)
		{		
			m_RevData2 += str;
		}	
	}
	return 0;
}
/***********************************************************************************************/
/* ��������: ��Ƭ������COM12                                                                   */
/* ��������: (1) �������Ҳ൥Ƭ��;                                                             */
/*           (2) �Ҳ������ܺʹ��崫����                                                        */
/* ����ע��:                                                                                   */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/

void CRobotDlg::OnControlHold()//���������ܸ߶�
{
	UpdateData(true);
	if (m_intHoldPosition <= 500 && m_intHoldPosition >= 120 )
	{
		unsigned char hexdata[9];
		CString       senddata;
		unsigned char checksum = 0;
		char data[16];
		
		hexdata[0] = 0x1b;
		hexdata[1] = 0x01;
		hexdata[4] = ((m_intHoldPosition) & 0x0f) | 0x30;
		hexdata[3] = ((m_intHoldPosition>>4) & 0x0f) | 0x30;
		hexdata[2] = ((m_intHoldPosition>>8) & 0x0f) | 0x30;
		hexdata[5] = 0x30;
		for(int i = 0; i <= 5; i++)
		{
			checksum = checksum + hexdata[i];
		}
		hexdata[6] = (checksum&0x0f)|0x30;
		hexdata[7] = 0x0a;	
		senddata.Format("%02x %02x %02x %02x %02x %02x %02x %02x",hexdata[0],hexdata[1],hexdata[2],hexdata[3],hexdata[4],hexdata[5],hexdata[6],hexdata[7]);
		if (m_bSerialPortOpened2)
		{
			int len = Str2Hex(senddata,data);
			m_serialport2.WriteToPort(data,len);
		}
		else
		{
			return;
		}	
	}
	else
	{
		return;
	}
}
//��ȡ������λ��
void CRobotDlg::OnReadHoldPos() 
{
	char data[16];
	CString cmd = "1b 02 30 30 30 30 3d 0a";
	if (m_bSerialPortOpened2)
	{
		int len = Str2Hex(cmd,data);
		m_serialport2.WriteToPort(data,len);
	}
	else
	{
		//AfxMessageBox("��򿪴���12!");		
		return;
	}	
}
//ץȡ����֮ǰ, ȷ������Ԥ��߶�
void CRobotDlg::OnPETHoldHeight() //PET�߶�
{
    m_intHoldPosition = DevicePosition.PETHoldPosition;
	UpdateData(false);
	UpdateData(true);
    OnControlHold();
	m_Holdstate = 1;
}
void CRobotDlg::OnCTHoldHeight() //CT�߶�
{
    m_intHoldPosition = DevicePosition.CTHoldPosition;
	UpdateData(false);
	UpdateData(true);
    OnControlHold();
	m_Holdstate = 2;
}
void CRobotDlg::OnMRIHoldHeight() //MRI�߶�
{
    m_intHoldPosition = DevicePosition.TOPHoldPosition;//MRIHight;
	UpdateData(false);
	UpdateData(true);
    OnControlHold();
	m_Holdstate = 3;	//�ֱ������ͬ�Ķ���,�˴�ΪMRI
}
//�������������ߵ��Ŵ����λ��,���������ߵ�һ���߶��Է�����ô���
void CRobotDlg::OnDownHoldPos() 
{
    m_intHoldPosition = 450;
	UpdateData(false);
	UpdateData(true);
    OnControlHold();
}
//ֱ�ӿ���������,û��ʹ�ñջ�����
void CRobotDlg::OnHoldUpBroad()  //����������
{
	char data[16];
	CString cmd = "1b 04 30 30 30 30 3f 0a";
	if (m_bSerialPortOpened2)
	{
		int len = Str2Hex(cmd,data);
		m_serialport2.WriteToPort(data,len);
	}
	else
	{
		return;
	}	
}
//�������½�
void CRobotDlg::OnHoldDownBroad()
{
	char data[16];
	CString cmd = "1b 05 30 30 30 30 30 0a";
	if (m_bSerialPortOpened2)
	{
		int len = Str2Hex(cmd,data);
		m_serialport2.WriteToPort(data,len);
	}
	else
	{
		return;
	}
}
//������ֹͣ
void CRobotDlg::OnHoldStopBroad() 
{
	char data[16];
	CString cmd = "1b 09 30 30 30 30 34 0a";
	if (m_bSerialPortOpened2)
	{
		int len = Str2Hex(cmd,data);
		m_serialport2.WriteToPort(data,len);
	}
	else
	{
		return;
	}	
}
//ʮ������ת��Ϊ�ַ���
char CRobotDlg::HexChar(char c)
{
	if((c >= '0') && (c <= '9'))
		return c - 0x30;
	else if((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;
	else if((c >= 'a')&&(c <= 'f'))
		return c - 'a' + 10;
	else 
		return 0x10;
}

//�ַ���ת��Ϊʮ������                                 
int CRobotDlg::Str2Hex(CString str, char *data)
{
	int t,t1;
	int rlen = 0,len = str.GetLength();
	for(int i = 0;i < len;)
	{
		char l,h = str[i];
		if(h == ' ')
		{
			i++;
			continue;
		}
		i++;
		if(i >= len)
			break;
		l  = str[i];
		t  = HexChar(h);
		t1 = HexChar(l);
		if((t == 16) || (t1 == 16))
			break;
		else 
			t = t * 16 + t1;
		i++;
		data[rlen]=(char)t;
		rlen++;
	}
	return rlen;
}  
//�ֱ�����
void CRobotDlg::OnControlHoldR()
{
	int wwd = m_AD3;
	m_intHoldPosition = (int)(wwd*1.6+200);
	UpdateData(false);
    if (m_IO1 == 0)
    {
		if (NUM == 0)
		{
			if (m_tempintHoldPosition != m_intHoldPosition)
			{           	
				OnControlHold();
				m_tempintHoldPosition = m_intHoldPosition;
				NUM = 10;
			}
			else
			{
				return;
			}
		}
		else
		{
			NUM--;
			if (NUM < 0)
			{
				NUM = 10;
			}
		}
    }
	else
	{
		return;
	}
}
/************************************************************************/
/*                            ��Ƭ��                                    */
/*                         ǰ�����⴫����                               */
/*                        COM9���COM11�Ҳ�                             */
/************************************************************************/
void CRobotDlg::OnLeftIRPosition() 
{
	char data[16];
	CString cmd="1b 10 30 30 30 30 3b 0a";
	if (m_bSerialPortOpened1)
	{
		int len=Str2Hex(cmd,data);
		m_serialport1.WriteToPort(data,len);
	}
	else
	{
		return;
	}	
}
void CRobotDlg::OnRightIRPosition() 
{
	char data[16];
	CString cmd="1b 10 30 30 30 30 3b 0a";
	if (m_bSerialPortOpened3)
	{
		int len=Str2Hex(cmd,data);
		m_serialport3.WriteToPort(data,len);
	}
	else
	{
		return;
	}	
}
/************************************************************************/
/*                          ��Ƭ��                                      */
/*                         ָʾ�ƿ���                                   */
/*                          COM9���                                    */
/************************************************************************/
void CRobotDlg::OnCloseLight() 
{
	char data[16];
	CString cmd="1b 04 30 30 30 30 3f 0a";
	if (m_bSerialPortOpened1)
	{
		int len=Str2Hex(cmd,data);
		m_serialport1.WriteToPort(data,len);
		CMenu* mmenu = GetMenu();
		mmenu->CheckMenuItem(IDC_GreenLight,MF_UNCHECKED);
		mmenu->CheckMenuItem(IDC_RedLight,MF_UNCHECKED);
		mmenu->CheckMenuItem(IDC_CloseLight,MF_CHECKED);
	}
	else
	{
		return;
	}	
}
void CRobotDlg::OnRedLight() 
{
	char data[16];
	CString cmd="1b 05 30 30 30 30 30 0a";
	if (m_bSerialPortOpened1)
	{
		int len=Str2Hex(cmd,data);
		m_serialport1.WriteToPort(data,len);
		CMenu* mmenu = GetMenu();
		mmenu->CheckMenuItem(IDC_GreenLight,MF_UNCHECKED);
		mmenu->CheckMenuItem(IDC_RedLight,MF_CHECKED);
		mmenu->CheckMenuItem(IDC_CloseLight,MF_UNCHECKED);
	}
	else
	{
		return;
	}		
}
void CRobotDlg::OnGreenLight() 
{
	char data[16];
	CString cmd="1b 06 30 30 30 30 31 0a";
	if (m_bSerialPortOpened1)
	{
		int len=Str2Hex(cmd,data);
		m_serialport1.WriteToPort(data,len);
		CMenu* mmenu = GetMenu();
		mmenu->CheckMenuItem(IDC_GreenLight,MF_CHECKED);
		mmenu->CheckMenuItem(IDC_RedLight,MF_UNCHECKED);
		mmenu->CheckMenuItem(IDC_CloseLight,MF_UNCHECKED);
	}
	else
	{
		return;
	}		
}
/***********************************************************************************************/
/* ��������: �ֶ����ƻ������˶�                                                                */
/* ��������: (1) ��ת90��OnTurn90()                                                            */
/*           (2) ��ת57��OnTurn57(),ʹ��������Բ����ת;                                        */
/*           (3) ������ת��OnTurnZero();                                                       */
/*           (4) ��������Ƕ�ת��RoundAngle(float angle1);                                     */
/*           (5) ����ÿһ�����ӵĽǶ�OnTurnAngle(int angle1, int angle2, int angle3, int angle4);                                                  */
/*           (6) ����ǰ��OnFront();
/*           (7) ���ƺ���OnRear(); 
/* ����ע��:                                                                                   */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/
void CRobotDlg::OnTurn90()
{
	RevTurnPosition();
	int temp1=4270;//90*854/18;
	int temp2=2400;//90*480/18;
	m_longPosition[0] = temp1 + TurnError1;	
	m_longPosition[2] = temp1 + TurnError2;	
	m_longPosition[4] = temp2 + TurnError3;	
	m_longPosition[6] = temp2 + TurnError4;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}

void CRobotDlg::OnTurn90New()
{
	int temp1=4270;//90*854/18;
	int temp2=2400;//90*480/18;
	m_longPosition[0] = -(temp1 + TurnError1);	
	m_longPosition[2] = temp1 + TurnError2;	
	m_longPosition[4] = temp2 + TurnError3;	
	m_longPosition[6] = -(temp2 + TurnError4);
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}


void CRobotDlg::OnTurn57()
{					
    m_tempIO6=1;
	RevTurnPosition();
	int temp1=2710;//-57.14*854/18;
	int temp2=1524;//-57.14*480/18;
	m_longPosition[0] = -temp1;	
	m_longPosition[2] =  temp1;	
	m_longPosition[4] =  temp2;	
	m_longPosition[6] = -temp2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}
//������ת��
void CRobotDlg::OnTurnZero() 
{
	m_longPosition[0] = TurningZero901;	
	m_longPosition[2] = (long)(TurningZero3782);	
	m_longPosition[4] = TurningZero903;	
	m_longPosition[6] = (long)(TurningZero3784);
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}
//����ת��------�������һ�����ӵĽǶ�--------����Ƕ�ת��
void CRobotDlg::RoundAngle(float angle1)
{
	float rad;
	float angle2;
	rad    = (float)(658 / (tan(angle1)));
    angle2 = (float)(atan(658 / (rad + 850)));
	m_longPosition[0] = (long)(angle1 * TurningAngle1);	
	m_longPosition[2] = (long)(angle2 * TurningAngle1);	
	m_longPosition[4] = (long)(-angle1 * TurningAngle2);
	m_longPosition[6] = (long)(-angle2 * TurningAngle2);
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}
void CRobotDlg::OnTurnAngle(int angle1, int angle2, int angle3, int angle4)
{
	int temp1 = (angle1 * 854) / 180;
	int temp2 = (angle2 * 854) / 180;
	int temp3 = (angle3 * 480) / 180;
	int temp4 = (angle4 * 480) / 180;
	m_longPosition[0] = temp1 + TurnError1;	
	m_longPosition[2] = temp2 + TurnError2;	
	m_longPosition[4] = temp3 + TurnError3;
	m_longPosition[6] = temp4 + TurnError4;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}
}
void CRobotDlg::OnFront() 
{
	TempMoveVel[0]=TempMoveVel[1]=TempMoveVel[2]=TempMoveVel[3]=0;
     RevTurnPosition();
	if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
	{
		UpdateData(false);
		SanorFRDirection = 1;
		MoveSameVelocityControl(-DirVelocityPara);
	}
	else if (m_RevTurnPos1 == -884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -883)
	{
		UpdateData(false);
		SanorLRDirection = 1;
		MoveSameVelocityControlNew(-VelocityPara90);
	}
	else if (m_RevTurnPos1 == -571 && m_RevTurnPos3 == 571 && m_RevTurnPos5 == 571&& m_RevTurnPos7 == -571)
	{     
		SanorLRDirection = 3;
		SanorFRDirection = 3;
        MoveDiffVelocityControl(-VelocityPara57);
	}
	else
	{
		SetDlgItemText(IDC_STATICwwd, _T("������ת��Ƕȴ���!"));
		AfxMessageBox("��ȷ��������ת��Ƕ�!57 or 90!");
	}
}
void CRobotDlg::OnRear() 
{
	TempMoveVel[0]=TempMoveVel[1]=TempMoveVel[2]=TempMoveVel[3]=0;
    RevTurnPosition();
	if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
	{
		UpdateData(false);
		SanorFRDirection = 2;
		MoveSameVelocityControl(DirVelocityPara);
	}
	else if (m_RevTurnPos1 == -884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -883)
	{
		UpdateData(false);
		SanorLRDirection = 2;
		MoveSameVelocityControlNew(VelocityPara90);
	}
	else if (m_RevTurnPos1 == -571 && m_RevTurnPos3 == 571 && m_RevTurnPos5 == 571&& m_RevTurnPos7 == -571)
	{    
		SanorLRDirection = 3;
		SanorFRDirection = 3;
        MoveDiffVelocityControl(VelocityPara57);
	}
	else 
	{
		SetDlgItemText(IDC_STATICwwd, _T("������ת��Ƕȴ���!"));
		AfxMessageBox("��ȷ��������ת��Ƕ�!57 or 90!");
	}
}

/*****************************************************************************************
  OnPaintSonar()��������Ҫ�ǻ��ƻ����������ɴ�����������ͼ���������ú���OnPaintSingleSonar
  ��Ҫ���ļ�ͷ������ͷ�ļ���
	#include "math.h"
	#define PI 3.141592653598
	#define min(a,b) (((a) < (b)) ? (a) : (b))

  ������Ҫ����Picture�ؼ���ID��Ϊ:IDC_SENSOR 

  %%%%%%%%%%%%%%%%%%%%%�����º����μ�������ɴ�����������%%%%%%%%%%%%%%%%%%%%%%%%
	/********���ڴ˸������ɴ���������ֵ*************/
//	*(sonardist + 0) = 2000;
//	*(sonardist + 1) = 1500;
//	*(sonardist + 2) = 500;
//	*(sonardist + 3) = 1500;
//	*(sonardist + 4) = 400;
//	*(sonardist + 5) = 1500;
//	*(sonardist + 6) = 600;
// 	*(sonardist + 7) = 1500;
	/********���ڴ˸������ɴ���������ֵ************

  	/********���ڴ˸��³��ֽǶȵ���ֵ*************
//	*(wheelangle + 0) = 15;
//	*(wheelangle + 1) = -25;
//	*(wheelangle + 2) = 85;
//	*(wheelangle + 3) = -15;
	/********���ڴ˸��³��ֽǶȵ���ֵ*************

  %%%%%%%%%%%%%%%%%%%%%�����º����μ���»����������ɵĳߴ�����%%%%%%%%%%%%%%%%%%%%
//	int robot_halfwidth		= 1000;     //����
//	int robot_halflength	= 1000;    //����
//	int sonarmaxdist		= 2000;   //���������Ч����
//	int sonarangle			= 30;     //������Ч�Ƕ�

  ��OnPaintSonar�������Է���OnPaint()��OnDraw()���Զ����ã�ʵʱ���´�����ͼ
***************************************************************************************/
void CRobotDlg::OnPaintSonar()
{
	int m_sonarnum = 8;					//������������Ŀ
	int *sonardist;
	sonardist = new int[m_sonarnum];	//��������������ֵ
	memset(sonardist,1000,m_sonarnum);
	/********���ڴ˸������ɴ���������ֵ*************/
    /********��ͼ��ѭ��ǰ����ʱ����ת*************/
    /********�ɼ����ݣ�ǰ4����ǰ3���Һ�2����1*************/
	*(sonardist + 0) = SonorPosition[RightSonor4].SonerPosition*10;//;//�Ҳ��4��  //ǰ���Ҳ�//right
	*(sonardist + 1) = SonorPosition[RightSonor3].SonerPosition*10;//;//�Ҳ��3��0;//�Ҳ�ǰ��//front
	*(sonardist + 2) = SonorPosition[LeftSonor3].SonerPosition*10;//����3��0;  //���ǰ��//front
	*(sonardist + 3) = SonorPosition[LeftSonor4].SonerPosition*10;//����4��    //ǰ�����//left
	*(sonardist + 4) = SonorPosition[LeftSonor2].SonerPosition*10;//��xia���2�� //�����//left
	*(sonardist + 5) = SonorPosition[LeftSonor1].SonerPosition*10;//����1��0;  //����//rear
	*(sonardist + 6) = SonorPosition[RightSonor2].SonerPosition*10;//����1��0;   //�Ҳ��//rear
	*(sonardist + 7) = SonorPosition[RightSonor1].SonerPosition*10;//�Ҳ��2��     //���Ҳ�//right
	
	/********���ڴ˸������ɴ���������ֵ*************/

	int m_wheelnum = 4;					//������Ŀ
	int *wheelangle;
	wheelangle = new int[m_wheelnum];  //������Ŀ
	memset(wheelangle,0,m_wheelnum);

	/********���ڴ˸��³��ֽǶȵ���ֵ*************/
	*(wheelangle + 0) = RevTurnpos[1];
	*(wheelangle + 1) = RevTurnpos[0];
	*(wheelangle + 2) = RevTurnpos[2];
	*(wheelangle + 3) = RevTurnpos[3];
	/********���ڴ˸��³��ֽǶȵ���ֵ*************/
	
	//��û�ͼ����
	RECT  rcWindow; 
	CClientDC  dc(this);  //����CClientDC����
    ::GetWindowRect(GetDlgItem(IDC_SENSOR)->GetSafeHwnd(),&rcWindow); 
	ScreenToClient(&rcWindow);
	int windowwidth = (rcWindow.right - rcWindow.left)/2;       //������ͼ����Ĵ�С
	int windowlength = (rcWindow.bottom - rcWindow.top)/2;

	int paintx = rcWindow.left + windowwidth;     //��ͼ������������
	int painty = rcWindow.top + windowlength;

	//��ͼ���ߴ�
	int robot_halfwidth		= 800;     //����
	int robot_halflength	= 1500;    //����
	int sonarmaxdist		= 2000;   //���������Ч����
	int sonarmindist        = 300;    //������С��Ч����
	int sonarangle			= 30;     //������Ч�Ƕ�
	int robot_wheelwidth    = int(robot_halflength*0.25);    //���ֿ�ϵ��0.25�ǳ��ֵı�����ֵԽ�󣬳��־�Խ��

    //ʵ�ʵ�λmm���ͼ��λ����pixel��Ӧ��ϵ
	double  coe = min(windowlength/1.0/(robot_halflength+sonarmaxdist),windowwidth/1.0/(robot_halfwidth+sonarmaxdist));             

	robot_halflength		= int(robot_halflength*coe);
	robot_halfwidth			= int(robot_halfwidth*coe);

	//��������
	CPen mypen,*poldpen;
	mypen.CreatePen(PS_SOLID,3,RGB(150,150,150));
    poldpen = dc.SelectObject(&mypen);   //����ɻ��ʣ��������µĻ���
	//��ʼ�������
	dc.MoveTo(int(paintx+robot_halfwidth),int(painty-robot_halflength));
	dc.LineTo(int(paintx-robot_halfwidth),int(painty-robot_halflength));
	dc.LineTo(int(paintx-robot_halfwidth),int(painty+robot_halflength));
	dc.LineTo(int(paintx+robot_halfwidth),int(painty+robot_halflength));
	dc.LineTo(int(paintx+robot_halfwidth),int(painty-robot_halflength));
	//�������
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);
	int iScrollX = ::GetSystemMetrics(SM_CXSIZE);
	int iScrollY = ::GetSystemMetrics(SM_CYSIZE);

	TEXTMETRIC tmFont;
	dc.GetTextMetrics(&tmFont);
	dc.TextOut(paintx - tmFont.tmMaxCharWidth*3/2 ,painty - tmFont.tmHeight/2 ,_T("������"));	
	dc.TextOut(paintx - tmFont.tmMaxCharWidth/2,int(painty - robot_halflength) ,_T("ǰ"));
	dc.TextOut(paintx - tmFont.tmMaxCharWidth/2,int(painty + robot_halflength - tmFont.tmHeight) ,_T("��"));
	dc.SetBkMode(nOldBkMode);

	//�Դ�������ֵת������ͼ��λ��
	for(int i = 0; i < m_sonarnum;i ++)
	{
		if(*(sonardist+i) > sonarmaxdist)
            *(sonardist+i) = sonarmaxdist;
		else if(*(sonardist+i) < sonarmindist)
			*(sonardist+i) = sonarmindist;
		*(sonardist+i) = int(*(sonardist+i)*coe);
		
	}
	//��ʼ�������ɴ���������
	//���Ͻ�
	OnPaintSingleSonar(&dc,int(paintx+robot_halfwidth),int(painty-robot_halflength),sonarangle,0,*(sonardist+0));
	OnPaintSingleSonar(&dc,int(paintx+robot_halfwidth),int(painty-robot_halflength),sonarangle,90,*(sonardist+1));
	//���Ͻ�
	OnPaintSingleSonar(&dc,int(paintx-robot_halfwidth),int(painty-robot_halflength),sonarangle,90,*(sonardist+2));
	OnPaintSingleSonar(&dc,int(paintx-robot_halfwidth),int(painty-robot_halflength),sonarangle,180,*(sonardist+3));
	//���½�
	OnPaintSingleSonar(&dc,int(paintx-robot_halfwidth),int(painty+robot_halflength),sonarangle,180,*(sonardist+4));
	OnPaintSingleSonar(&dc,int(paintx-robot_halfwidth),int(painty+robot_halflength),sonarangle,270,*(sonardist+5));
	//���½�
	OnPaintSingleSonar(&dc,int(paintx+robot_halfwidth),int(painty+robot_halflength),sonarangle,270,*(sonardist+6));
	OnPaintSingleSonar(&dc,int(paintx+robot_halfwidth),int(painty+robot_halflength),sonarangle,0,*(sonardist+7));
	
	OnPaintSingleWheel(&dc,int(paintx+robot_halfwidth),int(painty-robot_halflength)/*-int(robot_wheelwidth*coe)*/,int(robot_wheelwidth*coe),*(wheelangle + 0),0);
	OnPaintSingleWheel(&dc,int(paintx-robot_halfwidth),int(painty-robot_halflength)/*-int(robot_wheelwidth*coe)*/,int(robot_wheelwidth*coe),*(wheelangle + 1),1);
	OnPaintSingleWheel(&dc,int(paintx-robot_halfwidth),int(painty+robot_halflength)/*-int(robot_wheelwidth*coe)*/,int(robot_wheelwidth*coe),*(wheelangle + 2),2);
	OnPaintSingleWheel(&dc,int(paintx+robot_halfwidth),int(painty+robot_halflength)/*-int(robot_wheelwidth*coe)*/,int(robot_wheelwidth*coe),*(wheelangle + 3),3);
    if (countnub >=2)
	{
		InvalidateRect(&rcWindow,true);
		countnub=0;
	}
	countnub++;
}
/***********************************************************************************
	��������OnPaintSingleSonar   //����������ͼ
	��������dc              //��ͼ�豸
	��������x               //�������ĵ�X������
			y               //�������ĵ�Y������
			angle           //������Ч�Ƕ�
			centerangle     //���ɵİ�װ�Ƕȣ�����Ϊ0����ǰΪ90������Ϊ180�����Ϊ270
			dist            //���ɲ����ľ���ֵ
***********************************************************************************/
void CRobotDlg::OnPaintSingleSonar(CDC* dc,int x, int y, int angle, int centerangle,int dist)
{
	//��������
	CPen arcpen,linepen,*poldpen;
	arcpen.CreatePen(PS_SOLID,3,RGB(0,0,255));
    poldpen = dc->SelectObject(&arcpen);   //����ɻ��ʣ��������µĻ���
	int x1 = 0,y1 = 0,x2 = 0,y2 = 0,x3 = 0,y3 = 0,x4 = 0,y4 = 0;
	//Բ����Ӧ��Բ�����о��Σ�(x1,y1)Ϊ���Ͻ����꣬(x2,y3)Ϊ���½����ꣻ
	x1 = int(x - dist);
	y1 = int(y - dist);

	x2 = int(x + dist);
	y2 = int(y + dist);

	//��������Բ������ʼ����ͽ�������
	if(centerangle == 0 || centerangle == 180)
	{
		x3 = int(x + dist * cos((centerangle+angle/2)*PI/180.0));
		y3 = int(y + dist * sin((centerangle+angle/2)*PI/180.0));
		x4 = x3;
		y4 = int(y + dist * sin((centerangle-angle/2)*PI/180.0));
	}
	else if(centerangle == 90 || centerangle == 270)
	{
		x3 = int(x + dist * cos((centerangle-angle/2)*PI/180.0));
		y3 = int(y - dist * sin((centerangle-angle/2)*PI/180.0));
		x4 = int(x + dist * cos((centerangle+angle/2)*PI/180.0));
		y4 = y3;
	}
	else 
		return;

	dc->Arc(x1,y1,x2,y2,x3,y3,x4,y4);       //��Բ��

	linepen.CreatePen(PS_DOT,1,RGB(0,0,0));
    poldpen = dc->SelectObject(&linepen);   //����ɻ��ʣ��������µĻ���

	dc->MoveTo(x,y);
	dc->LineTo(x3,y3);
	dc->MoveTo(x,y);
	dc->LineTo(x4,y4);
	dc->SelectObject(poldpen);
}
/**************************************************************************************
	��������OnPaintSingleWheel   //����������ͼ
	��������dc              //��ͼ�豸
	��������x               //�������ĵ�X������
			y               //�������ĵ�Y������
			wheelwidth      //���ֿ�ȣ����أ�
			angle           //���ֵ�ת�ǣ���ǰ��0�ȣ�����Ϊ��(0~+180)������Ϊ��(0~-180)
			no              //���ֵı�ţ���ǰΪ0����ǰΪ1�����Ϊ2���Һ�Ϊ3
***************************************************************************************/
void CRobotDlg::OnPaintSingleWheel(CDC *dc, int x, int y,int wheelwidth,int angle,int no)
{
	//��������
	CPen arcpen,linepen,*poldpen;
	arcpen.CreatePen(PS_SOLID,2,RGB(0,0,0));
    poldpen = dc->SelectObject(&arcpen);   //����ɻ��ʣ��������µĻ���

	int x1 = 0,y1 = 0,x2 = 0,y2 = 0,x3 = 0,y3 = 0,x4 = 0,y4 = 0,x5 = 0,y5 = 0,x6 = 0,y6 = 0;

	//Բ����Ӧ��Բ�����о��Σ�(x1,y1)Ϊ���Ͻ����꣬(x2,y3)Ϊ���½����ꣻ
	x1 = int(x - wheelwidth/2*sin(angle*PI/180)-wheelwidth/2 + 0.5);
	y1 = int(y - wheelwidth/2*cos(angle*PI/180)-wheelwidth/2 + 0.5);
	
	x2 = int(x - wheelwidth/2*sin(angle*PI/180)+wheelwidth/2 + 0.5);
	y2 = int(y - wheelwidth/2*cos(angle*PI/180)+wheelwidth/2 + 0.5);	

	x3 = int(x - wheelwidth/2*sin(angle*PI/180)+wheelwidth/2*cos(angle*PI/180) + 0.5);
	y3 = int(y - wheelwidth/2*cos(angle*PI/180)-wheelwidth/2*sin(angle*PI/180) + 0.5);

	x4 = int(x - wheelwidth/2*sin(angle*PI/180)-wheelwidth/2*cos(angle*PI/180) + 0.5);
	y4 = int(y - wheelwidth/2*cos(angle*PI/180)+wheelwidth/2*sin(angle*PI/180) + 0.5);
	
	dc->Arc(x1,y1,x2,y2,x3,y3,x4,y4);       //��Բ��

	x1 = int(x + wheelwidth/2*sin(angle*PI/180)-wheelwidth/2 + 0.5);
	y1 = int(y + wheelwidth/2*cos(angle*PI/180)-wheelwidth/2 + 0.5);
	
	x2 = int(x + wheelwidth/2*sin(angle*PI/180)+wheelwidth/2 + 0.5);
	y2 = int(y + wheelwidth/2*cos(angle*PI/180)+wheelwidth/2 + 0.5);	

	x6 = int(x + wheelwidth/2*sin(angle*PI/180)+wheelwidth/2*cos(angle*PI/180) + 0.5);
	y6 = int(y + wheelwidth/2*cos(angle*PI/180)-wheelwidth/2*sin(angle*PI/180) + 0.5);

	x5 = int(x + wheelwidth/2*sin(angle*PI/180)-wheelwidth/2*cos(angle*PI/180) + 0.5);
	y5 = int(y + wheelwidth/2*cos(angle*PI/180)+wheelwidth/2*sin(angle*PI/180) + 0.5);
	
	dc->Arc(x1,y1,x2,y2,x5,y5,x6,y6);       //��Բ��

	dc->MoveTo(x6,y6);
	dc->LineTo(x3,y3);
	dc->MoveTo(x5,y5);
 	dc->LineTo(x4,y4);

	x1 = int(x - wheelwidth*1.5*sin(angle*PI/180) + 0.5);
	y1 = int(y - wheelwidth*1.5*cos(angle*PI/180) + 0.5);

	x2 = int(x + wheelwidth*1.5*sin(angle*PI/180) + 0.5);
	y2 = int(y + wheelwidth*1.5*cos(angle*PI/180) + 0.5);

	x3 = int(x - wheelwidth*cos(angle*PI/180) + 0.5);
	y3 = int(y + wheelwidth*sin(angle*PI/180) + 0.5);

	x4 = int(x + wheelwidth*cos(angle*PI/180) + 0.5);
	y4 = int(y - wheelwidth*sin(angle*PI/180) + 0.5);

	linepen.CreatePen(PS_DOT,1,RGB(255,0,0));
    poldpen = dc->SelectObject(&linepen);   //����ɻ��ʣ��������µĻ���

	dc->MoveTo(x1,y1);
	dc->LineTo(x2,y2);
	dc->MoveTo(x3,y3);
 	dc->LineTo(x4,y4);
	
	//���ĵ�
	dc->SetPixel(CPoint(x,y),RGB(255,0,0));
    //�Ƕȱ�ʶ
	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(RGB(0,0,0));
	dc->SetBkColor(RGB(255,255,255));
	CString str;
	str.Format("%d��",angle);
	int charnum = 0;
	TEXTMETRIC tmFont;
	dc->GetTextMetrics(&tmFont);
	if(no < 2)
		dc->TextOut(x - tmFont.tmMaxCharWidth,int(y + wheelwidth*1.5),str);	
	else if(no < 4)
		dc->TextOut(x - tmFont.tmMaxCharWidth,int(y - wheelwidth*1.5 -tmFont.tmHeight) ,str);	
	else
		return;
	dc->SelectObject(poldpen);	
}
/***********************************************************************************************/
/* ��������: ѡ��·�ߺ���                                                                 */
/* ��������: (1) ͨ��ѡ��·�ߴ���ѡ���ߵ�·��;                                                             */
/*           (2) ȷ��ִ�к���;                                                           */
/* ����ע��:                                                                                   */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/
void CRobotDlg::OnChoroadRemote()
{
    OnMoveZero();
	SetDlgItemText(IDC_STATICwwd,"�ѽ����µĲ�����");
    m_CountNumLimit	= 0;		
	m_intStat       = 0;
	tempNum         = 0;
	int m_intStat1 = 0;
	m_intStat1 = m_AD3;//Զ�̿��ƴ��������Ŀ��
	if (m_intStat1 == 32 || m_intStat1 == 33)
	{
	    SetDlgItemText(IDC_STATICwwd,"�ֶ���λ90!");
	}
	else if (m_intStat1 == 33)
	{
		SetDlgItemText(IDC_STATICwwd,"�ֶ���λ90!");
	}
	else
	{
		OnReZeroPosition();
	}
	m_intStat    = m_intStat1;
	DeviceNumber = 0;
	m_Turnstate  = 0;
	UpdateData(false);
}
void CRobotDlg::OnChoroad() 
{
    OnMoveZero();
	SetDlgItemText(IDC_STATICwwd,"�ѽ����µĲ�����");
    m_CountNumLimit	= 0;		
	m_intStat       = 0;
	tempNum         = 0;
	DevicePosition.DelayTimes = 0;
//	CCHORoad road
	CChosRoad1 road1;
	int nRespond = 	road1.DoModal();
	if (nRespond == IDOK)
	{    
		int m_intStat1 = 0;
		m_intStat1 = road1.Chooseflag;
		if (m_intStat1 == 32 || m_intStat1 == 33)
		{
	        SetDlgItemText(IDC_STATICwwd,"�ֶ���λ90!");
		}
	    else if (m_intStat1 == 33)
	    {
			SetDlgItemText(IDC_STATICwwd,"�ֶ���λ90!");
	    }
		else
		{
			OnReZeroPosition();
		}
		m_intStat    = m_intStat1;
		DeviceNumber = 0;
		m_Turnstate  = 0;
		UpdateData(false);
	}
}
/************************************************************************/
/*                 CT             PET              MRI
/* |               |              |                |                     */
/* |               |              |                |                     */
/* |               |              |                |                     */
/* |               | 1            | 2              | 3                   */
/*--------------------------------------------------------------------
/************************************************************************/
void CRobotDlg::ChoRoad()
{
	switch(m_intStat) 
	{
	case 5:	
		RobotRight(1);//�������е�һ��_CT_PET(); 
		break;
	case 8:	
		RobotLeft(1);//�������е�һ��_PET_CT();  
		break;
		
	case 13:
		DirRobotFront(DevicePosition.PETHoldPosition, DevicePosition.PET, DevicePosition.FisrtPET*10, DevicePosition.SecondPET*10);
     	//DirRobotFront();//PETֱ��ץ����;
		break;
	case 14:
		DirRoboRear(DevicePosition.PET);
       // DirRoboRear();//PETֱ���뿪;
		break;
	case 15:
		DirRoboLeftRight(DevicePosition.PET);
        //DirRoboLeftRight();//PETֱ�ӷŴ���;
		break;
		
	case 20:
	    DirRobotFront(DevicePosition.CTHoldPosition, DevicePosition.CT, DevicePosition.FisrtCT*10, DevicePosition.SecondCT*10);
         //	DirRobotFront2();//CTֱ��ץ����;
		break;
	case 21:
		DirRoboRear(DevicePosition.CT);
       // DirRoboRear2();//CTֱ���뿪;
		break;
	case 22:	
		DirRoboLeftRight(DevicePosition.CT);
        //DirRoboLeftRight2();//CTֱ�ӷŴ���;
		break;
		
	case 25:	
        DirNextLimited();//���е���һ����λ;
		break;
	case 26:	
        DirProLimited();//���е�����һ����λ;
		break;

	case 32:	
        DirNextLimited90();//90ǰ����һ��λ;
		break;
	case 33:	
        DirProLimited90();//90������һ��λ;
		break;
		
/*		
	case 0:		
		break;                      
	case 1:	
        RobotFront(1);//��ʼ���е���һ���豸_0_CT();		
		break;
	case 2:	
		RobotFront(2);//��ʼ���е��ڶ����豸_0_PET();	
		break;
	case 3:	
		RobotFront(3);//��ʼ���е��������豸_0_MRI();
		break;
	case 4:	
		RobotRear(0); //��һ���豸���е���ʼ_CT_0();	
		break;'
	case 6:	
		RobotRight(2);//�������еڶ���_CT_MRI();
		break;
	case 7:	
		RobotRear(1); //�ڶ����豸���е���ʼ_PET_0();	
		break;
	case 10:	
		RobotRear(2); //�������豸���е���ʼ_MRI_0();
		break;			

	case 9:	
		RobotRight(1);//�������е�һ��_PET_MRI();
		break;
	case 11:	
		RobotLeft(2);//�������еڶ���_MRI_CT();
		break;
	case 12:	
		RobotLeft(1);//�������е�һ��_MRI_PET();
		break;

	case 16:	
     	DirRobotFront1();//MRIֱ��ץ����;
		break;
	case 17:	
        DirRoboRear1();//MRIֱ���뿪;
		break;
	case 18:	
        DirRoboLeftRight1();//MRIֱ�ӷŴ���;
		break;
	case 19:	
        DirRoboTuring90();//MRI����
		break;

	case 23:	
        DirRoboRear3();//PETֱ���뿪��ȥMRI;
		break;
	case 24:	
        DirRoboTuring90PET();//PET����
		break;

	case 27:	
        DirRearMRI();//���Ƶ�MRI;
		break;
	case 28:	
        DirRearPET();//MRI���˵�PET;
		break;
	case 29:	
        DirRoboRearPET();//��MRI���뿪PET;
		break;
	case 30:	
        DirRoboRearMRI();//��MRI���뿪;
		break;
	case 31:	
        DirRobotFrontMRI();//ץȡPET-MRI����;
		break;
*/
	default:
		break;	
	}
}
/***********************************************************************************************/
/* ��������: ���ݺ���������ӽǶ�                                                                   */
/* ��������: 1�� ��������ӵ���
/*                 (1) ǰ���˶�����ƽ��CarTurnFR();               
/*                 (2) �����˶�����ƽ��CarTurnLR();   
/*                 (3) ǰ���˶�ǰ������ת�����WheelTurnFR();   
/*                 (4) �����˶�ǰ������ת�����WheelTurnLR();  
/*           2�����ݺ����źŵ��õ�������                                                     */
/*                 (1) RobotRightTurn(int a)
/*                 (2) RobotFrontTurn(int a)
/*                 (3) RobotLeftTurn(int a)              
/*                 (4) RobotRearTurnMRI(int a)   
/*                 (5) RobotRearTurn(int a)   
/*                 (6) RobotFrontTurnMRI(int a)
/* ����ע��:                                                                                   */
/* �޸�ʱ��: 2010��1��5��                                                                      */
/***********************************************************************************************/

void CRobotDlg::CarTurnFR(int frontdata1, int frontdata2, int reardata1,int reardata2) 
{//ǰ���˶�����ƽ��
	m_longPosition[0] = frontdata1;	
	m_longPosition[2] = frontdata2;	
	m_longPosition[4] = reardata1;	
	m_longPosition[6] = reardata2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	} 
}
void CRobotDlg::CarTurnLR(int leftdata1, int leftdata2, int rightdata1,int rightdata2) 
{//�����˶�����ƽ��
	m_longPosition[0] = leftdata1;	
	m_longPosition[2] = rightdata1;	
	m_longPosition[4] = leftdata2;	
	m_longPosition[6] = rightdata2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}  
}
void CRobotDlg::WheelTurnFR(int frontdata1, int frontdata2, int reardata1,int reardata2) 
{//ǰ���˶�ǰ������ת�����
	m_longPosition[0] = frontdata1;	
	m_longPosition[2] = frontdata2;	
	m_longPosition[4] = reardata1;	
	m_longPosition[6] = reardata2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}  
}
void CRobotDlg::WheelTurnLR(int leftdata1, int leftdata2, int rightdata1,int rightdata2)
{//�����˶�ǰ������ת�����
	m_longPosition[0] = leftdata1;	
	m_longPosition[2] = rightdata1;	
	m_longPosition[4] = leftdata2;	
	m_longPosition[6] = rightdata2;
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}  
}

//���ݺ����źŵ��õ������� 5-7��ǰ��1-3�ں�
void CRobotDlg::RobotFrontTurn(int a)
{
	m_FronTturNum =0;
	if (TempIR1291 != IR1291)
	{
		switch(IR1291)
		{
		case 0:
			//ֹͣ0000
			if (IR[2])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 
				TempIR1291 = IR1291;
				return;
			}
			else if (IR[3])
			{
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 
				TempIR1291 = IR1291;
				return;	                
			}
			else
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd,"������������!");
			}
			
			break;
		case 3:
			if (IR[2])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 	
			}
			if (IR[3])
			{
				//++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 
									
			}	
			break;
			//ˮƽ����
		case 5:
			//����ƽ��0001
			//++++
			CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 																		
			break;
		case 10:
			//����ƽ��0010
			//----
			CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);   		
			break;
	   //б�ŷ�������
		case 9:
			//����ת0101
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y2);    
			break;
		case 6:
			//����ת0110
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;
		//5-7���ڣ�����1-3
		case 7:
			//����ת1001
			//++00
			WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;
		case 11:
			//����ת0101
			//--00
			WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
			//1-3���ڣ�����5-7
		case 13:
			//����ת0101
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y2);    
			break;				
		case 14:
			//����ת1001
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;
		default:
			//ǰ��
			OnReZeroPosition(); 
			break;
		}	
		TempIR1291 = IR1291;
	}
	else
	{
		return;
	}
}
//5-7��ǰ��1-3�ں�
void CRobotDlg::RobotFrontTurnMRI(int a)
{
	m_FronTturNum =0;
	if (TempIR1291 != IR1291)
	{
		if (IR1291!=0)
		{
			MRIError=0;
		}
		switch(IR1291)
		{
		case 0:
			//ֹͣ0000
			if (IR[2])
			{
                //----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 
				TempIR1291 = IR1291;
				return;
			}
			else if (IR[3])
			{
				//++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 
				TempIR1291 = IR1291;
				return;	                
			}
			else
			{
				MRIError++;
				if (MRIError>3)
				{	
					OnMotionstop();
					SetDlgItemText(IDC_STATICwwd,"������������!");
					MRIError=0;
				}	
				else
				{
					SetDlgItemText(IDC_STATICwwd,"������������!�ݴ������á�");
				}				
			}			
			break;
		case 3:
			if (IR[2])
			{
                //----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 	
			}
			if (IR[3])
			{
				//++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 
						            
			}	
			break;
			//ˮƽ����
		case 5:
			//����ƽ��0001
            //++++
			CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 																		
			break;
		case 10:
			//����ƽ��0010
			//----
			CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);   		
			break;
       //б�ŷ�������
		case 9:
			//����ת0101
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y2);    
			break;
		case 6:
			//����ת0110
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;
        //5-7���ڣ�����1-3
		case 7:
			//����ת1001
			//++00
            WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;
		case 11:
			//����ת0101
			//--00
            WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
			//1-3���ڣ�����5-7
		case 13:
			//����ת0101
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y2);    
			break;				
		case 14:
			//����ת1001
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;
		default:
			//ǰ��
			OnReZeroPosition(); 
			break;
		}	
		TempIR1291 = IR1291;
	}
	else
	{
		return;
	}
}
//1-3��ǰ��5-7�ں�
void CRobotDlg::RobotRearTurn(int a) 
{
	if (TempIR1291 != IR1291)
	{
		switch(IR1291)
		{
		case 0:
			if (IR[2])
			{
			   //++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			}
			else if (IR[3])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);                 
			}
			else
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd,"������������!");
			}
			
			break;
		case 3:
			if (IR[2])
			{
			   //++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			}
			if (IR[3])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);                 
			}	
			break;
		case 5:
			//����ƽ��0001
			//----
			CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 																		
			break;
		case 10:
			//����ƽ��0010
		   //++++
			CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			break;
		case 9:
			//����ת0101
			//++00
			WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;
		case 11:
			//����ת0101
			//++00
			WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;

		case 6:
			//����ת0110
			//--00
			WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
		case 7:
			//����ת1001
			//--00
			WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
		case 13:
			//����ת0101
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;				
		case 14:
			//����ת1001
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y1);  
			break;
		default:
			//ǰ��
			OnReZeroPosition(); 
			break;
		}	
		TempIR1291 = IR1291;
	}
	else
	{
		return;
	}
}
void CRobotDlg::RobotRearTurnMRI(int a) 
{
	if (TempIR1291 != IR1291)
	{
		if (IR1291!=0)
		{
			MRIError=0;
		}
		switch(IR1291)
		{				
		case 0:
			//ֹͣ0000
			if (IR[2])
			{
			   //++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			}
			else if (IR[3])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);                 
			}
			else
			{
				MRIError++;
				if (MRIError>2)
				{	
					OnMotionstop();
					SetDlgItemText(IDC_STATICwwd,"������������!");
					MRIError=0;
				}
			}				
			break;
		case 3:
			if (IR[2])
			{
			   //++++
				CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			}
			if (IR[3])
			{
				//----
				CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2);                 
			}	
			break;
		case 5:
			//����ƽ��0001
			//----
			CarTurnFR(FrontTurning5N1,FrontTurning5N2,RearTurning5N1,RearTurning5N2); 																		
			break;
		case 10:
			//����ƽ��0010
		   //++++
			CarTurnFR(FrontTurning5Y1,FrontTurning5Y2,RearTurning5Y1,RearTurning5Y2); 	
			break;
		case 9:
			//����ת0101
			//++00
			WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;
		case 11:
			//����ת0101
			//++00
			WheelTurnFR(FrontTurning5Y1,FrontTurning5Y2,TurnError3,TurnError4); 
			break;

		case 6:
			//����ת0110
			//--00
			WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
		case 7:
			//����ת1001
			//--00
			WheelTurnFR(FrontTurning5N1,FrontTurning5N2,TurnError3,TurnError4); 
			break;
		case 13:
			//����ת0101
			//00--
			WheelTurnFR(TurnError1,TurnError2,RearTurning5N1,RearTurning5N2); 
			break;				
		case 14:
			//����ת1001
			//00++
			WheelTurnFR(TurnError1,TurnError2,RearTurning5Y1,RearTurning5Y1);  
			break;
		default:
			//ǰ��
			OnReZeroPosition(); 
			break;
		}	
		TempIR1291 = IR1291;
	}
	else
	{
		return;
	}
}

//����3-7��ǰ
void CRobotDlg::RobotLeftTurn(int a)
{
	if (TempIR3467 != IR3467)
	{
		switch(IR3467)
		{
		case 0:				
			break;
		case 3:
			break;
		case 5:
			//����ƽ��0001
		   //----
			CarTurnLR(LeftTurning90N1,LeftTurning90N2,RightTurning90N1,RightTurning90N2);  																			
			break;
		case 10:
			//����ƽ��0010
		   //++++
			CarTurnLR(LeftTurning90Y1,LeftTurning90Y2,RightTurning90Y1,RightTurning90Y2); 
			break;
		  //��������
			
		case 9:
			//����ת0101
			//9090--
			WheelTurnLR(LeftError901,LeftError902,RightTurning90N1,RightTurning90N2); 
			break;
		case 6:
			//����ת0110
		  //9090++
			WheelTurnLR(LeftError901,LeftError902,RightTurning90Y1,RightTurning90Y2); 
			break;
			//ǰ�����������ֵ���
		case 7:
			//����ת1001
			//--9090
			WheelTurnLR(LeftTurning90N1,LeftTurning90N2,RightError901,RightError902); 
			break;
		case 11:
			//����ת0101
			//++9090
			WheelTurnLR(LeftTurning90Y1,LeftTurning90Y2,RightError901,RightError902); 
			break;	
			//���������ǰ�ֵ���	
		case 13:
			//����ת0101
			//9090--
			WheelTurnLR(LeftError901,LeftError902,RightTurning90N1,RightTurning90N2); 
			break;				
		case 14:
			//����ת1001
		 //9090++
			WheelTurnLR(LeftError901,LeftError902,RightTurning90Y1,RightTurning90Y2); 
			break;
		default:
			//ǰ��
		//	OnTurn90New();
			OnTurn90(); 
			break;
		}	
		TempIR3467 = IR3467;
	}
	else
	{
		return;
	}
}
//����1-5��ǰ
void CRobotDlg::RobotRightTurn(int a)
{
	if (TempIR3467 != IR3467)
	{
		switch(IR3467)
		{
		case 0:	
			break;
		case 3:
			break;
		case 5:
			//����ƽ��0001
           //++++
			CarTurnLR(LeftTurning90Y1,LeftTurning90Y2,RightTurning90Y1,RightTurning90Y2); 																		
			break;
		case 10:
			//����ƽ��0010
           //----
			CarTurnLR(LeftTurning90N1,LeftTurning90N2,RightTurning90N1,RightTurning90N2); 
			break;
          //��������			
		case 9:
			//����ת0101
			//--9090
			WheelTurnLR(LeftTurning90N1,LeftTurning90N2,RightError901,RightError902); 
			break;
		case 6:
			//����ת0110
			//++9090
			WheelTurnLR(LeftTurning90Y1,LeftTurning90Y2,RightError901,RightError902); 
			break;
			//ǰ�����������ֵ���
		case 7:
			//����ת1001
			//++9090
			WheelTurnLR(LeftTurning90Y1,LeftTurning90Y2,RightError901,RightError902); 
			break;
		case 11:
			//����ת0101
			//--9090
			WheelTurnLR(LeftTurning90N1,LeftTurning90N2,RightError901,RightError902); 
			break;	
			//���������ǰ�ֵ���	
		case 13:
			//����ת0101
         //9090++
			WheelTurnLR(LeftError901,LeftError902,RightTurning90Y1,RightTurning90Y2); 
			break;				
		case 14:
			//����ת1001
            //9090--
			WheelTurnLR(LeftError901,LeftError902,RightTurning90N1,RightTurning90N2); 
			break;
		default:
			//ǰ��
		//	OnTurn90New();
	    	OnTurn90(); 
			break;
		}	
		TempIR3467 = IR3467;
	}
	else
	{
		return;
	}
}


//ץȡMRI����
void CRobotDlg::DirRobotFront1()
{
  switch(m_Turnstate) 
	{ 
    case 0:
		OnReadHoldPos(); 
        if (m_Holdstate == 3)
        {
            if (m_RevHoldPosition >=(MRIHight-5) && m_RevHoldPosition <= (MRIHight+5))
            {
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;
            }
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������MRI�߶�!");
			}
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"�뽫��������MRI�߶�!");
		}
    	break;		
    case 1:
		tempNum++;
		if (tempNum>=6)
		{
			tempNum=6;
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
                OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
			}
		}
		break;
    case 2:
		tempNum++;
		if (tempNum>=10)
		{
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeedMRI), (MaxFrontSpeed+MaxFrontSpeedMRI));
		}
		break;
	case 3:
		RobotFrontTurn(100);	 
		RevMoveSinglePosition();
		UpdateData(false);
		if (m_RevMovePos>=22000)
		{
			OnMotionMoveing(-150, 150);
		}
		else if (m_RevMovePos>=MaxMRIPosition)
		{
			OnMoveMotionstop();
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
		}
		break;
	case 4:
		RobotFrontTurn(100);
		m_Turnstate =m_Turnstate+1;	
		tempNum=0;
		break; 
    case 5:
		RobotFrontTurn(100);
		if (m_intLeftIR<200&&m_intRightIR<200)
		{
			if (m_intlimited == 191)
			{
				tempNum++;
				if (tempNum >= 4)
				{
					OnMotionstop();
					if (tempNum >= 6)
					{
						OnHoldDownBroad();
						m_Turnstate=m_Turnstate+1;
						tempNum=0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR>=200)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR>=200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;
   case 6:
		if (m_intlimited == 63)
		{
			tempNum++;
			if (tempNum>=30)
			{
				OnMotionMoveing(-200, 200);
			}			
		}
		break;
   case 7:
		RobotFrontTurn(100);
        tempNum++;
	    if (tempNum>=20)
		{
			OnMotionMoveing(-400, 400);			
		}		
        break;	  
   case 8:
	    RobotFrontTurnMRI(100);//�����������ݴ�����!
		tempNum++;
		if (tempNum>=WaitingTime)
		{
			tempNum=WaitingTime;
			 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			 {
				 tempNum=0;
			 	OnMotionstop();
				m_Turnstate =m_Turnstate+1;	
			 }
			 else
			 {
				   if (m_RevMovePos>=MaxMRIPosition)
				   {
					    OnMoveMotionstop();
		   				m_Turnstate =m_Turnstate+1;	
						tempNum=0;
				   }
			 }
		}
        break;	 
   case 9:
		m_intHoldPosition =450;
		UpdateData(false);
		UpdateData(true);
        OnControlHold();
		m_Turnstate =m_Turnstate+1;	
        break;
   case 10:
		tempNum++;
		if (tempNum>=100)
		{
			tempNum=0;
           OnHoldStopBroad();
		   m_Turnstate =m_Turnstate+1;	
		}
       break;
   case 11:
		m_intStat=0;//�����κβ���
		m_Turnstate =m_Turnstate+1;	
        break;
    default:
		break;
    }
}
void CRobotDlg::DirRobotFront(UINT Holdlimited, float limited, float Firstlimited, float Secondlimited)
{
  switch(m_Turnstate) 
	{
    case 0:
		OnReadHoldPos(); 
         if (m_RevHoldPosition >= (Holdlimited - 5) && m_RevHoldPosition <= (Holdlimited + 5))
        {
			OnHoldStopBroad();
			m_Turnstate =m_Turnstate+1;
			DevicePosition.DelayTimes = 0; 
        }
		else
		{
			 SetDlgItemText(IDC_STATICwwd,"�뽫��������PET�߶�!");
		}
    	break;	
   case 1:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}	
			else
			{
                SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
			}
		}
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
   case 2:
	    RobotFrontTurn(100);
		m_Turnstate = m_Turnstate + 1;	
		break;
   case 3:	
	    RobotFrontTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			RevMoveSinglePosition();	
			if (m_RevMovePos >= (Firstlimited - MinPositionError) && m_RevMovePos <= (Firstlimited + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
				DevicePosition.DelayTimes = 0;
			}	
			else if (m_RevMovePos >= (Firstlimited + MinPositionError))
			{
				OnMoveMotionstop(); 
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
			}
		}
    	break; 
    case 4:
		RobotFrontTurn(100);
		if (m_intLeftIR < 300 && m_intRightIR < 200)
		{
			if (m_intlimited == 63)
			{
				DevicePosition.DelayTimes++;
				if (DevicePosition.DelayTimes >= HoldDelayTimer)
				{
					OnMotionstop();
					if (DevicePosition.DelayTimes >= HoldDelayTimer*3)
					{
						OnHoldDownBroad();
						m_Turnstate = m_Turnstate+1;
						DevicePosition.DelayTimes = 0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR >= 300)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR >= 200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;
   case 5:
		if (m_intlimited == 0)
		{
			DevicePosition.DelayTimes++;
			if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
			{		
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}
		}
		else
		{
            OnHoldDownBroad();
            SetDlgItemText(IDC_STATICwwd,"��������˶�������!");
		}
		break;
   case 6:
		RobotFrontTurn(100);
        RevMoveSinglePosition();
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
		    OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
		}
		if (m_RevMovePos >= (Secondlimited - MinPositionError) && m_RevMovePos <= (Secondlimited + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);			
		}	
		else if (m_RevMovePos >= (Secondlimited + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}			
        break;	
    case 7:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (limited - PositionError) && m_RevMovePos <= (limited + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (limited + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 8:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (limited + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break; 
   case 9:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			m_intHoldPosition = 420;
			UpdateData(false);
			UpdateData(true);
			OnControlHold();
			m_Turnstate =m_Turnstate+1;	
			DevicePosition.DelayTimes = 0;
		}
        break;
   case 10:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer * 4) 
		{
			DevicePosition.DelayTimes=0;
            OnHoldStopBroad();
		    m_Turnstate =m_Turnstate+1;	
		}
       break;
   case 11:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;
    default:
		break;
    }
}
//�뿪�豸

void CRobotDlg::DirRoboRear(float limted)
{
  switch(m_Turnstate) 
	{
    case 0:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{	
			m_Turnstate = m_Turnstate+1;				
		}
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
    case 1:
	    RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{	
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);
			DevicePosition.DelayTimes = 0;
		}
    	break;
    case 2:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed), -(MaxFrontSpeed));
			DevicePosition.DelayTimes = 0;
		}	
    case 3:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed), -MaxFrontSpeed);
			DevicePosition.DelayTimes = 0;
		}
    case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (limted - PositionError) && m_RevMovePos <= (limted + PositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);	
		}	
		else if (m_RevMovePos >= (limted + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
     case 5:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (limted + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
    case 6:
		RobotRearTurn(100);
		if (IR[10] == 1 || IR[7] == 1)
		{			
			OnMoveMotionstop(); 		
			m_Turnstate = m_Turnstate+1;
		}
		if (m_RevMovePos >= (limted + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
   case 7:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;		
	default:
		break;
	}
}
//�ͷŴ���

void CRobotDlg::DirRoboLeftRight(float limited)
{
  switch(m_Turnstate) 
	{
    case 0:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{
			OnReadHoldPos(); 
		}
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{	
			DevicePosition.DelayTimes = HoldDelayTimer*2;	
			if (m_RevHoldPosition >= DevicePosition.TOPHoldPosition)
			{
				RevTurnPosition();
				if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
				{  
					OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
				}
				else
				{
                    SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
				}
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�뽫����������TOP���塱�߶�!");
				DevicePosition.DelayTimes=0;
			}
		}
		break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
            DevicePosition.DelayTimes = 0;
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeed1),(MaxFrontSpeed+MaxFrontSpeed1));
		}
		break;
    case 2:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (limited - PositionError) && m_RevMovePos <= (limited + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				m_Turnstate = m_Turnstate + 1;
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (limited + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 2;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 3:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (limited - PositionError) && m_RevMovePos <= (limited + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (limited + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 4:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (limited + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break;
    case 5:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 63)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldStopBroad();
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 6:
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 63)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldDownBroad(); 
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 7:		
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer * 50)
		{
			OnHoldStopBroad();
			DevicePosition.DelayTimes =0;
			m_Turnstate = m_Turnstate + 1;	
		}
		break;
    case 8:		
		m_intStat=0;
		break;
    default:
		break;
    }
}
//ץȡPET��CT����
void CRobotDlg::DirRobotFront()
{
  switch(m_Turnstate) 
	{
    case 0:
		OnReadHoldPos(); 
         if (m_RevHoldPosition >= (DevicePosition.PETHoldPosition-5) && m_RevHoldPosition <= (DevicePosition.PETHoldPosition+5))
        {
			OnHoldStopBroad();
			m_Turnstate =m_Turnstate+1;
        }
		else
		{
			 SetDlgItemText(IDC_STATICwwd,"�뽫��������PET�߶�!");
		}
    	break;	
   case 1:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}	
			else
			{
                SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
			}
		}
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
   case 2:
	    RobotFrontTurn(100);
		m_Turnstate = m_Turnstate + 1;	
		break;
   case 3:	
	    RobotFrontTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
			DevicePosition.DelayTimes = 0;
		}	
		RevMoveSinglePosition();
		if (IR[2] == 1 && IR[3] == 1)
		{
			OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}
		if (m_RevMovePos >= (DevicePosition.FisrtPET - MinPositionError) && m_RevMovePos <= (DevicePosition.FisrtPET + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}	
		else if (m_RevMovePos >= (DevicePosition.FisrtPET + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;	
   case 4:	
	    RobotFrontTurn(100);	
		RevMoveSinglePosition();
		if (IR[2] == 1 && IR[3] == 1)
		{
			OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}
		if (m_RevMovePos >= (DevicePosition.FisrtPET - MinPositionError) && m_RevMovePos <= (DevicePosition.FisrtPET + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}	
		else if (m_RevMovePos >= (DevicePosition.FisrtPET + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;	
    case 5:
		RobotFrontTurn(100);
		if (m_intLeftIR < 300 && m_intRightIR < 200)
		{
			if (m_intlimited == 63)
			{
				DevicePosition.DelayTimes++;
				if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
				{
					OnMotionstop();
					if (DevicePosition.DelayTimes >= HoldDelayTimer*3)
					{
						OnHoldDownBroad();
						m_Turnstate=m_Turnstate+1;
						DevicePosition.DelayTimes = 0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR >= 300)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR >= 200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;
   case 6:
		if (m_intlimited == 63)
		{
			DevicePosition.DelayTimes++;
			if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
			{		
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}
		}
		break;
   case 7:
		RobotFrontTurn(100);
        RevMoveSinglePosition();
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
		    OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
		}
		if (m_RevMovePos >= (DevicePosition.SecondPET - MinPositionError) && m_RevMovePos <= (DevicePosition.SecondPET + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);			
		}	
		else if (m_RevMovePos >= (DevicePosition.SecondPET + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}			
        break;	
    case 8:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 9:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break; 
   case 10:
		m_intHoldPosition =450;
		UpdateData(false);
		UpdateData(true);
        OnControlHold();
		m_Turnstate =m_Turnstate+1;	
        break;
   case 11:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= 50)
		{
			DevicePosition.DelayTimes=0;
            OnHoldStopBroad();
		    m_Turnstate =m_Turnstate+1;	
		}
       break;
   case 12:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;
    default:
		break;
    }
}
//PET�뿪
void CRobotDlg::DirRoboRear()
{
  switch(m_Turnstate) 
	{
    case 0:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{	
			OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);					
		}
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
    case 1:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}	
    case 2:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
    case 3:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);	
		}	
		else if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
     case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
    case 5:
		RobotRearTurn(100);
		if (IR[10] == 1 || IR[7] == 1)
		{			
			OnMoveMotionstop(); 		
			m_Turnstate = m_Turnstate+1;
		}
		if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
   case 6:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;		
	default:
		break;
	}
}
//ֱ�ӷ�PET����
void CRobotDlg::DirRoboLeftRight()
{
  switch(m_Turnstate) 
	{
    case 0:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{
			OnReadHoldPos(); 
		}
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{	
			DevicePosition.DelayTimes = HoldDelayTimer*2;	
			if (m_RevHoldPosition >= 420)
			{
				RevTurnPosition();
				if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
				{  
					OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
				}
				else
				{
                    SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
				}
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�뽫����������TOP���塱�߶�!");
				DevicePosition.DelayTimes=0;
			}
		}
		break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
            DevicePosition.DelayTimes = 0;
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeed1),(MaxFrontSpeed+MaxFrontSpeed1));
		}
		break;
    case 2:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				m_Turnstate = m_Turnstate + 2;
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 3;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 3:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				m_Turnstate = m_Turnstate + 1;
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 2;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 4:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 5:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break;
    case 6:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldStopBroad();
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 7:
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldDownBroad(); 
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 8:		
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer * 50)
		{
			OnHoldStopBroad();
			DevicePosition.DelayTimes =0;
			m_Turnstate = m_Turnstate + 1;	
		}
		break;
    case 9:		
		m_intStat=0;
		break;
    default:
		break;
    }
}
//ֱ��ץCT
void CRobotDlg::DirRobotFront2()
{
  switch(m_Turnstate) 
	{
    case 0:
		OnReadHoldPos(); 
         if (m_RevHoldPosition >= (DevicePosition.PETHoldPosition-5) && m_RevHoldPosition <= (DevicePosition.PETHoldPosition+5))
        {
			OnHoldStopBroad();
			m_Turnstate = m_Turnstate + 1;
        }
		else
		{
			 SetDlgItemText(IDC_STATICwwd,"�뽫��������CT�߶�!");
		}
    	break;	
   case 1:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}	
			else
			{
				SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
			}
		} 
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
   case 2:
	    RobotFrontTurn(100);
		m_Turnstate = m_Turnstate + 1;	
		break;	  
    case 3:	
	    RobotFrontTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
			DevicePosition.DelayTimes = 0;
		}	
		RevMoveSinglePosition();
		if (IR[2] == 1 && IR[3] == 1)
		{
			OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}
		if (m_RevMovePos >= (DevicePosition.FisrtCT - MinPositionError) && m_RevMovePos <= (DevicePosition.FisrtCT + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}	
		else if (m_RevMovePos >= (DevicePosition.FisrtCT + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;	
   case 4:	
	    RobotFrontTurn(100);	
		RevMoveSinglePosition();
		if (IR[2] == 1 && IR[3] == 1)
		{
			OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}
		if (m_RevMovePos >= (DevicePosition.FisrtCT - MinPositionError) && m_RevMovePos <= (DevicePosition.FisrtCT + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			DevicePosition.DelayTimes = 0;
		}	
		else if (m_RevMovePos >= (DevicePosition.FisrtCT + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;	
    case 5:
		RobotFrontTurn(100);
		if (m_intLeftIR < 300 && m_intRightIR < 200)
		{
			if (m_intlimited == 63)
			{
				DevicePosition.DelayTimes++;
				if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
				{
					OnMotionstop();
					if (DevicePosition.DelayTimes >= HoldDelayTimer*3)
					{
						OnHoldDownBroad();
						m_Turnstate=m_Turnstate+1;
						DevicePosition.DelayTimes = 0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR >= 300)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR >= 200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;
   case 6:
		if (m_intlimited == 63)
		{
			DevicePosition.DelayTimes++;
			if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
			{		
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);	
			}
		}
		break;
   case 7:
		RobotFrontTurn(100);
        RevMoveSinglePosition();
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
		    OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
		}
		if (m_RevMovePos >= (DevicePosition.SecondCT - MinPositionError) && m_RevMovePos <= (DevicePosition.SecondCT + MinPositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);			
		}	
		else if (m_RevMovePos >= (DevicePosition.SecondCT + MinPositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}			
        break;	
    case 8:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 9:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break; 
   case 10:
		m_intHoldPosition =450;
		UpdateData(false);
		UpdateData(true);
        OnControlHold();
		m_Turnstate =m_Turnstate+1;	
        break;
   case 11:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= 50)
		{
			DevicePosition.DelayTimes=0;
            OnHoldStopBroad();
		    m_Turnstate =m_Turnstate+1;	
		}
       break;
   case 12:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;
    default:
		break; 
    }
}
//CT�뿪
void CRobotDlg::DirRoboRear2()
{
  switch(m_Turnstate) 
	{
   case 0:
	    RobotFrontTurn(100);
		if (IR[2] == 1 && IR[3] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{	
			OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);					
		}
		else
		{	
			SetDlgItemText(IDC_STATICwwd,"��ȷ�ϴӳ�ʼ�߳���!");
		}	
    	break;
    case 1:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}	
    case 2:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
    case 3:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);	
		}	
		else if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
     case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
    case 5:
		RobotRearTurn(100);
		if (IR[10] == 1 || IR[7] == 1)
		{			
			OnMoveMotionstop(); 		
			m_Turnstate = m_Turnstate+1;
		}
		if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
   case 6:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;		
	default:
		break;
	}
}
//ֱ�ӷ�CT����
void CRobotDlg::DirRoboLeftRight2()
{
  switch(m_Turnstate) 
	{
    case 0:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{
			OnReadHoldPos(); 
		}
		if (DevicePosition.DelayTimes >= HoldDelayTimer*2)
		{	
			DevicePosition.DelayTimes = HoldDelayTimer*2;	
			if (m_RevHoldPosition >= 420)
			{
				RevTurnPosition();
				if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
				{  
					OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);
				}
				else
				{
                    SetDlgItemText(IDC_STATICwwd,"��ȷ������ת��Ƕ�!");
				}
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�뽫����������TOP���塱�߶�!");
				DevicePosition.DelayTimes=0;
			}
		}
		break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
            DevicePosition.DelayTimes = 0;
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeed1),(MaxFrontSpeed+MaxFrontSpeed1));
		}
		break;
    case 2:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 3:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 4:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop();
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 5:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break;
    case 6:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldStopBroad();
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 7:
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldDownBroad(); 
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 8:		
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer * 50)
		{
			OnHoldStopBroad();
			DevicePosition.DelayTimes =0;
			m_Turnstate = m_Turnstate + 1;	
		}
		break;
    case 9:		
		m_intStat=0;
		break;
    default:
		break; 
    }
}
//PET-CT
void CRobotDlg::RobotLeft(UINT SelNum)
{	
  switch(m_Turnstate) 
	{
    case 0:
		RevTurnPosition();//IR[4]==0 &&
    	if ( m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
		{
			OnReadHoldPos();
			if (m_RevHoldPosition >=(DevicePosition.TOPHoldPosition - 5) )
            {	
				OnMoveZero();
				OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);				
            }
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������TOP�߶�,�밴TOP�߶Ȱ�ť!");
			}
		}
		else
		{
             SetDlgItemText(IDC_STATICwwd,"����ת�����!");
		}
    	break;
    case 1:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed + MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
    	break;
     case 2:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);	
		}	
		else if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
     case 3:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
    case 4:
		RobotRearTurn(100);
		if (IR[10] == 1 || IR[7] == 1)
		{			
			OnMoveMotionstop(); 
			OnTurn90();//ת����90��
			m_Turnstate = m_Turnstate+1;
		}
		if (m_RevMovePos >= (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;  		
    case 5:	
		DevicePosition.DelayTimes ++;		
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{	
			DevicePosition.DelayTimes = LeaveTimer;			
			RevTurnPosition();
			OnMoveZero();//�˶��������			
			if (m_RevTurnPos1 == 884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900 && m_RevTurnPos7 == 883)
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;	
			}
			else
			{
                SetDlgItemText(IDC_STATICwwd,"90������ת�򲻵�λ!");
			}
		}
    	break;
    case 6:	
		RobotLeftTurn(100);//�������
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeed1), (MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
    	break;
    case 7:	
		RobotLeftTurn(100);//�������
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
            DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if (IR[0] == 1 || IR[1] == 1)
			{
				OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);
				DevicePosition.DelayTimes = 0;
				m_Turnstate = m_Turnstate + 1;//������9��
			}
			if (m_RevMovePos >= (DevicePosition.PETCT - PositionError) && m_RevMovePos <= (DevicePosition.PETCT + PositionError))//��37��ǰ�ߵ�һ�����ź�
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
			{
				DevicePosition.DelayTimes = 0;
				OnMoveMotionstop();
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");	
			}
		}
    	break;	
   case 8:	   
		RobotLeftTurn(100);//�������  
		RevMoveSinglePosition();
		if (IR[0] == 1 || IR[1] == 1)
		{	
			OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);
		}
		if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
		{
			OnMoveMotionstop();
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");	
		}
    	break;
    case 9:
		RobotLeftTurn(100);//�������
		if (IR[8] == 1 || IR[9] == 1)
		{
			OnMoveMotionstop();//����ֹͣ
			OnReZeroPosition();//ת��λ
			OnMoveZero();//�˶��������
			m_Turnstate =m_Turnstate+1;	
		}
		RevMoveSinglePosition();
		if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
		{
			OnMoveMotionstop();
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");	
		}
    	break;
    case 10:
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{	
			DevicePosition.DelayTimes = LeaveTimer;			
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
				DevicePosition.DelayTimes = 0;
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�����˸�λ�ǶȲ���!");				
			}
		}
		break;
   case 11:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
			//	OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				m_Turnstate = m_Turnstate + 1;
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
   case 12:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.CT + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 13:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break; 	
    case 14:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			DevicePosition.DelayTimes = HoldDelayTimer;
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldStopBroad();
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;  
    case 15:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			DevicePosition.DelayTimes = HoldDelayTimer;
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldDownBroad(); 
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 16:		
		m_intStat = 0;
		SetDlgItemText(IDC_STATICwwd,"�˲����������!");
		break;
    default:
		break;
    }
}


//PET�뿪����MRI.
void CRobotDlg::DirRoboRear3()
{
  switch(m_Turnstate) 
	{
    case 0:
 		OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);
    	break;
    case 1:
 		RobotRearTurn(100);
		OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
    	break;		
	case 2:
		RobotRearTurn(100);
		m_Turnstate =m_Turnstate+1;
		break;	
	case 3:
		tempNum++;
		RobotRearTurn(100);
		if (tempNum>=10)
		{
			tempNum=20;
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>=(MaxPETMRIPosition-3000) && m_RevMovePos<=MaxPETMRIPosition)
			{
				OnMotionMoveing(400, -400);
			}
			else if (m_RevMovePos>MaxPETMRIPosition)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
		break;
	case 4:
		tempNum++;
		m_CountNumLimit++;
		RobotRearTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
			if (m_RevMovePos>MaxPETMRIPosition)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
			else
			{	
				if (IR[2]==1&&IR[3]==1)
				{
					OnMoveMotionstop(); 
					m_Turnstate =m_Turnstate+1;	
					tempNum=0;
				}
				else
				{
					if (m_RevMovePos>MaxPETMRIPosition)//103
					{
						OnMoveMotionstop(); 
						m_Turnstate =m_Turnstate+1;	
						tempNum=0;
						SetDlgItemText(IDC_STATICwwd,"û��⵽����!������ǰ����.");
					}
				}
			}
		break;	
	default:
		break;
	}
}

//MRI�뿪
void CRobotDlg::DirRoboRear1()
{
  switch(m_Turnstate) 
	{
    case 0:
		OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);	
    	break;
	case 1:
		RobotRearTurnMRI(100);
		tempNum++;
		if (tempNum >=150)
		{
		 OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeedMRI), -(MaxFrontSpeed+MaxFrontSpeedMRI));
		}
		break;
	case 2:
		RobotRearTurnMRI(100);	
		m_Turnstate =m_Turnstate+1;	
		break;
	case 3:
		RobotRearTurn(100);
		tempNum++;
		if (tempNum>=10)
		{	
		tempNum=10;
		RevMoveSinglePosition();
		UpdateData(false);
		if (m_RevMovePos>=(MaxMRIPosition-3000) && m_RevMovePos<=MaxMRIPosition)
		{
			OnMotionMoveing(400, -400);
		}
		else if (m_RevMovePos>MaxMRIPosition)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		}
		break;
	case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
		 tempNum=0;
		OnMotionstop();
		m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxMRIPosition)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		break;
	default:
		break;
	}
}
//ֱ�ӷ�MRI����
void CRobotDlg::DirRoboLeftRight1()
{
  switch(m_Turnstate) 
	{
    case 0:
		tempNum++;
		if (tempNum==WaitingTime-1)
		{
			OnReadHoldPos(); 
		}
		if (tempNum>=WaitingTime)
		{	
			tempNum=WaitingTime;	
			if (m_RevHoldPosition >= 420)
			{
				RevTurnPosition();
				if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
				{  
					OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeedMRI), (MaxFrontSpeed+MaxFrontSpeedMRI));
				}
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�뽫�����������Ŵ��塱�߶�!");
				m_intStat=0;
				tempNum=0;
			}
		}
		break;
	case 1:
		RobotFrontTurn(100);
		m_Turnstate =m_Turnstate+1;
		break;
	case 2:
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=50)
		{	
			tempNum=50;
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>=(MaxMRIPosition-3000) && m_RevMovePos<=MaxMRIPosition)
			{
			   OnMotionMoveing(-400, 400);

			}
			else if (m_RevMovePos>MaxMRIPosition)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
	break;
	case 3:
		RobotFrontTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		 {
			 tempNum=0;
			OnMotionstop();
			m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxMRIPosition)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
	break;	
	case 4:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;			
			}
		}
	break;
    case 5:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldDownBroad(); 
				m_Turnstate =m_Turnstate+1;			
			}
		}
		break;
    case 6:		
		tempNum++;
		if (tempNum>=50)
		{
			OnHoldStopBroad();
			tempNum =0;
			m_Turnstate =m_Turnstate+1;			

		}
		break;
    case 7:		
		m_intStat=0;
		break;

    default:
		break;
    }
}
//MRI��PET��ͷ
void CRobotDlg::DirRoboTuring90()
{
  switch(m_Turnstate) 
	{
    case 0:
		tempNum++;
		if (tempNum==WaitingTime-1)
		{
			OnReadHoldPos(); 
		}
		if (tempNum>=WaitingTime)
		{	
			tempNum=WaitingTime;	
			if (m_RevHoldPosition >= 420)
			{
				OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeedMRI), -(MaxFrontSpeed+MaxFrontSpeedMRI));
			}
			else
			{
		        SetDlgItemText(IDC_STATICwwd,"�뽫�����������Ŵ��塱�߶�!");
				m_intStat=0;
				tempNum=0;
			}
		}
		break;
      case 1:
		tempNum++;
		RobotRearTurn(100);
		if (tempNum>=20)
		{
			tempNum=0;
            m_Turnstate =m_Turnstate+1;
		}
    	break;	
      case 2:
		RobotRearTurn(100);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
		 tempNum=0;
		OnMotionstop();
		m_Turnstate =m_Turnstate+1;	
		}
    	break;

    case 3:
        OnTurn57();
		OnMoveZero();
		m_Turnstate =m_Turnstate+1;			
	    break;
    case 4:
		tempNum++;
		if (tempNum>=28)
		{	
			tempNum =41;
			RevTurnPosition();
			if (m_RevTurnPos1 == -571 && m_RevTurnPos3 == 571 && m_RevTurnPos5 == 571&& m_RevTurnPos7 == -571)
			{     
				int tempVelocity = 30*36;
				MoveDiffVelocityControl(tempVelocity);
				tempNum=0;
				m_Turnstate =m_Turnstate+1;
			}	
		}
    	break;
    case 5:	
		tempNum++;
		if (tempNum>=100)
		{   
			tempNum =100;
			RevMovePosition();
			if (m_RevTurnPos2 >= 26600)
			{
				OnMotionMoveing(104, 170);
			}
		}			
		break;		
    case 6:
		RevMovePosition();
		if (m_RevTurnPos2 >= 27550)//11400
		{		
			OnMoveMotionstop();  	
			m_Turnstate =m_Turnstate+1;				 
		}
    	break;
    case 7:
		tempNum++;
		if (tempNum>=2)
		{   
			tempNum = 0;	
			int temp1 = -225;//90 / 4 = 22.5
			int temp2 = -225;//90 / 4 = 22.5
            OnTurnAngle(temp1, temp1, temp2, temp2);//ת�����22.5     
			m_Turnstate =m_Turnstate+1;	
		}
    	break;	
    case 8:
		tempNum++;
		if (tempNum>=40)
		{ 
			OnMotionMoveing(400, -400);
        }
    	break;
    case 9:
		tempNum++;
		if (tempNum>=4)
		{ 	
			tempNum=5;
			if (IR1291==5||IR1291==10||IR1291==9||IR1291==6||IR1291==7||IR1291==11||IR1291==13||IR1291==14)
			{
				OnMoveMotionstop(); 
				OnReZeroPosition();	
				m_Turnstate =m_Turnstate+1;	
		    	tempNum=0;	
			}
        }
    	break;
    case 10:
		tempNum++;
		if (tempNum>=2)
		{ 
			OnMotionMoveing(400, -400);
        }
    	break;
      case 11:
		tempNum++;
		RobotRearTurn(100);
		if (tempNum>=50)
		{ 	
			tempNum=50;
			 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			 {
				 tempNum=0;
			 	OnMotionstop();
				m_Turnstate =m_Turnstate+1;	
			 }
        }
    	break;
	  default:
    	break;
	}
}
//PET��MRI��ͷ
void CRobotDlg::DirRoboTuring90PET()
{
  switch(m_Turnstate) 
	{
    case 0:
		tempNum++;
		if (tempNum==WaitingTime-1)
		{
			OnReadHoldPos(); 
		}
		if (tempNum>=WaitingTime)
		{	
			tempNum=WaitingTime;	
			if (m_RevHoldPosition >= 420)
			{
                OnTurn57();	
				tempNum=0;
				m_Turnstate =m_Turnstate+1;
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�뽫�����������Ŵ��塱�߶�!");
				m_intStat=0;
				tempNum=0;
			}
		}
		break;
	case 1:            
		m_Turnstate =m_Turnstate+1;
		break;	
	case 2:
		m_Turnstate =m_Turnstate+1;	
		break;
    case 3:
		tempNum++;
		if (tempNum>=10)
		{	        
			OnMoveZero();
			m_Turnstate =m_Turnstate+1;	
            tempNum=0;
		}
	    break;
    case 4:
		tempNum++;
		if (tempNum>=20)
		{	
			tempNum =41;
			RevTurnPosition();
			if (m_RevTurnPos1 == -571 && m_RevTurnPos3 == 571 && m_RevTurnPos5 == 571&& m_RevTurnPos7 == -571)
			{ 
				int tempVelocity = -30*36;
				MoveDiffVelocityControl(tempVelocity);
				tempNum=0;
				m_Turnstate =m_Turnstate+1;
			}	
		}
    	break;
    case 5:	
		tempNum++;
		if (tempNum>=100)
		{   
			tempNum =100;
			RevMovePosition();
			if (m_RevTurnPos2 <= -26600)
			{
				OnMotionMoveing(-104, -170);
			}
		}			
		break;		
    case 6:
		RevMovePosition();
		if (m_RevTurnPos2 <= -27620)//11400
		{
			OnMoveMotionstop(); 
			m_Turnstate =m_Turnstate+1;				 
		}
    	break;
    case 7:
		tempNum++;
		if (tempNum>=2)
		{   
			tempNum   = 0;	
			int temp1 = 225;//90 / 4 = 22.5
			int temp2 = 225;//90 / 4 = 22.5
            OnTurnAngle(temp1, temp1, temp2, temp2);//ת�����22.5     
			m_Turnstate =m_Turnstate+1;	
		}
    	break;	
    case 8:
		tempNum++;
		if (tempNum>=40)
		{ 
			OnMotionMoveing(-400, 400);	
        }
    	break;
    case 9:
		tempNum++;
		if (tempNum>=5)
		{ 	
			tempNum=5;
			if (IR1291==5||IR1291==10||IR1291==9||IR1291==6||IR1291==7||IR1291==11||IR1291==13||IR1291==14)
			{
				OnMoveMotionstop(); 
				OnReZeroPosition();	
				m_Turnstate =m_Turnstate+1;	
		    	tempNum=0;	
			}
        }
    	break;
    case 10:
		tempNum++;
		if (tempNum>=2)
		{
			OnMotionMoveing(-400, 400);
        }
    	break;
	case 11:
		tempNum++;
		RobotFrontTurn(100);
		if (tempNum>=50)
		{ 
			tempNum =51;
			 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			 {
				tempNum=0;
				OnMotionstop();
				m_Turnstate =m_Turnstate+1;	
			 }	
			 if (IR[5]==1&&IR[6]==1)
			 {
				tempNum=0;
				OnMotionstop();
				m_Turnstate =m_Turnstate+1;	
			 }
		}
    	break;
	default:
		break;
	}
}
void CRobotDlg::RobotFront(UINT SelNum)
{
   switch(m_Turnstate) 
	{
    case 0:
		OnReadHoldPos(); 
		 if (m_Holdstate == 1)
        {
            if (m_RevHoldPosition >=(DevicePosition.PETHoldPosition-5) && m_RevHoldPosition <= (DevicePosition.PETHoldPosition+5))
            {
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;
            }
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������PET�߶�!");
			}
		}
		else if (m_Holdstate == 2)
		{
            if (m_RevHoldPosition >=245 && m_RevHoldPosition <= 255)
            {
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;
            }	
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������CT�߶�!");
			}
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"�����ô���߶�!");
		}
    	break;
    case 1:
		tempNum++;
		if (tempNum>=6)
		{	
			tempNum =6;		
			RevTurnPosition();
			if ( m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MaxFrontSpeed/2, MaxFrontSpeed/2);	
			}
			else if (statereset==0)	
			{
				statereset=1;
				OnReZeroPosition();
			}
		}
    	break;
    case 2:
		tempNum++;
		if (tempNum>=2)
		{
			OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
		}
    	break;
    case 3:
		RobotFrontTurn(100);
//��Ӧ��ͼ�ϵ�ֵ1-[1];2-[0];3-[2];4-[3];5-[4];6-[5];7-[7];8-[6];11-[9];22-[8]
        tempNum++;
		if (tempNum>=WaitingTime)
		{
             tempNum=WaitingTime;
		     if ( IR[7]==1||IR[10]==1)//4-7	IR[3]==1||||IR[10]==1	
			 {
				 OnMotionMoveing(-MinFrontSpeed, MinFrontSpeed);
			 }
		}
    	break;
    case 4:
		RobotFrontTurn(100);
		if (IR[5]==1)
		{
			DeviceNumber++;	 
			if (DeviceNumber == SelNum)
			{
				OnMoveMotionstop(); 
				DeviceNumber = 0;
				m_Turnstate =m_Turnstate+1;		
			}
			else
			{
				m_Turnstate=0;
			}
		}
    	break;
    case 5:
        OnTurnZero();//һ��������ת
		OnMoveZero();
		m_Turnstate =m_Turnstate+1;			
	    break;
    case 6:
		tempNum++;
		if (tempNum>=2)
		{	
			tempNum =3;
			RevTurnPosition();
			if (m_RevTurnPos1 == -909&& m_RevTurnPos3 == 369 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -394)
			{
				OnMotionMoveing(-300, -489);
			}
		}
    	break;
    case 7:	
		tempNum++;
		if (tempNum>=4)
		{   
			tempNum =5;
			RevMovePosition();
			if (m_RevTurnPos2 <= -10800)
			{
				OnMotionMoveing(-52, -85);
			}
		}			
		break;		
    case 8:
		RevMovePosition();
		if (m_RevTurnPos2 <= -11800)//11400
		{
			OnMoveMotionstop(); 	
			m_Turnstate =m_Turnstate+1;				 
		}
    	break;
    case 9:
		tempNum++;
		if (tempNum>=2)
		{   
			tempNum =0;		
            OnReZeroPosition();          
			m_Turnstate =m_Turnstate+1;	
		}
    	break;			
    case 10:
		tempNum++;
		if (tempNum>=2)
		{
			tempNum=3;
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				RobotFrontTurn(100);
				OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
			}
		}
		break;
    case 11:	
		RobotFrontTurn(100);
        tempNum++;
		if (tempNum>=WaitingTime)//wwd3.9
		{
             tempNum=0;
			 m_Turnstate =m_Turnstate+1;	
		}
 		break;
   case 12:	
	    RobotFrontTurn(100);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
		 OnMotionMoveing(-150, 150);
		}		
		break;
    case 13:
		RobotFrontTurn(100);
		if (m_intLeftIR<200&&m_intRightIR<200)
		{
			if (m_intlimited == 191)
			{
				tempNum++;
				if (tempNum >= 4)
				{
					OnMotionstop();
					if (tempNum >= 6)
					{
						OnHoldDownBroad();
						m_Turnstate=m_Turnstate+1;
						tempNum=0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR>=200)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR>=200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;	
   case 14:
		if (m_intlimited == 63)
		{
			tempNum++;
			if (tempNum>=10)
			{
				OnMotionMoveing(-200, 200);
			}
		}
		break;
   case 15:
		RobotFrontTurn(100);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
		 OnMotionMoveing(-600, 600);
		}	
		break;
   case 16:
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=WaitingTime)//wwd3.9
		{
			tempNum=10;
			 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			 {
				 tempNum=0;
			 	 OnMotionstop();
			     m_Turnstate =m_Turnstate+1;	
			 }
		}
        break;	 
   case 17:		
		OnHoldStopBroad();
		m_Turnstate =m_Turnstate+1;	
        break;
   case 18:
		tempNum++;
		if (tempNum>=2)
		{   
			tempNum =0;
			m_intHoldPosition =450;
			UpdateData(false);
			UpdateData(true);
			OnControlHold();
	    	m_Turnstate =m_Turnstate+1;	
		}
        break;
   case 19:
		tempNum++;
		if (tempNum>=50)
		{
			tempNum=0;
           OnHoldStopBroad();
		   m_Turnstate =m_Turnstate+1;	
		}
       break;
   case 20:
		m_intStat=0;
		m_Turnstate =m_Turnstate+1;	
        break;
    default:
		break;
    }
}
void CRobotDlg::RobotRear(UINT SelNum)
{
  switch(m_Turnstate) 
	{
    case 0:
        RevTurnPosition();
		if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
		{
			OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);
		}	
    	break;
	case 1:
		m_Turnstate =m_Turnstate+1;
		break;
	case 2:
		RobotRearTurn(100);
		if (IR[5]==1||IR[6]==1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		break;	
	case 3:
		RobotRearTurn(100);
		if (IR[3]==1&&IR[7]==1)
		{
			OnMoveMotionstop(); 
			m_Turnstate =m_Turnstate+1;	
		}
    	break;
    case 4:
        OnTurnZero();//һ��������ת
		OnMoveZero();//�����˶�����
		m_Turnstate =m_Turnstate+1;			
	    break;
    case 5:
		RevTurnPosition();
		if (m_RevTurnPos1 == -909&& m_RevTurnPos3 == 369 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -394)
		{
			OnMotionMoveing(300, 489);
		}
    	break;
    case 6:	
		tempNum++;
		if (tempNum >=20)
		{
			tempNum =20;
			RevMovePosition();
			if (m_RevTurnPos2 >= 10800)
			{
				OnMotionMoveing(52, 85);
			}
		}		
		break;		
    case 7:
		RevMovePosition();
		if (m_RevTurnPos2 >= 11800)//11400
		{			 
			OnMoveMotionstop();          
			m_Turnstate =m_Turnstate+1;	
		}
    	break;
    case 8:	
        OnReZeroPosition();          
		m_Turnstate =m_Turnstate+1;	
    	break;			
    case 9:
		RevTurnPosition();
	    if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
		{
			OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);
		}
		break;
    case 10:
		RobotRearTurn(100);	
		tempNum1++;
		if (tempNum1>=10)
		{
			tempNum1=11;
			if (IR[2]==1&&IR[3]==1&&IR[5]!=1&&IR[7]!=1&&IR[10]!=1&&IR[6]!=1)
			{
				OnMotionstop();	
				tempNum = 0;
				DeviceNumber =0;
				m_Turnstate =m_Turnstate+1;
			} 
		}			
		break;
    case 11:
		m_intStat =0;
		break;
    default:
		break;
    }
}
//CT-PET
void CRobotDlg::RobotRight(UINT SelNum)
{
    switch(m_Turnstate) 
	{
    case 0:
		RevTurnPosition();
		if ( m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
		{
			OnReadHoldPos();
			if (m_RevHoldPosition >= (DevicePosition.TOPHoldPosition-5) )
			{
				OnMoveZero();				
				OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);	
				tempNum = 0;
			}
			else
			{
				 SetDlgItemText(IDC_STATICwwd,"�뽫��������TOP�߶�,�밴TOP�߶Ȱ�ť!");
			}		
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"����ת�����!");
		}
    	break;
    case 1:
		RobotRearTurn(100);
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed + MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
		break;
    case 2:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[6] == 1)
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.CT - PositionError) && m_RevMovePos <= (DevicePosition.CT + PositionError))//��37��ǰ�ߵ�һ�����ź�
		{
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}	
		else if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
	 case 3:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[7] == 1)
		{
			OnMoveMotionstop();
			m_Turnstate = m_Turnstate+1;
			//OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
		}
		if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break;
     case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		if (IR[5] == 1 || IR[7] == 1)
		{			
			OnMoveMotionstop(); 
			OnTurn90();//ת����90��
			m_Turnstate = m_Turnstate+1;
		}
		if (m_RevMovePos >= (DevicePosition.CT + PositionError))
		{
			OnMoveMotionstop(); 
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");
		}
    	break; 
    case 5: 
		DevicePosition.DelayTimes ++;		
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{	
			DevicePosition.DelayTimes = LeaveTimer;			
			RevTurnPosition();
			OnMoveZero();//�˶��������			
			if (m_RevTurnPos1 == 884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900 && m_RevTurnPos7 == 883)
			{
				OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);
				DevicePosition.DelayTimes = 0;	
			}
			else
			{
                SetDlgItemText(IDC_STATICwwd,"90������ת�򲻵�λ!");
			}
		}
    	break;
    case 6: 
		RobotRightTurn(100);//�������
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeed1), -(MaxFrontSpeed+MaxFrontSpeed1));
			DevicePosition.DelayTimes = 0;
		}
    	break;
    case 7:	
		RobotRightTurn(100);//�������
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
            DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if (IR[8] == 1 || IR[9] == 1)
			{
				OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);
				DevicePosition.DelayTimes = 0;
				m_Turnstate = m_Turnstate + 1;//������9��
			}
			if (m_RevMovePos >= (DevicePosition.PETCT - PositionError) && m_RevMovePos <= (DevicePosition.PETCT + PositionError))//��37��ǰ�ߵ�һ�����ź�
			{
				OnMotionMoveing(MidFrontSpeed, -MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
			{
				DevicePosition.DelayTimes = 0;
				OnMoveMotionstop();
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ!�������.");	
			}
		}
    	break;		
    case 8:
		RobotRightTurn(100);//�������  
		RevMoveSinglePosition();
		if (IR[8] == 1 || IR[9] == 1)
		{	
			OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);
		}
		if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
		{
			OnMoveMotionstop();
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");	
		}
    	break;
    case 9:
		RobotRightTurn(100);//�������
		if (IR[0] == 1 || IR[1] == 1)
		{
			OnMoveMotionstop();//����ֹͣ
			OnReZeroPosition();//ת��λ
			OnMoveZero();//�˶��������
			m_Turnstate =m_Turnstate+1;	
		}
		RevMoveSinglePosition();
		if (m_RevMovePos >= (DevicePosition.PETCT + PositionError))
		{
			OnMoveMotionstop();
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");	
		}
    	break;
    case 10:
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{	
			DevicePosition.DelayTimes = LeaveTimer;			
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
				DevicePosition.DelayTimes = 0;
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd,"�����˸�λ�ǶȲ���!");				
			}
		}
		break;
    case 11:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
			//	OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 12:
        RobotFrontTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			RevMoveSinglePosition();
			if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
			{
				OnMoveMotionstop(); 
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
			}			
			if (m_RevMovePos >= (DevicePosition.PET - PositionError) && m_RevMovePos <= (DevicePosition.PET + PositionError))
			{
				OnMotionMoveing(-MidFrontSpeed, MidFrontSpeed);
				DevicePosition.DelayTimes = 0;
			}
			else if (m_RevMovePos > (DevicePosition.PET + PositionError))
			{
				OnMoveMotionstop() ;
				m_Turnstate = m_Turnstate + 1;	
				DevicePosition.DelayTimes = 0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
    case 13:
        RobotFrontTurn(100);
		RevMoveSinglePosition();
		if (m_RevMovePos > (DevicePosition.PET + PositionError))
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;	
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		if ( IR[3] == 1 && IR[2] == 1)//4-7	IR[3]==1||||IR[10]==1	
		{
			OnMoveMotionstop(); 
			m_Turnstate = m_Turnstate + 1;
		}
    	break; 
    case 14:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			DevicePosition.DelayTimes = HoldDelayTimer;
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldStopBroad();
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 15:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= HoldDelayTimer)
		{
			DevicePosition.DelayTimes = HoldDelayTimer;
			if (m_intlimited == 191)
			{
				DevicePosition.DelayTimes = 0;
				OnHoldDownBroad(); 
				m_Turnstate = m_Turnstate + 1;			
			}
		}
		break;
    case 16:		
		m_intStat = 0;
		SetDlgItemText(IDC_STATICwwd,"�˲����������!");
		break;
    default:
		break;
    }
}
//�ֶ����������ܲ���
void CRobotDlg::AHand2Control()
{
	if (m_tempAD3 != m_AD3)
	{
	//	OnRedLight();
		if (m_AD3 >= 5)
		{			
			OnHoldUpBroad(); 
		}	
		else if (m_AD3 <= 1)
		{
			OnHoldDownBroad();		
		}
		else
		{		
            OnHoldStopBroad();
		}
		m_tempAD3 = m_AD3;
		m_TCPNUB = 0;
	}
//	����90��ת��
	if (m_tempAD4 != m_AD4)//����ת��90��
	{
		if (m_AD4 >=8000)
		{
			//OnTurn90();
			OnTurn90New();
		}
		else if (m_AD4 <=-8000)
		{
			OnReZeroPosition();
		}
		else if (m_AD4 == 0)
		{
			OnMotionstop();
		}
		m_TCPNUB=0;
		m_tempAD4 = m_AD4;
	}
	if (m_tempAD1 != m_AD1)//����ת��90��
	{
		if (m_RevTurnPos1 == -884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -883)
		{
			UpdateData(false);
			SanorLRDirection = 1;
			m_AD1 = m_AD1/2;
			MoveSameVelocityControlNew(-m_AD1);
		//	MoveSameVelocityControl(-VelocityPara90);
		}
		m_TCPNUB=0;
		m_tempAD1 = m_AD1;
	}
	if (m_AD1 == 0)
	{
		OnMoveMotionstop();
	}
	if (m_AD1 == 0 && m_AD4 == 0)
	{
		OnMotionstop();

	}
}
//����ǰ��------�������һ�����ӵĽǶȺ��ٶ�
void CRobotDlg::RoundFront(float vel, float angle1)
{
	float rad;
	float angle2;
	rad = (float)(658/tan(angle1));
    angle2 =(float)(atan(658/(rad+850)));
	
    position1=(long)(-angle1*1800/3.14);
    position2=(long)(-angle2*1800/3.14);	
    RevTurnPosition();
	int  temp1=abs(position1);
	int   temp2=abs(position2);
	int  temp12=abs(m_RevTurnPos1);
	int   temp22=abs(m_RevTurnPos3);
	if ((temp12 >= temp1-1|| temp12 <= temp1+1) && (temp22 >= temp2-1|| temp22 <= temp2+1))// && m_RevTurnPos5 == position1 && m_RevTurnPos7 == position2)
	{
		float rad1=(float)(sqrt(rad*rad+658*658));
		float rad2=(float)(sqrt((rad+850)*(rad+850)+658*658));

		float vel1,vel2;
		vel1=-rad1/rad*vel;
		vel2=rad2/rad*vel; 

        OnMotionMoveing((long)vel1, (long)vel2);
	}
}
//��������һ�㷽ʽת�����
void CRobotDlg::NormalTurning()
{
	if (m_TCPNUB>=4)
	{				
		if (m_AD2<-8000)
		{					
			if (TurningAngle>80)
			{
				TurningAngle=80;
			}
			else
			{						
				TurningAngle+=8;	
				TempTurningAngle=(float)(TurningAngle*3.14/180);
				RoundAngle(TempTurningAngle);
				m_TCPNUB=0;
			}
		}
		else if (m_AD2>8000)
		{
			if (TurningAngle<-30)
			{
				TurningAngle=-30;
			}
			else
			{
				TurningAngle-=4;
				TempTurningAngle=(float)(TurningAngle*3.14/180);
				RoundAngle(TempTurningAngle);
				m_TCPNUB=0;
			}
		}
	}
	if (m_tempAD1 != m_AD1)
	{
		if (TempTurningAngle>0)
		{
			RoundFront((float)(-m_AD1/2),TempTurningAngle);
		}
		else
		{
			RoundFront((float)(m_AD1/2),TempTurningAngle);
		}
		m_tempAD1 = m_AD1;
		m_TCPNUB=0;
	}
	else
	{
		return;
	}
}
//����ת�����ǰ��
void CRobotDlg::OnFront2() 
{
    RevTurnPosition();
	if (m_RevTurnPos1 == -909&& m_RevTurnPos3 == 369 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -394)
	{
		OnMotionMoveing(200, 326);
	}
	else
	{
        GetDlgItem(IDC_Front2)->EnableWindow(FALSE);
		GetDlgItem(IDC_Rear2)->EnableWindow(FALSE);
	}
}
//����ת����ƺ���
void CRobotDlg::OnRear2() 
{
    RevTurnPosition();
	if (m_RevTurnPos1 == -909&& m_RevTurnPos3 == 369 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == -394)
	{
		OnMotionMoveing(-130, -212);
	}		
}
//����ת�����ֹͣ
void CRobotDlg::OnStopAll() 
{
	SanorFRDirection = 0;
	SanorLRDirection = 0;
	OnMotionstop();
	m_intStat=0;//�����κβ���
	UpdateData(FALSE);
}
//�����˶������������
void CRobotDlg::OnReSetEncoder() 
{
	CString str, sError;
	for (int i=0;i<8;i++)
	{   
		tempNum=i;
		if(MotorIsNULL(tempNum))
		{
			return;			
		}
		else
		{
			if (m_pDriver[tempNum]->SendCommand(_T("MO=0"),str,sError))
			{
				if (!m_pDriver[tempNum]->SendCommand(_T("PX=0"),str,sError))
				{
				//	AfxMessageBox(sError);						
				}
				else
				{
					m_pDriver[tempNum]->SendCommand(_T("MO=1"),str,sError);
					bfen[i]=true;
				}
			}
		}
	}
}
//ˮƽ�˶������������
void CRobotDlg::OnMoveZero() 
{
	CString str, sError;
	for (int i=0;i<4;i++)
	{   
		tempNum=i*2+1;
		if(MotorIsNULL(tempNum))
		{
			return;			
		}
		else
		{
			if (m_pDriver[tempNum]->SendCommand(_T("MO=0"),str,sError))
			{
				if (!m_pDriver[tempNum]->SendCommand(_T("PX=0"),str,sError))
				{
				//	AfxMessageBox(sError);						
				}
				else
				{
					m_pDriver[tempNum]->SendCommand(_T("MO=1"),str,sError);
					bfen[i]=true;
				}
			}
		}
	}	
}
void CRobotDlg::OnChangeHoldUp2() 
{
	UpdateData(true);	
}




/************************************************************************/
/*                   �������е���һ����λ
/*                   (1) ǰ������һ����λDirNextLimited();
/*                   (2) ���˵���һ����λDirProLimited();
/*                   (3) ����ǰ������һ����λDirNextLimited90();
/*                   (4) ������˵���һ����λDirproLimited90();                                                  */
/************************************************************************/
void CRobotDlg::DirNextLimited()//ǰ������һ����λ
{
	switch(m_Turnstate)
	{
    case 0:
		OnReZeroPosition();
		m_Turnstate =m_Turnstate+1;
    	break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes == HoldDelayTimer * 2)
		{
            RevTurnPosition();
		}
		else if (DevicePosition.DelayTimes > HoldDelayTimer * 2)
		{
			DevicePosition.DelayTimes = HoldDelayTimer * 3;
            if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{ 	
				RobotFrontTurn(100);
				OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
				DevicePosition.DelayTimes = 0;
			}          
			else
			{
                 SetDlgItemText(IDC_STATICwwd,"�븴λת����!");
			}
		}	
    	break;		
      case 2:
		DevicePosition.DelayTimes ++;
        RobotFrontTurn(100);
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			if (IR[5]==1 || IR[6]==1)
			{
				m_Turnstate =m_Turnstate+1;	
				DevicePosition.DelayTimes = 0;
			}
			if (IR[2]==1&&IR[3]==1)
			{
				m_Turnstate =m_Turnstate+1;
				DevicePosition.DelayTimes = 0;
				tempNum=0;
			}
		}
    	break; 
      case 3:		
			OnMoveMotionstop(); 	
			m_Turnstate = m_Turnstate + 1;	
    	break;
	  default:
    	break;
	}
}
void CRobotDlg::DirProLimited()//���˵���һ����λ
{
	switch(m_Turnstate)
	{
    case 0:
		OnReZeroPosition();
		m_Turnstate = m_Turnstate + 1;
    	break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes == HoldDelayTimer * 3) 
		{
            RevTurnPosition();
		}
		else if (DevicePosition.DelayTimes > HoldDelayTimer * 3)
		{
			DevicePosition.DelayTimes = HoldDelayTimer * 4;
            if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{ 	
				RobotRearTurn(100);
				OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);	
				DevicePosition.DelayTimes = 0;
			}	
			else
			{
                SetDlgItemText(IDC_STATICwwd,"�븴λת����!");
			}
		} 	
    	break;		
	case 2:		
		RobotRearTurn(100);
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			if (IR[5]==1 || IR[6]==1)
			{
				OnMotionMoveing(MinFrontSpeed, -MinFrontSpeed);	
				//m_Turnstate = m_Turnstate+1;	
				DevicePosition.DelayTimes = 0;
			}
			else
			{			
				if (IR[2]==1&&IR[3]==1)
				{
					OnMoveMotionstop(); 
					m_Turnstate =m_Turnstate+1;	
					DevicePosition.DelayTimes = 0;
				}
			}
		}
		break; 
	case 3:
		if (IR[2]==1 && IR[3]==1)
		{
			OnMoveMotionstop(); 
			m_Turnstate =m_Turnstate+1;		
		}
		DevicePosition.DelayTimes++;
		if (DevicePosition.DelayTimes >= LeaveTimer*2)
		{
			OnMoveMotionstop(); 
			m_Turnstate =m_Turnstate+1;	
			DevicePosition.DelayTimes = 0;
		}
		break;
	default:
		break;
	}
}
void CRobotDlg::DirNextLimited90()//����ǰ������һ����λ
{
	switch(m_Turnstate)
	{
    case 0:
		m_Turnstate =m_Turnstate+1;
    	break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes == HoldDelayTimer * 2)
		{
            RevTurnPosition();
		}
		else if (DevicePosition.DelayTimes > HoldDelayTimer * 3)
		{
			DevicePosition.DelayTimes = HoldDelayTimer * 3;
			if (m_RevTurnPos1 == 884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == 883)
			{	
				RobotLeftTurn(100);
				OnMotionMoveing(-600, 600);
                DevicePosition.DelayTimes = 0;
			}          
			else
			{
                 SetDlgItemText(IDC_STATICwwd,"��ȷ��90��ת��!");
			//	 AfxMessageBox("��ȷ��������ת��Ƕ�!��֤����ת����ͬ,��Ϊ90��!");
			}
		}	
    	break;		
	case 2:
		DevicePosition.DelayTimes ++;
		RobotLeftTurn(100);
		if (DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			if (IR[0]==1||IR[1]==1||IR[8]==1||IR[9]==1)
			{
				OnMotionMoveing(-200, 200);
				DevicePosition.DelayTimes = 0;
			}			
		}
		break; 
	case 3:
		RobotLeftTurn(100);
		if (IR[8]==1||IR[9]==1)
		{
			OnMoveMotionstop(); 	
	        m_Turnstate =m_Turnstate+1;	
			DevicePosition.DelayTimes = 0;
		}			
		break; 
	case 4:		
		OnMoveMotionstop(); 	
		m_Turnstate =m_Turnstate+1;	
		break;
	default:
		break;
	}
}
void CRobotDlg::DirProLimited90()//������˵���һ����λ
{
	switch(m_Turnstate)
	{
    case 0:
		m_Turnstate =m_Turnstate+1;
    	break;
    case 1:
		DevicePosition.DelayTimes ++;
		if (DevicePosition.DelayTimes ==HoldDelayTimer * 2)
		{
            RevTurnPosition();
		}
		else if (DevicePosition.DelayTimes > HoldDelayTimer * 3)
		{
			DevicePosition.DelayTimes = HoldDelayTimer * 3;
			if (m_RevTurnPos1 == 884 && m_RevTurnPos3 == 888 && m_RevTurnPos5 == 900&& m_RevTurnPos7 == 883)
			{		
				RobotRightTurn(100);
				OnMotionMoveing(400, -400);
				DevicePosition.DelayTimes = 0; 
			}	
			else
			{
                 SetDlgItemText(IDC_STATICwwd,"��ȷ����90��ת��!");
			//	 AfxMessageBox("��ȷ��������ת��Ƕ�!��֤����ת����ͬ,��Ϊ90��!");
			}
		}	
    	break;		
	case 2:
		 DevicePosition.DelayTimes ++;
		RobotRightTurn(100);
		if ( DevicePosition.DelayTimes >= LeaveTimer)
		{
			DevicePosition.DelayTimes = LeaveTimer;
			if (IR[0]==1||IR[1]==1||IR[8]==1||IR[9]==1)
			{
				m_Turnstate =m_Turnstate+1;	
			    DevicePosition.DelayTimes = 0;
			}
		}
		break; 
	case 3:		
		OnMoveMotionstop(); 	
		m_Turnstate =m_Turnstate+1;	
		break;
	default:
		break;
	}
}


void CRobotDlg::DirRearMRI()
{
  switch(m_Turnstate) 
	{
    case 0:
		OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);	
    	break;
	case 1:
		RobotRearTurnMRI(100);
		tempNum++;
		if (tempNum >=150)
		{
			OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeedMRI), -(MaxFrontSpeed+MaxFrontSpeedMRI));	
		}
		break;
	case 2:
		RobotRearTurnMRI(100);	
		m_Turnstate =m_Turnstate+1;	
		break;  
	case 3:
		RobotRearTurn(100);
		tempNum++;
		if (tempNum>=10)
		{	
			tempNum=10;
			RevMoveSinglePosition();
			UpdateData(false);
		    if (m_RevMovePos>=(MaxMRIPosition-3000) && m_RevMovePos<=MaxMRIPosition)
			{
				OnMotionMoveing(400, -400);	
			}
			else if (m_RevMovePos>MaxMRIPosition)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
		break;
	case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
			tempNum=0;
			OnMotionstop();
			m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxMRIPosition)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		break;
	default:
		break;
	}
}

void CRobotDlg::DirRearPET()
{
  switch(m_Turnstate) 
	{
    case 0:
		OnMotionMoveing(MaxFrontSpeed, -MaxFrontSpeed);		
    	break;
	case 1:
		RobotRearTurn(100);
		tempNum++;
		if (tempNum >=20)
		{
		 OnMotionMoveing((MaxFrontSpeed+MaxFrontSpeedMRI), -(MaxFrontSpeed+MaxFrontSpeedMRI));	
		}
		break;
	case 2:
		RobotRearTurn(100);	
		m_Turnstate =m_Turnstate+1;	
		break;  
	case 3:
		RobotRearTurn(100);
		tempNum++;
		if (tempNum>=50)
		{	
			tempNum=50;
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>=(MaxPETPosition1-3000) && m_RevMovePos<=MaxPETPosition1)
			{
				OnMotionMoveing(400, -400);	
			}
			else if (m_RevMovePos>MaxPETPosition1)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
		break;
	case 4:
		RobotRearTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
			tempNum=0;
			OnMotionstop();
			m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxPETPosition1)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		break;
    case 5:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;			
			}
		}
		break;
    case 6:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldDownBroad(); 
				m_Turnstate =m_Turnstate+1;			
			}
		}
		break;
    case 7:		
		tempNum++;
		if (tempNum>=50)
		{
			OnHoldStopBroad();
			tempNum =0;
			m_Turnstate =m_Turnstate+1;	
		}
		break;
    case 8:		
		m_intStat=0;
		break;
    default:
		break;
	}
}

void CRobotDlg::DirRoboRearPET()
{
 switch(m_Turnstate) 
	{
    case 0:
		tempNum++;
		if (tempNum==WaitingTime-1)
		{
			OnReadHoldPos(); 
		}
		if (tempNum>=WaitingTime)
		{	
			tempNum=WaitingTime;	
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{ 
				OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeedMRI), (MaxFrontSpeed+MaxFrontSpeedMRI));	
			}
		}
		break;
	case 1:
		RobotFrontTurn(100);
		m_Turnstate =m_Turnstate+1;
	case 2:
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=50)
		{	
			tempNum=50;
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>=(MaxPETPosition1-3000) && m_RevMovePos<=MaxPETPosition1)
			{
				OnMotionMoveing(-400, 400);	
			}
			else if (m_RevMovePos>MaxPETPosition1)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
		break;
	case 3:
		RobotFrontTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		 if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		 {
			 tempNum=0;
			OnMotionstop();
			m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxPETPosition1)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		break;
    case 4:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;			
			}
		}
		break;
	default:
		break;
	}
}
void CRobotDlg::DirRoboRearMRI()
{
 switch(m_Turnstate) 
	{
    case 0:
		tempNum++;
		if (tempNum==WaitingTime-1)
		{
			OnReadHoldPos(); 
		}
		if (tempNum>=WaitingTime)
		{	
			tempNum=WaitingTime;	
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{ 
				OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeedMRI), (MaxFrontSpeed+MaxFrontSpeedMRI));	
			}
		}
		break;
	case 1:
		RobotFrontTurn(100);
		m_Turnstate =m_Turnstate+1;
    case 2:
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=50)
		{	
		    tempNum=50;
			RevMoveSinglePosition();
			UpdateData(false);
		    if (m_RevMovePos>=(MaxMRIPosition-3000) && m_RevMovePos<=MaxMRIPosition)
			{
			   OnMotionMoveing(-400, 400);	
			}
			else if (m_RevMovePos>MaxMRIPosition)
			{
				OnMoveMotionstop() ;
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
    	break;
	case 3:
		RobotFrontTurn(100);
		RevMoveSinglePosition();
		UpdateData(false);
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
			tempNum=0;
			OnMotionstop();
			m_Turnstate =m_Turnstate+1;	
		}
		else if (m_RevMovePos>MaxMRIPosition)
		{
			OnMoveMotionstop() ;
			m_Turnstate =m_Turnstate+1;	
			tempNum=0;
			SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
		}
		break;
    case 4:
		tempNum++;
		if (tempNum>=2)
		{
			if (m_intlimited == 191)
			{
				tempNum =0;
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;			
			}
		}
		break;
	default:
		break;
	}
}
void CRobotDlg::DirRobotFrontMRI()
{
  switch(m_Turnstate) 
	{ 
    case 0:
		OnReadHoldPos(); 
        if (m_Holdstate == 1)
        {
            if (m_RevHoldPosition >=(DevicePosition.PETHoldPosition-5) && m_RevHoldPosition <= (DevicePosition.PETHoldPosition+5))
            {
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;
            }
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������PET�߶�!");
			}
			break;
		}
		else if (m_Holdstate == 2)
		{
            if (m_RevHoldPosition >=245 && m_RevHoldPosition <= 255)
            {
				OnHoldStopBroad();
				m_Turnstate =m_Turnstate+1;
            }
			else
			{
			     SetDlgItemText(IDC_STATICwwd,"�뽫��������CT�߶�!");
			}
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd,"�뽫��������PET/CT�߶�!");
		}
    	break;		
    case 1:
		tempNum++;
		if (tempNum>=6)
		{
			tempNum=6;
			RevTurnPosition();
			if (m_RevTurnPos1 == -15 && m_RevTurnPos3 == -11 && m_RevTurnPos5 == 0 && m_RevTurnPos7 == -16)
			{
				OnMotionMoveing(-MaxFrontSpeed, MaxFrontSpeed);	
			}
		}
		break;
    case 2:
		tempNum++;
		if (tempNum>=2)
		{
			OnMotionMoveing(-(MaxFrontSpeed+MaxFrontSpeed1), (MaxFrontSpeed+MaxFrontSpeed1));	
		}
		break;
	case 3:
		RobotFrontTurn(100);
		tempNum++;
		m_CountNumLimit++;
		if (tempNum>=10)
		{	
			tempNum=10;		
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>=MaxPETMRIPosition)//38000
			{
				OnMoveMotionstop();
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}	
			if (IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			{		
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;	
			}
			else
			{
				if (m_CountNumLimit>=58)//55
				{
					m_Turnstate =m_Turnstate+1;	
					tempNum=0;	
					SetDlgItemText(IDC_STATICwwd,"û��⵽����!������ǰ����.");
				}
			}
		}
		break;
	case 4:	
		RobotFrontTurn(100);
		OnMotionMoveing(-550, 550);		
		break;
	case 5:	
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=3)
		{
			OnMotionMoveing(-150, 150);	
		}
		break;
    case 6:
		RobotFrontTurn(100);
		if (m_intLeftIR<300&&m_intRightIR<200)
		{
			if (m_intlimited == 191)
			{
				tempNum++;
				if (tempNum >= 4)
				{
					OnMotionstop();

					if (tempNum >= 6)
					{
						OnHoldDownBroad();
						m_Turnstate=m_Turnstate+1;
						tempNum=0;
					}
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			OnMotionstop();
			if (m_intLeftIR>=300)
			{
				SetDlgItemText(IDC_STATICwwd,"����������⵽����!");
			}
			if (m_intRightIR>=200)
			{
				SetDlgItemText(IDC_STATICwwd,"�������Ҳ��⵽����!");
			}
			break;
		}
    	break;	
	case 7:
		if (m_intlimited == 63)
		{
			tempNum++;
			if (tempNum>=3)
			{
				OnMotionMoveing(-200, 200);	
			}
		}
		break;
	case 8:
		RobotFrontTurn(100);
		m_CountNumLimit++;
		if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
		{
		 OnMotionMoveing(-800, 800);
		}	
		else
		{
			if (m_CountNumLimit>=120)//117
			{
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;	
				SetDlgItemText(IDC_STATICwwd,"û��⵽����!������ǰ����.");
			}
		}
		break;  
	case 9:
		RobotFrontTurn(100);
		tempNum++;		
		if (tempNum>=WaitingTime)
		{
			tempNum=WaitingTime;			
			m_CountNumLimit++;
			RevMoveSinglePosition();
			UpdateData(false);
			if (m_RevMovePos>(MaxPETMRIPosition-3000)&&m_RevMovePos<MaxPETMRIPosition)//38000
			{
				OnMotionMoveing(-600, 600);	
			}	
			else if (m_RevMovePos>=MaxPETMRIPosition)
			{
				OnMoveMotionstop();
				m_Turnstate =m_Turnstate+1;	
				tempNum=0;
				SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
			}
		}
		break;
	case 10:
		RobotFrontTurn(100);
		tempNum++;
		if (tempNum>=5)
		{
			tempNum=5;
			m_CountNumLimit++;
			RevMoveSinglePosition();
			UpdateData(false);		
			if ( IR[3]==1&&IR[2]==1)//4-7	IR[3]==1||||IR[10]==1	
			{			   
				OnMotionstop();
				m_Turnstate =m_Turnstate+1;	 
				tempNum=0;
			}
			else
			{
				if (m_RevMovePos>=MaxPETMRIPosition)
				{
					OnMoveMotionstop();
					m_Turnstate =m_Turnstate+1;	
					tempNum=0;
					SetDlgItemText(IDC_STATICwwd,"�������з�Χ,�������.");
				}
			}			 
		}
		break;	 
	case 11:
		m_intHoldPosition =450;
		UpdateData(false);
		UpdateData(true);
		OnControlHold();
		m_Turnstate =m_Turnstate+1;	
		break;
	case 12:
		tempNum++;
		if (tempNum>=50)
		{
			tempNum=0;
		   OnHoldStopBroad();
		   m_Turnstate =m_Turnstate+1;	
		}
	   break;
	case 13:
		m_intStat = 0;//�����κβ���
		m_Turnstate =m_Turnstate+1;	
		break;
    default:
		break;
    }
}
//�������
void CRobotDlg::OnMotionMoveState() 
{
	DlgRunStateDlg.DoModal();
}
void CRobotDlg::OnResetMotionPara() 
{
	ParameterSetting = fopen("d:\\RobotParameter.txt","w+");
	OnVPSetting();
}
//
void CRobotDlg::OnReadHold() 
{
//	OnReadHoldPos();
}

//�ٶ��趨
void CRobotDlg::OnRVSetting1() 
{
	int nRespond = DlgRVSetting.DoModal();
	if (nRespond == IDOK)
	{
		DirVelocityPara = DlgRVSetting.m_intFrontVelocity;
		VelocityPara90 = DlgRVSetting.m_intMiddleVelocity;
		VelocityPara57 = DlgRVSetting.m_intRearVelocity;
		VelocityTurnPara = DlgRVSetting.m_intTurnVelocity;

		m_UINTSonorLimtedL1 = DlgRVSetting.m_intSonerLimted;
        m_UINTSonorLimtedL2 = DlgRVSetting.m_intSonerLimted;
		m_UINTSonorLimtedL3 = DlgRVSetting.m_intSonerLimted;
        m_UINTSonorLimtedL4 = DlgRVSetting.m_intSonerLimted;
		m_UINTSonorLimtedR1 = DlgRVSetting.m_intSonerLimted;
        m_UINTSonorLimtedR2 = DlgRVSetting.m_intSonerLimted;
		m_UINTSonorLimtedR3 = DlgRVSetting.m_intSonerLimted;
        m_UINTSonorLimtedR4 = DlgRVSetting.m_intSonerLimted;
		m_UINTIRLimtedR = DlgRVSetting.m_intIRLimted;
		UpdateData(false);		
	}
}
//�ֱ�����
void CRobotDlg::OnJoyStickControl()
{
	int nRespond = DlgJoystick.DoModal();
	if (nRespond == IDOK)
	{
		UpdateData(false);
	}
}
void CRobotDlg::ONConnectJoyStick()
{
    DlgJoystick.OnConnectJoy();
}
void CRobotDlg::OnStartJoystick()
{
	m_BoolJOYControl = false;
	BoolCommMoitor = false;//close comm monitor
	SetTimer(8,300,NULL);
	GetDlgItem(IDC_Choroad) -> EnableWindow(FALSE);
	JoyStikeControl = 1;
	CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_StopJoystick,MF_UNCHECKED);
	mmenu -> CheckMenuItem(ID_StartJoystick,MF_CHECKED);
}
void CRobotDlg::OnStopJoystick()
{
	m_BoolJOYControl = true;
	BoolCommMoitor = true;
	KillTimer(8);
	GetDlgItem(IDC_Choroad) -> EnableWindow(TRUE);
	JoyStikeControl = 0;
   	CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_StartJoystick,MF_UNCHECKED);
	mmenu -> CheckMenuItem(ID_StopJoystick,MF_CHECKED);
}
//����
void CRobotDlg::OnStartSonar()
{
    BoolSonarControl = true; 
    CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_StartSonar,MF_CHECKED);
	mmenu -> CheckMenuItem(ID_StopSonar,MF_UNCHECKED);
}
void CRobotDlg::OnStopSonar()
{
    BoolSonarControl = false;
	CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_StartSonar,MF_UNCHECKED);
	mmenu -> CheckMenuItem(ID_StopSonar,MF_CHECKED);
}
//ͨѶ�жϼ��
void CRobotDlg::CloseCommMonitor()
{
    BoolCommMoitor = false;
	CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_OpenCommMonitor,MF_UNCHECKED);
	mmenu -> CheckMenuItem(ID_CloseCommMonitor,MF_CHECKED);
}
void CRobotDlg::OpenCommMonitor()
{
    BoolCommMoitor = true;
	CMenu* mmenu = GetMenu();
	mmenu -> CheckMenuItem(ID_CloseCommMonitor,MF_UNCHECKED);
	mmenu -> CheckMenuItem(ID_OpenCommMonitor,MF_CHECKED);
}
void CRobotDlg::OnReZeroPosition() 
{
	RevTurnPosition();
	m_longPosition[0] = TurnError1;//-55;	//4270
	m_longPosition[2] = TurnError2;//-48;	//4270
	m_longPosition[4] = TurnError3;//-42;	//2400
	m_longPosition[6] = TurnError4;//-35;//2400
	UpdateData(true);
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningPosition(k, m_longVelocity[k], m_longPosition[k]); 
	}	
}
void CRobotDlg::OnVPSetting() 
{
	int nRespond = DlgVelocity.DoModal();
	if (nRespond == IDOK)
	{
		m_longPosition[0]  = (long)(DlgVelocity.m_Slongposition1*1.78);
		m_longPosition[2]  = (long)(DlgVelocity.m_Slongposition3*1.78);
	    m_longPosition[4]  = DlgVelocity.m_Slongposition5;
		m_longPosition[6]  = DlgVelocity.m_Slongposition7;
		
		m_longVelocity[1] = DlgVelocity.m_intvelocity2;
		m_longVelocity[3] = DlgVelocity.m_intvelocity4;
		m_longVelocity[5] = DlgVelocity.m_intvelocity6;
		m_longVelocity[7] = DlgVelocity.m_intvelocity8;

		m_longVelocity[0] = (long)(DlgVelocity.m_intvelocity1*1.78);
		m_longVelocity[2] = (long)(DlgVelocity.m_intvelocity3*1.78);
		m_longVelocity[4] = DlgVelocity.m_intvelocity5;
		m_longVelocity[6] = DlgVelocity.m_intvelocity7;
		
		m_longPosition[1] = DlgVelocity.m_Slongposition2;
		m_longPosition[3] = DlgVelocity.m_Slongposition4;
		m_longPosition[5] = DlgVelocity.m_Slongposition6;
		m_longPosition[7] = DlgVelocity.m_Slongposition8;
		m_intHoldPosition = DlgVelocity.m_intHoldPos;
		DevicePosition.PET   = DlgVelocity.m_floPetLimtedPosition;
		DevicePosition.CT    = DlgVelocity.m_floCtLimtedPosition;
		DevicePosition.PETCT = DlgVelocity.m_floPetCtLimtedPosition;
		DevicePosition.PETHoldPosition    = DlgVelocity.m_intPetHoldPosition;
		DevicePosition.CTHoldPosition     = DlgVelocity.m_intCTHoldPosition;
		DevicePosition.TOPHoldPosition    = DlgVelocity.m_intTopHoldPosition;
		DevicePosition.FisrtCT            = DlgVelocity.m_CTFisrtPosition;
		DevicePosition.SecondCT           = DlgVelocity.m_CTSecondPosition;
		DevicePosition.FisrtPET           = DlgVelocity.m_PETFisrtPosition;
		DevicePosition.SecondPET          = DlgVelocity.m_PETSecondPosition;
		UpdateData(false);
		fprintf(ParameterSetting,"01 %5.0f\n02 %5.0f\n03 %5.0f\n04 %3d\n05 %3d\n06 %3d\n07 %4.0f\n08 %4.0f\n09 %4.0f\n10 %4.0f\n" , 
			DevicePosition.PET,DevicePosition.PETCT,DevicePosition.CT,DevicePosition.PETHoldPosition,DevicePosition.CTHoldPosition, 
			DevicePosition.TOPHoldPosition,DevicePosition.FisrtCT,DevicePosition.SecondCT,DevicePosition.FisrtPET,DevicePosition.SecondPET); 
	
		fclose(ParameterSetting);
	}
}
/************************************************************************/
/*                       ϵͳ�ڲ�����                                   */
/*                      �رպ���OnDestroy();                            */
/*                      ��ʱ������OnTimer();                            */
/************************************************************************/
void CRobotDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    OnCloseLight();	
	Sleep(300);
	
	CString sError,str;
    OnHoldStopBroad() ;	
	OnMotionstop();
	for (UINT i = 0; i < MOTIONNUM; i++)
	{
		if(MotorIsNULL(i))
		{
			return;			
		}
		else
		{
			m_pDriver[i] -> SendCommand(_T("MO=0"),str,sError);
			IDriver::DriverDisconnect(m_pDriver[i]);
			m_pDriver[i] = NULL;
		}
	}
	KillTimer(3);	
    KillTimer(4);	
	KillTimer(5);
    OnCloseLight();	
	m_serialport1.ClosePort();
	m_serialport2.ClosePort();
	m_serialport3.ClosePort();
}

void CRobotDlg::OnTimer(UINT nIDEvent) 
{
	int ResetLim1,ResetLim2;
    ResetLim1 = 903;
	ResetLim2 = 903;

	if (nIDEvent==1)
	{   
		m_BoolResetPosition = false;
		RevTurnPosition();
		UpdateData(false);
		if (m_RevTurnPos1 <= -ResetLim1)
		{
			if (Motionstate[0] == false)
			{		
				m_longVelocity[0]=17800;
			    OnMotion1movev();
			    Motionstate[0] =true;				
			}				
		}
		else if (m_RevTurnPos1 > ResetLim1)
		{	
		    OnMotionStop(0); //��һ��ת����ֹͣ		
		}
		if (m_RevTurnPos3 <= -ResetLim1)
		{
			if (Motionstate[1] == false)
			{	
				m_longVelocity[2]=17800;
			    OnMotion3movev();			
			    Motionstate[1] = true;
			}						
		}
		else if (m_RevTurnPos3 > ResetLim1)
		{
			OnMotionStop(2); //�ڶ���ת����ֹͣ
		}
		
		if (m_RevTurnPos5 <= -ResetLim2)
		{
			if (Motionstate[2] == false)
			{	
				m_longVelocity[4]=10000;
		    	OnMotion5movev();		
			    Motionstate[2] = true;
			}						
		}
		else if (m_RevTurnPos5 > ResetLim2)
		{
			OnMotionStop(4); //������ת����ֹͣ
		}
		if (m_RevTurnPos7 <= -ResetLim2)
		{
			if (Motionstate[3] == false)
			{	
				m_longVelocity[6]=10000;
				OnMotion7movev();			
			    Motionstate[3] = true;
			}			
		}
		else if (m_RevTurnPos7 > ResetLim2)
		{
			OnMotionStop(6); //���ĸ�ת����ֹͣ
		}
		if(RevLimitResetZero())			
		{ 		
            KillTimer(1);
            OnReZeroPosition();//�����Ƕ�
			m_BoolResetPosition = true;
		}
	}
	/************************************************************************/
	/*      ��������                                                        */
	/************************************************************************/
	if (nIDEvent == 2)
	{
		DealCommMsg();
		DealSonorMsg();
		DealIRMsg();
	}
	/************************************************************************/
	/*      ��ʱ�ɼ���ʾ                                                    */
	/************************************************************************/
	if (nIDEvent == 3)
	{
		OnLeftIRPosition(); 
	    OnRightIRPosition(); 
		RevTurnPosition();//��ȡת��Ƕ�
		UpdateData(false);
		UpdateData(true);
		
		if (templednub < 22)
		{
			templednub++;
		}
		
		if (templednub == 6) 
		{
			OnGreenLight();
		}
		else if (templednub == 10) 
		{
			OnGreenLight();
		}
		else if (templednub == 16) 
		{
			OnGreenLight();	
		}

		else if (templednub == 20) 
		{
			OnCloseLight();	
		}

		for (UINT iii = 0; iii < 4; iii ++)
		{
			if (RevTurnpos[iii] >= TurnningLimted)
			{                
				if (RevTurnpos[iii] > TempRevTurnpos[iii])
				{
					LeftLimted = true;	
					UINT iij = 2 * iii;
					OnMotionStop(iij);
					TempRevTurnpos[iii] = RevTurnpos[iii]; 
					CString Temp;
					Temp.Format("ת����%d�������з�Χ,�뷴ת!",iij);
					SetDlgItemText(IDC_MOTIONSTATIC,Temp);	
				}			
			}
			else if (RevTurnpos[iii] <= -TurnningLimted)
			{
				if (RevTurnpos[iii] < TempRevTurnpos[iii])
				{
					RightLimted = true;
					UINT iij = 2 * iii;
					OnMotionStop(iij);
					TempRevTurnpos[iii] = RevTurnpos[iii]; 
					CString Temp;
					Temp.Format("ת����%d�������з�Χ,�뷴ת!",iij);
					SetDlgItemText(IDC_MOTIONSTATIC,Temp);
				}
			}
			else
			{
				RightLimted = false;
				LeftLimted  = false;
				//SetDlgItemText(IDC_MOTIONSTATIC,"ת������������!");
			}
		}
		SendSensorMSG();//�ش���Ϣ

		for (UINT i = 0; i < 8; i++)
		{
			if(!motorstation[i])
			{
				OnMotionstop();		
				char SendDatas[5]; 
				SendDatas[0] = '$';
				SendDatas[1] = '$';
				SendDatas[2] = '#';
				SendDatas[3] = i;
				SendDatas[4] = '$';	
				SocketSend(SendDatas,5);				
			}			    
		}
		OnPaintSonar();//���Ƴ���
	}
	/************************************************************************/
	/*          ѭ�߿���                                                    */
	/************************************************************************/
	if (nIDEvent == 5)
	{
		UINT IRDitemp1[11];
		for (int i=0; i<11;i++)
		{
			if (IRDi[i] == true)
			{
				IRDitemp1[i] = 1;
			}
			else
			{
				IRDitemp1[i] = 0;
			}
			
		}
		IR[0]=IRDitemp1[1];
		IR[1]=IRDitemp1[0];
		IR[2]=IRDitemp1[3];//3
		IR[3]=IRDitemp1[4];//4
		IR[4]=IRDitemp1[2];
		IR[5]=IRDitemp1[7];//6
		IR[6]=IRDitemp1[8];//8
		IR[7]=IRDitemp1[9];//7
		IR[8]=IRDitemp1[5];
		IR[9]=IRDitemp1[6];
		IR[10]=IRDitemp1[10];
			//��Ӧ��ͼ�ϵ�ֵ1-[1];2-[0];3-[2];4-[3];5-[4];6-[5];7-[7];8-[6];11-[9];22-[8]
		IR1234=IR[0]*8+IR[1]*4+IR[2]*2+IR[3];
		IR1291=IR[9]*8+IR[0]*4+IR[8]*2+IR[1];
		IR3467=IR[2]*8+IR[3]*4+IR[5]*2+IR[7];
		if (m_boolFindLine)
		{
			ChoRoad();
		}		
	}
	/************************************************************************/
	/*        0��ת��λ                                                   */
	/************************************************************************/
	if (nIDEvent == 6)
	{	
		RevTurnPosition();//��ȡת��Ƕ�
		if (m_RevTurnPos1>=190&&m_RevTurnPos3>=190&&m_RevTurnPos5>=190&&m_RevTurnPos7>=190)
		{			
			OnResetZeroLeft(); 
			KillTimer(6);
		}
	}
	/************************************************************************/
	/*       �����Ƕ�ת��λ                                               */
	/************************************************************************/
	if (nIDEvent == 7)
	{	
		RevTurnPosition();//��ȡת��Ƕ�		
		if (m_RevTurnPos1>=1050&&m_RevTurnPos7>=560&&m_RevTurnPos3<=-160&&m_RevTurnPos5<=-690)
		{
			OnReSetEncoder();
			Sleep(100);
			OnResetZero();
			KillTimer(7);
		}
	}
	/************************************************************************/
	/*      �߼��ֱ��ɼ�                                                    */
	/************************************************************************/
	if (nIDEvent == 8)
	{
		DlgJoystick.UpdateInputState();
		DlgJoystick.UpdataJoyStick1();
		m_Y1=DlgJoystick.m_Y1;
		m_X2=DlgJoystick.m_X2;
		m_Y2=DlgJoystick.m_Y2;
		for( int i = 0; i < 10; i++ )
		{
			m_IO[i]=DlgJoystick.m_IO[i];	
		}
		if (JoyStikeControl)
		{
			JoyControlRobot();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

int CRobotDlg::OnDataMedianValue(UINT daa[], int Nb)
{
	int i,j;
	int Temp;
	for(i = 0;i < Nb;i++)        //ð�ݷ�������С��������
		for(j = i;j < Nb;j++)
		{
			if(daa[i] < daa[j])
			{
				Temp	= daa[i];
				daa[i]	= daa[j];
				daa[j]	= Temp;
			}
		}
	return daa[(Nb+1)/2];
}

void CRobotDlg::OnNormalTurnLeft() 
{
	UpdateData(true);
	m_AD2 = VelocityTurnPara * 10;//ת������
	m_longVelocity[0] = m_longVelocity[2] = (long)(m_AD2 * 1.78);
	m_longVelocity[4] = m_longVelocity[6] = m_AD2 ;
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningVelocity(k, m_longVelocity[k]);
	}
}

void CRobotDlg::OnNormalTurnRight() 
{
    UpdateData(true); 
	m_AD2 = - (VelocityTurnPara * 10);//ת������
	m_longVelocity[0] = m_longVelocity[2] = (long)(m_AD2 * 1.78);
	m_longVelocity[4] = m_longVelocity[6] = m_AD2 ;
	for (UINT i = 0; i < MotionNum; i++)
	{
		UINT k = i * 2;
		OnMotionTurningVelocity(k, m_longVelocity[k]);
	}		
}

void CRobotDlg::OnNormalFront() 
{
    UpdateData(true);
	m_AD1 = - DirVelocityPara;
	SanorFRDirection = 1;
	SanorLRDirection = 3;
	MoveSameVelocityControl(m_AD1);	
}

void CRobotDlg::OnNormalRear() 
{
	UpdateData(true);
	m_AD1 = DirVelocityPara;
	SanorFRDirection = 2;
	SanorLRDirection = 3;
	MoveSameVelocityControl(m_AD1);		
}

void CRobotDlg::OnNormalStopAll() 
{
    SanorFRDirection = 0;
	SanorLRDirection = 0;
	m_boolFindLine   = 0;
	OnMotionstop();
}

void CRobotDlg::OnRestartAll()
{
	m_boolFindLine   = 1;
}

void CRobotDlg::OnStopIR() 
{
	BoolIRControl = false; 
    CMenu* mmenu = GetMenu();
	mmenu->CheckMenuItem(ID_StopIR,MF_CHECKED);
	mmenu->CheckMenuItem(ID_StartIR,MF_UNCHECKED);	
}

void CRobotDlg::OnStartIR() 
{
    BoolIRControl = true; 
    CMenu* mmenu = GetMenu();
	mmenu->CheckMenuItem(ID_StartIR,MF_CHECKED);
	mmenu->CheckMenuItem(ID_StopIR,MF_UNCHECKED);	
}

void CRobotDlg::DealSonorMsg()
{
    if (BoolSonarControl)
	{
		if (SanorFRDirection == 1)
		{
			if (SonorPosition[RightSonor3].SonerPosition < m_UINTSonorLimtedR1)//SanorFRLength)
			{
				OnMotionstop();
                
				SetDlgItemText(IDC_STATICwwd, _T("������ǰ���Ҳ෢���ϰ���!"));
			}
			else if (SonorPosition[LeftSonor3].SonerPosition < m_UINTSonorLimtedL1)//SanorFRLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("������ǰ����෢���ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}
		}
		else if (SanorFRDirection == 2)
		{
			if (SonorPosition[LeftSonor1].SonerPosition < m_UINTSonorLimtedL4)//SanorFRLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�����˺���෢���ϰ���!"));
			}
			else if (SonorPosition[RightSonor2].SonerPosition < m_UINTSonorLimtedR4)//SanorFRLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�����˺��Ҳ෢���ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}
		}
		else if (SanorFRDirection == 3)
		{
			//if (SonorPosition[LeftSonor1].SonerPosition < SanorFRLength||SonorPosition[RightSonor2].SonerPosition < SanorFRLength)
			if (SonorPosition[LeftSonor1].SonerPosition < m_UINTSonorLimtedL4||SonorPosition[RightSonor2].SonerPosition < m_UINTSonorLimtedR4)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�����˺󲿷����ϰ���!"));
			}
			//else if (SonorPosition[RightSonor3].SonerPosition < SanorFRLength||SonorPosition[LeftSonor3].SonerPosition < SanorFRLength)
			else if (SonorPosition[RightSonor3].SonerPosition < m_UINTSonorLimtedR1||SonorPosition[LeftSonor3].SonerPosition < m_UINTSonorLimtedL1)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("������ǰ�������ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}
		}

		if (SanorLRDirection == 1)
		{
			if (SonorPosition[LeftSonor4].SonerPosition < m_UINTSonorLimtedL2)//SanorRLLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("���������ǰ�������ϰ���!"));
			}
			else if (SonorPosition[LeftSonor2].SonerPosition < m_UINTSonorLimtedL3)//SanorRLLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("���������󲿷����ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}
		}
		else if (SanorLRDirection == 2)
		{
			if (SonorPosition[RightSonor4].SonerPosition < m_UINTSonorLimtedR2)//SanorRLLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�������Ҳ�ǰ�������ϰ���!"));
			}
			else if (SonorPosition[RightSonor1].SonerPosition < m_UINTSonorLimtedR3)//SanorRLLength)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�������Ҳ�󲿷����ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}
		}
		else if (SanorLRDirection == 3)
		{
			if (SonorPosition[RightSonor4].SonerPosition < m_UINTSonorLimtedR2||SonorPosition[RightSonor1].SonerPosition < m_UINTSonorLimtedR3)		
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("�������Ҳ෢���ϰ���!"));
			}
			//else if (SonorPosition[LeftSonor4].SonerPosition < SanorRLLength||SonorPosition[LeftSonor2].SonerPosition < SanorRLLength)
			else if (SonorPosition[LeftSonor4].SonerPosition < m_UINTSonorLimtedL2||SonorPosition[LeftSonor2].SonerPosition < m_UINTSonorLimtedL3)
			{
				OnMotionstop();
				SetDlgItemText(IDC_STATICwwd, _T("��������෢���ϰ���!"));
			}
			else
			{
				SetDlgItemText(IDC_STATICwwd, _T("û�з����ϰ���!"));
			}			
		}
	}
	else
	{
		SetDlgItemText(IDC_STATICwwd, _T("��Ҫʵ�ֳ�������,��Ӳ˵��п���!"));
	}
}

void CRobotDlg::DealIRMsg()
{
	if (SanorFRDirection == 1)
	{
		if (m_intRightIR > m_UINTIRLimtedR)
		{
			if (m_intRightIR > m_intTempRightIR)
			{
				OnMotionstop();
				m_intTempRightIR = m_intRightIR;
				SetDlgItemText(IDC_STATICwwd, _T("��������ǰ�������ϰ���!"));
			}
		}
		else if (m_intLeftIR > m_UINTIRLimtedL)
		{
			if (m_intLeftIR > m_intTempLeftIR)
			{
				OnMotionstop();
				m_intTempLeftIR = m_intLeftIR;
				SetDlgItemText(IDC_STATICwwd, _T("��������ǰ�������ϰ���!"));
			}
		}
		else
		{
			SetDlgItemText(IDC_STATICwwd, _T("ǰ��û�з����ϰ���!"));
		}
	}
}

void CRobotDlg::DealCommMsg()
{
	if (BoolCommMoitor)
	{
		m_CommCorrect++;
		if (m_CommCorrect >= 10)
		{
			m_CommCorrect = 0;
			SetDlgItemText(IDC_STATICwwd,_T("�������޻ش�����,��������״̬!"));
			OnMotionstop();		
		}
	}
}
BOOL CRobotDlg::MotorIsNULL(UINT i)
{
	if(m_pDriver[i] == NULL)
	{
		* (motorstation +i) = false; 
		OnMotionstop();
		OnRedLight();
		CString strerrors="please initial NO  motor!!!"; 
		CString innt;
		innt.Format("%d",i+1);
		strerrors.Insert(18,innt);
		SetDlgItemText(IDC_MOTIONSTATIC, strerrors);
		return true;
	}
	else
	{
		return false;
	}
}






void CRobotDlg::OnShutdown()
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), 
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		//AfxMessageBox(_T("OpenProcessToken Error."));
	}

	//���LUID
	TOKEN_PRIVILEGES tkp; 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	//��������Ȩ��
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
    if (GetLastError() != ERROR_SUCCESS) 
	{
       // AfxMessageBox(_T("AdjustTokenPrivileges Error.")); 
	}

	//�ر�ϵͳ
    if (!ExitWindowsEx(EWX_SHUTDOWN, 0)) 
	{
      //  AfxMessageBox(_T("ExitWindowsEx Error.")); 
	}
    OnDestroy(); 	
}

void CRobotDlg::OnRestart()
{
	// TODO: Add your control notification handler code here
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), 
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
	//	AfxMessageBox(_T("OpenProcessToken Error."));
	}

	//���LUID
	TOKEN_PRIVILEGES tkp; 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	//��������Ȩ��
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
    if (GetLastError() != ERROR_SUCCESS) 
	{
 //       AfxMessageBox(_T("AdjustTokenPrivileges Error.")); 
	}

	//����ϵͳ
    if (!ExitWindowsEx(EWX_REBOOT, 0)) 
	{
 //       AfxMessageBox(_T("ExitWindowsEx Error.")); 
	}
    OnDestroy(); 
}



void CRobotDlg::RecvDataTcpSensor(char recvdata[], int length)
{
     	unsigned char tempchar[20];
		int i=0;
		for (i=0;i<20;i++)
		{
			tempchar[i]=recvdata[i];
		}
        if (m_SensorData.SetByBytes(tempchar)==true)
		{ //	AfxMessageBox(recvdata);	
		   unsigned int m_AD[25];
   //1DirVelocityPara/3;         2VelocityPara57/3;          3VelocityTurnPara/10;
   //m_UINTSonorLimtedL1         m_UINTSonorLimtedL2         m_UINTSonorLimtedL3
   //m_UINTSonorLimtedL4         m_UINTSonorLimtedR1         m_UINTSonorLimtedR2
   //m_UINTSonorLimtedR3         m_UINTSonorLimtedR4
   //m_UINTIRLimtedL             m_UINTIRLimtedR
		   for (i=0;i<13;i++)
		   {
				m_SensorData.GetSenData(&m_AD[i],i); 
			//	m_ReturnSensorAD[i] = m_AD[i];
		   }            
		   UpdateData(FALSE);	
		   	//MessageBox("d");
		}
		else
		{
			return;				
		}
}

void CRobotDlg::OnSonorsettinf() 
{
	// TODO: Add your command handler code here
	CSonorSetting m_SensorSetting;
	int nRespond = m_SensorSetting.DoModal();
	if (nRespond == IDOK)
	{
		m_UINTSonorLimtedL1 = m_SensorSetting.m_SonorLimtedL1;
		m_UINTSonorLimtedL2 = m_SensorSetting.m_SonorLimtedL2;
		m_UINTSonorLimtedL3 = m_SensorSetting.m_SonorLimtedL3;
		m_UINTSonorLimtedL4 = m_SensorSetting.m_SonorLimtedL4;
		m_UINTSonorLimtedR1 = m_SensorSetting.m_SonorLimtedR1;
		m_UINTSonorLimtedR2 = 45;//m_SensorSetting.m_SonorLimtedR2;
		m_UINTSonorLimtedR3 = m_SensorSetting.m_SonorLimtedR3;
		m_UINTSonorLimtedR4 = m_SensorSetting.m_SonorLimtedR4;
		m_UINTIRLimtedL = (50 - m_SensorSetting.m_IRLimtedL) * 10;
		m_UINTIRLimtedR = (50 - m_SensorSetting.m_IRLimtedR) * 10;		
		UpdateData(false);		
	}	
}

void CRobotDlg::OnTurn91() 
{
	// TODO: Add your control notification handler code here
	OnTurn90();
}

void CRobotDlg::OnRaedEncoder() 
{
	// TODO: Add your command handler code here
	RevMoveSinglePosition();
	UpdateData(false);
}

int CRobotDlg::OnChar2int(char c)
{
	if(c >= 0x30 && c <= 0x39)
		return c-0x30;
	else
		return 0;
}
void CRobotDlg::OnFileRead(CString filename)
{
	CFile fileM;
	CFileException feM;
	char fheaderd[MAXTASKNUM];
	char flined[MAXHEADERNUM];
	char taskd[MAXLINENUM];
	int headernum = 0;
	int flinernum = 0;  //ÿ�еı��
	int i = 0;
	CString str = _T("");
   float  temp[6];
	if(!fileM.Open(filename,CFile::modeRead,&feM))
	{
		AfxMessageBox("�ļ���ȡ����!");	
		return;
	}
	TRY
	{
		fileM.Read(taskd,sizeof(char)*MAXLINENUM);
		if(taskd[1] == 0x31)  //'1'
		{		
			DevicePosition.PET = (float)(OnChar2int(taskd[3])*10000+OnChar2int(taskd[4])*1000+OnChar2int(taskd[5])*100+OnChar2int(taskd[6])*10+OnChar2int(taskd[7]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�1��)(3)!");					
			AfxMessageBox(str);		
		}
		if(taskd[11] == 0x32)  //'2'
		{		
			DevicePosition.PETCT = (float)(OnChar2int(taskd[13])*10000+OnChar2int(taskd[14])*1000+OnChar2int(taskd[15])*100+OnChar2int(taskd[16])*10+OnChar2int(taskd[17]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�2��)(3)!");				
			AfxMessageBox(str);		
		}
		if(taskd[21] == 0x33)  //'3'
		{		
			DevicePosition.CT = (float)(OnChar2int(taskd[23])*10000+OnChar2int(taskd[24])*1000+OnChar2int(taskd[25])*100+OnChar2int(taskd[26])*10+OnChar2int(taskd[27]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�3��)(3)!");				
			AfxMessageBox(str);		
		}
		if(taskd[31] == 0x34)  //'4'
		{		
			DevicePosition.PETHoldPosition = OnChar2int(taskd[33])*100+OnChar2int(taskd[34])*10+OnChar2int(taskd[35]);
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�4��)(3)!");				
			AfxMessageBox(str);		
		}
		if(taskd[39] == 0x35)  //'5'
		{		
			DevicePosition.CTHoldPosition = OnChar2int(taskd[41])*100+OnChar2int(taskd[42])*10+OnChar2int(taskd[43]);
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�5��)(3)!");					
			AfxMessageBox(str);		
		}
		if(taskd[47] == 0x36)  //'6'
		{		
			DevicePosition.TOPHoldPosition = OnChar2int(taskd[49])*100+OnChar2int(taskd[50])*10+OnChar2int(taskd[51]);
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�6��)(3)!");					
			AfxMessageBox(str);		
		}
		if(taskd[55] == 0x37)  //'3'
		{		
			DevicePosition.FisrtCT = (float)(OnChar2int(taskd[57])*1000+OnChar2int(taskd[58])*100+OnChar2int(taskd[59])*10+OnChar2int(taskd[60]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�7��)(3)!");				
			AfxMessageBox(str);		
		}
		if(taskd[64] == 0x38)  //'4'
		{		
			DevicePosition.SecondCT = (float)(OnChar2int(taskd[66])*1000+OnChar2int(taskd[67])*100+OnChar2int(taskd[68])*10+OnChar2int(taskd[69]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�8��)(3)!");				
			AfxMessageBox(str);		
		}
		if(taskd[73] == 0x39)  //'5'
		{		
			DevicePosition.FisrtPET = (float)(OnChar2int(taskd[75])*1000+OnChar2int(taskd[76])*100+OnChar2int(taskd[77])*10+OnChar2int(taskd[78]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�9��)(3)!");					
			AfxMessageBox(str);		
		}
		if(taskd[81] == 0x31)  //'6'
		{		
			DevicePosition.SecondPET = (float)(OnChar2int(taskd[84])*1000+OnChar2int(taskd[85])*100+OnChar2int(taskd[86])*10+OnChar2int(taskd[87]));
		}
		else
		{
			str.Format("�ļ���ʽ����(�ļ��е�10��)(3)!");					
			AfxMessageBox(str);		
		}
	}

	CATCH (CFileException, eLoad)
	{
		// ��ȡʧ��
		fileM.Abort();		
		// �ָ������״
		EndWaitCursor();		
		fileM.Close();
	}
	END_CATCH
}



