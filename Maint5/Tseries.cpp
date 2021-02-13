// Tseries.cpp : implementation file

#include "stdafx.h"
#include "Maint.h"
#include "Tseries.h"

#include "MaintDLL.h"
#include "PetApi.h"
#include "G_Fuction.h"

#include <math.h>                   // for exp(), log(), and log10()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT ReadProcTSeries(LPVOID  lpData);
//HANDLE hReadHandleTSeries;
CWinThread *g_pThread_Tseries;

//extern bool UCRC(unsigned char* buff);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

bool g_bRunThreadTSeries;
/////////////////////////////////////////////////////////////////////////////
// CTseries dialog


CTseries::CTseries(CWnd* pParent /*=NULL*/)
	: CDialog(CTseries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTseries)
	//}}AFX_DATA_INIT
}


void CTseries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTseries)
	DDX_Control(pDX, IDC_BLOCK_SEL, m_Block_Sel);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_TSERIES_EDIT, m_Tseries_Edit);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTseries, CDialog)
	//{{AFX_MSG_MAP(CTseries)
	ON_BN_CLICKED(IDC_INPUT_RADIO, OnInputRadio)
	ON_BN_CLICKED(IDC_OUTPUT_RADIO, OnOutputRadio)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_EN_KILLFOCUS(IDC_TSERIES_EDIT, OnKillfocusTseriesEdit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EEPROM_RADIO, OnEepromRadio)
	ON_BN_CLICKED(IDC_FPGA_RADIO, OnFpgaRadio)
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTseries message handlers

BEGIN_EVENTSINK_MAP(CTseries, CDialog)
    //{{AFX_EVENTSINK_MAP(CTseries)
	ON_EVENT(CTseries, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CTseries, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTseries::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	long fpRow=m_Grid.GetRowSel();
	long fpCol=m_Grid.GetColSel();
	CDC* pDc=GetDC();
//	ASSERT(pDc);
	int nTwipx=1440/pDc->GetDeviceCaps(LOGPIXELSX);
	int nTwipy=1440/pDc->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pDc);
	long fpCellWidth=m_Grid.GetCellWidth()/nTwipx;
	long fpCellHeight =m_Grid.GetCellHeight()/nTwipy;
	long fpCellLeft=m_Grid.GetCellLeft()/nTwipx;
	long fpCellTop=m_Grid.GetCellTop()/nTwipy;
	m_Tseries_Edit.SetWindowPos(&wndTop,
		                           fpCellLeft+12,
                                   fpCellTop+87,
							       fpCellWidth-4,
							       fpCellHeight+2,
							       NULL);
 
    m_Tseries_Edit.SetFocus();
    m_Tseries_Edit.ShowWindow(SW_SHOW);// 
    SetDlgItemText(IDC_TSERIES_EDIT,"");
    CString fpData=m_Grid.GetText();
    m_Tseries_Edit.ReplaceSel(fpData);	
}

void CTseries::OnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	VARIANT vResponse;
	int		k;
	CByteArray	array;
	unsigned char *str1;
	CString m_strGrid;
	
	if (m_MSComm.GetCommEvent() == 2) {  // ComEvReceive==2 接收到字节
		k=m_MSComm.GetInBufferCount();
		if (k>0) {		
			m_MSComm.SetInputLen(k);
			vResponse=m_MSComm.GetInput();   //读取接收到的数据
			str1 = (unsigned char *)vResponse.parray->pvData;

			for(int i=0;i<k;i++) 
				strRecData[iRecNum+i] = str1[i];					

			iRecNum += k;
			if(bAckFrame) {
				if((iRecNum >= 8) && (strRecData[5] == 0x55)){	
					iRecNum = 0;
					AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
				}					
			}
			else if(iRecNum >= 39)	{
				iRecNum = 0;
				for(int i=0,t=5;i<32;i++,t++) {                   
						m_Grid.SetRow(i+1);
						m_Grid.SetCol(1);
						rectempl[i][1]=strRecData[t]; 
 						m_strGrid.Format("%d",rectempl[i][1]);
						m_Grid.SetText(m_strGrid);
				}
			} //iRecNum

		};
	};	
}

