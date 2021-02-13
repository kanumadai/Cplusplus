// Set64EnergeLimit.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "Set64EnergeLimit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSet64EnergeLimit dialog


CSet64EnergeLimit::CSet64EnergeLimit(CWnd* pParent /*=NULL*/)
	: CDialog(CSet64EnergeLimit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSet64EnergeLimit)
	//}}AFX_DATA_INIT
}


void CSet64EnergeLimit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSet64EnergeLimit)
	DDX_Control(pDX, IDC_CORNER_CHECK, m_Corner_Check);
	DDX_Control(pDX, IDC_RING_4_CHECK, m_Ring_4_Check);
	DDX_Control(pDX, IDC_RING_3_CHECK, m_Ring_3_Check);
	DDX_Control(pDX, IDC_RING_2_CHECK, m_Ring_2_Check);
	DDX_Control(pDX, IDC_RING_1_CHECK, m_Ring_1_Check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSet64EnergeLimit, CDialog)
	//{{AFX_MSG_MAP(CSet64EnergeLimit)
	ON_BN_CLICKED(IDC_PERSEND_SEL, OnPersendSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSet64EnergeLimit message handlers

BOOL CSet64EnergeLimit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char chtext[10];

	sprintf(chtext,"%d",m_i64EthPara[0][0]);
	GetDlgItem(IDC_RING_1_BEF)->SetWindowText(chtext);
	sprintf(chtext,"%d",m_i64EthPara[0][1]);
	GetDlgItem(IDC_RING_1_BACK)->SetWindowText(chtext);
	if(m_i64EthPara[0][2] == 1) {
		m_Ring_1_Check.SetCheck(1);
	}else m_Ring_1_Check.SetCheck(0);

	sprintf(chtext,"%d",m_i64EthPara[1][0]);
	GetDlgItem(IDC_RING_2_BEF)->SetWindowText(chtext);
	sprintf(chtext,"%d",m_i64EthPara[1][1]);
	GetDlgItem(IDC_RING_2_BACK)->SetWindowText(chtext);
	if(m_i64EthPara[1][2] == 1) {
		m_Ring_2_Check.SetCheck(1);
	}else m_Ring_2_Check.SetCheck(0);

	sprintf(chtext,"%d",m_i64EthPara[2][0]);
	GetDlgItem(IDC_RING_3_BEF)->SetWindowText(chtext);
	sprintf(chtext,"%d",m_i64EthPara[2][1]);
	GetDlgItem(IDC_RING_3_BACK)->SetWindowText(chtext);
	if(m_i64EthPara[2][2] == 1) {
		m_Ring_3_Check.SetCheck(1);
	}else m_Ring_3_Check.SetCheck(0);

	sprintf(chtext,"%d",m_i64EthPara[3][0]);
	GetDlgItem(IDC_RING_4_BEF)->SetWindowText(chtext);
	sprintf(chtext,"%d",m_i64EthPara[3][1]);
	GetDlgItem(IDC_RING_4_BACK)->SetWindowText(chtext);
	if(m_i64EthPara[3][2] == 1) {
		m_Ring_4_Check.SetCheck(1);
	}else m_Ring_4_Check.SetCheck(0);

	sprintf(chtext,"%d",m_i64EthPara[4][0]);
	GetDlgItem(IDC_CORNER_BEF)->SetWindowText(chtext);
	sprintf(chtext,"%d",m_i64EthPara[4][1]);
	GetDlgItem(IDC_CORNER_BACK)->SetWindowText(chtext);
	if(m_i64EthPara[4][2] == 1) {
		m_Corner_Check.SetCheck(1);
	}else m_Corner_Check.SetCheck(0);

	m_bPersendSel = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSet64EnergeLimit::OnOK() 
{
	CString strinput;
	int itemp;

	UpdateData(true);
	//一环
	GetDlgItem(IDC_RING_1_BEF)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 14;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 25;
	}
	m_i64EthPara[0][0] = itemp;

	GetDlgItem(IDC_RING_1_BACK)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 13;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 61;
	}
	m_i64EthPara[0][1] = itemp;

	if(m_Ring_1_Check.GetCheck() == 1){
		m_i64EthPara[0][2] = 1;
	}
	else m_i64EthPara[0][2] = 0;


	//二环
	GetDlgItem(IDC_RING_2_BEF)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 14;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 14;
	}
	m_i64EthPara[1][0] = itemp;

	GetDlgItem(IDC_RING_2_BACK)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 13;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 13;
	}
	m_i64EthPara[1][1] = itemp;

	if(m_Ring_2_Check.GetCheck() == 1){
		m_i64EthPara[1][2] = 1;
	}
	else m_i64EthPara[1][2] = 0;


	//三环
	GetDlgItem(IDC_RING_3_BEF)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 14;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 14;
	}
	m_i64EthPara[2][0] = itemp;

	GetDlgItem(IDC_RING_3_BACK)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 13;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 13;
	}
	m_i64EthPara[2][1] = itemp;

	if(m_Ring_3_Check.GetCheck() == 1){
		m_i64EthPara[2][2] = 1;
	}
	else m_i64EthPara[2][2] = 0;


		//最外环
	GetDlgItem(IDC_RING_4_BEF)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 14;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 12;
	}
	m_i64EthPara[3][0] = itemp;

	GetDlgItem(IDC_RING_4_BACK)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 13;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 15;
	}
	m_i64EthPara[3][1] = itemp;

	if(m_Ring_4_Check.GetCheck() == 1){
		m_i64EthPara[3][2] = 1;
	}
	else m_i64EthPara[3][2] = 0;

	//四边角
	GetDlgItem(IDC_CORNER_BEF)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 14;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 5;
	}
	m_i64EthPara[4][0] = itemp;

	GetDlgItem(IDC_CORNER_BACK)->GetWindowText(strinput);
    if (strinput.IsEmpty()) {  itemp = 13;  }
	itemp = atol(strinput);
	if ((itemp > 60)&&(itemp < 0)) {
		itemp = 50;
	}
	m_i64EthPara[4][1] = itemp;

	if(m_Corner_Check.GetCheck() == 1){
		m_i64EthPara[4][2] = 1;
	}
	else m_i64EthPara[4][2] = 0;


	if(m_bPersendSel) {
		m_i64EthPara[5][2] = 1;
	}
	else m_i64EthPara[5][2] = 0;

	
	CDialog::OnOK();
}

void CSet64EnergeLimit::OnPersendSel() 
{
	m_bPersendSel = !m_bPersendSel;

	if(m_bPersendSel) {
		GetDlgItem(IDC_RING_1_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_RING_2_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_RING_3_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_RING_4_CHECK)->EnableWindow(false);
		GetDlgItem(IDC_CORNER_CHECK)->EnableWindow(false);

	}
	else {
		GetDlgItem(IDC_RING_1_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_RING_2_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_RING_3_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_RING_4_CHECK)->EnableWindow(true);
		GetDlgItem(IDC_CORNER_CHECK)->EnableWindow(true);
	}




}
