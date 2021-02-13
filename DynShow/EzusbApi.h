
#ifndef EZUSBAPI_H
#define EZUSBAPI_H

enum COMM_PORT_SEL {
	COM0_INVALID = 0,
	COM1_PORT = 1,
	COM2_PORT = 2,
	USB_PORT = 3
};






#include "winioctl.h"
#include "ioctls.h"

HANDLE OpenUsbDevice();
BOOL CloseUsbDevice(HANDLE hDev);
int ReadUsbData(HANDLE hDev, void* pBuf, int nBufLen);
int WriteUsbData(HANDLE hDev, void* pBuf, int nBufLen);


#endif


