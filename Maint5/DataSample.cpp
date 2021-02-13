// DataSample.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "DataSample.h"
#include "MaintDLL.h"
#include "PetApi.h"
#include "G_Fuction.h"
#include "MaintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_RECVEIVE_FULL WM_USER+101


UINT  ReadProcDSample(LPVOID  lpData);
//HANDLE hReadHandleDSample;
CWinThread *g_pThread_DataSample;

//extern bool UCRC(unsigned char*buff);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int	g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

bool g_bRunThreadDSample;
unsigned char g_chFileType;

int g_DataSample_Mod_Sel = 0xc7;

int	g_SamplingTime = 0;
int  g_SampleMaxTime = 0;

/////////////////////////////////////////////////////////////////////////////
// CDataSample dialog


CDataSample::CDataSample(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSample::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSample)
	m_Edit_recCounter = _T("");	
	m_strSetTime = _T("");
	m_strSetCount = _T("");
	m_Static_RecStatus = _T("");
	//}}AFX_DATA_INIT

	m_iModel_Sel = 0xC7;
}


void CDataSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSample)
	DDX_Control(pDX, IDC_RADIO_SDOT, m_Mod_Sel_Sdot);

	DDX_Control(pDX, IDC_RADIO_EDOT, m_Mod_Sel_Edot);
	DDX_Control(pDX, IDC_RADIO_TDOT, m_Mod_Sel_Tdot);
	DDX_Control(pDX, IDC_RADIO_FUHE0, m_Mod_Sel_Fuhe0);
	DDX_Control(pDX, IDC_RADIO_FUHE1, m_Mod_Sel_Fuhe1);


	DDX_Control(pDX, IDC_SETTIME, m_edit_SetTime);
	DDX_Control(pDX, IDC_SETCOUNT, m_edit_SetCount);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_BLOCK_SEL, m_Block_Sel);
	DDX_Control(pDX, IDC_MSCOMM, m_MSComm);
	DDX_Text(pDX, IDC_REC_COUNT, m_Edit_recCounter);
	DDX_Text(pDX, IDC_SETTIME, m_strSetTime);
	DDX_Text(pDX, IDC_SETCOUNT, m_strSetCount);
	DDX_Text(pDX, IDC_STATIC_RECSTATUS, m_Static_RecStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSample, CDialog)
	//{{AFX_MSG_MAP(CDataSample)
	ON_BN_CLICKED(IDC_START_SAMPLE, OnStartSample)
	ON_BN_CLICKED(IDC_STOP_SAMPLE, OnStopSample)
	ON_BN_CLICKED(IDC_COUNTER_REDRAW, OnCounterRedraw)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_BN_CLICKED(IDC_RADIO_SDOT, OnRadioSdot)
	ON_BN_CLICKED(IDC_RADIO_FUHE1, OnRadioFuhe1)
	ON_BN_CLICKED(IDC_RADIO_FUHE0, OnRadioFuhe0)
	ON_BN_CLICKED(IDC_RADIO_EDOT, OnRadioEdot)
	ON_BN_CLICKED(IDC_RADIO_TDOT, OnRadioTdot)
	ON_BN_CLICKED(IDC_UPLOAD_DATA, OnUploadData)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AUTO_SAMPLE, OnAutoSample)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_RECVEIVE_FULL, OnRecveiveFull)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSample message handlers

