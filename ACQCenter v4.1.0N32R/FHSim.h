#ifndef __FHSIM_H__
#define __FHSIM_H__

#include "OWSProtocol.h"

class CFHSim;

typedef struct _fh_thread_param
{
	HANDLE hEvtExit;
	HANDLE hDevice;
	CFHSim*   pFH;
}FH_THREAD_PARAM;

typedef enum _fh_int_type
{
	int_ontime,
	int_oncount,
	int_ongate
}FH_INT_TYPE;

typedef enum _fh_scanmode
{
	scan_static,
	scan_t,
	scan_g
}FH_SCANMODE;

class CFHSim
{
public:
	//符合数据传输控制
	BOOL FHStart(BOOL bTestMode=FALSE, int TestBucket=0);
	BOOL FHStop();
	BOOL FHParamSet(DWORD dwIntValue, FH_INT_TYPE intType, FH_SCANMODE scanmode, DWORD dwFHRing, DWORD dwFHFov,
				    DWORD dwFHTimeNs, DWORD dwMinTimeNs, DWORD dwMaxTimeNs,
				    int nOutFmt, int nEnergMode
				   );

	//棒源位置信息接收电路控制
	BOOL Rs485Start();
	BOOL RS485Stop();

	//门控电路控制
	BOOL GateStart();
	BOOL GateStop();
	BOOL GateParamSet(const ACQ_PROTOCOL* pProtocol);
	BOOL GateParamSet(const void* pParam, int nParamLen);

	//心电数据采集控制
	BOOL EcgStart(int BufSize=4096);
	BOOL EcgStop();
	BOOL EcgParamSet(int SampleFreq, int factor);
	
	BOOL OpenDevice();
	BOOL CloseDevice();

	static void TimeToCount(double fTimeInUs, DWORD* pdwCnt);
	static void TimeToCount(DWORD dwTimeInUs, DWORD* pdwCnt);

	CFHSim();
	~CFHSim();

//protected:
public:
	int m_nCoinFmt;
	int m_nTestBucket;
	BOOL m_bTestMode;
	FH_THREAD_PARAM m_ThreadParam;
	HANDLE m_hThread;

	int    m_nAdcSampleFreq;
	int    m_nAdcAmpFactor;

	BOOL   m_bFHStarted;
	BOOL   m_bEcgStarted;
	BOOL   m_bRs485Started;
	BOOL   m_bGateStarted;

	HANDLE  m_hDevice;
	OVERLAPPED m_olIOCtl;
	HANDLE  m_hEvtExit;
};


#endif  //end define __FHSIM_H__