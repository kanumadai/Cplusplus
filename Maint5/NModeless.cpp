// NModeless.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "NModeless.h"

#include "AutoSampleOption.h"
#include "MaintDLL.h"
#include "PetApi.h"
#include "G_Fuction.h"
#include "MaintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_32BK_RECVEIVE WM_USER+107


HANDLE hSendEvent;
HANDLE hReceiveEvent;
HANDLE hMutexOP32BK;
HANDLE hEnReceivEvent;

HANDLE h_ExitThreadEvent;

HANDLE g_hSleepEvent;

HANDLE hArray[2] = { hReceiveEvent,h_ExitThreadEvent };


UINT  Read32BK(LPVOID  lpData);
UINT  Send32BKThread(LPVOID lpData);
CWinThread *g_pRead32BK;
CWinThread *g_pSend32BK;
BOOL	g_bRunProcRead32BK;
BOOL	g_bRunProcSend32BK;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern unsigned char g_chFileType;
extern int g_DataSample_Mod_Sel;

extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;


/////////////////////////////////////////////////////////////////////////////
// CNModeless dialog


CNModeless::CNModeless(CWnd* pParent /*=NULL*/)
	: CDialog(CNModeless::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNModeless)
	m_strStartPoint = _T("");
	m_strEndPoint = _T("");
	//}}AFX_DATA_INIT
}


void CNModeless::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNModeless)
	DDX_Control(pDX, IDC_CHECK_TSERIES, m_Check_TSerise);
	DDX_Control(pDX, IDC_CHECK_TIMEMODFY, m_Check_TimeModfy);
	DDX_Control(pDX, IDC_CHECK_ENERGE, m_Check_Energe);
	DDX_Control(pDX, IDC_CHECK_CYSNUM, m_Check_CysNum);
	DDX_Control(pDX, IDC_CHECK_ANOL, m_Check_Anol);
	DDX_Control(pDX, IDC_CHECK_LOCAL, m_Check_Local);
	DDX_Text(pDX, IDC_START_POINT, m_strStartPoint);
	DDX_Text(pDX, IDC_END_POINT, m_strEndPoint);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LOAD, m_check_load);

}


BEGIN_MESSAGE_MAP(CNModeless, CDialog)
	//{{AFX_MSG_MAP(CNModeless)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RENAME, OnReName)
	ON_BN_CLICKED(IDC_DATA_PARA, OnDataPara)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAMPLE, OnCheckAutosample)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SAMPLE, OnButtonAutoSample)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_32BK_RECVEIVE, On32BKRecveive)
	ON_MESSAGE(WM_AUTOSAMPLE_RECFINISH, OnAutoSampRecFinish)		


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNModeless message handlers

CNModeless::CNModeless(CView *pView)
{
	m_pView = pView;

	m_iworkcounter = 0;
	memset(m_recbuff,0,9*1024);
	memset(m_secrecbuf,0,9*1024);
	m_precbuf = m_recbuff;
	m_8KdataNo = 0;
	strRecStatus = "";
	bCloseFile = false;

	m_DataSampleModeSel = 0;

	FileName = "Bkfile";

}

BOOL CNModeless::Create()
{
	return CDialog::Create(CNModeless::IDD);   //asdknag?????
}

void CNModeless::OnOK() 		 
{
	if ((m_bEnPara) &&(!m_bAutoSampleOperate) ) {
	//if (m_bEnPara) {
		if (WAIT_TIMEOUT == WaitForSingleObject(hMutexOP32BK,1000)) {
			AfxMessageBox("读线程 保护错误");
			return ;
		}
		if (m_check_load.GetCheck() == 1) {
			Op32BK.bReadOP = true;
			strRecStatus = "接受到Bucket:";
		}else {
			Op32BK.bReadOP = false;
			strRecStatus = "写完成Bucket:";
		}
		
		if (m_Check_Local.GetCheck() == 1) {
			Op32BK.bInputCMD[CHECKLOCAL] = true;
		}else { Op32BK.bInputCMD[CHECKLOCAL] = false; }

		if (m_Check_Anol.GetCheck() == 1) {
			Op32BK.bInputCMD[CHECKANOL] = true;
		}else { Op32BK.bInputCMD[CHECKANOL] = false; }

		if (m_Check_Energe.GetCheck() == 1) {			
			Op32BK.bInputCMD[CHECKENERGE] = true;
		}else { Op32BK.bInputCMD[CHECKENERGE] = false; }

		if (m_Check_TimeModfy.GetCheck() == 1) {
			Op32BK.bInputCMD[CHECKTIMEMODFY] = true;
		}else { Op32BK.bInputCMD[CHECKTIMEMODFY] = false; }

		if (m_Check_TSerise.GetCheck() == 1) {
			Op32BK.bInputCMD[CHECKTSERISE] = true;
		}else { Op32BK.bInputCMD[CHECKTSERISE] = false;}

		if (m_Check_CysNum.GetCheck() == 1) {
			Op32BK.bInputCMD[CHECKCYSNUM] = true;
		}else { Op32BK.bInputCMD[CHECKCYSNUM] = false;}

		ReleaseMutex(hMutexOP32BK);

	}

	unsigned char Stop[7]=	{0x70,0xb0,0x07,0xca,0x00,0x0d,0x88};
	int nWR;

	m_DataSampleModeSel = g_DataSample_Mod_Sel;

	if (m_bAutoSampleOperate) {  // 自动采样
		m_DataSampleModeSel = m_iCurSampleModel;
	}				//

	if(!m_bEnComm) {	//停止	
		if (!m_bAutoSampleOperate) {
			GetDlgItem(IDC_CHECK_AUTOSAMPLE)->EnableWindow(true);
		}

		g_bRunProcSend32BK = false;
		GetDlgItem(IDOK)->EnableWindow(false);	 
		//----------------------------------------------
		if( WAIT_TIMEOUT == WaitForSingleObject(hReceiveEvent,2000) ) { // 等待 接收超时  11-14
			//
		}	   
		//----------------------------------------------	 
		
		m_bEnComm = true;
		m_EnClearBuff = true;				

		SetEvent(hSendEvent);	
		SetEvent(hEnReceivEvent);
		SetEvent(h_ExitThreadEvent);
		SetEvent(hArray[1]); 

		Sleep(1000);

/*		if( WaitForSingleObject( g_pRead32BK->m_hThread,  5000) == WAIT_TIMEOUT ) {
			TerminateThread( g_pRead32BK->m_hThread, -1);	
			SetDlgItemText(IDC_MOD_STATUS,"接收数据内存泄漏！");
		}
		else delete g_pRead32BK;				   //akang 11-13
*/		
		SetEvent(hSendEvent);		
		SetEvent(h_ExitThreadEvent);
		SetEvent(hArray[1]); 

/*		if( WaitForSingleObject( g_pSend32BK->m_hThread,  5000) == WAIT_TIMEOUT ) {
			TerminateThread( g_pSend32BK->m_hThread, -1);
			SetDlgItemText(IDC_MOD_STATUS,"发送命令内存泄漏！");
		}
		else delete g_pSend32BK;				//akang 11-13
*/
		

		ResetEvent(hSendEvent);
		ResetEvent(hEnReceivEvent);
		ResetEvent(h_ExitThreadEvent);
		SetEvent(hArray[1]); 	 			  

		KillTimer(1);		
		#ifdef _SELECT_232_PORT
			nWR = WriteCommandMaint232(7,Stop);
		#else
			nWR = WriteCommandMaintainMod(7,Stop);
		#endif
		if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}				
 
		Sleep(300);					 

		#ifdef _SELECT_232_PORT
		CloseShmMaint232();
		#else
		CloseShmMaintain();
		#endif

		if(bCloseFile)	{//关闭文件；
			bCloseFile = false;
			frecfile.Close();
		}	
		SetDlgItemText(IDOK,"开始");
		pStatus->SetPaneText(2,"停止上传数据",1);
		m_EnClearBuff = true;
		m_iworkcounter = 0;
		memset(m_recbuff,0,9*1024);
		memset(m_secrecbuf,0,9*1024);
		m_precbuf = m_recbuff;
		m_8KdataNo = 0;
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_DATA_PARA)->EnableWindow(true);
		return;
	}
	  // 开始
		if (!m_bAutoSampleOperate) {
			GetDlgItem(IDC_CHECK_AUTOSAMPLE)->EnableWindow(false);
		}
		
		m_EnClearBuff = true;			
		KillTimer(1);
		if(!OnInitComm()) return;
		m_bEnComm = false;
		SetDlgItemText(IDOK,"停止");
		pStatus->SetPaneText(2,"开始上传...",1);		
		strRecStatus = "接收到Bucket ";
		SetDlgItemText(IDC_MOD_STATUS,"开始接收。。。");
		SetDlgItemText(IDC_BUCKETNO,"Bucket");

		GetDlgItem(IDC_DATA_PARA)->EnableWindow(false);
		m_precbuf = m_recbuff;
		m_8KdataNo = 0;
		m_iworkcounter = 0;
		m_precbuf[2] = 0x04;
		memset(m_recbuff,0,9*1024);
		m_ShowCounterTh = 500;

		Sleep(200);
		m_EnClearBuff = false;

		UpdateData(true);
		UpdateData(false);
		int temp = 0;
		if(!m_strStartPoint.IsEmpty()) {
			temp = _atoi64(m_strStartPoint);
			if((temp>=32)||(temp<0)) { 
				temp = 0;
				m_strStartPoint = "00";
				UpdateData(false);
			}	
			m_LoadBucketNo = 0x80 + temp;
		}
		else m_LoadBucketNo = 0x80;

		if(!m_strEndPoint.IsEmpty()) {
			int temp = _atoi64(m_strEndPoint);
			if((temp>=32)||(temp<0)||(m_LoadBucketNo > 0x80 + temp)) { 
				temp = 31;
				m_strEndPoint = "31";
				UpdateData(false);
			}	
			m_LoadBucketMax = 0x80 + temp;  
		}
		else m_LoadBucketMax = 0x9f;


	if (m_bEnPara) {
		if (Op32BK.bReadOP) {
			strRecStatus = "接受到Bucket:";
		}else {
			strRecStatus = "写完成Bucket:";
		}
		OnSendCmdPack();
		return;
	}
	else {
		OnOpenFile();
	}
	
	char chtemp[50];
	int ttt = m_LoadBucketNo-0x80;
	sprintf(chtemp,"%d: 00", ttt);
	SetDlgItemText(IDC_SHOW_COUNTER,chtemp);

	SetEvent(m_hSetTimeOut);
	Sleep(200);

	unsigned char senddata[10] = {0x80,0xb0,0x08,0xcb,0x01,0x00,0x00,0x88};
	senddata[0]=m_LoadBucketNo;	
	senddata[6]=senddata[0];	
	for(int j=1;j<6;j++)
		  senddata[6]^=senddata[j];
	senddata[7]=0X88;

	sprintf(chtemp,"Send=> %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x",senddata[0],senddata[1],senddata[2],senddata[3],senddata[4],senddata[5],senddata[6],senddata[7]);
	pStatus->SetPaneText(0,chtemp,1);

	#ifdef _SELECT_232_PORT
		nWR = WriteCommandMaint232(8,senddata);
	#else
		nWR = WriteCommandMaintainMod(8,senddata);
	#endif
	if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	#ifdef _SELECT_232_PORT
		m_iTimeReceivedData = 5;
	#else
		m_iTimeReceivedData = 28;
	#endif
	SetTimer(1,400,NULL);   //akang
	
}      