BEGIN_EVENTSINK_MAP(CDataSample, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataSample)
	ON_EVENT(CDataSample, IDC_MSCOMM, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataSample::OnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	VARIANT vResponse;
	int		k;
	CByteArray	array;
	unsigned char *str1;
	unsigned char UCRC;
	unsigned char Redata[7] = {0x80,0xb0,0x07,0xcd,0x00,0xfa,0x88};														
	
	if (m_MSComm.GetCommEvent() == 2) {  // ComEvReceive==2 接收到字节
		k=m_MSComm.GetInBufferCount();
		if (k>0) {		
			m_MSComm.SetInputLen(k);
			vResponse=m_MSComm.GetInput();   //读取接收到的数据
			str1 = (unsigned char *)vResponse.parray->pvData;

			for(int i=0;i<k;i++) m_recstrfile[m_lrecFileCounter+i] = str1[i];
			m_lrecCount += k;
			m_lworkCounter +=k;

			if(bRecDataToFile) 	{
				m_lrecFileCounter +=k;
				if(m_lrecFileCounter >= 8*1024+7) {//8*1024+7

					frecfile.Write(&m_recstrfile[5],8*1024);
					m_lrecFileCounter = 0;
					int nBucketindex=m_Bucket_Sel.GetCurSel();
					g_Bucketindex = m_Bucket_Sel.GetCurSel();
					Redata[0] = (unsigned char)nBucketindex + 0x80;
					UCRC = Redata[0];
					for(int i=1;i<5;i++) UCRC ^= Redata[i];
					Redata[5] = UCRC;
					SendCommand(Redata,7); // 0x80 的地址
					

					if(m_lrecCount < (8*1024+7)*8 ) m_Static_RecStatus += " .";
					else {
						m_strSetCount.Empty();
						//m_strSetTime.Empty();
						m_Static_RecStatus = " 数据采样结束了 ！";
						GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
						GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);
						GetDlgItem(IDOKe)->EnableWindow(true);
					}
				}	
			}
			else if(m_lworkCounter >= 8) {
				if(str1[5] == 0x55) {
					KillTimer(1);
					m_Static_RecStatus = " 数据采样结束了 ！";
					GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
					UpdateData(false);					
				}
				m_lworkCounter = 0;
			}
			
			m_Edit_recCounter.Format("%d",m_lrecCount);	
			UpdateData(false);

		};
	};
	
}

BOOL CDataSample::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Bucket_Sel.SetCurSel(g_Bucketindex);//
	m_Block_Sel.SetCurSel(g_Blockindex);

	switch(g_DataSample_Mod_Sel) {
		case 0xC5:  m_Mod_Sel_Edot.SetCheck(1);			
			break;
		case 0xC6:  m_Mod_Sel_Tdot.SetCheck(1);
			break;
		case 0xC7:  m_Mod_Sel_Sdot.SetCheck(1);
			break;
		case 0xC8:  m_Mod_Sel_Fuhe0.SetCheck(1);
			break;
		case 0xC9:  m_Mod_Sel_Fuhe1.SetCheck(1);
			break;

		default: break;
	}

	m_lrecCount = 0;
	m_lworkCounter = 0;
	m_lrecFileCounter = 0;
	bRecDataToFile = false;
	bCloseFile = false;	

	m_EnClearBuff = true;
	m_bSample_UploadDataing = false;

	m_PollingNo = 0;

	VERIFY (m_Sample_progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,CRect(20,360,280,380),this, 1));

#ifdef _SELECT_232_PORT
	if(!OpenShmMaint232(g_Port_Sel,g_HostBaud)) {
		MessageBox("Communication Port Open Error!");
		return true;
	}
#else
	if(!OpenShmMaintain()) {
		AfxMessageBox("OpenShmMaint Error");
		return false;
	}
