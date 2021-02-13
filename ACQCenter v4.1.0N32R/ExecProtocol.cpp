#include "StdAfx.h"
#include "math.h"
#include "Resource.h"
#include "OWS.h"
#include "FH.h"
#include "SIO.h"
//#include "BuckSIO.h"
#include "ExecProtocol.h"

#include "DbgPrt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HANDLE g_hEvtSysError;
extern COWS g_OWS;
#ifndef _FH_SIM
	extern CFH  g_FH;
#else
	extern CFHSim g_FH;
#endif
extern CSIO g_SIO;
//extern CBuckSIO g_BuckSIO;
volatile extern MCTL_INFO g_MCtlInfo;

extern int g_nRodRate[3];

extern HWND g_hWndOWS;
extern HWND g_hWndProtocol;

extern BOOL g_bOWSQuitNormal;

void ExecEMCReset();

/////////////////////////////////////////////////////////////////////////
//     visible in this file only
BOOL ExecEMControl(BOOL bRodOut, int rodrate, BOOL bSeptaOut, float fBedPos, float fBedOffset);
BOOL ExecRodControl( BOOL bRodOut, int rodrate);
BOOL ExecSeptaControl(BOOL bSeptaOut);
BOOL ExecBedControl(float fBedPos, float fBedOffset);
//     visible in this file only
//////////////////////////////////////////////////////////////////////////


