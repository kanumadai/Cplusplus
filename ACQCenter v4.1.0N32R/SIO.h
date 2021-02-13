#ifndef __sio_h__
#define __sio_h__

class CSIO;

//报警状态定义
typedef enum _alarm_stat
{
   normal=0,
   alarm=1
}ALARM;

typedef union _alarm_code
{
   BYTE code8;
   struct{
	   BYTE bAlarm_T:1;	 //温度报警
	   BYTE bAlarm_V:1;	 //电压报警
	   BYTE bAlarm_I:1;  //电流报警
	   BYTE bAlarm_Move:1;  //运动报警
	   BYTE bAlarm_Moto:1;  //电机
	   BYTE Reserve : 3;    //保留
   };
}ALARM_CODE;

//棒源状态定义
typedef enum _rod_stat
{
   unknow=0,
   out=1,	    //伸出到位
   in=2,		//缩回到位
   outing=3,	//伸出中
   ining=4,		//缩回中
   rolling=5,   //正在旋转
   stop=6		//停止
}ROD_STAT;

typedef ROD_STAT SEPTA_STAT;  //rolling is invalid

//床状态
typedef enum _bed_stat
{
	still,          //静止
	move,		    //运动中
	out_stop,	    //伸出到极限
	in_stop		    //收回到极限
}BED_STAT; 