#endif	

	g_pThread_DataSample = AfxBeginThread(ReadProcDSample,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_DataSample->m_bAutoDelete = FALSE;
	g_pThread_DataSample->ResumeThread();



	pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);


	ResetEvent(g_hEvtRec);
	g_bEvtSend = false;	

	CString strtext;
	if(g_SampleMaxTime > 0) {			
			strtext.Format("%d",g_SampleMaxTime/30);
			SetDlgItemText(IDC_SETTIME,strtext);
			m_progress_time = g_SampleMaxTime;
			m_sample_time = g_SamplingTime;
			m_Sample_progress.SetRange(0, m_progress_time);
			m_Sample_progress.SetStep(1);
			m_Sample_progress.SetPos(m_sample_time);
			
			GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(false);
			GetDlgItem(IDC_START_SAMPLE)->EnableWindow(false);

			SetTimer(3,2000,NULL);			
	}
	else {
		m_Static_RecStatus = "\n正常操作指示";
		KillTimer(2);
		UpdateData(false);	
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataSample::OnStartSample() 
{

	UpdateData(true);
	UpdateData(false);	
	int nBucketindex=m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex


	m_lrecCount = 0;
	m_lrecFileCounter = 0;
	m_recstrfile[2] = 4;
	m_Edit_recCounter.Empty();
	m_EnClearBuff = true;

	bRecDataToFile = false;
	if(bCloseFile) {
		frecfile.Close();
		bCloseFile = false;
	}	

	m_iModel_Sel = g_DataSample_Mod_Sel;
	if(!m_strSetCount.IsEmpty()&&(m_iModel_Sel != 0)) { //计数模式
	
			strSdotData[0]=0x70;//nBucketindex + 0x80;	
			strSdotData[1]=0Xb0;      
			strSdotData[2]=0X0b;
			strSdotData[3]=(unsigned char)m_iModel_Sel; //m_iModel_Sel;
			strSdotData[4]=0x01; 		  
			strSdotData[5]=(unsigned char)(_atoi64(m_strSetCount)%0x100); 
			strSdotData[6]=(unsigned char)(_atoi64(m_strSetCount)/0x100);
			strSdotData[7]=(unsigned char)(_atoi64(m_strSetCount)/0x10000);
			strSdotData[8]=(unsigned char)(_atoi64(m_strSetCount)/0x1000000);
			strSdotData[9]=strSdotData[0];	
			for(int j=1;j<9;j++)
			  strSdotData[9]^=strSdotData[j];
			strSdotData[10]=0X88;

			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(11,strSdotData);
			#else
				int nWR = WriteCommandMaintainMod(11,strSdotData);
			#endif

			if(nWR == false) {
					AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}

			m_Static_RecStatus = "\n正在采集数据... \n (请等待)";

			g_SamplingTime = 0;
			g_SampleMaxTime = 99999000;
			m_sample_time = 0;
			m_progress_time = 99999000;
			m_Sample_progress.SetRange(0, m_progress_time);
			m_Sample_progress.SetStep(1);
			m_lworkCounter = 0;

			//ASSERT(SetTimer(3,2000,NULL)!=0);
			SetTimer(3,2000,NULL);

			GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(false);
			GetDlgItem(IDC_START_SAMPLE)->EnableWindow(false);
			UpdateData(false);
	}
	else if(!m_strSetTime.IsEmpty()&&(m_iModel_Sel != 0)) {   //时间模式
			
			strSdotData[0]= 0x70;//nBucketindex	
			strSdotData[1]=0xb0;      
			strSdotData[2]=0x0b;
			strSdotData[3]=(unsigned char)m_iModel_Sel;  //m_iModel_Sel;
			strSdotData[4]=0x02; //m_iModel_Sel;		  
			strSdotData[5]=(unsigned char)(_atoi64(m_strSetTime)%0x100); 
			strSdotData[6]=(unsigned char)(_atoi64(m_strSetTime)/0x100);
			strSdotData[7]=(unsigned char)(_atoi64(m_strSetTime)/0x10000);
			strSdotData[8]=(unsigned char)(_atoi64(m_strSetTime)/0x1000000);
			strSdotData[9]=strSdotData[0];	
			for(int j=1;j<9;j++)
			  strSdotData[9]^=strSdotData[j];
			strSdotData[10]=0x88;

			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(11,strSdotData);
			#else
				int nWR = WriteCommandMaintainMod(11,strSdotData);
			#endif

			if(nWR == false) {
					AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}

			m_lworkCounter = 0;
			//ASSERT(SetTimer(3,2000,NULL)!=0);
			SetTimer(3,2000,NULL);
			m_Static_RecStatus = "\n正在采集数据...\n (请等待)";
			GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(false);
			GetDlgItem(IDC_START_SAMPLE)->EnableWindow(false);
			UpdateData(false);

			m_progress_time = _atoi64(m_strSetTime); 
			m_progress_time *= 30;
			m_sample_time = 0;
			m_Sample_progress.SetRange(0, m_progress_time);
			m_Sample_progress.SetStep(1);
			g_SamplingTime = 0;
			g_SampleMaxTime = m_progress_time;

	}
	else {
		m_Static_RecStatus = "    要先进行\n    工作参数参数初始化";
		UpdateData(false);			
	}

	m_bSample_UploadDataing = false;

//	AfxGetApp()->m_pMainWnd->SetTimer(1,2000,NULL);


}

void CDataSample::OnStopSample() 
{

	KillTimer(3);
	m_bSample_UploadDataing = false;

	unsigned char Stop[8]=	{0x70,0xb0,0x07,0xca,0x00,0x0d,0x88};
	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(7,Stop);
	#else
		int nWR = WriteCommandMaintainMod(7,Stop);
	#endif

	if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	Sleep(300);

	m_lworkCounter = 0;
	m_lrecFileCounter = 0;
	m_recstrfile[2] = 4;
	m_Edit_recCounter.Empty();

	bRecDataToFile = false;

	if(bCloseFile)	{
		bCloseFile = false;
		bRecDataToFile = false;
		frecfile.Close();
		m_strSetCount.Empty();
		m_Static_RecStatus = "\n    数据采样结束！";
		UpdateData(false);		
	}
	else {
		m_strSetCount.Empty();
		m_Static_RecStatus = "\n    停止数据采样 ！";
		UpdateData(false);	
	}

	GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
	GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);
	GetDlgItem(IDOKe)->EnableWindow(true);

	m_Sample_progress.SetRange(0, 0);
	m_sample_time = 0;
	g_SampleMaxTime = 0;

	KillTimer(2);//akang 01 18
	ResetEvent(g_hEvtRec);
	g_bEvtSend = false;			

}

