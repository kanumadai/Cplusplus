// HardWarePara.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "HardWarePara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HARDWARE_PARE g_HardWare_Pare;  // 硬件参数结构

extern CWinThread* g_pThread_Monitor; //监控线程指针
extern CWinThread* g_pThread_Send;		//发送线程指针
extern bool g_bRunMonitorThread;
extern bool g_bRunSendThread;

extern int	g_RecPackageLenth; //确定每个接收数据包的 长度
extern UINT		g_ThreadCMD;
extern BOOL		g_bExec;		//正在进行通信标志
extern BOOL     g_bComOpen;
extern HANDLE		g_hEvtSend;
extern HANDLE		g_hEvtFinishRec;     //完成接收句柄《数据收发同步用》 
extern HANDLE		g_hExitSendThread;
extern HANDLE		g_hExitMonitThread;

/////////////////////////////////////////////////////////////////////////////
// CHardWarePara dialog


CHardWarePara::CHardWarePara(CWnd* pParent /*=NULL*/)
	: CDialog(CHardWarePara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHardWarePara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//CHardWarePara::CHardWarePara(CView *pView)
//{
//	m_pView = pView;	
//}


void CHardWarePara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardWarePara)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHardWarePara, CDialog)
	//{{AFX_MSG_MAP(CHardWarePara)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardWarePara message handlers

BOOL CHardWarePara::OnInitDialog() 
{
	CDialog::OnInitDialog();

	OnUpdataShow();
		
	return TRUE;  

}

void CHardWarePara::OnOK() 
{
	CComboBox* pComboxTimeMode = (CComboBox *)GetDlgItem(IDC_CHECK_TIME_MODE);
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHECK_CHANNEL_WIDTH);
	CComboBox* pComboxDT = (CComboBox *)GetDlgItem(IDC_COMB_DT);
	CComboBox* pComboxReady = (CComboBox *)GetDlgItem(IDC_COMB_READY);
	CComboBox* pComboxAnswer = (CComboBox *)GetDlgItem(IDC_COMB_ANWSER);
	CComboBox* pComboxEnadc = (CComboBox *)GetDlgItem(IDC_COMB_ENADC);
	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);
	
	CButton* pIORead = (CButton *)GetDlgItem(IDC_IO_READ);
	CButton* pIOWrite = (CButton *)GetDlgItem(IDC_IO_WRITE);

	UpdateData(TRUE);

	CString strtext;
	int icursel;

	icursel = pComboxTimeMode->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bSampleModeRealTime = true;
	}
	if (icursel == 1) {
		g_HardWare_Pare.bSampleModeRealTime = false;
	}

	icursel = pComboxChannelWidth->GetCurSel();
	switch(icursel) {
	case 0:
		g_HardWare_Pare.iChannelWidth = 1024;
		break;
	case 1:
		g_HardWare_Pare.iChannelWidth = 2048;
		break;
	case 2:
		g_HardWare_Pare.iChannelWidth = 4096;
		break;
	case 3:
		g_HardWare_Pare.iChannelWidth = 8192;
		break;
	default:
		break;
	}
	int idata;
	GetDlgItem(IDC_SET_TIME_VALUE)->GetWindowText(strtext);
	idata = atol(strtext);
    if (idata > 0) {
		g_HardWare_Pare.iSampleTimeValue = idata;
    } 
	else AfxMessageBox("采样时间输入错误，请重新输入");

	icursel = pComboxDT->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bDT_Pulse = true;
	}
	else g_HardWare_Pare.bDT_Pulse = false;

	icursel = pComboxReady->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bREADY_Pulse = true;
	}
	else g_HardWare_Pare.bREADY_Pulse = false;

	icursel = pComboxAnswer->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bANSER_Pulse = true;
	}
	else g_HardWare_Pare.bANSER_Pulse = false;

	icursel = pComboxEnadc->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bENADC_Pulse = true;
	}
	else g_HardWare_Pare.bENADC_Pulse = false;

	float fdata;
	GetDlgItem(IDC_ENGER_KEDU_SLOPE)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleSlope = fdata;

	GetDlgItem(IDC_ENGER_KEDU_OFFSET)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleOffset = fdata;

	icursel = pComboxEngeUnit->GetCurSel();
	switch(icursel) {
	case 0:
		g_HardWare_Pare.iEngerScaleUnit = 0;
		break;
	case 1:
		g_HardWare_Pare.iEngerScaleUnit = 1;
		break;
	case 2:
		g_HardWare_Pare.iEngerScaleUnit = 2;
		break;
	case 3:
		g_HardWare_Pare.iEngerScaleUnit = 3;
		break;
	default:
		break;
	}

	icursel = pIORead->GetCheck();
	if (icursel == 1) {
		g_HardWare_Pare.bReadorWriteOperate = true;
	}
	icursel = pIOWrite->GetCheck();
	if (icursel == 1) {
		g_HardWare_Pare.bReadorWriteOperate = false;
	}

	//以上为读取用户输入信息，并更新到数据缓冲区；


	//读写数据操作
	if(!g_bComOpen) {
		AfxMessageBox("串口没有打开 \n请先打开串口！",MB_ICONINFORMATION);
		return;
	}
	if(g_bExec){
		AfxMessageBox("正在等待上一条命令的应答，请等一会儿重试",MB_ICONINFORMATION);
		return;
	}	

	if (g_HardWare_Pare.bReadorWriteOperate == true) { //读操作
		g_ThreadCMD = CTL_HARDWARE_READ;		
	}
	else { // 写入操作
		g_ThreadCMD = CTL_HARDWARE_WRITE;
	}
	SetEvent(g_hEvtSend);


}

