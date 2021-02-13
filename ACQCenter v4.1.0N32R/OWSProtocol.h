////////////////////////////////////////////////////////////////////////////////
//  DEFINE THE COMMUNICATION PORTOCOL BETWEEN ACS AND OWS
//	BASED ON DOCUMENT PUBLISHED 31/05/2002
////////////////////////////////////////////////////////////////////////////////

#ifndef   __OWSPROTOCOL_H__
#define   __OWSPROTOCOL_H__

#pragma pack(push)
#pragma pack(1)
//#define OWS_IP "192.168.0.42"
//#define OWS_PORT 8080

/// OWS<->ACS 光纤通信协议头定义
/** OWS与ACS传输数据时，先发送OWS_HEAD，再发送相应的数据。
    关于光纤通行协议的详细解释，请参考软件组提供的文档。
*/

typedef struct _ows_head
{
    DWORD dwDatLen;		 ///< include the size of OWS_HEAD 	  
	BYTE  Cmd;			 ///< defined by PET_XXXXXX		   	
	BYTE  cFlag[3];      ///< fixed to 0x7E7E7E			   
}OWS_HEAD;

/////////////////////////////////////////////////////////////////
//                        CMD DEFINE

//INIT CMD			              DIRECTION  
#define PET_LOGIN        0	   // ACS -> OWS
#define PET_INITFILE     1	   // OWS -> ACS
#define PET_INITOK       2     // ACS -> OWS
#define PET_SYNTIME      3     // ACS -> OWS

//TEST AND MAINTAIN	 CMD          DIRECTION
#define PET_TESTREQ      10    // OWS -> ACS
#define PET_TESTRET      11    // ACS -> OWS

//DAT ACQUIRE CMD                 DIRECTION
#define PET_PROTOCOL     20    // OWS -> ACS
#define PET_STOP         21    // OWS -> ACS
#define PET_SKIP         22    // OWS -> ACS
#define PET_STARTBEAT    23    // OWS -> ACS
#define PET_STOPBEAT     24    // OWS -> ACS

#define PET_LISTMODE     80    // ACS -> OWS
#define PET_BEAT         31    // ACS -> OWS
#define PET_MONITOR      32    // ACS -> OWS
#define PET_SYSINFO      33    // ACS -> OWS
#define PET_ACK          34    // ACS -> OWS
#define PET_NACK         35    // ACS -> OWS
#define PET_FRAMEOK      36    // ACS -> OWS
#define PET_FRAMESTART   37    // ACS -> OWS
#define PET_ALLPROTOCOLSOVER  38    // OWS -> ACS

//SYSTEM CMD					  DIRECTION
#define PET_ALARM        60    // ACS -> OWS
#define PET_STATUS       61    // OWS -> ACS
#define PET_WARN         62    // ACS -> OWS
#define PET_QUIT         63    // OWS -> ACS

//                       END CMD DEFINE
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//                       采集协议结构

typedef struct _acq_head
{
	int   nBlockSize;
	char  cBlockName[4];
	BOOL  LogFlag;
	BYTE  ScanClass;
	int   nTotalTime;
	BOOL  SeptaStatus;
	BOOL  RodStatus;
	int   RodSpeed;     ///< 1:2:3
	BYTE  RingDifference;  ///< FOV
	BYTE  Span;			   ///< ring difference
	float BedOffset;
	float BedPosition;
	float AxialFov;		   ///< no use
	int   ULD;			   ///< no use
	int   LLD;			   ///< no use
	int   ScatterEnery;	   ///< 0~3
	BYTE  DetectionMode;   ///< no use
	int   FrameNumber;
	//ACQ_FRAME_PARAM frameParam[0];
	//     ......
	//ACQ_FRAME_PARAM frameParam[FrameNumber-1]; 
	//ACQ_GATE_HEAD   gateHead;
	//ACQ_GATE_PARAM  gateParam[0];
	//     ......
	//ACQ_GATE_PARAM  gateParam[n];
}ACQ_HEAD;

typedef struct _acq_frame_param
{
	BYTE  FrameClass;
	BOOL  CountFlag;
	DWORD FrameTime;
	DWORD FrameCount;
	DWORD IntervalTime;
}ACQ_FRAME_PARAM;

