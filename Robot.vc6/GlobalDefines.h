 #ifndef __GLOVALDEFINES__H__
#define __GLOVALDEFINES__H__


#define PI 3.141592653598
#define min(a,b) (((a) < (b)) ? (a) : (b))
//#define MinFrontSpeed 70
#define MaxFrontSpeed    650
#define MaxFrontSpeed1   200
#define MidFrontSpeed    400
#define MinFrontSpeed    200
#define MaxFrontSpeedMRI 300
#define FrontDeviceTime 300
#define PETHight    185  
#define HightControl    390

#define MRIHight    225 
#define TurnError1 -72
#define TurnError2 -53
#define TurnError3 0//-45
#define TurnError4 -43
#define WaitingTime 8

#define MedianFilter      4
#define MotionNum         4
#define MOTIONNUM         8 //MotionNum 为电机数/2
#define MaxMovingSpeed    2400
#define MaxTuringSpeed1   82500
#define MaxTuringSpeed2   50000
#define MaxTuringPosition 5000

#define MaxPETPosition    20500

#define MaxPETPosition3   19000
#define MaxPETCTPosition  25000
#define MaxCTPosition3    19000

#define MaxPETMRIPosition  38000//适合MRI床板

#define MaxPETPosition1  38000 //MRI侧
#define MaxPETCNum      190//抓床板
#define MaxPETCNumR     110//离开床板
#define MaxPETCNumR1     132//放床板127
#define MaxPETCNumR2     220//放床板127

#define MaxCTPosition  29000
#define MaxCTCNum      220//抓床板

#define MaxMRIPosition  42800

#define LeftError901 90*854/18+TurnError1//左侧第一个1
#define LeftError902 90*480/18+TurnError3//左侧第二个5
#define RightError901 90*854/18+TurnError2//右侧第一个3
#define RightError902 90*480/18+TurnError4//右侧第二个7

#define FrontTurning5Y1 5*854/18+TurnError1
#define FrontTurning5N1 -5*854/18+TurnError1
#define RearTurning5Y1 5*480/18+TurnError3
#define RearTurning5N1 -5*480/18+TurnError3

#define FrontTurning5Y2 5*854/18+TurnError2
#define FrontTurning5N2 -5*854/18+TurnError2
#define RearTurning5Y2 5*480/18+TurnError4
#define RearTurning5N2 -5*480/18+TurnError4

#define LeftTurning90Y1 95*854/18+TurnError1//1
#define LeftTurning90N1 85*854/18+TurnError1
#define LeftTurning90Y2 95*480/18+TurnError3//5
#define LeftTurning90N2 85*480/18+TurnError3

#define RightTurning90Y1 95*854/18+TurnError2//3
#define RightTurning90N1 85*854/18+TurnError2
#define RightTurning90Y2 95*480/18+TurnError4//7
#define RightTurning90N2 85*480/18+TurnError4


#define TurningZero901 -90*854/18+TurnError1//7
#define TurningZero3782 37.83*854/18+TurnError2//7
#define TurningZero903 90*480/18+TurnError3
#define TurningZero3784 -37.83*480/18+TurnError4//7

#define SanorFRLength 60
#define SanorRLLength 60
#define TurnningLimted 96
#define IRLimtedLength 200

//#define MaxRecvLength 2//定义接收发送数据的缓冲区大小
//#define MaxSendLength 2

#define TIMERSENDDATA  1
#define TIMERBDTASK    2   

#define MAXNUMROBOT   3      //机器人数
#define MAXNUMTASK    50     //最大任务数

#define MAXTASKNUM    4      //4位 
#define MAXHEADERNUM  3      //3位
#define MAXLINENUM    100     //文件每行的长度

#define RightSonor1     1
#define RightSonor2      2   

#define RightSonor3      3
#define RightSonor4       4 

#define LeftSonor1       5
#define LeftSonor2      6 

#define LeftSonor3       7
#define LeftSonor4      8 

#define LeaveTimer       10 
#define HoldDelayTimer   2 
#define PositionError      2000
#define MinPositionError    200

extern UINT m_Turnstate;//运行状态
extern UINT DeviceNumber;//记录所走到的设备
extern UINT CTPETMRI;//CT=1;PET=2;MRI=3;
extern int EHoldpostion;//升降架位置
extern int EHoldUppostion;//升降架位置
extern int EHoldDownpostion;//升降架位置
/////////////////////////////////////////////////////////////////////////////
#endif //__GLOVALDEFINES__H__
/////////////////////////////////////////////////////////////////////////////
