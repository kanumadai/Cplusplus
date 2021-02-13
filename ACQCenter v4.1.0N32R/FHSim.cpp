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

#include "FHSim.h"
#include "DatFmt.h"

#include "DbgPrt.h"

#include <winioctl.h>
#include "..\sys.sg\ioctls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
			
#ifdef _FH_SIM

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

DWORD WINAPI ThreadMonitorFHSim(LPVOID  lpData);

//
////////////////////////////////////////////////////////////////////


CFHSim::CFHSim()
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

CFHSim::~CFHSim()
{
	CloseHandle( m_hEvtExit);
	CloseHandle(m_olIOCtl.hEvent);

	for(int i=0; i<BUF_FH_CNT; i++) GlobalFree( g_hBufFH[i] );
	for(i=0; i<BUF_ECG_CNT; i++) GlobalFree( g_hBufEcg[i] );
}

BOOL CFHSim::OpenDevice()
{
	if(m_hDevice != INVALID_HANDLE_VALUE) return FALSE;

	DbgPrt("CFH Runing in Simulation Mode!\r\n", g_hWndFH);
	m_hDevice = NULL;

	m_ThreadParam.hDevice  = m_hDevice;
	m_ThreadParam.hEvtExit = m_hEvtExit;
	m_ThreadParam.pFH  = this;

	DWORD dwThreadID;

	m_hThread = CreateThread(NULL, 0, ThreadMonitorFHSim, 
							 this, NULL,&dwThreadID);
	if(m_hThread == NULL){
		DbgPrt("Create Coin Monitor Thread Error!\r\n", g_hWndFH);
		CloseHandle(m_hDevice);
		return FALSE;
	}

	return TRUE;
}

BOOL CFHSim::CloseDevice()
{
	SetEvent(m_hEvtExit);
	WaitForSingleObject(m_hThread, 2000);

	m_hThread = NULL;
	m_hDevice = INVALID_HANDLE_VALUE;

	return TRUE;
}

DWORD WINAPI ThreadMonitorFHSim(LPVOID  lpData)
{
#define FHDAT_LEN 64*1024
#define ECGDAT_LEN  4096

    char szTemp[512];

	DbgPrt("(FHSim)Ready To Receive Coin Data\r\n", g_hWndFH);

	DWORD* pDW;
	for(int i=0; i<4; i++){
		pDW = (DWORD*)g_hBufFH[i];
		for(int j=0; j<FHDAT_LEN/4; j++)
		{
		   pDW[j] = 0x3fffffff;
		}
	}

	CFHSim* pFHSim = (CFHSim*)lpData;

	BOOL bContinue = TRUE;
	DWORD dwWait;
	DWORD dwFHBuf=0, dwECGBuf=0;

	do{
	   dwWait = WaitForSingleObject(pFHSim->m_hEvtExit, 1000);
	   switch( dwWait ){
	   case WAIT_TIMEOUT:
		   if( pFHSim->m_bFHStarted ) {
			   #if DBG_LEVEL != 0
				   sprintf(szTemp, "(Sim)Recv Coin Dat, Buf=%d, Len=%8.8x\r\n", dwFHBuf, FHDAT_LEN);
				   DbgPrt(szTemp, g_hWndFH);
			   #endif

			   g_OWS.SendFHDat((BYTE*)g_hBufFH[dwFHBuf], FHDAT_LEN, dwFHBuf );
			   //memset(g_hBufFH[dwBufOrder], 0x01, readed_fh[dwBufOrder]);

			   dwFHBuf++;
			   dwFHBuf %= 4;
		   }

		   if( pFHSim->m_bEcgStarted){
			   #ifdef _TESTFH
			   ((CFHSimDlg*)(AfxGetMainWnd()))->DrawADC((BYTE*)g_hBufEcg[dwECGBuf], ECGDAT_LEN);
			   #else
			   g_OWS.SendEcgDat((BYTE*)g_hBufEcg[dwECGBuf], ECGDAT_LEN);
			   #endif

			   dwECGBuf++;

			   dwECGBuf %= 4;
		   }
		   break;

	   default:
		   bContinue = FALSE;
	   }
	}while( bContinue == TRUE );

	return TRUE;
}

