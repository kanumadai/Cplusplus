#ifndef __DBGPRT_H__
#define __DBGPRT_H__

#include "PrintSocket.h"

//#define DBG_LEVEL  0	 //No Debug Info
//#define DBG_LEVEL  1	 //Debug Out To Trace
#define DBG_LEVEL  2	 //Debug Out To EDIT WND
//#define DBG_LEVEL  3    //Debug Out To Socket

#if (DBG_LEVEL==0)
	#pragma message( "DbgPrt Level=0(NO Debug Message Out)" )
    #define DbgPrt(szBuf, hWnd) 
#elif (DBG_LEVEL==1)
	#pragma message( "DbgPrt Level=1(Debug Message Out to TRACE)" )
    #define DbgPrt     DbgPrtT
#elif (DBG_LEVEL==2)
	#pragma message( "DbgPrt Level=2(Debug Message Out to Edit Window)" )
    #define DbgPrt     DbgPrtW
#elif( DBG_LEVEL==3)
	#pragma message( "DbgPrt Level=3(Debug Message Out to Socket)" )
	#define DbgPrt     DbgPrtS
#else
	#pragma message( "DbgPrt Level Unknow" )
#endif  

void DbgPrtT(TCHAR* szBuf,HWND hWnd);
void DbgPrtW(TCHAR* szBuf,HWND hWnd);
void DbgPrtS(TCHAR* szBuf,HWND hWnd);

#endif