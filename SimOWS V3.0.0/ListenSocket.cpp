// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "ListenSocket.h"
#include "SimOWSDlg.h"
//#include ".\BlkDraw\DatFmt.h"
#include "DatFmt.h"

#include "DbgPrt.h"
#include "..\AcqCenter\OWSProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRITICAL_SECTION g_lockFile; 
extern CFile g_saveFile;
extern HANDLE g_hEvtStopWriteFile;

extern BOOL g_bFH;
extern HANDLE g_hEvtExit;
extern HANDLE g_hFHEvt[4];
extern HANDLE g_hFHBuf[4];
extern DWORD  g_dwBufLen[4];   

extern DWORD g_dwEvtCnt;

extern HWND g_hWndOWS;

extern DWORD g_dwContCoinCnt;
extern DWORD g_dwDelayCoinCnt;

DWORD WINAPI ReceiveThread(LPVOID pData);
DWORD WINAPI WriteThread(LPVOID pData);



/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
	if( m_nulSock.m_hSocket != INVALID_SOCKET){
		m_nulSock.Close();
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	SOCKADDR sock_addr;
	int  addr_len;

	if( m_nulSock.m_hSocket != INVALID_SOCKET ){
		m_nulSock.Close();
		m_nulSock.m_hSocket = INVALID_SOCKET;
	}

	addr_len = sizeof(SOCKADDR);
	if(!Accept(m_nulSock,&sock_addr,&addr_len)){
       addr_len = GetLastError();
	   ASSERT(FALSE);
	   m_nulSock.m_hSocket = INVALID_SOCKET;
	   return;
	}    
	BOOL bRet = m_nulSock.AsyncSelect(0);
	DWORD arg=0;
	bRet = m_nulSock.IOCtl(FIONBIO, &arg);

	DbgPrt("ACS Connected!\r\n", g_hWndOWS);


	DWORD dwID;
    CreateThread(NULL,0, ReceiveThread,
	             this, 0, &dwID);
}

BOOL CListenSocket::IsConnected()
{
	if( m_nulSock.m_hSocket == INVALID_SOCKET) return FALSE;
	else return TRUE;
}

void CListenSocket::CloseClient()
{
	if( m_nulSock.m_hSocket == INVALID_SOCKET ) return;

	m_nulSock.Close();
	//m_nulSock.m_hSocket = INVALID_SOCKET;
	return;
}

COWSSock* CListenSocket::GetClient()
{
   return &m_nulSock;
}

BOOL CListenSocket::SendDatToClient(const void *pBuf, int nLen)
{
   if(m_nulSock.m_hSocket == INVALID_SOCKET ) return FALSE;
   return m_nulSock.Send(pBuf, nLen);
}

typedef union _coin_dat
{
	DWORD dwDat;
	struct{
		DWORD APos : 14;
		DWORD BPos : 14;
		DWORD energy_flag : 1;
		DWORD time_flag : 1;
		DWORD datfmt_flag : 2;
	};
}COIN_DAT;