void CDataSample::OnCounterRedraw() 
{
	m_lrecCount = 0;
	m_Edit_recCounter.Empty();
	UpdateData(false);
	
}

void CDataSample::OnOKe() 
{
	if (m_bSample_UploadDataing) {
		OnStopSample();		 
	}	
	KillTimer(2);

	Sleep(50);
	if(bCloseFile)	{
		bCloseFile = false;
		frecfile.Close();
	};
	KillTimer(3);
	Sleep(50);

//	AfxGetApp()->m_pMainWnd->KillTimer(1);

	if(g_bRunThreadDSample) {
		SetEvent(g_hEvtRec);
		g_bEvtSend = false;
		g_bRunThreadDSample = false;
		Sleep(1200);
		delete g_pThread_DataSample;
	}

	CDialog::OnOK();	
}

void CDataSample::SendCommand(unsigned char *CommandStr, int SendNum)
{
	CByteArray Sarray;   
	
    Sarray.RemoveAll();
    Sarray.SetSize(SendNum);

    for( int i=0;i<SendNum;i++)
	    Sarray.SetAt(i,CommandStr[i]);

	if (!m_MSComm.GetPortOpen())    //打开串口并发送数据
					m_MSComm.SetPortOpen(true);
    m_MSComm.SetOutput(COleVariant(Sarray));

}

void CDataSample::OnRadioSdot() 
{
	m_iModel_Sel = 0xC7; //1;
	g_DataSample_Mod_Sel = 0xC7;
	
}

void CDataSample::OnRadioFuhe1() 
{
	m_iModel_Sel = 0xC9;  //5;
	g_DataSample_Mod_Sel = 0xC9;
	
}

void CDataSample::OnRadioFuhe0() 
{
	m_iModel_Sel = 0xC8;  //4;
	g_DataSample_Mod_Sel = 0xC8;
	
}

void CDataSample::OnRadioEdot() 
{
	m_iModel_Sel = 0xC5;  //2;
	g_DataSample_Mod_Sel = 0xC5;
	
}

