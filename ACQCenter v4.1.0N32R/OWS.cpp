#include "stdafx.h"
#include "ACQCenter.h"
#include "ACQCenterDlg.h"

#include "OWS.h"
#include "ExecProtocol.h"

#include "SIO.h"
//#include "BuckSIO.h"
#include "SIOProtocol.h"

#include "FH.h"

#include "DbgPrt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWndOWS;
extern HWND g_hWndSIO;
extern HWND g_hWndProtocol;

extern BOOL g_bOWSQuitNormal;
extern COWS g_OWS;
#ifndef _FH_SIM
	extern CFH  g_FH;
#else
	extern CFHSim g_FH;
#endif
extern CSIO g_SIO;
//extern CBuckSIO g_BuckSIO;

extern HANDLE g_hEvtSysError;

volatile extern MCTL_INFO   g_MCtlInfo;   //机电控制电路信息
void ExecEMCReset();

////////////////////////////////////////////////////////////////////
//              visiable Global
HANDLE g_hEvtOWS_ConnectLost;	//trig when connect to ows lost
int g_nRodRate[3];
//
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//              visiable in this file only
CMySocket    g_Socket;
DWORD WINAPI ThreadMonitorOwsCmd(LPVOID  lpData);

HANDLE g_hEvtFrameOK;
//
////////////////////////////////////////////////////////////////////

COWS::COWS()
{
	m_hThread = NULL;
	m_hEvtExit = CreateEvent(NULL, FALSE, FALSE, NULL);

	g_hEvtOWS_ConnectLost = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_bProtocolValid = FALSE;
	m_nCurrFrame = 0;
	g_hEvtFrameOK = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_CLockProtocol);

	InitializeCriticalSection(&m_CLockSend);

	g_nRodRate[0] = 8;
	g_nRodRate[1] = 12;
	g_nRodRate[2] = 18;
}

COWS::~COWS()
{
   CloseHandle(m_hEvtExit);
   CloseHandle(g_hEvtOWS_ConnectLost);

   DeleteCriticalSection(&m_CLockProtocol);
   CloseHandle(g_hEvtFrameOK);

   DeleteCriticalSection(&m_CLockSend);

}

BOOL COWS::Connect(const char* pszHostIP, int nPort, int RetryCnt)
{
	//Init Console Socket(FC)
	int errCnt=0;
	DWORD opt1 = 0;
	#if DBG_LEVEL != 0
		char szTemp[512];
	#endif

	g_bOWSQuitNormal = FALSE;
	errCnt = 0;
	while(!g_Socket.Connect(pszHostIP, nPort)){
		#if DBG_LEVEL != 0
			int err = WSAGetLastError();
			sprintf(szTemp, "Connet to OWS Error,ErrCode=%8.8x, Retry=%d\r\n", err, errCnt+1);
			DbgPrt(szTemp, g_hWndOWS);
		#endif
		if( RetryCnt == -1 ) continue;
		if(errCnt++ >= RetryCnt) {
			DbgPrt("Stop Retry to Connect OWS\r\n", g_hWndOWS);
			g_Socket.Close();
			return FALSE;
		}
	}
	DbgPrt("Connet to OWS OK\r\n", g_hWndOWS);

    m_ThreadParam.pSocket = &g_Socket;
	m_ThreadParam.hEvtExit = m_hEvtExit;
	m_hThread = CreateThread(NULL, 0, ThreadMonitorOwsCmd,
				             &m_ThreadParam, 0,
							 &m_dwThreadID
							);

	BOOL ret = SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	//start send pet_monitor and pet_sysinfo thread
	DWORD dwThreadMonitor;
	CreateThread( NULL, 0, 
		          ThreadExecProtocolMonitor,
				  g_hEvtOWS_ConnectLost, 0,
				  &dwThreadMonitor
				);
	return TRUE;
}