void CNModeless::OnCancel() 
{
	if (!m_bAutoSampleStart) {
		return;
	}

	if(!m_bEnComm) return;

	m_bEnComm = true;
	g_bRunProcRead32BK = false;
	m_EnClearBuff = true;

	Sleep(200);

	KillTimer(1);
	if(bCloseFile)	{//关闭文件；
		bCloseFile = false;
		frecfile.Close();
	}	

	CloseHandle(hSendEvent);
	CloseHandle(hReceiveEvent);
	CloseHandle(hMutexOP32BK);
	CloseHandle(m_hSetTimeOut);
	CloseHandle(h_ExitThreadEvent);
	CloseHandle(hArray[1]);
	CloseHandle(hArray[0]);
	

	if(m_pView != NULL) {
		UpdateData(true);
		m_pView->PostMessage(WM_MOD_GOODBYE, IDCANCEL);
	}
	else {
		CDialog::OnOK();
	}
}

BOOL CNModeless::OnInitComm()
{
	if((m_PortNo > 2)||(m_PortNo <= 0)) {
			AfxMessageBox("端口参数越限");
			return false;
		}
	#ifdef _SELECT_232_PORT
		if(!OpenShmMaint232(m_PortNo,g_HostBaud)) {
			MessageBox("Communication Port Open Error!");
			return false;
		}
	#else
		if(!OpenShmMaintain()) {
			AfxMessageBox("OpenShmMaint Error");
			return false;
		}
	#endif	


	pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);

	
		ResetEvent(h_ExitThreadEvent);
		ResetEvent(hArray[1]); 

		g_pRead32BK = AfxBeginThread(Read32BK,
				(LPVOID)this,
				THREAD_PRIORITY_BELOW_NORMAL,
				0,
				CREATE_SUSPENDED);				
		
		g_pRead32BK->m_bAutoDelete = false;			// 11-13 	 
		g_pRead32BK->ResumeThread();
		
		g_pSend32BK = AfxBeginThread(Send32BKThread,
				(LPVOID)this,
				THREAD_PRIORITY_BELOW_NORMAL,
				0,
				CREATE_SUSPENDED);		
		
		g_pSend32BK->m_bAutoDelete = false;	 // akang 11-13	  
		
		g_pSend32BK->ResumeThread();	
  return true;

}

