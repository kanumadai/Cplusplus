// CommSet.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "CommSet.h"
#include "MaintDLL.h"
#include "PetApi.h"
#include "G_Fuction.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT  ReadProcCommSet(LPVOID  lpData);
HANDLE hReadHandleCommSet;
CWinThread *g_pThread_CommSet;

//extern bool UCRC(unsigned char*buff);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int	g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;
extern int  g_HostBaud;
extern int  g_BucketBaud;
extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

bool g_bRunThreadCommSet;

/////////////////////////////////////////////////////////////////////////////
// CCommSet dialog

CCommSet::CCommSet(CWnd* pParent /*=NULL*/)
	: CDialog(CCommSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommSet)
	m_PowerOn_Check = _T("");
	//}}AFX_DATA_INIT
}


void CCommSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommSet)
	DDX_Control(pDX, IDC_COMBO_SELOBJECT, m_Object_Sel);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_StopBit_Sel);
	DDX_Control(pDX, IDC_COMBO_PORTSET, m_PortSet_Sel);
	DDX_Control(pDX, IDC_COMBO_DATABIT, m_DataBit_Sel);
	DDX_Control(pDX, IDC_COMBO_BAUDSEL, m_Baud_Set);
	DDX_Control(pDX, IDC_MSCOMM, m_MSComm);
	DDX_Text(pDX, IDC_STATIC_POWERCHECK, m_PowerOn_Check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSet, CDialog)
	//{{AFX_MSG_MAP(CCommSet)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_BN_CLICKED(IDC_POWERON_CHECK, OnPoweronCheck)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COMSET_MANUL_CHECK, OnComsetManulCheck)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_BUCKET_SEL, OnSelchangeBucketSel)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SELOBJECT, OnSelchangeComboSelobject)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATUSBAR_SHOW, OnStatusShow)  

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSet message handlers

BOOL CCommSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Bucket_Sel.SetCurSel(g_Bucketindex);//
//	m_Block_Sel.SetCurSel(g_Blockindex);
	if (g_HostBaud == 9600) {
		m_Baud_Set.SetCurSel(1);
	}
	if (g_HostBaud == 38400) {
		m_Baud_Set.SetCurSel(3);
	}
	
	m_DataBit_Sel.SetCurSel(1);
	
	m_Object_Sel.SetCurSel(0);

	m_PortSet_Sel.SetCurSel(g_Port_Sel-1);
	m_StopBit_Sel.SetCurSel(0);

	m_bEnUcrcError = true;
	GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(false);  

	pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	pStatus->SetPaneInfo(0,5,SBPS_NORMAL,230);//400
	pStatus->SetPaneText(0,"�������ݣ�",1);
	pStatus->SetPaneInfo(1,5,SBPS_NORMAL,20);
	pStatus->SetPaneText(1,"����",1);
	pStatus->SetPaneInfo(2,5,SBPS_NORMAL,300);
	pStatus->SetPaneText(2,"��������:",1);

	VERIFY (m_CommSet_progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,CRect(20,295,180,305),this, 1));

	m_strOutMessage = " Bucket �Լ��������: \n/------------------------------------------------------------------------------/ \n";
	m_PollingNo = 0;
	m_bManulCheck = false;
//	GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(false);
	lrecCounter = 0;
#ifdef _SELECT_232_PORT
	if(!OpenShmMaint232(g_Port_Sel,g_HostBaud)) {
//		MessageBox("Communication Port Open Error!");
		m_bCommOpened = false;
		this->SetDlgItemText(IDC_STATIC_POWERCHECK,"�˿ڴ򿪴��󣡣���");
		this->SetDlgItemText(IDOKe,"�򿪶˿�");
		GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(false);  //IDC_BUCKET_SEL
		return true;
	}
#else
	if(!OpenShmMaintain()) {
		AfxMessageBox("OpenShmMaint Error");
		return false;
	}
#endif

