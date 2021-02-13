// ProgContrl.cpp : implementation file
//

#include "stdafx.h"
#include "DynShow.h"
#include "ProgContrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgContrl dialog


CProgContrl::CProgContrl(CWnd* pParent /*=NULL*/)
	: CDialog(CProgContrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgContrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgContrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgContrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgContrl, CDialog)
	//{{AFX_MSG_MAP(CProgContrl)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_SETPARA, OnSetpara)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgContrl message handlers

BOOL CProgContrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char chrtemp[10];
	sprintf(chrtemp,"%d",iProgCtrlData[1]);
	SetDlgItemText(IDC_EDIT_PARA_A,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[2]);
	SetDlgItemText(IDC_EDIT_PARA_B,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[3]);
	SetDlgItemText(IDC_EDIT_PARA_C,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[4]);
	SetDlgItemText(IDC_EDIT_PARA_D,chrtemp);

	sprintf(chrtemp,"%d",iProgCtrlData[5]);
	SetDlgItemText(IDC_EDIT_PARA_XOFFSET,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[6]);
	SetDlgItemText(IDC_EDIT_PARA_YOFFSET,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[7]);
	SetDlgItemText(IDC_EDIT_PARA_EOFFSET,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[8]);
	SetDlgItemText(IDC_EDIT_PARA_ETH,chrtemp);

	CButton *pbuttonA = (CButton*)GetDlgItem(IDC_SEL_CHANNEL_A);
	CButton *pbuttonB = (CButton*)GetDlgItem(IDC_SEL_CHANNEL_B);
	CButton *pTimeAB = (CButton*)GetDlgItem(IDC_AB_TIMEFH);
	CButton *pTimeBkt = (CButton*)GetDlgItem(IDC_BKT_FUHE);
	

	switch(m_iWorkMode) {
	case 0xAA:
			pbuttonA->SetCheck(1);
		break;
	case 0xBB:
		    pbuttonB->SetCheck(1);
		break;
	case 0xDD:
			pTimeAB->SetCheck(1);
		break;
	case 0xEE: 
			pTimeBkt->SetCheck(1);
			break;
	default:
			pbuttonA->SetCheck(1);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgContrl::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
	
}

void CProgContrl::OnSetpara() 
{
	// TODO: Add your control notification handler code here
	CEdit *pPara_A = (CEdit*)GetDlgItem(IDC_EDIT_PARA_A);
	CEdit *pPara_B = (CEdit*)GetDlgItem(IDC_EDIT_PARA_B);
	CEdit *pPara_C = (CEdit*)GetDlgItem(IDC_EDIT_PARA_C);
	CEdit *pPara_D = (CEdit*)GetDlgItem(IDC_EDIT_PARA_D);

	CEdit *pPara_X = (CEdit*)GetDlgItem(IDC_EDIT_PARA_XOFFSET);
	CEdit *pPara_Y = (CEdit*)GetDlgItem(IDC_EDIT_PARA_YOFFSET);
	CEdit *pPara_E = (CEdit*)GetDlgItem(IDC_EDIT_PARA_EOFFSET);
	CEdit *pPara_Eth = (CEdit*)GetDlgItem(IDC_EDIT_PARA_ETH);

	CString strtemp;
	pPara_A->GetWindowText(strtemp);
	iProgCtrlData[1] = atoi(strtemp);
	pPara_B->GetWindowText(strtemp);
	iProgCtrlData[2] = atoi(strtemp);
	pPara_C->GetWindowText(strtemp);
	iProgCtrlData[3] = atoi(strtemp);
	pPara_D->GetWindowText(strtemp);
	iProgCtrlData[4] = atoi(strtemp);

	pPara_X->GetWindowText(strtemp);
	iProgCtrlData[5] = atoi(strtemp);
	pPara_Y->GetWindowText(strtemp);
	iProgCtrlData[6] = atoi(strtemp);
	pPara_E->GetWindowText(strtemp);
	iProgCtrlData[7] = atoi(strtemp);
	pPara_Eth->GetWindowText(strtemp);
	iProgCtrlData[8] = atoi(strtemp);

	for(int i=1;i<=8;i++) {
		if((iProgCtrlData[i]<0)||(iProgCtrlData[i]>255)) {
			AfxMessageBox("设置的参数超出范围！");
			return;
		}
	}

	CButton *pbutton = (CButton*)GetDlgItem(IDC_SEL_CHANNEL_A);
	if (pbutton->GetCheck() == 1) {  
		m_iWorkMode = 0xAA;     // 选择A路通道
	}
	pbutton = (CButton*)GetDlgItem(IDC_SEL_CHANNEL_B);
	if (pbutton->GetCheck() == 1) { 
		m_iWorkMode = 0xBB;    // 选择B路通道
	}
	pbutton = (CButton*)GetDlgItem(IDC_AB_TIMEFH);
	if (pbutton->GetCheck() == 1) { 
		m_iWorkMode = 0xDD;   // 选择 AB 时间符合
	}
	pbutton = (CButton*)GetDlgItem(IDC_BKT_FUHE);
	if (pbutton->GetCheck() == 1) { 
		m_iWorkMode = 0xEE;   // 选择 Bucket 2路符合线 数据符合
	}
	


	CDialog::OnOK();	
}
