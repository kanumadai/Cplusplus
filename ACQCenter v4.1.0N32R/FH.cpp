#include "StdAfx.h"

#ifdef _ACQCENTER
#include "..\AcqCenter\ACQCenter.h"
#include "..\AcqCenter\ACQCenterDlg.h"
#include "..\AcqCenter\OWS.h"
#endif

#ifdef _TESTFH
#include "..\TestFH\TestFH.h"
#include "..\TestFH\FHDlg.h"
#include "..\TestFH\FHOWS.h"	
#endif

#include "math.h"

#include "FH.h"

#include "DbgPrt.h"

#include <winioctl.h>
#include "..\sys.sg\ioctls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _FH_SIM

extern COWS g_OWS;
extern HWND g_hWndFH;

////////////////////////////////////////////////////////////////////
//              visiable in global 
#define BUF_FH_CNT 4   
#define BUF_FH_LEN (32*1024*1024)
HANDLE  g_hBufFH[ BUF_FH_CNT ];

#define BUF_ECG_CNT 4
#define BUF_ECG_LEN (16*1024)
HANDLE  g_hBufEcg[ BUF_ECG_CNT ];
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//              visiable in this file only

DWORD WINAPI ThreadMonitorFH(LPVOID  lpData);

//
////////////////////////////////////////////////////////////////////


CFH::CFH()
{
	m_hEvtExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_olIOCtl.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	for(int i=0; i<BUF_FH_CNT; i++){
		g_hBufFH[i] = GlobalAlloc(GMEM_FIXED, BUF_FH_LEN);
		//ASSERT(g_hBufFH[i] != NULL);
	}

	for(i=0; i<BUF_ECG_CNT; i++){
		g_hBufEcg[i] = GlobalAlloc(GMEM_FIXED, BUF_ECG_LEN);
		//ASSERT(g_hBufEcg[i] != NULL);
	}

	m_bFHStarted = FALSE;   
	m_bEcgStarted = FALSE;  
	m_bRs485Started = FALSE;
	m_bGateStarted = FALSE;  
	m_hDevice = INVALID_HANDLE_VALUE;

	m_nAdcSampleFreq = 0x04;  //adc sample freq: 1600
	m_nAdcAmpFactor = 3;      //0~7
}

CFH::~CFH()
{
	CloseHandle( m_hEvtExit);
	CloseHandle(m_olIOCtl.hEvent);

	for(int i=0; i<BUF_FH_CNT; i++) GlobalFree( g_hBufFH[i] );
	for(i=0; i<BUF_ECG_CNT; i++) GlobalFree( g_hBufEcg[i] );
}

BOOL CFH::OpenDevice()
{
	if(m_hDevice != INVALID_HANDLE_VALUE) return FALSE;

	m_hDevice = CreateFile( "\\\\.\\t9054", 
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE, 
		                    NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
							NULL
						  );
	if(m_hDevice == INVALID_HANDLE_VALUE){
		DbgPrt("Open Plx9054 Device Error!\r\n", g_hWndFH);
		return FALSE;
	}

	m_ThreadParam.hDevice  = m_hDevice;
	m_ThreadParam.hEvtExit = m_hEvtExit;
	m_ThreadParam.pFH  = this;

	DWORD dwThreadID;

	m_hThread = CreateThread(NULL, 0, ThreadMonitorFH, 
							 &m_ThreadParam, NULL,&dwThreadID);
	if(m_hThread == NULL){
		DbgPrt("Create Coin Monitor Thread Error!\r\n", g_hWndFH);
		CloseHandle(m_hDevice);
		return FALSE;
	}

	return TRUE;
}

BOOL CFH::CloseDevice()
{
	SetEvent(m_hEvtExit);
	WaitForSingleObject(m_hThread, 2000);
	CancelIo(m_hDevice);

	CloseHandle(m_hDevice);
	m_hThread = NULL;
	m_hDevice = INVALID_HANDLE_VALUE;

	return TRUE;
}