void CNModeless::OnTimer(UINT nIDEvent) 
{
	unsigned char Reloaddata[7] = {0x80,0xb0,0x07,0xcd,0x00,0xfa,0x88};	//要求继续上传8K的 数据
	unsigned char stloaddata[10] = {0x80,0xb0,0X08,0Xcb,0x01,0x00,0x00,0x88 };
	unsigned char Stop[7]=	{0x70,0xb0,0x07,0xca,0x00,0x0d,0x88};

	char chtemp[100];
	int nWR,nWR2;
	int j;
	CString strtime;

	WaitForSingleObject(m_hSetTimeOut,50);
	
	CString strmodel;
	switch(m_iCurSampleModel) {
	case 0xc7: strmodel = "||====(正在采集 散点)====||";
		break;
	case 0xc5: strmodel = "||====(正在采集 能谱)====||";
		break;
	case 0xc6: strmodel = "||====(正在采集 时间)====||";
		break;
	case 0xc8: strmodel = "||====(正在采集 符合0)====||";
		break;
	case 0xc9: strmodel = "||====(正在采集 符合1)====||";
		break;
	default:   strmodel = "||----(未知方式)----||";
	}
	

	switch(nIDEvent) {
		case 0:   //自动采集定时用
			m_iCurSampleTime ++;
			strtime.Format("\n时间：%d分 %d秒   循环: %d",m_iCurSampleTime/60, m_iCurSampleTime%60, m_iCurReAutoSamplNo);
			SetDlgItemText(IDC_MOD_STATUS,m_strAutoSampleStatus + strmodel + strtime);
			
			if (m_iCurSampleTime >= m_iSampleTime*60) {
				KillTimer(0);
				OnStopAutoSample();	 
				Sleep(200);
				m_bEnPara = false; //默认上传的是数据					// akang 11-14						  				
				OnOK();  //开始收集数据				
			}				
			break;
		case 1:						
				if((--m_iTimeReceivedData) > 0) {
					return;	
				}
				m_iworkcounter = 0;
				memset(m_recbuff,0,9*1024);
				m_precbuf = m_recbuff;
				m_8KdataNo = 0;
				SetDlgItemText(IDC_MOD_STATUS,strRecStatus + "(有接收超时)");
				if((++m_LoadBucketNo) > m_LoadBucketMax) {
					KillTimer(1);
					if(bCloseFile)	{//关闭文件；
						bCloseFile = false;
						frecfile.Close();
					}						
					SetDlgItemText(IDC_MOD_STATUS, strRecStatus + " 完成(" + FileName + ")");
					m_EnClearBuff = true;	  
					GetDlgItem(IDOK)->EnableWindow(true);	 

					if (m_bAutoSampleOperate) {						
						PostMessage(WM_AUTOSAMPLE_RECFINISH,0, 0);
						Sleep(200);
					}	
					return;
				}
				if(bCloseFile)	{//关闭文件；
					bCloseFile = false;
					frecfile.Close();
				}
				OnOpenFile();//打开 开文件

				Sleep(100);
				#ifdef _SELECT_232_PORT
					nWR2 = WriteCommandMaint232(7,Stop);
				#else
					nWR2 = WriteCommandMaintainMod(7,Stop);
				#endif
				if(nWR2 == false) {
					AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
				}

				Sleep(100);
				
				if (m_bEnPara) {
					m_iTimeReceivedData = 3;
				}else {
					#ifdef _SELECT_232_PORT
						m_iTimeReceivedData = 5;
					#else
						m_iTimeReceivedData = 28;
					#endif					
				}
				
				stloaddata[0] = m_LoadBucketNo;					
				stloaddata[6]=stloaddata[0];	
				for(j=1;j<6;j++)
					  stloaddata[6]^=stloaddata[j];
				stloaddata[7]=0X88;
				sprintf(chtemp,"Send=> %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x",stloaddata[0],stloaddata[1],stloaddata[2],stloaddata[3],stloaddata[4],stloaddata[5],stloaddata[6],stloaddata[7]);
				pStatus->SetPaneText(0,chtemp,1);

				sprintf(chtemp,"%d: 00", m_LoadBucketNo-0x80);
				SetDlgItemText(IDC_SHOW_COUNTER,chtemp);


				#ifdef _SELECT_232_PORT
					nWR = WriteCommandMaint232(8,stloaddata);
				#else
					nWR = WriteCommandMaintainMod(8,stloaddata);
				#endif
				if(nWR == false) {
						AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
				}

			break;
		case 3:
			break;
		default:
			break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CNModeless::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pStatus=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	
	if(m_bEnComm) {
		SetDlgItemText(IDOK,"开始");
	} 
	else {
		SetDlgItemText(IDOK,"停止");
	}

	pStatus->SetPaneText(2,"上传32Bucket数据",1);

	SetDlgItemText(IDC_MOD_STATUS,"默认文件名: \n (当前路径)"+ FileName );

	m_strStartPoint = "00";
	m_strEndPoint = "31";
	m_LoadBucketMax = 0x9f;
	UpdateData(false);

	m_hSetTimeOut = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_check_load.SetCheck(1);
	GetDlgItem(IDC_LOAD)->ShowWindow(false);  
	GetDlgItem(IDC_DOWN)->ShowWindow(false); 
	GetDlgItem(IDC_CHECK_LOCAL)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_ANOL)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_ENERGE)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_TSERIES)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_TIMEMODFY)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_CYSNUM)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(false);

	GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->ShowWindow(false);

	SetDlgItemText(IDC_SEL_TYPE,"数据");
	m_bEnPara = false;

	hSendEvent = CreateEvent(NULL,true,FALSE,NULL);
	hReceiveEvent = CreateEvent(NULL,true,FALSE,NULL);
	hMutexOP32BK = CreateMutex(NULL,FALSE,NULL);
	hEnReceivEvent = CreateEvent(NULL,true,FALSE,NULL);
	g_hSleepEvent = CreateEvent(NULL,true,FALSE,NULL);

	hArray[0] = CreateEvent(NULL,true,FALSE,NULL);
	hArray[1] = CreateEvent(NULL,true,FALSE,NULL);
	

	h_ExitThreadEvent = CreateEvent(NULL,true,FALSE,NULL);

	OninitOp32BK();

	//自动采集数据用的
	m_bAutoSampleOperate = false;
	m_bShowAutoCheck = true;
	memset(m_arrSampleModSel,0,5*4);
	m_arrSampleModSel[0] = 0xc7;
	m_arrSampleModSel[1] = 0xc5;
	memset(m_arrAutoSampParaSel,0,10*4);
	m_arrAutoSampParaSel[2] = 1;

	m_iSampleTime = 20;
	m_iReSampleNum = 10;
	m_bAutoSampleStart = true;
	m_iAutoSampleModleNo = 0;
	m_bRecAnolPara = false;

	return TRUE;    
}

UINT  Read32BK(LPVOID  lpData)
{
	int num = 0;
	int size;
	unsigned char *data;
	data = new unsigned char[9*1024];

	CNModeless *ph=(CNModeless *)lpData;
	
	g_bRunProcRead32BK = true;
	while (1) {
		if (WAIT_OBJECT_0 == WaitForSingleObject(h_ExitThreadEvent,0)) {
			
			delete [] data;
			return 0L;
		}		 
							
		#ifdef _SELECT_232_PORT
			size = ReadResultMaint232(data);
		#else
			size = ReadResultMaintainMod(data);
		#endif	 		
			
		if(ph->m_EnClearBuff) {  //屏蔽接收用的 akang			   // 11-14
			size = 0;			
		}; 		 
		 
		if(size > 0) {	   			
			ph->SendMessage(WM_32BK_RECVEIVE, (WPARAM)data,(LPARAM)size);	 
		}
		else Sleep(5);

	}

	delete [] data;
	return 0L;
}

LRESULT CNModeless::On32BKRecveive(WPARAM wParam, LPARAM lParam)
{
	unsigned char* ptext = (unsigned char*)wParam;
	unsigned char reloaddata[10] = {0x80,0xb0,0x08,0xcd,0x00,0x00,0xfa,0x88};	//要求继续上传8K的 数据
	unsigned char stloaddata[10] = {0x80,0xb0,0X08,0Xcb,0x01,0x00,0x00,0x88 };
	unsigned char senddata[10];
	char recchar[20];

	memcpy(m_precbuf,ptext,(int)lParam);

	if (m_bEnPara) {	
		memcpy(&(m_recbuff[m_iworkcounter]),ptext,(int)lParam);  //??????
	}

	m_iworkcounter += (int)lParam;

	if (m_bEnPara) {	
		m_precbuf += (int)lParam;//workcounter;
		if (m_iworkcounter >= m_recbuff[2]) {
			m_iworkcounter = 0;
			if(!On32FileProc(m_recbuff)) {  //处理数据有错时
				SetDlgItemText(IDC_MOD_STATUS," UCRC ERROR !");	  					
				strRecStatus = strRecStatus + " x ";  //akang  11-14  
				memset(m_recbuff,0,9*1024);		//akang 11-14		8*1024
				m_precbuf[2] = 4;
				m_precbuf = m_recbuff;
				return 0L;	
			}
			memset(m_recbuff,0,9*1024);		 //akang 11-14		8*1024	  
			m_precbuf[2] = 0x04;
			m_precbuf = m_recbuff;
			SetEvent(hReceiveEvent);
			SetEvent(hArray[0]);
		}
		return 0L;	
	}
		
	int kkk = m_8KdataNo * 8199 + m_iworkcounter;
	if(kkk > m_ShowCounterTh) {
		sprintf(recchar,"%d: %d",m_LoadBucketNo-0x80,kkk);
		SetDlgItemText(IDC_SHOW_COUNTER,recchar);  //
		UpdateData(false);
		m_ShowCounterTh += 500;
	}

	ResetEvent(m_hSetTimeOut);	
 
	#ifdef _SELECT_232_PORT
		m_iTimeReceivedData = 8;   //default 5;		//akang 11-12
	#else
		m_iTimeReceivedData = 35;	//default 28;	//akang 11-12
	#endif

    SetEvent(m_hSetTimeOut);

	if(m_iworkcounter < 8*1024 + 7) {// 	
		m_precbuf += (int)lParam;//workcounter;		
		return 0L;									
	} 
	
	memcpy(m_secrecbuf,m_recbuff,m_iworkcounter);
	
	if(!UCRC(m_secrecbuf)) {
		SetDlgItemText(IDC_MOD_STATUS, "UCRC 校验错 ！");
		strRecStatus = strRecStatus + " x ";		//akang  11-14  
	}
	frecfile.Write(&m_secrecbuf[5],8*1024);		//写文件

	m_iworkcounter = 0;
	m_precbuf = m_recbuff;

	GetDlgItem(IDOK)->EnableWindow(false);					//akang 11-14
	GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->EnableWindow(false);			//akang 11-14


	if( (++m_8KdataNo) < 8) { //续传
		memcpy(senddata,reloaddata,8);	
	}
	else {	//完成一个 Bucket 64K数据
		memcpy(senddata,stloaddata,8);
		m_8KdataNo = 0;
		CString strtext;		
		strtext.Format("%d ,",m_LoadBucketNo - 0x80);
		strRecStatus += strtext;
		SetDlgItemText(IDC_MOD_STATUS, strRecStatus);

		sprintf(recchar,"%d: 00", m_LoadBucketNo-0x80);
		SetDlgItemText(IDC_SHOW_COUNTER,recchar);

		if((++m_LoadBucketNo) > m_LoadBucketMax) {
			KillTimer(1);
			strtext = "完成";
			strRecStatus += strtext;
			SetDlgItemText(IDC_MOD_STATUS, strRecStatus + "(" + FileName + ")");
			m_EnClearBuff = true;
			if(bCloseFile)	{
				bCloseFile = false;
				frecfile.Close();
			}

			if (m_bAutoSampleOperate) {						
				PostMessage(WM_AUTOSAMPLE_RECFINISH,0, 0);
				Sleep(200);
			}
															
			GetDlgItem(IDOK)->EnableWindow(true);		//akang 11-14
			GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->EnableWindow(true);			//akang 11-14	

			return 0L;				
		};
		if(bCloseFile)	{//关闭文件；
			bCloseFile = false;
			frecfile.Close();
		}
		OnOpenFile();//打开 开文件
		m_ShowCounterTh = 500;
	}

	senddata[0]=m_LoadBucketNo;
	senddata[6]=senddata[0];	
	for(int j=1;j<6;j++)
		  senddata[6]^=senddata[j];
	senddata[7]=0X88;

	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(8,senddata);
	#else
		int nWR = WriteCommandMaintainMod(8,senddata);
	#endif
	if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	return 0L;
}