void CDataSample::OnRadioTdot() 
{
	m_iModel_Sel = 0xC6;  //3;
	g_DataSample_Mod_Sel = 0xC6;
	
}

void CDataSample::OnUploadData() 
{
	int nBucketindex=m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

	if(!bCloseFile) {
		if( !frecfile.Open( "recfile.dat", CFile::modeCreate | CFile::modeWrite) ) {
		}
		frecfile.SetLength((128*4 +4)*1024);
		unsigned char chartemp[128];
		chartemp[0] = 0x01;
		chartemp[1] = 0x01;
		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		frecfile.Write(chartemp,2);

		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		//初始化默认位置边界值
		frecfile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );		
		unsigned char position_X0[8][8];
		unsigned char position_Y0[8][8];
		unsigned char position_X1[8][8];
		unsigned char position_Y1[8][8];
		unsigned char chrtext[4];		
		for(int block=0;block<8;block++) {
			for(int i=0;i<8;i++) {
				for(int j=0;j<8;j++) {
					position_X0[i][j] = j*8; 
					position_Y0[i][j] = i*8;
					position_X1[i][j] = (j+1)*8 -1;
					position_Y1[i][j] = (i+1)*8 -1;
					
					chrtext[0] = position_X0[i][j];
					chrtext[1] = position_Y0[i][j];
					chrtext[2] = position_X1[i][j];
					chrtext[3] = position_Y1[i][j];
					frecfile.Write(chrtext,4);
				}
			}
		}
		frecfile.Seek(0,CFile::begin);
		//初始化能谱边界
		chrtext[0] = 0x01;
		frecfile.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
		frecfile.Write(chrtext,1);
		frecfile.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
		chrtext[0] = 5;
		chrtext[1] = 62;		
		for(block=0;block<8;block++) {
			for(int i=0;i<64;i++) {					
				frecfile.Write(chrtext,2);				
			}	
		}
		frecfile.Seek(0,CFile::begin);

		switch(g_DataSample_Mod_Sel) {
			case 0xc7: 
						frecfile.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin);
						chartemp[0] = 0x01;
						frecfile.Write(chartemp,1);	
						frecfile.Seek(DATA_SDOT_ADDR,CFile::begin);
						break;
			case 0xc5:	
						frecfile.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin);
						chartemp[0] = 0x01;
						frecfile.Write(chartemp,1);
						frecfile.Seek(DATA_ESPECT_ADDR,CFile::begin);
						break;
			case 0xc6:  
						frecfile.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
						chartemp[0] = 0x01;
						frecfile.Write(chartemp,1);
						frecfile.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
						break;
			case 0xc8:	
						frecfile.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
						chartemp[0] = 0x01;
						frecfile.Write(chartemp,1);
						frecfile.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
						g_chFileType = 0xc8;
						break;
			default:	g_chFileType = 0x00;
						break;
		}

		bRecDataToFile = true;
		bCloseFile = true;

		g_chFileType = (unsigned char)m_iModel_Sel; 
	}
	else {
		AfxMessageBox("文件操作失败！，注意操作规范");
		return;
	}

	m_EnClearBuff = false;   //akang ????????

	g_bEvtSend = true;
	ResetEvent(g_hEvtRec);
	Sleep(300);