DWORD WINAPI ThreadMonitorFH(LPVOID  lpData)
{
	HANDLE hEvtWait[BUF_FH_CNT+BUF_ECG_CNT+1];

	OVERLAPPED ol_fh[BUF_FH_CNT];
	DWORD readed_fh[BUF_FH_CNT];
	
	OVERLAPPED ol_ecg[BUF_ECG_CNT];
	DWORD readed_ecg[BUF_ECG_CNT];

    char szTemp[512];

	DbgPrt("Ready To Receive Coin Data\r\n", g_hWndFH);

	FH_THREAD_PARAM* pParam;

	pParam = (FH_THREAD_PARAM*)lpData;

	for(int i=0; i < (BUF_FH_CNT+BUF_ECG_CNT); i++){
	   hEvtWait[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	hEvtWait[i] = pParam->hEvtExit;

	for(i=0; i<BUF_FH_CNT; i++){
	   ol_fh[i].Offset = 0;  ol_fh[i].OffsetHigh = 0;
	   ol_fh[i].hEvent = hEvtWait[i];
	   ReadFile(pParam->hDevice, g_hBufFH[i], 
		        BUF_FH_LEN, &readed_fh[i],
				&ol_fh[i]);
	}

	for(i=0; i<BUF_ECG_CNT; i++){
	   ol_ecg[i].Offset = 0;  ol_ecg[i].OffsetHigh = 0;
	   ol_ecg[i].hEvent = hEvtWait[i+BUF_FH_CNT];
	   DeviceIoControl(pParam->hDevice, IOCTL_READADCDATA,
			           g_hBufEcg[i], BUF_ECG_LEN, //in buf
					   g_hBufEcg[i], BUF_ECG_LEN, //out buf
					   &readed_ecg[i],
					   &ol_ecg[i]
					   );
	}

	BOOL bContinue = TRUE;
	DWORD dwWait;
	DWORD dwBufOrder;
	do{
	   dwWait = WaitForMultipleObjects(BUF_FH_CNT+BUF_ECG_CNT+1, hEvtWait, FALSE, INFINITE);
	   if( 
		   dwWait >= WAIT_OBJECT_0 && 
		   dwWait < (WAIT_OBJECT_0+BUF_FH_CNT)
		 )
	   {   //-----------------------  Process Receive FH Data ------------------------------------

		   dwBufOrder = dwWait - WAIT_OBJECT_0;
		   //ASSERT(dwBufOrder>=0 && dwBufOrder<BUF_FH_CNT);
		   
		   GetOverlappedResult(pParam->hDevice, &ol_fh[dwBufOrder], &readed_fh[dwBufOrder], TRUE);
		   #if DBG_LEVEL != 0
			   sprintf(szTemp, "Recv Coin Dat, Buf=%d, Len=%8.8x\r\n", dwBufOrder, readed_fh[dwBufOrder]);
		       DbgPrt(szTemp, g_hWndFH);
		   #endif

		   //send fh data in g_hBufFH[dwBufOrder], len=readed_fh[dwBufOrder]
		   g_OWS.SendFHDat((BYTE*)g_hBufFH[dwBufOrder], readed_fh[dwBufOrder], dwBufOrder );
		   //memset(g_hBufFH[dwBufOrder], 0x01, readed_fh[dwBufOrder]);
		   //end send

		   //ready for next transfer
		   ol_fh[dwBufOrder].Offset = 0; ol_fh[dwBufOrder].OffsetHigh = 0;
		   ResetEvent(ol_fh[dwBufOrder].hEvent);
		   ReadFile(pParam->hDevice, g_hBufFH[dwBufOrder], 
			        BUF_FH_LEN, &readed_fh[dwBufOrder],
					&ol_fh[dwBufOrder]
				   );		   

		   //End Process Receive FH Data
	   }else if( 
		   dwWait >= (WAIT_OBJECT_0+BUF_FH_CNT) &&
		   dwWait <  (WAIT_OBJECT_0+BUF_FH_CNT+BUF_ECG_CNT)
		   	   )
	   {   //-----------------  Process Receive ECG Data ----------------------------------------

		   dwBufOrder = dwWait - WAIT_OBJECT_0 - BUF_FH_CNT;
		   //ASSERT(dwBufOrder>=0 && dwBufOrder<BUF_ECG_CNT);
		   
		   GetOverlappedResult(pParam->hDevice, &ol_ecg[dwBufOrder], &readed_ecg[dwBufOrder], TRUE);
		   #if DBG_LEVEL != 0
			   sprintf(szTemp, "---------Recv ECG Dat, Buf=%d, Len=%8.8x\r\n", dwBufOrder, readed_ecg[dwBufOrder]);
		       DbgPrt(szTemp, g_hWndFH);
		   #endif

		   //send ecg data in g_hBufEcg[dwBufOrder], len=readed_ecg[dwBufOrder]
		   #ifdef _TESTFH
		   ((CFHDlg*)(AfxGetMainWnd()))->DrawADC((BYTE*)g_hBufEcg[dwBufOrder], readed_ecg[dwBufOrder]);
		   #else
		   g_OWS.SendEcgDat((BYTE*)g_hBufEcg[dwBufOrder], readed_ecg[dwBufOrder]);
		   #endif

		   //end send
		   

		   //ready for next transfer
		   ol_ecg[dwBufOrder].Offset = 0; ol_ecg[dwBufOrder].OffsetHigh = 0;
		   ResetEvent(ol_ecg[dwBufOrder].hEvent);
		   DeviceIoControl(pParam->hDevice, IOCTL_READADCDATA,
				           g_hBufEcg[dwBufOrder], BUF_ECG_LEN, //in buf
						   g_hBufEcg[dwBufOrder], BUF_ECG_LEN, //out buf
						   &readed_ecg[dwBufOrder],
						   &ol_ecg[dwBufOrder]
						  );

		   //End Process Receive ECG Data
	   }else if(
		   dwWait == (WAIT_OBJECT_0+BUF_FH_CNT+BUF_ECG_CNT)
		       )
	   {
		   //Proecess Cancel Event
		   bContinue = FALSE;
		   //CancelIo(pParam->hDevice);
		   DeviceIoControl(pParam->hDevice, IOCTL_CANCEL_CURRENTIRP,
				           NULL, 0, //in buf
						   NULL, 0, //out buf
						   &readed_ecg[0],
						   &ol_ecg[0]
						  );

		   //End Process Cancel Event
	   }else 
	   {
	   
	   }
	}while( bContinue == TRUE );

	for(i=0; i<(BUF_FH_CNT+BUF_ECG_CNT); i++)
		CloseHandle( hEvtWait[i] );

	return TRUE;
}

BOOL CFH::EcgParamSet(int SampleFreq, int factor)
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::EcgParamSet\r\n", g_hWndFH);

	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;
	if( factor <0 || factor >7) return FALSE;

	m_nAdcSampleFreq = SampleFreq;
	m_nAdcAmpFactor = factor;

	//…Ë÷√≤…—˘∆µ¬ 
	// space0 offset: 0x20300
	//      D31~D05 :: Reserved
	//      D04~D01 :: sample rate , 
	//                 0000 => 12200,  0001 => 200,  0010 => 400
	//                 0011 => 800,    0100 => 1600, 0101 => 3200
	//                 0110 => 6400,   others => 1600
	//                 
	//      D0      :: start stop control
	space0.offset = 0x20300;
	if( m_bEcgStarted )
		space0.value = (m_nAdcSampleFreq<<1) | 0x01;	   // start
	else
		space0.value = (m_nAdcSampleFreq<<1) | 0x0;	   // stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//…Ë÷√«∞∂À–≈∫≈∑≈¥Û±∂ ˝
	//space0 offset: 0x20380
	//       D31~D3 :: Reserved
	//        D2~D0 :: Factor
	space0.offset = 0x20380;
	space0.value = m_nAdcAmpFactor;	
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	return TRUE;
}

BOOL CFH::EcgStart(int BufSize)
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::EcgStart\r\n", g_hWndFH);

	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//…Ë÷√–ƒµÁ ˝æ›ª∫≥Â«¯≥§∂»
	//mailbox 2 :: 
	//   D31~D15 => Reserved
	//   D14~D04 => –ƒµÁ ˝æ›ª∫¥Ê≥§∂»£®◊÷Ω⁄£©
	//   D03~D02 => 00: Scan On Time; 01:Scan On Count; 10: Gated; 11:Reserve
	//   D01~D00 => 00: Stactic Scan; 01:T Scan         10: G Scan;  
	port.offset = 0x48;	   // mailbox 2
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.value &= 0x0f;	 //keep mailbox2 D3~D0 from change
	port.value = port.value | (BufSize << 4);
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//…Ë÷√≤…—˘∆µ¬ 
	// space0 offset: 0x20300
	//      D31~D05 :: Reserved
	//      D04~D01 :: sample rate , 
	//                 0000 => 12200,  0001 => 200,  0010 => 400
	//                 0011 => 800,    0100 => 1600, 0101 => 3200
	//                 0110 => 6400,   others => 1600
	//                 
	//      D0      :: start stop control
	space0.offset = 0x20300;
	space0.value = (m_nAdcSampleFreq<<1) | 0x01;	   //sample rate 1600, start
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//–ƒµÁ ˝æ›¥´ ‰∆Ù∂Øøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.offset = 0x40;	   // mailbox 0
	port.value = port.value | 0x2;        //start
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bEcgStarted = TRUE;
	return TRUE;
}

