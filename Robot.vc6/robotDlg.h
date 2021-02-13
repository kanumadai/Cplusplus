// robotDlg.h : header file
//
#include "Setting.h"
#include "ControlData.h"
#include "SerialPort.h"
#include "SensorData.h"
//#include "CHORoad.h"
#include "math.h"
#include "GlobalDefines.h"
#include "RunStateDlg.h"
#include "RVSetting.h"
#include "JoyStick.h"
#include "SonorSetting.h"

#if !defined(AFX_ROBOTDLG_H__5DC721AD_BBD2_4B49_B408_44146E2E0EC2__INCLUDED_)
#define AFX_ROBOTDLG_H__5DC721AD_BBD2_4B49_B408_44146E2E0EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MaxRecvLength	7				//接收数据最大长度  
#define MaxSendLength	21				//发送数据最大长度
#define TurningAngle1   (180*854)/(3.14*18)   //转向弧度转换1
#define TurningAngle2   (180*480)/(3.14*18)   //转向弧度转换2
//动态连接库
#include "IDriver.h"
#ifdef _DEBUG
#pragma message("ElmoComApi_d.Lib")
#pragma comment(lib, "ElmoComApi_d.Lib")
#else
#pragma message("ElmoComApi.Lib")
#pragma comment(lib, "ElmoComApi.Lib")
#endif
/////////////////////////////////////////////////////////////////////////////
// CRobotDlg dialog
typedef union
{ unsigned int IRData;
  struct 
  {	
		unsigned int IR0:1;            
		unsigned int IR1:1;            
		unsigned int IR2:1;           
		unsigned int IR3:1;           
		unsigned int IR4:1;	           
		unsigned int IR5:1;            
		unsigned int IR6:1;           
		unsigned int IR7:1;           		
   }IRBITS;
} IRDATA;
typedef union
{ unsigned int IRData2;
  struct 
  {	
		unsigned int IR0:1;            
		unsigned int IR1:1;            
		unsigned int IR2:1;           
		unsigned int IR3:1;           
		unsigned int IR4:1;	           
		unsigned int IR5:1;            
		unsigned int IR6:1;           
		unsigned int IR7:1;           		
   }IRBITS2;
} IRDATA2;
typedef struct tagPosition
{
	int x;
	int y;
	float th;
	double velocity;
}Position;		//笛卡儿坐标系位置
typedef struct tagRobotPosition
{
	int x;
	int y;
	float th;
	double velocity;
}RobotPosition;		//笛卡儿坐标系位置
typedef struct SonorPosition
{
    UINT  SonerPosition;
	UINT  SonerFilterPostion[MedianFilter];
}SonorPosition;		//笛卡儿坐标系位置

typedef struct tagDevicePosition
{
	float CT;
	float FisrtCT;
	float SecondCT;
	float PET;
	float FisrtPET;
	float SecondPET;
   	float PETCT;
	UINT  DelayTimes;
	UINT  PETHoldPosition;
	UINT  CTHoldPosition;
	UINT  TOPHoldPosition;
}DevicePosition;

class CRobotDlg : public CDialog
{
// Construction
public:
	void DirRoboLeftRight(float limited);
	void DirRoboRear(float limted);
	void DirRobotFront(UINT Holdlimited, float limited,float Firstlimited,float Secondlimited);
	void OnRestartAll();
	int OnChar2int(char c);
	void RecvDataTcpSensor(char recvdata[], int length);
	void OnChoroadRemote();
	void OnRestart();
	void OnShutdown();
	void RecvDataTcp1(char recvdata[], int length);
	BOOL MotorIsNULL(UINT i);
	void OnMotionMoveingNew(long velocity1, long Velocity2);
	void MoveSameVelocityControlNew(int Velocity);
	void OnTurn90New();
	void DealCommMsg();
	void DealIRMsg();
	void DealSonorMsg();

	void OnTurnMotionstart();
	void OnMoveMotionstart();
	void OnTurnMotionstop();
	void OnMoveMotionstop();
	void OnMotionStop(UINT i);

	void OnMotionTurningVelocity(UINT i, long Velocity);
	void OnMotionTurningPosition(UINT i, long Velocity, long Position);	
	void TurnVelocityControl(int Velocity);
	void OnMotionMoveingVelocity(UINT i, long Velocity);
	void MoveDiffVelocityControl(int Velocity);
	void MoveSameVelocityControl(int Velocity);
	void OnMotionMoveing(long velocity1, long Velocity2);

	bool RevMovePosition();
	bool RevTurnPosition();
	bool RevMoveSinglePosition();	

