//---------------------------------------------------------
//Model:	串口通信API模块
//FileName: CommApi.cpp
//Author:	akang
//Last Edit:  2004.3.21
//Discription:
//		HANDLE OpenShmMaint232(int portNo,DWORD baud);
//		int WriteCommandMaint232(HANDLE hcomm,int size,BYTE *command);
//		int WINAPI ReadResultMaint232(HANDLE hcomm,BYTE *command);
//		int WINAPI CloseShmMaint232(HANDLE hcomm);
//--------------------------------------------------------
#include "stdafx.h"
#include "CommApi.h"

//--------------------【 连接维护信箱】-------------------//
HANDLE OpenShmMaint232(DWORD portNo,DWORD baud)
{
	DCB dcb;
	HANDLE hTemp; 
	COMMTIMEOUTS CommTimeouts;    //超时选项
	CString strport;

	if(portNo == 0) strport = "COM1";
	else if(portNo == 1) strport = "COM2";
		 else return INVALID_HANDLE_VALUE;

	hTemp = CreateFile(strport, GENERIC_READ | GENERIC_WRITE, 0,
		                NULL,OPEN_EXISTING,
					    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
					    NULL);
    if(hTemp == INVALID_HANDLE_VALUE) {
		//AfxMessageBox("Comm Open Error 0 !");
		return INVALID_HANDLE_VALUE;
	}
	if(SetCommMask(hTemp,EV_RXCHAR) == 0){
   	    CloseHandle(hTemp);
		AfxMessageBox("Comm Open Error 1 !");
		return INVALID_HANDLE_VALUE;
	}
	if(SetupComm(hTemp,1024*10, 1024*10) == 0){
 	    CloseHandle(hTemp);
		AfxMessageBox("Comm Open Error 2 !");
		return INVALID_HANDLE_VALUE;
	}
	if(GetCommState(hTemp,&dcb) == 0){
  	    CloseHandle(hTemp);
		AfxMessageBox("Comm Open Error 3 !");
		return INVALID_HANDLE_VALUE;
	}

	dcb.BaudRate = baud; //CBR_57600;
	dcb.fParity  = FALSE;  //disable parity check
	dcb.ByteSize = 8; 
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
    dcb.XonChar = 0x11;
    dcb.XoffChar = 0x13;
    dcb.fDtrControl = 1; //DTR_CONTROL_DISABLE; 
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fRtsControl = 1;
    dcb.XonLim  =  100;
    dcb.XoffLim = 100;

	if(SetCommState(hTemp,&dcb) == 0){
	 	 CloseHandle(hTemp);
		 AfxMessageBox("Comm Open Error 5");
		 return INVALID_HANDLE_VALUE;
	}
    if(PurgeComm(hTemp,PURGE_TXABORT | PURGE_RXABORT 
	 	| PURGE_TXCLEAR | PURGE_RXCLEAR) == 0){
		 CloseHandle(hTemp);
		 AfxMessageBox("Comm Open Error 6");
		 return INVALID_HANDLE_VALUE;
	}

	CommTimeouts.ReadIntervalTimeout = 0;	// 间隔时间应该小于，最长空等时间
	CommTimeouts.ReadTotalTimeoutConstant = 300;  // akang  2003.3.05   <.... 500
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;

	if(!SetCommTimeouts(hTemp,&CommTimeouts)) {
		AfxMessageBox("设置超时出错");	
		return INVALID_HANDLE_VALUE;
	}

	return hTemp;
}

//--------------------【 断开维护信箱】-------------------//
int CloseShmMaint232(HANDLE hcomm)
{
	if(hcomm != INVALID_HANDLE_VALUE) {
		CloseHandle(hcomm);
		hcomm = INVALID_HANDLE_VALUE;
	}
	return TRUE;
}