void CNModeless::OnReName() 
{
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("保存 32 Bucket数据路径");//
	if(dlg.DoModal()==IDOK) { 
		FilePathName=dlg.GetPathName(); 
		FileName = dlg.GetFileName();

	}  
	else return;

	SetDlgItemText(IDC_MOD_STATUS,FilePathName + "*");

}

void CNModeless::OnOpenFile()
{
	CString fileno;
	CString strtemp;
	fileno.Format("%.2d.dat",m_LoadBucketNo-0x80);

	if (m_bAutoSampleOperate) {
		fileno.Format("_C%.2d_B%.2d.dat",m_iCurReAutoSamplNo-1,m_LoadBucketNo-0x80);
	}
	strtemp = FilePathName + fileno;

	BOOL bfileexist = true;
	if(!frecfile.Open(strtemp,CFile::modeRead)) {
		bfileexist = false;
	}
	if (bfileexist) {
		frecfile.Close();
	}	
	
	if(!bCloseFile) {
		if (bfileexist) {
			frecfile.Open( strtemp, CFile::modeWrite); //modeReadWrite
		}
		else {
			frecfile.Open( strtemp, CFile::modeCreate | CFile::modeWrite);
			frecfile.SetLength((128*4 +4)*1024);	

			//保存默认位置边界值
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

		}

		unsigned char chartemp[128];
		chartemp[0] = 0x01;
		chartemp[1] = 0x01;
		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		frecfile.Write(chartemp,2);
		
		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		switch(m_DataSampleModeSel) { //g_DataSample_Mod_Sel
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

		memset(chartemp,0,128);
		for(int kk=0;kk<128*8;kk++) {
			frecfile.Write(chartemp,128);
		}

	//	unsigned char chartemp[128];
		chartemp[0] = 0x01;
		chartemp[1] = 0x01;
		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		frecfile.Write(chartemp,2);

		frecfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //写 文件类型
		switch(m_DataSampleModeSel) { //g_DataSample_Mod_Sel
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
		bCloseFile = true;
	
	}
	else {
		AfxMessageBox("文件操作失败！，注意操作规范");
		return;
	}
}


void CNModeless::OnDataPara() 
{
	m_bEnPara = ! m_bEnPara;
	if (m_bEnPara) {
		GetDlgItem(IDC_LOAD)->ShowWindow(true);  
		GetDlgItem(IDC_DOWN)->ShowWindow(true); 
		GetDlgItem(IDC_CHECK_LOCAL)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_ANOL)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_ENERGE)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_TSERIES)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_TIMEMODFY)->ShowWindow(true);
		GetDlgItem(IDC_CHECK_CYSNUM)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(true);

		SetDlgItemText(IDC_SEL_TYPE,"参数");
		SetDlgItemText(IDC_DATA_PARA,"<<<");

	}
	else {
		GetDlgItem(IDC_LOAD)->ShowWindow(false);  
		GetDlgItem(IDC_DOWN)->ShowWindow(false); 
		GetDlgItem(IDC_CHECK_LOCAL)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_ANOL)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_ENERGE)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_TSERIES)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_TIMEMODFY)->ShowWindow(false);
		GetDlgItem(IDC_CHECK_CYSNUM)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(false);

		SetDlgItemText(IDC_SEL_TYPE,"数据");
		SetDlgItemText(IDC_DATA_PARA,">>>");
	}	
}

BOOL CNModeless::OnSendCmdPack()
{
	int temp;
	UpdateData(true);
	if(!m_strStartPoint.IsEmpty()) {
		temp = _atoi64(m_strStartPoint);
		if((temp>=32)||(temp<0)) { 
			temp = 0;
			m_strStartPoint = "00";
			UpdateData(false);
		}	
		m_LoadBucketNo = 0x80 + temp;
	}
	else m_LoadBucketNo = 0x80;
	
	if(!m_strEndPoint.IsEmpty()) {
		int temp = _atoi64(m_strEndPoint);
		if((temp>=32)||(temp<0)||(m_LoadBucketNo > 0x80 + temp)) { 
			temp = 31;
			m_strEndPoint = "31";
			UpdateData(false);
		}	
		m_LoadBucketMax = 0x80 + temp;  
	}
	else m_LoadBucketMax = 0x9f;

	
/*	int size = 1;  
	while (size>0) {
	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(tempdata);
	#else
		size = ReadResultMaintainMod(tempdata);
	#endif	  
	} 
*///	 akang 11-11

	m_precbuf[2] = 4;
	m_precbuf = &(m_recbuff[0]);
	m_iworkcounter = 0;	
	
	Sleep(100);
	SetEvent(hSendEvent);	
	SetEvent(hEnReceivEvent);
	m_EnClearBuff = false;

	return true;
}