DWORD WINAPI ReceiveThread(LPVOID pData)
{
	COWSSock* pSock;
	CListenSocket* pServer;
	OWS_HEAD head;
	COIN_DAT* pCoinDat;

	char szTemp[128];

	CSimOWSDlg* pDlg = (CSimOWSDlg* )AfxGetMainWnd();

	pServer = (CListenSocket*)pData;
	pSock = pServer->GetClient();

	int BufID = 0;

	do{
		if( pSock->Receive(&head, sizeof(head)) != sizeof(head) )
		{																	   
			//connect closed
			DbgPrt("Connection to ACS lost!\r\n", g_hWndOWS);	
			pServer->CloseClient();
			
			AfxGetMainWnd()->KillTimer(100);
			pDlg->m_bFHStart = FALSE;
			SetEvent(g_hEvtStopWriteFile);

			break;
		}

		if( head.cFlag[0] != 0x7e || head.cFlag[1] != 0x7e || head.cFlag[2] != 0x7e)
		{
			DbgPrt("Receive ACQ PAKCET HEAD ERROR!\r\n", g_hWndOWS);
			pServer->CloseClient();
			break;
		}

		if( head.dwDatLen - sizeof(head) != 0 )
		{
			int RecDat,LastError;
			RecDat = pSock->Receive(g_hFHBuf[BufID%4], head.dwDatLen-sizeof(head) );
			if(  RecDat != int(head.dwDatLen-sizeof(head)) )
			{
				//receive data error
				LastError=WSAGetLastError();
				sprintf(szTemp, "Recv Dat From ACQ Error, ErrCode=%d, Dat To Recv=%d, Received=%d\r\n", LastError, head.dwDatLen-sizeof(head), RecDat);
				DbgPrt(szTemp, g_hWndOWS);
				pServer->CloseClient();
				break;
			}
		}

		switch(head.Cmd)
		{
		case PET_LISTMODE:
			 g_dwBufLen[BufID%4] = head.dwDatLen - sizeof(head);
			 if( g_bFH )
				sprintf(szTemp, "Recv Listmode data, BufID=%d, Len=%8.8x\r\n", BufID%4, g_dwBufLen[BufID%4]);
			 else
				sprintf(szTemp, "Recv Bucket data, BufID=%d, Len=%8.8x\r\n", BufID%4, g_dwBufLen[BufID%4]);
			 DbgPrt(szTemp, g_hWndOWS);

			 if( g_bFH ){
				pCoinDat = (COIN_DAT*)g_hFHBuf[BufID%4];
				for(DWORD EvtCnt=0; EvtCnt<g_dwBufLen[BufID%4]/4; EvtCnt++){
					if( pCoinDat[EvtCnt].datfmt_flag != 0) continue;
					if( pCoinDat[EvtCnt].time_flag == 0)
						g_dwDelayCoinCnt++;
					else
						g_dwContCoinCnt++;
				}
				if( g_dwDelayCoinCnt+g_dwContCoinCnt != 0 ){
					double allEvt = g_dwDelayCoinCnt+g_dwContCoinCnt;
					sprintf(szTemp, "立即符合记数:%d  延迟符合记数:%d  延迟符合率:%f%%\r\n\r\n",
						     g_dwContCoinCnt, g_dwDelayCoinCnt, (double(g_dwDelayCoinCnt))/allEvt*100
						   );
					DbgPrt(szTemp, g_hWndOWS);
				}
			 }

			 SetEvent(g_hFHEvt[BufID%4]);
			 break;

		case PET_FRAMESTART:
			 sprintf(szTemp, "ACS=>PET_FRAMESTART, Frame=%d\r\n", *(WORD*)g_hFHBuf[BufID%4]);
			 DbgPrt(szTemp, g_hWndOWS);
			 break;

		case PET_FRAMEOK:
			 DbgPrt("ACS=>PET_FRAMEOK\r\n", g_hWndOWS);
			 break;

		case PET_ACK:
			 DbgPrt("ACS=>PET_ACK\r\n", g_hWndOWS);
			 AfxGetMainWnd()->KillTimer(100);
			 pDlg->m_bFHStart = FALSE;
			 SetEvent(g_hEvtStopWriteFile);

			 pDlg->SendPetAllProtocolOver();
			 break;

		case PET_NACK:
			 DbgPrt("ACS=>PET_NACK\r\n", g_hWndOWS);
			 AfxGetMainWnd()->KillTimer(100);
			 pDlg->m_bFHStart = FALSE;
			 SetEvent(g_hEvtStopWriteFile);

			 pDlg->SendPetAllProtocolOver();
			 break;

		case PET_LOGIN:
			 DbgPrt("ACS=>PET_LOGIN\r\n", g_hWndOWS);
			 pDlg->SendInitACS();
			 break;


		case PET_SYNTIME:
			{
			 DbgPrt("ACS=>PET_SYNTIME\r\n", g_hWndOWS);
			 PET_SYSTEMTIME * pPetTime = (PET_SYSTEMTIME*)g_hFHBuf[BufID%4];
			 sprintf(szTemp, "ACS Time: %d-%d-%d %d:%d:%d TickCount=%d\r\n",
				                        pPetTime->wYear, pPetTime->wMonth, pPetTime->wDay,
										pPetTime->wHour, pPetTime->wMinute, pPetTime->wSecond,
										pPetTime->dwTickCount
				    );
			 DbgPrt(szTemp, g_hWndOWS);
			}
			 break;

		case PET_INITOK:
			 sprintf(szTemp, "ACS=>PET_INITOK, obJ=%4.4x\r\n", *(WORD*)g_hFHBuf[BufID%4]);
			 DbgPrt(szTemp, g_hWndOWS);
			 break;

		case PET_TESTRET:
			{
			 BYTE* testObj = (BYTE*)g_hFHBuf[BufID%4];
			 sprintf(szTemp, "\r\nACS=>PET_TESTRET,Obj=%d\r\n",testObj[0]);
			 DbgPrt(szTemp, g_hWndOWS);
			 switch(testObj[0]){
			 case INIT_OBJ_BUCKET_ALL:   //bucket
			 	  break;
			 case INIT_OBJ_GANTRY: case INIT_OBJ_ROD: 
			 case INIT_OBJ_SEPTA: case INIT_OBJ_BED: 
			 case INIT_OBJ_YE1: case INIT_OBJ_YE2:
				  Process_GantryDat( (SIO_FRAME12*)(testObj+1) );
			 	  break;
			 default:
				  break;
			 }
			}
			 break;
		case PET_BEAT:
			 DbgPrt("\r\nACS=>PET_BEAT\r\n", g_hWndOWS);
			 break;
		case PET_MONITOR:
			{
			 ACQ_MONITOR* pInfo = (ACQ_MONITOR*)g_hFHBuf[BufID%4];
			 pDlg->ShowPetMonitor(pInfo, NULL);
			}
/*			 DbgPrt("\r\nACS=>PET_MONITOR\r\n", g_hWndOWS);
			 {
			 ACQ_MONITOR* pInfo = (ACQ_MONITOR*)g_hFHBuf[BufID%4];
			 sprintf(szTemp, "-- CurrFrame=%d, FrameClass=%d\r\n", pInfo->cCurrFrameNumber, pInfo->cCurrFrameClass);
			 DbgPrt(szTemp, g_hWndOWS);
			 sprintf(szTemp, "-- RodRate=%d\r\n", pInfo->fWobbleTilt);
			 DbgPrt(szTemp, g_hWndOWS);
			 sprintf(szTemp, "-- CoinEventCnt=%d\r\n", pInfo->dwNetTrues);
			 DbgPrt(szTemp, g_hWndOWS);
			 sprintf(szTemp, "-- cFlag=%8.8x\r\n", pInfo->cFlag);
			 DbgPrt(szTemp, g_hWndOWS);
			 }
*/
			 break;
		case PET_SYSINFO:
			{
			 ACQ_SYSINFO* pInfo = (ACQ_SYSINFO*)g_hFHBuf[BufID%4];
			 pDlg->ShowPetMonitor(NULL, pInfo);
			}
/*			 DbgPrt("\r\nACS=>PET_SYSINFO\r\n", g_hWndOWS);
			 {
			 ACQ_SYSINFO* pInfo = (ACQ_SYSINFO*)g_hFHBuf[BufID%4];
			 sprintf(szTemp, "-- Bed Y Pos=%f cm, X Pos=%f cm\r\n", pInfo->fBed_Elevation, pInfo->fInitBed_Position);
			 DbgPrt(szTemp, g_hWndOWS);
			 }
*/			 break;
		case PET_ALARM:
			 sprintf(szTemp, "\r\nACS=>PET_ALARM,Alarm Code=%4.4X\r\n", *(WORD*)g_hFHBuf[BufID%4]);
			 DbgPrt(szTemp, g_hWndOWS);
			 break;
		default:
			 DbgPrt("ACS=>Unknow Head\r\n", g_hWndOWS);
			 break;
		}

		BufID++; 
	}while(TRUE);

	DbgPrt("Exit Receive Thread\r\n", g_hWndOWS);
	return 0;
}