	void RevLimit2(); 
	bool RevLimitResetZero();	
	
	
	void OnTurnAngle(int angle1, int angle2, int angle3, int angle4);

	void OpenCommMonitor();
	void CloseCommMonitor();
	void OnStopSonar();
	void OnStartSonar();
	int  OnDataMedianValue(UINT daa[], int Nb);
	void ControlRobotJoy();
	void DirProLimited90();
	void DirNextLimited90();
	void DirRobotFrontMRI();
	void OnStopJoystick();
	void JoyControlRobot();
	void OnStartJoystick();
	void ONConnectJoyStick();
	void OnJoyStickControl();
	void DirRoboRearMRI();
	void DirRoboRearPET();
	void DirRearPET();
	void DirRearMRI();
	void DirRobotFront2();



	void RobotRearTurnMRI(int a);
	void RobotFrontTurnMRI(int a);
	void DirProLimited();
	void DirNextLimited();
	void DirRoboLeftRight2();
	void DirRoboTuring90PET();
	void DirRoboRear3();
	void DirRoboRear2();
	void DirRoboTuring90();
	void DirRoboLeftRight1();
	void DirRobotFront1();
	void DirRoboRear1();
	void NormalTurning();
	void RoundAngle(float angle1);
	void RoundFront(float vel, float angle1);

	void AHand2Control();
	void DirRoboLeftRight();
	void DirRoboRear();
	void DirRobotFront();
	void OnRightIRPosition();
	void OnLeftIRPosition();
	void OnOpenSerialPort3();
	void OnOpenSerialPort1();
	void OnOpenSerialPort2();
	void ChoRoad();	
//不同运行方向控制	
	void RobotRight(UINT SelNum);
	void RobotLeft(UINT SelNum);
	void RobotRear(UINT SelNum);
	void RobotFront(UINT SelNum);
//不同运行方向调整
	void RobotRightTurn(int a);
	void RobotLeftTurn(int a);
	void RobotFrontTurn(int a);//向前运行调整
	void RobotRearTurn(int a);
//不同调整方式
	void CarTurnFR(int frontdata1, int frontdata2, int reardata1,int reardata2);
	void CarTurnLR(int leftdata1, int leftdata2, int rightdata1,int rightdata2);
	void WheelTurnFR(int frontdata1, int frontdata2, int reardata1,int reardata2) ;
    void WheelTurnLR(int leftdata1, int leftdata2, int rightdata1,int rightdata2);


	void OnPaintSonar();
	void OnPaintSingleSonar(CDC* dc,int x, int y, int angle, int centerangle,int dist);
	void OnPaintSingleWheel(CDC *dc, int x, int y,int wheelwidth,int angle,int no);

	void SocketSend(char* str,int n);
	void SendSensorMSG();
	void OnControlHoldR();
	int  Str2Hex(CString str, char* data);
	char HexChar(char c);


	void ControlRobot();
	bool OnSocketOpen();
	CRobotDlg(CWnd* pParent = NULL);	// standard constructor
	void OnSocketRead(WPARAM wParam, LPARAM lParam);
	void OnSocketAccept(WPARAM wParam, LPARAM lParam);
	void RecvDataTcp(char recvdata[], int length);
	bool OnSocketIni();
	void OnFileRead(CString filename);

	UINT TempNumCount;
	int  IR[11];
	int  tempIR2;
	int  tempIR3;
	int  IR1234;
	int  TempIR1234;
	int  IR1291;
	int  TempIR1291;
	int  IR3467;
	int  TempIR3467;
	UINT MRIError;
	
	UINT  statereset;



    BOOL  m_IO[10];
	long  m_longPosition[MOTIONNUM];
	long  m_longVelocity[MOTIONNUM];
	FILE    *ParameterSetting;
// Dialog Data
	//{{AFX_DATA(CRobotDlg)
	enum { IDD = IDD_ROBOT_DIALOG };
	control_data m_ControlData;	
	int  	m_AD1;
	int     m_tempAD1;
	int 	m_AD2;
	int     m_tempAD2;
	UINT	m_AD3;
	UINT    m_tempAD3;
	int  	m_AD4;
	int     m_tempAD4;
	UINT	m_IO1;
	UINT    m_tempIO1;
	UINT    m_tempIO2;
	UINT    m_tempIO3;
	UINT	m_IO2;
	UINT	m_IO3;
	UINT	m_IO4;
	UINT	m_tempIO4;
	UINT	m_IO5;
	UINT  	m_tempIO5;
	UINT	m_IO6;
	UINT  	m_tempIO6;
	UINT  	m_tempIO62;
	UINT	m_IO7;
	UINT	m_IO8;
	long		m_RevTurnPos1;
	long		m_RevTurnPos3;
	long		m_RevTurnPos5;
	long		m_RevTurnPos7;
	long		m_RevTurnPos2;
	long		m_RevTurnPos4;
	long		m_RevTurnPos6;
	long		m_RevTurnPos8;

