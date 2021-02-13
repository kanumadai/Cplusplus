// FpgaCounter.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "FpgaCounter.h"
#include "G_Fuction.h"

#include "MaintDLL.h"
#include "PetApi.h"
#include "FpgaCountMap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT  ReadProcFpgaCounter(LPVOID  lpData);
HANDLE hReadHandleFpgaCount;
CWinThread *g_pThread_EnFpgaCount;

//extern bool UCRC(unsigned char*buff);
extern bool ClearCommRecBuff(void);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int g_DataSample_Mod_Sel;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

bool g_bRunThreadFpgaCount;


HANDLE m_hSendDelay;

/////////////////////////////////////////////////////////////////////////////
// CFpgaCounter dialog


CFpgaCounter::CFpgaCounter(CWnd* pParent /*=NULL*/)
	: CDialog(CFpgaCounter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFpgaCounter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFpgaCounter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFpgaCounter)
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFpgaCounter, CDialog)
	//{{AFX_MSG_MAP(CFpgaCounter)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SHOWMAP, OnShowmap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFpgaCounter message handlers

void CFpgaCounter::OnOK() 
{

	unsigned char buf[10] = {0x70,0xB0,0x06,0xC1,0x00,0x88};
	int nBuindex=m_Bucket_Sel.GetCurSel();

	if(nBuindex == -1) {
		AfxMessageBox("请选择Bucket或Block！",MB_ICONINFORMATION);
		return;
	}
	
	if (nBuindex == 32) {
		m_bSave32BuckData = true;
		OnSend32BuckCMD();
		return;
	}
	m_bSave32BuckData = false;

	SetDlgItemText(IDC_CHANNEL0_ALL,"00");
	SetDlgItemText(IDC_CHANNEL0_G2S,"00");
	SetDlgItemText(IDC_CHANNEL1_ALL,"00");
	SetDlgItemText(IDC_CHANNEL1_G2S,"00");
	SetDlgItemText(IDC_CHANNEL2_ALL,"00");
	SetDlgItemText(IDC_CHANNEL2_G2S,"00");
	SetDlgItemText(IDC_CHANNEL3_ALL,"00");
	SetDlgItemText(IDC_CHANNEL3_G2S,"00");
	SetDlgItemText(IDC_CHANNEL4_ALL,"00");
	SetDlgItemText(IDC_CHANNEL4_G2S,"00");
	SetDlgItemText(IDC_CHANNEL5_ALL,"00");
	SetDlgItemText(IDC_CHANNEL5_G2S,"00");
	SetDlgItemText(IDC_CHANNEL6_ALL,"00");
	SetDlgItemText(IDC_CHANNEL6_G2S,"00");
	SetDlgItemText(IDC_CHANNEL7_ALL,"00");
	SetDlgItemText(IDC_CHANNEL7_G2S,"00");

	m_RecCounter = 0;
	recbuf[2] = 4;

	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

	buf[0] = g_Bucketindex +0x80;
	unsigned char ucrc=buf[0];
	for(int i=1;i<4;i++) ucrc ^= buf[i];
	buf[4] = ucrc;

	if(g_bEvtSend) {
		AfxMessageBox("正在等待上一条命令的应答\n请稍后再试");
		return;
	}
	KillTimer(2);
	g_bEvtSend = true;
	
	ResetEvent(g_hEvtRec);
	Sleep(300);
	ClearCommRecBuff();

	SetEvent(g_hEvtRec);
	SetTimer(2,2000,NULL);

	Sleep(50);

	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(6,buf);
	#else
		int nWR = WriteCommandMaintainMod(6,buf);
	#endif
	
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}



}

void CFpgaCounter::OnCancel() 
{
	KillTimer(2);

	SetEvent(g_hEvtRec);
	if(g_bRunThreadFpgaCount) {
		g_bRunThreadFpgaCount = false;
		g_bEvtSend = false;
		Sleep(500);
		delete g_pThread_EnFpgaCount;
	}

	CDialog::OnCancel();
}