DWORD WINAPI ThreadExecProtocol(LPVOID pData)
{
	EXEC_THREAD_PARAM* pParam;
	const ACQ_PROTOCOL* pProtocol;

/*
	HANDLE hMonitorThread;
	DWORD dwMonitorThreadID;
	HANDLE hEvtExitMonitor;
*/
	HANDLE hEvtWait[10];
	HANDLE hTimer;
	LARGE_INTEGER time;
	DWORD dwRet;
	BOOL bExit, bErrExit=FALSE, bEMCReset=FALSE;
	int i;

	//coin paramter
	DWORD dwIntCnt;  FH_INT_TYPE intType;
	FH_SCANMODE scanmode = scan_static;
	DWORD dwFHRing;  DWORD dwFHFov;
	DWORD dwFHTimeNs, dwFHTimeMax, dwFHTimeMin;
	int nOutFmt; 	int nEnergyMode;

	#if DBG_LEVEL != 0
		char szTemp[512];
	#endif

	DbgPrt("Begin Exec Protocol!\r\n", g_hWndProtocol);

	pParam = (EXEC_THREAD_PARAM*)(pData);
	
	pProtocol = g_OWS.ProtocolGet();

	hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	//create monitor thread
/*	hEvtExitMonitor = CreateEvent(NULL, FALSE, FALSE, NULL);
	hMonitorThread = CreateThread(NULL,0,ThreadExecProtocolMonitor, 
								  hEvtExitMonitor, 0, 
								  &dwMonitorThreadID);
*/
	DbgPrt("\tBegin Control EMC!\r\n", g_hWndProtocol);
	if( ! ExecEMControl( pProtocol->acq_head.RodStatus, pProtocol->acq_head.RodSpeed,
		                 pProtocol->acq_head.SeptaStatus,
				         pProtocol->acq_head.BedPosition, pProtocol->acq_head.BedOffset
				       )
	  )
	{
		DbgPrt("\tFailed To Control EMC!\r\n", g_hWndProtocol);
		bEMCReset = TRUE;
		bErrExit = TRUE;
		goto exit2;
	}
	DbgPrt("\tEnd Control EMC!\r\n", g_hWndProtocol);

	//Init Gate Param
	if( pProtocol->acq_head.ScanClass == 3){
		//gate scan
		if( !g_FH.GateParamSet(pProtocol) ){
			g_OWS.SendAlarm(ERROR_FH_GATEPARAM);
			bErrExit = TRUE;
			bEMCReset = TRUE;
			goto exit2;
		}
	}

	//Init FH Param
	intType = int_ontime;
	dwIntCnt = 50*1000*1000;  //50m clock count

	switch(pProtocol->acq_head.ScanClass)
	{
	case 0: scanmode = scan_t;       sprintf(szTemp, "B SCAN");  break;  //blank scan
	case 1: scanmode = scan_t;       sprintf(szTemp, "T SCAN");  break;  //t scan
	case 2: scanmode = scan_static;  sprintf(szTemp, "E SCAN");  break;  //e scan
	case 3: scanmode = scan_g;       sprintf(szTemp, "G SCAN");  break;  //g scan
	default:scanmode = scan_static;  sprintf(szTemp, "unknow SCAN");
	}

	//0->光电符合 1->散射符合 2->光电觼E⑸丒3->全选
	nEnergyMode = pProtocol->acq_head.ScatterEnery;

	//dwFHRing = pProtocol->acq_head.Span;
	//dwFHFov  = (DWORD)pProtocol->acq_head.AxialFov;
	dwFHRing = pProtocol->acq_head.Span;		    //coin ring difference
	dwFHFov  = pProtocol->acq_head.RingDifference;  //coin fov

	sprintf(szTemp, "%s  EnergyMode=%d Ring(0~31)=%d  FOV(0~15)=%d\r\n", 
		             szTemp, nEnergyMode, dwFHRing, dwFHFov);
	DbgPrt(szTemp, g_hWndProtocol);

	dwFHTimeNs  = AfxGetMainWnd()->GetDlgItemInt(IDC_TIME_LJ);   //立即符合时紒E
	dwFHTimeMin = AfxGetMainWnd()->GetDlgItemInt(IDC_TIME_MIN);  //臁延迟符合时紒E
	dwFHTimeMax = AfxGetMainWnd()->GetDlgItemInt(IDC_TIME_MAX);  //畜延迟符合时紒E
	

	///////////////////////////////////////////////////////////////////////////////
	//                    Exec data acquire

	BOOL bEnBuckDbg;
	bEnBuckDbg = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_DBGBUCK))->GetCheck();

	//coin data fmt:  0->out position; 1->out time dif
	if( bEnBuckDbg ){
		if( (pProtocol->acq_head.nTotalTime&0x40000000) == 0)
			nOutFmt = 0;
		else 
			nOutFmt = 1;
	}
	else{
		nOutFmt = 0;
	}
	((CComboBox*)AfxGetMainWnd()->GetDlgItem(IDC_OUTFMT))->SetCurSel(nOutFmt);

	g_FH.FHParamSet( dwIntCnt, intType, scanmode,
		             dwFHRing, dwFHFov,
					 dwFHTimeNs, dwFHTimeMin, dwFHTimeMax,
					 nOutFmt, nEnergyMode
				   );

	bExit = FALSE;
	int nFrame;
	nFrame = pProtocol->acq_head.FrameNumber;
	for(i=0; i<nFrame; i++){

		#if DBG_LEVEL !=0
			sprintf(szTemp, "----- Begin Frame %d\r\n", i);
			DbgPrt(szTemp, g_hWndProtocol);
		#endif

		g_OWS.SetCurrFrame(i);
		g_OWS.SetFrameStart(TRUE);
		g_OWS.SendFrameStart(i);

		if(bEnBuckDbg){
			if( (pProtocol->acq_head.nTotalTime & 0x80000000) == 0 )
			{	//exec coin acq
				if( scanmode == scan_t ) g_FH.Rs485Start();
				if( scanmode == scan_g ) g_FH.GateStart();
				g_FH.FHStart(FALSE, pProtocol->acq_head.nTotalTime&0x01f );
				if( nOutFmt == 0 ) DbgPrt("ACQ Coin Data(Pos Fmt)\r\n", g_hWndProtocol);
				else DbgPrt("ACQ Coin Data(Time Fmt)\r\n", g_hWndProtocol);
			}
			else
			{	//exec bucket acq
				if( scanmode == scan_t) g_FH.Rs485Start();

				g_FH.FHStart(TRUE, pProtocol->acq_head.nTotalTime&0x01f );
				sprintf(szTemp, "ACQ BUCKET%d Data\r\n", pProtocol->acq_head.nTotalTime&0x01f);
				DbgPrt(szTemp, g_hWndProtocol);
			}
		}else{
			//disable bucket debug
			if( scanmode == scan_t ) g_FH.Rs485Start();
			if( scanmode == scan_g ) g_FH.GateStart();
			g_FH.FHStart();
			if( nOutFmt == 0 )	DbgPrt("ACQ Coin Data(Pos Fmt)\r\n", g_hWndProtocol);
			else 	            DbgPrt("ACQ Coin Data(Time Fmt)\r\n", g_hWndProtocol);
		}
		
		switch(pProtocol->pFrame[i].FrameClass)
		{
		case 0:   //记数终止帧
			 sprintf(szTemp, "Stop By Event Cnt, cnt=%d\r\n", pProtocol->pFrame[i].FrameCount);
			 DbgPrt(szTemp, g_hWndProtocol);
			 hEvtWait[0] = pParam->hEvtFrameOK;
			 break;
		case 1:   //时间终止帧
			 sprintf( szTemp, "Stop By Time, time=%d\r\n", pProtocol->pFrame[i].FrameTime);
			 DbgPrt(szTemp, g_hWndProtocol);
			 hEvtWait[0] = hTimer;
			 time.QuadPart = -1*LONGLONG(pProtocol->pFrame[i].FrameTime)*1000*1000*10;
			 dwRet = SetWaitableTimer(hTimer, &time, 0, NULL, NULL, FALSE);
			 break;
		case 2:   //用户终止帧
			 DbgPrt("Stop By User\r\n", g_hWndProtocol);
			 hEvtWait[0] = pParam->hEvtFrameSkip;
			 break;
		default:
			 //ASSERT(FALSE);
			 break;
		}						  
	    hEvtWait[1] = pParam->hEvtFrameSkip;       //OWS SKIP THIS FRAME
		hEvtWait[2] = pParam->hEvtProtocolStop;    //OWS STOP THIS PROTOCOL
		hEvtWait[3] = g_hEvtSysError;              //SYSTEM ERROR UP

		bExit = FALSE;
		dwRet = WaitForMultipleObjects(4, hEvtWait, FALSE, INFINITE);
		g_OWS.SetFrameStart(FALSE);
		g_FH.FHStop();
		switch(dwRet)
		{
		case WAIT_OBJECT_0:   //FRAME OK
			 ResetEvent(hEvtWait[0]);
			 #if DBG_LEVEL != 0
				sprintf(szTemp, "----- End Frame %d\r\n", i);
				DbgPrt(szTemp, g_hWndProtocol);
			 #endif
			 break;
		case WAIT_OBJECT_0+1:   //ows skip this frame
			 ResetEvent(hEvtWait[1]);
			 #if DBG_LEVEL != 0
				sprintf(szTemp, "----- Skip Frame %d By OWS\r\n", i);
				DbgPrt(szTemp, g_hWndProtocol);
			 #endif
			 break;
		case WAIT_OBJECT_0+2:  //用户终止协襾E
			 ResetEvent(hEvtWait[2]);
			 bExit = TRUE;
			 bEMCReset = FALSE; //TRUE;
			 #if DBG_LEVEL != 0
				sprintf(szTemp, "----- Stop Protocol By OWS\r\n");
				DbgPrt(szTemp, g_hWndProtocol);
			 #endif
			 break;
		case WAIT_OBJECT_0+3:  //SYSTEM ERROR UP
			 ResetEvent(hEvtWait[3]);
			 bExit = TRUE;
			 bErrExit = TRUE;
			 bEMCReset = TRUE;
			 #if DBG_LEVEL != 0
				sprintf(szTemp, "----- Stop Protocol By System Error\r\n");
				DbgPrt(szTemp, g_hWndProtocol);
			 #endif
			 break;
		default:
			 bExit = TRUE;
			 //ASSERT(FALSE);
			 break;
		}

		if(bExit) break;  //exit thread

		//stop at least 1 seconds
		if(pProtocol->pFrame[i].IntervalTime < 1){
			 time.QuadPart = -1*1*1000*1000*10;
			 sprintf(szTemp, "ACQ Pause 1 Second!\r\n");
		}else{
			 time.QuadPart = -1*LONGLONG(pProtocol->pFrame[i].IntervalTime)*1000*1000*10;
			 sprintf(szTemp, "ACQ Pause %d Second!\r\n", pProtocol->pFrame[i].IntervalTime);
		}
		DbgPrt(szTemp, g_hWndProtocol);
		SetWaitableTimer(hTimer, &time, 0, NULL, NULL, FALSE);

		hEvtWait[0] = hTimer;
		hEvtWait[1] = pParam->hEvtFrameSkip;
		hEvtWait[2] = pParam->hEvtProtocolStop;
		hEvtWait[3] = g_hEvtSysError;
		bExit = FALSE;
		dwRet = WaitForMultipleObjects(4, hEvtWait, FALSE, INFINITE);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:  //timer up
			 ResetEvent( hEvtWait[0] );
			 g_OWS.SendFrameOK();
			 break;
		case WAIT_OBJECT_0+1: //frame skip
			 ResetEvent( hEvtWait[1] );
		     g_OWS.SendFrameOK();
			 break;
		case WAIT_OBJECT_0+2: //ows stop acq
			 ResetEvent( hEvtWait[2] );
			 bExit = TRUE;
			 bEMCReset = FALSE; //TRUE;
			 break;
		case WAIT_OBJECT_0+3: //SYSTEM ERROR
			 ResetEvent( hEvtWait[3] );
			 bErrExit = TRUE;
			 bEMCReset = TRUE;
			 bExit = TRUE;
			 break;
		default:
			 bExit = TRUE;
			 break;
		}

		if(bExit) break;
	}
	//					   Exec data acquire
	///////////////////////////////////////////////////////////////////////////////////////

