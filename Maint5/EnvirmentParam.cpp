// EnvirmentParam.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "EnvirmentParam.h"
#include "G_Fuction.h"

#include "MaintDLL.h"
#include "PetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
#ifndef _DLLIMPORT_DECLARE
#define _DLLIMPORT_DECLARE
_declspec(dllimport) int WINAPI OpenShmMaint232(int portNo);
_declspec(dllimport) int WINAPI WriteCommandMaint232(int size,BYTE *command);
_declspec(dllimport) int WINAPI ReadResultMaint232(BYTE *command);
_declspec(dllimport) int WINAPI CloseShmMaint232(void);
#endif
*/

UINT  ReadProcEnvirmParam(LPVOID  lpData);
CWinThread *g_pThread_EnPara;

//extern bool UCRC(unsigned char*buff);
extern int g_Bucketindex;
extern int g_Blockindex;

extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

bool g_bRunThreadEnvirmParam;

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentParam dialog


CEnvirmentParam::CEnvirmentParam(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvirmentParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvirmentParam)
	iENPara_ShowLineNum = 0;
	m_strVali_Static = _T("");
	m_strThem_Static = _T("");
	m_strNvali_Static = _T("");
	m_strENPara_Show = _T("");
	//}}AFX_DATA_INIT

	memset(m_iArrayTemp,0,32*4);
	m_iSumTemp = 0;
	m_iRecTempNo = 0;

	m_iTempHighEth = 25;
	m_iTempLowEth = 20;
	m_fSumVoltage = 0;
	memset(m_fArrayVoltage,0,32*4);
	m_fSumNegVoltage = 0;
	memset(m_fArrayNegVoltage,0,32*4);

	for(int i=0;i<590;i++) m_fdrawtempdata[i] = 22.5;
	memset(recData,0,100);
}


void CEnvirmentParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvirmentParam)
	DDX_Control(pDX, IDC_ENPARA_SHOW, m_EditENPara_Show);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);

	DDX_Text(pDX, IDC_THEM_STATIC, m_strThem_Static);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvirmentParam, CDialog)
	//{{AFX_MSG_MAP(CEnvirmentParam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTT_START_QUERY, OnButtStartQuery)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_CBN_SELCHANGE(IDC_BUCKET_SEL, OnSelchangeBucketSel)
	ON_CBN_DROPDOWN(IDC_BUCKET_SEL, OnDropdownBucketSel)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnEnQuery32Bucket)
	ON_BN_CLICKED(IDC_CLEARSHOW, OnClearshow)
	ON_BN_CLICKED(IDC_STOP_QUERY, OnStopQuery)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATUSBAR_SHOW, OnStatusShow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentParam message handlers

