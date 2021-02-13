#ifndef __sio_h__
#define __sio_h__

class CSIO;

//����״̬����
typedef enum _alarm_stat
{
   normal=0,
   alarm=1
}ALARM;

typedef union _alarm_code
{
   BYTE code8;
   struct{
	   BYTE bAlarm_T:1;	 //�¶ȱ���
	   BYTE bAlarm_V:1;	 //��ѹ����
	   BYTE bAlarm_I:1;  //��������
	   BYTE bAlarm_Move:1;  //�˶�����
	   BYTE bAlarm_Moto:1;  //���
	   BYTE Reserve : 3;    //����
   };
}ALARM_CODE;

//��Դ״̬����
typedef enum _rod_stat
{
   unknow=0,
   out=1,	    //�����λ
   in=2,		//���ص�λ
   outing=3,	//�����
   ining=4,		//������
   rolling=5,   //������ת
   stop=6		//ֹͣ
}ROD_STAT;

typedef ROD_STAT SEPTA_STAT;  //rolling is invalid

//��״̬
typedef enum _bed_stat
{
	still,          //��ֹ
	move,		    //�˶���
	out_stop,	    //���������
	in_stop		    //�ջص�����
}BED_STAT; 

//������Ƶ�·�����Ϣ
typedef struct _mctl_info
{
	BOOL bInitOK;
	BYTE cInitStatus[4];

    WORD  wT_GANTRY;         //GANTRY�¶�
	WORD  wT_WATERCOOL;      //ˮ����¶�
	WORD  wT_Rod;            //ROD�¶�
	WORD  wT_Septa;			 //Septa�¶�
	WORD  wT_Bed; 			 //���¶�

	BYTE  HVStatus;          //��ѹ״̬
	WORD  wV_1;              //��ѹ1
	WORD  wV_2;              //��ѹ2
	WORD  wI_1;              //����1 
	WORD  wI_2;				 //����2

	//��ֵ
	WORD  wTH_V_H;           //��ѹ����
	WORD  wTH_V_L;           //��ѹ����
	WORD  wTH_I_H;           //��������
	WORD  wTH_I_L;		     //��������
	WORD  wTH_GANTRYT_H;     //GANTRY�¶�����
	WORD  wTH_GANTRYT_L;     //GANTRY�¶�����
	WORD  wTH_WATERCOOLT_H;  //ˮ����¶�����
	WORD  wTH_WATERCOOLT_L;  //ˮ����¶�����

	//����
	ALARM alarm_Gantry;      //GANTRY����
	ALARM_CODE ac_Gantry;
	ALARM alarm_Bed;         //BED����
	ALARM_CODE ac_Bed;
	ALARM alarm_Septa;       //SEPTA����
	ALARM_CODE ac_Septa;
	ALARM alarm_Rod;		 //ROD����
	ALARM_CODE ac_Rod;
	ALARM alarm_ESTOP;       //��ͣ����
	ALARM alarm_HV;
	ALARM_CODE ac_HV;        //��ѹ����

	//��Դ��Ϣ
	ROD_STAT Rod_stat;		 //��Դ״̬�����/����
	WORD     wRod_rate;      //��Դת��
	WORD     wRod_SwitchSet;  //��Դ����������

	//����Ϣ
	BED_STAT Bed_stat;
	WORD     wBed_SwitchSet;   //
	WORD     wBed_posx;        //��λ��,0.1MM
	WORD     wBed_posy;
	WORD     wBed_step;       //������
	WORD     wBed_bRelateCoord;  //������ϵͳ�� 1=��������꣬0=����������

	//SEPTAλ����Ϣ
	SEPTA_STAT  septa_stat;
	WORD     wSepta_SwitchSet;

	BOOL  bHVOn;				 //��ѹ״̬
	BOOL  bLaserOn;              //�����״̬
	BOOL  bMainPowerOn;          //ǰ������Դ״̬

}MCTL_INFO;

#define SIO_ANSWER_TIMEOUT    2000/*ms*/
#define ROD_ANSWER_TIMEOUT    90000/*ms*/		 
#define SEPTA_ANSWER_TIMEOUT  90000/*ms*/
#define BED_ANSWER_TIMEOUT    60000/*ms*/    //move speed:700mm/60s->

class CSIO
{
public:
	CSIO();
	~CSIO();

public:
	BOOL QueryInitStatus();
	BOOL QueryRodStatus();
	BOOL SendTestDat(const BYTE* pBuf, int nBufLen);
	BOOL Control_RodRoll();
	BOOL Dbg_GCPollingEnable(BOOL bEnable);
	BOOL SetRodRate(WORD rate);                 //���ð�Դת��
	BOOL SetBedCoord(BOOL bRelate);             //���ô�����ϵͳ��������ꡢ��������
	BOOL SetBedStep(WORD step);                 //���ô�����

//	BOOL Control_SeptaIn(void);
//	BOOL Control_SeptaStop(void);
//	BOOL Control_SeptaOut(void);