//	this->SetDlgItemText(IDOKe,"�رն˿�");
//	this->SetDlgItemText(IDC_STATIC_POWERCHECK,"�˿ڴ򿪳ɹ�");
//	GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
	m_bCommOpened = true;

	g_pThread_CommSet = AfxBeginThread(ReadProcCommSet,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_CommSet->m_bAutoDelete = FALSE;
	g_pThread_CommSet->ResumeThread();

	ResetEvent(g_hEvtRec);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommSet::OnOKe() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(false);
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

	g_Port_Sel = m_PortSet_Sel.GetCurSel() +1; //��˿ںŶ�Ӧ


	if (m_Object_Sel.GetCurSel() == 0) { //�趨����
		g_HostBaud = 9600;
		if (m_Baud_Set.GetCurSel() == 1) {
			g_HostBaud = 9600;
		}
		if (m_Baud_Set.GetCurSel() == 3) {
			g_HostBaud = 38400;
		}
	}
	if (m_Object_Sel.GetCurSel() == 1) { //Bucket�趨
		g_BucketBaud = 9600;
		if (m_Baud_Set.GetCurSel() == 1) {
			g_BucketBaud = 9600;			
		}
		if (m_Baud_Set.GetCurSel() == 3) {
			g_BucketBaud = 38400;			
		}
		unsigned char SetBaud[10] = {0x80,0xb0,0x07,0xc0,0x38,0x55,0x88};
		char chrtext[100];
		if (m_bManulCheck) {		
			sprintf(chrtext,"��ǰ����������Ϊ %d bps \r\nȷ��Ҫ�� Bucket %d �趨Ϊ %d bps \r\n",g_HostBaud,g_Bucketindex,g_BucketBaud);
			SetBaud[0] = 0x80 + g_Bucketindex;
		}
		else {
			sprintf(chrtext,"��ǰ����������Ϊ %d bps \r\nȷ��Ҫ�� 32 Bucket �趨Ϊ %d bps \r\n",g_HostBaud,g_BucketBaud);
			SetBaud[0] = 0x70;
		}
		if ( IDYES != AfxMessageBox(chrtext,MB_YESNO)) {
			GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
			return;
		}
		
		g_HostBaud = g_BucketBaud;		

		if (g_BucketBaud == 9600) {
			SetBaud[4] = 0x96;
		}
		if (g_BucketBaud == 38400) {
			SetBaud[4] = 0x38;
		}
		SetBaud[5] = SetBaud[0];
		for(int i=1;i<5;i++) {
			SetBaud[5] ^= SetBaud[i];
		}
		
		#ifdef _SELECT_232_PORT
			int nWR = WriteCommandMaint232(7,SetBaud);
		#else
			int nWR = WriteCommandMaintainMod(7,SetBaud);
		#endif
		if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}
		char chtemp[100];
		sprintf(chtemp,"�������ݰ��� %.2x  %.2x  %.2x  %.2x  %.2x  %.2x %.2x",SetBaud[0],SetBaud[1],SetBaud[2],SetBaud[3],SetBaud[4],SetBaud[5],SetBaud[6]);
		pStatus->SetPaneText(0,chtemp,1);

	}


	SetEvent(g_hEvtRec);
	if(g_bRunThreadCommSet) {
		g_bRunThreadCommSet = false;
		Sleep(300);
		delete g_pThread_CommSet;
	}

	CDialog::OnCancel();
}

void CCommSet::OnCancel() 
{
	SetEvent(g_hEvtRec);
	if(g_bRunThreadCommSet) {
		g_bRunThreadCommSet = false;
		Sleep(300);
		delete g_pThread_CommSet;
	}

	pStatus->SetPaneText(0," ",1);
	pStatus->SetPaneText(1,"����",1);
	pStatus->SetPaneText(2," ",1);
	CDialog::OnOK();
	
}