BOOL CFH::EcgStop()
{
	RW_SPACEX_LONG space0;
	RW_PORT_LONG port;
	DWORD dwRet;

	DbgPrt("CFH::EcgStop\r\n", g_hWndFH);

	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	space0.offset = 0x20300;
	space0.value = (m_nAdcSampleFreq<<1); //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//–ƒµÁ ˝æ›¥´ ‰Õ£÷πøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);


	port.offset = 0x40;	   // mailbox 0
	port.value = port.value & (~0x2);  //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bEcgStarted = FALSE;

	return TRUE;
}

BOOL CFH::GateParamSet(const void *pParam, int nParamLen)
{
	if(nParamLen != 256) return FALSE;
	if(pParam == NULL) return FALSE;
	if( m_bGateStarted ) return FALSE;
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	LONG* pLong;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	pLong = (LONG*)pParam;

	space0.offset = 0x20000;
	for(int i=0; i<64; i++){
		space0.value = pLong[i];

		m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
		DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
						&space0, sizeof(space0),  //in buf
						&space0, sizeof(space0),  //out buf
						&dwRet,
						&m_olIOCtl
						);			            
		WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
		ResetEvent(m_olIOCtl.hEvent);

		space0.offset += 4;
	}

	return TRUE;
}

BOOL CFH::GateParamSet(const ACQ_PROTOCOL *pProtocol)
{
	double fCircleUTH, fCircleLTH;  //–ƒÃ¯÷‹∆⁄…œœ¬œﬁ
	double fCircleRef, fCircleTH;
	DWORD  dwCircleRef, dwCircleTH;
	double fFSTime, fFETime;
	double fFrameLen, fTimeBase;
	double fPrevEndTime;
	int i, nGate;

	DbgPrt("CFH::GateParamSet\r\n", g_hWndFH);

	if( m_bGateStarted ) return FALSE;
	if( pProtocol == NULL ) return FALSE;

	DWORD dwGateParam[64];

	//–ƒ¬ (–ƒÃ¯√ø∑÷)->–ƒÃ¯÷‹∆⁄(us√ø–ƒÃÅE
	fCircleUTH = 1000000.*1./(pProtocol->gate_head.LPB/60.); 
	fCircleLTH = 1000000.*1./(pProtocol->gate_head.UPB/60.);

	//≤Œøº÷‹∆⁄->»°–ƒÃ¯÷‹∆⁄…œœ¬œﬁµƒ∆Ωæ˘÷µ
	fCircleRef = (fCircleUTH + fCircleLTH)/2.0;
	TimeToCount(fCircleRef, &dwCircleRef);

	//÷‹∆⁄‘ –˙ÍÛ≤ÅE
	fCircleTH  = fabs(fCircleRef - fCircleUTH);
	TimeToCount(fCircleTH, &dwCircleTH);

	for(i=0; i<64; i++) dwGateParam[i] = 0xffffffff;
	if( pProtocol->gate_head.GateClass == FALSE ) 
	{   //≤…ºØ∫Û∑÷√≈
		dwGateParam[61] = 2;            //mode 2
		dwGateParam[62] = dwCircleTH;   //≤ŒøºŒÛ≤ÅE
		dwGateParam[63] = dwCircleRef;  //≤Œøº÷‹∆⁄
	}
	else if( pProtocol->gate_head.GateClass == TRUE &&
		     pProtocol->gate_head.PhaseOrFixed == TRUE
		   )
	{	//≤…ºØ ±ª˘”⁄πÃ∂® ±º‰∑÷√≈
		
		//1. process rr backword
		nGate = pProtocol->gate_head.ForwordRRNumber;
		fFSTime = 20.0;   //20us
		fPrevEndTime = 0.0;
		for(i=0; i<pProtocol->gate_head.BackwordRRNumber; i++)
		{
		   	fFSTime =   pProtocol->pGate[nGate+i].GateBeginTime 
				      - pProtocol->pGate[nGate].GateBeginTime;  //frame start time
			fFETime =   pProtocol->pGate[nGate+i].GateEndTime
				      - pProtocol->pGate[nGate].GateBeginTime;  //frame end time
			fFSTime *= 1000.;   //ms -> us
			fFETime *= 1000.;   //ms->us
			while( (fFSTime - fPrevEndTime) <20.0 )
			{
				fFSTime += 20.0;
				fFETime += 20.0;
			}

			TimeToCount( fFSTime, &dwGateParam[2*i] );    //frame begin count
			TimeToCount( fFETime, &dwGateParam[2*i+1] );  //frame end count

			fPrevEndTime = fFETime;
		}

		//2. process rr forward
		nGate = pProtocol->gate_head.BackwordRRNumber;
		fFrameLen =   pProtocol->pGate[nGate-1].GateEndTime 
			        - pProtocol->pGate[0].GateBeginTime;
		fFrameLen *= 1000.; //ms -> us
		fTimeBase = fCircleRef - fFrameLen;
		for(i=0; i<pProtocol->gate_head.ForwordRRNumber; i++)
		{
		   fFSTime =   fTimeBase 
			         + pProtocol->pGate[i].GateBeginTime*1000.;
		   fFETime =   fTimeBase
					 + pProtocol->pGate[i].GateEndTime*1000.;

		   while( (fFSTime - fPrevEndTime) < 20. )
		   {
			  fFSTime += 20.0;
			  fFETime += 20.0;
		   }

		   TimeToCount( fFSTime, &dwGateParam[2*(i+nGate)] );
		   TimeToCount( fFETime, &dwGateParam[2*(i+nGate)+1] );

		   fPrevEndTime = fFETime;
		}

		//3. set workmode. ref circle. circle th
		dwGateParam[61] = 0;            //mode 0
		dwGateParam[62] = dwCircleTH;   //≤ŒøºŒÛ≤ÅE
		dwGateParam[63] = dwCircleRef;  //≤Œøº÷‹∆⁄
	}
	else
	{	//≤…ºØ ±ª˘”⁄R≤®∑÷√≈

		// 1. process frame after r-wave
		fFrameLen = fCircleRef * (pProtocol->gate_head.BackwordRRPer/100.);
		fFSTime = 20.;
		for(i=0; i<pProtocol->gate_head.BackwordRRNumber; i++)
		{
			fFETime = fFSTime + fFrameLen;

			TimeToCount(fFSTime, &dwGateParam[i*2]);
			TimeToCount(fFETime, &dwGateParam[i*2+1]);
			
			fFSTime = fFETime + 20.0;
		}

		//2. process frame before r-wave
		nGate = pProtocol->gate_head.BackwordRRNumber;
		fFrameLen = fCircleRef * (pProtocol->gate_head.ForwordRRPer / 100.);
		fFSTime = fCircleRef - (fFrameLen+20.)*pProtocol->gate_head.ForwordRRNumber;
		for(i=0; i<pProtocol->gate_head.ForwordRRNumber; i++)
		{
			fFETime = fFSTime + fFrameLen;

			TimeToCount( fFSTime, &dwGateParam[(i+nGate)*2] );
			TimeToCount( fFETime, &dwGateParam[(i+nGate)*2+1] );

			fFSTime = fFETime + 20.0;
		}

		//3. set workmode. ref circle. circle th
		dwGateParam[61] = 0;            //mode 0
		dwGateParam[62] = dwCircleTH;   //≤ŒøºŒÛ≤ÅE
		dwGateParam[63] = dwCircleRef;  //≤Œøº÷‹∆⁄
	}

	return GateParamSet(&dwGateParam, 64*4);
}