void CTseries::OnInputRadio() 
{
	// TODO: Add your control notification handler code here
		m_InOut_Sel = 1;
}

void CTseries::OnOutputRadio() 
{
	// TODO: Add your control notification handler code here
		m_InOut_Sel = 2;
}

void CTseries::OnOKe() 
{
	// TODO: Add your control notification handler code here
	unsigned char fpTempdata[17][5];
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	if((nBuindex == -1)||(nBlindex == -1)) {
		AfxMessageBox("请选择Bucket 或 Block ！",MB_ICONINFORMATION);
		return;
	}
	if((m_Chip_Sel < 1)||(m_Chip_Sel >3)) {
		AfxMessageBox("请选择 芯片 ！",MB_ICONINFORMATION);
		return;
	}

    if(m_InOut_Sel == 1) {	//导入						
				TxdData[0]= nBuindex + 0x80; 
				TxdData[1]=0xb0;
				TxdData[2]=73;
				TxdData[3]=0xc3;
				TxdData[4] = (unsigned char)m_Chip_Sel;
				TxdData[5]=nBlindex; 
				TxdData[6]=0x07;
				int t=7;
				for(int i=1;i<17;i++) {
					for(int j=1;j<5;j++){
						m_Grid.SetRow(i);
						m_Grid.SetCol(j);
						fpTempdata[i][j]=atoi(m_Grid.GetText());
						TxdData[t]=fpTempdata[i][j];//
						t++;
					}
				}
				TxdData[71]=TxdData[0];
				for(  i=1;i<71;i++)
					TxdData[71]^=TxdData[i];
				TxdData[72]=0x88;

				#ifdef _SELECT_232_PORT
					int nWR = WriteCommandMaint232(73,TxdData);
				#else
					int nWR = WriteCommandMaintainMod(73,TxdData);
				#endif

				if(nWR == false) {
					AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
				}
				bAckFrame = true;
		
	}
	else if(m_InOut_Sel == 2) {		//导出	
			bAckFrame = false;
			for(int i=1;i<17;i++) {
				for(int j=1;j<5;j++) {
					m_Grid.SetRow(i);
					m_Grid.SetCol(j);
					m_Grid.SetText("00");							
				}
			}
			TxdData[0]= nBuindex + 0x80; 
            TxdData[1]=0xb0;
            TxdData[2]=0x09;
            TxdData[3]=0xc3;
			TxdData[4] = (unsigned char)m_Chip_Sel + 1;
			TxdData[5]=nBlindex; //nBlindex;
            TxdData[6]=0x07;
			TxdData[7]=TxdData[0];
			for(i=1;i<7;i++)
				TxdData[7]^=TxdData[i];
			TxdData[8]=0x88;

			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(9,TxdData);
			#else
				int nWR = WriteCommandMaintainMod(9,TxdData);
			#endif
			if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}
			
	}
	else  AfxMessageBox(" 请选择 输入输出 方式 ！",MB_ICONINFORMATION);	

}