BOOL CFHSim::EcgParamSet(int SampleFreq, int factor)
{
	char szTemp[512];
	sprintf(szTemp, "CFHSim::EcgParamSet( Sample Freq=%d, ADC Factor=%d\r\n", SampleFreq, factor);
	DbgPrt(szTemp, g_hWndFH);

	m_nAdcSampleFreq = SampleFreq;
	m_nAdcAmpFactor = factor;

	return TRUE;
}

BOOL CFHSim::EcgStart(int BufSize)
{
	char szTemp[512];

	sprintf(szTemp, "CFHSim::EcgStart, BufSize=%d\r\n", BufSize);
	DbgPrt(szTemp, g_hWndFH);
	
	m_bEcgStarted = TRUE;
	return TRUE;
}

BOOL CFHSim::EcgStop()
{
	DbgPrt("CFHSim::EcgStop\r\n", g_hWndFH);

	m_bEcgStarted = FALSE;

	return TRUE;
}

BOOL CFHSim::GateParamSet(const void *pParam, int nParamLen)
{
	if(nParamLen != 256) return FALSE;
	if(pParam == NULL) return FALSE;
	if( m_bGateStarted ) return FALSE;

	DbgPrt("CFHSim::GateParamSet\r\n", g_hWndFH);

	return TRUE;
}

BOOL CFHSim::GateParamSet(const ACQ_PROTOCOL *pProtocol)
{
	double fCircleUTH, fCircleLTH;  //ﾐﾄﾌﾜﾆﾚﾉﾏﾏﾂﾏﾞ
	double fCircleRef, fCircleTH;
	DWORD  dwCircleRef, dwCircleTH;
	double fFSTime, fFETime;
	double fFrameLen, fTimeBase;
	double fPrevEndTime;
	int i, nGate;

	DbgPrt("CFHSim::GateParamSet\r\n", g_hWndFH);

	if( m_bGateStarted ) return FALSE;
	if( pProtocol == NULL ) return FALSE;

	DWORD dwGateParam[64];

	//ﾐﾄﾂﾊ(ﾐﾄﾌｿｷﾖ)->ﾐﾄﾌﾜﾆﾚ(usﾃｿﾐﾄﾌ・
	fCircleUTH = 1000000.*1./(pProtocol->gate_head.LPB/60.); 
	fCircleLTH = 1000000.*1./(pProtocol->gate_head.UPB/60.);

	//ｲﾎｿｼﾖﾜﾆﾚ->ﾈ｡ﾐﾄﾌﾜﾆﾚﾉﾏﾏﾂﾏﾞｵﾄﾆｽｾｵ
	fCircleRef = (fCircleUTH + fCircleLTH)/2.0;
	TimeToCount(fCircleRef, &dwCircleRef);

	//ﾖﾜﾆﾚﾔﾊﾐ﨔・
	fCircleTH  = fabs(fCircleRef - fCircleUTH);
	TimeToCount(fCircleTH, &dwCircleTH);

	for(i=0; i<64; i++) dwGateParam[i] = 0xffffffff;
	if( pProtocol->gate_head.GateClass == FALSE ) 
	{   //ｲﾉｼｯｺﾖﾃﾅ
		dwGateParam[61] = 2;            //mode 2
		dwGateParam[62] = dwCircleTH;   //ｲﾎｿｼﾎ・
		dwGateParam[63] = dwCircleRef;  //ｲﾎｿｼﾖﾜﾆﾚ
	}
	else if( pProtocol->gate_head.GateClass == TRUE &&
		     pProtocol->gate_head.PhaseOrFixed == TRUE
		   )
	{	//ｲﾉｼｯﾊｱｻﾚｹﾌｶｨﾊｱｼ莵ﾖﾃﾅ
		
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
		dwGateParam[62] = dwCircleTH;   //ｲﾎｿｼﾎ・
		dwGateParam[63] = dwCircleRef;  //ｲﾎｿｼﾖﾜﾆﾚ
	}
	else
	{	//ｲﾉｼｯﾊｱｻﾚRｲｨｷﾖﾃﾅ

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
		dwGateParam[62] = dwCircleTH;   //ｲﾎｿｼﾎ・
		dwGateParam[63] = dwCircleRef;  //ｲﾎｿｼﾖﾜﾆﾚ
	}

	return GateParamSet(&dwGateParam, 64*4);
}

