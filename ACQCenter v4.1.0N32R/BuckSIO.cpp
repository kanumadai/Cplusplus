#include "StdAfx.h"
#include "BuckSio.h"

#ifdef _ACQCENTER
#include "..\AcqCenter\OWS.h"
#include "..\AcqCenter\ACQCenter.h"
#include "..\AcqCenter\ACQCenterDlg.h"
#endif

#ifdef _TESTBUCKSIO
#include "..\TestBuckSIO\BuckOWS.h"
#endif

#include "DbgPrt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COWS g_OWS;
extern HWND g_hWndSIO;

////////////////////////////////////////////////////////////////////
//              visiable in global 
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//              visiable in this file only

DWORD WINAPI ThreadMonitorBuck(LPVOID  lpData);

//
////////////////////////////////////////////////////////////////////


CBuckSIO::CBuckSIO()
{
    
	m_hCom = NULL;
	m_hEvtExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_bSIOMode_Test = FALSE;
	m_bSelfTest = FALSE;

	InitializeCriticalSection(&m_CLockWrite);
	m_hEvtWrite = CreateEvent( NULL, TRUE, FALSE, NULL);
	m_hEvtAnswer = CreateEvent( NULL, TRUE, FALSE, NULL);

	for(int i=0; i<32; i++) m_SelfTestStatus[i] = status_unknow;
	m_WorkStatus.bWorking = FALSE;
	m_WorkStatus.datMode = mode_unknow;
	m_WorkStatus.dwWorkTime = 0;
	m_WorkStatus.b9600 = FALSE;
}

CBuckSIO::~CBuckSIO()
{
	CloseHandle(m_hEvtExit);

	LeaveCriticalSection(&m_CLockWrite);
	DeleteCriticalSection(&m_CLockWrite);

	CloseHandle(m_hEvtWrite);
	CloseHandle(m_hEvtAnswer);

}

//设置SIO工作模式
//bTest=TRUE  : 串口数据送交OWS处理
//bTest=FALSE :	串口数据由本机处理
void CBuckSIO::SetSIOMode(BOOL bTest)
{
	m_bSIOMode_Test = bTest;
}

BOOL CBuckSIO::OpenSIO(const char* szPort, int nBaudRate)
{
	DCB dcb;
	HANDLE hTemp; 

	if(m_hCom != NULL) return FALSE;

#if DBG_LEVEL != 0
	char szTemp[128];
	sprintf(szTemp, "Start Comm:%s, baudrate %d!\r\n", szPort, nBaudRate);
	DbgPrt(szTemp, g_hWndSIO);
#endif

	hTemp = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0,
		               NULL,OPEN_EXISTING,
					   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
					   NULL);
    if(hTemp == INVALID_HANDLE_VALUE) return FALSE;

	if(SetCommMask(hTemp,EV_RXCHAR/*EV_RXFLAG*/) == 0){
		 CloseHandle(hTemp);
		 return FALSE;
	}
	if(SetupComm(hTemp,16*1024, 1024) == 0){
		 CloseHandle(hTemp);
		 return FALSE;
	}
	if(GetCommState(hTemp,&dcb) == 0){
	 	 CloseHandle(hTemp);
		 return FALSE;
	}
	dcb.BaudRate = nBaudRate;  //CBR_9600; //CBR_57600;
	dcb.fParity  = FALSE; //TRUE;  //disable parity check
	dcb.ByteSize = 8; 
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
    //dcb.XonChar = ASCII_XON ;
    //dcb.XoffChar = ASCII_XOFF ;
    dcb.fDtrControl = 1;
    dcb.fOutX = 0;
    dcb.fInX = 0;
    dcb.fRtsControl = 1;
    dcb.XonLim  =  100;
    dcb.XoffLim = 100;
	dcb.EvtChar = 0x7f;
	if(SetCommState(hTemp,&dcb) == 0){
	 	 CloseHandle(hTemp);
		 return FALSE;
	}

	COMMTIMEOUTS timeout;
	if( !GetCommTimeouts(hTemp,&timeout) ){
		CloseHandle(hTemp);
		return FALSE;
	}
	timeout.ReadIntervalTimeout = 1000;
	timeout.ReadTotalTimeoutMultiplier = 1000;
	timeout.ReadTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutConstant = 0;
	timeout.WriteTotalTimeoutMultiplier = 0;
	if( !SetCommTimeouts(hTemp, &timeout) ){
		CloseHandle(hTemp);
		return FALSE;
	}

    if(PurgeComm(hTemp,PURGE_TXABORT | PURGE_RXABORT 
	 	| PURGE_TXCLEAR | PURGE_RXCLEAR) == 0){
		CloseHandle(hTemp);
		return FALSE;
	}

	m_hCom = hTemp;

	DWORD dwThreadID;
    m_hThread = CreateThread( NULL,0, ThreadMonitorBuck,
				              this, 0,
							  &dwThreadID
							);

	dwThreadID = SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return TRUE;
}