void CNModeless::OninitOp32BK()
{

	Op32BK.bInputCMD[CHECKLOCAL] = false;
	Op32BK.funcode[CHECKLOCAL] = 0xc3;
	Op32BK.funWTEEprom[CHECKLOCAL] = 0x03;
	Op32BK.funReadEEprom[CHECKLOCAL] = 0x02;	// 0x04: 读EEPROM		0x02:读FPGA		//akang 11-12
	Op32BK.funObject[CHECKLOCAL] = 0x00;
	Op32BK.packlenth[CHECKLOCAL] = 73;
	Op32BK.fileaddr[CHECKLOCAL] = 128;
	Op32BK.ackpackNo[CHECKLOCAL] = 4;

	Op32BK.bInputCMD[CHECKANOL] = false;
	Op32BK.funcode[CHECKANOL] = 0xc2;
	Op32BK.funWTEEprom[CHECKANOL] = 0x02;
	Op32BK.funReadEEprom[CHECKANOL] = 0x03;
	Op32BK.funObject[CHECKANOL] = 0x00;
	Op32BK.packlenth[CHECKANOL] = 16;
	Op32BK.fileaddr[CHECKANOL] = 3200;
	Op32BK.ackpackNo[CHECKANOL] = 1;

	Op32BK.bInputCMD[CHECKENERGE] = false;
	Op32BK.funcode[CHECKENERGE] = 0xc3;
	Op32BK.funWTEEprom[CHECKENERGE] = 0x03;
	Op32BK.funReadEEprom[CHECKENERGE] = 0x02;	// 0x04: 读EEPROM		0x02:读FPGA		//akang 11-12 
	Op32BK.funObject[CHECKENERGE] = 0x04;
	Op32BK.packlenth[CHECKENERGE] = 73;
	Op32BK.fileaddr[CHECKENERGE] = 2176;
	Op32BK.ackpackNo[CHECKENERGE] = 2;

	Op32BK.bInputCMD[CHECKTIMEMODFY] = false;
	Op32BK.funcode[CHECKTIMEMODFY] = 0xc3;
	Op32BK.funWTEEprom[CHECKTIMEMODFY] = 0x03;
	Op32BK.funReadEEprom[CHECKTIMEMODFY] = 0x02;	// 0x04: 读EEPROM		0x02:读FPGA		//akang 11-12 
	Op32BK.funObject[CHECKTIMEMODFY] = 0x06;
	Op32BK.packlenth[CHECKTIMEMODFY] = 73;
	Op32BK.fileaddr[CHECKTIMEMODFY] = 3300;
	Op32BK.ackpackNo[CHECKTIMEMODFY] = 1;


	Op32BK.bInputCMD[CHECKTSERISE] = false;
	Op32BK.funcode[CHECKTSERISE] = 0xc3;
	Op32BK.funWTEEprom[CHECKTSERISE] = 0x03;
	Op32BK.funReadEEprom[CHECKTSERISE] = 0x02;	// 0x04: 读EEPROM		0x02:读FPGA		//akang 11-12 
	Op32BK.funObject[CHECKTSERISE] = 0x07;
	Op32BK.packlenth[CHECKTSERISE] = 73;
	Op32BK.fileaddr[CHECKTSERISE] = 3812;
	Op32BK.ackpackNo[CHECKTSERISE] = 1;

	Op32BK.bInputCMD[CHECKCYSNUM] = false;
	Op32BK.funcode[CHECKCYSNUM] = 0xc3;
	Op32BK.funWTEEprom[CHECKCYSNUM] = 0x03;
	Op32BK.funReadEEprom[CHECKCYSNUM] = 0x02;	// 0x04: 读EEPROM		0x02:读FPGA		//akang 11-12 
	Op32BK.funObject[CHECKCYSNUM] = 0x08;
	Op32BK.packlenth[CHECKCYSNUM] = 73;
	Op32BK.fileaddr[CHECKCYSNUM] = 3876;
	Op32BK.ackpackNo[CHECKCYSNUM] = 1;


}

UINT  Send32BKThread(LPVOID  lpData) 
{
	CNModeless *ph=(CNModeless *)lpData;

  	int packlenth;				
	unsigned char *senddata;
	unsigned char *pdata;

	pdata = new unsigned char [5000];
	senddata = new unsigned char [1000]; 
	CString strkk;
	BOOL flag;
	BOOL b1blockfinish = false;
	int block;
	int ackno=0;

	g_bRunProcSend32BK = true;	
	
	while (g_bRunProcSend32BK) {
			WaitForSingleObject(hSendEvent,INFINITE);
			if(WAIT_OBJECT_0 == WaitForSingleObject(h_ExitThreadEvent,0) ) {					
				delete [] senddata;	
				delete [] pdata;
				senddata = NULL;
				pdata = NULL;
				return 0L;
			}
			for(int buckno= ph->m_LoadBucketNo;((buckno<= ph->m_LoadBucketMax)&&(g_bRunProcSend32BK)); buckno ++) {
				block = 0;
				b1blockfinish = false;
				flag = true;
				for(int cmdnum=2;((cmdnum<CMDNUM)&&(flag)&&(g_bRunProcSend32BK));cmdnum++) {			
					if (ph->Op32BK.bInputCMD[cmdnum]) {						
						block = 0;
						if(! ph->ON32FileOpen(buckno,ph->Op32BK.bReadOP,cmdnum,pdata) ) {
							flag = false;	
						}
						for(block=0;((block<8)&&(flag));block++) {							
							for( ackno=0;((ackno< ph->Op32BK.ackpackNo[cmdnum])&&(flag)&&(g_bRunProcSend32BK));ackno++) {						
								
								if(WAIT_OBJECT_0 == WaitForSingleObject(h_ExitThreadEvent,0) ||(g_bRunProcSend32BK == false)) {    									 
									delete [] senddata;	
									delete [] pdata;	
									return 0L;
								}
								senddata[0] = (unsigned char)buckno;
								senddata[1] = 0xB0; //
								senddata[2] = (unsigned char)ph->Op32BK.packlenth[cmdnum];
								senddata[3] = ph->Op32BK.funcode[cmdnum];							
								senddata[5] = block;
								senddata[6] = ph->Op32BK.funObject[cmdnum] + ackno;
								if (ph->Op32BK.bReadOP) {  //读取
									packlenth = 9;
									senddata[2] = packlenth;
									senddata[4] = ph->Op32BK.funReadEEprom[cmdnum];								
								}
								else { //写入
									senddata[4] = ph->Op32BK.funWTEEprom[cmdnum];
									if (cmdnum == CHECKANOL) {
										memcpy(&senddata[6],(pdata + (ph->Op32BK.packlenth[cmdnum]- 8)*(block * ph->Op32BK.ackpackNo[cmdnum] + ackno)),(ph->Op32BK.packlenth[cmdnum]- 8));
									}
									else {								
										memcpy(&senddata[7],(pdata + (ph->Op32BK.packlenth[cmdnum]- 9)*(block * ph->Op32BK.ackpackNo[cmdnum] + ackno)),(ph->Op32BK.packlenth[cmdnum]- 9));
									}	
									packlenth = ph->Op32BK.packlenth[cmdnum];
								}
								
								senddata[packlenth-2] = 0x00;
								for(int i=0;i<packlenth-2;i++) { senddata[packlenth-2] ^= senddata[i];	}
								senddata[packlenth-1] = 0x88;							
								
								strkk.Format("%d",buckno-0x80);
								ph->SetDlgItemText(IDC_SHOW_COUNTER,strkk);

								ResetEvent(hReceiveEvent);
								ResetEvent(hArray[0]);
								Sleep(80);				//akang 11-14   Sleep(80)  要是串口通信, 不正常,在此添加沿时！！！;	

								ph->m_iworkcounter = 0; 
								ph->Op32BK.opratingAckNo = ackno;
								ph->Op32BK.opratingCMDNo = cmdnum;
								ph->Op32BK.opratingBucket = buckno;
								ph->Op32BK.opratingBlock = block;

								ph->m_precbuf[2] = 4;
								ph->m_precbuf = &(ph->m_recbuff[0]);
								ph->m_iworkcounter = 0;
								
								#ifdef _SELECT_232_PORT
								WriteCommandMaint232(packlenth,senddata);
								#else
								WriteCommandMaintainMod(packlenth,senddata);
								#endif									
								if((CHECKTSERISE == cmdnum)||(CHECKCYSNUM == cmdnum)) {
									flag = false; 
									b1blockfinish = true; //此时用来控制 发送一次
								}

								if(WAIT_TIMEOUT == WaitForMultipleObjects(2,hArray,false,1000)) {	  // 1000
									if(WAIT_OBJECT_0 == WaitForSingleObject(h_ExitThreadEvent,0) || (g_bRunProcSend32BK == false) ) {  											 
										delete [] senddata;	
										delete [] pdata;	
										return 0L;
									}	

									flag = false;  ////
									if ((cmdnum == CHECKTSERISE)||(cmdnum == CHECKCYSNUM)) {
										b1blockfinish = false;	
									}															
									if ((ph->Op32BK.bReadOP)&&(ph->bCloseFile)) {
										ph->bCloseFile = false;
										ph->frecfile.Close();
									}		
									
									ph->strRecStatus = ph->strRecStatus + " x ";  //akang  11-12
									ph->SetDlgItemText(IDC_MOD_STATUS,ph->strRecStatus + "(超时)");
								}								
								Sleep(60);  // 模拟参数用的， 有用		 //akang 11-14	 Sleep(60);	 要是串口通信, 不正常,在此添加沿时！！！;
							}
						}
						if ((ph->Op32BK.bReadOP)&&(ph->bCloseFile)) {
							ph->bCloseFile = false;
							ph->frecfile.Close();
						}					
						flag = true;							
					}
				}
				if ((block >= 8)||(b1blockfinish)) {  //b1blockfinish 只发送一次  
					strkk.Format(" %d,",buckno-0x80);
					ph->strRecStatus += strkk;
					ph->SetDlgItemText(IDC_MOD_STATUS,ph->strRecStatus);
				}
				
			}
			
			ph->strRecStatus += " (完成)";
			ph->SetDlgItemText(IDC_MOD_STATUS,ph->strRecStatus);

			if (ph->m_bAutoSampleOperate) {
				ph->PostMessage(WM_AUTOSAMPLE_RECFINISH,0, 0);
			}	
		
			ResetEvent(hSendEvent);
			if(WAIT_OBJECT_0 == WaitForSingleObject(h_ExitThreadEvent,0) ) {
				SetEvent(h_ExitThreadEvent);
				SetEvent(hArray[1]);	  
					 
				delete [] senddata;	
				delete [] pdata;	
				return 0L;
			}	
			
		}	  
		
		delete [] senddata;	
		delete [] pdata;	
		return 0L;	 
}