typedef struct _acq_gate_head
{
	BOOL  GateClass;
	int   UPB;
	int   LPB;
	int   BeatSkip;
	BOOL  PhaseOrFixed;
	int   GateStop;          //0: user stop; none zero:stop by event cnt
	int   ForwordRRNumber;
	int   ForwordRRPer;
	int   BackwordRRNumber;
	int   BackwordRRPer;
}ACQ_GATE_HEAD;

typedef struct _acq_gate_param
{
	int GateBeginTime;
	int GateEndTime;
}ACQ_GATE_PARAM;

typedef struct _acq_protocol
{
	ACQ_HEAD  acq_head;
	ACQ_FRAME_PARAM* pFrame;
	ACQ_GATE_HEAD gate_head;
	ACQ_GATE_PARAM* pGate;
}ACQ_PROTOCOL;

//				   采集协议结构
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//  PET_SYNTIME
typedef struct _PET_SYSTEMTIME { 
    WORD wYear; 
    WORD wMonth; 
    WORD wDayOfWeek; 
    WORD wDay; 
    WORD wHour; 
    WORD wMinute; 
    WORD wSecond; 
	WORD wMilliseconds; 
	DWORD dwTickCount;
} PET_SYSTEMTIME;
//
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//  PET_SYSINFO
typedef struct _acq_sysinfo
{
	float fBed_Elevation;	   ///< 床高度(cm)
	float fInitBed_Position;   ///< 床初始绝对位置(cm)
	DWORD dwFrameStartTime;    ///< 第一帧开始时间(ms)
}ACQ_SYSINFO;
//  PET_SYSINFO
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//  PET_MONITOR
typedef struct _acq_monitor
{
    DWORD cCurrFrameNumber;        ///< 当前帧数
	DWORD dwCurrFrameStartTime;   ///< 当前帧开始时间(ms)
	DWORD cCurrFrameClass;         ///< 当前帧类别
	float fCurrBedPosition;       ///< 当前床位置
	DWORD dwDelayed;              ///< 帧或门控中的延迟记数
	DWORD dwMultiples;            ///< 帧或门控中的多重记数
	DWORD dwNetTrues;             ///< 真实记数
	float fWobbleTilt;            ///< 棒源转速(度/分)
	DWORD dwCoinDifData;          ///< 符合数据 
	union{
		DWORD  cFlag;                  //标记，
		struct{
			DWORD bAcqStart : 8;
			DWORD bBedMove  : 8;
			DWORD Rod_Stat  : 8;
			DWORD Septa_Stat : 8;
		};
	};
}ACQ_MONITOR;
//  PET_MONITOR
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// PET_INITFILE

#define INIT_OBJ_BUCKET_0      0
#define INIT_OBJ_BUCKET_31     31
#define INIT_OBJ_BUCKET_ALL    32
#define INIT_OBJ_GANTRY        41
#define INIT_OBJ_ROD           42
#define INIT_OBJ_SEPTA         43
#define INIT_OBJ_BED           44
#define INIT_OBJ_YE1           45
#define INIT_OBJ_YE2           46
#define INIT_OBJ_ACS           47

typedef struct _initfile_head
{
	char  cFlag[8];           ///< const:"PET_INIT"
	WORD  nInitObj;
}INITFILE_HEAD;

typedef struct _init_item
{
	char szKey[32];
	DWORD dwValue;
}INIT_ITEM;

typedef struct _initfile_acs
{
    DWORD dwTimeInstant;      ///< 立即符合时间（ns）
	DWORD dwTimeMin;		  ///< 最小延迟符合时间
	DWORD dwTimeMax;		  ///< 最大延迟符合时间
    DWORD dwEcgSampleFreq;    ///< ECG采样频率: 0->1600 1->200 2->400 3->800 4->1600 5->3200 6->6400
	DWORD dwEcgPreAmpFactor;  ///< ECG前方倍数(0~7)
	DWORD dwCoinFmt;          ///< 输出的符合数据格式。0: Pos Info  1:Time Info
	BOOL  bEnEMControl;       ///< 允许协议控制机电电路
	BOOL  bEnDbgBucket;       ///< 允许协议调试BUCKET
	DWORD Reserved[20];	
}INITFILE_ACS;


//
///////////////////////////////////////////////////////////////////////



#pragma pack(pop)
#endif   //__OWSPROTOCOL_H__