//	ClearCommRecBuff();

	
	g_bEvtSend = true;
	SetEvent(g_hEvtRec);

	//ASSERT(SetTimer(2,1000,NULL)!=0);
	#ifdef _SELECT_232_PORT
		SetTimer(2,TIMEOUT/2,NULL);
	#else
		SetTimer(2,TIMEOUT*5,NULL);
	#endif
	

	m_lrecCount = 0;
	m_lworkCounter = 0;
	m_lrecFileCounter = 0;
	m_Edit_recCounter.Empty();

	strSdotData[0]= nBucketindex + 0x80;//0x70;	
	strSdotData[1]=0Xb0;      
	strSdotData[2]=0X08;
	strSdotData[3]=0Xcb;			
	strSdotData[4]=0x01; 		  
	strSdotData[5]=0x00; 

	strSdotData[6]=strSdotData[0];	
	for(int j=1;j<6;j++)
		  strSdotData[6]^=strSdotData[j];
	strSdotData[7]=0X88;

	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(8,strSdotData);
	#else
		int nWR = WriteCommandMaintainMod(8,strSdotData);
	#endif

	if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	m_strSetCount.Empty();
	m_Static_RecStatus = "\n正在接收数据...\n (请等待)";

	char chtemp[100];
	sprintf(chtemp,"Send=> ");
	for(int i=0;i<8;i++) {
		sprintf(chtemp,"%s %.2x",chtemp,strSdotData[i]);
	}
	pStatus->SetPaneText(2,chtemp,1);
			

	GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(false);
	GetDlgItem(IDC_START_SAMPLE)->EnableWindow(false);	
	GetDlgItem(IDOKe)->EnableWindow(false);

	UpdateData(false);

	m_Sample_progress.SetRange(0, 0);
	m_Sample_progress.SetRange(0, 8);
	m_Sample_progress.SetStep(1);
	m_sample_time = 0;

	m_bSample_UploadDataing = true;
	
}

void CDataSample::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent) {
		case 1: 
			break;
		case 2:	
				KillTimer(2);			
				//EnterCriticalSection(&g_ClkRecSafe);				
				m_lworkCounter = 0;
				m_lrecFileCounter = 0;
				
				ResetEvent(g_hEvtRec);
				g_bEvtSend = false;			
				//LeaveCriticalSection(&g_ClkRecSafe);

				if(bCloseFile)	{
					bCloseFile = false;
					bRecDataToFile = false;
					frecfile.Close();
				}
				m_Static_RecStatus = "\n接收超时！";
				m_EnClearBuff = true;  //// akang ??????

				m_bSample_UploadDataing = false;
 
				UpdateData(false);

				GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
				GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);
				GetDlgItem(IDOKe)->EnableWindow(true);

				break;		
		case 3:	{
				m_Sample_progress.StepIt();
				m_sample_time ++;

				CString strtext;
				strtext.Format("已经采样时间：\n   %d分 %d 秒", (int)(2*m_sample_time/60),((m_sample_time*2)%60));
				m_Static_RecStatus = strtext;
				UpdateData(false);

				if(m_sample_time > m_progress_time) {  //多采一个定时时间

					m_sample_time --; // akang 
					m_Sample_progress.SetRange(0, 0);
					int nBucketindex = m_Bucket_Sel.GetCurSel();
					g_Bucketindex = m_Bucket_Sel.GetCurSel();					

					strSdotData[0]=0x80	+ m_PollingNo;				//nBucketindex + 0x80;//0x70;
					m_PollingNo ++;
					if(m_PollingNo >=32) {   // m_PollingNo >=32
						m_PollingNo = 0;						
					}
					strSdotData[1]=0Xb0;      
					strSdotData[2]=0X08;
					strSdotData[3]=0Xcc;				
					strSdotData[4]=0x01; 		  
					strSdotData[5]=0x00; 
					strSdotData[6]=strSdotData[0];
					for(int j=1;j<6;j++)
					  strSdotData[6]^=strSdotData[j];
					strSdotData[7]=0X88;

					m_EnClearBuff = false;
				/*	if(m_EnClearBuff) {
						m_EnClearBuff = false;
						g_bEvtSend = true;
						ResetEvent(g_hEvtRec);
						Sleep(200);
						ClearCommRecBuff();
					}
				*/

					SetEvent(g_hEvtRec);

					//ASSERT(SetTimer(2,1500,NULL)!=0);

					#ifdef _SELECT_232_PORT
						int nWR = WriteCommandMaint232(8,strSdotData);
					#else
						int nWR = WriteCommandMaintainMod(8,strSdotData);
					#endif

					if(nWR == false) {
							AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
					}
				//	ASSERT(SetTimer(3,2000,NULL)!=0);

				}
				break;
			};

		default: break;
	};	

	UpdateData(false);

	CString strtemp;
	strtemp.Format("%d",m_lrecCount);
	SetDlgItemText(IDC_REC_COUNT,strtemp);

	CDialog::OnTimer(nIDEvent);
}

