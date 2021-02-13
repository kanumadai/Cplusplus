// UsbParaSet.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "UsbParaSet.h"
#include "ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HARDWARE_PARE g_HardWare_Pare;  // 硬件参数结构

/////////////////////////////////////////////////////////////////////////////
// CUsbParaSet dialog


CUsbParaSet::CUsbParaSet(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbParaSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsbParaSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUsbParaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbParaSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsbParaSet, CDialog)
	//{{AFX_MSG_MAP(CUsbParaSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbParaSet message handlers

BOOL CUsbParaSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHN_WIDTH);	
	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);

	CButton* pAnol = (CButton *)GetDlgItem(IDC_ANOL_SIGNAL_SEL);
	CButton* pDigit = (CButton *)GetDlgItem(IDC_DIGIT_SIGNAL_SEL);
	if (g_HardWare_Pare.bInputAnolSignalSel == TRUE) {
		pAnol->SetCheck(1);
		pDigit->SetCheck(0);		
	}
	else {
		pAnol->SetCheck(0);
		pDigit->SetCheck(1);
	}
	
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

    CComboBox* pButt;
	pButt= (CComboBox*)GetDlgItem(IDC_READY_POL);
	if (g_HardWare_Pare.bREADY_Pulse == TRUE) {
	   pButt->SetCurSel(0);
	}else { pButt->SetCurSel(1); }

	pButt= (CComboBox*)GetDlgItem(IDC_DEADTIME_POL);
	if (g_HardWare_Pare.bDT_Pulse == TRUE) {
		pButt->SetCurSel(0);
	}else { pButt->SetCurSel(1); }

	pButt= (CComboBox*)GetDlgItem(IDC_ANSWER_POL);
	if (g_HardWare_Pare.bANSER_Pulse == TRUE) {
		pButt->SetCurSel(0);
	}else { pButt->SetCurSel(1); }

	pButt= (CComboBox*)GetDlgItem(IDC_ENADC_POL);
	if (g_HardWare_Pare.bENADC_Pulse == TRUE) {
		pButt->SetCurSel(0);
	}else { pButt->SetCurSel(1); }

	pButt= (CComboBox*)GetDlgItem(IDC_DATA_POL);
	if (g_HardWare_Pare.bDataPolarity_Pulse == TRUE) {
		pButt->SetCurSel(0);
	}else { pButt->SetCurSel(1); }

	
	char strtest[20];	
	sprintf(strtest,"%.3f",g_HardWare_Pare.fEngerScaleSlope);
	GetDlgItem(IDC_ENGER_KEDU_SLOPE)->SetWindowText(strtest);
	sprintf(strtest,"%.3f",g_HardWare_Pare.fEngerScaleOffset);
	GetDlgItem(IDC_ENGER_KEDU_OFFSET)->SetWindowText(strtest);

	sprintf(strtest,"%d",g_HardWare_Pare.iEth_Up_UsbADC);
	GetDlgItem(IDC_ETH_UP_USBADC)->SetWindowText(strtest);
	sprintf(strtest,"%d",g_HardWare_Pare.iEth_Down_UsbADC);
	GetDlgItem(IDC_ETH_DOWN_USBADC)->SetWindowText(strtest);

	sprintf(strtest,"%d",g_HardWare_Pare.iTime_Up_ETH);
	GetDlgItem(IDC_TIME_UP_ETH)->SetWindowText(strtest);
	sprintf(strtest,"%d",g_HardWare_Pare.iTime_Down_ETH);
	GetDlgItem(IDC_TIME_DOWN_ETH)->SetWindowText(strtest);
	
	if (g_HardWare_Pare.iEngerScaleUnit >= 4) {
		pComboxEngeUnit->SetCurSel(0);
	}
	else {
		pComboxEngeUnit->SetCurSel(g_HardWare_Pare.iEngerScaleUnit);
	}	
	return TRUE; 
}

void CUsbParaSet::OnOK() 
{
	CComboBox* pComboxChannelWidth = (CComboBox *)GetDlgItem(IDC_CHN_WIDTH);
	CComboBox* pComboxEngeUnit = (CComboBox *)GetDlgItem(IDC_CHECK_ENGER_KEDU_UNIT);
	
	UpdateData(TRUE);

	CString strtext;
	int icursel = pComboxChannelWidth->GetCurSel();
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
		g_HardWare_Pare.iChannelWidth = 2048;
		break;
	}

	float fdata;
	GetDlgItem(IDC_ENGER_KEDU_SLOPE)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleSlope = fdata + 0.000001;

	GetDlgItem(IDC_ENGER_KEDU_OFFSET)->GetWindowText(strtext);
	fdata = atof(strtext);	
	g_HardWare_Pare.fEngerScaleOffset = fdata + 0.000001;

	GetDlgItem(IDC_ETH_UP_USBADC)->GetWindowText(strtext);
	int ibuff = atoi(strtext);
//	if ((ibuff > 16383)||(ibuff < 8192)) {
//		MessageBox("能量上阈越界，范围是:(8192--16383)",MB_OK);
//		return;		
//	}

	g_HardWare_Pare.iEth_Up_UsbADC = ibuff;
	GetDlgItem(IDC_ETH_DOWN_USBADC)->GetWindowText(strtext);
	ibuff = atoi(strtext);
