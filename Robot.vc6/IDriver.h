// IDriver.h: interface for the CDriver class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __IDRIVER_ELMO_23490729038472903749023874290847
#define __IDRIVER_ELMO_23490729038472903749023874290847

#define  WM__STEP				WM_USER+1
#define  WM__BEGIN				WM_USER+2
#define  WM__FINISH				WM_USER+3

#define  PVT_STOP				WM_USER+13
#define  PVT_INIT_DONE			WM_USER+14
#define  WM_PVT_DONE   			WM_USER+17

#define  WM_FIRMDOWNLOAD_ERROR  WM_USER+1025
#define  WM_FIRMDOWNLOAD_OK		WM_USER+1026
#define  WM_FIRMDOWNLOAD_STEP	WM_USER+1027

#define  MAX_TABLE_BUF_PVT		64
#define  MAX_TABLE_BUF_PT		64
#define  MANUFACTURE_NAME_SIZE	50
#define  BOARDTYPE_NAME_SIZE	255

enum COMMUNICATION_TYPE{CT_NONE=0, CT_RS232=1, CT_CAN=2};
enum CAN_BOUD_RATE{BR_10=0, BR_20=1, BR_50=2, BR_100=3, BR_125=4, BR_250=5, BR_500=6, BR_1000=7};

struct SCommunicationInfo
{
	char sLineName[_MAX_PATH]; // User Description
	COMMUNICATION_TYPE CommType;
	struct RS_232
	{
		BYTE btPort; // 1,2,3,...,8 (COM1, COM2, ..., COM8)
		DWORD dwBaudRate; // CBR_4800=4800, CBR_9600=9600, CBR_19200=19200, CBR_38400=38400, CBR_57600=57600, CBR_115200=115200
		BYTE btParity; //NOPARITY = 0, ODDPARITY = 1, EVENPARITY = 2
	} rs;
	struct CAN
	{
		BYTE  btNodeID; // 1,2,3,...,127
		char  sManufacture[MANUFACTURE_NAME_SIZE]; // "IXXAT"
		char  sBoardType[BOARDTYPE_NAME_SIZE]; // "tinCAN", "iPC-I 165", "iPC-I 320", "iPC-I 386", "iPC-I 165 PCI", "iPC-I 320 PCI"...
		BYTE  btCanNr; // 0,1
		CAN_BOUD_RATE BaudRateIndex;
		BYTE  bt_irq1; // Interrupt Number
		WORD dw_address1; // Board Address/Segment Number
	} can;
};

enum FAST_MOTION_TYPE{PVT_TYPE=0, PT_TYPE};
struct FAST_MOTION_PARAM
{
	unsigned int StructSize;
	FAST_MOTION_TYPE MotionType;//0 - PVT, 1 - PT; 
	unsigned int StaticTrajLength;// Length of data in points number
	//unsigned int FirstRow;//MP[1] = First valid row in PVT table	
	//unsigned int LastRow;//MP[2] = Last valid row in PVT table
	//unsigned int QueueLowAlarm;	//MP[5] = Number of yet unexecuted table rows for queue low alarm.
	//unsigned int WritePointer;//MP[6] = Initial value for the write pointer 
	unsigned int PTsegmentTime;// MP[4] For PT only.The number of controller sampling times in each PT motion segment 		
	//struct CYCLICAL_MOTION //MP[3]
	//{
	//	unsigned CyclicalMotion :1;//Bit0  = Cyclical motion (0 non-cyclical, 1 cyclical)
	//	unsigned ExpectedStop   :1;//Bit1 = Expected stop (0 - Issue Emergency on stop, 1 - Expect stop)
	//	unsigned                :30;//Unused
	//} MP_3;
	BOOL m_bRepetitive;
	HWND m_hControlWnd; //To this window will be sand massages about Fast Motion Status
							// PVT_STOP	- Motion terminate
							// PVT_INIT_DONE - Finish init Fast Motion 
							// WM_PVT_DONE  - Fast Motion Stoped (Trajectory was done)
};

interface AFX_EXT_CLASS IDriver;

typedef void (__cdecl *DRIVER_CALLBACK)(IDriver *pDriver, const char *lpcszFormatError);
typedef	struct __EMERGENCY_STRUCT 
{
	unsigned short ErrorCode;
	BYTE           ErrorRegister;
	BYTE           ElmoErrorCode;
	unsigned short DataField1;
	unsigned short DataField2;
}EMERGENCY_STRUCT;
typedef void (__cdecl *EMERGENCY_CALLBACK)(IDriver *pDriver, const EMERGENCY_STRUCT *pEMCY,const char *lpcszFormatError);