BOOL CNModeless::ON32FileOpen(int bucketNo,BOOL breadfile,int cmdnum,unsigned char *data)
{
	CString fileno;
	CString strtemp;
	fileno.Format("%.2d.dat",bucketNo-0x80); //akang 8.8 好

	if (m_bAutoSampleOperate) {
		fileno.Format("_C%.2d_B%.2d.dat",m_iCurReAutoSamplNo-1,bucketNo-0x80);
	}

	strtemp = FilePathName + fileno;	

	unsigned char ucharray[64][4];

	if(bCloseFile) {
		AfxMessageBox("文件操作失败！，注意操作规范");
		return false;
	}

	BOOL bfileexist = true;
	if(!frecfile.Open(strtemp,CFile::modeRead)) {
		bfileexist = false;
		if (!breadfile) {
			return false;	
		}		
	};
	if (bfileexist) {
		frecfile.Close();
	}	
	if (bfileexist) {
		frecfile.Open( strtemp, CFile::modeReadWrite); //modeReadWrite
	}
	else {
		frecfile.Open( strtemp, CFile::modeCreate | CFile::modeReadWrite);
		frecfile.SetLength((128*4 +4)*1024);			
	}
	
	int i,j,bb;
	unsigned char *chartemp = NULL;
	chartemp = new unsigned char [5000];
	ASSERT(chartemp);
//	unsigned char chartemp[500];   //akang 11-12

	switch(cmdnum) { 
	case CHECKLOCAL: 
		frecfile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin);
		if (breadfile) {		
			//memset(chartemp,0,BLOCK_PLIMIT_DATA_SIZE);			//akang 11-12
			//frecfile.Write(chartemp,BLOCK_PLIMIT_DATA_SIZE);		//akang 11-12
			frecfile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_PLIMIT_DATA_SIZE*8);
			if (0 != (bucketNo%2)) {
				memcpy(chartemp,data,BLOCK_PLIMIT_DATA_SIZE*8);
				for(i=0;i<8;i++) {
					memcpy(data + BLOCK_PLIMIT_DATA_SIZE *i,chartemp + BLOCK_PLIMIT_DATA_SIZE *(7-i),BLOCK_PLIMIT_DATA_SIZE);
				}
			}
			for(bb=0;bb<8;bb++) {
				for(i=0;i<BLOCK_PLIMIT_DATA_SIZE/4;i++) {
					for(j=0;j<4;j++) {						
						ucharray[i][j] = data[i*4 + j + bb*BLOCK_PLIMIT_DATA_SIZE];						
					}
				}
				for(i=0;i<BLOCK_PLIMIT_DATA_SIZE/4;i++) {
					data[i + bb*BLOCK_PLIMIT_DATA_SIZE] = ucharray[i][0];
					data[i+BLOCK_PLIMIT_DATA_SIZE/4 + bb*BLOCK_PLIMIT_DATA_SIZE] = ucharray[i][2];
					data[i+BLOCK_PLIMIT_DATA_SIZE/2 + bb*BLOCK_PLIMIT_DATA_SIZE] = ucharray[i][1];
					data[i+BLOCK_PLIMIT_DATA_SIZE*3/4 + bb*BLOCK_PLIMIT_DATA_SIZE] = ucharray[i][3];
				}				
			}	
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	case CHECKANOL:	
		frecfile.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
		if (breadfile) {
		//	memset(chartemp,0,BLOCK_ANOLPARA_DATA_SIZE*8);					//akang 11-12
		//	frecfile.Write(chartemp,BLOCK_ANOLPARA_DATA_SIZE*8);
			frecfile.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_ANOLPARA_DATA_SIZE*8);			
			if (0 != (bucketNo%2)) {
				memcpy(chartemp,data,BLOCK_ANOLPARA_DATA_SIZE*8);
				for(i=0;i<8;i++) {
					memcpy(data + BLOCK_ANOLPARA_DATA_SIZE *i,chartemp + BLOCK_ANOLPARA_DATA_SIZE *(7-i),BLOCK_ANOLPARA_DATA_SIZE);
				}
			}		
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	case CHECKENERGE:  
		frecfile.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
		if (breadfile) {
			//memset(chartemp,0,BLOCK_ETHLIMIT_DATA_SIZE);
			//frecfile.Write(chartemp,BLOCK_ETHLIMIT_DATA_SIZE);		//akang 11-12
			frecfile.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_ETHLIMIT_DATA_SIZE*8);
			if (0 != (bucketNo%2)) {
				memcpy(chartemp,data,BLOCK_ETHLIMIT_DATA_SIZE*8);
				for(i=0;i<8;i++) {
					memcpy(data + BLOCK_ETHLIMIT_DATA_SIZE *i,chartemp + BLOCK_ETHLIMIT_DATA_SIZE *(7-i),BLOCK_ETHLIMIT_DATA_SIZE);
				}
			}
			for(bb=0;bb<8;bb++) {				
				for(i=0;i<BLOCK_ETHLIMIT_DATA_SIZE/2;i++) {
					for(j=0;j<2;j++) {						
						ucharray[i][j] = data[i*2 + j + bb*BLOCK_ETHLIMIT_DATA_SIZE];						
					}
				}
				for(i=0;i<BLOCK_ETHLIMIT_DATA_SIZE/2;i++) {
					data[i + bb*BLOCK_ETHLIMIT_DATA_SIZE] = ucharray[i][0];
					data[i+BLOCK_ETHLIMIT_DATA_SIZE/2 + bb*BLOCK_ETHLIMIT_DATA_SIZE] = ucharray[i][1];
				}
			}			
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	case CHECKTIMEMODFY:	
		frecfile.Seek(PARA_TIMEMODFY_DATA_ADDR,CFile::begin);
		if (breadfile) {
			//memset(chartemp,0,BLOCK_TIMEMODFY_DATA_SIZE);
			//frecfile.Write(chartemp,BLOCK_TIMEMODFY_DATA_SIZE);		//akang 11-12
			frecfile.Seek(PARA_TIMEMODFY_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_TIMEMODFY_DATA_SIZE*8);
			if (0 != (bucketNo%2)) {
				memcpy(chartemp,data,BLOCK_TIMEMODFY_DATA_SIZE*8);
				for(i=0;i<8;i++) {
					memcpy(data + BLOCK_TIMEMODFY_DATA_SIZE *i,chartemp + BLOCK_TIMEMODFY_DATA_SIZE *(7-i),BLOCK_TIMEMODFY_DATA_SIZE);
				}
			}
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	case CHECKTSERISE:
		frecfile.Seek(PARA_TSERIES_DATA_ADDR,CFile::begin);
		if (breadfile) {
			//memset(chartemp,0,BLOCK_TSERIES_DATA_SIZE);
			//frecfile.Write(chartemp,BLOCK_TSERIES_DATA_SIZE);			//akang 11-12
			frecfile.Seek(PARA_TSERIES_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_TSERIES_DATA_SIZE);
			memcpy(ucharray,data,BLOCK_TSERIES_DATA_SIZE);
			for(i=1;i<8;i++) {
				memcpy((data+(BLOCK_TSERIES_DATA_SIZE * i)),ucharray,BLOCK_TSERIES_DATA_SIZE);
			}			
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	case CHECKCYSNUM:
		frecfile.Seek(PARA_CYSNUM_DATA_ADDR,CFile::begin);
		if (breadfile) {
			//memset(chartemp,0,BLOCK_CYSNUM_DATA_SIZE);
			//frecfile.Write(chartemp,BLOCK_CYSNUM_DATA_SIZE);			//akang 11-12
			frecfile.Seek(PARA_CYSNUM_DATA_ADDR,CFile::begin);
			bCloseFile = true;
		}
		else {
			frecfile.Read(data,BLOCK_CYSNUM_DATA_SIZE);
			memcpy(ucharray,data,BLOCK_CYSNUM_DATA_SIZE);
			for(i=1;i<8;i++) {
				memcpy((data+(BLOCK_TSERIES_DATA_SIZE * i)),ucharray,BLOCK_CYSNUM_DATA_SIZE);
			}
			bCloseFile = false;
			frecfile.Close();
		}
		break;
	default:
		frecfile.Close();
		delete [] chartemp;			     // akang 11-12
		return false; 		
	}

	delete [] chartemp;					   //akang 11-12	 
	chartemp = NULL;

	return true;
}

BOOL CNModeless::On32FileProc(unsigned char *data)
{
	if(!UCRC(data)) {		
//		AfxMessageBox("UCRC ERROR");			 //  akang 11-12
		return false;
	}

	if (Op32BK.bReadOP) { //为读操作， 则写数据到文件
		if (data[4] == 0x56) { //功能码
			if (Op32BK.opratingCMDNo == CHECKENERGE) {
				memcpy(&m_dataEnerge[Op32BK.opratingAckNo][0],&(data[5]),data[2]-7);
				if (Op32BK.opratingAckNo == 1) {
					if (0 == (Op32BK.opratingBucket % 2)) {
						frecfile.Seek(PARA_ETHLIMIT_DATA_ADDR + BLOCK_ETHLIMIT_DATA_SIZE *Op32BK.opratingBlock,CFile::begin);
					}
					else { 						
						frecfile.Seek(PARA_ETHLIMIT_DATA_ADDR + BLOCK_ETHLIMIT_DATA_SIZE *(7-Op32BK.opratingBlock),CFile::begin);
					}
					for(int i=0;i<64;i++) {
						frecfile.Write(&(m_dataEnerge[0][i]),1);
						frecfile.Write(&(m_dataEnerge[1][i]),1);
					}
				}
			}
			else if (Op32BK.opratingCMDNo == CHECKLOCAL) {
				memcpy(&m_dataLocal[Op32BK.opratingAckNo][0],&(data[5]),data[2]-7);
				if (Op32BK.opratingAckNo == 3) {
					if (0 == (Op32BK.opratingBucket % 2)) {
						frecfile.Seek(PARA_PLIMIT_DATA_ADDR + BLOCK_PLIMIT_DATA_SIZE *Op32BK.opratingBlock,CFile::begin);
					}
					else { 						
						frecfile.Seek(PARA_PLIMIT_DATA_ADDR + BLOCK_PLIMIT_DATA_SIZE *(7-Op32BK.opratingBlock),CFile::begin);
					}
					for(int i=0;i<64;i++) {
						frecfile.Write(&(m_dataLocal[0][i]),1);
						frecfile.Write(&(m_dataLocal[2][i]),1);
						frecfile.Write(&(m_dataLocal[1][i]),1);
						frecfile.Write(&(m_dataLocal[3][i]),1);
					}
				}
			}
			else if (Op32BK.opratingCMDNo == CHECKANOL) {
				if (0 == (Op32BK.opratingBucket % 2)) {
					frecfile.Seek(PARA_ANOLPARA_DATA_ADDR + BLOCK_ANOLPARA_DATA_SIZE*Op32BK.opratingBlock,CFile::begin);
				}
				else { 						
					frecfile.Seek(PARA_ANOLPARA_DATA_ADDR + BLOCK_ANOLPARA_DATA_SIZE *(7-Op32BK.opratingBlock),CFile::begin);
				}
				frecfile.Write(&(data[5]),BLOCK_ANOLPARA_DATA_SIZE);//模拟数据
				Sleep(50);
			};
			if (Op32BK.opratingCMDNo == CHECKTIMEMODFY) {
				if (0 == (Op32BK.opratingBucket % 2)) {
					frecfile.Seek(PARA_TIMEMODFY_DATA_ADDR + BLOCK_TIMEMODFY_DATA_SIZE*Op32BK.opratingBlock,CFile::begin);
				}
				else { 						
					frecfile.Seek(PARA_TIMEMODFY_DATA_ADDR + BLOCK_TIMEMODFY_DATA_SIZE *(7-Op32BK.opratingBlock),CFile::begin);
				}
				frecfile.Write(&(data[5]),data[2]-7);
			}
			
			if	(Op32BK.opratingCMDNo == CHECKTSERISE) {				
				frecfile.Seek(PARA_TSERIES_DATA_ADDR,CFile::begin);
				frecfile.Write(&(data[5]),data[2]-7);
				Sleep(50);
			};
			if (Op32BK.opratingCMDNo == CHECKCYSNUM) {
				frecfile.Seek(PARA_CYSNUM_DATA_ADDR,CFile::begin);
				frecfile.Write(&(data[5]),data[2]-7);
				Sleep(50);
			};			
			SetEvent(hReceiveEvent);
			SetEvent(hArray[0]);

		}
		else return false;		
	}
	else {
		if (data[5] == 0x55) { //应答成功
			SetEvent(hReceiveEvent);
			SetEvent(hArray[0]);
		}else return false;
	}
	
//	ReleaseMutex(hMutexOP32BK);
	return true;
}


void CNModeless::OnCheckAutosample() 
{ 
	m_bAutoSampleOperate = (!m_bAutoSampleOperate);

	if (m_bAutoSampleOperate) {
		GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->ShowWindow(true);
		GetDlgItem(IDOK)->ShowWindow(false);
		GetDlgItem(IDC_DATA_PARA)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->ShowWindow(false);
		GetDlgItem(IDOK)->ShowWindow(true);
		GetDlgItem(IDC_DATA_PARA)->EnableWindow(true);
	}
	GetDlgItem(IDC_LOAD)->ShowWindow(false);  
	GetDlgItem(IDC_DOWN)->ShowWindow(false); 
	GetDlgItem(IDC_CHECK_LOCAL)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_ANOL)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_ENERGE)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_TSERIES)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_TIMEMODFY)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_CYSNUM)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(false);
	
	SetDlgItemText(IDC_SEL_TYPE,"数据");
	SetDlgItemText(IDC_DATA_PARA,">>>");
	m_bEnPara = false;
}