DWORD WINAPI WriteThread(LPVOID pData)
{
	HANDLE hEvtExit[5];
	char szTemp[8192];

	FH_ROWDAT* pBucketBuf;
	int j;
	pBucketBuf = (FH_ROWDAT*)GlobalAlloc(GMEM_FIXED, 32*1024*1024);
	ASSERT(pBucketBuf != NULL);

	CSimOWSDlg* pDlg = (CSimOWSDlg*)AfxGetMainWnd();

	DbgPrt("Enter SaveFile Thread!\r\n", g_hWndOWS);

	hEvtExit[0] = g_hFHEvt[0];
	hEvtExit[1] = g_hFHEvt[1];
	hEvtExit[2] = g_hFHEvt[2];
	hEvtExit[3] = g_hFHEvt[3];
	hEvtExit[4] = g_hEvtExit;

	DWORD dwWait;
	DWORD bufID;
	BOOL bContinue = TRUE;

	do{
		dwWait = WaitForMultipleObjects(5,hEvtExit,FALSE,INFINITE);
		switch(dwWait)
		{
		case WAIT_OBJECT_0:	 //FH BUF 0
		case WAIT_OBJECT_0+1: //FH BUF 1
		case WAIT_OBJECT_0+2: //FH BUF 2
		case WAIT_OBJECT_0+3: //FH BUF 3
			 bufID = dwWait - WAIT_OBJECT_0;
			 ResetEvent(g_hFHEvt[bufID]);
			 
			 if( !pDlg->m_bFHStart ) break;

			 if( g_bFH )
			 {	//采集的是符合数据
				EnterCriticalSection(&g_lockFile);
					if(g_saveFile.m_hFile == CFile::hFileNull )
					{
						LeaveCriticalSection(&g_lockFile);
						break;
					}
					g_saveFile.Write(g_hFHBuf[bufID], g_dwBufLen[bufID]);
				LeaveCriticalSection(&g_lockFile);
			 }
			 else
			 {	//采集的是单路BUCKET数据
				FH_ROWDAT* pInBuf = (FH_ROWDAT*)g_hFHBuf[bufID];

				//valid data cnt
				int validCnt=0;
				int energyCnt=0;
				int dispCnt=0;
				for(DWORD i=0; i<g_dwBufLen[bufID]/4; i++)
				{
					if(pInBuf[i].valid){
						pBucketBuf[validCnt] = pInBuf[i];
						validCnt++;
						if( dispCnt<10 ){
							sprintf(szTemp, "ROWDat %8.8x =>(time=%d,strip=%d,energy flag=%d,valid=%d,block=%d,bucket=%d\r\n",
											 pBucketBuf[validCnt].dwDat,
				   							 pBucketBuf[validCnt].time,  pBucketBuf[validCnt].strip, pBucketBuf[validCnt].energy,
											 pBucketBuf[validCnt].valid, pBucketBuf[validCnt].block, pBucketBuf[validCnt].bucket 
								   );
							DbgPrt(szTemp, g_hWndOWS);
							dispCnt++;
						}

						if( pInBuf[i].energy==1 ) energyCnt++;
					}
				}
				j=validCnt;

				if(validCnt==0)
				{
					DbgPrt("BUCKET没有有效事例\r\n\r\n", g_hWndOWS);
				}else{
					sprintf(szTemp, "有效事件数=%d, 光电峰记数=%d\r\n\r\n", validCnt, energyCnt);
					DbgPrt(szTemp, g_hWndOWS);
				}

				EnterCriticalSection(&g_lockFile);
					if(validCnt==0 || g_saveFile.m_hFile == CFile::hFileNull )
					{
						LeaveCriticalSection(&g_lockFile);
						break;
					}
					g_saveFile.Write(pBucketBuf, validCnt*4);
				LeaveCriticalSection(&g_lockFile);

				CWndDisp* pWnd = ((CSimOWSDlg*)AfxGetMainWnd())->GetDispWnd();
				if( pWnd != NULL){
					pWnd->ShowDat(pBucketBuf, validCnt*4);
				}
			 }
			 sprintf(szTemp, "Write Data to Disk, BufID=%d\r\n\r\n", bufID);
			 DbgPrt(szTemp, g_hWndOWS);

			 if(g_bFH)
				g_dwEvtCnt += 	(g_dwBufLen[bufID]/4);
			 else
				g_dwEvtCnt += j; 
			 break;

		case WAIT_OBJECT_0+4:
			 bContinue = FALSE;
			 ResetEvent(g_hEvtExit);
			 break;
		}
	}while( bContinue );

	DbgPrt("Exit SaveFile Thread!\r\n", g_hWndOWS);
	
	GlobalFree( pBucketBuf );

	return 0;
}