BOOL CEnvirmentParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//
//	m_Block_Sel.SetCurSel(g_Blockindex);
	// TODO: Add extra initialization here
	bKillTimerFinished = false;
	irecCounter = 0;
	iENPara_ShowLineNum = 0;
	m_bEnQuery32Bucket = false;
	m_loopnum = 0;
	
	SetDlgItemText(IDC_THEM_STATIC,"XXX");
	SetDlgItemText(IDC_VOLTAGE_STATIC,"XXX");
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,"XXX");

	KillTimer(1);

	pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	pStatus->SetPaneInfo(0,5,SBPS_NORMAL,230);
	pStatus->SetPaneText(0,"发送数据：",1);
	pStatus->SetPaneInfo(1,5,SBPS_NORMAL,300);
	pStatus->SetPaneText(1,"* * *",1);
	pStatus->SetPaneInfo(2,5,SBPS_NORMAL,500);
	pStatus->SetPaneText(2,"◎ ◎ ◎ ◎ ◎",1);

	memset(m_iArrayTemp,0,32*4);
	m_iSumTemp = 0;
	m_iRecTempNo = 0;
	for(int i=0;i<590;i++) m_fdrawtempdata[i] = 22.5;
	memset(recData,0,100);

	m_iTempHighEth = 25;
	m_iTempLowEth = 20;

	m_bEnUcrcError = true;
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

	g_pThread_EnPara = AfxBeginThread(ReadProcEnvirmParam,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_EnPara->m_bAutoDelete = FALSE;
	g_pThread_EnPara->ResumeThread();


	ResetEvent(g_hEvtRec);
	g_bEvtSend = false;	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvirmentParam::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent) {
		case 1:	{
				int nBucketindex = m_Bucket_Sel.GetCurSel();
				g_Bucketindex = m_Bucket_Sel.GetCurSel();

				if(m_bEnQuery32Bucket) { 
					m_loopnum ++;
					if(m_loopnum >= 32) m_loopnum = 0;
					txdData[0] = m_loopnum + 0x80;										
				}
				else {
					txdData[0] = nBucketindex + 0x80;;
				}
				txdData[1] = 0xb0;
				txdData[2] = 0x08;
				txdData[3] = 0xb0;
				txdData[4] = 0x00;
				txdData[5] = 0x96;				
				txdData[6] = txdData[0];				
				for(int i=1;i<6;i++)
						txdData[6]^=txdData[i];
				txdData[7] = 0x88;

				g_bEvtSend = true;
				ResetEvent(g_hEvtRec);
				Sleep(100);
				ClearCommRecBuff();
				SetEvent(g_hEvtRec);

				//ASSERT(SetTimer(2,1000,NULL)!=0);
				SetTimer(2,1300,NULL);

				if(	!bKillTimerFinished ) 	{
					#ifdef _SELECT_232_PORT
						int nWR = WriteCommandMaint232(8,txdData);
					#else
						int nWR = WriteCommandMaintainMod(8,txdData);
					#endif
					if(nWR == false) {
						AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
					}
				}
				char chtemp[100];
				sprintf(chtemp,"发送数据包： %.2x  %.2x  %.2x  %.2x  %.2x  %.2x   %.2x  %.2x",txdData[0],txdData[1],
					txdData[2],txdData[3],txdData[4],txdData[5],txdData[6],txdData[7]);
				pStatus->SetPaneText(0,chtemp,1);

				break;
				};
		case 2:	 
				KillTimer(2);			
				EnterCriticalSection(&g_ClkRecSafe);
				memset(recData,0,100);				
				irecCounter = 0; 
				recData[2] = 4;					
				ResetEvent(g_hEvtRec);
				g_bEvtSend = false;					

				SetDlgItemText(IDC_THEM_STATIC,"XXX");

				PrtString(txdData, false);

				LeaveCriticalSection(&g_ClkRecSafe);							
				break;
		default: break;
	};

	
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CEnvirmentParam, CDialog)
    //{{AFX_EVENTSINK_MAP(CEnvirmentParam)
	ON_EVENT(CEnvirmentParam, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CEnvirmentParam::OnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	VARIANT vResponse;
	int		k;
	CByteArray	array;
	unsigned char *str1;
	
	if (m_MSComm.GetCommEvent() == 2) {  // ComEvReceive==2 接收到字节
		k=m_MSComm.GetInBufferCount();
		if (k>0) {		
			m_MSComm.SetInputLen(k);
			vResponse=m_MSComm.GetInput();   //读取接收到的数据
			str1 = (unsigned char *)vResponse.parray->pvData;
			
			for(int i=0;i<k;i++) recData[irecCounter+i] = str1[i];

			irecCounter += k;
			if(irecCounter >= 12) {
				if(recData[5] < 10) {
					m_strVali_Static.Format("    %d.0%d", recData[6],recData[5]);
				}else m_strVali_Static.Format("    %d.%d", recData[6],recData[5]);

				if(recData[7] < 10) {
					m_strVali_Static.Format("    %d.0%d", recData[8],recData[7]);
				}else m_strNvali_Static.Format("    %d.%d", recData[8],recData[7]);
				
				m_strThem_Static.Format("    %d",recData[9]);

				UpdateData(false);
				m_strNvali_Static.Empty();
				m_strVali_Static.Empty();
				m_strThem_Static.Empty();

				irecCounter = 0;
			};

		};
	};		
	
}