BOOL CFHSim::GateStart()
{
	DbgPrt("CFHSim::GateStart\r\n", g_hWndFH);

	m_bGateStarted = TRUE;

	return TRUE;
}

BOOL CFHSim::GateStop()
{
	DbgPrt("CFHSim::GateStop\r\n", g_hWndFH);

	m_bGateStarted = FALSE;

	return TRUE;
}

BOOL CFHSim::Rs485Start()
{
	DbgPrt("CFHSim::Rs485Start\r\n", g_hWndFH);

	m_bRs485Started = TRUE;

	return TRUE;
}

BOOL CFHSim::RS485Stop()
{
	DbgPrt("CFHSim::RS485Stop\r\n", g_hWndFH);

	m_bRs485Started = FALSE;

	return TRUE;
}

BOOL CFHSim::FHParamSet( DWORD dwIntValue, FH_INT_TYPE intType, 
					  FH_SCANMODE scanmode, 
					  DWORD dwFHRing, DWORD dwFHFov,
					  DWORD dwFHTimeNs, DWORD dwMinTimeNs, DWORD dwMaxTimeNs,
					  int nOutFmt, int nEneryMode
					 )
{
	DbgPrt("CFHSim::FHParamSet\r\n", g_hWndFH);

	if( m_bFHStarted ) return FALSE;

	m_nCoinFmt = nOutFmt;

	return TRUE;
}

BOOL CFHSim::FHStart(BOOL bTestMode, int TestBucket)
{
	DbgPrt("CFHSim::FHStart\r\n", g_hWndFH);

	m_bTestMode = bTestMode;
	m_nTestBucket = TestBucket;

	int i,j;
	DWORD* pDW;
	if( bTestMode ){
		FH_ROWDAT rowDat;
		rowDat.dwDat = 0;
		rowDat.bucket = TestBucket;
		rowDat.valid = 1;
		for( i=0; i<4; i++){
			pDW = (DWORD*)g_hBufFH[i];
			for( j=0; j<FHDAT_LEN/4; j++){
				rowDat.block++;
				rowDat.energy++;
				rowDat.strip++;
				rowDat.time++;
				pDW[j] = rowDat.dwDat;
			}
		}
	}else{
		for( i=0; i<4; i++){
			pDW = (DWORD*)g_hBufFH[i];
			for( j=0; j<FHDAT_LEN/4; j++){
				pDW[j] = 0x3fffffff;
			}
		}
	}

	m_bFHStarted = TRUE;

	return TRUE;
}

BOOL CFHSim::FHStop()
{
	DbgPrt("CFHSim::FHStop\r\n", g_hWndFH);

    m_bFHStarted = FALSE;
	return TRUE;
}


#define  F  (50*1000*1000)

//us -> 50M clk count
void CFHSim::TimeToCount(DWORD dwTimeInUs, DWORD *pdwCnt)
{
	double fTime,fCnt;

	fTime = (double)dwTimeInUs;
	fCnt =	(fTime * F) /1000000.;   //s
	*pdwCnt = (DWORD)(fCnt);
}

void CFHSim::TimeToCount(double fTimeInUs, DWORD *pdwCnt)
{
	double fCnt;

	fCnt =	(fTimeInUs * F) /1000000.;   //s
	*pdwCnt = (DWORD)(fCnt);
}

#endif //end ifdef _FH_SIM