void CNModeless::OnButtonAutoSample() 
{
	if (!m_bAutoSampleOperate) {	
		return ;
	}
	if (m_bAutoSampleStart) {  // 开始		
		CAutoSampleOption autoSampl;		
		memcpy(autoSampl.m_arrSampleModSel,m_arrSampleModSel,5*4);
		memcpy(autoSampl.m_arrAutoSampParaSel,m_arrAutoSampParaSel,10*4);
		autoSampl.m_iSampleTime = m_iSampleTime;
		autoSampl.m_iReSampleNum = m_iReSampleNum;
		if( IDOK != autoSampl.DoModal() ) {  return; }		
		
		SetDlgItemText(IDC_BUTTON_AUTO_SAMPLE,"停止S");			
		memcpy(m_arrSampleModSel,autoSampl.m_arrSampleModSel,5*4);
		memcpy(m_arrAutoSampParaSel,autoSampl.m_arrAutoSampParaSel,10*4);
		m_iSampleTime = autoSampl.m_iSampleTime;
		m_iReSampleNum = autoSampl.m_iReSampleNum;
		m_strAutoSampleStatus.Format("(* 自动采集数据  请等待....... *)\n时间: %d 分钟   需循环: %d\n",m_iSampleTime,m_iReSampleNum);
		
		m_iCurSampleTime = 0;
		m_iCurReAutoSamplNo = 1;
		m_iAutoSampleModleNo = 0;
		  
		OnStartAutoSample();
		GetDlgItem(IDC_CHECK_AUTOSAMPLE)->EnableWindow(false);
		
	}
	else {   // 停止
		GetDlgItem(IDC_CHECK_AUTOSAMPLE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_AUTO_SAMPLE)->EnableWindow(true);			//akang 11-14

		KillTimer(0);
		m_iCurSampleTime = 0;
		m_iCurReAutoSamplNo = 1;


 		OnStopAutoSample();	   // akangakang 11-11		

		if (!m_bEnComm) { //停止接受线程
			m_bAutoSampleOperate = false;
			OnOK();  
			WaitForSingleObject(g_hSleepEvent,1000); //延时1000ms
			m_bAutoSampleOperate = true;			
		}	
		
		//OnStopAutoSample();	   // akangakang 11-11

	
		SetDlgItemText(IDC_BUTTON_AUTO_SAMPLE,"开始S");			
	}

	m_bAutoSampleStart = !m_bAutoSampleStart;

}