BEGIN_EVENTSINK_MAP(CCommSet, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommSet)
	ON_EVENT(CCommSet, IDC_MSCOMM, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommSet::OnPoweronCheck() 
{
	unsigned char PowerOn_Check[6]=	{0x70,0xb0,0x06,0xce,0x08,0x88};
   
	int nBuindex=m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex


	crecData[2] = 4;	//���ջ��帴λ
	lrecCounter = 0;
	m_bEnUcrcError = true;

	m_iRecBuckNo = 55;	// Bucket���ڲ�����״̬
	for(int k=0;k<32;k++)	//Ĭ�������ʽΪû���յ�����
		m_OutputFormat[k] = 0x00;	

	if(m_bManulCheck) {
		SetDlgItemText(IDC_STATIC_POWERCHECK,"���ڽ���Bucket�Լ�...");

		m_CommSet_progress.SetRange(0, 0);
		m_CommSet_progress.SetRange(0, 32);
		m_CommSet_progress.SetStep(1);
		GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(false);

		SetTimer(1,3500,NULL);
		m_PollingNo = 0;
	}
	else {
		m_CommSet_progress.SetRange(0, 32);
		m_CommSet_progress.SetStep(1);

		m_PollingNo = -1;

		SetTimer(1,3500,NULL);
		GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(false);		
		GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(false);
		m_strOutMessage = " Bucket �Լ��������: \n/------------------------------------------------------------------------------/ \n";

		m_RecStatus = 0x00;
		SetDlgItemText(IDC_STATIC_POWERCHECK," ");
	}

	ResetEvent(g_hEvtRec);
	Sleep(200);
	ClearCommRecBuff();
	SetEvent(g_hEvtRec);
	
	#ifdef _SELECT_232_PORT
			int nWR = WriteCommandMaint232(6,PowerOn_Check);
	#else
			int nWR = WriteCommandMaintainMod(6,PowerOn_Check);
	#endif
	if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	char chtemp[100];
	sprintf(chtemp,"�������ݰ��� %.2x  %.2x  %.2x  %.2x  %.2x  %.2x",PowerOn_Check[0],PowerOn_Check[1],PowerOn_Check[2],PowerOn_Check[3],PowerOn_Check[4],PowerOn_Check[5]);
	pStatus->SetPaneText(0,chtemp,1);	

}

void CCommSet::OnCommMscomm() 
{
	VARIANT vResponse;
	int		k;
	CByteArray	array;
	unsigned char *str1;
	
	if (m_MSComm.GetCommEvent() == 2) {  // ComEvReceive==2 ���յ��ֽ�
		k=m_MSComm.GetInBufferCount();
		if (k>0) {		
			m_MSComm.SetInputLen(k);
			vResponse=m_MSComm.GetInput();   //��ȡ���յ�������
			str1 = (unsigned char *)vResponse.parray->pvData;
			for(int i=0;i<k;i++) crecData[lrecCounter+i] = str1[i];			
			lrecCounter += k;
			
			if(lrecCounter >= 0x08) {
				if(crecData[5] == 0x55) {
					m_PowerOn_Check = "Bucket �Լ��ɹ���";
					lrecCounter = 0;
					UpdateData(false);
				};
			};			

		};
	};	
}

void CCommSet::SendCommand(unsigned char *CommandStr, int SendNum)
{
	CByteArray Sarray;   
	
    Sarray.RemoveAll();
    Sarray.SetSize(SendNum);

    for( int i=0;i<SendNum;i++)
	    Sarray.SetAt(i,CommandStr[i]);

	if (!m_MSComm.GetPortOpen())    //�򿪴��ڲ���������
					m_MSComm.SetPortOpen(true);
    m_MSComm.SetOutput(COleVariant(Sarray));

}



UINT  ReadProcCommSet(LPVOID  lpData)
{
	int size;
	unsigned char data[100];
	CString m_strGrid;
	CCommSet *ph=(CCommSet *)lpData;

	ph->crecData[2] = 4;

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	g_bRunThreadCommSet = true;
	while (g_bRunThreadCommSet)
	{
		WaitForSingleObject(g_hEvtRec, -1);	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadCommSet = false;
		}
		if(size > 0) {	
			
			#ifdef _SELECT_232_PORT
			#else
				if(data[2] != size) {			 // test 
					AfxMessageBox("�����Ȳ���ͷ");				
				}  
			#endif

		
			for(int i=0;i<size;i++) {
				ph->crecData[ph->lrecCounter+i] = data[i];				
			}

			ph->lrecCounter += size;   			

			if((ph->lrecCounter) >= ph->crecData[2]) {//0x08
				
				memcpy(ph->m_uchStatusShow,ph->crecData,ph->crecData[2]);
				//ph->PostMessage(WM_STATUSBAR_SHOW,ph->crecData[2], 0); //akang 11-17
				ph->SendMessage(WM_STATUSBAR_SHOW,ph->crecData[2], 0);

				if( !UCRC(ph->crecData) ) { // !UCRC(ph->crecData) //UCRC
					ph->lrecCounter = 0;
					ph->crecData[2] = 4;

					if(ph->m_bEnUcrcError) {
						CString strucrcerror;
						strucrcerror.Format("��������CRC����!\n ԭ��ַ��0x%2.2x",ph->crecData[1]);
						AfxMessageBox(strucrcerror);
						ph->m_bEnUcrcError = false;

						ph->SetDlgItemText(IDC_STATIC_POWERCHECK,"Bucket �Լ�� ʧ�ܣ���");
						ph->KillTimer(1);
						ResetEvent(g_hEvtRec);
						ph->m_CommSet_progress.SetRange(0, 0);
						ph->GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
						ph->GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(true);
					}
				}else {
							if(ph->crecData[5] == 0x55) {
								ph->m_RecStatus = 0x55;
								ph->m_iRecBuckNo = ph->crecData[1] - 0x80;		//ָ�����

								if(ph->m_bManulCheck) {
									ph->SetDlgItemText(IDC_STATIC_POWERCHECK,"Bucket �Լ��ɹ���");
									ph->KillTimer(1);
									ResetEvent(g_hEvtRec);
									ph->m_CommSet_progress.SetRange(0, 0);
									ph->GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
									ph->GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(true);
								}
							} 
							else if(ph->crecData[5] == 0x54) {		
								ph->m_iRecBuckNo = ph->crecData[1] - 0x80;		//ָ�����
								switch(ph->crecData[6]) {
									case 0x01:{	ph->m_RecStatus = 0x54; break;};//λ�ô�
									case 0x02:{	ph->m_RecStatus = 0x64;	break;};//������
									case 0x04:{	ph->m_RecStatus = 0x74;	break;};//ʱ��������
									case 0x08:{	ph->m_RecStatus = 0x84;	break;};//ʱ���
									case 0x10:{	ph->m_RecStatus = 0x94;	break;};//�����
									case 0x20:{	ph->m_RecStatus = 0xA4;	break;};//EEprom��
									case 0x40:{ ph->m_RecStatus = 0xB4;	break;};//������
									default: { ph->m_RecStatus = 0xC4; break; };// δ֪��
								}
								if(ph->m_bManulCheck) {
									ph->SetDlgItemText(IDC_STATIC_POWERCHECK,"Bucket�Լ�� ʧ�ܣ���");
									ph->KillTimer(1);
									ResetEvent(g_hEvtRec);
									ph->m_CommSet_progress.SetRange(0, 0);
									ph->GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
									ph->GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(true);
								}
							}
							if((ph->crecData[5] != 0x54)&&(ph->crecData[5] != 0x55)) {
								ph->m_iRecBuckNo = 31;
								ph->m_RecStatus = 0xC4; break;
							}

					}
				ph->lrecCounter = 0;
				ph->crecData[2] = 4;
			};

		}
		//else Sleep(1);
	}
#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif
	return 0;
}

