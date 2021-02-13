
//=======================日志信箱管理=====================================
int WINAPI OpenLogSH();//打开日志共享内存 1成功 0失败
int WINAPI CloseLogSH();//关闭日志共享内存 1成功 0失败
int WINAPI WrtClinicLog(char* fmt, ...);//写入临床检查日志,1成功,0失败
int WINAPI WrtDebugLog(char* fmt, ...);//写入调试日志,1成功,0失败
int WINAPI SetAliveLog(int sec);//写入存活时间(秒)
//========================================================================

//======================采集信箱管理======================================
enum ACQ_COMMAND
{
	ACQ_NOCOMMAND=0,
	ACQ_START,
	ACQ_STOP,
	ACQ_SKIP,
	ACQ_STARTBEAT,
	ACQ_STOPBEAT,
};
enum ACQ_RESULT
{
	ACQ_NORESULT=0,
	ACQ_SUCCESS,
	ACQ_FAIL,
	ACQ_CORRECT,
};
struct PET_MONITOR
{
	int number_protocol;//第n条协议
	int frame_number;	//当前帧数
	int  frame_start_time;	//当前帧的开始时间（毫秒）
	int frame_class;	//当前帧的类别
	float frame_bedposition;	 //当前床的位置
	int  delayed;	//该帧或门控中的延迟计数
	int multiples;	//该帧或门控中的多重计数
	int net_trues;	//真实计数
	float wobble_tilt;	//棒源旋转角速度（度/分）
	int coincidence_dif_data;	//符合数据
	int start;	//采集开始标志(1=start,0=no)
				//床移动标志(1=move,0=no)
				//棒源状态(0=未知.1=伸出到位，2＝缩回到位，3＝伸出过程中，4＝缩回过程中，5＝正在旋转，6＝伸出平旋转)
				//SEPTA状态（0=未知.1=伸出到位，2＝缩回到位，3＝伸出过程中，4＝缩回过程中，5＝正在旋转，6＝伸出平旋转）
};
int WINAPI OpenShmAcquire();//连接采集信箱,1：成功，0：失败
int WINAPI	CloseShmAcquire();//断开采集信箱1：成功，0：失败
void WINAPI	WriteCommandAcquire(ACQ_COMMAND);//写采集命令,输入：命令码
void WINAPI ReadMonitorAcquire(PET_MONITOR *);//读监视数据
ACQ_RESULT WINAPI	ReadResultAcquire();//读采集结果
//========================================================================

//======================采集协议管理======================================
int WINAPI OpenShmProtocolAcquire();//连接采集协议
int WINAPI CloseShmProtocolAcquire();//断开采集协议
int WINAPI WriteNumProtocolAcquire(int);//写采集协议数
int WINAPI WriteProtocolAcquire(int,BYTE*, int);//写采集协议
//========================================================================

//=======================性能测试信箱管理=================================
enum PFM_COMMAND
{
	PFM_NOCOMMAND=0,
	PFM_START,
	PFM_STOP,
	PFM_SKIP,
	PFM_STARTBEAT,
	PFM_STOPBEAT,
};

enum PFM_RESULT
{
	PFM_NORESULT=0,
	PFM_SUCCESS,
	PFM_FAIL,
	PFM_CORRECT,
};
struct PET_MONITORPFM
{
	int number_protocol;//第n条协议
	int frame_number;	//当前帧数
	int  frame_start_time;	//当前帧的开始时间（毫秒）
	int frame_class;	//当前帧的类别
	float frame_bedposition;	 //当前床的位置
	int  delayed;	//该帧或门控中的延迟计数
	int multiples;	//该帧或门控中的多重计数
	int net_trues;	//真实计数
	float wobble_tilt;	//棒源旋转角速度（度/分）
	int coincidence_dif_data;	//符合数据
	int start;	//采集开始标志(1=start,0=no)
				//床移动标志(1=move,0=no)
				//棒源状态(0=未知.1=伸出到位，2＝缩回到位，3＝伸出过程中，4＝缩回过程中，5＝正在旋转，6＝伸出平旋转)
				//SEPTA状态（0=未知.1=伸出到位，2＝缩回到位，3＝伸出过程中，4＝缩回过程中，5＝正在旋转，6＝伸出平旋转）
};
int WINAPI OpenShmPerform();//连接性能测试信箱,1：成功，0：失败
int WINAPI	CloseShmPerform();//断开性能测试信箱1：成功，0：失败
void WINAPI	WriteCommandPerform(PFM_COMMAND);//写性能测试命令,输入：命令码
void WINAPI ReadMonitorPerform(PET_MONITORPFM *);//读监视数据
PFM_RESULT WINAPI	ReadResultPerform();//读性能测试结果
//========================================================================