//	if ((ibuff > 16383)||(ibuff < 8000)) {
//		MessageBox("能量下阈越界，范围是:(8000--16383)",MB_OK);
//		return;		
//	}

	g_HardWare_Pare.iEth_Down_UsbADC = ibuff;

	if (g_HardWare_Pare.iEth_Up_UsbADC <= g_HardWare_Pare.iEth_Down_UsbADC) {
		MessageBox("能量上阈值要大于下阈值",MB_OK);
		return;
	}

	GetDlgItem(IDC_TIME_UP_ETH)->GetWindowText(strtext);
	ibuff = atoi(strtext);
	if ((ibuff > 1023)||(ibuff < 5)) {
		MessageBox("时间上阈越界，范围是:(5--1023)",MB_OK);
		return;		
	}
	g_HardWare_Pare.iTime_Up_ETH = ibuff;
	GetDlgItem(IDC_TIME_DOWN_ETH)->GetWindowText(strtext);
	ibuff = atoi(strtext);
	if ((ibuff > 1023)||(ibuff < 5)) {
		MessageBox("时间下阈越界，范围是:(5--1023)",MB_OK);
		return;		
	}
	g_HardWare_Pare.iTime_Down_ETH = ibuff;

	if (g_HardWare_Pare.iTime_Down_ETH >= g_HardWare_Pare.iTime_Up_ETH) {
		MessageBox("能量上阈值要大于下阈值",MB_OK);
		return;
	}

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

    CComboBox* pButt;
	pButt= (CComboBox*)GetDlgItem(IDC_READY_POL);
	if (pButt->GetCurSel() == 0) {
		g_HardWare_Pare.bREADY_Pulse = 1;
	}else {g_HardWare_Pare.bREADY_Pulse = 0;}
	
	pButt= (CComboBox*)GetDlgItem(IDC_DEADTIME_POL);
	if (pButt->GetCurSel() == 0) {
			g_HardWare_Pare.bDT_Pulse = 1;
	}else {g_HardWare_Pare.bDT_Pulse = 0;}
	
	pButt= (CComboBox*)GetDlgItem(IDC_ANSWER_POL);
	if (pButt->GetCurSel() == 0) {
		g_HardWare_Pare.bANSER_Pulse = 1;
	}else {g_HardWare_Pare.bANSER_Pulse = 0;}

	
	pButt= (CComboBox*)GetDlgItem(IDC_ENADC_POL);
	if (pButt->GetCurSel() == 0) {
		g_HardWare_Pare.bENADC_Pulse = 1;
	}else {g_HardWare_Pare.bENADC_Pulse = 0;}
	
	pButt= (CComboBox*)GetDlgItem(IDC_DATA_POL);
	if (pButt->GetCurSel() == 0) {
		g_HardWare_Pare.bDataPolarity_Pulse = 1;
	}else {g_HardWare_Pare.bDataPolarity_Pulse = 0;}
	
	
	CButton* pAnol = (CButton *)GetDlgItem(IDC_ANOL_SIGNAL_SEL);
	CButton* pDigit = (CButton *)GetDlgItem(IDC_DIGIT_SIGNAL_SEL);
	if (1 == pAnol->GetCheck()) {
		g_HardWare_Pare.bInputAnolSignalSel = TRUE;
	}
	if (1 == pDigit->GetCheck()) {
		g_HardWare_Pare.bInputAnolSignalSel = FALSE;
	}


/*	//以下为保存参数到文件“C:\\MCBini_05_1028.ini”
	CIni inifile;
	int intbuff;
	if(inifile.Open("MCBini_05_1028.ini")){ 		
		inifile.Write("MCB HardWare", "fEngerScaleOffset_Int", g_HardWare_Pare.fEngerScaleOffset);
		intbuff = (int)(g_HardWare_Pare.fEngerScaleOffset*10000.) % 10000;
		inifile.Write("MCB HardWare", "fEngerScaleOffset_Dot",intbuff);
		
		inifile.Write("MCB HardWare", "fEngerScaleSlope_Int", g_HardWare_Pare.fEngerScaleSlope);
		intbuff = (int)(g_HardWare_Pare.fEngerScaleSlope*10000.) % 10000;
		inifile.Write("MCB HardWare", "fEngerScaleSlope_Dot", intbuff);		
		
		inifile.Write("MCB HardWare", "iChannelWidth", g_HardWare_Pare.iChannelWidth);
		inifile.Write("MCB HardWare", "iEngerScaleUnit", g_HardWare_Pare.iEngerScaleUnit);

		inifile.Write("MCB HardWare", "bANSER_Pulse", g_HardWare_Pare.bANSER_Pulse);
		inifile.Write("MCB HardWare", "bDT_Pulse", g_HardWare_Pare.bDT_Pulse);
		inifile.Write("MCB HardWare", "bENADC_Pulse", g_HardWare_Pare.bENADC_Pulse);
		inifile.Write("MCB HardWare", "bREADY_Pulse", g_HardWare_Pare.bREADY_Pulse);
		inifile.Write("MCB HardWare", "bDataPolarity_Pulse", g_HardWare_Pare.bDataPolarity_Pulse);


		inifile.Write("USB Para", "iEth_Up_UsbADC", g_HardWare_Pare.iEth_Up_UsbADC);
		inifile.Write("USB Para", "iEth_Down_UsbADC", g_HardWare_Pare.iEth_Down_UsbADC);

		inifile.Write("USB Para", "iTime_Up_ETH", g_HardWare_Pare.iTime_Up_ETH);
		inifile.Write("USB Para", "iTime_Down_ETH", g_HardWare_Pare.iTime_Down_ETH);
	    inifile.Write("USB Para", "bInputAnolSignalSel", g_HardWare_Pare.bInputAnolSignalSel);

		inifile.Save();
		inifile.Close();
	}
	else {
		AfxMessageBox("硬件参数保存出错");
	}
*/	

	
	CDialog::OnOK();
}