UINT  ReadProcDSample(LPVOID  lpData)
{
	unsigned char Redata[7] = {0x80,0xb0,0x07,0xcd,0x00,0xfa,0x88};	
	unsigned char Stop[7]=	{0x70,0xb0,0x07,0xca,0x00,0x0d,0x88};

	unsigned char* data;
	data = new unsigned char[9*1024];

//	unsigned char data[9000];
	int size;	
	int i;
	CString strtemp;
	int temp;

	CDataSample *ph=(CDataSample *)lpData;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	ph->m_lworkCounter = 0;
	ph->m_lrecFileCounter = 0;
	ph->m_recstrfile[2] = 4;
	g_bRunThreadDSample = true;
	while (g_bRunThreadDSample)
	{

//		WaitForSingleObject(g_hEvtRec, -1);		
			
	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif
		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadDSample = false;
		}

		if(ph->m_EnClearBuff) {  size = 0; };  //屏蔽接收用的 akang

		if(size > 0) {
			//EnterCriticalSection(&g_ClkRecSafe);
			for( i=0;i<size;i++) {
				ph->m_recstrfile[ph->m_lworkCounter+i] = data[i];
			}
			ph->m_lrecCount += size;
			ph->m_lworkCounter += size;

			if(ph->bRecDataToFile) 	{	
				ph->KillTimer(3);
				ph->KillTimer(2);
				if(ph->m_lworkCounter >= 8*1024+7) {//8*1024+7

					if( !UCRC(ph->m_recstrfile) ) {  //UCRC
						ph->m_lworkCounter = 0;
						ph->m_lrecFileCounter = 0;

						ph->KillTimer(2);
						ResetEvent(g_hEvtRec);	
						g_bEvtSend = false;
						AfxMessageBox(" 8K 接收数据 CRC 错误!");
					}else {	
								ph->frecfile.Write(&(ph->m_recstrfile[5]),8*1024);					
								ph->m_lrecFileCounter += 1;//ph->m_lworkCounter;	
								if(ph->m_lworkCounter >8*1024+7) ph->m_lworkCounter = ph->m_lworkCounter -8*1024+7;
								else ph->m_lworkCounter = 0;

								temp = ph->m_lrecFileCounter;
								ph->m_Sample_progress.StepIt();

								if(ph->m_lrecFileCounter < 8 ) {//(8*1024+7)*8						
									ph->PostMessage(WM_RECVEIVE_FULL, 20, 0);
								}
								else {
									ph->m_lrecFileCounter = 0;

									temp = ph->m_lrecFileCounter;
									if(ph->bCloseFile) {
										ph->frecfile.Close();
										ph->bCloseFile = false;	
										ph->bRecDataToFile = false;
									}									
									
									ph->KillTimer(3);
									Sleep(50);
									ph->KillTimer(2);	
									Sleep(50);
									ResetEvent(g_hEvtRec);	
									g_bEvtSend = false;

									ph->m_EnClearBuff = true;  //akang ?????

									ph->m_Static_RecStatus = " 上传 数据 结束了 ！";
									ph->m_Sample_progress.SetRange(0, 0);
									ph->m_strSetCount.Empty();
									//ph->m_strSetTime.Empty();
									ph->SetDlgItemText(IDC_STATIC_RECSTATUS,ph->m_Static_RecStatus);
									ph->GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
									ph->GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);
									ph->GetDlgItem(IDOKe)->EnableWindow(true);
								}
					} //end ucrc

				}	
			}
			else if(ph->m_lworkCounter >= 8) {
				if( !UCRC(ph->m_recstrfile) ) {  //UCRC					   
						AfxMessageBox(" 接收数据 UCRC 错误!");
				}else {	
						if(ph->m_recstrfile[5] == 0x55) {
							#ifdef _SELECT_232_PORT
								int nWR = WriteCommandMaint232(7,Stop);
							#else
								int nWR = WriteCommandMaintainMod(7,Stop);
							#endif																
							
							ph->m_EnClearBuff = true;  ///akang   ?????
 

							ph->m_strSetCount.Empty();
							//ph->m_strSetTime.Empty();
							ph->m_Static_RecStatus = " 数据采样结束了 ！";						
							ph->SetDlgItemText(IDC_STATIC_RECSTATUS,ph->m_Static_RecStatus);
							ph->GetDlgItem(IDC_UPLOAD_DATA)->EnableWindow(true);
							ph->GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);
							ph->GetDlgItem(IDOKe)->EnableWindow(true);

							ph->m_Sample_progress.SetRange(0, 0);
							ph->m_sample_time = 0;
						}
						else {							
							ph->SetDlgItemText(IDC_STATIC_RECSTATUS,"数据采样时\n收到意外数据包");
						}
				}; // end of ucrc
				ph->m_recstrfile[2] = 4; //接收缓冲复位
				ph->m_lworkCounter = 0;
				ph->m_lrecFileCounter = 0;

				ph->KillTimer(2);
				ph->KillTimer(3);
				ResetEvent(g_hEvtRec);	
				g_bEvtSend = false;

			}			
			strtemp.Format("%d",ph->m_lrecCount);
			ph->SetDlgItemText(IDC_REC_COUNT,strtemp);
			//LeaveCriticalSection(&g_ClkRecSafe);

		}
		else Sleep(5);  //akang 11-17
	}