void CEnvirmentParam::OnButtStartQuery() 
{
	// TODO: Add your control notification handler code here
	int nBucketindex = m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	//g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex


		
	SetDlgItemText(IDC_THEM_STATIC,"XXX");
	SetDlgItemText(IDC_VOLTAGE_STATIC,"XXX");
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,"XXX");

	m_bEnUcrcError = true;
	if(nBucketindex == -1) AfxMessageBox(" 请选择 Bucket 号 ！",MB_ICONINFORMATION);
	else {
			
			//txdData[0] = nBucketindex;
			if(m_bEnQuery32Bucket) { 					
					m_loopnum = 0;
					txdData[0] = m_loopnum + 0x80;										
			}
			else {
					txdData[0] = nBucketindex + 0x80;;
			}

			txdData[1] = 0xb0;
			txdData[2] = 0x08;
			txdData[3] = 0xb0;
			txdData[4] = 0x00;
			txdData[5] = 0x96;			
			txdData[6] = txdData[0];
			for(int i=1;i<6;i++)
					txdData[6]^=txdData[i];
			txdData[7] = 0x88;

			if(g_bEvtSend) { AfxMessageBox("正在等待上一条命令的应答，稍后"); return;};
			g_bEvtSend = true;
			ResetEvent(g_hEvtRec);
			Sleep(200);
			ClearCommRecBuff();
			SetEvent(g_hEvtRec);
			//ASSERT(SetTimer(2,1000,NULL)!=0);
			SetTimer(2,1000,NULL);
				
			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(8,txdData);
			#else
				int nWR = WriteCommandMaintainMod(8,txdData);
			#endif
			if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}

			bKillTimerFinished = false;
			char chtemp[100];
			sprintf(chtemp,"发送数据包： %.2x  %.2x  %.2x  %.2x  %.2x  %.2x   %.2x  %.2x",txdData[0],txdData[1],
				txdData[2],txdData[3],txdData[4],txdData[5],txdData[6],txdData[7]);
			pStatus->SetPaneText(0,chtemp,1);

			//ASSERT(SetTimer(1,1500,NULL)!=0);
			SetTimer(1,1500,NULL);
	}
	
}

void CEnvirmentParam::SendCommand(unsigned char *CommandStr, int SendNum)
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

void CEnvirmentParam::OnOKe() 
{
	KillTimer(2);
	SetEvent(g_hEvtRec);
	g_bEvtSend = false;
	KillTimer(1);

	if(g_bRunThreadEnvirmParam) {
		g_bRunThreadEnvirmParam = false;
		Sleep(300);
		delete g_pThread_EnPara;
	}
	pStatus->SetPaneText(0," ",1);
	pStatus->SetPaneText(1,"＃＃",1);
	pStatus->SetPaneText(2," ",1);
	CDialog::OnOK();
	
}

void CEnvirmentParam::OnSelchangeBucketSel() 
{
	
}

void CEnvirmentParam::OnDropdownBucketSel() 
{
	// TODO: Add your control notification handler code here
	KillTimer(2);
	ResetEvent(g_hEvtRec);
	g_bEvtSend = false;

	KillTimer(1);
	bKillTimerFinished = true;

		
	SetDlgItemText(IDC_THEM_STATIC,"XXX");
	SetDlgItemText(IDC_VOLTAGE_STATIC,"XXX");
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,"XXX");

}


