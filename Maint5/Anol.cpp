// Anol.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "Anol.h"

#include "G_Fuction.h"
#include "MaintDLL.h"
#include "PetApi.h"
#include <Math.h>
#include "MaintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT  ReadProcAnol(LPVOID  lpData);
//HANDLE hReadHandleAnol;


CWinThread *g_pThread_Anol;


//extern bool UCRC(unsigned char*buff);
extern int g_Bucketindex;
extern int g_Blockindex;
extern int g_bChannelChangeSel;

extern HANDLE	g_hEvtSend;

extern HANDLE g_hEvtRec;
extern BOOL g_bEvtSend;
extern CRITICAL_SECTION  g_ClkRecSafe;

extern int g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

bool g_bRunThreadAnol;
int g_Anol_Chip_Sel = 2;
int g_Anol_IOsel = 1;

CString	g_Anol_Eth_strValue = "0";
CString	g_Anol_Td_strValue = "0";
CString	g_Anol_strPlus_B = "0";
CString	g_Anol_strPlus_C = "0";
CString	g_Anol_strPlus_D = "0";
CString	g_Anol_strOffset_E = "0";
CString	g_Anol_strOffset_X = "0";
CString	g_Anol_strOffset_Y = "0";
CString	g_Anol_strPlus_A = "0";
/////////////////////////////////////////////////////////////////////////////
// CAnol dialog


CAnol::CAnol(CWnd* pParent /*=NULL*/)
	: CDialog(CAnol::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnol)
	m_Eth_strValue = _T("");
	m_Td_strValue = _T("");
	m_strPlus_B = _T("");
	m_strPlus_C = _T("");
	m_strPlus_D = _T("");
	m_strOffset_E = _T("");
	m_strOffset_X = _T("");
	m_strOffset_Y = _T("");
	m_strPlus_A = _T("");
	//}}AFX_DATA_INIT

	bAckFrame = false;
	m_bLoaded8BlockPara = false;
	m_curblock = 0;



}