BOOL CFH::GateStart()
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::GateStart\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//gate start
	space0.offset = 0x20100;
	space0.value  = 1;	//start

	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bGateStarted = TRUE;

	return TRUE;
}

BOOL CFH::GateStop()
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::GateStop\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	space0.offset = 0x20100;
	space0.value  = 0;	//stop

	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bGateStarted = FALSE;

	return TRUE;
}

BOOL CFH::Rs485Start()
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::Rs485Start\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//D2D1: baud rate select:00=>56000; 01=>9600; 
	//                       10=>57600; 11=>115200;   
    //D0:   1=>START, 0=>STOP
	space0.offset = 0x20200;
	space0.value  = 7;	

	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bRs485Started = TRUE;

	return TRUE;
}

BOOL CFH::RS485Stop()
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::RS485Stop\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	space0.offset = 0x20200;
	space0.value  = 0;	//D2D1: baud rate select; 00=>56000; D1:1=>START, 0=>STOP

	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bRs485Started = FALSE;

	return TRUE;
}

BOOL CFH::FHParamSet( DWORD dwIntValue, FH_INT_TYPE intType, 
					  FH_SCANMODE scanmode, 
					  DWORD dwFHRing, DWORD dwFHFov,
					  DWORD dwFHTimeNs, DWORD dwMinTimeNs, DWORD dwMaxTimeNs,
					  int nOutFmt, int nEneryMode
					 )
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;
	int ret;

	DbgPrt("CFH::FHParamSet\r\n", g_hWndFH);

	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;
	if( m_bFHStarted ) return FALSE;

	//mailbox 2 :: 
	//   D31~D15 => Reserved
	//   D14~D04 => –ƒµÁ ˝æ›ª∫¥Ê≥§∂»£®◊÷Ω⁄£©
	//   D03~D02 => 00: Scan On Time; 01:Scan On Count; 10: Gated; 11:Reserve
	//   D01~D00 => 00: Stactic Scan; 01:T Scan         10: G Scan;  
	port.offset = 0x48;	   // mailbox 2
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	ret = DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);	
	ret = GetLastError();
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//…Ë÷√÷–∂œ∑Ω Ω, MAILBOX2 D3~D2
	port.value &= 0xfffffff0;
	switch(intType){
	case int_ontime:
		 port.value |= (0<<2);
		 break;
	case int_oncount:
		 port.value |= (1<<2);
		 break;
	case int_ongate:
		 port.value |= (2<<2);
		 break;
	default:
		return FALSE;
	}
	//…Ë÷√…®√Ë∑Ω Ω£¨MAILBOX2 D1~D0
	switch(scanmode){
	case scan_static:
		 port.value |= 0;
		 break;
	case scan_t:
		 port.value |= 1;
		 break;
	case scan_g:
		 port.value |= 2;
		 break;
	default:
		 return FALSE;
	}

	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
	
	//mailbox 1:: ∑˚∫œ÷–∂œº« ˝
	port.offset = 0x44;
	port.value  = dwIntValue;
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	typedef union _fh_param1
	{
	   DWORD dwValue;
	   struct{
		   DWORD maxTime : 7;
		   DWORD minTime : 7;
		   DWORD Time : 7;
		   DWORD Ring : 5;
		   DWORD OutFmt : 1;
		   DWORD EnergyMode : 2;
		   DWORD reseved : 3;
	   };
	}FH_PARAM1;

	FH_PARAM1 para1;
	para1.dwValue = 0;
	para1.maxTime = dwMaxTimeNs/2;
	para1.minTime = dwMinTimeNs/2;
	para1.Time = dwFHTimeNs/2;
	para1.Ring = dwFHRing;
	para1.OutFmt = nOutFmt;
	para1.EnergyMode = nEneryMode;


	//∑˚∫œª∑ ˝°¢ ±º‰°¢ƒ‹¡øƒ£ Ω°¢ ‰≥ˆƒ£ Ω
	space0.offset = 0x10000;
	space0.value  = para1.dwValue;
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ ”“∞
	const int _fov_mask[16] =
	{	            // 									 x
		            //	  D14 D13 D12    D11 D10 D9 D8    D7 D6 D5 D4    D3 D2 D1 D0
	  0x7f7f,       //0:   1   1   1      1   1   1  1     0  1  1  1    1 1  1  1
	  0x7f7f,       //1:   1   1   1      1   1   1  1     0  1  1  1    1 1  1  1
	  0x7e3f,       //2:   1   1   1      1   1   1  0     0  0  1  1    1 1  1  1
	  0x7e3f,       //3:   1   1   1      1   1   1  0     0  0  1  1    1 1  1  1
	  0x7c1f,       //4:   1   1   1      1   1   0  0     0  0  0  1    1 1  1  1
	  0x7c1f,       //5:   1   1   1      1   1   0  0     0  0  0  1    1 1  1  1
	  0x780f,       //6:   1   1   1      1   0   0  0     0  0  0  0    1 1  1  1
	  0x780f,       //7:   1   1   1      1   0   0  0     0  0  0  0    1 1  1  1
	  0x7007,       //8:   1   1   1      0   0   0  0     0  0  0  0    0 1  1  1
	  0x7007,       //9:   1   1   1      0   0   0  0     0  0  0  0    0 1  1  1
	  0x6003,       //10:  1   1   0      0   0   0  0     0  0  0  0    0 0  1  1
	  0x6003,       //11:  1   1   0      0   0   0  0     0  0  0  0    0 0  1  1
	  0x4001,       //12:  1   0   0      0   0   0  0     0  0  0  0    0 0  0  1
	  0x4001,       //13:  1   0   0      0   0   0  0     0  0  0  0    0 0  0  1
	  0x0000,       //14:  0   0   0      0   0   0  0     0  0  0  0    0 0  0  0
	  0x0000        //15:  0   0   0      0   0   0  0     0  0  0  0    0 0  0  0
	};
	//ASSERT(dwFHFov>=0 && dwFHFov <=15);

	space0.offset = 0x14000;
	space0.value  = _fov_mask[dwFHFov];
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
	
	return TRUE;
}

