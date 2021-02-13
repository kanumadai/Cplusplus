//error code ver 2.1   2003.11.18

#ifndef __ERRCODE_H__
#define __ERRCODE_H__

#define ERROR_FH            0x1000
#define ERROR_OWS           0x2000
#define ERROR_GANTRY        0x3000
#define ERROR_BED           0x4000
#define ERROR_ROD           0x5000
#define ERROR_SEPTA         0x6000
#define ERROR_BUCKET        0x7000

#define ERROR_ALARM_GANTRY  0x8000 
#define ERROR_ALARM_HV      0x9000 
#define ERROR_ALARM_BED     0xA000 
#define ERROR_ALARM_ROD     0xB000
#define ERROR_ALARM_SEPTA   0xC000

//////////////////////////  符合电路错误码定义 //////////////////////////////
//    0x1000 ~ 0x1FFF
#define ERROR_FH_OPENDEVICE  (ERROR_FH+1)
#define ERROR_FH_GATEPARAM   (ERROR_FH+2)
#define ERROR_FH_PORT_EMC    (ERROR_FH+3)
#define ERROR_FH_PORT_BUCKET (ERROR_FH+4)
//    符合电路错误码定义
//////////////////////////////////////////////////////////////////////

//-------------------- 主控程序接口错误码定义 -----------------------
//	  0x2000 ~ 0x2FFF
#define ERROR_OWS_PROTOCOL    (ERROR_OWS+1)      //采集协议错误
#define ERROR_OWS_INIT_ACS    (ERROR_OWS+2)      //PET_INIT ACS
#define ERROR_OWS_INIT_GANTRY (ERROR_OWS+3)
#define ERROR_OWS_INIT_BUCKET (ERROR_OWS+4)
#define ERROR_OWS_INIT_ROD    (ERROR_OWS+5)
#define ERROR_OWS_INIT_SEPTA  (ERROR_OWS+6)
#define ERROR_OWS_INIT_BED    (ERROR_OWS+7)
#define ERROR_OWS_INIT_UNKNOW (ERROR_OWS+8)
//
//-------------------------------------------------------------------

///////////////////// GANTRY错误码定义  ////////////////////////
//   0x3000 ~ 0x3FFF
#define ERROR_GANTRY_INIT          (ERROR_GANTRY+1)	//初始化状态
#define ERROR_GANTRY_QUERY         (ERROR_GANTRY+2) //查询温度、电压、电流、阈值错误
#define ERROR_GANTRY_SETTH         (ERROR_GANTRY+3) //设置阈值错误
#define ERROR_GANTRY_MAINPOWER_ON  (ERROR_GANTRY+4)	//开前端主电源
#define ERROR_GANTRY_MAINPOWER_OFF (ERROR_GANTRY+5)	//关前端主电源
#define ERROR_GANTRY_HV_ON         (ERROR_GANTRY+6) //开高压
#define ERROR_GANTRY_HV_OFF        (ERROR_GANTRY+7)	//关高压
//#define ERROR_GANTRY_ALARM         (ERROR_GANTRY+8) //GANTRY报警
#define ERROR_GANTRY_ESTOP         (ERROR_GANTRY+8) //GANTRY Emergency Stop 
//
//////////////////////////////////////////////////////////////////////

///////////////////// BED 错误码定义  ////////////////////////
//   0x4000 ~ 0x4FFF
#define ERROR_BED_QUERY_POS        (ERROR_BED+1)      //查询床位置
#define ERROR_BED_QUERY_STEP       (ERROR_BED+2)	  //查询床步长
#define ERROR_BED_QUERY_MOVESTATUS (ERROR_BED+3)      //查询床运动状态
#define ERROR_BED_QUERY_SWITCH     (ERROR_BED+4)	  //查询床开关量
#define ERROR_BED_CTRLSTOP         (ERROR_BED+5)      //控制床停止
#define ERROR_BED_SETCOORD         (ERROR_BED+6)	  //设置床坐标方式
#define ERROR_BED_MOVEX            (ERROR_BED+7)	  //控制床X运动
#define ERROR_BED_MOVEY            (ERROR_BED+8)	  //控制床Y运动
//#define ERROR_BED_ALARM            (ERROR_BED+9)
//
//////////////////////////////////////////////////////////////

///////////////////// ROD 错误码定义  ////////////////////////
//   0x5000 ~ 0x5FFF
#define ERROR_ROD_QUERY_RATE     (ERROR_ROD+1)   //查询棒源转速
#define ERROR_ROD_QUERY_SWITCH	 (ERROR_ROD+2)   //查询棒源开关量
#define ERROR_ROD_SETRATE		 (ERROR_ROD+3)   //设置棒源转速
#define ERROR_ROD_CTRL_OUT       (ERROR_ROD+4)	 //控制棒源伸出
#define ERROR_ROD_CTRL_IN        (ERROR_ROD+5)	 //控制棒源缩回
#define ERROR_ROD_CTRL_ROLL      (ERROR_ROD+6)
#define ERROR_ROD_CTRL_STOP      (ERROR_ROD+7)	 //控制棒源停止旋转
#define ERROR_ROD_QUERY_STATUS   (ERROR_ROD+8)
//#define ERROR_ROD_ALARM          (ERROR_ROD+9)
//
///////////////////////////////////////////////////////////////

///////////////////// SEPTA 错误码定义  ////////////////////////
//   0x6000 ~ 0x6FFF
#define ERROR_SEPTA_QUERY_SWITCH (ERROR_SEPTA+1) //查询SEPTA开关量
#define ERROR_SEPTA_CTRL_OUT	 (ERROR_SEPTA+2) //控制SEPTA伸出
#define ERROR_SEPTA_CTRL_IN      (ERROR_SEPTA+3) //控制SEPTA缩回
#define ERROR_SEPTA_CTRL_STOP    (ERROR_SEPTA+4)
#define ERROR_SEPTA_QUERY_STATUS (ERROR_SEPTA+5) //查询SEPTA STATUS
//#define ERROR_SEPTA_ALARM        (ERROR_SEPTA+6)
//
//////////////////////////////////////////////////////////////////

///////////////////// GANTRY错误码定义  //////////////////////////
//   0x7000 ~ 0x7FFF
#define ERROR_BUCKET_SELFTEST    (ERROR_BUCKET+1)
//
//////////////////////////////////////////////////////////////////


#endif  //__ERRCODE_H__