#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif

	delete [] data;

	return 0;
}

void CDataSample::OnClose() 
{
	if (m_bSample_UploadDataing) {
		OnStopSample();
	}	

	KillTimer(2);
	Sleep(50);
	SetEvent(g_hEvtRec);	
	KillTimer(3);
	Sleep(50);

//	AfxGetApp()->m_pMainWnd->KillTimer(1);

	if(g_bRunThreadDSample) {
		SetEvent(g_hEvtRec);
		g_bEvtSend = false;
		g_bRunThreadDSample = false;
		Sleep(500);
		delete g_pThread_DataSample;
	}

	CDialog::OnClose();

}


LRESULT CDataSample::OnRecveiveFull(WPARAM wParam, LPARAM lParam)
{
	unsigned char UCRC;
	unsigned char Redata[7] = {0x80,0xb0,0x07,0xcd,0x00,0xfa,0x88};	//要求继续上传8K的 数据

	int nBucketindex = m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();

	Redata[0] = (unsigned char)nBucketindex + 0x80;
	UCRC = Redata[0];
	for(int i=1;i<5;i++) UCRC ^= Redata[i];
	Redata[5] = UCRC;

	//for(int delay=0;delay<100000;delay++) ;
	Sleep(100);
	
#ifdef _SELECT_232_PORT
	int nWR = WriteCommandMaint232(7,Redata);
#else
	int nWR = WriteCommandMaintainMod(7,Redata);
#endif
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	char chtemp[100];
	sprintf(chtemp,"Send=> ");
	for(i=0;i<7;i++) {
		sprintf(chtemp,"%s %.2x",chtemp,Redata[i]);
	}
	pStatus->SetPaneText(2,chtemp,1);

	return 0L;
}

void CDataSample::OnDestroy() 
{
//	OnStopSample();

	KillTimer(2);		
	Sleep(50);								
	if(bCloseFile)	{
		bCloseFile = false;
		frecfile.Close();
	};
	KillTimer(3);
	Sleep(50);
//	AfxGetApp()->m_pMainWnd->KillTimer(1);
	if(g_bRunThreadDSample) {
		SetEvent(g_hEvtRec);
		g_bEvtSend = false;
		g_bRunThreadDSample = false;
		Sleep(500);
		delete g_pThread_DataSample;
	}
	CDialog::OnDestroy();
}


void CDataSample::OnAutoSample() 
{


	
}