interface AFX_EXT_CLASS ICanService
{
	virtual void SetEmergencyCallback(EMERGENCY_CALLBACK pfnCANEmergencyCallback)=0;
	virtual BOOL SendCommandGroupID(BYTE GroupID, const CString &sOutput,CString& sError)=0;
	virtual BOOL SendBGSyncOnTime(DWORD Delay,BYTE NodeID,CString &sError)=0;	
	virtual BOOL ReadyFastMotion(CString& sError)=0;
	virtual BOOL StopFastMotion(CString& sError)=0;
	virtual BOOL InitFastMotionMode(const FAST_MOTION_PARAM* pParam, CString& sError)=0;
	virtual BOOL SetPVTPoint(__int32 Pos, __int32 Vel, BYTE time, unsigned long counter, CString& sError)=0;
	virtual BOOL SetPTPoint(__int32 Pos1, __int32 Pos2, unsigned long counter, CString& sError)=0;
	virtual BOOL CreateSyncManager(CString& sError)=0; 
	static void DestroySyncManager();
};

interface AFX_EXT_CLASS ITabMotion
{
	virtual BOOL InitPVT(CString& sError, HWND hWnd=NULL)=0;
	virtual BOOL InitPVTNoAutoSync(CString& sError, HWND hWnd=NULL)=0;//Call function StartSYNC(WORD SyncPeriod) before
	virtual BOOL AddPVTPoint(__int32 Pos, __int32 Vel, BYTE time, CString& sError)=0;
	virtual UINT GetQueueSize()=0;
	virtual BOOL StopSYNC()=0;
	virtual BOOL StartSYNC(WORD SyncPeriod)= 0;//This function MUST BE call before InitPVT	
	virtual void SetEmergencyCallback1(EMERGENCY_CALLBACK pfnCANEmergencyCallback)=0;
	virtual BOOL SendCommandGroupID1(BYTE GroupID, const CString &sOutput,CString& sError)=0;
	virtual BOOL SendBGSyncOnTime1(DWORD Delay,BYTE NodeID,CString &sError)=0;	
	virtual DWORD GetDriveTimer()=0;
	
};

interface ICanOpen;
interface AFX_EXT_CLASS IDriver
{
	virtual void SetErrorCallback(DRIVER_CALLBACK pfnErrorProc)=0;
	virtual BOOL SendCommand(CString sInput, CString& sOutput, CString& sError,DWORD TimeOut=5000)=0;
	virtual BOOL SendCommand(CString sInput, CString& sOutput,DWORD TimeOut=5000)=0;
	virtual COMMUNICATION_TYPE GetComInfo(CString *pstrCommunicationInfo = NULL)=0;
	virtual int DownloadFirmware()=0;
	virtual int DownloadFirmwareEx(CString sPathToFile, BOOL bBoot, HWND hProgressWindow=NULL)=0;
	virtual BOOL UploadApplication(CString sPathToFile, CString &sError)=0;
	virtual BOOL DownloadApplication(CString sPathToFile, CString &sError, HWND hProgressWindow=NULL)=0;
	virtual BOOL UploadProgram(CString sPathToFile, CString &sError)=0;
	virtual BOOL DownloadProgram(CString sPathToFile, CString &sError, HWND hProgressWindow=NULL)=0;
	static IDriver* DriverConnect(DRIVER_CALLBACK pfnErrorProc=NULL);
	static IDriver* DriverConnect(CString sPathToApplicationFile, DRIVER_CALLBACK pfnErrorProc=NULL);
	static IDriver* DriverConnect(const SCommunicationInfo &SComInfo, DRIVER_CALLBACK pfnErrorProc=NULL);
	static void DriverDisconnect(IDriver *);
	virtual BOOL IsEqualCommunicationParam(CString sPathToApplicationFile, CString &pstrCommunicationInfoFromFile)=0;
	virtual ICanService * GetCanService()=0;
	virtual ITabMotion * GetTabMotion()=0;
	static UINT GetSizeSComInfoCollection(CString pathToFile);
	static BOOL SaveSComInfoCollection(CString sPathToNetworkFile, CString &sError);
	//return FALSE if end-of-file was reached without reading any data	
	static BOOL LoadSComInfoFromCollection(SCommunicationInfo &SComInfo,CString sPathToNetworkFile,CString &sError,int index = 0);
public:
};	

#endif //#ifndef __IDRIVER_ELMO_23490729038472903749023874290847