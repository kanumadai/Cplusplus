
#ifndef __MCBIO_H__
#define __MCBIO_H__

//------------------【“开始采集”的轮寻控制字】------------------------------
#define SENDTIMEOUT			1500
#define MCBMAX_QUERYNUM		6

#define MCB_START_QUERYNO	0
#define	BUFF_START_QUERYNO  3

//-----------------【自定义控制命令定义】----------
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
	WORD	tag_nL;  //拆开了
	WORD	tag_nH;  //
	char	id[26];
	WORD	acq_mode;
	WORD	timeL;  //拆开了
	WORD	timeH;
	WORD	millitm;
	WORD	timezone;
	WORD	dstflag;
	WORD	livetimeL; //拆开了
	WORD	livetimeH;

	WORD	realtimeL; //拆开了
	WORD	realtimeH;
	WORD	sweepsL;  //拆开了
	WORD	sweepsH;
	WORD	presetL1; //拆开了
	WORD	presetL2;
	WORD	presetL3; //拆开了
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

//MCB采集轮寻控制的相关信息
typedef struct _mcb_oper
{
	int	QueryTable[10];
	int iCurQueryNo;
	int iStartQueryNo;
	BOOL bQueryEn;
	BOOL bStopCMDEn;
}MCB_OPER;

//硬件参数设置内容
typedef struct _hardware_pare {
	//硬件参数
	int iChannelWidth;  // 道宽
	int iSampleTimeValue; // 采样时间
	BOOL bSampleModeRealTime;  //采样模式 （实时 or 活时） true＝ 实时；
	
	//ADC极性
	BOOL bDT_Pulse;		// true 为正有效
	BOOL bREADY_Pulse;   // true 为正有效
	BOOL bANSER_Pulse;   // true 为正有效
	BOOL bENADC_Pulse;   // true 为正有效
	BOOL bDataPolarity_Pulse; // true 为正码， false 为反码；

	//能量刻度
	float fEngerScaleSlope; // 能量刻度斜率
	float fEngerScaleOffset;  // 能量刻度偏置
	int iEngerScaleUnit;   //能量刻度单位 ；NULL＝无刻度；1＝ev；2＝kev；3＝mev；

	//操作方式
	BOOL bReadorWriteOperate; //读写操作 true = read  , false = write;
	BOOL bInputAnolSignalSel; //输入信号选择，TRUE ＝ Anol，False＝Digit

	//USB采集模式的ADC 上阈、下阈
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