void CAnol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnol)
	DDX_Control(pDX, IDC_CHANNEL_CHANGE_SEL, m_bChannelChangeSel);
	DDX_Control(pDX, IDC_CHIP_EEPROM, m_Anol_Chip_Eeprom);
	DDX_Control(pDX, IDC_CHIP_ANOL, m_Anol_Chip_Anol);
	DDX_Control(pDX, IDC_CHIP_ALL, m_Anol_Chip_All);
	DDX_Control(pDX, IDC_IOSEL_IN, m_Anol_IOsel_In);
	DDX_Control(pDX, IDC_IOSEL_OUT, m_Anol_IOsel_Out);
	DDX_Control(pDX, IDC_BUCKET_SEL, m_Bucket_Sel);
	DDX_Control(pDX, IDC_BLOCK_SEL, m_Block_Sel);
	DDX_Text(pDX, IDC_ETH_VALUE, m_Eth_strValue);
	DDX_Text(pDX, IDC_TD_VALUE, m_Td_strValue);
	DDX_Text(pDX, IDC_PLUS_B, m_strPlus_B);
	DDX_Text(pDX, IDC_PLUS_C, m_strPlus_C);
	DDX_Text(pDX, IDC_PLUS_D, m_strPlus_D);
	DDX_Text(pDX, IDC_OFFSET_E, m_strOffset_E);
	DDX_Text(pDX, IDC_OFFSET_X, m_strOffset_X);
	DDX_Text(pDX, IDC_OFFSET_Y, m_strOffset_Y);
	DDX_Text(pDX, IDC_PLUS_A, m_strPlus_A);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnol, CDialog)
	//{{AFX_MSG_MAP(CAnol)
	ON_BN_CLICKED(IDC_CHIP_EEPROM, OnChipEeprom)
	ON_BN_CLICKED(IDC_CHIP_ANOL, OnChipAnol)
	ON_BN_CLICKED(IDC_CHIP_ALL, OnChipAll)
	ON_BN_CLICKED(IDC_IOSEL_OUT, OnIOsel_Out)
	ON_BN_CLICKED(IDC_IOSEL_IN, OnIOsel_In)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LOADFILE, OnLoadfile)
	ON_CBN_SELCHANGE(IDC_BLOCK_SEL, OnSelchangeBlockSel)
	ON_BN_CLICKED(IDC_SAVE_PARA, OnSavePara)
	ON_BN_CLICKED(IDC_CHANNEL_CHANGE_SEL, OnChannelChangeSel)
	ON_CBN_EDITCHANGE(IDC_BUCKET_SEL, OnEditchangeBucketSel)
	ON_CBN_SELCHANGE(IDC_BUCKET_SEL, OnSelchangeBucketSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnol message handlers
BOOL CAnol::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//DWORD dwThreadID;

//	bAckFrame = true;

	m_iSuccessACKNum = 0;

	switch (g_Anol_Chip_Sel) {
		case 1:	 m_Anol_Chip_Anol.SetCheck(1);
			break;
		case 2:	 m_Anol_Chip_Eeprom.SetCheck(1);	
			break;
		case 5:  m_Anol_Chip_All.SetCheck(1);
			break;
		default: break;
	}

	switch (g_Anol_IOsel) {
		case 0:	 m_Anol_IOsel_In.SetCheck(1);
				 bAckFrame = true;
			break;
		case 1:	 m_Anol_IOsel_Out.SetCheck(1);
				 bAckFrame = false;
			break;
		default: break;
	}
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	m_Eth_strValue = g_Anol_Eth_strValue; 
	m_Td_strValue = g_Anol_Td_strValue;
	m_strPlus_A = g_Anol_strPlus_A;
	m_strPlus_B = g_Anol_strPlus_B;
	m_strPlus_C = g_Anol_strPlus_C;
	m_strPlus_D = g_Anol_strPlus_D;
	m_strOffset_X = g_Anol_strOffset_X;
	m_strOffset_Y = g_Anol_strOffset_Y;	
	m_strOffset_E = g_Anol_strOffset_E;

	UpdateData(false);	
	memset(m_uchrAnolPara,0,8*8);

	RecCounter = 0;

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

	g_pThread_Anol = AfxBeginThread(ReadProcAnol,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);					

	g_pThread_Anol->m_bAutoDelete = FALSE;
	g_pThread_Anol->ResumeThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------//
void CAnol::OnOK() 
{
	// TODO: Add extra validation here

	unsigned char buf[128];
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();

	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	CString strtemp;

	RecCounter = 0;
	memset(recbuf,0,128);
	recbuf[2] = 4;
	m_iSuccessACKNum = 0;

	m_Chip_Sel = g_Anol_Chip_Sel;
	m_InOut_Sel = g_Anol_IOsel;

	if((nBuindex == -1)||(nBlindex == -1)) {
		AfxMessageBox("请选择Bucket或Block！",MB_ICONINFORMATION);
		return;
	}
	if((m_Chip_Sel < 1)||(m_Chip_Sel >5)) {
		AfxMessageBox("请选择 芯片 ！",MB_ICONINFORMATION);
		return;
	}
	
	UpdateData(true);
	UpdateData(false);
	m_iSuccessACKNum = 0;


	if((m_Chip_Sel == 1)&&(m_InOut_Sel == 1)) { 
		AfxMessageBox("模拟板不能进行读操作",MB_ICONINFORMATION);
		return;
	}

	if(m_Chip_Sel == 5) {  //选择全体
		AfxMessageBox("目前还不能进行全体操作",MB_ICONINFORMATION);
		return;
	}

	if((nBlindex == 8)&&(m_InOut_Sel != 1)) {
		m_iSuccessACKNum = 0;
		OnSend8BlockPara();		
		return;
	}
	m_bLoaded8BlockPara = false;
	m_bEnLoad8BlockPara = false;
	if((nBlindex == 8)&&(m_InOut_Sel == 1)) {
//		AfxMessageBox("通道选择超出范围了！\n程序不支持连续读8个Block");
		OnLoad8BlockPara();			
		return;
	}

	buf[0] = nBuindex + 0x80;
	buf[1] = 0xb0;
	buf[2] = 0x11;
	buf[3] = 0xc2;
	buf[4] = m_Chip_Sel + m_InOut_Sel ;	
	buf[5] = nBlindex;
	buf[6] = (unsigned char)atoi(m_strPlus_A);
	buf[7] = (unsigned char)atoi(m_strPlus_B);
	buf[8] = (unsigned char)atoi(m_strPlus_C);
	buf[9] = (unsigned char)atoi(m_strPlus_D);
	buf[10] = (unsigned char)atoi(m_strOffset_X);
	buf[11] = (unsigned char)atoi(m_strOffset_Y);
	buf[12] = (unsigned char)atoi(m_strOffset_E);
	buf[13] = (unsigned char)atoi(m_Eth_strValue);
	buf[14] = (unsigned char)atoi(m_Td_strValue);

	g_Anol_Eth_strValue = m_Eth_strValue; 
	g_Anol_Td_strValue =m_Td_strValue;
	g_Anol_strPlus_A = m_strPlus_A;
	g_Anol_strPlus_B = m_strPlus_B;
	g_Anol_strPlus_C = m_strPlus_C;
	g_Anol_strPlus_D = m_strPlus_D;
	g_Anol_strOffset_X  = m_strOffset_X;
	g_Anol_strOffset_Y = m_strOffset_Y;	
	g_Anol_strOffset_E = m_strOffset_E;
	buf[15] = buf[0];
	for(int i=1;i<15;i++) buf[15] ^= buf[i];
	buf[16] = 0x88;

	#ifdef _SELECT_232_PORT
		int nWR = WriteCommandMaint232(17,buf);
	#else
		//int nWR = WriteCommandMaintain(17,buf);
		int nWR = WriteCommandMaintainMod(17,buf); //akangquanakangquan
	#endif
	
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	
}

//-------------------------------------------------------------------//
UINT  ReadProcAnol(LPVOID  lpData)
{
	CFile recfile;
	int size;
	int nBlindex;
	unsigned char data[100];
	CString strtemp;
	CAnol *ph=(CAnol *)lpData ;	

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif	

	g_bRunThreadAnol = true;
	while (g_bRunThreadAnol) 
	{

	#ifdef _SELECT_232_PORT
		size = ReadResultMaint232(data);
	#else
		size = ReadResultMaintainMod(data);
	#endif		
		
		if(size == 0xffff) {
			AfxMessageBox("Read Error!");
			g_bRunThreadAnol = false;
		}

		if(size > 0) {
			for(int i=0;i<size;i++) 
				ph->recbuf[ph->RecCounter+i] = data[i];
			ph->RecCounter +=size;

			if(ph->bAckFrame) { //
				if(ph->RecCounter >= ph->recbuf[2]) {
					if( !UCRC(ph->recbuf) ) {
								ph->RecCounter = 0;		
								AfxMessageBox(" 接收数据 UCRC 错误!");							
					}
					else {					
						if((ph->RecCounter>=8)&&(ph->recbuf[5] == 0x55)) { //判断帧长
								ph->RecCounter = 0;
								//==============================
										nBlindex = ph->m_Block_Sel.GetCurSel();
										if(nBlindex == 8) {
											ph->m_iSuccessACKNum ++;													
											if(ph->m_iSuccessACKNum >= 8) {
												ph->m_iSuccessACKNum = 0;
												AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
											}
										}
										else {										
											AfxMessageBox("数据导入成功！",MB_ICONINFORMATION);
										}
								//=================================
						}
						if((ph->RecCounter>=8)&&(ph->recbuf[5] == 0x54)) { //判断帧长
								ph->RecCounter = 0;
								AfxMessageBox("数据导入出错了！！！",MB_ICONINFORMATION);
						}
					}
					ph->recbuf[2] = 4;
				}

			}
			else if(ph->RecCounter >= ph->recbuf[2]) { //判断帧长
				ph->RecCounter = 0;
				if( !UCRC(ph->recbuf) ) {
							ph->RecCounter = 0;							
							AfxMessageBox(" 接收数据 UCRC 错误!");							
				}				
				else {	
					if(ph->m_bEnLoad8BlockPara) {
						memcpy(ph->m_uchrAnolPara[ph->m_curblock],&(ph->recbuf[5]),8);
						ph->m_bLoaded8BlockPara = false;
						if((++ph->m_curblock)>=8) {
							ph->m_bLoaded8BlockPara = true;
							ph->m_curblock = 0;							
						}
					}
					else {
						strtemp.Format(" %d",ph->recbuf[5]);
						ph->SetDlgItemText(IDC_PLUS_A,strtemp);
						strtemp.Format(" %d",ph->recbuf[6]);
						ph->SetDlgItemText(IDC_PLUS_B,strtemp);
						strtemp.Format(" %d",ph->recbuf[7]);
						ph->SetDlgItemText(IDC_PLUS_C,strtemp);
						strtemp.Format(" %d",ph->recbuf[8]);
						ph->SetDlgItemText(IDC_PLUS_D,strtemp);
						strtemp.Format(" %d",ph->recbuf[9]);
						ph->SetDlgItemText(IDC_OFFSET_X,strtemp);
						strtemp.Format(" %d",ph->recbuf[10]);
						ph->SetDlgItemText(IDC_OFFSET_Y,strtemp);
						strtemp.Format(" %d",ph->recbuf[11]);
						ph->SetDlgItemText(IDC_OFFSET_E,strtemp);
						strtemp.Format(" %d",ph->recbuf[12]);
						ph->SetDlgItemText(IDC_ETH_VALUE,strtemp);
						strtemp.Format(" %d",ph->recbuf[13]);
						ph->SetDlgItemText(IDC_TD_VALUE,strtemp);
					}
				}
				ph->recbuf[2] = 4;

			}
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


void CAnol::OnChipEeprom() 
{
	m_Chip_Sel = 2;
	g_Anol_Chip_Sel = 2;
	
}

void CAnol::OnChipAnol() 
{
	m_Chip_Sel = 1;
	g_Anol_Chip_Sel = 1;
	
}

void CAnol::OnChipAll() 
{
	m_Chip_Sel = 5; 
	g_Anol_Chip_Sel = 5;
	
}

void CAnol::OnIOsel_Out() 
{
	m_InOut_Sel = 1;
	g_Anol_IOsel = 1;
	bAckFrame = false;
	
}

void CAnol::OnIOsel_In() 
{
		m_InOut_Sel = 0;
		g_Anol_IOsel = 0;
		bAckFrame = true;
}

void CAnol::OnCancel() 
{
	UpdateData(true);
	g_Anol_Eth_strValue = m_Eth_strValue; 
	g_Anol_Td_strValue =m_Td_strValue;
	g_Anol_strPlus_B = m_strPlus_B;
	g_Anol_strPlus_C = m_strPlus_C;
	g_Anol_strPlus_D = m_strPlus_C;
	g_Anol_strOffset_E = m_strOffset_E;
	g_Anol_strOffset_X  = m_strOffset_X;
	g_Anol_strOffset_Y = m_strOffset_Y;
	g_Anol_strPlus_A = m_strPlus_A;

	
	if(g_bRunThreadAnol) {
		g_bRunThreadAnol = false;
		Sleep(300);
		delete g_pThread_Anol;
	}
	CDialog::OnCancel();
}

void CAnol::OnClose() 
{

	UpdateData(true);
	g_Anol_Eth_strValue = m_Eth_strValue; 
	g_Anol_Td_strValue =m_Td_strValue;
	g_Anol_strPlus_B = m_strPlus_B;
	g_Anol_strPlus_C = m_strPlus_C;
	g_Anol_strPlus_D = m_strPlus_C;
	g_Anol_strOffset_E = m_strOffset_E;
	g_Anol_strOffset_X  = m_strOffset_X;
	g_Anol_strOffset_Y = m_strOffset_Y;
	g_Anol_strPlus_A = m_strPlus_A;

	if(g_bRunThreadAnol) {
		g_bRunThreadAnol = false;
		Sleep(300);
		delete g_pThread_Anol;
	}

	CDialog::OnClose();
}

void CAnol::OnDestroy() 
{

	if(g_bRunThreadAnol) {
		g_bRunThreadAnol = false;
		Sleep(300);
		delete g_pThread_Anol;
	}


//		CDialog::OnDestroy();
	CDialog::OnOK();
}

void CAnol::OnLoadfile() 
{
/*	int nBucklindex=m_Bucket_Sel.GetCurSel();
	CString strtext;	
	int iEnChannelChangeSel;
	if(g_bChannelChangeSel) {
		strtext.Format("当前Bucket号为 %d ， 确定 “要” 进行通道转置？",nBucklindex);
		iEnChannelChangeSel = AfxMessageBox(strtext,MB_YESNO);
		//if(IDYES !=AfxMessageBox(strtext,MB_YESNO) ) return;
	}
	else {
		strtext.Format("当前Bucket号为 %d ， 确定 “不” 进行通道转置？",nBucklindex);
		iEnChannelChangeSel = AfxMessageBox(strtext,MB_YESNO);
	//	if(IDYES !=AfxMessageBox(strtext,MB_YESNO) ) return;
	}
*/

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	dlg.m_ofn.lpstrFilter = ("模拟参数导入.dat");//
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
	file.Seek(FILEHEAD_ANOLPARA_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("文件的模拟参数无效！");
//		return;
//	}

	file.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin );
	
	for(int block=0;block<8;block++) {
		file.Read(chartemp,8);
		if(!g_bChannelChangeSel) { memcpy(m_uchrAnolPara[block],chartemp,8); }		
		else {			
			memcpy(m_uchrAnolPara[7-block],chartemp,8);			
		}
	}
	file.Close();

	
	unsigned char Anoldata[8];
	int nBlindex=m_Block_Sel.GetCurSel();
	if(nBlindex == 8) { memset(Anoldata,55,8); }
	else { 	memcpy(Anoldata,m_uchrAnolPara[nBlindex],8); }
	char charttt[10];
	sprintf(charttt,"%d",Anoldata[0]);
	m_strPlus_A = charttt;
	sprintf(charttt,"%d",Anoldata[1]);
	m_strPlus_B = charttt;
	sprintf(charttt,"%d",Anoldata[2]);
	m_strPlus_C = charttt;
	sprintf(charttt,"%d",Anoldata[3]);
	m_strPlus_D = charttt;
	sprintf(charttt,"%d",Anoldata[4]);
	m_strOffset_X = charttt;
	sprintf(charttt,"%d",Anoldata[5]);
	m_strOffset_Y = charttt;
	sprintf(charttt,"%d",Anoldata[6]);
	m_strOffset_E = charttt;
	sprintf(charttt,"%d",Anoldata[7]);
	m_Eth_strValue = charttt; 
	m_Td_strValue = "0";

	UpdateData(false);

/*	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {				
			file.Read(chartemp,2);			
			m_uchELimit[block][i][0] = chartemp[0];
			m_uchELimit[block][i][1] = chartemp[1];				
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

*/
	
}

void CAnol::OnSelchangeBlockSel() 
{
	unsigned char Anoldata[8];
	int nBlindex=m_Block_Sel.GetCurSel();
	if(nBlindex == 8) { memset(Anoldata,55,8); }
	else { 	memcpy(Anoldata,m_uchrAnolPara[nBlindex],8); }
	char charttt[10];
	sprintf(charttt,"%d",Anoldata[0]);
	m_strPlus_A = charttt;
	sprintf(charttt,"%d",Anoldata[1]);
	m_strPlus_B = charttt;
	sprintf(charttt,"%d",Anoldata[2]);
	m_strPlus_C = charttt;
	sprintf(charttt,"%d",Anoldata[3]);
	m_strPlus_D = charttt;
	sprintf(charttt,"%d",Anoldata[4]);
	m_strOffset_X = charttt;
	sprintf(charttt,"%d",Anoldata[5]);
	m_strOffset_Y = charttt;
	sprintf(charttt,"%d",Anoldata[6]);
	m_strOffset_E = charttt;
	sprintf(charttt,"%d",Anoldata[7]);
	m_Eth_strValue = charttt; 
	m_Td_strValue = "0";

	UpdateData(false);
	
}

void CAnol::OnSend8BlockPara()
{
	unsigned char buf[128];
	int nBuindex=m_Bucket_Sel.GetCurSel();
	int nBlindex=m_Block_Sel.GetCurSel();


	ResetEvent(g_hEvtSend);

	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	g_Blockindex = m_Block_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//g_Blockindex
	m_Block_Sel.SetCurSel(g_Blockindex);

	m_Chip_Sel = g_Anol_Chip_Sel;
	m_InOut_Sel = g_Anol_IOsel;

	GetDlgItem(IDOK)->EnableWindow(false);

	for(int block=0;block<8;block++) {
			buf[0] = nBuindex + 0x80;
			buf[1] = 0xb0;
			buf[2] = 0x11;
			buf[3] = 0xc2;
			buf[4] = m_Chip_Sel + m_InOut_Sel ;	
			buf[5] = (unsigned char)block;

			buf[6] = m_uchrAnolPara[block][0];
			buf[7] = m_uchrAnolPara[block][1];
			buf[8] = m_uchrAnolPara[block][2];
			buf[9] = m_uchrAnolPara[block][3];
			buf[10] = m_uchrAnolPara[block][4];
			buf[11] = m_uchrAnolPara[block][5];
			buf[12] = m_uchrAnolPara[block][6];
			buf[13] = m_uchrAnolPara[block][7];
			buf[14] = 0;

			buf[15] = buf[0];
			for(int i=1;i<15;i++) buf[15] ^= buf[i];

			buf[16] = 0x88;

			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(17,buf);
			#else
				int nWR = WriteCommandMaintainMod(17,buf);
			#endif
			
			if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}
			
			WaitForSingleObject(g_hEvtSend, 400);
	}

	GetDlgItem(IDOK)->EnableWindow(true);

}

void CAnol::OnSavePara() 
{
	int nBlindex=m_Block_Sel.GetCurSel();
	int bBucketlindex=m_Bucket_Sel.GetCurSel();

	if(nBlindex == 8) { 
		if(!m_bLoaded8BlockPara) {
			AfxMessageBox("没有上传 8个Block参数呢");
			return;
		}
	}

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");//TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("模拟参数导入.dat") ;
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;

	CFile tempfile;
	if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("不能进行编辑操作 (没有找到此文件) ");
			return;
	}
	tempfile.Close();

	CFile savefile;
	if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
	}

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！保存不成功！");
//		return;
//	}


	unsigned char filehead[128];
	savefile.SeekToBegin();
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ANOLPARA_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	if(nBlindex == 8) { 		
		for(int block=0;block<8;block++) {
			if(0 == (bBucketlindex%2)) { //偶数不变
				savefile.Seek(PARA_ANOLPARA_DATA_ADDR + block*BLOCK_ANOLPARA_DATA_SIZE,CFile::begin);
			}
			else savefile.Seek(PARA_ANOLPARA_DATA_ADDR + (7-block)*BLOCK_ANOLPARA_DATA_SIZE,CFile::begin);
		
			savefile.Write(m_uchrAnolPara[block],8);
		}
		savefile.Close();
		return;
	}	

	if(0 == (bBucketlindex%2)) { //偶数不变
		savefile.Seek(PARA_ANOLPARA_DATA_ADDR + nBlindex*BLOCK_ANOLPARA_DATA_SIZE,CFile::begin);
	}
	else savefile.Seek(PARA_ANOLPARA_DATA_ADDR + (7-nBlindex)*BLOCK_ANOLPARA_DATA_SIZE,CFile::begin);

	unsigned char buf[10];
	UpdateData(true);
	UpdateData(false);

	buf[0] = (unsigned char)atoi(m_strPlus_A);
	buf[1] = (unsigned char)atoi(m_strPlus_B);
	buf[2] = (unsigned char)atoi(m_strPlus_C);
	buf[3] = (unsigned char)atoi(m_strPlus_D);
	buf[4] = (unsigned char)atoi(m_strOffset_X);
	buf[5] = (unsigned char)atoi(m_strOffset_Y);
	buf[6] = (unsigned char)atoi(m_strOffset_E);
	buf[7] = (unsigned char)atoi(m_Eth_strValue);
//	buf[8] = (unsigned char)atoi(m_Td_strValue);

	savefile.Write(buf,8);

	savefile.Close();	
}

void CAnol::OnChannelChangeSel() 
{
	g_bChannelChangeSel = (!g_bChannelChangeSel);
}

void CAnol::OnEditchangeBucketSel() 
{

}

void CAnol::OnSelchangeBucketSel() 
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

void CAnol::OnLoad8BlockPara()
{
	unsigned char buf[128];
	int nBuindex=m_Bucket_Sel.GetCurSel();

	ResetEvent(g_hEvtSend);

	g_Bucketindex = m_Bucket_Sel.GetCurSel();
	m_Bucket_Sel.SetCurSel(g_Bucketindex);//

	m_Chip_Sel = g_Anol_Chip_Sel;
	m_InOut_Sel = g_Anol_IOsel;

	GetDlgItem(IDOK)->EnableWindow(false);
	m_bLoaded8BlockPara = false;
	m_bEnLoad8BlockPara = true;
	m_curblock = 0;

	for(int block=0;block<8;block++) {
			buf[0] = nBuindex + 0x80;
			buf[1] = 0xb0;
			buf[2] = 0x11;
			buf[3] = 0xc2;
			buf[4] = m_Chip_Sel + m_InOut_Sel ;	
			buf[5] = (unsigned char)block;

			buf[6] = m_uchrAnolPara[block][0];
			buf[7] = m_uchrAnolPara[block][1];
			buf[8] = m_uchrAnolPara[block][2];
			buf[9] = m_uchrAnolPara[block][3];
			buf[10] = m_uchrAnolPara[block][4];
			buf[11] = m_uchrAnolPara[block][5];
			buf[12] = m_uchrAnolPara[block][6];
			buf[13] = m_uchrAnolPara[block][7];
			buf[14] = 0;
			buf[15] = buf[0];
			for(int i=1;i<15;i++) buf[15] ^= buf[i];

			buf[16] = 0x88;

			#ifdef _SELECT_232_PORT
				int nWR = WriteCommandMaint232(17,buf);
			#else
				int nWR = WriteCommandMaintainMod(17,buf);
			#endif
			
			if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}
			
			WaitForSingleObject(g_hEvtSend, 400);
	}

	GetDlgItem(IDOK)->EnableWindow(true);

	Sleep(400);
	if(m_bLoaded8BlockPara) {
		AfxMessageBox("上载 8 Block 数据成功\n 请保存数据");
	}

}