BOOL CFH::FHStart(BOOL bTestMode, int TestBucket)
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::FHStart\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set fh start flag in drivers
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_FHSTART, 
					NULL, 0,  //in buf
					NULL, 0,  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ ˝æ›¥´ ‰∆Ù∂Øøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.offset = 0x40;	   // mailbox 0
	port.value |= 0x1;        //start
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ¥¶¿˙‚Ù∂Øøÿ÷∆
	space0.offset = 0x18000;
	space0.value  = 0;
	if(bTestMode)
		space0.value = (TestBucket<<2) | (0<<1) | 1; //start bucket test
	else
		space0.value = (TestBucket<<2) | (1<<1) | 0; //space0.value = (1<<1) | 0;  //start coin
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bFHStarted = TRUE;

	return TRUE;
}

BOOL CFH::FHStop()
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::FHStop\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set fh stop flag in drivers
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_FHSTOP, 
					NULL, 0,  //in buf
					NULL, 0,  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
	
	//∑˚∫œ¥¶¿˙È£÷πøÿ÷∆
	space0.offset = 0x18000;
	space0.value  = 3;       //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ ˝æ›¥´ ‰Õ£÷πøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.offset = 0x40;	   // mailbox 0
	port.value &= (~0x1);        //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
/*
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_RESET, 
					NULL, 0,  //in buf
					NULL, 0,  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
*/	
	m_bFHStarted = FALSE;
	return TRUE;
}