BOOL CBuckSIO::CloseSIO()
{
	SetEvent(m_hEvtExit);
	WaitForSingleObject(m_hThread, 3000/*ms*/);

	ResetEvent(m_hEvtExit);
	CloseHandle(m_hCom);

	m_hCom = NULL;
	m_hThread = NULL;

	return TRUE;
}

DWORD WINAPI ThreadMonitorBuck(LPVOID  lpData)
{
	CBuckSIO* pSIO = (CBuckSIO*)lpData;

	DWORD exitCode = pSIO->_ThreadRecvComm();
    ExitThread(exitCode);

	return exitCode;
}

DWORD CBuckSIO::_ThreadRecvComm()
{
    COMSTAT ComStat;
	BOOL bContinue;
	DWORD dwWaitRet,dwMask,dwErr;
	OVERLAPPED ol,ol_read;
	HANDLE hEvt[2];

	char szTemp[1024];

	BYTE* pFrameBuf;

	DbgPrt("MonitorBuckProc Thread Begin!\r\n", g_hWndSIO);
	sprintf(szTemp, "Wait Answer Timeout Set to %d ms\r\n", BUCK_ANSWER_TIMEOUT);
	DbgPrt(szTemp, g_hWndSIO);

#define BUF_LEN 128*1024
	pFrameBuf = new BYTE[BUF_LEN];

	ol_read.Offset = 0;   ol_read.OffsetHigh = 0;
	ol_read.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ol.Offset = 0;        ol.OffsetHigh=0;
	ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hEvt[0] = ol.hEvent;
	hEvt[1] = m_hEvtExit;

	bContinue = TRUE;
	while(bContinue){
		WaitCommEvent(m_hCom, &dwMask, &ol);
		dwWaitRet = WaitForMultipleObjects(2, hEvt, FALSE, INFINITE);
		switch(dwWaitRet){
			////////////////////////////////////////////////////
			//          Exit event 
			case WAIT_OBJECT_0+1:
				 CancelIo(m_hCom);
				 bContinue = FALSE;
				 break;
			//          end process exit event
			//////////////////////////////////////////////////////


			default:  //Comm IO Error handle
				 bContinue = FALSE;
				 break;
			////////////////////////////////////////////////////
			//		    comm event
			case WAIT_OBJECT_0:
				 ResetEvent(ol.hEvent);
				 ClearCommError(m_hCom, &dwErr, &ComStat);
				 if( dwErr != 0){
					 sprintf(szTemp, "Bucket Com Error=%d\r\n", dwErr);
					 TRACE(szTemp);
					 DbgPrt(szTemp, g_hWndSIO);
				 }							  
				 sprintf(szTemp, "Dat In Queue:%d\r\n", ComStat.cbInQue);
				 DbgPrt(szTemp, g_hWndSIO);

				 if( ComStat.cbInQue<sizeof(BUCK_FRAME_IN) ) break;

				 if( !RecvFrame(&ol_read, pFrameBuf, BUF_LEN) )
				 {
					 DbgPrt("Recv Frame Error\r\n", g_hWndSIO);
					 PurgeComm(m_hCom, PURGE_RXCLEAR); 
				 }

				 break; 
			//          end process com event
			/////////////////////////////////////////////////////
		} //end switch
	}//end while
	

	//DbgPrt("MonitorCOMThread Exit!\r\n", g_hWndSIO);
	
	CloseHandle(ol.hEvent);
	CloseHandle(ol_read.hEvent);

	delete [] pFrameBuf;

    return 0L;
}

