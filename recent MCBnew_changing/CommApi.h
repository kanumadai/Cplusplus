//---------------------------------------------------------
//Model:	串口通信API模块 头文件
//FileName: CommApi.h
//Author:	akang
//Last Edit:  2004.3.21
//--------------------------------------------------------

#ifndef G_COMMAPI_H
#define G_COMMAPI_H

#include "StdAfx.h"

HANDLE OpenShmMaint232(DWORD portNo,DWORD baud);
int WriteCommandMaint232(HANDLE hcomm,int size,BYTE *command);
int ReadResultMaint232(HANDLE hcomm,BYTE *command);
int CloseShmMaint232(HANDLE hcomm);

BOOL WriteComm(HANDLE hComm, OVERLAPPED* pOL, void* pBuf, DWORD dwByteToWrite);
DWORD ReadComm(HANDLE hComm, OVERLAPPED* pOL, unsigned char* pBuf, DWORD dwByteToRead);

#endif   //G_COMMAPI_H