
//=======================��־�������=====================================
int WINAPI OpenLogSH();//����־�����ڴ� 1�ɹ� 0ʧ��
int WINAPI CloseLogSH();//�ر���־�����ڴ� 1�ɹ� 0ʧ��
int WINAPI WrtClinicLog(char* fmt, ...);//д���ٴ������־,1�ɹ�,0ʧ��
int WINAPI WrtDebugLog(char* fmt, ...);//д�������־,1�ɹ�,0ʧ��
int WINAPI SetAliveLog(int sec);//д����ʱ��(��)
//========================================================================

//======================�ɼ��������======================================
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
	int number_protocol;//��n��Э��
	int frame_number;	//��ǰ֡��
	int  frame_start_time;	//��ǰ֡�Ŀ�ʼʱ�䣨���룩
	int frame_class;	//��ǰ֡�����
	float frame_bedposition;	 //��ǰ����λ��
	int  delayed;	//��֡���ſ��е��ӳټ���
	int multiples;	//��֡���ſ��еĶ��ؼ���
	int net_trues;	//��ʵ����
	float wobble_tilt;	//��Դ��ת���ٶȣ���/�֣�
	int coincidence_dif_data;	//��������
	int start;	//�ɼ���ʼ��־(1=start,0=no)
				//���ƶ���־(1=move,0=no)
				//��Դ״̬(0=δ֪.1=�����λ��2�����ص�λ��3����������У�4�����ع����У�5��������ת��6�����ƽ��ת)
				//SEPTA״̬��0=δ֪.1=�����λ��2�����ص�λ��3����������У�4�����ع����У�5��������ת��6�����ƽ��ת��
};
int WINAPI OpenShmAcquire();//���Ӳɼ�����,1���ɹ���0��ʧ��
int WINAPI	CloseShmAcquire();//�Ͽ��ɼ�����1���ɹ���0��ʧ��
void WINAPI	WriteCommandAcquire(ACQ_COMMAND);//д�ɼ�����,���룺������
void WINAPI ReadMonitorAcquire(PET_MONITOR *);//����������
ACQ_RESULT WINAPI	ReadResultAcquire();//���ɼ����
//========================================================================

//======================�ɼ�Э�����======================================
int WINAPI OpenShmProtocolAcquire();//���Ӳɼ�Э��
int WINAPI CloseShmProtocolAcquire();//�Ͽ��ɼ�Э��
int WINAPI WriteNumProtocolAcquire(int);//д�ɼ�Э����
int WINAPI WriteProtocolAcquire(int,BYTE*, int);//д�ɼ�Э��
//========================================================================

//=======================���ܲ����������=================================
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
	int number_protocol;//��n��Э��
	int frame_number;	//��ǰ֡��
	int  frame_start_time;	//��ǰ֡�Ŀ�ʼʱ�䣨���룩
	int frame_class;	//��ǰ֡�����
	float frame_bedposition;	 //��ǰ����λ��
	int  delayed;	//��֡���ſ��е��ӳټ���
	int multiples;	//��֡���ſ��еĶ��ؼ���
	int net_trues;	//��ʵ����
	float wobble_tilt;	//��Դ��ת���ٶȣ���/�֣�
	int coincidence_dif_data;	//��������
	int start;	//�ɼ���ʼ��־(1=start,0=no)
				//���ƶ���־(1=move,0=no)
				//��Դ״̬(0=δ֪.1=�����λ��2�����ص�λ��3����������У�4�����ع����У�5��������ת��6�����ƽ��ת)
				//SEPTA״̬��0=δ֪.1=�����λ��2�����ص�λ��3����������У�4�����ع����У�5��������ת��6�����ƽ��ת��
};
int WINAPI OpenShmPerform();//�������ܲ�������,1���ɹ���0��ʧ��
int WINAPI	CloseShmPerform();//�Ͽ����ܲ�������1���ɹ���0��ʧ��
void WINAPI	WriteCommandPerform(PFM_COMMAND);//д���ܲ�������,���룺������
void WINAPI ReadMonitorPerform(PET_MONITORPFM *);//����������
PFM_RESULT WINAPI	ReadResultPerform();//�����ܲ��Խ��
//========================================================================

//======================���ܲ���Э�����==================================
int WINAPI OpenShmProtocolPerform();//�������ܲ���Э��
int WINAPI CloseShmProtocolPerform();//�Ͽ����ܲ���Э��
int WINAPI WriteNumProtocolPerform(int);//д���ܲ���Э����
int WINAPI WriteProtocolPerform(int,BYTE*, int);//д���ܲ���Э��
//========================================================================