	float		m_RevMovePos;
	CSerialPort m_serialport1;
	CSerialPort m_serialport2;
	CSerialPort m_serialport3;
	BOOL   m_bSerialPortOpened1;
	BOOL   m_bSerialPortOpened2;
	BOOL   m_bSerialPortOpened3;
	UINT	m_intHoldPosition;
	UINT	m_tempintHoldPosition;
	bool    m_StartSensorData;     //com1接收数据包的包头
	bool    m_EndSensorData;	   //com1接收数据包的包尾
	bool    m_StartSensorData1;     //com2接收数据包的包头
	bool    m_EndSensorData1;	   //com2接收数据包的包尾
	bool    m_StartSensorData2;     //com3接收数据包的包头
	bool    m_EndSensorData2;	   //com3接收数据包的包尾
	CString	m_RevData;
	int		m_intRevData;
	int		m_intRevData2;
	IRDATA  IrData;
	IRDATA2 IrData2; 

	SonorPosition SonorPosition[8];
	UINT    SonorFilterNubRight;
	UINT    SonorFilterNubLeft;
    UINT    SonerFilterPostion[MedianFilter];

	tagDevicePosition DevicePosition; 

	UINT	m_RevHoldPosition;	

	int		m_intRevData3;
	SensorData m_SensorData;		
	CString	m_RevData1;
	CString	m_RevData2;
	bool    m_ControlMode;
	int		m_intStat;
	int     tempNum;
	int     tempNum1;
	UINT    tempstate;
	UINT	m_intLeftIR;
	UINT    m_intRightIR;
	int		m_intlimited;
	int     m_FronTturNum;
	UINT    m_Holdstate;
	UINT    m_TCPNUB; 
	long    position1;
	long    position2;
	long    TurningAngle;//任意半径转向
	float   TempTurningAngle;
	int		m_CountNumLimit;
	int     m_Y1;
	int     m_X2;
	int     m_Y2;

    BOOL    JoyStikeControl;//手柄控制
	BOOL    BoolSonarControl;
	BOOL    BoolCommMoitor;
	BOOL    BoolIRControl;
	UINT    SanorFRDirection;
	UINT    SanorLRDirection;
	UINT    m_CommCorrect;
	int     DirVelocityPara;
	int     VelocityPara90;
	int     VelocityPara57;

	UINT		m_intTempLeftIR;
	UINT		m_intTempRightIR;
	BOOL    RightLimted;
	BOOL    LeftLimted;
	BOOL    m_BoolResetPosition;
	UINT    m_COMMNUB;
	BOOL    m_BoolJOYControl;
	UINT    m_UINTSonorLimtedL1;
	UINT    m_UINTSonorLimtedL2;
	UINT    m_UINTSonorLimtedL3;
	UINT    m_UINTSonorLimtedL4;
	UINT    m_UINTSonorLimtedR1;
	UINT    m_UINTSonorLimtedR2;
	UINT    m_UINTSonorLimtedR3;
	UINT    m_UINTSonorLimtedR4;
	bool    m_boolSonorlimted;
	UINT    m_UINTIRLimtedL;
	UINT    m_UINTIRLimtedR;
	float   m_floPetLimtedPosition;
	float   m_floPetCtLimtedPosition;
	float   m_floCtLimtedPosition;

	bool    m_boolFindLine;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	
	HICON m_hIcon;	
	HICON m_hIconRed;    //开关打开时的红灯图标句柄
	HICON m_hIconOff;    //开关关闭时的指示图标句柄

	IDriver* m_pDriver[9];
	//初始化设备数目
	CString fileName1;
	CString fileName2;
	UINT size;
	CStatic	m_ctrlStaticIconS12;
	CStatic	m_ctrlStaticIconS11;
	CStatic	m_ctrlStaticIconS10;
	CStatic	m_ctrlStaticIconS9;
	CStatic	m_ctrlStaticIconS8;
	CStatic	m_ctrlStaticIconS7;
	CStatic	m_ctrlStaticIconS6;
	CStatic	m_ctrlStaticIconS5;
	CStatic	m_ctrlStaticIconS4;
	CStatic	m_ctrlStaticIconS3;
	CStatic	m_ctrlStaticIconS2;
	CStatic	m_ctrlStaticIconS1;	
	CStatic	m_ctrlStaticNO1;
	CStatic	m_ctrlStaticNO2;
	CStatic	m_ctrlStaticNO3;
	CStatic	m_ctrlStaticNO4;
//	CStatic	m_ctrlStaticNO5;
	CStatic	m_ctrlStaticNO6;
	CStatic	m_ctrlStaticNO7;
	CStatic	m_ctrlStaticNO8;	
	CStatic	m_ctrlStaticNO9;
	CStatic	m_ctrlStaticNO10;
	CStatic	m_ctrlStaticNO13;
//控制窗口
	CSetting DlgVelocity;
    CRunStateDlg DlgRunStateDlg;
    CRVSetting   DlgRVSetting;
	CJoyStick    DlgJoystick;
	bool Motionstate[4];
	bool bfen[4];
	UINT LimtedDi[4];
	bool IRDi[11];