//======================性能测试协议管理==================================
int WINAPI OpenShmProtocolPerform();//连接性能测试协议
int WINAPI CloseShmProtocolPerform();//断开性能测试协议
int WINAPI WriteNumProtocolPerform(int);//写性能测试协议数
int WINAPI WriteProtocolPerform(int,BYTE*, int);//写性能测试协议
//========================================================================

//========================系统信息管理====================================
int WINAPI OpenShmInfo();//连接系统信息
int WINAPI CloseShmInfo();//断开系统信息
int WINAPI WriteAllMainHeadInfo(BYTE *);//写病人信息主头全部
int WINAPI WriteAllSubHeadInfo(BYTE *);//写病人信息子头全部
int WINAPI WriteMainHeadInfo(BYTE *,int,int);//写病人信息主头一项
int WINAPI WriteSubHeadInfo(BYTE *,int,int);//写病人信息子头一项
int WINAPI WriteIsCorrectInfo(BYTE*);//写病人信息是否采集校正
int WINAPI WriteListmodeInfo(BYTE *);//写病人LISTMODE文件名
int WINAPI WriteSinoInfo(BYTE *);//写病人信息Sino文件名
int WINAPI WriteFrameLocation(int,__int64);//写帧数据在文件中的位置
int WINAPI WriteCorrectProtocolInfo(BYTE *);//写病人信息校正协议
int WINAPI ReadAllMainHeadInfo(BYTE *);//读病人信息主头全部
int WINAPI ReadAllSubHeadInfo(BYTE *);//读病人信息子头全部
int WINAPI ReadMainHeadInfo(BYTE *,int,int);//读病人信息主头一项
int WINAPI ReadSubHeadInfo(BYTE *,int,int);//读病人信息子头一项
int WINAPI ReadIsCorrectInfo(BYTE*);//读病人信息是否采集校正
int WINAPI ReadListmodeInfo(BYTE *);//读病人LISTMODE文件名
int WINAPI ReadSinoInfo(BYTE *);//读病人信息Sino文件名
int WINAPI ReadNumberOfFrames();//读帧数
__int64 WINAPI ReadFrameLocation(int);//读帧数据在文件中的位置
int WINAPI ReadCorrectProtocolInfo(BYTE *);//读病人信息校正协议
int WINAPI ReadPatientInfo(BYTE*,int,int);//读系统信息，调试用
//========================================================================

//=========================校正信箱管理===================================
enum CORRECT_COMMAND
{
	CORRECT_NOCOMMAND=0,
	CORRECT_START,
	CORRECT_STOP
};
enum CORRECT_RESULT
{
	CORRECT_NORESULT=0,
	CORRECT_SUCCESS,
	CORRECT_FAIL,
};
int	WINAPI OpenShmCorrect();//连接校正信箱
int WINAPI CloseShmCorrect();//断开校正信箱
CORRECT_COMMAND WINAPI ReadCommandCorrect();//读校正命令
void WINAPI WriteResultCorrect(CORRECT_RESULT);//写校正结果
int WINAPI SetAliveCorrect(int);//设置存活时间
//========================================================================

enum MAN_DEVICE
{
	BUCKET,
	GANTRY,
	ACS
};
//========================重建矩阵管理====================================
BYTE* WINAPI OpenShmReconstruct();//连接重建信箱
int WINAPI CloseShmReconstruct();//断开重建信箱
//========================================================================

//=========================维护信箱=======================================
int WINAPI OpenShmMaintain(MAN_DEVICE);//连接维护信箱
int WINAPI CloseShmMaintain(MAN_DEVICE);//断开维护信箱
int WINAPI WriteCommandMaintain(MAN_DEVICE,int, BYTE*);//写维护命令
int WINAPI ReadResultMaintain(MAN_DEVICE,BYTE*);//读维护结果
//========================================================================



//=========================通用队列标准函数===============================
//
#define BEATMQKEY 8

//打开通用队列
//输入：关键字
//输出：标识号，-1：出错
int	WINAPI	MqOpen (int key);
//关闭通用队列
//输入：标识号
//输出：0：成功，-1：出错
int	WINAPI	MqClose (int id);
//写队列
//输入：标识号，数据块
//输出：0：成功，-1：出错
int	WINAPI	MqWrite(int id, char* addr);
//读队列
//输入：标识号，数据块
//输出：0：成功，-1：出错
int	WINAPI	MqRead(int id, char* addr);
//退出读队列
//输入：标识号
//输出：0：成功，-1：出错
int	WINAPI	MqAbort(int id);
//========================================================================
