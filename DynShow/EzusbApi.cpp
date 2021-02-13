

#include "stdafx.h"
#include "EzusbApi.h"


HANDLE OpenUsbDevice()
{
	HANDLE hDev;

	hDev = CreateFile( "\\\\.\\BUCKDEMO_0", 0, //GENERIC_READ | GENERIC_WRITE,
		               0, //FILE_SHARE_READ | FILE_SHARE_WRITE,
					   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
					   NULL
					 );
	if( hDev == INVALID_HANDLE_VALUE ){
		DWORD dwErr = GetLastError();
		return NULL;
	}

	return hDev;  		               
}

BOOL CloseUsbDevice(HANDLE hDev)
{
	if( hDev == NULL ) return FALSE;

	return CloseHandle(hDev);  
}

int ReadUsbData(HANDLE hDev, void* pBuf, int nBufLen)
{
	if( hDev == NULL ) return 0;
	if( nBufLen > MAX_TRANS_SIZE ) return 0;

	DWORD dwReaded;
	ULONG pipeNum = 0;
	
	BOOL bRet = DeviceIoControl( hDev, IOCTL_READ, 
		                         NULL, 0,	  //IN BUF
              					 pBuf, nBufLen,				  //OUT BUF
			            		 &dwReaded,
            					 NULL
			                   );
	if( !bRet )	return 0;

	return dwReaded;
}

int WriteUsbData(HANDLE hDev, void* pBuf, int nBufLen)
{
	if( hDev == NULL ) return 0;
	if( nBufLen > MAX_TRANS_SIZE )  return 0;

	DWORD dwWrited;
	ULONG pipeNum = 1;
	
	BOOL bRet = DeviceIoControl( hDev, IOCTL_WRITE, 
		                         pBuf, nBufLen,	  //IN BUF
              					 NULL, 0,				  //OUT BUF
			            		 &dwWrited,
            					 NULL
			                   );
	if( !bRet ) 	return 0;

	return dwWrited;
}