BOOL CFH::SelfTestStart(int nSpeed)
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::SelfTestStart\r\n", g_hWndFH);
	if( nSpeed<0 || nSpeed>3 ){
		DbgPrt( "Invalid Speed Paramter(0~3)!\r\n", g_hWndFH);
		return FALSE;
	}
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set fh start flag in drivers
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_FHSTART, 
					NULL, 0,  //in buf
					NULL, 0,  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ ˝æ›¥´ ‰∆Ù∂Øøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.offset = 0x40;	   // mailbox 0
	port.value |= 0x1;        //start
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ¥¶¿˙Û‘ºÅEÙ∂Øøÿ÷∆
	space0.offset = 0x1C000;
	space0.value  = 0;
	//D31~D4 : RESERVE 
	//D3    : VALID/ENERGY CNT START/STOP CONTROL( 0->START, 1->STOP)
	//D2~D1 : SPEED
	//        00 -> 4MEVENT/S 
	//        01 -> 2MEVENT/S
	//        10 -> 5MEVENT/S
	//        11 -> 8MEVENT/S
	//D0    : START/STOP CONTROL ( 0->START, 1->STOP)
	space0.value = ( ( ( (nSpeed<<1) <<1 ) & 0xfffffffe) | 0xffffff80 ); //
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bFHStarted = TRUE;

	return TRUE;

}

