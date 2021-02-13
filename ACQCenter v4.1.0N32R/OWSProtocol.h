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

/// OWS<->ACS ����ͨ��Э��ͷ����
/** OWS��ACS��������ʱ���ȷ���OWS_HEAD���ٷ�����Ӧ�����ݡ�
    ���ڹ���ͨ��Э�����ϸ���ͣ���ο�������ṩ���ĵ���
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
//                       �ɼ�Э��ṹ

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

//				   �ɼ�Э��ṹ
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
	float fBed_Elevation;	   ///< ���߶�(cm)
	float fInitBed_Position;   ///< ����ʼ����λ��(cm)
	DWORD dwFrameStartTime;    ///< ��һ֡��ʼʱ��(ms)
}ACQ_SYSINFO;
//  PET_SYSINFO
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//  PET_MONITOR
typedef struct _acq_monitor
{
    DWORD cCurrFrameNumber;        ///< ��ǰ֡��
	DWORD dwCurrFrameStartTime;   ///< ��ǰ֡��ʼʱ��(ms)
	DWORD cCurrFrameClass;         ///< ��ǰ֡���
	float fCurrBedPosition;       ///< ��ǰ��λ��
	DWORD dwDelayed;              ///< ֡���ſ��е��ӳټ���
	DWORD dwMultiples;            ///< ֡���ſ��еĶ��ؼ���
	DWORD dwNetTrues;             ///< ��ʵ����
	float fWobbleTilt;            ///< ��Դת��(��/��)
	DWORD dwCoinDifData;          ///< �������� 
	union{
		DWORD  cFlag;                  //��ǣ�
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
    DWORD dwTimeInstant;      ///< ��������ʱ�䣨ns��
	DWORD dwTimeMin;		  ///< ��С�ӳٷ���ʱ��
	DWORD dwTimeMax;		  ///< ����ӳٷ���ʱ��
    DWORD dwEcgSampleFreq;    ///< ECG����Ƶ��: 0->1600 1->200 2->400 3->800 4->1600 5->3200 6->6400
	DWORD dwEcgPreAmpFactor;  ///< ECGǰ������(0~7)
	DWORD dwCoinFmt;          ///< ����ķ������ݸ�ʽ��0: Pos Info  1:Time Info
	BOOL  bEnEMControl;       ///< ����Э����ƻ����·
	BOOL  bEnDbgBucket;       ///< ����Э�����BUCKET
	DWORD Reserved[20];	
}INITFILE_ACS;


//
///////////////////////////////////////////////////////////////////////



#pragma pack(pop)
#endif   //__OWSPROTOCOL_H__