//-------------------------------------------------------------------//
UINT ReadProcEnvirmParam(LPVOID  lpData)
{
	int size;
	unsigned char data[100];
	CString strtemp;

	CEnvirmentParam *ph=(CEnvirmentParam *)lpData ;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	ph->recData[2] = 4;  //接收缓冲复位
	g_bRunThreadEnvirmParam = true;
	while (g_bRunThreadEnvirmParam) 
	{

		WaitForSingleObject(g_hEvtRec, -1);

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadEnvirmParam = false;
		}
		if(size > 0) {
			EnterCriticalSection(&g_ClkRecSafe);
			for(int i=0;i<size;i++) ph->recData[ph->irecCounter+i] = data[i];

			ph->irecCounter += size;
			if(ph->irecCounter >= ph->recData[2]) { //13
				ph->KillTimer(2);
				memcpy(ph->m_uchStatusShow,ph->recData,ph->recData[2]);
				ph->PostMessage(WM_STATUSBAR_SHOW,ph->recData[2], 0);  //akang 11-17
				//ph->SendMessage(WM_STATUSBAR_SHOW,ph->recData[2], 0);

				if( !UCRC(ph->recData)||(ph->recData[4] != 0xb0) ) {  //UCRC 和 命令字校验
					if(ph->m_bEnUcrcError) {
						CString strucrcerror;
						strucrcerror.Format("UCRC错误!\n 原地址:%2.2x",ph->recData[1]);

						ph->SetDlgItemText(IDC_ENVIREN_STATE,"");
						//akang  ph->m_bEnUcrcError = false;											
						ph->SetDlgItemText(IDC_ENVIREN_STATE,strucrcerror);//"UCRC!!"						
					}
				}else {
					if(ph->recData[5] > 0) { //  环境参数不正常
						CString strshow = "";
						if((ph->recData[5]&0x01) == 0x01) { strshow += "正电压报警!";}
						if((ph->recData[5]&0x02) == 0x02) { strshow += "负电压报警!";}
						if((ph->recData[5]&0x04) == 0x04) { strshow += "温度报警!";}
						if(ph->recData[5]&0xf8) { strshow += "未知报警!\n";	}
					}
					if(ph->recData[6] < 10) {
						strtemp.Format("    %d.0%d",ph->recData[7],ph->recData[6]);									
					}else strtemp.Format("    %d.%d",ph->recData[7],ph->recData[6]);
									
				
					if(ph->recData[8] < 10) {
						strtemp.Format("    %d.0%d",ph->recData[9],ph->recData[8]);									
					}else strtemp.Format("    %d.%d",ph->recData[9],ph->recData[8]);
									
					ph->PrtString(ph->recData, true);

				} //end ucrc				
				ph->KillTimer(2);
				ph->irecCounter = 0;
				ph->recData[2] = 4;   //接收缓冲复位
				ResetEvent(g_hEvtRec);	
				g_bEvtSend = false;
				
			};
			LeaveCriticalSection(&g_ClkRecSafe);
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

void CEnvirmentParam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);

	KillTimer(2);
	SetEvent(g_hEvtRec);
	g_bEvtSend = false;

	if(g_bRunThreadEnvirmParam) {
		g_bRunThreadEnvirmParam = false;
		Sleep(300);
		delete g_pThread_EnPara;
	}
	
	pStatus->SetPaneText(0," ",1);
	pStatus->SetPaneText(1,"＃＃",1);
	pStatus->SetPaneText(2," ",1);

	CDialog::OnClose();
}

void CEnvirmentParam::OnDestroy() 
{
	CDialog::OnDestroy();

	KillTimer(1);
	KillTimer(2);
	SetEvent(g_hEvtRec);
	g_bEvtSend = false;

	if(g_bRunThreadEnvirmParam) {
		g_bRunThreadEnvirmParam = false;
		Sleep(300);
		delete g_pThread_EnPara;
	}

	pStatus->SetPaneText(0," ",1);
	pStatus->SetPaneText(1,"＃＃",1);
	pStatus->SetPaneText(2," ",1);
	
}

LRESULT CEnvirmentParam::OnStatusShow(WPARAM wParam, LPARAM lParam)
{
	CString strStatus;
	CString strtemp;

	strStatus = "接收数据包：";
		for(int i=0;i<wParam;i++) {
			strtemp.Format("  %.2x",m_uchStatusShow[i]);
			strStatus += strtemp;
		}
	
	pStatus->SetPaneText(2,strStatus,1);

	return 0L;
}