exit2:
	CloseHandle(hTimer);
	if( scanmode == scan_t ) g_FH.RS485Stop();
	if( scanmode == scan_g ) g_FH.GateStop();
	//g_FH.FHStop();

	if(bEMCReset) ExecEMCReset();

	if(bErrExit)
		g_OWS.SendNACK();
	else
		g_OWS.SendACK();

/*
	SetEvent(hEvtExitMonitor);
	WaitForSingleObject(hMonitorThread, 1000);
	CloseHandle(hEvtExitMonitor);
*/
	g_OWS.ProtocolRelease();

	DbgPrt("End Exec Protocol!\r\n", g_hWndProtocol);

	return 1;
}

DWORD WINAPI ThreadExecProtocolMonitor(LPVOID pData)
{
	DWORD dwRet;
	HANDLE hWait[2];

	hWait[0] = 	(HANDLE)pData;
	hWait[1] =  (HANDLE)g_hEvtSysError;

	///////////////////////////////////////////////////////////////////
//	return 0;
	///////////////////////////////////////////////////////////////////

	do{
		dwRet = WaitForMultipleObjects(2, hWait, FALSE, 1000/*ms*/);
		switch(dwRet){
		case WAIT_TIMEOUT:
			 if( g_bOWSQuitNormal ) return 1;

			 if( !g_OWS.SendMonitor() ){
				 PulseEvent(g_hEvtSysError);
				 return 1;
			 }
			 if( !g_OWS.SendSysInfo() ){
				 PulseEvent(g_hEvtSysError);
				 return 1;
			 }
			 break;

		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0+1:
			 return 1;

		default:
			 DbgPrt("Monitor FH EXEC Thread WaitForSingleObject Error\r\n", g_hWndOWS);
			 break;
		}
	}while(TRUE);

	return 1;
}