BOOL CFH::SelfTestStop()
{
	RW_PORT_LONG port;
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::SelfTestStop\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set fh stop flag in drivers
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_FHSTOP, 
					NULL, 0,  //in buf
					NULL, 0,  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);
	
	//∑˚∫œ¥¶¿˙Û‘ºÅE£÷πøÿ÷∆
	space0.offset = 0x1C000;
	space0.value  = 0xffffffff;       //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//∑˚∫œ ˝æ›¥´ ‰Õ£÷πøÿ÷∆
	port.offset = 0x40;	   // mailbox 0
	port.value = 0x0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_READPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	port.offset = 0x40;	   // mailbox 0
	port.value &= (~0x1);        //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITEPORT_LONG, 
					&port, sizeof(port),  //in buf
					&port, sizeof(port),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	m_bFHStarted = FALSE;
	return TRUE;
}


BOOL CFH::VECntStart(int nBuckID)
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::VECntStart\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set bucket id
	space0.offset = 0x18000;
	space0.value  = 0;
	space0.value = (nBuckID<<2) | (1<<1) | 1; 
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//ƒ‹¡øŒª°¢”––ßŒªÕ≥º∆ø™ º
	space0.offset = 0x1C000;
	//D31~D4 : RESERVE   
	//D3    : ENERGY\VALID BIT CNT START/STOP CONTROL( 0->START, 1->STOP ) 
	//D2~D1 : SELF_TEST SPEED
	//        00 -> 4MEVENT/S 
	//        01	-> 2MEVENT/S
	//        10 -> 5MEVENT/S
	//        11 -> 8MEVENT/S
	//D0    : SELF_TEST START/STOP CONTROL ( 0->START, 1->STOP)
	space0.value  = ( 0xffffffff & (~0x8) );       //start
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	return TRUE;
}