void CHardWarePara::OnUpdataShow()
{
	CComboBox* pComboxTimeMode = (CComboBox *)GetDlgItem(IDC_CHECK_TIME_MODE);
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHECK_CHANNEL_WIDTH);
	CComboBox* pComboxDT = (CComboBox *)GetDlgItem(IDC_COMB_DT);
	CComboBox* pComboxReady = (CComboBox *)GetDlgItem(IDC_COMB_READY);
	CComboBox* pComboxAnswer = (CComboBox *)GetDlgItem(IDC_COMB_ANWSER);
	CComboBox* pComboxEnadc = (CComboBox *)GetDlgItem(IDC_COMB_ENADC);
	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);

	CButton* pIORead = (CButton *)GetDlgItem(IDC_IO_READ);
	CButton* pIOWrite = (CButton *)GetDlgItem(IDC_IO_WRITE);

	CString strtext;
	//初始化各个变量
    if (g_HardWare_Pare.bSampleModeRealTime == true) {
		pComboxTimeMode->SetCurSel(0);
    }
	else{
		pComboxTimeMode->SetCurSel(1);
	}

	strtext.Format("%d",g_HardWare_Pare.iSampleTimeValue);
	GetDlgItem(IDC_SET_TIME_VALUE)->SetWindowText(strtext);
	switch(g_HardWare_Pare.iChannelWidth) {
	case 1024:
		pComboxChannelWidth->SetCurSel(0);
		break;
	case 2048:
		pComboxChannelWidth->SetCurSel(1);
		break;
	case 4096:
		pComboxChannelWidth->SetCurSel(2);
		break;
	case 8192:
		pComboxChannelWidth->SetCurSel(3);
		break;
	default:
		break;
	}
	
	if (g_HardWare_Pare.bDT_Pulse == true) {
		pComboxDT->SetCurSel(0);
	}
	else pComboxDT->SetCurSel(1);	
	
	if (g_HardWare_Pare.bREADY_Pulse == true) {
		pComboxReady->SetCurSel(0);
	}
	else pComboxReady->SetCurSel(1);
	

	if (g_HardWare_Pare.bANSER_Pulse == true) {
		pComboxAnswer->SetCurSel(0);
	}
	else pComboxAnswer->SetCurSel(1);
	

	if (g_HardWare_Pare.bENADC_Pulse == true) {
		pComboxEnadc->SetCurSel(0);
	}
	else pComboxEnadc->SetCurSel(1);

	strtext.Format("%.3f",g_HardWare_Pare.fEngerScaleSlope);
	GetDlgItem(IDC_ENGER_KEDU_SLOPE)->SetWindowText(strtext);
	strtext.Format("%.3f",g_HardWare_Pare.fEngerScaleOffset);
	GetDlgItem(IDC_ENGER_KEDU_OFFSET)->SetWindowText(strtext);

	switch(g_HardWare_Pare.iEngerScaleUnit) {
	case 0:
		pComboxEngeUnit->SetCurSel(0);
		break;
	case 1:
		pComboxEngeUnit->SetCurSel(1);
		break;
	case 2:
		pComboxEngeUnit->SetCurSel(2);
		break;
	case 3:
		pComboxEngeUnit->SetCurSel(3);
		break;
	default:
		pComboxEngeUnit->SetCurSel(0);
		break;
	}
	
	if (g_HardWare_Pare.bReadorWriteOperate == true) {
		pIORead->SetCheck(1);
		pIOWrite->SetCheck(0);
	}
	else {
		pIORead->SetCheck(0);
		pIOWrite->SetCheck(1);
	}

	UpdateData(false);
}


void CHardWarePara::OnCancel() 
{
/*	if(m_pView != NULL) {
		UpdateData(true);
		m_pView->PostMessage(WM_HARDEWAREPARA_GOODBYE, IDCANCEL);
	}
	else {
		CDialog::OnOK();
	}	
*/
}