BOOL ExecEMControl( BOOL bRodOut, int rodrate, 
				    BOOL bSeptaOut, 
					float fBedPos, float fBedOffset
				  )
{
	BOOL bEMControlEn;
	bEMControlEn = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_SIOEMC))->GetCheck();

	char szTemp[512];
	sprintf(szTemp, "Rod=%d, RodRate=%d, Septa=%d, BedPos=%f, BedOffset=%f\r\n", bRodOut, rodrate, bSeptaOut, fBedPos, fBedOffset);
	DbgPrt(szTemp, g_hWndProtocol);

	if( !bEMControlEn) return TRUE;

	BOOL bRodOK, bSeptaOK, bBedOK;

	if( bRodOut ){
//		bSeptaOK = ExecSeptaControl(bSeptaOut);
		bRodOK  = ExecRodControl(bRodOut, rodrate);
	}else{
		bRodOK  = ExecRodControl(bRodOut, rodrate);
//		bSeptaOK = ExecSeptaControl(bSeptaOut);
	}

	bBedOK =  ExecBedControl( fBedPos, fBedOffset);

	return (bBedOK && bRodOK );	
}

void ExecEMCReset()
{
	BOOL bEMControlEn;
	bEMControlEn = ((CButton*)AfxGetMainWnd()->GetDlgItem(IDC_EN_SIOEMC))->GetCheck();

	if( !bEMControlEn ) return;

	BOOL bRodOut,bSepatOut;
	int RodRate;

	bRodOut = FALSE; RodRate=1;
	ExecRodControl(bRodOut, RodRate);

	bSepatOut = FALSE;
	ExecSeptaControl(bSepatOut);

}