	//以下是网络通讯变量
	char m_chServerInfo[64];
	HOSTENT *m_hostent;
	CString m_csServerIPAddr;
	SOCKET m_RobTcpServer;				//建立tcp套接字
	SOCKET m_RobTcpClient;				//用于tcp下accept后产生的新套接字
	SOCKADDR_IN m_RobTcpServerAddr;		//tcp,本地的地址信息
	SOCKADDR_IN m_RobTcpClientAddr;		//tcp,client端的地址信息
	short m_comipport;                  //端口
	char m_chRobRecvBuff[MaxRecvLength];
	int m_iByteFromTcpClient;
	int m_sendchar[MaxSendLength];		//返回给客户端信息
	int TempRevTurnPos[4];
	int NUM;
	int TempTurnVel[4];
	int TempMoveVel[4];
	int RevTurnpos[4]; 
	int TempRevTurnpos[4]; 
	int RevFrontpos[4]; 
	int countnub;
	long VelocityTurnPara;

	UINT IRLimted;
	BOOL motorstation[8];
	UINT templednub;
	// Generated message map functions
	//{{AFX_MSG(CRobotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDownload();
	afx_msg void OnMotion1stop();
	afx_msg void OnMotion3stop();
	afx_msg void OnMotion5stop();
	afx_msg void OnMotion7stop();
	afx_msg void OnVPSetting();
	afx_msg void OnSocketopen();
	afx_msg void OnDestroy();
	afx_msg void OnMotionstop();
	afx_msg void OnResetZero();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMotion1movev();
	afx_msg void OnMotion3movev();
	afx_msg void OnMotion5movev();
	afx_msg void OnMotion7movev();
	afx_msg void OnControlHold();
	afx_msg LONG OnComm(WPARAM ch,LPARAM port);
	afx_msg void OnChoroad();
	afx_msg void OnTurn90();
	afx_msg void OnFront();
	afx_msg void OnRear();
	afx_msg void OnTurn57();
	afx_msg void OnTurnZero();
	afx_msg void OnFront2();
	afx_msg void OnRear2();
	afx_msg void OnStopAll();
	afx_msg void OnHoldDownBroad();
	afx_msg void OnHoldUpBroad();
	afx_msg void OnReSetEncoder();
	afx_msg void OnMoveZero();
	afx_msg void OnChangeHoldUp2();
	afx_msg void OnHoldStopBroad();
	afx_msg void OnCloseLight();
	afx_msg void OnRedLight();
	afx_msg void OnGreenLight();
	afx_msg void OnResetZero2();
	afx_msg void OnMotion1movev2();
	afx_msg void OnMotion3movev2();
	afx_msg void OnMotion5movev2();
	afx_msg void OnMotion7movev2();
	afx_msg void OnResetZero3();
	afx_msg void OnPETHoldHeight();
	afx_msg void OnCTHoldHeight();
	afx_msg void OnMRIHoldHeight();
	afx_msg void OnReadHold();
	afx_msg void OnDownHoldPos();
	afx_msg void OnResetMotionPara();
	afx_msg void OnMotionMoveState();
	afx_msg void OnRVSetting1();
	afx_msg void OnReZeroPosition();
	afx_msg void OnRevHoldPos();
	afx_msg void OnReadHoldPos();
	afx_msg void OnButton5();
	afx_msg void OnNormalTurnLeft();
	afx_msg void OnNormalTurnRight();
	afx_msg void OnNormalFront();
	afx_msg void OnNormalRear();
	afx_msg void OnNormalStopAll();
	afx_msg void OnStopIR();
	afx_msg void OnStartIR();
	afx_msg void OnResetZeroLeft();
	afx_msg void OnResetZeroRight();
	afx_msg void OnSonorsettinf();
	afx_msg void OnTurn91();
	afx_msg void OnRaedEncoder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTDLG_H__5DC721AD_BBD2_4B49_B408_44146E2E0EC2__INCLUDED_)
