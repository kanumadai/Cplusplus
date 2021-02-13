// BoardEnerge.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "BoardEnerge.h"
#include "MaintDLL.h"
#include "PetApi.h"
#include "G_Fuction.h"
#include "MaintDoc.h"

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

UINT ReadProcBEnerge(LPVOID  lpData);
HANDLE hReadHandleBEnerge;
CWinThread *g_pThread_BoardEnerge;

//extern bool UCRC(unsigned char*buff);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;
extern BOOL g_bChannelChangeSel;

extern HANDLE	g_hEvtSend;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;


bool g_bRunThreadBEnerge;

/////////////////////////////////////////////////////////////////////////////
// CBoardEnerge dialog


CBoardEnerge::CBoardEnerge(CWnd* pParent /*=NULL*/)
	: CDialog(CBoardEnerge::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoardEnerge)
	m_strEner_E1Value = _T("");
	m_strEner_E2Value = _T("");
	//}}AFX_DATA_INIT
}


void CBoardEnerge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoardEnerge)
	DDX_Control(pDX, IDC_CHANNEL_CHANGE_SEL, m_bChannelChangeSel);
	DDX_Control(pDX, IDC_BOARDENERG_EDIT, m_BoardEnerge_Edit);
	DDX_Control(pDX, IDC_BLOCK_SEL, m_Block_Sel);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_Grid);
	DDX_Text(pDX, IDC_ENER_E1_VALUE, m_strEner_E1Value);
	DDX_Text(pDX, IDC_ENER_E2_VALUE, m_strEner_E2Value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoardEnerge, CDialog)
	//{{AFX_MSG_MAP(CBoardEnerge)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_BN_CLICKED(IDC_OUTPUT_RADIO, OnOutputRadio)
	ON_BN_CLICKED(IDC_INPUT_RADIO, OnInputRadio)
	ON_BN_CLICKED(IDC_FPGA_RADIO, OnFpgaRadio)
	ON_BN_CLICKED(IDC_EEPROM_RADIO, OnEepromRadio)
	ON_EN_KILLFOCUS(IDC_BOARDENERG_EDIT, OnKillfocusBoardenergEdit)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SET_ENER_INIT, OnSetEnerInit)
	ON_BN_CLICKED(IDC_LOADFILE, OnLoadfile)
	ON_CBN_SELCHANGE(IDC_BLOCK_SEL, OnSelchangeBlockSel)
	ON_BN_CLICKED(IDC_CHANNEL_CHANGE_SEL, OnChannelChangeSel)
	ON_CBN_SELCHANGE(IDC_BUCKET_SEL, OnSelchangeBucketSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoardEnerge message handlers

BEGIN_EVENTSINK_MAP(CBoardEnerge, CDialog)
    //{{AFX_EVENTSINK_MAP(CBoardEnerge)
	ON_EVENT(CBoardEnerge, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	ON_EVENT(CBoardEnerge, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBoardEnerge::OnOKe() 
{
	unsigned char fpTempdata[64][4];
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();

	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	iRecNum = 0;
	memset(strRecData,0,80*4);
	strRecData[2] = 4;
	m_iSuccessACKNum = 0;

	if((nBuindex == -1)||(nBlindex == -1)) {
		AfxMessageBox("请选择Bucket或Block！",MB_ICONINFORMATION);
		return;
	}
	if((m_Chip_Sel < 1)||(m_Chip_Sel >3)) {
		AfxMessageBox("请选择 芯片 ！",MB_ICONINFORMATION);
		return;
	}
	
    if(m_InOut_Sel == 1) {	//导入
		for(unsigned char j=1;j<2;j++)	{ // 74*4 
				m_sendnum = 0;
				iRecNum = 0;
				strRecData[2] = 4;

				if(nBlindex == 8) {
						bAckFrame = true;
						m_iSuccessACKNum = 0;
						OnSend8BlockPara();
						return;
				}

				TxdData[0]= nBuindex + 0x80; 
				TxdData[1]=0xb0;
				TxdData[2]=0x49;
				TxdData[3]=0xc3;
				TxdData[4] = (unsigned char)m_Chip_Sel;
				TxdData[5]= nBlindex;  
				TxdData[6]=j+3;
				for(int i=1,t=7;i<65,t<71;i++,t++) {
					m_Grid.SetRow(i);
					m_Grid.SetCol(j);
					fpTempdata[i-1][j-1]=atoi(m_Grid.GetText());
					TxdData[t]=fpTempdata[i-1][j-1];//
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
		}
		bAckFrame = true;
	}
	else if(m_InOut_Sel == 2) {		//导出
		if(nBlindex == 8) {
			AfxMessageBox("通道选择超出范围了！\n程序不支持连续读8个Block");
			return;
		}

		bAckFrame = false;
		for(int j=0;j<2;j++) { 
			for(int i=0;i<64;i++)	{                   
				m_Grid.SetRow(i+1);
				m_Grid.SetCol(j+1);
				m_Grid.SetText("00"); 			
			};
		}
		m_sendnum = 0;
		iRecNum = 0;
		strRecData[2] = 4;
		for(j=0;j<1;j++) {		  
			TxdData[0]= nBuindex + 0x80; 
            TxdData[1]=0xb0;
            TxdData[2]=0x09;
            TxdData[3]=0xc3;
			TxdData[4] = 1 + (unsigned char)m_Chip_Sel ;
			TxdData[5]= nBlindex;
            TxdData[6]=j+4;
			TxdData[7]=TxdData[0];
			for(int i=1;i<7;i++)
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
		
	}
	else  AfxMessageBox(" 请选择 输入输出 方式 ！",MB_ICONINFORMATION);
	
}



void CBoardEnerge::OnCommMscomm() 
{
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
			else if(iRecNum >= 71*2)	{
				iRecNum = 0;
				for(int j=0;j<2;j++) { 
					for(int i=0,t=71*j+5;i<64;i++,t++)
					{                   
						m_Grid.SetRow(i+1);
						m_Grid.SetCol(j+1);
						rectempl[i][j]=strRecData[t]; 
 						m_strGrid.Format("%d",rectempl[i][j]);
						m_Grid.SetText(m_strGrid);
					}
				}
			} //iRecNum

		};
	};
	
}

void CBoardEnerge::OnInputRadio() 
{
	m_InOut_Sel = 1;
	
}

void CBoardEnerge::OnFpgaRadio() 
{
	m_Chip_Sel = 1;	
	
}

void CBoardEnerge::OnEepromRadio() 
{
	m_Chip_Sel = 3;	
}

void CBoardEnerge::OnOutputRadio() 
{
	m_InOut_Sel = 2;	
}

BOOL CBoardEnerge::OnInitDialog() 
{
	CDialog::OnInitDialog();

	iRecNum = 0;
	strRecData[2] = 4;
	m_iSuccessACKNum = 0;

	CButton* pChipSel=(CButton*)GetDlgItem(IDC_FPGA_RADIO);
	pChipSel->SetCheck(1);
	m_Chip_Sel = 1;  

	CButton* pIOSel=(CButton*)GetDlgItem(IDC_OUTPUT_RADIO);
	pIOSel->SetCheck(1);
	m_InOut_Sel = 2;	


	m_Bucket_Sel.SetCurSel(g_Bucketindex);//
	m_Block_Sel.SetCurSel(g_Blockindex);
	CString m_strGrid;
	m_Grid.SetCols(3);
	m_Grid.SetRows(65);

	m_Grid.SetCol(0);
	for(int j=1;j<65;j++)
	{
	   m_Grid.SetRow(j);
	   m_strGrid.Format("%d",j-1);
	   m_Grid.SetText(m_strGrid); 
	}
    m_Grid.SetRow(0);
    m_Grid.SetColWidth(0,500);
	m_Grid.SetRow(0);
	for(int i=1;i<3;i++)
	{ 
	   m_Grid.SetColWidth(i,1825);
	}
	m_Grid.SetCol(1);
    m_Grid.SetText(CString("            E1"));
	m_Grid.SetCol(2);
    m_Grid.SetText(CString("            E2"));

	for(j=0;j<2;j++) { 
		for(int i=0;i<64;i++)	{                   
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(j+1);
			m_Grid.SetText("00"); 			
		};
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

	g_pThread_BoardEnerge = AfxBeginThread(ReadProcBEnerge,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_BoardEnerge->m_bAutoDelete = FALSE;
	g_pThread_BoardEnerge->ResumeThread();


	if(g_bChannelChangeSel) {
		m_bChannelChangeSel.SetCheck(1);
	}else m_bChannelChangeSel.SetCheck(0);
	int nBucklindex=m_Bucket_Sel.GetCurSel();
	if(0 == (nBucklindex%2)) {
		m_bChannelChangeSel.SetCheck(0);
		g_bChannelChangeSel = false;
	}
	else {
		m_bChannelChangeSel.SetCheck(1);
		g_bChannelChangeSel = true;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBoardEnerge::OnDblClickMsflexgrid() 
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
	m_BoardEnerge_Edit.SetWindowPos(&wndTop,
		                           fpCellLeft+14,
                                   fpCellTop+100,
							       fpCellWidth,
							       fpCellHeight+3,
							       NULL);
 
    m_BoardEnerge_Edit.SetFocus();
    m_BoardEnerge_Edit.ShowWindow(SW_SHOW);// 
    SetDlgItemText(IDC_BOARDENERG_EDIT,"");
    CString fpData=m_Grid.GetText();
    m_BoardEnerge_Edit.ReplaceSel(fpData);
	
}

void CBoardEnerge::OnKillfocusBoardenergEdit() 
{
	// TODO: Add your control notification handler code here
	CString fpInputData;
	GetDlgItemText(IDC_BOARDENERG_EDIT,fpInputData);
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
    m_BoardEnerge_Edit.ShowWindow(SW_HIDE);
	
}


void CBoardEnerge::SendCommand(unsigned char *CommandStr, int SendNum)
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

UINT ReadProcBEnerge(LPVOID  lpData)
{
	int nBuindex;
	int nBlindex;
	int workcounter = 0;
	unsigned char TxdData[100];	
	unsigned char fpTempdata[64][4];
	int size;
	unsigned char data[100];

	CString m_strGrid;
	CBoardEnerge *ph=(CBoardEnerge *)lpData ;	
	ph->strRecData[2] = 4;

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	g_bRunThreadBEnerge = true;
	while (g_bRunThreadBEnerge) 
	{
	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadBEnerge = false;
		}
		if(size > 0) {			
			for(int i=0;i<size;i++) 				
				ph->strRecData[ph->iRecNum + workcounter+i] = data[i];

			workcounter += size;

			if(ph->bAckFrame) { //
				if(workcounter>=ph->strRecData[2]) { //判断帧长					
					workcounter = 0;
					ph->iRecNum = 0;					

					if( !UCRC(ph->strRecData) ) {

							AfxMessageBox(" 接收数据 UCRC 错误!");							
					}
					else {
							if(ph->strRecData[5] == 0x55) {
								//-----------------------
								nBlindex = ph->m_Block_Sel.GetCurSel();
								if(nBlindex == 8) {ph->m_sendnum = 10;}

								ph->m_sendnum ++;
								if(ph->m_sendnum < 2)  {
									nBuindex = ph->m_Bucket_Sel.GetCurSel();
									nBlindex = ph->m_Block_Sel.GetCurSel();

									g_Bucketindex = ph->m_Bucket_Sel.GetCurSel();
									g_Blockindex = ph->m_Block_Sel.GetCurSel();
									ph->m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
									ph->m_Block_Sel.SetCurSel(g_Blockindex);

									TxdData[0]= nBuindex + 0x80; 
									TxdData[1]=0xb0;
									TxdData[2]=0x49;
									TxdData[3]=0xc3;
									TxdData[4] = (unsigned char)ph->m_Chip_Sel;
									TxdData[5]= nBlindex; 							
									TxdData[6] = ph->m_sendnum + 4;
									for(int i=1,t=7;i<65,t<71;i++,t++)
									{
										ph->m_Grid.SetRow(i);
										ph->m_Grid.SetCol(ph->m_sendnum +1);
										fpTempdata[i-1][ph->m_sendnum]=atoi(ph->m_Grid.GetText());
										TxdData[t]=fpTempdata[i-1][ph->m_sendnum];//
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
								}
								else {//=========================================	
										ph->iRecNum = 0;
										workcounter = 0;
										ph->m_sendnum = 0;
										nBlindex = ph->m_Block_Sel.GetCurSel();
										if(nBlindex == 8) {
											ph->m_iSuccessACKNum ++;
											//SetEvent(g_hEvtSend);
											if(ph->m_iSuccessACKNum >= 8*2) {
												ph->m_iSuccessACKNum = 0;
												AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
											}
										}
										else {										
											AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
										}
										//=========================================	
								}
							} else if(ph->strRecData[5] == 0x54) {	
								ph->iRecNum = 0;
								workcounter = 0;
								ph->m_sendnum = 0;
								AfxMessageBox("数据导入出错了！！！",MB_ICONINFORMATION);
							}
					} //end ucrc
					ph->strRecData[2] = 4;

				}
			}			
			else if(workcounter >= 71) {
				workcounter = 0;
				ph->iRecNum +=71;
				if(ph->iRecNum >= 71*2) {
					ph->iRecNum = 0;
					ph->m_sendnum = 0;
					if( !UCRC(ph->strRecData) ) {
							AfxMessageBox(" 接收数据 UCRC 错误!");							
					}
					else {
						for(int j=0;j<2;j++) { 
							for(int i=0,t=71*j+5;i<64;i++,t++)	{                   
								ph->m_Grid.SetRow(i+1);
								ph->m_Grid.SetCol(j+1);
								ph->rectempl[i][j]=ph->strRecData[t]; 
 								m_strGrid.Format("%d",ph->rectempl[i][j]);
								ph->m_Grid.SetText(m_strGrid);
							}
						} //end first for()	
					}//end ucrc
				}
				else{
					Sleep(80);
					nBuindex = ph->m_Bucket_Sel.GetCurSel();
					nBlindex = ph->m_Block_Sel.GetCurSel();

					g_Bucketindex = ph->m_Bucket_Sel.GetCurSel();
					g_Blockindex = ph->m_Block_Sel.GetCurSel();
					ph->m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
					ph->m_Block_Sel.SetCurSel(g_Blockindex);

					TxdData[0]= nBuindex + 0x80; 
					TxdData[1]=0xb0;
					TxdData[2]=0x09;
					TxdData[3]=0xc3;
					TxdData[4] = (unsigned char)ph->m_Chip_Sel +1;
					TxdData[5]= nBlindex;

					ph->m_sendnum ++;
					TxdData[6]=ph->m_sendnum +4;
					TxdData[7]=TxdData[0];
					for(int i=1;i<7;i++)
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

			}//end of else
		}
		//else Sleep(5);
	}
#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif
	return 0;
}

void CBoardEnerge::OnCancel() 
{
	if(g_bRunThreadBEnerge) {
		g_bRunThreadBEnerge = false;
		Sleep(300);
		delete g_pThread_BoardEnerge;
	}

	CDialog::OnCancel();
}

void CBoardEnerge::OnClose() 
{
	if(g_bRunThreadBEnerge) {
		g_bRunThreadBEnerge = false;
		Sleep(300);
		delete g_pThread_BoardEnerge;
	}
	CDialog::OnClose();
}

void CBoardEnerge::OnDestroy() 
{
	
	if(g_bRunThreadBEnerge) {
		g_bRunThreadBEnerge = false;
		Sleep(300);
		delete g_pThread_BoardEnerge;
	}
//	CDialog::OnDestroy();	
	CDialog::OnOK();
}

void CBoardEnerge::OnSetEnerInit() 
{
	UpdateData(true);
	UpdateData(false);

	for(int i=0;i<64;i++)	{                   
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(0+1);
		m_Grid.SetText(m_strEner_E1Value); 			
	};

	for(i=0;i<64;i++)	{                   
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(1+1);
		m_Grid.SetText(m_strEner_E2Value); 			
	};

	int e1 = 0;
	int e2 = 0;
	if(!m_strEner_E2Value.IsEmpty()) { //计数模式	
		 e1 = (int)_atoi64(m_strEner_E1Value);
		 e2 = (int)_atoi64(m_strEner_E2Value);
	}

	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {						
			m_uchELimit[block][i][0] = (unsigned char)e1;
			m_uchELimit[block][i][1] = (unsigned char)e2;				
		}
	}
	
}

void CBoardEnerge::OnLoadfile() 
{
	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("能量边界导入.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("文件读错误");
		return;
	}
//	int filelenth = file.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！");
//		return;
//	}

	unsigned char chartemp[128];
	file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("文件的能量边界数据无效");
//		return;
//	}

	file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin );	

	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {				
			file.Read(chartemp,2);	
			if(!g_bChannelChangeSel) { 
				m_uchELimit[block][i][0] = chartemp[0];
				m_uchELimit[block][i][1] = chartemp[1];				
			}		
			else {			
				m_uchELimit[7-block][i][0] = chartemp[0];
				m_uchELimit[7-block][i][1] = chartemp[1];							
			}

		}
	}
	file.Close();


	unsigned char griddata[64][2];
	int nBlindex=m_Block_Sel.GetCurSel();
	if(nBlindex == 8) { memset(griddata,0,64*2); }
	else { 	memcpy(griddata,m_uchELimit[nBlindex],64*2); }

	CString strdd;
	for(int i=0;i<64;i++) {
		strdd.Format("%d",griddata[i][0]);
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(1);
		m_Grid.SetText(strdd);

		strdd.Format("%d",griddata[i][1]);
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(2);
		m_Grid.SetText(strdd);	
	}
	
}

void CBoardEnerge::OnSelchangeBlockSel() 
{
	unsigned char griddata[64][2];
	int nBlindex=m_Block_Sel.GetCurSel();
	if(nBlindex == 8) { memset(griddata,55,64*2); }
	else { 	memcpy(griddata,m_uchELimit[nBlindex],64*2); }

	CString strdd;
	for(int i=0;i<64;i++) {
		strdd.Format("%d",griddata[i][0]);
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(1);
		m_Grid.SetText(strdd);

		strdd.Format("%d",griddata[i][1]);
		m_Grid.SetRow(i+1);
		m_Grid.SetCol(2);
		m_Grid.SetText(strdd);	
	}
	
}

void CBoardEnerge::OnSend8BlockPara()
{
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();
	
	ResetEvent(g_hEvtSend);
	
	GetDlgItem(IDOKe)->EnableWindow(false);
	for(int block=0;block<8;block++) {
		for(int j=0;j<2;j++) {
			TxdData[0]= nBuindex + 0x80; 
			TxdData[1]=0xb0;
			TxdData[2]=0x49;
			TxdData[3]=0xc3;
			TxdData[4] = (unsigned char)m_Chip_Sel;
			
			TxdData[5]= (unsigned char)block;
			if(j==0) { TxdData[6]=4; };
			if(j==1) { TxdData[6]=5; };
			
			for(int num=0;num<64;num++) {				
				TxdData[num +7] = m_uchELimit[block][num][j];					
			}
			
			TxdData[71]=TxdData[0];
			for(int i=1;i<71;i++)
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
			
			WaitForSingleObject(g_hEvtSend, 300);
		}
		
	}
	
	GetDlgItem(IDOKe)->EnableWindow(true);

}

void CBoardEnerge::OnChannelChangeSel() 
{
	g_bChannelChangeSel = (!g_bChannelChangeSel);
	
}

void CBoardEnerge::OnSelchangeBucketSel() 
{
	int nBucklindex=m_Bucket_Sel.GetCurSel();
	if(0 == (nBucklindex%2)) {
		m_bChannelChangeSel.SetCheck(0);
		g_bChannelChangeSel = false;
	}
	else {
		m_bChannelChangeSel.SetCheck(1);
		g_bChannelChangeSel = true;
	}
	g_Bucketindex = nBucklindex;
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex

}
