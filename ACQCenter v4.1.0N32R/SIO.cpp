#include "StdAfx.h"
#include "Sio.h"

#ifdef _ACQCENTER
#include "..\AcqCenter\OWS.h"
#include "..\AcqCenter\ACQCenter.h"
#include "..\AcqCenter\ACQCenterDlg.h"
#endif

#ifdef _TESTSIO
#include "..\TestSIO\SIOOWS.h"
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
CRITICAL_SECTION g_CLockMCtlInfo;
volatile MCTL_INFO   g_MCtlInfo;             //机电控制电路相关信息

HANDLE    g_hEvtSIO_Alarm;          //报警事件
HANDLE    g_hEvtSIO_EMCReset;        //机电控制电路手动复位
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//              visiable in this file only
DWORD WINAPI ThreadMonitorEMC(LPVOID  lpData);
//
////////////////////////////////////////////////////////////////////

DWORD WINAPI ThreadMonitorEMC(LPVOID  lpData)
{
	CSIO* pSIO = (CSIO*)lpData;

	DWORD dwRet = pSIO->_ThreadRecvComm();

	ExitThread( dwRet );
	return dwRet;
}

DWORD CSIO::_ThreadRecvComm()
{
    COMSTAT ComStat;
	BOOL bContinue;
	DWORD dwWaitRet,dwMask,dwErr;
	OVERLAPPED ol,ol_read;
	HANDLE hEvt[2];

	char szTemp[1024];

	BYTE* pFrameBuf;

	DbgPrt("MonitorComProc Thread Begin!\r\n", g_hWndSIO);
	sprintf(szTemp, "Wait Answer Timeout Set to %d ms\r\n", SIO_ANSWER_TIMEOUT);
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
			//		    comm event
			case WAIT_OBJECT_0:
				 ResetEvent(ol.hEvent);
				 ClearCommError(m_hCom, &dwErr, &ComStat);
				 if(dwErr != 0){
					sprintf(szTemp, "EMC Com Error=%d\r\n", dwErr);
					TRACE(szTemp);
					DbgPrt(szTemp, g_hWndSIO);
				 }							  
				 //sprintf(szTemp, "Dat In Queue:%d\r\n", ComStat.cbInQue);
				 //DbgPrt(szTemp, g_hWndSIO);

				 if( ComStat.cbInQue<4 ) break;

				 if( !RecvFrame(&ol_read, pFrameBuf, BUF_LEN) )
				 {
					 DbgPrt("Recv Frame Error\r\n", g_hWndSIO);
					 PurgeComm(m_hCom, PURGE_RXCLEAR); 
				 }

				 break; 
			//          end process com event
			/////////////////////////////////////////////////////

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
		} //end switch
	}//end while
	

	//DbgPrt("MonitorCOMThread Exit!\r\n", g_hWndSIO);
	
	CloseHandle(ol.hEvent);
	CloseHandle(ol_read.hEvent);

	delete [] pFrameBuf;

    //ExitThread(0);
    return 0L;
}