void CNModeless::OnStartAutoSample()
{
	unsigned char startsample[12] = {0x70,0xb0,0x0b,0x00,0x02,0x00,0x00,0x00,0x00,0x55,0x88 };

	int modeldata;
	while (m_iAutoSampleModleNo < 5){ 
		if(m_arrSampleModSel[m_iAutoSampleModleNo] > 0) {
			modeldata = m_arrSampleModSel[m_iAutoSampleModleNo];
			m_iCurSampleModel = m_arrSampleModSel[m_iAutoSampleModleNo];			
			break;
		}
		else m_iAutoSampleModleNo ++;
	}
	m_bRecAnolPara = false;	
	m_iAutoSampleModleNo ++;
	if (m_iAutoSampleModleNo > 5) {
		//set flag  //准备接受模拟参数
		m_bRecAnolPara = true;
		m_iAutoSampleModleNo = 0;
		return;
	}

	
	if((m_PortNo > 2)||(m_PortNo <= 0)) {
		AfxMessageBox("端口参数越限");
		return ;
	}
	#ifdef _SELECT_232_PORT
	if(!OpenShmMaint232(m_PortNo,g_HostBaud)) {
		MessageBox("Communication Port Open Error!");
		return ;
	}
	#else
	if(!OpenShmMaintain()) {
		AfxMessageBox("OpenShmMaint Error");
		return ;
	}
	#endif
				

	SetTimer(0,1000,NULL);
	m_iCurSampleTime = 0;
	
	startsample[0]= 0x70;//nBucketindex	
	startsample[1]=0xb0;      
	startsample[2]=0x0b;
	startsample[3]=(unsigned char)modeldata;  //m_iModel_Sel;
	startsample[4]=0x02; //m_iModel_Sel;		  
	startsample[5]=(unsigned char)(m_iSampleTime%0x100); 
	startsample[6]=(unsigned char)(m_iSampleTime/0x100);
	startsample[7]=(unsigned char)(m_iSampleTime/0x10000);
	startsample[8]=(unsigned char)(m_iSampleTime/0x1000000);
	startsample[9]=startsample[0];	
	for(int j=1;j<9;j++)	startsample[9]^=startsample[j];
	startsample[10]=0x88;
								
	int nWR;
	#ifdef _SELECT_232_PORT
	nWR = WriteCommandMaint232(11,startsample);
	#else
	nWR = WriteCommandMaintainMod(11,startsample);
	#endif
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}				
	Sleep(200);			
				
	#ifdef _SELECT_232_PORT
	CloseShmMaint232();
	#else
	CloseShmMaintain();
	#endif

}

void CNModeless::OnStopAutoSample()
{
	unsigned char stopsample[12] = {0x70,0xb0,0x07,0xca,0x00,0x0d,0x88};

	KillTimer(0);

	if((m_PortNo > 2)||(m_PortNo <= 0)) {
		AfxMessageBox("端口参数越限");
		return ;
	}
	#ifdef _SELECT_232_PORT
	if(!OpenShmMaint232(m_PortNo,g_HostBaud)) {	
		// 强迫 接收线程 停止
		m_bEnComm = false;	
		return ;
	}
	#else
	if(!OpenShmMaintain()) {  
		// 强迫 接收线程 停止
		m_bEnComm = false;	
		return ;
	}
	#endif
	
	int nWR;
	#ifdef _SELECT_232_PORT
	nWR = WriteCommandMaint232(7,stopsample);
	#else
	nWR = WriteCommandMaintainMod(7,stopsample);
	#endif
	if(nWR == false) {
		// 强迫 接收线程 停止
		m_bEnComm = false;	
		return ;    
	}				
	Sleep(200);			
	
	#ifdef _SELECT_232_PORT
		CloseShmMaint232();
	#else
		CloseShmMaintain();
	#endif		
}

LRESULT CNModeless::OnAutoSampRecFinish(WPARAM wParam,LPARAM lParam)
{
	OnOK();

	if (m_bRecAnolPara) {  //收集完模拟参数
		m_bRecAnolPara = false;
		m_iCurReAutoSamplNo ++;
		if (m_iCurReAutoSamplNo > m_iReSampleNum) { //完成循环 
			m_strAutoSampleStatus.Format("完成自动采集操作 !  \n\n单次模式采集: %d分钟  循环: %d次\n",m_iSampleTime,m_iReSampleNum);	 
			SetDlgItemText(IDC_MOD_STATUS,m_strAutoSampleStatus);
			return 0L;
		}
	} 
    
	OnStartAutoSample();

	if (m_bRecAnolPara) { //接受模拟参数
		KillTimer(0);

		if (m_arrAutoSampParaSel[0] == 1) {
			Op32BK.bInputCMD[CHECKLOCAL] = true;
		}
		else Op32BK.bInputCMD[CHECKLOCAL] = false;

		if (m_arrAutoSampParaSel[1] == 1) {
			Op32BK.bInputCMD[CHECKENERGE] = true;
		}
		else Op32BK.bInputCMD[CHECKENERGE] = false;

		if (m_arrAutoSampParaSel[2] == 1) {
			Op32BK.bInputCMD[CHECKANOL] = true;
		}
		else Op32BK.bInputCMD[CHECKANOL] = false;

		if (m_arrAutoSampParaSel[3] == 1) {
			Op32BK.bInputCMD[CHECKTIMEMODFY] = true;
		}
		else Op32BK.bInputCMD[CHECKTIMEMODFY] = false;

		if (m_arrAutoSampParaSel[4] == 1) {
			Op32BK.bInputCMD[CHECKCYSNUM] = true;
		}
		else Op32BK.bInputCMD[CHECKCYSNUM] = false;

		if (m_arrAutoSampParaSel[5] == 1) {
			Op32BK.bInputCMD[CHECKTSERISE] = true;
		}
		else Op32BK.bInputCMD[CHECKTSERISE] = false; 	
		
		m_bEnPara = true;
		Op32BK.bReadOP = true;			 
		OnOK();	
	}
	
	return 0L;
}