BOOL CBuckSIO::RecvFrame(OVERLAPPED* pOL, BYTE* pFrameBuf, int nBufLen)
{
	if(pFrameBuf == NULL) return FALSE;
	if(pOL == NULL) return FALSE;

	DWORD dwReaded;
	DWORD dwWaitRet;
	BUCK_FRAME_IN* pHead;

	memset(pFrameBuf, 0, nBufLen);

	// read frame head first
	ResetEvent( pOL->hEvent );
	if( !ReadFile( m_hCom, pFrameBuf, sizeof(BUCK_FRAME_IN), &dwReaded, pOL) )
	{
		if( GetLastError() == ERROR_IO_PENDING ){
  		    dwWaitRet = GetOverlappedResult(m_hCom, pOL, &dwReaded, TRUE);
			dwWaitRet = GetLastError();
			if(dwReaded != sizeof(BUCK_FRAME_IN)){
				DbgPrt("Read Frame Head Timeout\r\n", g_hWndSIO);
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}   

	int datlen;

	pHead = (BUCK_FRAME_IN*)pFrameBuf;
	if( !( pHead->sAddr==ADDR_BUCKET_ALL || 
		  (pHead->sAddr>=ADDR_BUCKET0 && pHead->sAddr<=ADDR_BUCKET31)
		 )
	  )
	{
		char szTemp[512];
		sprintf(szTemp, "Recv<=");
		for( int i=0; i<sizeof(BUCK_FRAME_IN); i++)
			sprintf(szTemp, "%s %2.2x", szTemp, pFrameBuf[i]);
		sprintf(szTemp, "%s  BuckSIO sAddr Error\r\n", szTemp);
		DbgPrt(szTemp, g_hWndSIO);
	}
	datlen = pHead->len16;

	// read frame dat after head
	datlen -= sizeof(BUCK_FRAME_IN);
	if(datlen > nBufLen || datlen<0){
	   DbgPrt("Dat Len Error\r\n", g_hWndSIO);
	   return FALSE;
	}
	ResetEvent( pOL->hEvent );
	if( !ReadFile( m_hCom, pFrameBuf+sizeof(BUCK_FRAME_IN), datlen, &dwReaded, pOL) )
	{
		if( GetLastError() == ERROR_IO_PENDING ){
			dwWaitRet = GetOverlappedResult(m_hCom, pOL, &dwReaded, TRUE);
			dwWaitRet = GetLastError();
			if( dwReaded!=datlen ){
				DbgPrt("Read Frame Timeout\r\n", g_hWndSIO);
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}  

	//Test Mode : Comm Data Send to OWS
	if( m_bSIOMode_Test ){
		g_OWS.SendTestRet( INIT_OBJ_BUCKET_ALL, (BYTE*)pHead,  datlen+sizeof(BUCK_FRAME_IN) );
	}

	dwWaitRet = Process_BucketFrame((BYTE*)pHead, pHead->len16);

	memset(pFrameBuf, 0, nBufLen);

	return dwWaitRet;
}

BOOL CBuckSIO::Process_BucketFrame(BYTE* pFrame, int datLen)
{
#if DBG_LEVEL != 0 
	char szTemp[512];
#endif
	int i;

#if DBG_LEVEL != 0
	sprintf(szTemp, "Recv<==");
	if( datLen<=12 ){
		for(i=0; i<datLen; i++){
			sprintf(szTemp, "%s%2.2x ",szTemp, pFrame[i]);
		}
	}else{
		for(i=0; i<10; i++){//sizeof(BUCK_FRAME_IN); i++){
			sprintf(szTemp, "%s%2.2x ",szTemp, pFrame[i]);
		}
		sprintf( szTemp, "%s data too len, show 10Bytes only!", szTemp);
	}
	sprintf(szTemp,"%s\r\n",szTemp);
	DbgPrt(szTemp, g_hWndSIO);
#endif

	//verify eof flag
	if(pFrame[datLen-1] != 0x88 ){
		DbgPrt("Frame EOF FLAG ERROR!\r\n", g_hWndSIO);
		return FALSE;
	}

	//verify checksum
	BYTE checksum=0;
	for(i=0; i< (datLen-2); i++)
		checksum ^= pFrame[i];

	if(checksum != pFrame[datLen-2]){
		DbgPrt("CheckSum Error!\r\n", g_hWndSIO);
		return FALSE;
	}

	BUCK_FRAME_IN* pHead = (BUCK_FRAME_IN*)pFrame;
	switch( pHead->dat[0] ){
	case 0x55:  //general answer ok
		 if( m_bSelfTest && (pHead->len16==8) ){
			 int Bucket = pHead->sAddr - ADDR_BUCKET0;
			 if( Bucket<0 || Bucket>=32 ){
				 //ASSERT(FALSE);
				 break;
			 }
			 m_SelfTestStatus[Bucket] = status_ok;
			 PulseEvent(m_hEvtAnswer);

			 DbgPrt("Self Test Answer:ok\r\n", g_hWndSIO);
		 }
		 break;

	case 0x54:  //general answer err
		 if( m_bSelfTest && (pHead->len16==9) ){
			 int Bucket = pHead->sAddr - ADDR_BUCKET0;
			 if( Bucket<0 || Bucket>=32 ){
				 //ASSERT(FALSE);
				 break;
			 }
			 m_SelfTestStatus[Bucket] = BUCKET_SELFTEST_STATUS( pHead->dat[1] );
			 PulseEvent(m_hEvtAnswer);

			 sprintf(szTemp, "Self Test Answer:err, code=%2.2x\r\n", pHead->dat[1]);
			 DbgPrt(szTemp, g_hWndSIO);
		 }

		 break;

	default:
		 DbgPrt("Answer Skiped\r\n", g_hWndSIO);
		 break;
	}

	return TRUE;
}

BOOL CBuckSIO::WriteComm(LPCVOID pBuf, int len)
{
	DWORD dwRet,dwWrited;

	if(m_hCom == NULL) return FALSE;

#if DBG_LEVEL != 0
	 char szTemp[512];
	 BYTE* pBBuf = (BYTE*)pBuf;
	 sprintf(szTemp, "Send==>");
	 for(int j=0; j<len; j++)
		 sprintf(szTemp, "%s%2.2x ",szTemp,pBBuf[j]);
	 sprintf(szTemp, "%s\r\n", szTemp);
	 DbgPrt(szTemp, g_hWndSIO);
#endif 

	m_olWrite.hEvent = m_hEvtWrite;
	m_olWrite.Offset = 0;
	m_olWrite.OffsetHigh = 0;
	ResetEvent(m_olWrite.hEvent);

	if( !WriteFile(m_hCom, pBuf, len, &dwWrited, &m_olWrite) ){
		if( GetLastError() == ERROR_IO_PENDING ){
			dwRet = WaitForSingleObject(m_olWrite.hEvent, 1000/*ms*/);
			switch(dwRet){
				case WAIT_OBJECT_0:
					 return TRUE;
					 
				case WAIT_TIMEOUT:
				case WAIT_ABANDONED:
				default:
					 CancelIo(m_hCom);
					 return FALSE;
			}
		}else{
			CancelIo(m_hCom);
			return FALSE;
		}
	}

	return TRUE;
}

//要求BUCKET自检
BOOL CBuckSIO::Control_SelfTest()
{
    if(m_hCom == NULL) return FALSE;

    EnterCriticalSection(&m_CLockWrite);

	BUCK_FRAME6 frame;
	
	frame.checksum = 0;
	frame.tAddr = ADDR_BUCKET_ALL;  frame.checksum ^= frame.tAddr;
	frame.sAddr = ADDR_ACS;			frame.checksum ^= frame.sAddr;
	frame.len8  = 6;                frame.checksum ^= frame.len8;
	frame.CmdMain = 0xCE;           frame.checksum ^= frame.CmdMain;
	frame.eof     = 0x88;

	BOOL ret = WriteComm( &frame, sizeof(frame) );

	if( ret )	Sleep(BUCK_SELFTEST_DELAY);
	
	LeaveCriticalSection(&m_CLockWrite);

	return ret;
}

//查询BUCKET自检结果
BOOL CBuckSIO::Query_SelfTestResult(int nBucket)
{
    if(m_hCom == NULL) return FALSE;
	if( nBucket<0 || nBucket>=32 ) return FALSE;

    EnterCriticalSection(&m_CLockWrite);

	BUCK_FRAME6 frame;
	
	frame.checksum = 0;
	frame.tAddr = ADDR_BUCKET0+nBucket;  frame.checksum ^= frame.tAddr;
	frame.sAddr = ADDR_ACS;			frame.checksum ^= frame.sAddr;
	frame.len8  = 6;                frame.checksum ^= frame.len8;
	frame.CmdMain = 0xCF;           frame.checksum ^= frame.CmdMain;
	frame.eof     = 0x88;

	m_bSelfTest = TRUE;

	if( !WriteComm( &frame, sizeof(frame) ) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	}

	switch(WaitForSingleObject(m_hEvtAnswer, BUCK_ANSWER_TIMEOUT))
	{
    m_bSelfTest = FALSE;
	case WAIT_OBJECT_0:
		 break;
			 
	default:
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	}
	
	LeaveCriticalSection(&m_CLockWrite);

	return TRUE;
}


//设置BUCKET数据采集模式及采集时间（分钟）
//同时启动数据采集
BOOL CBuckSIO::Control_DataAcqBegin(BUCKET_DAT_MODE mode, DWORD time)
{
    if(m_hCom == NULL) return FALSE;

    EnterCriticalSection(&m_CLockWrite);

	BUCK_FRAME11 frame;

	frame.checksum = 0;
	frame.tAddr  = ADDR_BUCKET_ALL; frame.checksum ^= frame.tAddr;
	frame.sAddr  = ADDR_ACS;        frame.checksum ^= frame.sAddr;
	frame.len8   = 11;              frame.checksum ^= frame.len8;
	frame.CmdMain = mode;           frame.checksum ^= frame.CmdMain;
	frame.CmdSub  = 0x02;           frame.checksum ^= frame.CmdSub;
	frame.dwDat   = time;           frame.checksum ^= frame.cDat[0];
	                                frame.checksum ^= frame.cDat[1];  
	                                frame.checksum ^= frame.cDat[2];  
	                                frame.checksum ^= frame.cDat[3];  
	frame.eof	= 0x88;

	if( !WriteComm(&frame, sizeof(frame)) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	}

	LeaveCriticalSection(&m_CLockWrite);

	m_WorkStatus.bWorking = TRUE;
	m_WorkStatus.datMode = mode;
	m_WorkStatus.dwWorkTime = time;

#ifdef _ACQCENTER
	CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
	pMainWnd->ShowTips();
#endif

	return TRUE;
}

//停止数据采集
BOOL CBuckSIO::Control_DataAcqStop()
{
    if(m_hCom == NULL) return FALSE;

    EnterCriticalSection(&m_CLockWrite);

	BUCK_FRAME8 frame;

	frame.checksum = 0;
	frame.tAddr  = ADDR_BUCKET_ALL; frame.checksum ^= frame.tAddr;
	frame.sAddr  = ADDR_ACS;        frame.checksum ^= frame.sAddr;
	frame.len8   = 8;              frame.checksum ^= frame.len8;
	frame.CmdMain = 0xCA;           frame.checksum ^= frame.CmdMain;
	frame.CmdSub  = 0x00;           frame.checksum ^= frame.CmdSub;
	frame.cDat    = 0x00;           frame.checksum ^= frame.cDat;
	frame.eof	= 0x88;

	if( !WriteComm(&frame, sizeof(frame)) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	}

	LeaveCriticalSection(&m_CLockWrite);

	m_WorkStatus.bWorking = FALSE;

#ifdef _ACQCENTER
	CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
	pMainWnd->ShowTips();
#endif

	return TRUE;
}

BOOL CBuckSIO::SendTestData(BYTE *pBuf, int nBufLen)
{
    if(m_hCom == NULL) return FALSE;

    EnterCriticalSection(&m_CLockWrite);

	if( !WriteComm(pBuf, nBufLen) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	}

	LeaveCriticalSection(&m_CLockWrite);

	/////////////////////////////////////////////////////////
	// change baudrate
	BUCK_FRAME7* pFrame = (BUCK_FRAME7*)pBuf;
	if( (pFrame->len8==7) && (pFrame->eof==0x88) && (pFrame->CmdMain==0xC0) ){
		DCB dcb;
		char szTemp[128];
		GetCommState(m_hCom, &dcb);
		switch(pFrame->CmdSub) {
		case 0x38:  //set baudrate to 38400
			 dcb.BaudRate = CBR_38400;
			 m_WorkStatus.b9600 = FALSE;
			 sprintf(szTemp, "Bucket SIO Baudrate set to 38400\r\n");
			 break;
		case 0x96:  //set baudrate to 9600
			 dcb.BaudRate =	CBR_9600;
			 m_WorkStatus.b9600 = TRUE;
			 sprintf(szTemp, "Bucket SIO Baudrate set to 9600\r\n");
			 break;
		default:
			 sprintf(szTemp, "Bucket SIO Baudrate set unknow\r\n");
			 break;
		}
		DbgPrt(szTemp, g_hWndSIO);
		SetCommState(m_hCom, &dcb);
		#ifdef _ACQCENTER
			CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
			pMainWnd->ShowTips();
		#endif
	}
	// change baudrate
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	// stop acq
	BUCK_FRAME8* pFrame8;
	pFrame8 = (BUCK_FRAME8*)pBuf;
	if( (pFrame8->CmdMain==0xCA) && (pFrame8->CmdSub==0) &&
		(pFrame8->cDat==0) && (pFrame8->len8==8) && (pFrame8->eof==0x88) 
	  )
	{
		m_WorkStatus.bWorking = FALSE;
		#ifdef _ACQCENTER
			CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
			pMainWnd->ShowTips();
		#endif
	}
	// stop acq
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	// start acq
	BUCK_FRAME11* pFrame11;
	pFrame11 = (BUCK_FRAME11*)pBuf;
	if( (pFrame11->len8==11) && (pFrame11->eof==0x88) ){
		m_WorkStatus.bWorking = TRUE;
		m_WorkStatus.datMode  = (BUCKET_DAT_MODE)pFrame11->CmdMain;
		m_WorkStatus.dwWorkTime = pFrame11->dwDat;
		#ifdef _ACQCENTER
			CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
			pMainWnd->ShowTips();
		#endif
	}
	// start acq
	//////////////////////////////////////////////////////////////

	return TRUE;
}
