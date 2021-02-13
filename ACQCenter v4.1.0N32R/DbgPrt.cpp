#include "stdafx.h"
#include "DbgPrt.h"

extern BOOL g_bDbgPrtEn;

extern HWND g_hWndSIO;     
BOOL g_bSIODbgPrtEn=TRUE;
extern HWND g_hWndFH;		
BOOL g_bFHDbgPrtEn=TRUE;

#if (DBG_LEVEL==3)
extern CPrintSocket g_PrtSocket;
#endif

//DbgPrt for TRACE
void DbgPrtT(TCHAR* szBuf,HWND hWnd)
{
	TRACE(szBuf);
}

void DbgPrtW(TCHAR* szBuf, HWND hWnd)
{
    if(!g_bDbgPrtEn) return;
	if(hWnd == NULL) return;
    
	if( (hWnd==g_hWndSIO) && (!g_bSIODbgPrtEn) ) return;
	if( (hWnd==g_hWndFH) && (!g_bFHDbgPrtEn) ) return;
	
    int len = ::GetWindowTextLength(hWnd);
	if(len >= 16*1024) ::SendMessage(hWnd, EM_SETSEL, 0, len);
	else ::SendMessage(hWnd, EM_SETSEL, len, len);
    ::SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM) szBuf);
}

#if (DBG_LEVEL==3)
void DbgPrtS(TCHAR* szBuf,HWND hWnd)
{ 
	DbgPrtW(szBuf, hWnd);

	g_PrtSocket.SendToClient(szBuf, hWnd);
}
#endif

