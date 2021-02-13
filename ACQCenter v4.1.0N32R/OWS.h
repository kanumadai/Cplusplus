#ifndef __OWS_H__
#define __OWS_H__

#include "OWSProtocol.h"

typedef struct _ows_thread_param
{
	CMySocket* pSocket;
	HANDLE hEvtExit;
}OWS_THREAD_PARAM;

typedef enum _bucket_mode{
	 mode_unknow = 0xFF,
	 mode_energy = 0xC5,
	 mode_time   = 0xC6,
	 mode_sd     = 0xC7,
	 mode_coin0  = 0xC8,
	 mode_coin1  = 0xC9
}BUCKET_DAT_MODE;

class COWS
{
public:
	BOOL Connect(const char* pszHostIP, int nPort, int RetryCnt=10);
	BOOL Close();
	//void AcqFrame(int nFrame);

public:
	BOOL SendSynTime();
	BOOL SendEcgDat(const BYTE* pBuf, DWORD dwBufLen);
	BOOL SendFHDat(const BYTE* pBuf, DWORD dwBufLen, int nBufID=0);
	BOOL SendLogin();
	BOOL SendPetInitOK(WORD obj);
	BOOL SendNACK(void);
	BOOL SendACK(void);
	BOOL SendFrameOK(void);
	BOOL SendFrameStart(WORD wFrameNum);
	BOOL SendTestRet(BYTE object, const BYTE* pBuf, int nBufLen);
	BOOL SendMonitor();
	BOOL SendSysInfo();
	BOOL SendAlarm(WORD wAlarm);

public:
	virtual void ExecPetInit(const BYTE* pBuf, int nBufLen);

public:
	const ACQ_PROTOCOL* ProtocolGet();
	BOOL ProtocolIsValid();
	BOOL ProtocolSave(const BYTE* pBuf, DWORD dwBufLen);
	BOOL ProtocolRelease();

	BOOL SetCurrFrame(int nFrameNum);
	void SetFrameStart(BOOL bStart);
	void SetFrameOKEvt();

protected:
	CRITICAL_SECTION m_CLockProtocol;
	BOOL m_bProtocolValid;
	ACQ_PROTOCOL m_AcqProtocol;

	ACQ_SYSINFO m_SysInfo;   //PET_SYSINFO
	ACQ_MONITOR m_Monitor;   //PET_MONITOR

	int  m_nCurrFrame;		 //当前帧
	BOOL m_bFrameFirstDat;
	DWORD m_dwCurrFrameStartTime;   //当前帧开始时紒E
	BOOL m_bFrameStart;
	DWORD m_dwCurrEvtCnt_Frame;
	DWORD m_dwCurrEvtCnt_All;

protected:
	HANDLE m_hThread;
	DWORD  m_dwThreadID;
	OWS_THREAD_PARAM m_ThreadParam;
	HANDLE m_hEvtExit;

	CRITICAL_SECTION m_CLockSend;
	OWS_HEAD  m_owsSendHead;

	BOOL PetInit_Acs(INIT_ITEM* pBuf, int nItemCnt);
	BOOL PetInit_Gantry(INIT_ITEM *pItem, int nItemCnt);
//	BOOL PetInit_Bucket(INIT_ITEM *pItem, int nItemCnt);

public:
	COWS();
	~COWS();
};


#endif  //end define __OWS_H__