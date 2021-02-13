// SetHardWarePara.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "SetHardWarePara.h"
#include "ini.h"
#include "MCBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HARDWARE_PARE g_HardWare_Pare;  // Ӳ�������ṹ

extern CWinThread* g_pThread_Monitor;	//����߳�ָ��
extern CWinThread* g_pThread_Send;		//�����߳�ָ��
extern bool g_bRunMonitorThread;
extern bool g_bRunSendThread;

extern int	g_RecPackageLenth; //ȷ��ÿ���������ݰ��� ����
extern UINT		g_ThreadCMD;
extern BOOL		g_bExec;		//���ڽ���ͨ�ű�־
extern BOOL     g_bComOpen;
extern HANDLE		g_hEvtSend;
extern HANDLE		g_hEvtFinishRec;     //��ɽ��վ���������շ�ͬ���á� 
extern HANDLE		g_hExitSendThread;
extern HANDLE		g_hExitMonitThread;


/////////////////////////////////////////////////////////////////////////////
// CSetHardWarePara dialog

CSetHardWarePara::CSetHardWarePara(CView *pView)
{
	m_pView = pView;  //akang add
}

CSetHardWarePara::CSetHardWarePara(CWnd* pParent /*=NULL*/)
	: CDialog(CSetHardWarePara::IDD, pParent)
{
	m_pView = NULL;   //akang add

	//{{AFX_DATA_INIT(CSetHardWarePara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CSetHardWarePara::Create()  //akang add
{
	return CDialog::Create(CSetHardWarePara::IDD);
}

void CSetHardWarePara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHardWarePara)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHardWarePara, CDialog)
	//{{AFX_MSG_MAP(CSetHardWarePara)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHardWarePara message handlers

BOOL CSetHardWarePara::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OnUpdataShow();
	
	CFrameWnd *pMainframe=GetParentFrame();
	CMCBView *View=(CMCBView *)(pMainframe->GetActiveView());
	CStatic *phardstate=(CStatic *)GetDlgItem(IDC_STATIC_HARDSTATE);
	if (View->m_startflag||View->m_continueflag)
	{
		CButton *pOK=(CButton *)GetDlgItem(IDOK);
		pOK->EnableWindow(FALSE);
		phardstate->SetWindowText("Ӳ���ɼ�״̬:  ��ʼ�ɼ�");
	}
    else
	{
        phardstate->SetWindowText("Ӳ���ɼ�״̬:  ��");
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetHardWarePara::OnUpdataShow()
{
	CComboBox* pComboxTimeMode = (CComboBox *)GetDlgItem(IDC_CHECK_TIME_MODE);
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHECK_CHANNEL_WIDTH);
	CComboBox* pComboxDT = (CComboBox *)GetDlgItem(IDC_COMB_DT);
	CComboBox* pComboxReady = (CComboBox *)GetDlgItem(IDC_COMB_READY);
	CComboBox* pComboxAnswer = (CComboBox *)GetDlgItem(IDC_COMB_ANWSER);
	CComboBox* pComboxEnadc = (CComboBox *)GetDlgItem(IDC_COMB_ENADC);
	CComboBox* pComboxDataPola = (CComboBox *)GetDlgItem(IDC_COMB_DATA_POLARITY);

	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);
	
	CButton* pIORead = (CButton *)GetDlgItem(IDC_IO_READ);
	CButton* pIOWrite = (CButton *)GetDlgItem(IDC_IO_WRITE);
	
	CString strtext;
	//��ʼ����������
    if (g_HardWare_Pare.bSampleModeRealTime == TRUE) {
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
	
	if (g_HardWare_Pare.bDT_Pulse == TRUE) {
		pComboxDT->SetCurSel(0);
	}
	else pComboxDT->SetCurSel(1);	
	
	if (g_HardWare_Pare.bREADY_Pulse == TRUE) {
		pComboxReady->SetCurSel(0);
	}
	else pComboxReady->SetCurSel(1);
	
	
	if (g_HardWare_Pare.bANSER_Pulse == TRUE) {
		pComboxAnswer->SetCurSel(0);
	}
	else pComboxAnswer->SetCurSel(1);
	
	
	if (g_HardWare_Pare.bENADC_Pulse == TRUE) {
		pComboxEnadc->SetCurSel(0);
	}
	else pComboxEnadc->SetCurSel(1);

	if (g_HardWare_Pare.bDataPolarity_Pulse == TRUE) {
		pComboxDataPola->SetCurSel(0);
	}else pComboxDataPola->SetCurSel(1);
	
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
	
	if (g_HardWare_Pare.bReadorWriteOperate == TRUE) {
		pIORead->SetCheck(1);
		pIOWrite->SetCheck(0);
	}
	else {
		pIORead->SetCheck(0);
		pIOWrite->SetCheck(1);
	}
	
	UpdateData(false);
}


