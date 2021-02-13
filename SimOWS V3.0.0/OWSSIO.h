#ifndef __owssio_h__
#define __owssio_h__

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

typedef ROD_STAT SEPTA_STAT;

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
    WORD  wT_GANTRY;         //GANTRY�¶�
	WORD  wT_WATERCOOL;      //ˮ����¶�
	WORD  wT_Rod;            //ROD�¶�
	WORD  wT_Septa;			 //Septa�¶�
	WORD  wT_Bed; 			 //���¶�

	BYTE  HVStatus;
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

	//��Դ��Ϣ
	ROD_STAT Rod_stat;		 //��Դ״̬�����/����
	WORD     wRod_rate;      //��Դת��
	WORD     wRod_SwitchSet;  //��Դ����������

	//����Ϣ
	BED_STAT Bed_stat;
	WORD     wBed_SwitchSet;   //
	WORD     wBed_posx;        //��λ��
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

#pragma pack(push)
#pragma pack(1)

//receive frame form GC, 12byte len
typedef struct _sio_frame12
{
   BYTE tAddr;        //target addr
   BYTE sAddr;        //source addr
   BYTE len;  	      //frame len, include this head
   BYTE attr;         //attribe
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE Dat[4];
   BYTE CheckSum;
   BYTE	eof;      	  //should be 0x7F
}SIO_FRAME12;

//Frame send to GC, 8byte len
typedef struct _sio_frame8
{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len;
   BYTE attr;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE CheckSum;
   BYTE eof;
}SIO_FRAME8;

//Frame send to GC, 9byte len
typedef struct _sio_frame9
{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len;
   BYTE attr;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE Dat;
   BYTE CheckSum;
   BYTE eof;
}SIO_FRAME9;

//address define
#define ADDR_ACS         0xB0
#define ADDR_GANTRY      0xA0
#define ADDR_Y1          0x11
#define ADDR_Y2          0x12
#define ADDR_Y1Y2        0x10
#define ADDR_ROD         0x03
#define ADDR_BED         0x04
#define ADDR_SEPTA       0x05
#define ADDR_RODBEDSEPTA 0x08
#define ADDR_BUCKET_ALL  0x70
#define ADDR_BUCKET0     0x80
#define ADDR_BUCKET31    0x9F

#pragma pack(pop)

BOOL Process_GantryDat(SIO_FRAME12* pFrame);
BOOL Process_BucketDat(BYTE* pBuf, int len);

class COWSSIO
{
public:
	COWSSIO();
	~COWSSIO();

public:
	BOOL Control_RodRoll();
	BOOL Dbg_GCPollingEnable(BOOL bEnable);

	BOOL SetThreshold_HV(WORD upper, WORD lower); //���ø�ѹ������
	BOOL SetThreshold_I(WORD upper, WORD lower);  //���õ���������
	BOOL SetThreshold_TW(WORD upper, WORD lower); //����ˮ����¶�������
	BOOL SetThreshold_TG(WORD upper, WORD lower); //����GANTRY�¶�������
	BOOL SetRodRate(WORD rate);                 //���ð�Դת��
	BOOL SetBedCoord(BOOL bRelate);             //���ô�����ϵͳ��������ꡢ��������
	BOOL SetBedStep(WORD step);                 //���ô�����

	BOOL Control_SeptaEStop();
	BOOL Control_SeptaIn(void);
	BOOL Control_SeptaStop(void);
	BOOL Control_SeptaOut(void);
	BOOL Control_BedStopAll(void);     //�������д��˶�
	BOOL Control_BedStopY(void);	   //������Y�����˶�
	BOOL Control_BedStopX(void);	   //������X�����˶�
	BOOL Control_BedMoveYStep(BOOL bUp);    //���ƴ������˶�
	BOOL Control_BedMoveXStep(BOOL bOut);
	BOOL Control_BedMoveY(WORD len);
	BOOL Control_BedMoveYRelate(WORD len, BOOL bUp);
	BOOL Control_BedMoveXRelate(WORD len, BOOL bOut); //���ƴ�����˶�
	BOOL Control_BedMoveX(WORD len); //���ƴ������˶�
	BOOL Control_RodOut();	            //���ư�Դ�����ת��
	BOOL Control_RodStop();		//���ư�Դֹͣת��
	BOOL Control_RodIn();       //���ư�Դ�ջ�
	BOOL Control_RodEStop();    //���ư�Դ����ֹͣ
	BOOL Control_MainPowerON(BOOL bON);   //����ǰ������Դͨ����
	BOOL Control_LaserON(BOOL bON);	//���Ƽ����ͨ����
	BOOL Control_HVON(BOOL  bON);  //���Ƹ�ѹͨ����

	BOOL QueryHVStatus();
	BOOL QueryTBed();
	BOOL QueryTSepta();
	BOOL QueryTRod();
	BOOL QueryTWaterCool();	     //��ѯˮ����¶�
	BOOL QueryTGantry();         //��ѯGANTRY�¶�

	BOOL QueryRodStatus();
	BOOL QueryRodRate();		 //��ѯ��Դת��
	
	BOOL QueryThreshold_HV();	 //��ѯ��ѹ������ 
	BOOL QueryThreshold_I();	 //��ѯ����������
	BOOL QueryThreshold_TW();	 //��ѯˮ����¶�������
	BOOL QueryThreshold_TG();    //��ѯGANTRY�¶�������

	BOOL QuerySeptaStatus();
	BOOL QuerySeptaSwitchSet();
	BOOL QueryRodSwitchSet();

	BOOL QueryBedSwitchSet();
	BOOL QueryBedMoveStatus();
	BOOL QueryBedStep();         //��ѯ���˶�����
	BOOL QueryBedPos(void);		 //��ѯ������

protected:
	SIO_FRAME8        m_WFrame8;
	SIO_FRAME9        m_WFrame9;
	SIO_FRAME12       m_WFrame12;
};

#endif