//机电控制电路相关信息
typedef struct _mctl_info
{
	BOOL bInitOK;
	BYTE cInitStatus[4];

    WORD  wT_GANTRY;         //GANTRY温度
	WORD  wT_WATERCOOL;      //水冷机温度
	WORD  wT_Rod;            //ROD温度
	WORD  wT_Septa;			 //Septa温度
	WORD  wT_Bed; 			 //床温度

	BYTE  HVStatus;          //高压状态
	WORD  wV_1;              //高压1
	WORD  wV_2;              //高压2
	WORD  wI_1;              //电流1 
	WORD  wI_2;				 //电流2

	//阈值
	WORD  wTH_V_H;           //高压上阈
	WORD  wTH_V_L;           //高压下阈
	WORD  wTH_I_H;           //电流上阈
	WORD  wTH_I_L;		     //电流下阈
	WORD  wTH_GANTRYT_H;     //GANTRY温度上阈
	WORD  wTH_GANTRYT_L;     //GANTRY温度下阈
	WORD  wTH_WATERCOOLT_H;  //水冷机温度上阈
	WORD  wTH_WATERCOOLT_L;  //水冷机温度下阈

	//报警
	ALARM alarm_Gantry;      //GANTRY报警
	ALARM_CODE ac_Gantry;
	ALARM alarm_Bed;         //BED报警
	ALARM_CODE ac_Bed;
	ALARM alarm_Septa;       //SEPTA报警
	ALARM_CODE ac_Septa;
	ALARM alarm_Rod;		 //ROD报警
	ALARM_CODE ac_Rod;
	ALARM alarm_ESTOP;       //急停报警
	ALARM alarm_HV;
	ALARM_CODE ac_HV;        //高压报警

	//棒源信息
	ROD_STAT Rod_stat;		 //棒源状态：伸出/缩回
	WORD     wRod_rate;      //棒源转速
	WORD     wRod_SwitchSet;  //棒源开关量设置

	//床信息
	BED_STAT Bed_stat;
	WORD     wBed_SwitchSet;   //
	WORD     wBed_posx;        //床位置,0.1MM
	WORD     wBed_posy;
	WORD     wBed_step;       //床步长
	WORD     wBed_bRelateCoord;  //床坐标系统， 1=》相对坐标，0=》绝对坐标

	//SEPTA位置信息
	SEPTA_STAT  septa_stat;
	WORD     wSepta_SwitchSet;

	BOOL  bHVOn;				 //高压状态
	BOOL  bLaserOn;              //激光灯状态
	BOOL  bMainPowerOn;          //前端主电源状态

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
	BOOL SetRodRate(WORD rate);                 //设置棒源转速
	BOOL SetBedCoord(BOOL bRelate);             //设置床坐标系统：相对坐标、绝对坐标
	BOOL SetBedStep(WORD step);                 //设置床步长

//	BOOL Control_SeptaIn(void);
//	BOOL Control_SeptaStop(void);
//	BOOL Control_SeptaOut(void);

	BOOL Control_BedStopAll(void);     //放弃所有床运动
	BOOL Control_BedStopY(void);	   //放弃床Y方向运动
	BOOL Control_BedStopX(void);	   //放弃床X方向运动
	BOOL Control_BedMoveYStep(BOOL bUp);    //控制床单步运动
	BOOL Control_BedMoveXStep(BOOL bOut);
	BOOL Control_BedMoveX(WORD len/*0.1mm*/); //控制床绝对运动
	BOOL Control_BedMoveY(WORD len/*0.1mm*/);
	BOOL Control_BedMoveYRelate(WORD len/*0.1mm*/, BOOL bUp);
	BOOL Control_BedMoveXRelate(WORD len/*0.1mm*/, BOOL bOut); //控制床相对运动
	
	BOOL Control_RodOut();	            //控制棒源伸出并转动
	BOOL Control_RodStop();		        //控制棒源停止转动
	BOOL Control_RodIn();               //控制棒源收回
	BOOL Control_RodEStop();            //控制棒源紧急停止
	
	BOOL Control_LaserON(BOOL bON);	//控制激光灯通、断

	BOOL QueryTBed();
//	BOOL QueryTRod();

	BOOL QueryRodRate();		 //查询棒源转速
	
	BOOL QueryRodSwitchSet();

	BOOL QueryBedSwitchSet();
	BOOL QueryBedMoveStatus();
	BOOL QueryBedStep();         //查询床运动步长
	BOOL QueryBedPos(void);		 //查询床坐标

	BOOL OpenSIO(const char* szPort);
	BOOL CloseSIO();
	void SetSIOMode(BOOL bTest);

//	BOOL QueryHVStatus();
//	BOOL QuerySeptaStatus();

//	BOOL SetThreshold_HV(WORD upper, WORD lower); //设置高压上下限
//	BOOL SetThreshold_I(WORD upper, WORD lower);  //设置电流上下限
//	BOOL SetThreshold_TW(WORD upper, WORD lower); //设置水冷机温度上下限
//	BOOL SetThreshold_TG(WORD upper, WORD lower); //设置GANTRY温度上下限

//	BOOL Control_SeptaEStop();
//	BOOL Control_SeptaIn(void);
//	BOOL Control_SeptaStop(void);
//	BOOL Control_SeptaOut(void);
	
//	BOOL Control_MainPowerON(BOOL bON);   //控制前端主电源通、断
//	BOOL Control_HVON(BOOL  bON);  //控制高压通、断

//	BOOL QueryTSepta();
//	BOOL QueryTWaterCool();	     //查询水冷机温度
//	BOOL QueryTGantry();         //查询GANTRY温度

	
//	BOOL QueryThreshold_HV();	 //查询高压上下限 
//	BOOL QueryThreshold_I();	 //查询电流上下限
//	BOOL QueryThreshold_TW();	 //查询水冷机温度上下限
//	BOOL QueryThreshold_TG();    //查询GANTRY温度上下限

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
	CRITICAL_SECTION  m_CLockWrite;	 //写串口临界量
	SIO_FRAME8        m_WFrame8;
	SIO_FRAME9        m_WFrame9;
	SIO_FRAME12       m_WFrame12;
	OVERLAPPED        m_olWrite;
	HANDLE            m_hEvtWrite;

protected:
	HANDLE    m_hEvtSIO_HandShake;      //GANTRAY握手应答事件
	HANDLE    m_hEvtSIO_Retry;          //数据重发请求事件
	HANDLE    m_hEvtSIO_TVI;            //收到温度、高压数据事件
	HANDLE    m_hEvtSIO_ROD;            //棒源事件
	HANDLE    m_hEvtSIO_Bed;            //床事件
	HANDLE    m_hEvtSIO_Septa;          //Septa事件
	HANDLE    m_hEvtSIO_SetOK;			//设置数据有效
	HANDLE    m_hEvtSIO_Answer;         //通用应答事件
};

#endif