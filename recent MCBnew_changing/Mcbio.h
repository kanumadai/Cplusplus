
#ifndef __MCBIO_H__
#define __MCBIO_H__

//------------------������ʼ�ɼ�������Ѱ�����֡�------------------------------
#define SENDTIMEOUT			1500
#define MCBMAX_QUERYNUM		6

#define MCB_START_QUERYNO	0
#define	BUFF_START_QUERYNO  3

//-----------------���Զ����������塿----------
#define CTL_LOAD_FULL_RAM				3000
#define CTL_LOAD_EXP_RAM				3001
#define CTL_LOAD_TIME_VALUE				3002
#define CTL_LOAD_INTERST_CHANNEL_COUNTER	3003
#define CTL_PROG_CONFIG_ANOLPARA			3004	

#define CTL_HARDWARE_READ				3008
#define CTL_HARDWARE_WRITE				3009

class CMCBIO;

typedef enum COMMUNICATION_PORT {
	COM1_PORT = 0,
	COM2_PORT = 1,
	USB_PORT = 2
};

typedef enum _alarm_stat
{
   normal=0,
   alarm=1
}ALARM;

union TYPE 
{
	UINT intdata;
	unsigned char uchdata[4];
};

typedef struct _date {
	int year;
	int month;
	int day;
	int hour;
	int minite;
	int second;
}MYDATE;

typedef struct _fileheader {
	WORD	readout_type;
	WORD	mac_number;
	WORD	region;
	WORD	tag_nL;  //����
	WORD	tag_nH;  //
	char	id[26];
	WORD	acq_mode;
	WORD	timeL;  //����
	WORD	timeH;
	WORD	millitm;
	WORD	timezone;
	WORD	dstflag;
	WORD	livetimeL; //����
	WORD	livetimeH;

	WORD	realtimeL; //����
	WORD	realtimeH;
	WORD	sweepsL;  //����
	WORD	sweepsH;
	WORD	presetL1; //����
	WORD	presetL2;
	WORD	presetL3; //����
	WORD	presetL4;
	float	c2;
	float	c1;
	float	c0;
	char	unit[5];
	char	unittype;
	char	format;
	char	order;
	WORD	spm_num;
	WORD	naa_flag;
	WORD	filler[17];
	WORD	chn_number;

}FILEHEADER;

//MCB�ɼ���Ѱ���Ƶ������Ϣ
typedef struct _mcb_oper
{
	int	QueryTable[10];
	int iCurQueryNo;
	int iStartQueryNo;
	BOOL bQueryEn;
	BOOL bStopCMDEn;
}MCB_OPER;

//Ӳ��������������
typedef struct _hardware_pare {
	//Ӳ������
	int iChannelWidth;  // ����
	int iSampleTimeValue; // ����ʱ��
	BOOL bSampleModeRealTime;  //����ģʽ ��ʵʱ or ��ʱ�� true�� ʵʱ��
	
	//ADC����
	BOOL bDT_Pulse;		// true Ϊ����Ч
	BOOL bREADY_Pulse;   // true Ϊ����Ч
	BOOL bANSER_Pulse;   // true Ϊ����Ч
	BOOL bENADC_Pulse;   // true Ϊ����Ч
	BOOL bDataPolarity_Pulse; // true Ϊ���룬 false Ϊ���룻

	//�����̶�
	float fEngerScaleSlope; // �����̶�б��
	float fEngerScaleOffset;  // �����̶�ƫ��
	int iEngerScaleUnit;   //�����̶ȵ�λ ��NULL���޿̶ȣ�1��ev��2��kev��3��mev��

	//������ʽ
	BOOL bReadorWriteOperate; //��д���� true = read  , false = write;
	BOOL bInputAnolSignalSel; //�����ź�ѡ��TRUE �� Anol��False��Digit

	//USB�ɼ�ģʽ��ADC ���С�����
	int  iEth_Up_UsbADC;
	int  iEth_Down_UsbADC;
	int  iTime_Up_ETH;
	int  iTime_Down_ETH;

}HARDWARE_PARE;

class CMCBIO
{
public:
	CMCBIO();
	~CMCBIO();

public:
	void Ctl_HardWare_Write(LPVOID lpData);
	void Ctl_HardWare_Read(LPVOID lpData);
	void Ctl_ReceiveExternalRam(LPVOID lpData);
	void Ctl_QuerySampleState(LPVOID lpData);
	void Ctl_QueryTimeOut(LPVOID lpData);
	void Ctl_SendExternalRam(LPVOID lpData);
	void Ctl_ContinueSample(LPVOID lpData);
	void Ctl_ResetSystem(LPVOID lpData);
	void Ctl_LoadInterstChannCounter(LPVOID lpData);
	void Ctl_LoadTimeValue(LPVOID lpData);
	void Ctl_LoadExpRam(LPVOID lpData);
	void Ctl_LoadFullRam(LPVOID lpData);
	void Ctl_ComTest(LPVOID lpData);
	BOOL Ctl_StartSample(LPVOID  lpData);
	BOOL Ctl_StopSample(LPVOID  lpData);

	void Ctl_Prog_ConfigAnolPara(LPVOID lpData);

	unsigned char m_SendFrame[1024*4];


protected:

	
	
};


UINT  MonitorThread(LPVOID  lpData);
UINT  SendThread(LPVOID  lpData);

#endif //__MCBIO_H__