BOOL CTseries::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	CButton* pChipSel=(CButton*)GetDlgItem(IDC_FPGA_RADIO);
	pChipSel->SetCheck(1);
	m_Chip_Sel = 1;  
	CButton* pIOSel=(CButton*)GetDlgItem(IDC_OUTPUT_RADIO);
	pIOSel->SetCheck(1);
	m_InOut_Sel = 2;

	iRecNum = 0;

	m_Bucket_Sel.SetCurSel(g_Bucketindex);//
	m_Block_Sel.SetCurSel(g_Blockindex);
	CString m_strGrid;
	m_Grid.SetCols(5);
	m_Grid.SetRows(17);

	m_Grid.SetCol(0);
	for(int j=1;j<17;j++)
	{
	   m_Grid.SetRow(j);
	   m_strGrid.Format("%d",(j-1)*4);
	   m_Grid.SetText(m_strGrid); 
	}
    m_Grid.SetRow(0);
    m_Grid.SetColWidth(0,600);
	m_Grid.SetRow(0);
	for(int i=1;i<5;i++)
	{ 
	   m_Grid.SetColWidth(i,840);  
	}
	m_Grid.SetCol(0);
    m_Grid.SetText(CString("序号"));

	for(i=1;i<5;i++) {
		m_Grid.SetCol(i);
		m_Grid.SetText(CString(" TSer"));
	}

	for(i=1;i<17;i++) {
		for(int j=1;j<5;j++) {
			m_Grid.SetRow(i);
			m_Grid.SetCol(j);
			m_Grid.SetText("00");							
		}
	}


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

	g_pThread_Tseries = AfxBeginThread(ReadProcTSeries,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_Tseries->m_bAutoDelete = FALSE;
	g_pThread_Tseries->ResumeThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTseries::OnKillfocusTseriesEdit() 
{
	// TODO: Add your control notification handler code here
	CString fpInputData;
	GetDlgItemText(IDC_TSERIES_EDIT,fpInputData);
	if(fpInputData.IsEmpty())
	{
		m_Grid.SetText("");
		return;
	}
	m_Grid.SetText(fpInputData);
	long fpCol=m_Grid.GetColSel();
	if(fpCol>0&&fpCol<=9)
	{
       double fpValue=atol(fpInputData); 
	}

    m_Tseries_Edit.ShowWindow(SW_HIDE);
}

void CTseries::SendCommand(unsigned char *CommandStr, int SendNum)
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

//-------------------------------------------------------------------//
UINT ReadProcTSeries(LPVOID  lpData)
{
	int size;
	unsigned char data[100];
	CString strtemp;

	CTseries *ph=(CTseries *)lpData ;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	ph->strRecData[2] = 4;
	g_bRunThreadTSeries = true;
	while (g_bRunThreadTSeries)
	{
	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadTSeries = false;
		}
		if(size > 0) {
				for(int i=0;i<size;i++) {
					ph->strRecData[ph->iRecNum+i] = data[i];					
				}
				ph->iRecNum += size;

				if(ph->bAckFrame) {
					if((ph->iRecNum >= 8)){	
						ph->iRecNum = 0;
						
						if( !UCRC(ph->strRecData) ) {
							AfxMessageBox(" 接收数据 UCRC 错误!");							
						}
						else {
							if(ph->strRecData[5] == 0x55) {
							AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
							}
							else  if(ph->strRecData[5] == 0x54) {
								AfxMessageBox("数据导入 失败！",MB_ICONINFORMATION);
							}
						}
						ph->strRecData[2] = 4;

					}
				}
				else if((ph->iRecNum) >= 71)	{
					ph->iRecNum = 0;
					if( !UCRC(ph->strRecData) ) {  //UCRC
						AfxMessageBox(" 接收数据 UCRC 错误!");
					}else {
							int t=5;
							for(int i=1;i<17;i++) {
								for(int j=1;j<5;j++) {
									ph->m_Grid.SetRow(i);
									ph->m_Grid.SetCol(j);
									ph->rectempl[i][j]=ph->strRecData[t]; 
									strtemp.Format("%d",ph->rectempl[i][j]);
									ph->m_Grid.SetText(strtemp);
									t++;
								}
							}
					} //end of UCRC	
					ph->strRecData[2] = 4;
					
				} //iRecNum
				
		}
		//else Sleep(10);
	}

#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif

	return 0;
}

void CTseries::OnClose() 
{
	if(g_bRunThreadTSeries) {
		g_bRunThreadTSeries = false;
		Sleep(300);
		delete g_pThread_Tseries;
	}
	CDialog::OnClose();
}

void CTseries::OnEepromRadio() 
{
	m_Chip_Sel = 3;
}

void CTseries::OnFpgaRadio() 
{
	m_Chip_Sel = 1;	
}

void CTseries::OnOut() 
{
	if(g_bRunThreadTSeries) {
		g_bRunThreadTSeries = false;
		Sleep(300);
		delete g_pThread_Tseries;
	}
	CDialog::OnOK();
  
}

void CTseries::OnDestroy() 
{
	if(g_bRunThreadTSeries) {
		g_bRunThreadTSeries = false;
		Sleep(300);
		delete g_pThread_Tseries;
	}	

	CDialog::OnOK();	
}
