
#ifndef EZUSBAPI_H
#define EZUSBAPI_H


#include "winioctl.h"
#include "ioctls.h"

HANDLE OpenUsbDevice();
BOOL ResetUsbDevice(HANDLE hDev);
BOOL CloseUsbDevice(HANDLE hDev);
int ReadUsbData(HANDLE hDev, void* pBuf, int nBufLen);
int WriteUsbData(HANDLE hDev, void* pBuf, int nBufLen);


#endif