void CEnvirmentParam::PrtString(unsigned char *pdata, BOOL enshow)
{
	if(iENPara_ShowLineNum >= 32*30)  {
		iENPara_ShowLineNum = 32;
		m_strENPara_Show = "<-------------------------------------------------------------------->\r\n";
	}
    CString strtemp;

	float averagetemp;
	CString stravertemp;
	//平均温度
	m_iSumTemp -= m_iArrayTemp[iENPara_ShowLineNum % 32]; 
	m_iArrayTemp[iENPara_ShowLineNum % 32] = pdata[10];
	m_iSumTemp += pdata[10];
	if(iENPara_ShowLineNum < 32) {		
		averagetemp = (float)m_iSumTemp / (iENPara_ShowLineNum +1);
	}
	else {
		averagetemp = (float)m_iSumTemp / 32;
	}

	stravertemp.Format("%2.2f",averagetemp);
	SetDlgItemText(IDC_THEM_STATIC,stravertemp);

	OnDrawTempLine(averagetemp);

	if(enshow) {
		strtemp.Format(" Bucket %.2d: 正电压: %d.%.2d  负电压: %d.%.2d  温度: %d 状态:",pdata[1] - 0x80, pdata[7],pdata[6],pdata[9],pdata[8],pdata[10]);
		
		if(pdata[5] > 0) {
			if((pdata[5]&0x01) == 0x01) { strtemp += "正电压报警 !";}
			if((pdata[5]&0x02) == 0x02) { strtemp += "负电压报警 !";}
			if((pdata[5]&0x04) == 0x04) { strtemp += "温度报警 !";}
			if(pdata[5]&0xf8) { strtemp += "未知报警!";	}
		}
		else {
			strtemp += "正常!";
		    strtemp += "     平均温度=>" + stravertemp;
		}
		strtemp += "\r\n";
	}
	else {
		strtemp.Format(" 没有收到数据:      正电压： **** 负电压: ****  温度: ****   \r\n");
	}

	//平均正电压
	m_fSumVoltage -= m_fArrayVoltage[iENPara_ShowLineNum % 32]; 
	m_fArrayVoltage[iENPara_ShowLineNum % 32] = (float)pdata[7]+ (float)pdata[6]/100;
	m_fSumVoltage += (float)pdata[7]+ (float)pdata[6]/100;
	if(iENPara_ShowLineNum < 32) {		
		averagetemp = (float)m_fSumVoltage / (iENPara_ShowLineNum +1);
	}
	else {
		averagetemp = (float)m_fSumVoltage / 32;
	}

	stravertemp.Format("%2.2f",averagetemp);
	SetDlgItemText(IDC_VOLTAGE_STATIC,stravertemp);

	//平均负电压
	m_fSumNegVoltage -= m_fArrayNegVoltage[iENPara_ShowLineNum % 32]; 
	m_fArrayNegVoltage[iENPara_ShowLineNum % 32] = (float)pdata[9]+ (float)pdata[8]/100;
	m_fSumNegVoltage += (float)pdata[9]+ (float)pdata[8]/100;
	if(iENPara_ShowLineNum < 32) {		
		averagetemp = (float)m_fSumNegVoltage / (iENPara_ShowLineNum +1);
	}
	else {
		averagetemp = (float)m_fSumNegVoltage / 32;
	}

	stravertemp.Format("-%2.2f",averagetemp);
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,stravertemp);


	m_strENPara_Show += strtemp;
	iENPara_ShowLineNum ++;	
	if(iENPara_ShowLineNum % 32 == 0) { 
		strtemp = "<----------------------------------------------------------------------->\r\n";
		m_strENPara_Show += strtemp;
	}

	SetDlgItemText(IDC_ENPARA_SHOW,m_strENPara_Show);

	m_EditENPara_Show.LineScroll(iENPara_ShowLineNum, 0);

}

void CEnvirmentParam::OnEnQuery32Bucket() 
{
	m_bEnQuery32Bucket = (!m_bEnQuery32Bucket);

	g_Bucketindex = 0;
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//


	if(m_bEnQuery32Bucket) {
		GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(false);
	}
	else GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(true);
	
}

void CEnvirmentParam::OnClearshow() 
{
	iENPara_ShowLineNum = 0;
	m_strENPara_Show = "<-------------------------------------------------------------------->\r\n";
	SetDlgItemText(IDC_ENPARA_SHOW,m_strENPara_Show);

	SetDlgItemText(IDC_THEM_STATIC," ");
	SetDlgItemText(IDC_VOLTAGE_STATIC,"  ");
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,"  ");

	memset(m_iArrayTemp,0,32*4);
	m_iSumTemp = 0;
	m_iRecTempNo = 0;
	for(int i=0;i<590;i++) m_fdrawtempdata[i] = 22.5;

	m_iTempHighEth = 25;
	m_iTempLowEth = 20;
	m_fSumVoltage = 0;
	memset(m_fArrayVoltage,0,32*4);
	m_fSumNegVoltage = 0;
	memset(m_fArrayNegVoltage,0,32*4);
}