void CCommSet::OnClose() 
{
	SetEvent(g_hEvtRec);
	if(g_bRunThreadCommSet) {
		g_bRunThreadCommSet = false;
		Sleep(300);
		delete g_pThread_CommSet;
	}
	CDialog::OnClose();
}

void CCommSet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
}

void CCommSet::OnComsetManulCheck() 
{
	m_bManulCheck = !m_bManulCheck;
	if(m_bManulCheck) {
		GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(true);
	}
	else GetDlgItem(IDC_BUCKET_SEL)->EnableWindow(false);
}

void CCommSet::OnTimer(UINT nIDEvent) 
{
	unsigned char PowerOn_Check[8]=	{0x80,0xb0,0x08,0xcf,0x00,0x00,0xF8,0x88};

	int nBuindex=m_Bucket_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

	CString strtemp;

	//ASSERT(SetTimer(1,400,NULL)!=0);
	SetTimer(1,1200,NULL);			//akang 11-12
	switch(nIDEvent) {
		case 1:	{				
				m_CommSet_progress.StepIt();

				if((m_iRecBuckNo <= 32)&&(m_iRecBuckNo >=0)) {
					m_OutputFormat[m_iRecBuckNo] = m_RecStatus;
				}
				m_iRecBuckNo = 55; //�´β��ظ�

				m_PollingNo++;
				if(m_PollingNo >= 32) {
						OnOutputFormat();
						KillTimer(1);
						ResetEvent(g_hEvtRec);

						m_CommSet_progress.SetRange(0, 0);
						GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
						GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(true);
						m_PollingNo = 0;
						if(m_bManulCheck) {
							SetDlgItemText(IDC_STATIC_POWERCHECK,"Bucket �Լ� ��ʱ��");
						}
						else{							
							m_strOutMessage += "\n/------------------------------------------------------------------------------/\n";
							AfxMessageBox(m_strOutMessage,MB_ICONINFORMATION);							
							break;
						}
				}			

				if(m_bManulCheck) {	
							PowerOn_Check[0] = nBuindex + 0x80;
							PowerOn_Check[6] = PowerOn_Check[0];
							for(int i=1;i<6;i++) PowerOn_Check[6] ^= PowerOn_Check[i];
				} 
				else {
					PowerOn_Check[0] = m_PollingNo + 0x80;
					PowerOn_Check[6] = PowerOn_Check[0];
					for(int i=1;i<6;i++) PowerOn_Check[6] ^= PowerOn_Check[i];
				}

				crecData[2] = 4;	//���ջ��帴λ
				lrecCounter = 0;

				#ifdef _SELECT_232_PORT
						int nWR = WriteCommandMaint232(8,PowerOn_Check);
				#else
						int nWR = WriteCommandMaintainMod(8,PowerOn_Check);
				#endif
				if(nWR == false) {
						AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
				}
				m_RecStatus = 0;
				char chtemp[100];
				sprintf(chtemp,"�������ݰ��� %.2x  %.2x  %.2x  %.2x  %.2x  %.2x  %.2x  %.2x",PowerOn_Check[0],
					PowerOn_Check[1],PowerOn_Check[2],PowerOn_Check[3],PowerOn_Check[4],PowerOn_Check[5],
					PowerOn_Check[6],PowerOn_Check[7]);
				pStatus->SetPaneText(0,chtemp,1);

				break;
				};
		default: break;
	};	

	CDialog::OnTimer(nIDEvent);
}