BOOL CFpgaCounter::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(2);

	SetEvent(g_hEvtRec);
	if(g_bRunThreadFpgaCount) {
		g_bRunThreadFpgaCount = false;
		g_bEvtSend = false;
		Sleep(500);
		delete g_pThread_EnFpgaCount;
	}

	return CDialog::DestroyWindow();
}

BOOL CFpgaCounter::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_BuckIndex = 0;
	m_bSave32BuckData = false;	

	memset(m_arr32BuckData,0x10,32*8*2*4);
	
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

	SetDlgItemText(IDC_CHANNEL0_ALL,"00");
	SetDlgItemText(IDC_CHANNEL0_G2S,"00");
	SetDlgItemText(IDC_CHANNEL1_ALL,"00");
	SetDlgItemText(IDC_CHANNEL1_G2S,"00");
	SetDlgItemText(IDC_CHANNEL2_ALL,"00");
	SetDlgItemText(IDC_CHANNEL2_G2S,"00");
	SetDlgItemText(IDC_CHANNEL3_ALL,"00");
	SetDlgItemText(IDC_CHANNEL3_G2S,"00");
	SetDlgItemText(IDC_CHANNEL4_ALL,"00");
	SetDlgItemText(IDC_CHANNEL4_G2S,"00");
	SetDlgItemText(IDC_CHANNEL5_ALL,"00");
	SetDlgItemText(IDC_CHANNEL5_G2S,"00");
	SetDlgItemText(IDC_CHANNEL6_ALL,"00");
	SetDlgItemText(IDC_CHANNEL6_G2S,"00");
	SetDlgItemText(IDC_CHANNEL7_ALL,"00");
	SetDlgItemText(IDC_CHANNEL7_G2S,"00");

	switch(g_DataSample_Mod_Sel) {
		case 0xC5: SetDlgItemText(IDC_STATIC_G2S,"有效计数"); 		
			break; 
		case 0xC6: SetDlgItemText(IDC_STATIC_G2S,"有效计数");  
			break; 
		case 0xC7: SetDlgItemText(IDC_STATIC_G2S,"有效计数");  
			break;
		case 0xC8: SetDlgItemText(IDC_STATIC_G2S,"光电事件数");
			break;
		case 0xC9: SetDlgItemText(IDC_STATIC_G2S,"有效事件数");
			break;

		default: SetDlgItemText(IDC_STATIC_G2S,"有效计数"); 
			break; 
	}

	m_RecCounter = 0;
	recbuf[2] = 4;
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
    
	//hReadHandleAnol = CreateThread(NULL,0, ReadProcAnol,(LPVOID)this,0,&dwThreadID);	
	g_pThread_EnFpgaCount = AfxBeginThread(ReadProcFpgaCounter,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_EnFpgaCount->m_bAutoDelete = FALSE;
	g_pThread_EnFpgaCount->ResumeThread();


	m_hSendDelay = CreateEvent(NULL,true,FALSE,NULL);
	
	return TRUE;  
	
}


UINT  ReadProcFpgaCounter(LPVOID  lpData)
{
	int size;
	unsigned char data[100];

	CFpgaCounter *ph=(CFpgaCounter *)lpData ;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif	

	g_bRunThreadFpgaCount = true;
	while (g_bRunThreadFpgaCount) 
	{
		WaitForSingleObject(g_hEvtRec, -1);

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif			
		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadFpgaCount = false;
		}
		if(size > 0) {
			
			for(int i=0;i<size;i++) {
				ph->recbuf[ph->m_RecCounter+i] = data[i];
			}
			ph->m_RecCounter +=size;
			
			if(ph->m_RecCounter >= ph->recbuf[2]) { //判断帧长	
				ph->m_RecCounter = 0;	
				if( !UCRC(ph->recbuf) ) {
							ph->m_RecCounter = 0;
							ph->recbuf[2] = 4;								
							ph->SetDlgItemText(IDC_CHANNEL0_ALL,"错误");
							ph->SetDlgItemText(IDC_CHANNEL0_G2S,"UCRC");
				}
				else
				{	
					if (ph->m_bSave32BuckData) 	{
						ph->OnSave32BuckData(ph->recbuf);
					}
					else{
						ph->recbuf[2] = 4;
						ph->OnShowData(ph->recbuf);
						memset(ph->recbuf,0,100);
					}					
				}	
				ph->KillTimer(2);							
				ResetEvent(g_hEvtRec);	
				g_bEvtSend = false;
			}
			//LeaveCriticalSection(&g_ClkRecSafe);
		}
		else Sleep(5);
	}