//--------------------【 写维护命令】-------------------//
int WriteCommandMaint232(HANDLE hcomm,int size,BYTE *command)
{
	OVERLAPPED ol;

	if(size == 0) return FALSE;
	if(hcomm == INVALID_HANDLE_VALUE) return 0xffff;

	ol.hEvent = CreateEvent(NULL, TRUE, FALSE,"SENDDATA");//"SENDDATA"---NULL

	if(!WriteComm(hcomm, &ol, command, size)){
		CloseHandle(ol.hEvent);
		AfxMessageBox("WriteComm Error!");		
		return FALSE;
	}
	
	CloseHandle(ol.hEvent);
	return TRUE;
}

//--------------------【 读取数据，命令】-------------------//
int ReadResultMaint232(HANDLE hcomm,BYTE *command)
{
	OVERLAPPED ol;
	int	num;
	char message[20];
	DWORD msg;

	ol.hEvent = CreateEvent(NULL, TRUE, FALSE, "RECDATA");
	
	if(hcomm == INVALID_HANDLE_VALUE) {
		CloseHandle(ol.hEvent);
		return 0xffff;
	}
	if(hcomm == NULL) {
		CloseHandle(ol.hEvent);
		return 0xffff;
	}
	num = ReadComm(hcomm, &ol, command, 4000);     //缓冲区开为4000 个字节
	CloseHandle(ol.hEvent);

	if( !num ){  		  
		  msg=GetLastError();
		  if(0 == msg) { return num;  }			//akang quan ??????????
		  if(ERROR_IO_PENDING != msg) {			  
				  _itoa(msg,message,10);
				  AfxMessageBox(message);
				  return 0xffff;			 
		  } 		  
		  return 0;
	}
	else {
		return num;
	}
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------//
BOOL WriteComm(HANDLE hComm, OVERLAPPED* pOL, void* pBuf, DWORD dwByteToWrite)
{
	 if(hComm == NULL) return FALSE;
	 if(pBuf  == NULL) return FALSE;
	 if(pOL == NULL || pOL->hEvent == NULL) return FALSE;
	 if(dwByteToWrite == 0) return FALSE;

	 int nRet;
	 DWORD dwWrited=0,dwError;	//akangquan    dwWrited=0

	 ResetEvent(pOL->hEvent);
	 pOL->Offset     = 0;
	 pOL->OffsetHigh = 0;
	 nRet = WriteFile(hComm, pBuf, dwByteToWrite, &dwWrited, pOL);
	 if(!nRet){
		 switch(dwError=GetLastError()){
			 case ERROR_IO_PENDING:
				  nRet = GetOverlappedResult(hComm, pOL, &dwWrited, TRUE) ; 
				  if(!nRet){
					  ResetEvent(pOL->hEvent);
					  return FALSE;
				  }
				  break;
			 default:
	 		      ResetEvent(pOL->hEvent);
				  return FALSE;
		 }//end switch
	 }//end if	     

	 ResetEvent(pOL->hEvent);

	 return TRUE;
}

//
DWORD ReadComm(HANDLE hComm, OVERLAPPED* pOL, unsigned char* pBuf, DWORD dwByteToRead)
{
	 if(hComm == NULL) return FALSE;
	 if(pBuf  == NULL) return FALSE;
	 if(pOL == NULL || pOL->hEvent == NULL) return FALSE;

	 int nRet;
	 DWORD dwReaded=0, dwError;	 //dwReaded = 0; akangquan

	 ResetEvent(pOL->hEvent);
	 pOL->Offset     = 0;
	 pOL->OffsetHigh = 0;
	 nRet = ReadFile(hComm, pBuf, dwByteToRead, &dwReaded, pOL);
	 if(!nRet){
		 switch(dwError=GetLastError()){
			 case ERROR_IO_PENDING:
				  nRet = GetOverlappedResult(hComm, pOL, &dwReaded, TRUE) ; 
				  if(!nRet){
					  ResetEvent(pOL->hEvent);					  
					  return FALSE;
				  }
				  break;
			 default:
	 		      ResetEvent(pOL->hEvent);
				  return FALSE;
		 }//end switch
	 }//end if	

	 ResetEvent(pOL->hEvent);

	 return dwReaded;
}