BOOL CFH::VECntStop()
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::VECntStop\r\n", g_hWndFH);
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	space0.offset = 0x14000;
	space0.value  = 0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	//ƒ‹¡øŒª°¢”––ßŒªÕ≥º∆ø™ º
	space0.offset = 0x1C000;
	//D31~D4 : RESERVE   
	//D3    : ENERGY\VALID BIT CNT START/STOP CONTROL( 0->START, 1->STOP ) 
	//D2~D1 : SELF_TEST SPEED
	//        00 -> 4MEVENT/S 
	//        01	-> 2MEVENT/S
	//        10 -> 5MEVENT/S
	//        11 -> 8MEVENT/S
	//D0    : SELF_TEST START/STOP CONTROL ( 0->START, 1->STOP)
	space0.value  = 0xffffffff;       //stop
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	return TRUE;
}


BOOL CFH::VECntRead(DWORD *pVCnt, DWORD *pECnt, DWORD* pBCnt, int nBufLen)
{
	RW_SPACEX_LONG space0;
	DWORD dwRet;

	DbgPrt("CFH::ReadVECnt\r\n", g_hWndFH);
	if( nBufLen < 33 ) return FALSE;
	if( m_hDevice==INVALID_HANDLE_VALUE) return FALSE;

	//set bit6->1 to pause count
	space0.offset = 0x14000;
	space0.value  = 0xFFFFFFFF;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	for(int i=0; i<33; i++){
	
		space0.offset = 0x14000;
		space0.value  = i+0x40;       //set read counter channel,and keep pause counter
		m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
		DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
						&space0, sizeof(space0),  //in buf
						&space0, sizeof(space0),  //out buf
						&dwRet,
						&m_olIOCtl
						);			            
		WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
		ResetEvent(m_olIOCtl.hEvent);

	    //∂¡ƒ‹¡øŒªÕ≥º∆º« ˝
		space0.offset = 0x10000+4*i;
		space0.value  = 0x0;       
		m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
		DeviceIoControl(m_hDevice, IOCTL_READSPACE0_LONG, 
						&space0, sizeof(space0),  //in buf
						&space0, sizeof(space0),  //out buf
						&dwRet,
						&m_olIOCtl
						);			            
		WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
		ResetEvent(m_olIOCtl.hEvent);
		pECnt[i] = space0.value;

	    //∂¡”––ßŒªÕ≥º∆º« ˝
		space0.offset = 0x14000+4*i;
		space0.value  = 0x0;       
		m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
		DeviceIoControl(m_hDevice, IOCTL_READSPACE0_LONG, 
						&space0, sizeof(space0),  //in buf
						&space0, sizeof(space0),  //out buf
						&dwRet,
						&m_olIOCtl
						);			            
		WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
		ResetEvent(m_olIOCtl.hEvent);
		pVCnt[i] = space0.value;

		if( i > 8 ) continue;

	    //∂¡BLOCK”––ßÕ≥º∆º« ˝
		space0.offset = 0x18000+4*i;
		space0.value  = 0x0;       
		m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
		DeviceIoControl(m_hDevice, IOCTL_READSPACE0_LONG, 
						&space0, sizeof(space0),  //in buf
						&space0, sizeof(space0),  //out buf
						&dwRet,
						&m_olIOCtl
						);			            
		WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
		ResetEvent(m_olIOCtl.hEvent);
		pBCnt[i] = space0.value;
		//Sleep(100);
	}

	//set bit6->0 to restart count
	space0.offset = 0x14000;
	space0.value  = 0;       
	m_olIOCtl.Offset = 0; m_olIOCtl.OffsetHigh = 0;
	DeviceIoControl(m_hDevice, IOCTL_WRITESPACE0_LONG, 
					&space0, sizeof(space0),  //in buf
					&space0, sizeof(space0),  //out buf
					&dwRet,
					&m_olIOCtl
					);			            
	WaitForSingleObject(m_olIOCtl.hEvent, INFINITE);
	ResetEvent(m_olIOCtl.hEvent);

	return TRUE;
}

#define  F  (50*1000*1000)

//us -> 50M clk count
void CFH::TimeToCount(DWORD dwTimeInUs, DWORD *pdwCnt)
{
	double fTime,fCnt;

	fTime = (double)dwTimeInUs;
	fCnt =	(fTime * F) /1000000.;   //s
	*pdwCnt = (DWORD)(fCnt);
}

void CFH::TimeToCount(double fTimeInUs, DWORD *pdwCnt)
{
	double fCnt;

	fCnt =	(fTimeInUs * F) /1000000.;   //s
	*pdwCnt = (DWORD)(fCnt);
}

#endif  //end ifndef _FH_SIM


