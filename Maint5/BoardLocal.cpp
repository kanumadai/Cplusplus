// BoardLocal.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "BoardLocal.h"
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

UINT  ReadProcBLocal(LPVOID  lpData);
HANDLE hReadHandleBLocal;
CWinThread *g_pThread_BoardLocal;

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
bool g_bRunThreadBLocal;
/////////////////////////////////////////////////////////////////////////////
// CBoardLocal dialog


CBoardLocal::CBoardLocal(CWnd* pParent /*=NULL*/)
	: CDialog(CBoardLocal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoardLocal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(TxdData,0,72*4);

	memset(m_uchPLimit,0,8*8*8*4);

}


void CBoardLocal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoardLocal)
	DDX_Control(pDX, IDC_CHANNEL_CHANGE_SEL, m_bChannelChangeSel);
	DDX_Control(pDX, IDC_BOARDLOCAL_EDIT, m_BoardLocal_Edit);
	DDX_Control(pDX, IDC_BLOCK_SEL, m_Block_Sel);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_Grid);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSComm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoardLocal, CDialog)
	//{{AFX_MSG_MAP(CBoardLocal)
	ON_EN_KILLFOCUS(IDC_BOARDLOCAL_EDIT, OnKillfocusBoardlocalEdit)
	ON_BN_CLICKED(IDC_EEPROM_RADIO, OnEepromRadio)
	ON_BN_CLICKED(IDC_FPGA_RADIO, OnFpgaRadio)
	ON_BN_CLICKED(IDC_INPUT_RADIO, OnInputRadio)
	ON_BN_CLICKED(IDC_OUTPUT_RADIO, OnOutputRadio)
	ON_BN_CLICKED(IDOKe, OnOKe)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BLOCAL_LOADDATA, OnLoaddata)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_BLOCK_SEL, OnSelchangeBlockSel)
	ON_BN_CLICKED(IDC_CHANNEL_CHANGE_SEL, OnChannelChangeSel)
	ON_CBN_SELCHANGE(IDC_BUCKET_SEL, OnSelchangeBucketSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoardLocal message handlers

BEGIN_EVENTSINK_MAP(CBoardLocal, CDialog)
    //{{AFX_EVENTSINK_MAP(CBoardLocal)
	ON_EVENT(CBoardLocal, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CBoardLocal, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBoardLocal::OnDblClickMsflexgrid1() 
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
	m_BoardLocal_Edit.SetWindowPos(&wndTop,
		                           fpCellLeft+14,
                                   fpCellTop+100,
							       fpCellWidth,
							       fpCellHeight+3,
							       NULL);
 
    m_BoardLocal_Edit.SetFocus();
    m_BoardLocal_Edit.ShowWindow(SW_SHOW);// 
    SetDlgItemText(IDC_BOARDLOCAL_EDIT,"");
    CString fpData=m_Grid.GetText();
    m_BoardLocal_Edit.ReplaceSel(fpData);
	
}

void CBoardLocal::OnKillfocusBoardlocalEdit() 
{
	// TODO: Add your control notification handler code here
	CString fpInputData;
	GetDlgItemText(IDC_BOARDLOCAL_EDIT,fpInputData);
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
    m_BoardLocal_Edit.ShowWindow(SW_HIDE);
	
}

void CBoardLocal::OnEepromRadio() 
{
	// TODO: Add your control notification handler code here
	m_Chip_Sel = 3;	
}

void CBoardLocal::OnFpgaRadio() 
{
	// TODO: Add your control notification handler code here
	m_Chip_Sel = 1;  
}

void CBoardLocal::OnInputRadio() 
{
	// TODO: Add your control notification handler code here
	m_InOut_Sel = 1;
	
}

void CBoardLocal::OnOutputRadio() 
{
	// TODO: Add your control notification handler code here
	m_InOut_Sel = 2;
	
}

BOOL CBoardLocal::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	m_iSuccessACKNum = 0;


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
	m_Grid.SetRows(65);

	m_Grid.SetCol(0);
	for(int j=1;j<65;j++)
	{
	   m_Grid.SetRow(j);
	   m_strGrid.Format("%d",j-1);
	   m_Grid.SetText(m_strGrid); 
	}
    m_Grid.SetRow(0);
    m_Grid.SetColWidth(0,455);
	m_Grid.SetRow(0);
	for(int i=1;i<5;i++)
	{ 
	   m_Grid.SetColWidth(i,922);
	}
	m_Grid.SetCol(1);
    m_Grid.SetText(CString("   X1"));
	m_Grid.SetCol(2);
    m_Grid.SetText(CString("   X2"));
	m_Grid.SetCol(3);
    m_Grid.SetText(CString("   Y1"));
	m_Grid.SetCol(4);
    m_Grid.SetText(CString("   Y2")); 

	for(j=0;j<4;j++) { 
		for(int i=0;i<64;i++)	{                   
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(j+1);
			m_Grid.SetText("00");
		}
	}

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
	
	g_pThread_BoardLocal = AfxBeginThread(ReadProcBLocal,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	g_pThread_BoardLocal->m_bAutoDelete = FALSE;
	g_pThread_BoardLocal->ResumeThread();


//	m_workcounter = 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBoardLocal::OnCommMscomm() 
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

			for(int i=0;i<k;i++) {
				strRecData[iRecNum+i] = str1[i];
			}

			iRecNum += k;
			if(bAckFrame) {
				if((iRecNum >= 8) && (strRecData[5] == 0x55)){	
					iRecNum = 0;
					AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
				}					
			}
			else if(iRecNum >= 71*4)	{
				iRecNum = 0;
				for(int j=0;j<4;j++) { 
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

void CBoardLocal::OnOKe() 
{
	unsigned char fpTempdata[64][4];
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();
	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	if((nBuindex == -1)||(nBlindex == -1)) {
		AfxMessageBox("请选择Bucket或Block！",MB_ICONINFORMATION);
		return;
	}

	if((m_Chip_Sel < 1)||(m_Chip_Sel >3)) {
		AfxMessageBox("请选择 芯片 ！",MB_ICONINFORMATION);
		return;
	}
	
	iRecNum = 0;
	memset(strRecData,0,80*4);
	strRecData[2] = 4;
	m_iSuccessACKNum = 0;	

    if(m_InOut_Sel == 1) {	//导入
		for(unsigned char j=1;j<2;j++)	{ // 74*4 
				if(nBlindex == 8) {
						m_iSuccessACKNum = 0;
						bAckFrame = true;   
						OnSend8BlockPara();
						return;
				}
				m_sendnum = 0;		
				TxdData[0]= nBuindex + 0x80; 
				TxdData[1]=0xb0;
				TxdData[2]=0x49;
				TxdData[3]=0xc3;
				TxdData[4] = (unsigned char)m_Chip_Sel;

				TxdData[5]= nBlindex;
				TxdData[6]=j-1;
				for(int i=1,t=7;i<65,t<71;i++,t++)	{
					m_Grid.SetRow(i);
					m_Grid.SetCol(j);
					fpTempdata[i-1][j-1]=atoi(m_Grid.GetText());
					TxdData[t]=fpTempdata[i-1][j-1];//
				}	
				
				TxdData[71]=TxdData[0];
				for( i=1;i<71;i++)
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
		for(int j=0;j<4;j++) { 
			for(int i=0;i<64;i++)	{                   
				m_Grid.SetRow(i+1);
				m_Grid.SetCol(j+1);
				m_Grid.SetText("00");
			}
		}

		m_sendnum = 0;
		for(j=0;j<1;j++)   // j<4
		 {		  
			TxdData[0]= nBuindex + 0x80; 
            TxdData[1]=0xb0;
            TxdData[2]=0x09;
            TxdData[3]=0xc3;
			TxdData[4] = (unsigned char)m_Chip_Sel +1;

			TxdData[5]= nBlindex;
			TxdData[6]=m_sendnum;
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

void CBoardLocal::SendCommand(unsigned char *CommandStr, int SendNum)
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


UINT  ReadProcBLocal(LPVOID  lpData)
{
	int size;
	int nBuindex;
	int nBlindex;
	int workcounter = 0;
	unsigned char fpTempdata[64][4];
	unsigned char TxdData[100];

	unsigned char data[100];
	CString m_strGrid;
	CBoardLocal *ph=(CBoardLocal *)lpData ;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

	ph->strRecData[2] = 4;
	g_bRunThreadBLocal = true;
	ph->iRecNum = 0;
	while (g_bRunThreadBLocal)
	{
	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif

		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadBLocal = false;
		}
		if(size > 0) {			
			for(int i=0;i<size;i++) 
				ph->strRecData[ph->iRecNum + workcounter+i] = data[i];

			workcounter += size;
			if(ph->bAckFrame) { //
				if(workcounter>=ph->strRecData[2]) {
					if( !UCRC(ph->strRecData) ) {
							workcounter = 0;
							ph->iRecNum = 0;
							AfxMessageBox(" 接收数据 UCRC 错误!");							
					}
					else {
						if((ph->strRecData[5] == 0x55)) { //判断帧长
							workcounter = 0;
							ph->iRecNum = 0;
							//-----------------------
							nBlindex = ph->m_Block_Sel.GetCurSel();
							if(nBlindex == 8) {ph->m_sendnum = 10;}

							ph->m_sendnum ++;
							if(ph->m_sendnum < 4)  {
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
								//TxdData[6]=j-1;
								TxdData[6] = ph->m_sendnum;
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
							else {
									//=========================================	
									ph->iRecNum = 0;
									workcounter = 0;
									ph->m_sendnum = 0;
									nBlindex = ph->m_Block_Sel.GetCurSel();
									if(nBlindex == 8) {										
										ph->m_iSuccessACKNum ++;
										SetEvent(g_hEvtSend);										
										if(ph->m_iSuccessACKNum >= 8*4) {
											ph->m_iSuccessACKNum = 0;
											AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
										}
										
									}
									else {										
										AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
									}
									//=========================================	
							}	
						}
						if((ph->strRecData[5] == 0x54)) { //判断帧长
								ph->iRecNum = 0;
								workcounter = 0;
								ph->m_sendnum = 0;
								AfxMessageBox("数据导入出错了！！！",MB_ICONINFORMATION);
						}

					} //end ucrc
					workcounter = 0;
					ph->iRecNum = 0;
					ph->strRecData[2] = 4;
				}//end workcounter

			}
			//else if(ph->iRecNum >= 71*4) {
			else if(workcounter >= 71) {
				workcounter = 0;
				ph->iRecNum +=71;
				if(ph->iRecNum >= 71*4) {
					ph->iRecNum = 0;
					ph->m_sendnum = 0;
					if( !UCRC(ph->strRecData) ) {
							AfxMessageBox(" 接收数据 UCRC 错误!");							
					}
					else {
						for(int j=0;j<4;j++) { 
							for(int i=0,t=71*j+5;i<64;i++,t++)	{                   
								ph->m_Grid.SetRow(i+1);
								ph->m_Grid.SetCol(j+1);
								ph->rectempl[i][j]=ph->strRecData[t]; 
 								m_strGrid.Format("%d",ph->rectempl[i][j]);
								ph->m_Grid.SetText(m_strGrid);
							}
						} //end first for()	
					} //end ucrc
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
					TxdData[6]=ph->m_sendnum;
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
		//else Sleep(10);
	}
#ifdef _SELECT_232_PORT
	CloseShmMaint232();
#else
	CloseShmMaintain();
#endif
	return 0;
}

void CBoardLocal::OnCancel() 
{
	if(g_bRunThreadBLocal) {
		g_bRunThreadBLocal = false;
		Sleep(300);	
		delete g_pThread_BoardLocal;
	}
	CDialog::OnCancel();
}

void CBoardLocal::OnClose() 
{
	if(g_bRunThreadBLocal) {
		g_bRunThreadBLocal = false;
		Sleep(300);	
		delete g_pThread_BoardLocal;
	}
	CDialog::OnClose();
}

void CBoardLocal::OnLoaddata() 
{
	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("位置边界导入.dat");//
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
	file.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("文件的位置边界数据无效");
//		return;
//	}

	file.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );	

	for(int block=0;block<8;block++) {
		for(int i=0;i<8;i++) {
				for(int j=0;j<8;j++) {
					file.Read(chartemp,4);	
					if(!g_bChannelChangeSel) {
						m_uchPLimit[block][i][j][0] = chartemp[0];
						m_uchPLimit[block][i][j][1] = chartemp[1];
						m_uchPLimit[block][i][j][2] = chartemp[2];
						m_uchPLimit[block][i][j][3] = chartemp[3];
					}
					else {
						m_uchPLimit[7-block][i][j][0] = chartemp[0];
						m_uchPLimit[7-block][i][j][1] = chartemp[1];
						m_uchPLimit[7-block][i][j][2] = chartemp[2];
						m_uchPLimit[7-block][i][j][3] = chartemp[3];
					}
				}
		}
	}

	file.Close();

	unsigned char griddata[8*8*4];
	int nBlindex=m_Block_Sel.GetCurSel();
	if(nBlindex == 8) { memset(griddata,0,8*8*4); }
	else { 	memcpy(griddata,m_uchPLimit[nBlindex],8*8*4); }

	CString  strgrid;
	int t=0;
	for(int i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(1);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=2;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(2);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=1;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(3);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=3;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(4);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}

}

void CBoardLocal::OnDestroy() 
{

	if(g_bRunThreadBLocal) {
		g_bRunThreadBLocal = false;
		Sleep(300);	
		delete g_pThread_BoardLocal;
	}
	//	CDialog::OnDestroy();
	CDialog::OnOK();
}

void CBoardLocal::OnSelchangeBlockSel() 
{
	unsigned char griddata[8*8*4] = { 0 };
	int nBlindex=m_Block_Sel.GetCurSel();

	memset(griddata,0,8*8*4);

	if(nBlindex == 8) { memset(griddata,55,8*8*4); }
	else memcpy(griddata,m_uchPLimit[nBlindex],8*8*4);

	CString  strgrid;
	int t=0;
	for(int i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(1);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=2;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(2);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=1;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(3);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}
	t=3;
	for(i=0;i<64;i++)	{		
			m_Grid.SetRow(i+1);
			m_Grid.SetCol(4);			 
 			strgrid.Format("%d",griddata[t]);
			m_Grid.SetText(strgrid);
			t +=4;
	}



	
}

void CBoardLocal::OnSend8BlockPara()
{
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();
	
	ResetEvent(g_hEvtSend);
	
	GetDlgItem(IDOKe)->EnableWindow(false);
	
	for(int block=0;block<8;block++) {
		for(int j=0;j<4;j++) {
			TxdData[0]= nBuindex + 0x80; 
			TxdData[1]=0xb0;
			TxdData[2]=0x49;
			TxdData[3]=0xc3;
			TxdData[4] = (unsigned char)m_Chip_Sel;
			
			TxdData[5]= (unsigned char)block;
			if(j==0) { TxdData[6]=0; };
			if(j==1) { TxdData[6]=2; };
			if(j==2) { TxdData[6]=1; };
			if(j==3) { TxdData[6]=3; };
			for(int y=0;y<8;y++) {
				for(int x=0;x<8;x++) {
					TxdData[y*8 + x +7] = m_uchPLimit[block][y][x][j];
				}
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

void CBoardLocal::OnChannelChangeSel() 
{
	g_bChannelChangeSel = (!g_bChannelChangeSel);	
}

void CBoardLocal::OnSelchangeBucketSel() 
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