void CSetHardWarePara::OnOK() 
{
	CComboBox* pComboxTimeMode = (CComboBox *)GetDlgItem(IDC_CHECK_TIME_MODE);
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHECK_CHANNEL_WIDTH);
	CComboBox* pComboxDT = (CComboBox *)GetDlgItem(IDC_COMB_DT);
	CComboBox* pComboxReady = (CComboBox *)GetDlgItem(IDC_COMB_READY);
	CComboBox* pComboxAnswer = (CComboBox *)GetDlgItem(IDC_COMB_ANWSER);
	CComboBox* pComboxEnadc = (CComboBox *)GetDlgItem(IDC_COMB_ENADC);
	CComboBox* pComboxDataPola = (CComboBox *)GetDlgItem(IDC_COMB_DATA_POLARITY);
	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);
	
	CButton* pIORead = (CButton *)GetDlgItem(IDC_IO_READ);
	CButton* pIOWrite = (CButton *)GetDlgItem(IDC_IO_WRITE);

	UpdateData(TRUE);

	CString strtext;
	int icursel;
    
	CFrameWnd *pMainframe=GetParentFrame();
	CMCBView *View=(CMCBView *)(pMainframe->GetActiveView());

  if (View->m_bMCBModeSel)
  {
	icursel = pComboxTimeMode->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bSampleModeRealTime = TRUE;
	}
	if (icursel == 1) {
		g_HardWare_Pare.bSampleModeRealTime = FALSE;
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

//	pComboxChannelWidth->SetCurSel(icursel);
	
	int idata;
	GetDlgItem(IDC_SET_TIME_VALUE)->GetWindowText(strtext);
	idata = atol(strtext);
    if (idata > 0) 
	{
		g_HardWare_Pare.iSampleTimeValue = idata;
    } 
	else {
		AfxMessageBox("����ʱ�������������������");
		strtext.Format("%s","����ʱ���������,����������");
		View->TextScroll(strtext);
		return;
	}

	icursel = pComboxDT->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bDT_Pulse = TRUE;
	}
	else g_HardWare_Pare.bDT_Pulse = FALSE;

	icursel = pComboxReady->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bREADY_Pulse = TRUE;
	}
	else g_HardWare_Pare.bREADY_Pulse = FALSE;

	icursel = pComboxAnswer->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bANSER_Pulse = TRUE;
	}
	else g_HardWare_Pare.bANSER_Pulse = FALSE;

	icursel = pComboxEnadc->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bENADC_Pulse = TRUE;
	}
	else g_HardWare_Pare.bENADC_Pulse = FALSE;

	icursel = pComboxDataPola->GetCurSel();
	if (icursel == 0) {
		g_HardWare_Pare.bDataPolarity_Pulse = TRUE;
	}else g_HardWare_Pare.bDataPolarity_Pulse = FALSE;

	float fdata;
	GetDlgItem(IDC_ENGER_KEDU_SLOPE)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleSlope = fdata + 0.000001;

	GetDlgItem(IDC_ENGER_KEDU_OFFSET)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleOffset = fdata + 0.000001;

	icursel = pComboxEngeUnit->GetCurSel();
	switch(icursel) 
	{
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
   
	//����Ϊ��ȡ�û�������Ϣ�������µ����ݻ�������

	int intbuff;
	//����Ϊ����������ļ���C:\\MCBini_05_1028.ini��
/*	CIni inifile;
	if(inifile.Open("MCBini_05_1028.ini"))
	{ //���ļ����ɹ����½�һ���ļ�		
		inifile.Write("MCB HardWare", "bANSER_Pulse", g_HardWare_Pare.bANSER_Pulse);
		inifile.Write("MCB HardWare", "bDT_Pulse", g_HardWare_Pare.bDT_Pulse);
		inifile.Write("MCB HardWare", "bENADC_Pulse", g_HardWare_Pare.bENADC_Pulse);
		inifile.Write("MCB HardWare", "bREADY_Pulse", g_HardWare_Pare.bREADY_Pulse);
		inifile.Write("MCB HardWare", "bReadorWriteOperate", g_HardWare_Pare.bReadorWriteOperate);
		inifile.Write("MCB HardWare", "bSampleModeRealTime", g_HardWare_Pare.bSampleModeRealTime);
		inifile.Write("MCB HardWare", "bDataPolarity_Pulse", g_HardWare_Pare.bDataPolarity_Pulse);

		inifile.Write("MCB HardWare", "fEngerScaleOffset_Int", g_HardWare_Pare.fEngerScaleOffset);
		intbuff = (int)(g_HardWare_Pare.fEngerScaleOffset*10000.) % 10000;
		inifile.Write("MCB HardWare", "fEngerScaleOffset_Dot",intbuff);
		
		inifile.Write("MCB HardWare", "fEngerScaleSlope_Int", g_HardWare_Pare.fEngerScaleSlope);
		intbuff = (int)(g_HardWare_Pare.fEngerScaleSlope*10000.) % 10000;
		inifile.Write("MCB HardWare", "fEngerScaleSlope_Dot", intbuff);		

		inifile.Write("MCB HardWare", "iChannelWidth", g_HardWare_Pare.iChannelWidth);
		inifile.Write("MCB HardWare", "iEngerScaleUnit", g_HardWare_Pare.iEngerScaleUnit);
		inifile.Write("MCB HardWare", "iSampleTimeValue", g_HardWare_Pare.iSampleTimeValue);

		inifile.Save();
		inifile.Close();
	}
	else 
	{
		AfxMessageBox("Ӳ�������������");
	}*/
  }


	//��д���ݲ����ж�
	if(!g_bComOpen) {
		AfxMessageBox("����û�д� \n���ȴ򿪴��ڣ�",MB_ICONINFORMATION);
		strtext.Format("%s","����û�д�,���ȴ򿪴���!");
		View->TextScroll(strtext);
		return;
	}
	if(g_bExec){
		AfxMessageBox("���ڵȴ���һ�������Ӧ�����һ�������",MB_ICONINFORMATION);
		strtext.Format("%s","���ڵȴ���һ�������Ӧ�����һ�������!");
		View->TextScroll(strtext);		
		return;
	}	

	
	if (g_HardWare_Pare.bReadorWriteOperate == TRUE) 
	{ //������
		g_ThreadCMD = CTL_HARDWARE_READ;		
	}
	else 
	{ // д�����
		g_ThreadCMD = CTL_HARDWARE_WRITE;
	}
	SetEvent(g_hEvtSend);
	
	
//	CDialog::OnOK();
}

void CSetHardWarePara::OnCancel() 
{

	if (m_pView != NULL) {
		m_pView->PostMessage(WM_HARDEWAREPARA_GOODBYE,IDCANCEL);
	}
	else CDialog::OnCancel();
}
