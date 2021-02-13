#ifndef __FH_H__
#define __FH_H__

#include "OWSProtocol.h"

#ifndef _FH_SIM

class CFH;

typedef struct _fh_thread_param
{
	HANDLE hEvtExit;
	HANDLE hDevice;
	CFH*   pFH;
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

class CFH
{
public:
	//�������ݴ������
	BOOL FHStart(BOOL bTestMode=FALSE, int TestBucket=0);
	BOOL FHStop();
	BOOL FHParamSet(DWORD dwIntValue, FH_INT_TYPE intType, FH_SCANMODE scanmode, DWORD dwFHRing, DWORD dwFHFov,
				    DWORD dwFHTimeNs, DWORD dwMinTimeNs, DWORD dwMaxTimeNs,
				    int nOutFmt, int nEnergMode
				   );

	//��·�Լ����
	BOOL SelfTestStop();
	BOOL SelfTestStart(int nSpeed);

	//��Դλ����Ϣ���յ�·����
	BOOL Rs485Start();
	BOOL RS485Stop();

	//�ſص�·����
	BOOL GateStart();
	BOOL GateStop();
	BOOL GateParamSet(const ACQ_PROTOCOL* pProtocol);
	BOOL GateParamSet(const void* pParam, int nParamLen);

	//�ĵ����ݲɼ�����
	BOOL EcgStart(int BufSize=4096);
	BOOL EcgStop();
	BOOL EcgParamSet(int SampleFreq, int factor);
	
	//����λ����Чλͳ��
	BOOL VECntStart(int nBuckID);
	BOOL VECntStop();
	BOOL VECntRead(DWORD *pVCnt, DWORD *pECnt, DWORD* pBCnt, int nBufLen);

	BOOL OpenDevice();
	BOOL CloseDevice();

	static void TimeToCount(double fTimeInUs, DWORD* pdwCnt);
	static void TimeToCount(DWORD dwTimeInUs, DWORD* pdwCnt);

	CFH();
	~CFH();

protected:
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
#else

	#include "FHSim.h"

#endif   //end define _FH_SIM

#endif  //end define __FH_H__