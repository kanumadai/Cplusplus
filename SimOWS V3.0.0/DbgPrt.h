#ifndef __DBGPRT_H__
#define __DBGPRT_H__

//#define DBG_LEVEL  0	 //No Debug Info
//#define DBG_LEVEL  1	 //Debug Out To Trace
#define DBG_LEVEL  2	 //Debug Out To EDIT WND

#if (DBG_LEVEL==0)
    #define DbgPrt(szBuf, hWnd) 
#elif (DBG_LEVEL==1)
    #define DbgPrt     DbgPrtT
#else
    #define DbgPrt     DbgPrtW
#endif  

void DbgPrtT(TCHAR* szBuf,HWND hWnd);
void DbgPrtW(TCHAR* szBuf,HWND hWnd);

#endif