#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif

	return 0;
}

void CFpgaCounter::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent) {
		case 0:
			break;
		case 1:
			break;
		case 2: //接收超时
			KillTimer(2);			
			//EnterCriticalSection(&g_ClkRecSafe);
			memset(recbuf,0,100);			
			m_RecCounter = 0; 
			recbuf[2] = 4;		
			AfxMessageBox("接收超时了");	
			
			ResetEvent(g_hEvtRec);
			g_bEvtSend = false;		
			//LeaveCriticalSection(&g_ClkRecSafe);
			break;
		default:
			break;
	}

	
	CDialog::OnTimer(nIDEvent);
}

void CFpgaCounter::OnClose() 
{
	KillTimer(2);

	SetEvent(g_hEvtRec);
	if(g_bRunThreadFpgaCount) {
		g_bRunThreadFpgaCount = false;
		g_bEvtSend = false;
		Sleep(500);
		delete g_pThread_EnFpgaCount;
	}

	CDialog::OnClose();
}

void CFpgaCounter::OnDestroy() 
{
	KillTimer(2);
	SetEvent(g_hEvtRec);
	if(g_bRunThreadFpgaCount) {
		g_bRunThreadFpgaCount = false;
		g_bEvtSend = false;
		Sleep(500);
		delete g_pThread_EnFpgaCount;
	}

	CDialog::OnOK();
	
}

void CFpgaCounter::OnShowmap() 
{
	CFpgaCountMap fpgamap;

	memcpy(fpgamap.m_arr32BuckData,m_arr32BuckData,32*8*2*4);	
	fpgamap.DoModal();
}

void CFpgaCounter::OnShowData(unsigned char *pdata)
{
	int num;
	CString strtemp;
	num = pdata[7]*256*256 + pdata[6]*256 + pdata[5];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL0_ALL,strtemp);
	
	num = pdata[10]*256*256 + pdata[9]*256 + pdata[8];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL0_G2S,strtemp);
	
	num = pdata[13]*256*256 + pdata[12]*256 + pdata[11];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL1_ALL,strtemp);
	
	num = pdata[16]*256*256 + pdata[15]*256 + pdata[14];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL1_G2S,strtemp);
	
	num = pdata[19]*256*256 + pdata[18]*256 + pdata[17];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL2_ALL,strtemp);
	
	num = pdata[22]*256*256 + pdata[21]*256 + pdata[20];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL2_G2S,strtemp);
	
	num = pdata[25]*256*256 + pdata[24]*256 + pdata[23];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL3_ALL,strtemp);
	
	num = pdata[28]*256*256 + pdata[27]*256 + pdata[26];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL3_G2S,strtemp);
	
	num = pdata[31]*256*256 + pdata[30]*256 + pdata[29];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL4_ALL,strtemp);
	
	num = pdata[34]*256*256 + pdata[33]*256 + pdata[32];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL4_G2S,strtemp);
	
	num = pdata[37]*256*256 + pdata[36]*256 + pdata[35];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL5_ALL,strtemp);
	
	num = pdata[40]*256*256 + pdata[39]*256 + pdata[38];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL5_G2S,strtemp);
	
	num = pdata[43]*256*256 + pdata[42]*256 + pdata[41];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL6_ALL,strtemp);
	
	num = pdata[46]*256*256 + pdata[45]*256 + pdata[44];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL6_G2S,strtemp);
	
	num = pdata[49]*256*256 + pdata[48]*256 + pdata[47];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL7_ALL,strtemp);
	
	num = pdata[52]*256*256 + pdata[51]*256 + pdata[50];
	strtemp.Format(" %d",num);
	SetDlgItemText(IDC_CHANNEL7_G2S,strtemp);
}