BOOL ExecRodControl( BOOL bRodOut, int rodrate)
{
	int RodRate;
	switch(rodrate){
	case 1: RodRate = g_nRodRate[0];  break;
	case 2: RodRate = g_nRodRate[1]; break;
	default:
	case 3: RodRate = g_nRodRate[2]; break;
	}

	////////////////////////////////////////////////////////////////
	// show tips
	char szTemp[512];
	switch(g_MCtlInfo.Rod_stat){
	case outing:
		 sprintf(szTemp, "\tCurrent Rod Status: OUT In Process, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	case ining:
		 sprintf(szTemp, "\tCurrent Rod Status: IN In Process, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	case rolling:
		 sprintf(szTemp, "\tCurrent Rod Status: Rolling, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	case stop:
		 sprintf(szTemp, "\tCurrent Rod Status: STOP, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	case out:
		 sprintf(szTemp, "\tCurrent Rod Status: OUT, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	case in:
		 sprintf(szTemp, "\tCurrent Rod Status: IN, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	default:
		 sprintf(szTemp, "\tCurrent Rod Status: UNKNOW, Rod Rate=%d\r\n", 
			                g_MCtlInfo.wRod_rate
				);
		 break;
	}
	DbgPrt(szTemp, g_hWndProtocol);

	sprintf(szTemp, "\tNext Rod Status should be %d, RodRate=%d(%d)\r\n",
		              bRodOut, RodRate, rodrate
		   );
	DbgPrt(szTemp, g_hWndProtocol);
	// End Show Tips
	//////////////////////////////////////////////////////////////

	if( bRodOut )
	{   //蓙E丒
		if( RodRate != g_MCtlInfo.wRod_rate ){
			//stop, then set rod rate
			switch(g_MCtlInfo.Rod_stat){
			case outing:
			case ining:
			case rolling:
				 if( !g_SIO.Control_RodStop() ){
					 g_OWS.SendAlarm(ERROR_ROD_CTRL_STOP);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			case stop:
			case out:
			case in:
				 break;
			default:
				 g_OWS.SendAlarm(ERROR_ROD_QUERY_STATUS);
				 return FALSE;
			}
			if( !g_SIO.SetRodRate(RodRate) ){
				g_OWS.SendAlarm(ERROR_ROD_SETRATE); 
				return FALSE;
			}
			Sleep(500);
		}

		switch(g_MCtlInfo.Rod_stat){
			case rolling:
			case outing:
				 break;
			case in:	
			case out:				
				 if( !g_SIO.Control_RodOut() ){
					 g_OWS.SendAlarm(ERROR_ROD_CTRL_OUT);
					 return FALSE;
				 }
				 Sleep(500);
				 break;

			case stop:
				 if( !g_SIO.Control_RodRoll() ){
					 g_OWS.SendAlarm(ERROR_ROD_CTRL_ROLL);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			case ining:
			default:
				 g_OWS.SendAlarm(ERROR_ROD_QUERY_STATUS);
				 return FALSE;
		}
		Sleep(4000);
	}
	else
	{   //缩回
		switch(g_MCtlInfo.Rod_stat){
		case in:
		case ining:
			 break;
		case outing:
		case rolling:
			 if( !g_SIO.Control_RodStop() ){
				 g_OWS.SendAlarm(ERROR_ROD_CTRL_STOP);
				 return FALSE;
			 }
			 Sleep(500);
			 if( !g_SIO.Control_RodIn() ){
				 g_OWS.SendAlarm(ERROR_ROD_CTRL_IN);
				 return FALSE;
			 }
			 Sleep(500);
			 break;
		case out:
		case stop:
			 if( !g_SIO.Control_RodIn() ){
				 g_OWS.SendAlarm(ERROR_ROD_CTRL_IN);
				 return FALSE;
			 }
			 Sleep(500);
			 break;
		default:
			 g_OWS.SendAlarm(ERROR_ROD_QUERY_STATUS);
			 return FALSE;
		}
	}

	return TRUE;
}

BOOL ExecSeptaControl(BOOL bSeptaOut)
{
	////////////////////////////////////////////////////////////////
	// show tips
	char szTemp[512];
	switch( g_MCtlInfo.septa_stat){
	case in:
		 strcpy(szTemp, "\tCurrent SEPTA Status: IN\r\n");
		 break;
	case stop:
		 strcpy(szTemp, "\tCurrent SEPTA Status: STOP\r\n");
		 break;
		 break;
	case out:
		 strcpy(szTemp, "\tCurrent SEPTA Status: OUT\r\n");
		 break;
	case outing:
		 strcpy(szTemp, "\tCurrent SEPTA Status: OUT In Process\r\n");
		 break;
		 break;
	case ining:
		 strcpy(szTemp, "\tCurrent SEPTA Status: IN In Process\r\n");
		 break;
		 break;
	default:
		 strcpy(szTemp, "\tCurrent SEPTA Status: UNKNOW\r\n");
		 break;
	}
	DbgPrt(szTemp, g_hWndProtocol);

	sprintf(szTemp, "\tNext SEPTA Status should be: %d\r\n", bSeptaOut);
	DbgPrt(szTemp, g_hWndProtocol);
	// end show tips
	////////////////////////////////////////////////////////////////
/*
	if( bSeptaOut )
	{	//蓙E丒
		switch( g_MCtlInfo.septa_stat){
			case in:
			case stop:
				 if( !g_SIO.Control_SeptaOut() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_OUT);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			case out:
			case outing:
				 break;
			case ining:
				 if( !g_SIO.Control_SeptaStop() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_STOP);
					 return FALSE;
				 }
				 Sleep(500);
				 if( !g_SIO.Control_SeptaIn() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_IN);
					 return FALSE;
				 }
				 Sleep(500);
				 if( !g_SIO.Control_SeptaOut() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_OUT);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			default:
				 g_OWS.SendAlarm(ERROR_SEPTA_QUERY_STATUS);
				 return FALSE;
		}
	}
	else
	{	//缩回
		switch( g_MCtlInfo.septa_stat ){
			case in:
			case ining:
				 break;
			case stop:
			case out:
				 if( !g_SIO.Control_SeptaIn() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_IN);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			case outing:
				 if( !g_SIO.Control_SeptaStop() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_STOP);
					 return FALSE;
				 }
				 Sleep(500);
				 if( !g_SIO.Control_SeptaIn() ){
					 g_OWS.SendAlarm(ERROR_SEPTA_CTRL_IN);
					 return FALSE;
				 }
				 Sleep(500);
				 break;
			default:
				 g_OWS.SendAlarm(ERROR_SEPTA_QUERY_STATUS);
				 return FALSE;
		}
	}
*/
	return TRUE;
}

BOOL ExecBedControl(float fBedPos, float fBedOffset)
{
	if( !g_SIO.QueryBedMoveStatus() ) {
		g_OWS.SendAlarm(ERROR_BED_QUERY_MOVESTATUS);
		return FALSE;
	}
	Sleep(500);
	if( !g_SIO.QueryBedPos() )	{
		g_OWS.SendAlarm(ERROR_BED_QUERY_POS);
		return FALSE;
	}
    Sleep(500);

	////////////////////////////////////////////////////////////////
	// show tips
	char szTemp[512];
	switch( g_MCtlInfo.Bed_stat){
		case still:
			 sprintf(szTemp, "\tCurrent Bed Status: still, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
			 break;
		case move:
			 sprintf(szTemp, "\tCurrent Bed Status: move, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
			 break;
		case out_stop:
			 sprintf(szTemp, "\tCurrent Bed Status: out limited, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
			 break;
		case in_stop:
			 sprintf(szTemp, "\tCurrent Bed Status: in limited, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
			 break;
		default:
			 sprintf(szTemp, "\tCurrent Bed Status: unknow, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
			 break;
	}
	DbgPrt(szTemp, g_hWndProtocol);

	sprintf(szTemp, "\rNext Bed Pos should Be: %f, Offset=%f\r\n", fBedPos, fBedOffset);
	DbgPrt(szTemp, g_hWndProtocol);
	// end show tips
	/////////////////////////////////////////////////////////////
/*
	if( g_MCtlInfo.wBed_posy < 3150){
		if( !g_SIO.SetBedCoord(FALSE) ){
			g_OWS.SendAlarm(ERROR_BED_SETCOORD);
			return FALSE;
		}
		Sleep(500);
		if( !g_SIO.Control_BedMoveY(3150) ){
			g_OWS.SendAlarm(ERROR_BED_MOVEY);
			return FALSE;
		}
		Sleep(500);
	}
*/
	//床绝对定位
	int decimal,sign;
	WORD pos, offset;
	fBedPos *= 100.; //cm->0.1mm
	pos = atoi( _fcvt(fBedPos, 0, &decimal, &sign));
	if( (sign==0) && (pos!=g_MCtlInfo.wBed_posx) ){
		if( !g_SIO.SetBedCoord( FALSE ) ){
			g_OWS.SendAlarm(ERROR_BED_SETCOORD);
			return FALSE;
		}
		Sleep(500);

		if( !g_SIO.Control_BedMoveX( pos ) ){
			g_OWS.SendAlarm(ERROR_BED_MOVEX);
			return FALSE;
		}				    
		Sleep(500);
	}

	//床相对定位
	BOOL bOut;
	fBedOffset = fBedOffset*100.; //cm->0.1mm
	offset = atoi( _fcvt(fBedOffset, 0, &decimal, &sign));
	if( offset != 0 ){
		if( !g_SIO.SetBedCoord( TRUE ) ){
			g_OWS.SendAlarm(ERROR_BED_SETCOORD);
			return FALSE;
		}
		Sleep(500);

		if( sign == 0 ) bOut = TRUE;
		else bOut = FALSE;

		if( !g_SIO.Control_BedMoveXRelate( offset, bOut ) ){
			g_OWS.SendAlarm(ERROR_BED_MOVEX);
			return FALSE;
		}
		Sleep(500);
	}

	return TRUE;
}