void CCommSet::OnDestroy() 
{


	SetEvent(g_hEvtRec);
	if(g_bRunThreadCommSet) {
		g_bRunThreadCommSet = false;
		Sleep(300);
		delete g_pThread_CommSet;
	}

	pStatus->SetPaneText(0," ",1);
	pStatus->SetPaneText(1,"����",1);
	pStatus->SetPaneText(2," ",1);

	Sleep(300);
	// TODO: Add your message handler code here
	//CDialog::OnDestroy();	
	CDialog::OnOK();

}

void CCommSet::OnOutputFormat()
{
	CString strtemp;

	m_strOutMessage = " Bucket �Լ��������: \n/------------------------------------------------------------------------------/ \n";


	for(int i=0;i<32;i++) {
		switch(m_OutputFormat[i]) {
					case 0x00:	{  //״̬δ�仯�����ճ�ʱ
							strtemp.Format("  Bucket %.2d  û���յ�����.            ",i);							
							break;}
					case 0x55: {  //�Լ��ɹ�
							strtemp.Format("  Bucket %.2d  �Լ�ɹ�.                    ",i);//m_PollingNo					
						    break;}
					case 0x54:  {  //
							strtemp.Format("  Bucket %.2d  λ��-У���.               ",i);
							break;}
					case 0x64:  {  //
							strtemp.Format("  Bucket %.2d  ����-У���.               ",i);
							break;}
					case 0x74:  {  //
							strtemp.Format("  Bucket %.2d  ʱ������-����.           ",i);
							break;}
					case 0x84:  {  //
							strtemp.Format("  Bucket %.2d  ʱ��-У���.               ",i);
							break;}
					case 0x94:  {  //
							strtemp.Format("  Bucket %.2d  ����-У���.               ",i);						
							break;}
					case 0xA4:  {  //
							strtemp.Format("  Bucket %.2d  EEprom-У���.           ",i);							
							break;}
					case 0xB4:  {  //
							strtemp.Format("  Bucket %.2d  ��������-Խ��.           ",i);						
							break;}
					case 0xC4:  {  //
							strtemp.Format("  Bucket %.2d  δ֪����....                ",i);							
							break;}
					default: 
							strtemp.Format("  Bucket %.2d  δ֪����....                ",i);
						break;
		}

		m_strOutMessage +=  strtemp;
		if(i%2 == 1) m_strOutMessage += " \n";
	}

	ResetEvent(g_hEvtRec);

}

void CCommSet::OnSelchangeBucketSel() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_POWERON_CHECK)->EnableWindow(true);
	GetDlgItem(IDC_COMSET_MANUL_CHECK)->EnableWindow(true);
	
}


LRESULT CCommSet::OnStatusShow(WPARAM wParam, LPARAM lParam)
{
	CString strStatus;
	CString strtemp;

	strStatus = "�������ݰ���";
		for(int i=0;i< (int)wParam;i++) {
			strtemp.Format("  %.2x",m_uchStatusShow[i]);
			strStatus += strtemp;
		}
	
	pStatus->SetPaneText(2,strStatus,1);

	return 0L;
}


void CCommSet::OnSelchangeComboSelobject() 
{
	if (m_Object_Sel.GetCurSel() == 1) { //Bucket�趨
		if (g_BucketBaud == 9600 )  {
			m_Baud_Set.SetCurSel(1);
		}
		else if (g_BucketBaud == 38400) {
			m_Baud_Set.SetCurSel(3);
		}
		else m_Baud_Set.SetCurSel(0);
	}	
	
	if (m_Object_Sel.GetCurSel() == 0) { //�趨����
		if (g_HostBaud == 9600 )  {
			m_Baud_Set.SetCurSel(1);
		}
		if (g_HostBaud == 38400) {
			m_Baud_Set.SetCurSel(3);
		}
	}

	UpdateData(false);
}