void CFpgaCounter::OnSave32BuckData(unsigned char *pdata)
{
	for(int block=0;block<8;block++){
		m_arr32BuckData[m_BuckIndex][block][0] = pdata[7+ block*6]*256*256 + pdata[6+ block*6]* 256 + pdata[5+ block*6];
		m_arr32BuckData[m_BuckIndex][block][1] = pdata[10+ block*6]*256*256 + pdata[9+ block*6]* 256 + pdata[8+ block*6];
	}
	m_iRecBucketNum ++;
}


void CFpgaCounter::OnSend32BuckCMD()
{
	m_BuckIndex = 0;
	unsigned char buf[10] = {0x70,0xB0,0x06,0xC1,0x00,0x88};

	GetDlgItem(IDOK)->EnableWindow(false);

	SetDlgItemText(IDC_CHANNEL0_ALL,"**");
	SetDlgItemText(IDC_CHANNEL0_G2S,"**");
	SetDlgItemText(IDC_CHANNEL1_ALL,"**");
	SetDlgItemText(IDC_CHANNEL1_G2S,"**");
	SetDlgItemText(IDC_CHANNEL2_ALL,"**");
	SetDlgItemText(IDC_CHANNEL2_G2S,"**");
	SetDlgItemText(IDC_CHANNEL3_ALL,"**");
	SetDlgItemText(IDC_CHANNEL3_G2S,"**");
	SetDlgItemText(IDC_CHANNEL4_ALL,"**");
	SetDlgItemText(IDC_CHANNEL4_G2S,"**");
	SetDlgItemText(IDC_CHANNEL5_ALL,"**");
	SetDlgItemText(IDC_CHANNEL5_G2S,"**");
	SetDlgItemText(IDC_CHANNEL6_ALL,"**");
	SetDlgItemText(IDC_CHANNEL6_G2S,"**");
	SetDlgItemText(IDC_CHANNEL7_ALL,"**");
	SetDlgItemText(IDC_CHANNEL7_G2S,"**");

	UpdateData(false);
	m_iRecBucketNum = 0;
	m_bSave32BuckData = true;

	KillTimer(2);
	g_bEvtSend = true;
	
	ResetEvent(g_hEvtRec);
	Sleep(300);
	ClearCommRecBuff();
	SetEvent(g_hEvtRec);

	memset(m_arr32BuckData,0,32*8*2*4);	
	for(int bucket=0;bucket<32;bucket++) {
		m_RecCounter = 0;
		recbuf[2] = 4;
		
		m_BuckIndex = bucket;  ////////
		buf[0] = bucket +0x80;
		unsigned char ucrc=buf[0];
		for(int i=1;i<4;i++) ucrc ^= buf[i];
		buf[4] = ucrc;
		
	   KillTimer(2);
	   g_bEvtSend = true;
	
	   ResetEvent(g_hEvtRec);
	   Sleep(30);
	   SetEvent(g_hEvtRec);
		
	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(6,buf);
	#else
		int nWR = WriteCommandMaintainMod(6,buf);
	#endif		
		if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}
		//Sleep(2000);		
		ResetEvent(m_hSendDelay);
	    WaitForSingleObject(m_hSendDelay,100);
	}

	GetDlgItem(IDOK)->EnableWindow(true);

	char chtext[50];
	if (m_iRecBucketNum >= 32) {
		AfxMessageBox("32Bucket数据接受成功！");
	}
	else {
		sprintf(chtext,"接收 %d 个Bucket 数据！",m_iRecBucketNum);
		AfxMessageBox(chtext);
	}
}