void CEnvirmentParam::OnStopQuery() 
{
	KillTimer(1);
	KillTimer(2);
	g_bEvtSend = false;
	iENPara_ShowLineNum = 0;
	
	memset(m_iArrayTemp,0,32*4);
	m_iSumTemp = 0;
	SetDlgItemText(IDC_THEM_STATIC," ");
	SetDlgItemText(IDC_VOLTAGE_STATIC,"  ");
	SetDlgItemText(IDC_NEGVOLTAGE_STATIC,"  ");

	m_iRecTempNo = 0;
	for(int i=0;i<590;i++) m_fdrawtempdata[i] = 22.5;

	m_iTempHighEth = 25;
	m_iTempLowEth = 20;
	m_fSumVoltage = 0;
	memset(m_fArrayVoltage,0,32*4);
	m_fSumNegVoltage = 0;
	memset(m_fArrayNegVoltage,0,32*4);

}

void CEnvirmentParam::OnDrawTempLine(float tempvalue)
{
	CDC *pdc = GetDC();

	CRect clientrect;
	GetClientRect(clientrect);
	pdc->FillSolidRect(24,64,590,153,RGB(219,206,198)); 

	CPen pShow(PS_SOLID,1,RGB(0,128,0));
	pdc->SelectObject(&pShow);
	
	pdc->MoveTo(25,65);
	pdc->LineTo(25+590,65);

	pdc->MoveTo(25,65+ 76 - 45);
	pdc->LineTo(25+590,65+ 76 - 45);

	pdc->MoveTo(25,65+ 76 - 15);
	pdc->LineTo(25+590,65+ 76 - 15);

	pdc->MoveTo(25,65+ 76 +15);
	pdc->LineTo(25+590,65+ 76 + 15);

	pdc->MoveTo(25,65+ 76 + 45);
	pdc->LineTo(25+590,65+ 76 + 45);

	pdc->MoveTo(25,65+153);
	pdc->LineTo(25+590,65+153);

	CPen pShow2(PS_SOLID,1,RGB(0,0,255));//   RGB(0,210,0)
	pdc->SelectObject(&pShow2);

	//一度对应30个象素
	 
	if(tempvalue >= m_iTempHighEth) {
		int kkk =  (int)(tempvalue - m_iTempHighEth + 1);
		m_iTempHighEth += kkk;
		m_iTempLowEth += kkk;
	}
	if(tempvalue <= m_iTempLowEth) {
		int kkk = (int)(m_iTempLowEth - tempvalue + 1);
		m_iTempHighEth -= kkk;
		m_iTempLowEth -= kkk;
	}
	
	float iTempMiddle = (float)(m_iTempHighEth + m_iTempLowEth)/2;
  
	if(m_iRecTempNo > 589) {
		m_iRecTempNo = 589;
		for(int i=1;i<= m_iRecTempNo;i++) {
			m_fdrawtempdata[i-1] = m_fdrawtempdata[i];
		}
	}
	m_fdrawtempdata[m_iRecTempNo] = tempvalue;

	if( m_fdrawtempdata[0] > m_iTempHighEth) {
		pdc->MoveTo(25,65);
	}
	else if(m_fdrawtempdata[0] < m_iTempLowEth) {
		pdc->MoveTo(25,65+153);
	}
	else {
		pdc->MoveTo(25,65+76 + ((iTempMiddle- (float)m_fdrawtempdata[0])*30));
	}

	for(int i=0;i<=m_iRecTempNo;i++) {
		if( m_fdrawtempdata[i] > m_iTempHighEth) {
			pdc->LineTo(25+i,65);
		}
		else if(m_fdrawtempdata[i] < m_iTempLowEth) {
			pdc->LineTo(25+i,65+153);
		}
		else {
			pdc->LineTo(25+i,65+76 + ((iTempMiddle- (float)m_fdrawtempdata[i])*30)); //m_fdrawtempdata[i]
		}		
	}

	m_iRecTempNo ++;
	
	char chtextout[10];
	sprintf(chtextout,"%d",m_iTempHighEth);
	pdc->TextOut(6,58,chtextout);
	sprintf(chtextout,"%d",m_iTempHighEth-1);
	pdc->TextOut(6,85,chtextout);
	sprintf(chtextout,"%d",m_iTempHighEth-2);
	pdc->TextOut(6,114,chtextout);
	sprintf(chtextout,"%d",m_iTempHighEth-3);
	pdc->TextOut(6,145,chtextout);
	sprintf(chtextout,"%d",m_iTempHighEth-4);
	pdc->TextOut(6,177,chtextout);
	sprintf(chtextout,"%d",m_iTempHighEth-5);
	pdc->TextOut(6,208,chtextout);


	ReleaseDC(pdc);
}
