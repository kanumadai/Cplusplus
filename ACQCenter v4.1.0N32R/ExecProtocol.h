#ifndef __EXECPROTOCOL_H__
#define __EXECPROTOCOL_H__

#include "OWSProtocol.h"

typedef struct _exec_thread_param
{
   HANDLE hEvtFrameSkip;
   HANDLE hEvtFrameOK;
   HANDLE hEvtProtocolStop;
}EXEC_THREAD_PARAM;

DWORD WINAPI ThreadExecProtocol(LPVOID pParam);
DWORD WINAPI ThreadExecProtocolMonitor(LPVOID pData);  //thread send PET_MONITOR and PET_SYSINFO


#endif //__EXECPROTOCOL_H__