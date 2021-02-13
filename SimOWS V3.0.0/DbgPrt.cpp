#include "stdafx.h"
#include "DbgPrt.h"

extern BOOL g_bDbgPrtEn;

//DbgPrt for TRACE
void DbgPrtT(TCHAR* szBuf,HWND hWnd)
{
	TRACE(szBuf);
}

void DbgPrtW(TCHAR* szBuf, HWND hWnd)
{
    if(!g_bDbgPrtEn) return;
	if(hWnd == NULL) return;
    
    int len = ::GetWindowTextLength(hWnd);
	if(len >= 16*1024) ::SendMessage(hWnd, EM_SETSEL, 0, len);
	else ::SendMessage(hWnd, EM_SETSEL, len, len);
    ::SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM) szBuf);
}
