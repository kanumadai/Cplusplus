// ProgCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "ProgCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgCtrl dialog


CProgCtrl::CProgCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CProgCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgCtrl, CDialog)
	//{{AFX_MSG_MAP(CProgCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgCtrl message handlers

BOOL CProgCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
	SetDlgItemText(IDC_EDIT_PARA_X,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[6]);
	SetDlgItemText(IDC_EDIT_PARA_Y,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[7]);
	SetDlgItemText(IDC_EDIT_PARA_E,chrtemp);
	sprintf(chrtemp,"%d",iProgCtrlData[8]);
	SetDlgItemText(IDC_EDIT_PARA_ETH,chrtemp);



	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgCtrl::OnOK() 
{
	CEdit *pPara_A = (CEdit*)GetDlgItem(IDC_EDIT_PARA_A);
	CEdit *pPara_B = (CEdit*)GetDlgItem(IDC_EDIT_PARA_B);
	CEdit *pPara_C = (CEdit*)GetDlgItem(IDC_EDIT_PARA_C);
	CEdit *pPara_D = (CEdit*)GetDlgItem(IDC_EDIT_PARA_D);

	CEdit *pPara_X = (CEdit*)GetDlgItem(IDC_EDIT_PARA_X);
	CEdit *pPara_Y = (CEdit*)GetDlgItem(IDC_EDIT_PARA_Y);
	CEdit *pPara_E = (CEdit*)GetDlgItem(IDC_EDIT_PARA_E);
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
	
	CDialog::OnOK();
}