//========================ϵͳ��Ϣ����====================================
int WINAPI OpenShmInfo();//����ϵͳ��Ϣ
int WINAPI CloseShmInfo();//�Ͽ�ϵͳ��Ϣ
int WINAPI WriteAllMainHeadInfo(BYTE *);//д������Ϣ��ͷȫ��
int WINAPI WriteAllSubHeadInfo(BYTE *);//д������Ϣ��ͷȫ��
int WINAPI WriteMainHeadInfo(BYTE *,int,int);//д������Ϣ��ͷһ��
int WINAPI WriteSubHeadInfo(BYTE *,int,int);//д������Ϣ��ͷһ��
int WINAPI WriteIsCorrectInfo(BYTE*);//д������Ϣ�Ƿ�ɼ�У��
int WINAPI WriteListmodeInfo(BYTE *);//д����LISTMODE�ļ���
int WINAPI WriteSinoInfo(BYTE *);//д������ϢSino�ļ���
int WINAPI WriteFrameLocation(int,__int64);//д֡�������ļ��е�λ��
int WINAPI WriteCorrectProtocolInfo(BYTE *);//д������ϢУ��Э��
int WINAPI ReadAllMainHeadInfo(BYTE *);//��������Ϣ��ͷȫ��
int WINAPI ReadAllSubHeadInfo(BYTE *);//��������Ϣ��ͷȫ��
int WINAPI ReadMainHeadInfo(BYTE *,int,int);//��������Ϣ��ͷһ��
int WINAPI ReadSubHeadInfo(BYTE *,int,int);//��������Ϣ��ͷһ��
int WINAPI ReadIsCorrectInfo(BYTE*);//��������Ϣ�Ƿ�ɼ�У��
int WINAPI ReadListmodeInfo(BYTE *);//������LISTMODE�ļ���
int WINAPI ReadSinoInfo(BYTE *);//��������ϢSino�ļ���
int WINAPI ReadNumberOfFrames();//��֡��
__int64 WINAPI ReadFrameLocation(int);//��֡�������ļ��е�λ��
int WINAPI ReadCorrectProtocolInfo(BYTE *);//��������ϢУ��Э��
int WINAPI ReadPatientInfo(BYTE*,int,int);//��ϵͳ��Ϣ��������
//========================================================================

//=========================У���������===================================
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
int	WINAPI OpenShmCorrect();//����У������
int WINAPI CloseShmCorrect();//�Ͽ�У������
CORRECT_COMMAND WINAPI ReadCommandCorrect();//��У������
void WINAPI WriteResultCorrect(CORRECT_RESULT);//дУ�����
int WINAPI SetAliveCorrect(int);//���ô��ʱ��
//========================================================================

enum MAN_DEVICE
{
	BUCKET,
	GANTRY,
	ACS
};
//========================�ؽ��������====================================
BYTE* WINAPI OpenShmReconstruct();//�����ؽ�����
int WINAPI CloseShmReconstruct();//�Ͽ��ؽ�����
//========================================================================

//=========================ά������=======================================
int WINAPI OpenShmMaintain(MAN_DEVICE);//����ά������
int WINAPI CloseShmMaintain(MAN_DEVICE);//�Ͽ�ά������
int WINAPI WriteCommandMaintain(MAN_DEVICE,int, BYTE*);//дά������
int WINAPI ReadResultMaintain(MAN_DEVICE,BYTE*);//��ά�����
//========================================================================



//=========================ͨ�ö��б�׼����===============================
//
#define BEATMQKEY 8

//��ͨ�ö���
//���룺�ؼ���
//�������ʶ�ţ�-1������
int	WINAPI	MqOpen (int key);
//�ر�ͨ�ö���
//���룺��ʶ��
//�����0���ɹ���-1������
int	WINAPI	MqClose (int id);
//д����
//���룺��ʶ�ţ����ݿ�
//�����0���ɹ���-1������
int	WINAPI	MqWrite(int id, char* addr);
//������
//���룺��ʶ�ţ����ݿ�
//�����0���ɹ���-1������
int	WINAPI	MqRead(int id, char* addr);
//�˳�������
//���룺��ʶ��
//�����0���ɹ���-1������
int	WINAPI	MqAbort(int id);
//========================================================================
