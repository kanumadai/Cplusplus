#ifndef __bucksio_h__
#define __bucksio_h__

#include "..\AcqCenter\SIOProtocol.H"

#define BUCK_ANSWER_TIMEOUT    2000/*ms*/
#define BUCK_SELFTEST_DELAY    20000/*ms*/

typedef enum _bucket_mode{
	 mode_unknow = 0xFF,
	 mode_energy = 0xC5,
	 mode_time   = 0xC6,
	 mode_sd     = 0xC7,
	 mode_coin0  = 0xC8,
	 mode_coin1  = 0xC9
}BUCKET_DAT_MODE;

typedef enum _bucket_selftest_status{
	status_unknow        = 0xff,
	status_ok            = 0x00,
	status_err_pos       = 0x01,
	status_err_energy    = 0x02,
	status_err_time      = 0x04,
	status_err_strip     = 0x10,
	status_err_eeprom    = 0x20,
	status_err_envirment = 0x40
}BUCKET_SELFTEST_STATUS;

typedef struct _bucket_work_status{
	BUCKET_DAT_MODE datMode;
	BOOL bWorking;
	DWORD dwWorkTime;    //采集时间长度
	BOOL b9600;
}BUCET_WORK_STATUS;

class CBuckSIO
{
public:
	BOOL Control_SelfTest(void);   //要求BUCKET自检
	BOOL Query_SelfTestResult(int nBucket);	    //查询BUCKET自检结果
	BOOL Control_DataAcqBegin(BUCKET_DAT_MODE mode, DWORD time);	//设置BUCKET数据采集模式
	BOOL Control_DataAcqStop(void);
	BOOL SendTestData(BYTE* pBuf, int nBufLen);

	BOOL OpenSIO(const char* szPort, int nBaudRate);
	BOOL CloseSIO();
	void SetSIOMode(BOOL bTest);

public:
	CBuckSIO();
	~CBuckSIO();

public:
	DWORD _ThreadRecvComm();

public:	
	BUCET_WORK_STATUS m_WorkStatus;
	BUCKET_SELFTEST_STATUS  m_SelfTestStatus[32];  //32bucket self test status

protected:
	HANDLE m_hCom;
	HANDLE m_hThread;
	HANDLE m_hEvtExit;
	BOOL m_bSIOMode_Test;     //TRUE: comm handle by OWS;   FALSE: comm handle by ACS

	BOOL   m_bSelfTest;
	HANDLE m_hEvtAnswer;

	BOOL RecvFrame(OVERLAPPED* pOL, BYTE* pFrameBuf, int nBufLen);
	BOOL Process_BucketFrame(BYTE* pBuf, int datLen);

	BOOL WriteComm(LPCVOID pBuf, int len);
	CRITICAL_SECTION  m_CLockWrite;	 //写串口临界量
	OVERLAPPED        m_olWrite;
	HANDLE            m_hEvtWrite;
};

#endif