	BOOL Control_BedStopAll(void);     //�������д��˶�
	BOOL Control_BedStopY(void);	   //������Y�����˶�
	BOOL Control_BedStopX(void);	   //������X�����˶�
	BOOL Control_BedMoveYStep(BOOL bUp);    //���ƴ������˶�
	BOOL Control_BedMoveXStep(BOOL bOut);
	BOOL Control_BedMoveX(WORD len/*0.1mm*/); //���ƴ������˶�
	BOOL Control_BedMoveY(WORD len/*0.1mm*/);
	BOOL Control_BedMoveYRelate(WORD len/*0.1mm*/, BOOL bUp);
	BOOL Control_BedMoveXRelate(WORD len/*0.1mm*/, BOOL bOut); //���ƴ�����˶�
	
	BOOL Control_RodOut();	            //���ư�Դ�����ת��
	BOOL Control_RodStop();		        //���ư�Դֹͣת��
	BOOL Control_RodIn();               //���ư�Դ�ջ�
	BOOL Control_RodEStop();            //���ư�Դ����ֹͣ
	
	BOOL Control_LaserON(BOOL bON);	//���Ƽ����ͨ����

	BOOL QueryTBed();
//	BOOL QueryTRod();

	BOOL QueryRodRate();		 //��ѯ��Դת��
	
	BOOL QueryRodSwitchSet();

	BOOL QueryBedSwitchSet();
	BOOL QueryBedMoveStatus();
	BOOL QueryBedStep();         //��ѯ���˶�����
	BOOL QueryBedPos(void);		 //��ѯ������

	BOOL OpenSIO(const char* szPort);
	BOOL CloseSIO();
	void SetSIOMode(BOOL bTest);

//	BOOL QueryHVStatus();
//	BOOL QuerySeptaStatus();

//	BOOL SetThreshold_HV(WORD upper, WORD lower); //���ø�ѹ������
//	BOOL SetThreshold_I(WORD upper, WORD lower);  //���õ���������
//	BOOL SetThreshold_TW(WORD upper, WORD lower); //����ˮ����¶�������
//	BOOL SetThreshold_TG(WORD upper, WORD lower); //����GANTRY�¶�������

//	BOOL Control_SeptaEStop();
//	BOOL Control_SeptaIn(void);
//	BOOL Control_SeptaStop(void);
//	BOOL Control_SeptaOut(void);
	
//	BOOL Control_MainPowerON(BOOL bON);   //����ǰ������Դͨ����
//	BOOL Control_HVON(BOOL  bON);  //���Ƹ�ѹͨ����

//	BOOL QueryTSepta();
//	BOOL QueryTWaterCool();	     //��ѯˮ����¶�
//	BOOL QueryTGantry();         //��ѯGANTRY�¶�

	
//	BOOL QueryThreshold_HV();	 //��ѯ��ѹ������ 
//	BOOL QueryThreshold_I();	 //��ѯ����������
//	BOOL QueryThreshold_TW();	 //��ѯˮ����¶�������
//	BOOL QueryThreshold_TG();    //��ѯGANTRY�¶�������

//	BOOL QuerySeptaSwitchSet();

public:
	DWORD _ThreadRecvComm();

protected:
	HANDLE m_hCom;
	HANDLE m_hThread;
	HANDLE m_hEvtExit;

	BOOL m_bSIOMode_Test;

	BOOL Process_GantryDat(SIO_FRAME12* pFrame);
	BOOL RecvFrame(OVERLAPPED* pOL, BYTE* pFrameBuf, int nBufLen);
	BOOL WriteComm(LPCVOID pBuf, int len);
	CRITICAL_SECTION  m_CLockWrite;	 //д�����ٽ���
	SIO_FRAME8        m_WFrame8;
	SIO_FRAME9        m_WFrame9;
	SIO_FRAME12       m_WFrame12;
	OVERLAPPED        m_olWrite;
	HANDLE            m_hEvtWrite;

protected:
	HANDLE    m_hEvtSIO_HandShake;      //GANTRAY����Ӧ���¼�
	HANDLE    m_hEvtSIO_Retry;          //�����ط������¼�
	HANDLE    m_hEvtSIO_TVI;            //�յ��¶ȡ���ѹ�����¼�
	HANDLE    m_hEvtSIO_ROD;            //��Դ�¼�
	HANDLE    m_hEvtSIO_Bed;            //���¼�
	HANDLE    m_hEvtSIO_Septa;          //Septa�¼�
	HANDLE    m_hEvtSIO_SetOK;			//����������Ч
	HANDLE    m_hEvtSIO_Answer;         //ͨ��Ӧ���¼�
};

#endif