BOOL COWS::Close()
{
	//EnterCriticalSection(&m_CLockSend);
		g_Socket.Close();
	//LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

DWORD WINAPI ThreadMonitorOwsCmd(LPVOID  lpData)
{
	OWS_HEAD head;
	BYTE*  pOwsDat;	 WORD* pwOwsDat;
	DWORD  dwDatLen;
	
	HANDLE hThreadExec = NULL;
	DWORD dwID;
	EXEC_THREAD_PARAM ThreadParam;

	HANDLE hEvtFrameSkip;    //skip current frame
	HANDLE hEvtProtocolStop; //stop current acq protocol
	
	OWS_THREAD_PARAM* pParam;
	DWORD ret;

#if DBG_LEVEL != 0
	char   szTemp[1024];
#endif

	hEvtFrameSkip = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEvtProtocolStop  = CreateEvent(NULL, TRUE, FALSE, NULL);

	pParam = (OWS_THREAD_PARAM*)lpData;

	//g_OWS.SendLogin();

    #define OWD_DAT_LEN  (256*1024)
	pOwsDat = (BYTE*)GlobalAlloc(GMEM_FIXED, OWD_DAT_LEN);
	pwOwsDat = (WORD*)pOwsDat;
	dwDatLen = OWD_DAT_LEN;

	do{
		//receive ows cmd head
		ret = g_Socket.Receive(&head, sizeof(OWS_HEAD));
		if(ret == 0){//connection closed
			DbgPrt("Connection Closed(0)\r\n", g_hWndOWS);

			//cause system stop acq and reboot
			SetEvent(g_hEvtOWS_ConnectLost);	  
			break;
		}else if(ret == SOCKET_ERROR ){
			#if DBG_LEVEL != 0
				//ret = pParam->pSocket->GetLastError();
				sprintf(szTemp, "Receive OWS Head Error, ErrCode=%8.8x.\r\n", ret);
				DbgPrt(szTemp, g_hWndOWS);
			#endif
			SetEvent(g_hEvtOWS_ConnectLost);
			break;
		}

		//ASSERT(ret == sizeof(head));

		if( head.cFlag[0] != 0x7E || 
			head.cFlag[1] != 0x7E ||
			head.cFlag[2] != 0x7E
		  )
		{
			DbgPrt("OWS head Flag error!\r\n", g_hWndOWS);
			continue;
		}

		if( head.dwDatLen > dwDatLen)
		{
			pOwsDat = (BYTE*)GlobalReAlloc(pOwsDat, head.dwDatLen, GMEM_FIXED);
			dwDatLen = head.dwDatLen;
		}
		if( head.dwDatLen-sizeof(head) != 0)
		{
			ret = g_Socket.Receive( pOwsDat, 
									head.dwDatLen-8
								  );
			//ASSERT(ret == head.dwDatLen-8);
		}

		switch(head.Cmd){
			case PET_PROTOCOL:
				 DbgPrt("OWS==>PET_PORTOCOL\r\n", g_hWndOWS);

				 if(!g_OWS.ProtocolSave(pOwsDat, head.dwDatLen-8))
				 {
					 g_OWS.SendAlarm(ERROR_OWS_PROTOCOL);
					 g_OWS.SendNACK();
					 break;
				 }

				 //begin exec protocol
				 ThreadParam.hEvtFrameOK = g_hEvtFrameOK;
				 ThreadParam.hEvtFrameSkip = hEvtFrameSkip;
				 ThreadParam.hEvtProtocolStop = hEvtProtocolStop;

				 ResetEvent(hEvtFrameSkip);
				 ResetEvent(hEvtProtocolStop);
				 ResetEvent(g_hEvtSysError);

				 hThreadExec = CreateThread( NULL, 0, 
					                         ThreadExecProtocol, 
											 &ThreadParam, 
											 0, &dwID
										   );
				 break;

			case PET_SKIP:   //终止当前帧
				 DbgPrt("OWS==>PET_SKIP\r\n",g_hWndOWS);

				 SetEvent(hEvtFrameSkip);
				 break;

			case PET_STOP:  //终止当前所有帧
				 DbgPrt("OWS==>PET_STOP\r\n",g_hWndOWS);

				 SetEvent(hEvtProtocolStop);
				 break;

			case PET_ALLPROTOCOLSOVER:  //EMC RESET
				 DbgPrt("OWS==>PET_ALLPROTOCOL_OVER\r\n", g_hWndOWS);
				 DbgPrt("Begin Control ROD&SEPTA In\r\n", g_hWndProtocol);
				 ExecEMCReset();
				 DbgPrt("End Control ROD&SEPTA In\r\n", g_hWndProtocol);
				 break;

			case PET_STARTBEAT:
				 DbgPrt("OWS==>PET_STARTBEAT\r\n",g_hWndOWS);

				 g_FH.EcgStart(1024);
				 break;

			case PET_STOPBEAT:
				 DbgPrt("OWS==>PET_STOPBEAT\r\n",g_hWndOWS);

				 g_FH.EcgStop();
				 break;

			case PET_INITFILE:
				{
				 //DbgPrt("OWS==>PET_INITFILE\r\n", g_hWndOWS);
				 int datLen = head.dwDatLen-sizeof(head);
				 g_OWS.ExecPetInit(pOwsDat, datLen);
				}
				 break;

			case PET_STATUS:
				 //改变系统状态  0:acq 1:维护 2:测试
				 switch(pwOwsDat[0]){
				 case 0:
				 	  g_SIO.SetSIOMode( FALSE );
//					  g_BuckSIO.SetSIOMode( FALSE );
					  DbgPrt("OWS==>PET_STATUS, status=ACQ\r\n", g_hWndOWS);
					  break;
				 case 1:
					  g_SIO.SetSIOMode( TRUE );
//					  g_BuckSIO.SetSIOMode( TRUE );
					  DbgPrt("OWS==>PET_STATUS, status=MAINTAIN\r\n", g_hWndOWS);
					  break;
				 case 2:
					  g_SIO.SetSIOMode( TRUE );
//					  g_BuckSIO.SetSIOMode( TRUE );
					  DbgPrt("OWS==>PET_STATUS, status=TEST\r\n", g_hWndOWS);
					  break;
				 default:
					  sprintf(szTemp, "OWS==>PET_STATUS, status=UNKNOW(%d) XXXXXXXXXX\r\n", pwOwsDat[0]);
					  DbgPrt(szTemp, g_hWndOWS);
					  break;
				 }
				 break;

				 //系统维护脕E丒
			case PET_TESTREQ:
				 switch( pOwsDat[0] )
				 {
				 case INIT_OBJ_BUCKET_ALL: //bucket
					  sprintf(szTemp, "OWS==>PET_TESTREQ, OBJ=BUCKET(%2.2x)\r\n", pOwsDat[0]);
//					  g_BuckSIO.SendTestData(&pOwsDat[1], head.dwDatLen-sizeof(head)-1 );
					  break;
				 case INIT_OBJ_GANTRY: //grantry
				 case INIT_OBJ_ROD: //rod
				 case INIT_OBJ_SEPTA: //septa
				 case INIT_OBJ_BED: //bed
				 case INIT_OBJ_YE1: //ye1
				 case INIT_OBJ_YE2: //ye2
					  sprintf(szTemp, "OWS==>PET_TESTREQ, OBJ=GANTRY(%2.2x)\r\n", pOwsDat[0]);
					  g_SIO.SendTestDat(&pOwsDat[1], head.dwDatLen-sizeof(head)-1 );
					  break;

				 case INIT_OBJ_ACS: //acq computer
					  sprintf(szTemp, "OWS==>PET_TESTREQ, OBJ=ACS(%2.2x)\r\n", pOwsDat[0]);
					  break;

				 default:
					  #if DBG_LEVEL != 0
					      sprintf(szTemp,"OWS==>PET_TESTREQ, Obj=Unknow(%2.2x)\r\n", pwOwsDat[0]);
					  #endif
					  break;
				 }
			     DbgPrt(szTemp, g_hWndOWS);
				 break;

			case PET_QUIT:
				 DbgPrt("OWS==>PET_QUIT\r\n",g_hWndOWS);

				 g_bOWSQuitNormal = TRUE;
				 break;

			default:
				 #if DBG_LEVEL != 0
				 sprintf(szTemp, "XXXXX  unknow ows cmd, cmd head=[%8.8x  %2.2 %2.2 %2.2 %2.2]\r\n",
					                             head.dwDatLen, head.Cmd,
												 head.cFlag[0], head.cFlag[1], head.cFlag[2]
						);
				 DbgPrt(szTemp, g_hWndOWS);
			     #endif
				 break;
		}
	}while(TRUE);

	GlobalFree(pOwsDat);
	CloseHandle(hEvtFrameSkip);
	CloseHandle(hEvtProtocolStop);

	//TRACE("exit ows\r\n");
	DbgPrt("Connection to OWS Has Lost!\r\n", g_hWndOWS);
	g_OWS.Close();

	return TRUE;
}

BOOL COWS::SetCurrFrame(int nFrameNum)
{
	EnterCriticalSection(&m_CLockProtocol);

	if( nFrameNum >= m_AcqProtocol.acq_head.FrameNumber)
	{
		LeaveCriticalSection(&m_CLockProtocol);
		return FALSE;
	}

	m_nCurrFrame = nFrameNum;
	m_bFrameFirstDat = TRUE;
	m_dwCurrFrameStartTime = GetTickCount();
	m_dwCurrEvtCnt_Frame = 0;
	ResetEvent(g_hEvtFrameOK);

	LeaveCriticalSection(&m_CLockProtocol);

	return TRUE;
}

void COWS::SetFrameOKEvt()
{
	SetEvent(g_hEvtFrameOK);
	return;
}

void COWS::SetFrameStart(BOOL bStart)
{
	m_bFrameStart = bStart;
	return;
}


BOOL COWS::SendLogin()
{
	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_LOGIN\r\n",g_hWndOWS);

	m_owsSendHead.Cmd = PET_LOGIN;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;

	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD)	){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendSynTime()
{
	PET_SYSTEMTIME petTime;

	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_SYNTIME\r\n", g_hWndOWS);

	m_owsSendHead.Cmd = PET_SYNTIME;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+sizeof(petTime);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;

	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD)	){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	GetSystemTime( ((SYSTEMTIME*)(&petTime)) );
	petTime.dwTickCount = GetTickCount();

	if( g_Socket.Send(&petTime, sizeof(petTime)) != sizeof(petTime)	){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendAlarm(WORD wAlarm)
{
	EnterCriticalSection(&m_CLockSend);

	#if DBG_LEVEL != 0
	char szTemp[512];
	sprintf(szTemp, "OWS<==PET_Alarm, Code=%4.4x\r\n", wAlarm);
	DbgPrt(szTemp, g_hWndOWS);
	#endif

	m_owsSendHead.Cmd = PET_ALARM;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+sizeof(wAlarm);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;

	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));
	if( g_Socket.Send(&wAlarm, sizeof(WORD)) != sizeof(WORD) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendFrameOK()
{
	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_FrameOK\r\n", g_hWndProtocol);
	
	m_owsSendHead.Cmd = PET_FRAMEOK;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendFrameStart(WORD wFrameNum)
{
	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_FrameStart\r\n", g_hWndProtocol);
	
	m_owsSendHead.Cmd = PET_FRAMESTART;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+sizeof(wFrameNum);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	g_Socket.Send(&wFrameNum, sizeof(wFrameNum));

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendACK()
{
	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_ACK\r\n", g_hWndProtocol);

	m_owsSendHead.Cmd = PET_ACK;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}


BOOL COWS::SendTestRet(BYTE object, const BYTE *pBuf, int nBufLen)
{
	EnterCriticalSection(&m_CLockSend);

#if DBG_LEVEL != 0
	char szTemp[512];
	sprintf(szTemp, "OWS<==PET_TESTRET, Obj=%2.2x, len=%d\r\n", object, nBufLen);
	DbgPrt(szTemp, g_hWndOWS);
#endif

	m_owsSendHead.Cmd = PET_TESTRET;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+1+nBufLen;
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	g_Socket.Send(&object, 1);
	if( g_Socket.Send(pBuf, nBufLen) != nBufLen ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendSysInfo()
{
	if( g_bOWSQuitNormal ) return TRUE;

	EnterCriticalSection(&m_CLockSend);

	//DbgPrt("OWS<==PET_SYSINFO\r\n", g_hWndOWS);

	m_owsSendHead.Cmd = PET_SYSINFO;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+sizeof(ACQ_SYSINFO);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	m_SysInfo.dwFrameStartTime  = m_dwCurrFrameStartTime;   //帧开始时紒E
	m_SysInfo.fBed_Elevation    = (float)(g_MCtlInfo.wBed_posy/100.); //床高度（cm）
	m_SysInfo.fInitBed_Position = (float)(g_MCtlInfo.wBed_posx/100.); //床位置（cm）
	if( g_Socket.Send( &m_SysInfo, sizeof(m_SysInfo) ) != sizeof(m_SysInfo) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendMonitor()
{
	//if( !ProtocolIsValid() ) return FALSE;

	if( g_bOWSQuitNormal ) return TRUE;

	EnterCriticalSection(&m_CLockSend);

	//DbgPrt("OWS<==PET_MONITOR\r\n", g_hWndOWS);

	m_owsSendHead.Cmd = PET_MONITOR;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+sizeof(ACQ_MONITOR);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	m_Monitor.cCurrFrameNumber = m_nCurrFrame;
	m_Monitor.dwCurrFrameStartTime = m_dwCurrFrameStartTime;
	if ( ProtocolIsValid() ) m_Monitor.cCurrFrameClass = m_AcqProtocol.pFrame[m_nCurrFrame].FrameClass;
	else m_Monitor.cCurrFrameClass = 0;
	m_Monitor.fCurrBedPosition = (float)(g_MCtlInfo.wBed_posx/100.);   //mm->cm
	m_Monitor.dwDelayed   = 0; //no use, 帧中的延迟记数
	m_Monitor.dwMultiples = m_dwCurrEvtCnt_All;   //所有帧事例数
	m_Monitor.dwNetTrues  = m_dwCurrEvtCnt_Frame; //当前帧事例数
	m_Monitor.fWobbleTilt = g_MCtlInfo.wRod_rate; //棒源旋转速度
	m_Monitor.dwCoinDifData = 0x0;				  //符合数据
	m_Monitor.cFlag = 0;
	if( m_bFrameStart ) m_Monitor.bAcqStart = 1;
	else m_Monitor.bAcqStart = 0;
	if( g_MCtlInfo.Bed_stat == move) m_Monitor.bBedMove = 1;
	else  m_Monitor.bBedMove = 0;
	m_Monitor.Rod_Stat = g_MCtlInfo.Rod_stat;
	m_Monitor.Septa_Stat = g_MCtlInfo.septa_stat;//g_OWS.m_AcqProtocol.acq_head.SeptaStatus;

	if( g_Socket.Send(&m_Monitor, sizeof(m_Monitor)) != sizeof(m_Monitor) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendNACK()
{
	EnterCriticalSection(&m_CLockSend);

	DbgPrt("OWS<==PET_NACK\r\n", g_hWndProtocol);

	m_owsSendHead.Cmd = PET_NACK;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	if( g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD)) != sizeof(OWS_HEAD) ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

//发送符合数据；
//在以记数为帧结束眮E镜那榭鱿拢菁鞘柚檬录_hEvtFrameOK
//如果已满足帧记数，则不发送数据
BOOL COWS::SendFHDat(const BYTE* pBuf, DWORD dwBufLen, int nBufID)
{
	if(pBuf == NULL) return FALSE;
	if(dwBufLen == 0) return TRUE;
	if(!ProtocolIsValid()) return TRUE;
	if(!m_bFrameStart) return TRUE;

	DWORD* pdwBuf;   DWORD dwLen;
	const ACQ_PROTOCOL* pProto;

	//丢弃第一次数据
	if(m_bFrameFirstDat){
		m_bFrameFirstDat = FALSE;
		return TRUE;
	}

	//判断是否满足记数要莵E
	//EnterCriticalSection(&m_CLockProtocol);
	pProto = ProtocolGet();
	if( pProto == NULL ) return FALSE;
    
	DWORD dwCurrCount;
	if(pProto->pFrame[m_nCurrFrame].FrameClass == 0)  //记数停止方式
	{
		if(pProto->pFrame[m_nCurrFrame].CountFlag==TRUE) //绝对记数终止方式
			dwCurrCount = m_dwCurrEvtCnt_All;
		else //相对记数终止方式
			dwCurrCount = m_dwCurrEvtCnt_Frame;

		if( dwCurrCount >= pProto->pFrame[m_nCurrFrame].FrameCount ){
			DbgPrt("SKIP Send LISTMODE(EVT CNT)\r\n", g_hWndOWS);
			//LeaveCriticalSection(&m_CLockProtocol);
			return TRUE;
		}

		dwLen  = dwBufLen/4;
		pdwBuf = (DWORD*)pBuf;
		for(DWORD i=0; i<dwLen; i++) {
			if( (pdwBuf[i] & 0xc0000000) == 0 )	{
				m_dwCurrEvtCnt_Frame++;
				m_dwCurrEvtCnt_All++;
			}
		}

		if(pProto->pFrame[m_nCurrFrame].FrameClass==TRUE) //绝对记数终止方式
			dwCurrCount = m_dwCurrEvtCnt_All;
		else  //相对记数终止方式
			dwCurrCount = m_dwCurrEvtCnt_Frame;

		if( dwCurrCount >= pProto->pFrame[m_nCurrFrame].FrameCount)
			SetFrameOKEvt();
	}else{
		//非记数停止方式
		m_dwCurrEvtCnt_All += (dwBufLen/4);
		m_dwCurrEvtCnt_Frame += (dwBufLen/4);
	}
	//LeaveCriticalSection(&m_CLockProtocol);

	char szTemp[256];

	//发送数据
	EnterCriticalSection(&m_CLockSend);

	//dwBufLen = 64*1024;

	m_owsSendHead.Cmd = PET_LISTMODE;
	m_owsSendHead.dwDatLen = dwBufLen+sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	int send = g_Socket.Send(pBuf, dwBufLen);
	if( send != dwBufLen ){
		sprintf(szTemp, "Send FH Dat Error, ErrCode=%d, sended=%d : %d\r\n", WSAGetLastError(), send, dwBufLen);
		DbgPrt( szTemp, g_hWndOWS);
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}else{
		sprintf(szTemp, "OWS<==PET_ListMode, BufID=%d\r\n", nBufID);
		//DbgPrt(szTemp, g_hWndOWS);
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendEcgDat(const BYTE *pBuf, DWORD dwBufLen)
{
	if(pBuf == NULL) return FALSE;
	if(dwBufLen == 0) return TRUE;

	EnterCriticalSection(&m_CLockSend);

    DbgPrt("OWS<==PET_HEARTBEAT\r\n", g_hWndOWS);

	m_owsSendHead.Cmd = PET_BEAT;
	m_owsSendHead.dwDatLen = dwBufLen+sizeof(OWS_HEAD);
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	if( g_Socket.Send(pBuf, dwBufLen) != dwBufLen ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

BOOL COWS::SendPetInitOK(WORD obj)
{
	EnterCriticalSection(&m_CLockSend);

	char szTemp[128];
	sprintf(szTemp, "OWS<==PET_INITOK,OBJ=%4.4x\r\n", obj);
    DbgPrt(szTemp, g_hWndOWS);

	m_owsSendHead.Cmd = PET_INITOK;
	m_owsSendHead.dwDatLen = sizeof(OWS_HEAD)+2;
	m_owsSendHead.cFlag[0] = 0x7e;
	m_owsSendHead.cFlag[1] = 0x7e;
	m_owsSendHead.cFlag[2] = 0x7e;
	g_Socket.Send(&m_owsSendHead, sizeof(OWS_HEAD));

	if( g_Socket.Send(&obj, 2) != 2 ){
		LeaveCriticalSection(&m_CLockSend);
		return FALSE;
	}

	LeaveCriticalSection(&m_CLockSend);

	return TRUE;
}

inline BOOL COWS::ProtocolIsValid()
{
	return m_bProtocolValid;
}

BOOL COWS::ProtocolSave(const BYTE* pBuf, DWORD dwBufLen)
{
	DWORD offset, len;

	EnterCriticalSection(&m_CLockProtocol);
	
	if(ProtocolIsValid()) {
		DbgPrt("Save Protocol Err: Current Protocol Is Valid\r\n", g_hWndOWS);
		LeaveCriticalSection(&m_CLockProtocol);
		return FALSE;
	}
	
	//Save Head
	////////pBuf += 68;   //skip protocol name
	len = sizeof(ACQ_HEAD);
	memcpy( &m_AcqProtocol.acq_head, pBuf, len);//sizeof(ACQ_HEAD) );
	offset = sizeof(ACQ_HEAD);

	//save frame param
	if( m_AcqProtocol.acq_head.ScanClass != 3){
		pBuf += offset;
		len = sizeof(ACQ_FRAME_PARAM) * m_AcqProtocol.acq_head.FrameNumber;
		if( m_AcqProtocol.acq_head.FrameNumber<1 || 
			m_AcqProtocol.acq_head.FrameNumber>128 ||
			(offset+len/*+68*/) > dwBufLen
		  )
		{
			LeaveCriticalSection(&m_CLockProtocol);
			DbgPrt("Save Protocol Err: frame Param Err\r\n", g_hWndOWS);
			return FALSE;
		}
		m_AcqProtocol.pFrame = (ACQ_FRAME_PARAM*)new BYTE[len];
		memcpy( m_AcqProtocol.pFrame, pBuf, len );
		offset += len;
	}else{
		m_AcqProtocol.acq_head.FrameNumber = 1;
		m_AcqProtocol.pFrame = (ACQ_FRAME_PARAM*)new BYTE[sizeof(ACQ_FRAME_PARAM)];
	}

	//save gate param
	pBuf += len;
	if( m_AcqProtocol.acq_head.ScanClass == 3){
	    //save gate head
		len = sizeof(ACQ_GATE_HEAD);
		if( offset+len/*+68*/ > dwBufLen ){
			delete [] (BYTE*)m_AcqProtocol.pFrame;
			m_AcqProtocol.pFrame = NULL;
			LeaveCriticalSection(&m_CLockProtocol);
			DbgPrt("Save Protocol Err: gate head err\r\n", g_hWndOWS);
			return FALSE;
		}
		memcpy( &m_AcqProtocol.gate_head, pBuf, sizeof(ACQ_GATE_HEAD));
		offset += len;

		//save gate param, if GateClass==TRUE and PhaseOrFixed==TRUE => 采集时分门 并且 门控基于固定时紒E
		if( m_AcqProtocol.gate_head.GateClass &&
			m_AcqProtocol.gate_head.PhaseOrFixed)
		{
			pBuf += len;
			int gateCnt = m_AcqProtocol.gate_head.ForwordRRNumber +
				          m_AcqProtocol.gate_head.BackwordRRNumber;
			if( gateCnt<1 || gateCnt>30 )
			{
				delete [] (BYTE*)m_AcqProtocol.pFrame;
				m_AcqProtocol.pFrame = NULL;
				LeaveCriticalSection(&m_CLockProtocol);
				DbgPrt("Save Protocol Err: too many gate\r\n", g_hWndOWS);
				return FALSE;
			}
			len = sizeof(ACQ_GATE_PARAM)*(gateCnt);
			if( offset+len/*+68*/ > dwBufLen){
				delete [] (BYTE*)m_AcqProtocol.pFrame;
				m_AcqProtocol.pFrame = NULL;
				LeaveCriticalSection(&m_CLockProtocol);
				DbgPrt("Save Protocol Err:gate param err\r\n", g_hWndOWS); 
				return FALSE;
			}
			if( len != 0 ){
				m_AcqProtocol.pGate = (ACQ_GATE_PARAM*)new BYTE[len];
				memcpy(m_AcqProtocol.pGate, pBuf, len);
			}
		}

		if(m_AcqProtocol.gate_head.GateStop ==0){
			//stop by user
			m_AcqProtocol.pFrame[0].FrameClass   = 2;
			m_AcqProtocol.pFrame[0].CountFlag    = FALSE; 
			m_AcqProtocol.pFrame[0].FrameTime    = 0;
			m_AcqProtocol.pFrame[0].FrameCount   = 0;
			m_AcqProtocol.pFrame[0].IntervalTime = 1;
		}else{
			//stop by event
			m_AcqProtocol.pFrame[0].FrameClass   = 0;
			m_AcqProtocol.pFrame[0].CountFlag    = FALSE; 
			m_AcqProtocol.pFrame[0].FrameTime    = 0;
			m_AcqProtocol.pFrame[0].FrameCount   = m_AcqProtocol.gate_head.GateStop;
			m_AcqProtocol.pFrame[0].IntervalTime = 1;
		}
	}

	m_bFrameStart = FALSE;
	m_nCurrFrame = 0;
	m_dwCurrEvtCnt_All = 0;
	m_dwCurrEvtCnt_Frame = 0;
	m_bProtocolValid = TRUE;

	LeaveCriticalSection(&m_CLockProtocol);

	DbgPrt("Save Procotol ok\r\n", g_hWndOWS);

	return TRUE;
}

BOOL COWS::ProtocolRelease()
{
	EnterCriticalSection(&m_CLockProtocol);

	if( !ProtocolIsValid() ){
		LeaveCriticalSection(&m_CLockProtocol);
		return TRUE;
	}

	m_bProtocolValid = FALSE;
	m_bFrameStart = FALSE;
	m_nCurrFrame = 0;
	m_dwCurrEvtCnt_All = 0;
	m_dwCurrEvtCnt_Frame = 0;

	if( m_AcqProtocol.pFrame != NULL )
		delete  [] (BYTE*)m_AcqProtocol.pFrame; 
	if( m_AcqProtocol.acq_head.ScanClass == 3 &&
	    m_AcqProtocol.pGate != NULL
	  )
	{
		delete [] (BYTE*)m_AcqProtocol.pGate;
	}

	memset(&m_AcqProtocol, 0, sizeof(ACQ_PROTOCOL));

	LeaveCriticalSection(&m_CLockProtocol);

	DbgPrt("Release Protocol\r\n", g_hWndOWS);
	return TRUE;
}

const ACQ_PROTOCOL* COWS::ProtocolGet()
{
	EnterCriticalSection(&m_CLockProtocol);

	if( !ProtocolIsValid() ){
		LeaveCriticalSection(&m_CLockProtocol);
		return NULL;
	}

	LeaveCriticalSection(&m_CLockProtocol);

	return &m_AcqProtocol;
}

//EXEC PET_INITFILE
void COWS::ExecPetInit(const BYTE *pBuf, int nBufLen)
{
	char szTemp[512];

	if(nBufLen<sizeof(INITFILE_HEAD)){
		DbgPrt("Invalid PET_INITFILE XXXXXXXXXXXXX\r\n", g_hWndOWS);
		SendAlarm( ERROR_OWS_INIT_UNKNOW );
		return;
	}

	INITFILE_HEAD* pHead = (INITFILE_HEAD*)pBuf;
	WORD initObj = pHead->nInitObj;
	pHead->nInitObj = 0;

	if( stricmp(pHead->cFlag, "PET_INIT") != 0){
		DbgPrt("Invalid PET_INITFILE(1)\r\n", g_hWndOWS);
		SendAlarm( ERROR_OWS_INIT_UNKNOW );
		return;
	}

	INIT_ITEM* pItem = (INIT_ITEM*)( pBuf+sizeof(INITFILE_HEAD) );
	int itemCnt = ( nBufLen-sizeof(INITFILE_HEAD) )/sizeof(INIT_ITEM);
	switch(initObj){
	case INIT_OBJ_ACS:
		 sprintf(szTemp, "OWS==>PET_INITFILE, OBJ=ACS(%4.4x)\r\n",initObj);
		 DbgPrt(szTemp, g_hWndOWS);
		 if( PetInit_Acs( pItem, itemCnt) )
			 SendPetInitOK(INIT_OBJ_ACS);
		 else
			 SendAlarm(ERROR_OWS_INIT_ACS);
		 break;
		 
	case INIT_OBJ_GANTRY:
	case INIT_OBJ_ROD:
	case INIT_OBJ_SEPTA:
	case INIT_OBJ_BED:
	case INIT_OBJ_YE1:
	case INIT_OBJ_YE2:
		 sprintf(szTemp, "OWS==>PET_INITFILE, OBJ=GANTRY(%4.4x)\r\n",initObj);
		 DbgPrt(szTemp, g_hWndOWS);
		 if( PetInit_Gantry( pItem, itemCnt) )
			 SendPetInitOK( INIT_OBJ_GANTRY );
		 else
			 SendAlarm( ERROR_OWS_INIT_GANTRY );
		 break;

	case INIT_OBJ_BUCKET_0:
	case INIT_OBJ_BUCKET_31:
	case INIT_OBJ_BUCKET_ALL:
//		 sprintf(szTemp, "OWS==>PET_INITFILE, OBJ=BUCKET(%4.4x)\r\n",initObj);
//		 DbgPrt(szTemp, g_hWndOWS);
//		 if( PetInit_Bucket( pItem, itemCnt) )
//			 SendPetInitOK( INIT_OBJ_BUCKET_ALL);
//		 else
//			 SendAlarm( ERROR_OWS_INIT_BUCKET);
		 break;

	default:
		 sprintf(szTemp, "OWS==>PET_INITFILE, OBJ=Unknow(%4.4x)\r\n",initObj);
		 DbgPrt(szTemp, g_hWndOWS);
		 SendAlarm( ERROR_OWS_INIT_UNKNOW );
		 break;
	}
}


BOOL COWS::PetInit_Acs(INIT_ITEM *pItem, int nItemCnt)
{
	 const char szKey[12][32] = 
	 {  
		"Coin_Prompt",            
	    "Coin_DelayMin",		  
	    "Coin_DelayMax",
	    "Coin_DatFmt",
	    "Protocol_DbgBucket",
	    "Protocol_EnSIO",
	    "Protocol_RodRate1",
	    "Protocol_RodRate2",
	    "Protocol_RodRate3",
	    "Adc_SampleFreq",
	    "Adc_AmpFactor",
		"Protocol_EnSIOBucket",	 //new
	 };

	 if( nItemCnt != 12 ) return FALSE;

	 int Coin_Prompt = 12, Coin_min = 48, Coin_max=60;
	 int Coin_datFmt = 0;
	 int DbgBucket=0, EnSIO=1, EnSIOBucket=1;
	 int freq=4, factor=2;

	 BOOL bOK = TRUE;
	 for( int i=0; i<nItemCnt; i++){
		 if( stricmp(pItem[i].szKey, "Coin_Prompt")==0 ){
			 Coin_Prompt=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Coin_DelayMin")==0){
			 Coin_min = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Coin_DelayMax")==0){
			 Coin_max = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Coin_DatFmt")==0){
			 Coin_datFmt = pItem[i].dwValue;
			 continue;
		 }

		 if( stricmp(pItem[i].szKey, "Protocol_DbgBucket")==0){
			 DbgBucket = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Protocol_EnSIO")==0){
			 EnSIO = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Protocol_RodRate1")==0){
			 g_nRodRate[0] = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Protocol_RodRate2")==0){
			 g_nRodRate[1] = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Protocol_RodRate3")==0){
			 g_nRodRate[2] = pItem[i].dwValue;
			 continue;
		 }

		 if( stricmp(pItem[i].szKey, "Adc_SampleFreq")==0){
			 freq = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Adc_AmpFactor")==0){
			 factor = pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "Protocol_EnSIOBucket")==0){
			EnSIOBucket = pItem[i].dwValue;
			continue;
		 }

		 //error, unknow keywords
		 bOK = FALSE;
		 break;
	 }
	 if( !bOK ) return FALSE;

	 CWnd* pMain = AfxGetMainWnd();

	 pMain->SetDlgItemInt(IDC_TIME_LJ, Coin_Prompt);
	 pMain->SetDlgItemInt(IDC_TIME_MIN, Coin_min);
	 pMain->SetDlgItemInt(IDC_TIME_MAX, Coin_max);

	 ((CComboBox*)(pMain->GetDlgItem(IDC_OUTFMT)))->SetCurSel( Coin_datFmt );

	 ((CButton*)(pMain->GetDlgItem(IDC_EN_DBGBUCK)))->SetCheck( DbgBucket );
	 ((CButton*)(pMain->GetDlgItem(IDC_EN_SIOEMC)))->SetCheck( EnSIO );
//	 ((CButton*)(pMain->GetDlgItem(IDC_EN_SIOBUCKET)))->SetCheck( EnSIOBucket );

	 g_FH.EcgParamSet( freq, factor);

	 return TRUE;
}


BOOL COWS::PetInit_Gantry(INIT_ITEM *pItem, int nItemCnt)
{
	const char szKey[8][32] =
	{
		"set_hv_value",
		"set_lv_value",
		"tem_limen_value_up",
		"tem_limen_value_down",
		"hv_amp_limen_up",
		"hv_am_limen_down",
		"cool_device_limen_up",
		"cool_device_limen_down"
	};
	if( nItemCnt != 8 ) return FALSE;

	BOOL bHVON=TRUE, bLVON=TRUE;
	int gantray_temp_h=400, gantray_temp_l=100;
	int hv_amp_h=100, hv_amp_l=60;
	int cool_h=300, cool_l=50;

	BOOL bOK = TRUE;
/*	for( int i=0; i<nItemCnt; i++){
		 if( stricmp(pItem[i].szKey, "set_hv_value")==0 ){
			 bHVON=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "set_lv_value")==0 ){
			 bLVON=pItem[i].dwValue;
			 continue;
		 }

		 if( stricmp(pItem[i].szKey, "tem_limen_value_up")==0 ){
			 gantray_temp_h=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "tem_limen_value_down")==0 ){
			 gantray_temp_l=pItem[i].dwValue;
			 continue;
		 }

		 if( stricmp(pItem[i].szKey, "hv_amp_limen_up")==0 ){
			 hv_amp_h=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "hv_amp_limen_down")==0 ){
			 hv_amp_l=pItem[i].dwValue;
			 continue;
		 }

		 if( stricmp(pItem[i].szKey, "cool_device_limen_up")==0 ){
			 cool_h=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "cool_device_limen_down")==0 ){
			 cool_l=pItem[i].dwValue;
			 continue;
		 }

		 //error, unknow keywords
		 bOK = FALSE;
		 break;
	}
*/
	if( !bOK ) return FALSE;

	char szTemp[512];

	BOOL bEnEMC = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_SIOEMC))->GetCheck();   //协议控制机电电路选蟻E
	if(!bEnEMC){
		DbgPrt("PET_INIT GANTRY SKIPPED!\r\n", g_hWndOWS);
		return TRUE;
	}

/*	sprintf( szTemp, "MainPower:%d HV:%d\r\n", bLVON, bHVON);
	DbgPrt(szTemp, g_hWndSIO);


*/
	//床信息初始化
	if(!g_SIO.QueryBedPos()){
		SendAlarm(ERROR_BED_QUERY_POS);
		bOK = FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedStep() ){
		SendAlarm(ERROR_BED_QUERY_STEP);
		bOK = FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedMoveStatus() ){
		SendAlarm(ERROR_BED_QUERY_MOVESTATUS);
		bOK = FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryBedSwitchSet()){
		SendAlarm(ERROR_BED_QUERY_SWITCH);
		bOK = FALSE;
	}
	Sleep(500);

	//ROD信息初始化
	if(!g_SIO.QueryRodStatus()){
		SendAlarm(ERROR_ROD_QUERY_STATUS);
		bOK = FALSE;
	}
	Sleep(500);
/*	if(!g_SIO.QueryRodRate()){
		SendAlarm(ERROR_ROD_QUERY_RATE);
		bOK = FALSE;
	}
	Sleep(500);
	if(!g_SIO.QueryRodSwitchSet()){
		SendAlarm(ERROR_ROD_QUERY_SWITCH);
		bOK = FALSE;
	}
	Sleep(500);


*/
	return bOK;
}
/*
BOOL COWS::PetInit_Bucket(INIT_ITEM *pItem, int nItemCnt)
{
	const char szKey[2][32] =
	{
		"acq_time",
		"acq_mode"
	};
	if( nItemCnt != 2 ) return FALSE;

	int acq_time=2000;
	int acq_mode=3;

	BOOL bOK = TRUE;
	for( int i=0; i<nItemCnt; i++){
		 if( stricmp(pItem[i].szKey, "acq_time")==0 ){
			 acq_time=pItem[i].dwValue;
			 continue;
		 }
		 if( stricmp(pItem[i].szKey, "acq_mode")==0 ){
			 acq_mode=pItem[i].dwValue;
			 continue;
		 }

		 //error, unknow keywords
		 bOK = FALSE;
		 break;
	}
	if( !bOK ) return FALSE;

	BUCKET_DAT_MODE mode;
	switch(acq_mode){
	case 0: mode = mode_energy; break;
	case 1: mode = mode_time;   break;
	case 2: mode = mode_sd;     break;
	case 3: mode = mode_coin0;  break;
	case 4: mode = mode_coin1;  break;
	default: return FALSE;
	}

//	BOOL bEnEMC = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_SIOBUCKET))->GetCheck();   //协议控制机电电路选蟻E
//	if(!bEnEMC){
		DbgPrt("PET_INIT BUCKET SKIPPED!\r\n", g_hWndOWS);
//		return TRUE;
//	}

//	g_BuckSIO.Control_DataAcqStop();
	Sleep(500);

//	g_BuckSIO.Control_DataAcqBegin(mode, acq_time);

	return TRUE;
}
*/