BOOL CSIO::RecvFrame(OVERLAPPED* pOL, BYTE* pFrameBuf, int nBufLen)
{
	if(pFrameBuf == NULL) return FALSE;
	if(pOL == NULL) return FALSE;

	DWORD dwReaded;
	DWORD dwWaitRet;
	SIO_FRAME_HEAD* pHead;
	pHead = (SIO_FRAME_HEAD*)pFrameBuf;

	memset(pFrameBuf, 0, nBufLen);

	// read frame head first
	ResetEvent( pOL->hEvent );
	if( !ReadFile( m_hCom, pFrameBuf, sizeof(SIO_FRAME_HEAD), &dwReaded, pOL) )
	{
		if( GetLastError() == ERROR_IO_PENDING ){
  		    dwWaitRet = GetOverlappedResult(m_hCom, pOL, &dwReaded, TRUE);
			dwWaitRet = GetLastError();
			if(dwReaded != sizeof(SIO_FRAME_HEAD)){
				DbgPrt("Read Frame Head Timeout\r\n", g_hWndSIO);
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}   

	int datlen;
	BOOL bGantryDat = FALSE;
	if( pHead->sAddr==ADDR_GANTRY || pHead->sAddr==ADDR_ROD    || pHead->sAddr==ADDR_BED ||
		pHead->sAddr==ADDR_Y1Y2    || pHead->sAddr==ADDR_Y1     || pHead->sAddr==ADDR_Y2  ||
		pHead->sAddr==ADDR_SEPTA  || pHead->sAddr==ADDR_RODBEDSEPTA || pHead->sAddr == ADDR_HV
	  )
	{
		datlen = pHead->len8;
		bGantryDat = TRUE;
	}
	else
	{
		DbgPrt("SIO head source addr error\r\n", g_hWndSIO);
		return FALSE;
	}

	// read frame dat after head
	datlen -= sizeof(SIO_FRAME_HEAD);
	if(datlen > nBufLen || datlen<0){
	   DbgPrt("Dat Len Error\r\n", g_hWndSIO);
	   return FALSE;
	}
	ResetEvent( pOL->hEvent );
	if( !ReadFile( m_hCom, pFrameBuf+sizeof(SIO_FRAME_HEAD), datlen, &dwReaded, pOL) )
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
		g_OWS.SendTestRet( INIT_OBJ_GANTRY, (BYTE*)pHead,  datlen+sizeof(SIO_FRAME_HEAD) );
	}

	if(bGantryDat){
		if( pHead->len8 != 12)
			DbgPrt("EMC Head Len Error!\r\n", g_hWndSIO);
		dwWaitRet = Process_GantryDat( (SIO_FRAME12*)pHead);
	}

	memset(pFrameBuf, 0, 12);

	return dwWaitRet;
}


BOOL CSIO::Process_GantryDat(SIO_FRAME12* pFrame)
{
#if DBG_LEVEL != 0 
	char szTemp[512];
#endif
	int i;
	BYTE* pBuf = (BYTE*)pFrame;

#if DBG_LEVEL != 0
	sprintf(szTemp, "Recv<==");
	for(i=0; i<sizeof(SIO_FRAME12); i++){
	   sprintf(szTemp, "%s%2.2x ",szTemp, pBuf[i]);
	}
	sprintf(szTemp,"%s\r\n",szTemp);
	DbgPrt(szTemp, g_hWndSIO);
#endif

	//verify eof flag
	if(pFrame->eof != 0x88 ){
		DbgPrt("GANTRY EOF FLAG ERROR!\r\n", g_hWndSIO);
		return FALSE;
	}

	//verify checksum
	BYTE checksum=0;
	pBuf = (BYTE*)pFrame;
	for(i=0; i< (sizeof(SIO_FRAME12)-2); i++)
		checksum ^= pBuf[i];


	if(checksum != pFrame->CheckSum){
		DbgPrt("CheckSum Error!\r\n", g_hWndSIO);
		return FALSE;
	}

	if( pFrame->tAddr == ADDR_DEBUG ) return TRUE;

	switch(pFrame->CmdMain){
		case 0xA0:  // 查询GANTRY初始化状态应答   //GANTRYGANTRY握手应答,
			 if( pFrame->CmdSub == 0x66){
				 EnterCriticalSection(&g_CLockMCtlInfo);
					 if( (pFrame->Dat[2]==0xff) && (pFrame->Dat[3]==0xff) )
						 g_MCtlInfo.bInitOK = TRUE;
					 else
						 g_MCtlInfo.bInitOK = FALSE;
					 g_MCtlInfo.cInitStatus[0] = pFrame->Dat[0];
					 g_MCtlInfo.cInitStatus[1] = pFrame->Dat[1];
					 g_MCtlInfo.cInitStatus[2] = pFrame->Dat[2];
					 g_MCtlInfo.cInitStatus[3] = pFrame->Dat[3];

				 LeaveCriticalSection(&g_CLockMCtlInfo);
				 
				 PulseEvent(m_hEvtSIO_HandShake);
				 DbgPrt("Query GANTRY Init Answer\r\n", g_hWndSIO);
			 }
			 break;

		case 0xA5:  //设置数据有效应答
			 PulseEvent(m_hEvtSIO_SetOK);
			 DbgPrt("Set Data Valid\r\n", g_hWndSIO);
			 break;

		case 0xE0:  // 数据重发请求
			 PulseEvent(m_hEvtSIO_Retry);
			 DbgPrt("Require to Send Again\r\n", g_hWndSIO);
			 break;

		case 0xB3:  //回传温度（GANTRY、ROD、YE12、SEPTA、BED）
			 EnterCriticalSection(&g_CLockMCtlInfo);
			    switch( pFrame->CmdSub )
				{
				case ADDR_GANTRY:
				     //Gantry温度
			 		 g_MCtlInfo.wT_GANTRY = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload GANTRY T=%d\r\n", g_MCtlInfo.wT_GANTRY);
					 #endif
					 break;

				case ADDR_ROD:
					 //ROD温度
					 g_MCtlInfo.wT_Rod = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload ROD T=%d\r\n", g_MCtlInfo.wT_Rod);
					 #endif
					 break;

				case ADDR_BED:
					 //BED温度
					 g_MCtlInfo.wT_Bed = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload BED T=%d\r\n", g_MCtlInfo.wT_Bed);
					 #endif
					 break;

				case ADDR_SEPTA:
					 //SEPTA温度
					 g_MCtlInfo.wT_Septa = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload Septa T=%d\r\n", g_MCtlInfo.wT_Septa);
					 #endif
					 break;

				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload 0x%2.2x T=%d\r\n", pFrame->CmdSub, g_MCtlInfo.wT_GANTRY);
					 #endif
					 break;
				}

				DbgPrt(szTemp, g_hWndSIO);
			     
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_TVI);
			 break;

		case 0xC0:	// 回传WATERCOOL温度
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if(pFrame->CmdSub==0x20)
				{   //WaterCool温度
					g_MCtlInfo.wT_WATERCOOL = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					#if DBG_LEVEL != 0
						sprintf(szTemp,"Upload WaterCool T=%d\r\n", g_MCtlInfo.wT_WATERCOOL);
					#endif
				}
				DbgPrt(szTemp, g_hWndSIO);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_TVI);
			 break;

		case 0xC1:  // 回传状态、高压、电流值
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				 g_MCtlInfo.HVStatus = pFrame->CmdSub;
				 if( pFrame->CmdSub > 0) g_MCtlInfo.bHVOn = TRUE;
				 else  g_MCtlInfo.bHVOn = FALSE;

				 //高压,
				 g_MCtlInfo.wV_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 g_MCtlInfo.wI_1 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
				 #if DBG_LEVEL != 0
				    sprintf(szTemp, "Upload HV Status=%d, HV=%d, HV I=%d\r\n", 
						             g_MCtlInfo.HVStatus, 
									 g_MCtlInfo.wV_1, 
									 g_MCtlInfo.wI_1
							);
					DbgPrt(szTemp, g_hWndSIO);
				 #endif

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 PulseEvent(m_hEvtSIO_TVI);
			 break;

		case 0xC2:	//高压控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 //ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bHVOn = TRUE;
					 DbgPrt("HV ON/Off Control Answer:ON\n", g_hWndSIO);
				 }
				 else
				 {
					 g_MCtlInfo.bHVOn = FALSE;
					 DbgPrt("HV ON/Off Control Answer:OFF\n", g_hWndSIO);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC3:  //激光灯控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 //ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bLaserOn = TRUE;
					 DbgPrt("LaserLed On/Off Control Answer:ON\n", g_hWndSIO);
				 }
				 else
				 {
					 g_MCtlInfo.bLaserOn = FALSE;
					 DbgPrt("LaserLed On/Off Control Answer:Off\n", g_hWndSIO);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC4:  //前端主电源控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 //ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bMainPowerOn = TRUE;
					 DbgPrt("Main Power On/Off Answer:On\n", g_hWndSIO);
				 }
				 else
				 {
					 g_MCtlInfo.bMainPowerOn = FALSE;
					 DbgPrt("Main Power On/Off Answer:Off\n", g_hWndSIO);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC5:  // 回传电压边界
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				 //ASSERT(pFrame->CmdSub == 0x10);

				 g_MCtlInfo.wTH_V_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//高压上限
				 g_MCtlInfo.wTH_V_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//高压下限
				 #if DBG_LEVEL != 0
				    sprintf(szTemp, "Upload HV Threshold=%d, %d\r\n", 
						             g_MCtlInfo.wTH_V_H,
									 g_MCtlInfo.wTH_V_L
							);
				 #endif
				 DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC6:  // 回传电流边界
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				g_MCtlInfo.wTH_I_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//电流上限
				g_MCtlInfo.wTH_I_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//电流下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload HV I Threshold=%d, %d\r\n", 
						            g_MCtlInfo.wTH_I_H,
									g_MCtlInfo.wTH_I_L
							);
				#endif
				DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC7:  // 回传WATERCOOL边界
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_WATERCOOLT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//上限
				g_MCtlInfo.wTH_WATERCOOLT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload WaterCool T Threshold=%d, %d\r\n", 
						            g_MCtlInfo.wTH_WATERCOOLT_H,
									g_MCtlInfo.wTH_WATERCOOLT_L
						   );
				#endif
				DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0xC8:  // 回传GANTRY温度边界
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_GANTRYT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//上限
				g_MCtlInfo.wTH_GANTRYT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload GANTRY T Threshold=%d, %d\r\n", 
						            g_MCtlInfo.wTH_GANTRYT_H,
									g_MCtlInfo.wTH_GANTRYT_L
						   );
				#endif
				DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0x04:  // 棒源消息
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				if(pFrame->CmdSub == 0x00){      //棒源正在伸出

					g_MCtlInfo.Rod_stat = outing;
					DbgPrt("ROD OUT In Process\r\n", g_hWndSIO);

				}else if(pFrame->CmdSub == 0x01){ //棒源伸出到位

					g_MCtlInfo.Rod_stat = out;
					DbgPrt("ROD OUT\r\n", g_hWndSIO);

				}else if(pFrame->CmdSub == 0x02){ //棒源正在收回或旋转
					
					if( pFrame->Dat[0] == 0x00 ){ //正在收回

						g_MCtlInfo.Rod_stat = ining;
						DbgPrt("ROD IN In Process\r\n", g_hWndSIO);

					}else if( pFrame->Dat[0] == 0x01){ //正在旋转
						
						g_MCtlInfo.Rod_stat = rolling;
						DbgPrt("ROD Rolling\r\n", g_hWndSIO);

					}else
						DbgPrt("Unknow ROD Status 1\r\n", g_hWndSIO);

				}else if(pFrame->CmdSub == 0x03){ //棒源收回到位

					g_MCtlInfo.Rod_stat = in;
					DbgPrt("ROD In\r\n", g_hWndSIO);

				}else if(pFrame->CmdSub == 0x04){ //棒源停止

					g_MCtlInfo.Rod_stat = stop;
					DbgPrt("ROD Stop Roll\r\n", g_hWndSIO);

				}else{							  //错误命令
					DbgPrt("Unknow Rod Status 2\r\n", g_hWndSIO);
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 PulseEvent(m_hEvtSIO_ROD);
			 break;

		case 0x01:  // septa消息
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if( pFrame->CmdSub == 0x00 ){      //正在伸出

					 g_MCtlInfo.septa_stat = outing;
					 DbgPrt("SEPTA OUT In Process\r\n", g_hWndSIO);

				}else if( pFrame->CmdSub == 0x01 ){ //伸出到位

					 g_MCtlInfo.septa_stat = out;
					 DbgPrt("SEPTA OUT\r\n", g_hWndSIO);

				}else if( pFrame->CmdSub == 0x02 ){ //正在缩回

					 g_MCtlInfo.septa_stat = ining;
					 DbgPrt("SEPTA IN in Process\r\n", g_hWndSIO);

				}else if( pFrame->CmdSub == 0x03 ){ //缩回到位

					 g_MCtlInfo.septa_stat = in;
					 DbgPrt("SEPTA IN\r\n", g_hWndSIO);

				}else if( pFrame->CmdSub == 0x04 ){ //正在停止

					 g_MCtlInfo.septa_stat = stop;
					 DbgPrt("SEPTA stop\r\n", g_hWndSIO);

				}else{							   //错误命令
					 DbgPrt("Unknow SEPTA Status 1\r\n", g_hWndSIO);
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Septa);
			 break;

		case 0x0A:  // 回传棒源转速
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 
				 g_MCtlInfo.wRod_rate = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Rod Rate=%d\r\n", g_MCtlInfo.wRod_rate);
				 #endif
				 DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_ROD);
			 break;

		case 0xE1:    //上传BED SPETA ROD开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
			    switch(pFrame->CmdSub)
				{
				case ADDR_ROD:
					 g_MCtlInfo.wRod_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload ROD SwitchSet=0x%4.4x\r\n", g_MCtlInfo.wRod_SwitchSet);
					 #endif
					 break;

				case ADDR_BED:
					 g_MCtlInfo.wBed_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload BED SwitchSet=0x%4.4x\r\n", g_MCtlInfo.wBed_SwitchSet);
					 #endif
					 break;

				case ADDR_SEPTA:
					 g_MCtlInfo.wSepta_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload SEPTA SwitchSet=0x%4.4x\r\n", g_MCtlInfo.wSepta_SwitchSet);
					 #endif
					 break;

				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload 0x%2.2x SwitchSet=0x%4.4x\r\n", pFrame->CmdSub, g_MCtlInfo.wSepta_SwitchSet);
					 #endif
					 break;
				}
				DbgPrt(szTemp, g_hWndSIO);
			
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Answer);
			 break;

		case 0x0C:  // 回传床坐标
			 EnterCriticalSection(&g_CLockMCtlInfo);

			     g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
			     g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Bed Pos, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posy);
				 #endif
				 DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0x14:  //回床床运动状态
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				if( pFrame->CmdSub == 0 )
				{
					g_MCtlInfo.Bed_stat = still;
					DbgPrt("Upload Bed Status:stop\r\n", g_hWndSIO);
				}
				else
				{
					g_MCtlInfo.Bed_stat = move;
					DbgPrt("Upload Bed Status:move\r\n", g_hWndSIO);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0x0D:  // 回传床步长
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     g_MCtlInfo.wBed_step = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				 #if DBG_LEVEL!=0
				    sprintf(szTemp, "Upload Bed Step Len: %d\r\n", g_MCtlInfo.wBed_step);
					DbgPrt(szTemp, g_hWndSIO);
				 #endif
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0xe5:   //控制床水平运动，但垂直高度不够, 新版协议
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 if( pFrame->CmdSub == 0x04 ){
					 g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Y Pos too low, Cann't Move In X, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posy);
					 #endif
					 DbgPrt(szTemp, g_hWndSIO);
				 }else{
					 DbgPrt("Unknow Message!\r\n", g_hWndSIO);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x6e:	// 床伸出/回缩到极限通告, 新版协议
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 if( pFrame->CmdSub == 0){
					 g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Move Parameter Error, x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posy);
					 #endif
					 DbgPrt(szTemp, g_hWndSIO);
				 }else{
					 DbgPrt("Unknow Message\r\n", g_hWndSIO);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 //PulseEvent(m_hEvtSIO_Bed);
			 break;
			 
		case 0x0F:  // 床伸出/回缩到极限通告, 旧版协议，应该不会出现
			 EnterCriticalSection(&g_CLockMCtlInfo);

				if( pFrame->CmdSub == 0x0 ){        //伸出到极限

					 g_MCtlInfo.Bed_stat = out_stop;
					 DbgPrt("Bed Out Limited\r\n", g_hWndSIO);

				}else{/*pFrame->CmdSub == 0x01*/     //收回到极限
					 
					 g_MCtlInfo.Bed_stat = in_stop;
					 DbgPrt("Bed In Limited\r\n", g_hWndSIO);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0xA1:    //上传床开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wBed_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload Bed SwitchSet\r\n", g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0xE7:	 //床开始找零, 新版协议
			 EnterCriticalSection(&g_CLockMCtlInfo);

				if( pFrame->CmdSub == 0x04 ){    
					g_MCtlInfo.Bed_stat = move;
					DbgPrt("Bed Begin Adjust To Zero\r\n", g_hWndSIO);
				}else{
					DbgPrt("Unknow Message\r\n", g_hWndSIO);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xA8:   //床运动开始
			 EnterCriticalSection(&g_CLockMCtlInfo);

			 	g_MCtlInfo.Bed_stat = move;
				DbgPrt("Bed Moveing\r\n", g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Bed);
			 break;

		case 0xA7:	 //床运动结束
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.Bed_stat = still;
				g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
			    #if DBG_LEVEL != 0
				    sprintf(szTemp, "Bed Moving Stop,x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		        #endif
				DbgPrt(szTemp, g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 //PulseEvent(m_hEvtSIO_Bed);
 			 SetEvent(m_hEvtSIO_Bed);

			 break;

		case 0x05:  //设置床坐标系统应答
			 EnterCriticalSection(&g_CLockMCtlInfo);
				//ASSERT(pFrame->CmdSub==0x00 || pFrame->CmdSub==0x01);
				if(pFrame->CmdSub == 0x00)
				{
					g_MCtlInfo.wBed_bRelateCoord = FALSE;
					#if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Cord. System:Absolute\r\n");
					#endif
				}
				else
				{
					g_MCtlInfo.wBed_bRelateCoord = TRUE;
					#if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Cord. system: Relate\r\n");
					#endif
				}
				DbgPrt(szTemp, g_hWndSIO);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_SetOK);
			 break;

		case 0x06:  //设置床步长应答
			 EnterCriticalSection(&g_CLockMCtlInfo);
				g_MCtlInfo.wBed_step = pFrame->Dat[0];
				#if DBG_LEVEL != 0
					sprintf(szTemp, "Bed Step Len: %d\r\n", g_MCtlInfo.wBed_step);
				#endif
				DbgPrt(szTemp, g_hWndSIO);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_SetOK);
			 break;

		case 0x0E:  // 床电机状态通告
			 DbgPrt("Upload Bed Machine StatueXXXXXXXXXX\r\n", g_hWndSIO);
			 break;

		case 0x71:    //上传SEPTA开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				g_MCtlInfo.wSepta_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload SEPTA SwitchSet\r\n", g_hWndSIO);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 PulseEvent(m_hEvtSIO_Septa);
			 break;

		case 0xB4:  // 板级查询超时报警
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case 0:
					 DbgPrt("Board Level Query OK\r\n", g_hWndSIO);
					 break;
				case ADDR_GANTRY:
					 DbgPrt("GANTRY Board Level Query Error\r\n", g_hWndSIO);
					 break;
				case ADDR_BED:
					 DbgPrt("BED Board Level Query Error\r\n", g_hWndSIO);
					 break;
				case ADDR_ROD:
					 DbgPrt("ROD Board Level Query Error\r\n", g_hWndSIO);
					 break;
				case ADDR_SEPTA:
					 DbgPrt("SEPTA Board Level Query Error\r\n", g_hWndSIO);
					 break;
				default:
					 DbgPrt("Unknow Board Level Query Error\r\n", g_hWndSIO);
					 break;					 
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xEE:  // 紧急停止通告
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 if(pFrame->CmdSub == 0x01)
				 {
					 g_MCtlInfo.alarm_ESTOP = alarm;
					 DbgPrt("EStop!!!!!!!!!!!!!!!!!!\r\n", g_hWndSIO);
					 
					 SetEvent(g_hEvtSIO_Alarm);
				 }
				 else
				 {
					 g_MCtlInfo.alarm_ESTOP = normal;
					 DbgPrt("EStop Reset\r\n", g_hWndSIO);
					 
					 SetEvent(g_hEvtSIO_EMCReset);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xB1:    //报警
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case ADDR_GANTRY:
					 g_MCtlInfo.alarm_Gantry = alarm;
					 g_MCtlInfo.ac_Gantry.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "GANTRY Alarm: 0x%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_ROD:
					 g_MCtlInfo.alarm_Rod = alarm;
					 g_MCtlInfo.ac_Rod.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "ROD Alarm: 0x%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_BED:
					 g_MCtlInfo.alarm_Bed = alarm;
					 g_MCtlInfo.ac_Bed.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Alarm: 0x%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_SEPTA:
					 g_MCtlInfo.alarm_Septa = alarm;
					 g_MCtlInfo.ac_Septa.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "SEPTA Alarm: 0x%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_HV:
					 g_MCtlInfo.alarm_HV = alarm;
					 g_MCtlInfo.ac_HV.code8    = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "HV Alarm: 0x%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "0x%2.2X Alarm: 0x%2.2X\r\n", pFrame->CmdSub, pFrame->Dat[0]);
					 #endif
					 break;
				}

				DbgPrt(szTemp, g_hWndSIO);
				SetEvent( g_hEvtSIO_Alarm);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		default:
			 TRACE("Receive UNKONW SIO Dat\r\n");
			 break;
	}//   end switch pFrame->CmdMain

#ifdef _ACQCENTER
	CACQCenterDlg* pMainWnd = (CACQCenterDlg*)AfxGetMainWnd();
	pMainWnd->ShowTips();
#endif

	return TRUE;
}

CSIO::CSIO()
{
    
	m_hCom = NULL;
	m_hEvtExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_bSIOMode_Test = FALSE;

	InitializeCriticalSection(&g_CLockMCtlInfo);
	InitializeCriticalSection(&m_CLockWrite);
	m_hEvtWrite = CreateEvent( NULL, TRUE, FALSE, NULL);

	g_MCtlInfo.bInitOK = FALSE;
	g_MCtlInfo.cInitStatus[0] = 0;
	g_MCtlInfo.cInitStatus[1] = 0;
	g_MCtlInfo.cInitStatus[2] = 0;
	g_MCtlInfo.cInitStatus[3] = 0;

	g_MCtlInfo.alarm_ESTOP = normal;
	g_MCtlInfo.alarm_Gantry = normal;
	g_MCtlInfo.alarm_Bed    = normal;
	g_MCtlInfo.alarm_Rod    = normal;
	g_MCtlInfo.alarm_Septa  = normal;
	g_MCtlInfo.alarm_HV     = normal;
	g_MCtlInfo.ac_Gantry.code8 = 0;
	g_MCtlInfo.ac_Bed.code8    = 0;
	g_MCtlInfo.ac_Rod.code8    = 0;
	g_MCtlInfo.ac_Septa.code8  = 0;
	g_MCtlInfo.ac_HV.code8     = 0;
	
	g_MCtlInfo.Rod_stat = unknow;
	g_MCtlInfo.wRod_SwitchSet =0;
	g_MCtlInfo.wRod_rate = 0;

	g_MCtlInfo.septa_stat = in;
	g_MCtlInfo.wSepta_SwitchSet = 0;

	g_MCtlInfo.Bed_stat  = still;
	g_MCtlInfo.wBed_SwitchSet = 0;
	g_MCtlInfo.wBed_posx  = -1;
	g_MCtlInfo.wBed_posy  = -1;
	g_MCtlInfo.wBed_step = -1;
	g_MCtlInfo.wBed_bRelateCoord = 1;
	
	g_MCtlInfo.wT_GANTRY    = -1;
	g_MCtlInfo.wT_WATERCOOL = -1;
	g_MCtlInfo.wT_Bed       = -1;
	g_MCtlInfo.wT_Rod       = -1;
	g_MCtlInfo.wT_Septa     = -1;
	g_MCtlInfo.wTH_GANTRYT_H = -1;
	g_MCtlInfo.wTH_GANTRYT_L = -1;
	g_MCtlInfo.wTH_I_H = -1;
	g_MCtlInfo.wTH_I_L = -1;
	g_MCtlInfo.wTH_V_H = -1;
	g_MCtlInfo.wTH_V_L = -1;
	g_MCtlInfo.wTH_WATERCOOLT_H = -1;
	g_MCtlInfo.wTH_WATERCOOLT_L = -1;
	g_MCtlInfo.wV_1 = -1;
	g_MCtlInfo.wV_2 = -1;
	g_MCtlInfo.wI_1 = -1;
	g_MCtlInfo.wI_2 = -1;
	
	g_MCtlInfo.bHVOn = FALSE;	
	g_MCtlInfo.bLaserOn = FALSE;    
	g_MCtlInfo.bMainPowerOn = FALSE;

	m_hEvtSIO_HandShake = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_Retry     = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_TVI       = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_ROD		= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_Bed       = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_Septa     = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_SetOK     = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEvtSIO_Answer    = CreateEvent(NULL, TRUE, FALSE, NULL);

	g_hEvtSIO_Alarm     = CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hEvtSIO_EMCReset  = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CSIO::~CSIO()
{
	CloseHandle(m_hEvtExit);

	LeaveCriticalSection(&g_CLockMCtlInfo);
	LeaveCriticalSection(&m_CLockWrite);

	DeleteCriticalSection(&g_CLockMCtlInfo);
	DeleteCriticalSection(&m_CLockWrite);
	CloseHandle(m_hEvtWrite);

	CloseHandle(m_hEvtSIO_HandShake);
	CloseHandle(m_hEvtSIO_Retry);
	CloseHandle(m_hEvtSIO_TVI);
	CloseHandle(m_hEvtSIO_ROD);
	CloseHandle(m_hEvtSIO_Bed);
	CloseHandle(m_hEvtSIO_SetOK);
	CloseHandle(m_hEvtSIO_Answer);

	CloseHandle(g_hEvtSIO_Alarm);
	CloseHandle(g_hEvtSIO_EMCReset);
}

//设置SIO工作模式
//bTest=TRUE  : 串口数据送交OWS处理
//bTest=FALSE :	串口数据由本机处理
void CSIO::SetSIOMode(BOOL bTest)
{
	m_bSIOMode_Test = bTest;
}

BOOL CSIO::OpenSIO(const char* szPort)
{
	DCB dcb;
	HANDLE hTemp; 

	if(m_hCom != NULL) return FALSE;

#if DBG_LEVEL != 0
	char szTemp[128];
	sprintf(szTemp, "Start Comm:%s, baudrate 9600!\r\n", szPort);
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
	if(SetupComm(hTemp,1024, 1024) == 0){
		 CloseHandle(hTemp);
		 return FALSE;
	}
	if(GetCommState(hTemp,&dcb) == 0){
	 	 CloseHandle(hTemp);
		 return FALSE;
	}
	dcb.BaudRate = CBR_9600; //CBR_57600;
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
    m_hThread = CreateThread( NULL,0, ThreadMonitorEMC,
				              this, 0,
							  &dwThreadID
							);

	dwThreadID = SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return TRUE;
}

BOOL CSIO::CloseSIO()
{
	SetEvent(m_hEvtExit);
	WaitForSingleObject(m_hThread, 10000/*ms*/);

	ResetEvent(m_hEvtExit);
	CloseHandle(m_hCom);

	m_hCom = NULL;
	m_hThread = NULL;

	return TRUE;
}

BOOL CSIO::WriteComm(LPCVOID pBuf, int len)
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
/*
//查询GANTRY温度
BOOL CSIO::QueryTGantry()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xB3;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x00;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
//查询棒源温度
/*BOOL CSIO::QueryTRod()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
/*
//查询SEPTA温度
BOOL CSIO::QueryTSepta()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
//查询床温度
BOOL CSIO::QueryTBed()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
/*
//查询水冷机温度
BOOL CSIO::QueryTWaterCool()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC0;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x20;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询高压状态、电流、电压
BOOL CSIO::QueryHVStatus()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_HV;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC1; 		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x00;			m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
//查询两路高压值
//DEL BOOL CSIO::QueryHV()
//DEL {
//DEL 	 DWORD dwRet;
//DEL 
//DEL 	 if(m_hCom == NULL) return FALSE;
//DEL 
//DEL 	 EnterCriticalSection(&m_CLockWrite);
//DEL 
//DEL 	 m_WFrame8.CheckSum = 0;
//DEL 	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
//DEL 	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
//DEL 	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
//DEL 	 m_WFrame8.attr    = 0;
//DEL 	 m_WFrame8.CmdMain = 0xC1; 		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
//DEL 	 m_WFrame8.CmdSub  = 0x10;			m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
//DEL 	 m_WFrame8.eof     = 0x7F;
//DEL 
//DEL      if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
//DEL 		 TRACE("Write Comm Error!\r\n");
//DEL 		 LeaveCriticalSection(&m_CLockWrite);
//DEL 		 return FALSE;
//DEL 	 }
//DEL 
//DEL 	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
//DEL 	 switch(dwRet)
//DEL 	 {
//DEL 		case WAIT_OBJECT_0:
//DEL 			 break;
//DEL 			 
//DEL 		default:
//DEL 			 LeaveCriticalSection(&m_CLockWrite);
//DEL 			 return FALSE;
//DEL 	 }
//DEL 
//DEL 	 LeaveCriticalSection(&m_CLockWrite);
//DEL 	 return TRUE;
//DEL }

/*
//查询两路电流值
BOOL CSIO::QueryI()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC1;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x20;	        m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_TVI, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
/*
//控制高压通、断
BOOL CSIO::Control_HVON(BOOL bON)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain =	0xC2;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
//控制激光灯通、断
BOOL CSIO::Control_LaserON(BOOL bON)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC3;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
/*
//控制前端主电源通、断
BOOL CSIO::Control_MainPowerON(BOOL bON)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain =	0xC4;		m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询高压上下限
BOOL CSIO::QueryThreshold_HV()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC5;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//设置高压上下限
BOOL CSIO::SetThreshold_HV(WORD upper, WORD lower)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC5;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 g_MCtlInfo.wTH_V_H = upper;
	 g_MCtlInfo.wTH_V_L = lower;

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询电流上限
BOOL CSIO::QueryThreshold_I()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC6;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//设置电流上下限
BOOL CSIO::SetThreshold_I(WORD upper, WORD lower)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC6;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 g_MCtlInfo.wTH_I_H = upper;
	 g_MCtlInfo.wTH_I_L = lower;

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询水冷机温度上下限
BOOL CSIO::QueryThreshold_TW()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;  m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;         m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC7;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//设置水冷机温度上下限
BOOL CSIO::SetThreshold_TW(WORD upper, WORD lower)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC7;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 g_MCtlInfo.wTH_WATERCOOLT_H = upper;
	 g_MCtlInfo.wTH_WATERCOOLT_L = lower;

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询GANTRY温度上下限
BOOL CSIO::QueryThreshold_TG()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;  m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;         m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC8;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//设置GANTRY温度上下限
BOOL CSIO::SetThreshold_TG(WORD upper, WORD lower)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC8;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 g_MCtlInfo.wTH_GANTRYT_H = upper;
	 g_MCtlInfo.wTH_GANTRYT_L = lower;

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}
*/
//设置棒源转速
BOOL CSIO::SetRodRate(WORD rate)
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0A;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = LOBYTE(rate);	m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = HIBYTE(rate);  m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_SetOK, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 g_MCtlInfo.wRod_rate = rate;
	 return TRUE;
}

//查询棒源状态
BOOL CSIO::QueryRodStatus()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 DbgPrt("Write Comm Error!\r\n", g_hWndSIO);
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_ROD, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询棒源转速
BOOL CSIO::QueryRodRate()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;	       m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 DbgPrt("Write Comm Error!\r\n", g_hWndSIO);
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_ROD, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询棒源开关量设置
BOOL CSIO::QueryRodSwitchSet()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_ROD;    m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	   m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;        m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;	       m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = ADDR_ROD;	   m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 DbgPrt("Write Comm Error!\r\n", g_hWndSIO);
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//控制棒源伸出并转动
BOOL CSIO::Control_RodOut()
{
	 if(m_hCom == NULL) return FALSE;
	 if(g_MCtlInfo.Rod_stat == out ) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x01;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_ROD, 
			                                ROD_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Rod_stat != out) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制棒源在已伸出的状态下开始旋转
BOOL CSIO::Control_RodRoll()
{
	 if(m_hCom == NULL) return FALSE;
	 //if(g_MCtlInfo.Rod_stat != out ) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x01;       m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_ROD, 
			                                ROD_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Rod_stat != rolling) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制棒源停止转动
BOOL CSIO::Control_RodStop()
{
	 if(m_hCom == NULL) return FALSE;
	 if(g_MCtlInfo.Rod_stat == out ) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x02;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_ROD, 
			                                ROD_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
//				 if( g_MCtlInfo.Rod_stat != stop ) bConti = TRUE;	//老PET
//				else bConti = FALSE;
//				break;
				 if( g_MCtlInfo.Rod_stat == out || g_MCtlInfo.Rod_stat ==stop) bConti = FALSE;	//新32环，李建伟
				 else bConti = TRUE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制棒源收回
BOOL CSIO::Control_RodIn()
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	 m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_ROD, 
			                                ROD_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Rod_stat != in) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制棒源紧急停止
BOOL CSIO::Control_RodEStop()
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	 m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xEE;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制床水平运动，绝对运动
BOOL CSIO::Control_BedMoveX(WORD len)
{
	 EnterCriticalSection(&m_CLockWrite);

	 if(g_MCtlInfo.wBed_bRelateCoord != FALSE)
	 {
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x01;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);	m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still ) bConti = FALSE;
				 else bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制床相对运动
BOOL CSIO::Control_BedMoveXRelate(WORD len, BOOL bOut)
{
	 EnterCriticalSection(&m_CLockWrite);

	 if(g_MCtlInfo.wBed_bRelateCoord != TRUE)
	 {
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 if( !bOut )  len |= 0x8000;

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x01;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);	m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still ) bConti = FALSE;
				 else bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制床Y绝对运动
BOOL CSIO::Control_BedMoveY(WORD len)
{
	 EnterCriticalSection(&m_CLockWrite);

	 if(g_MCtlInfo.wBed_bRelateCoord != FALSE)
	 {
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x02;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;			m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still) //stop ) 
					 bConti = FALSE;
				 else 
					 bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//控制床Y相对运动
BOOL CSIO::Control_BedMoveYRelate(WORD len, BOOL bUp)
{
	 EnterCriticalSection(&m_CLockWrite);

	 if(g_MCtlInfo.wBed_bRelateCoord != TRUE)
	 {
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 WORD wCurrPos = g_MCtlInfo.wBed_posy;

	 if( !bUp )  len |= 0x8000;

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x02;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;			m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still ) bConti = FALSE;
				 else bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

BOOL CSIO::Control_BedMoveXStep(BOOL bOut)
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x07;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 if(bOut)	m_WFrame12.CmdSub  = 0;  
	 else		m_WFrame12.CmdSub  = 1;		 
									    m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still ) bConti = FALSE;
				 else bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

BOOL CSIO::Control_BedMoveYStep(BOOL bUp)
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x08;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x01;
	 if(bUp)	m_WFrame12.Dat[0]  = 0;  
	 else		m_WFrame12.Dat[0]  = 1;		 
									    m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet;
		 dwRet = WaitForSingleObject( m_hEvtSIO_Bed,
									  BED_ANSWER_TIMEOUT
									);
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.Bed_stat == still ) bConti = FALSE;
				 else bConti = TRUE;
				 ResetEvent( m_hEvtSIO_Bed);

				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

BOOL CSIO::Control_BedStopX()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Bed,SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 ResetEvent( m_hEvtSIO_Bed);
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

BOOL CSIO::Control_BedStopY()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x02; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Bed,SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 ResetEvent( m_hEvtSIO_Bed);
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

BOOL CSIO::Control_BedStopAll()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_Bed,SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 ResetEvent( m_hEvtSIO_Bed);
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//设置床坐标方式：TRUE=>相对定位，FALSE=>绝对定位
BOOL CSIO::SetBedCoord(BOOL bRelate)
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x05;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 if(bRelate) m_WFrame9.CmdSub  = 0x01;
	 else        m_WFrame9.CmdSub  = 0x00;  
									   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }
	 
	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_SetOK,SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }
 
	 g_MCtlInfo.wBed_bRelateCoord = bRelate;

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//查询床开关量设置
BOOL CSIO::QueryBedSwitchSet()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;    m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	   m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;        m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;		   m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;        m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;        m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//查询床XY坐标
BOOL CSIO::QueryBedPos()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0C;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet = WaitForSingleObject(m_hEvtSIO_Bed, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//查询床运动步长
BOOL CSIO::QueryBedStep()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0D;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Bed, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询床运动状态
BOOL CSIO::QueryBedMoveStatus()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 DbgPrt("Write Comm Error!\r\n", g_hWndSIO);
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Bed, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//设置床运动步长
BOOL CSIO::SetBedStep(WORD step)
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x06;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(step); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet;
	 dwRet = WaitForSingleObject(m_hEvtSIO_SetOK, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 g_MCtlInfo.wBed_step = step;

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}
/*
//查询SEPTA状态
BOOL CSIO::QuerySeptaStatus()
{
	 DWORD dwRet;

	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 DbgPrt("Write Comm Error!\r\n", g_hWndSIO);
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 dwRet = WaitForSingleObject(m_hEvtSIO_Septa, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

//查询SEPTA开关量设置
BOOL CSIO::QuerySeptaSwitchSet()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_SEPTA;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

     if( !WriteComm(&m_WFrame12, m_WFrame12.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet = WaitForSingleObject(m_hEvtSIO_Answer, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//控制SEPTA伸出
BOOL CSIO::Control_SeptaOut()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x01;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_Septa, 
			                                SEPTA_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.septa_stat != out) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//控制SEPTA停止
BOOL CSIO::Control_SeptaStop()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x02;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x02;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_Septa, 
			                                SEPTA_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.septa_stat != stop) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//控制SEPTA缩回
BOOL CSIO::Control_SeptaIn()
{
	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

     if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 BOOL bConti = FALSE;
	 do{
		 DWORD dwRet = WaitForSingleObject( m_hEvtSIO_Septa, 
			                                SEPTA_ANSWER_TIMEOUT
										  );
		 switch(dwRet)
		 {
			case WAIT_OBJECT_0:
				 if( g_MCtlInfo.septa_stat != in) bConti = TRUE;
				 else bConti = FALSE;
				 break;
				 
			default:
				 LeaveCriticalSection(&m_CLockWrite);
				 return FALSE;
		 }
	 }while(bConti);

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}
*/
//发送数据包到BUCKET
//DEL BOOL CSIO::SendToBucket(const BYTE *pBuf, int nBufLen)
//DEL {
//DEL 	 EnterCriticalSection(&m_CLockWrite);
//DEL 
//DEL      if( !WriteComm( pBuf, nBufLen) ){
//DEL 		 TRACE("Write Comm Error!\r\n");
//DEL 		 LeaveCriticalSection(&m_CLockWrite);
//DEL 		 return FALSE;
//DEL 	 }
//DEL 
//DEL 	 LeaveCriticalSection(&m_CLockWrite);
//DEL 
//DEL 	 return TRUE;
//DEL }

//发送OWS测试数据
BOOL CSIO::SendTestDat(const BYTE *pBuf, int nBufLen)
{
	 EnterCriticalSection(&m_CLockWrite);

     if( !WriteComm( pBuf, nBufLen) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

BOOL CSIO::Dbg_GCPollingEnable(BOOL bEnable)
{
	 EnterCriticalSection(&m_CLockWrite);

     m_WFrame9.CheckSum = 0x00;
	 m_WFrame9.tAddr   = ADDR_GANTRY;  m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB2;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 if(bEnable) m_WFrame9.CmdSub = 1;		   
	 else        m_WFrame9.CmdSub = 0;
									   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat      = 0x03;        m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof      = 0x7F;

	 if( !WriteComm(&m_WFrame9, m_WFrame9.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);

	 return TRUE;
}

//查询机电控制电路初始化状态
BOOL CSIO::QueryInitStatus()
{
	 if(m_hCom == NULL) return FALSE;

	 EnterCriticalSection(&m_CLockWrite);

	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xA0; 		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x66;			m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

     if( !WriteComm(&m_WFrame8, m_WFrame8.len) ){
		 TRACE("Write Comm Error!\r\n");
		 LeaveCriticalSection(&m_CLockWrite);
		 return FALSE;
	 }

	 DWORD dwRet = WaitForSingleObject(m_hEvtSIO_HandShake, SIO_ANSWER_TIMEOUT);
	 switch(dwRet)
	 {
		case WAIT_OBJECT_0:
			 break;
			 
		default:
			 LeaveCriticalSection(&m_CLockWrite);
			 return FALSE;
	 }

	 LeaveCriticalSection(&m_CLockWrite);
	 return TRUE;
}

