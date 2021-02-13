#ifndef __owssio_h__
#define __owssio_h__

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

typedef ROD_STAT SEPTA_STAT;

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
    WORD  wT_GANTRY;         //GANTRY温度
	WORD  wT_WATERCOOL;      //水冷机温度
	WORD  wT_Rod;            //ROD温度
	WORD  wT_Septa;			 //Septa温度
	WORD  wT_Bed; 			 //床温度

	BYTE  HVStatus;
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

	//棒源信息
	ROD_STAT Rod_stat;		 //棒源状态：伸出/缩回
	WORD     wRod_rate;      //棒源转速
	WORD     wRod_SwitchSet;  //棒源开关量设置

	//床信息
	BED_STAT Bed_stat;
	WORD     wBed_SwitchSet;   //
	WORD     wBed_posx;        //床位置
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

	BOOL SetThreshold_HV(WORD upper, WORD lower); //设置高压上下限
	BOOL SetThreshold_I(WORD upper, WORD lower);  //设置电流上下限
	BOOL SetThreshold_TW(WORD upper, WORD lower); //设置水冷机温度上下限
	BOOL SetThreshold_TG(WORD upper, WORD lower); //设置GANTRY温度上下限
	BOOL SetRodRate(WORD rate);                 //设置棒源转速
	BOOL SetBedCoord(BOOL bRelate);             //设置床坐标系统：相对坐标、绝对坐标
	BOOL SetBedStep(WORD step);                 //设置床步长

	BOOL Control_SeptaEStop();
	BOOL Control_SeptaIn(void);
	BOOL Control_SeptaStop(void);
	BOOL Control_SeptaOut(void);
	BOOL Control_BedStopAll(void);     //放弃所有床运动
	BOOL Control_BedStopY(void);	   //放弃床Y方向运动
	BOOL Control_BedStopX(void);	   //放弃床X方向运动
	BOOL Control_BedMoveYStep(BOOL bUp);    //控制床单步运动
	BOOL Control_BedMoveXStep(BOOL bOut);
	BOOL Control_BedMoveY(WORD len);
	BOOL Control_BedMoveYRelate(WORD len, BOOL bUp);
	BOOL Control_BedMoveXRelate(WORD len, BOOL bOut); //控制床相对运动
	BOOL Control_BedMoveX(WORD len); //控制床绝对运动
	BOOL Control_RodOut();	            //控制棒源伸出并转动
	BOOL Control_RodStop();		//控制棒源停止转动
	BOOL Control_RodIn();       //控制棒源收回
	BOOL Control_RodEStop();    //控制棒源紧急停止
	BOOL Control_MainPowerON(BOOL bON);   //控制前端主电源通、断
	BOOL Control_LaserON(BOOL bON);	//控制激光灯通、断
	BOOL Control_HVON(BOOL  bON);  //控制高压通、断

	BOOL QueryHVStatus();
	BOOL QueryTBed();
	BOOL QueryTSepta();
	BOOL QueryTRod();
	BOOL QueryTWaterCool();	     //查询水冷机温度
	BOOL QueryTGantry();         //查询GANTRY温度

	BOOL QueryRodStatus();
	BOOL QueryRodRate();		 //查询棒源转速
	
	BOOL QueryThreshold_HV();	 //查询高压上下限 
	BOOL QueryThreshold_I();	 //查询电流上下限
	BOOL QueryThreshold_TW();	 //查询水冷机温度上下限
	BOOL QueryThreshold_TG();    //查询GANTRY温度上下限

	BOOL QuerySeptaStatus();
	BOOL QuerySeptaSwitchSet();
	BOOL QueryRodSwitchSet();

	BOOL QueryBedSwitchSet();
	BOOL QueryBedMoveStatus();
	BOOL QueryBedStep();         //查询床运动步长
	BOOL QueryBedPos(void);		 //查询床坐标

protected:
	SIO_FRAME8        m_WFrame8;
	SIO_FRAME9        m_WFrame9;
	SIO_FRAME12       m_WFrame12;
};

#endif