// FHParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "FHParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHParamDlg dialog


CFHParamDlg::CFHParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFHParamDlg::IDD, pParent)
{
	m_dlgMode = expert;
	//{{AFX_DATA_INIT(CFHParamDlg)
	m_fov = 15;
	m_ring = 31;
	m_energymode = 0;
	m_stopType = 2;   //stop by user
	m_stopTime = 10;
	m_stopEvent = 200000;
	m_bFH = 0;
	m_scanmode = 2;
	m_Backword_RR_Num = 8;
	m_Backword_RR_Per = 5;
	m_Forword_RR_Num = 8;
	m_Forword_RR_Per = 5;
	m_LBP = 60;
	m_UBP = 120;
	m_GateMode = 0;
	m_RodSpeed = 2;
	m_bRodEnable = FALSE;
	m_bSeptaEnable = FALSE;
	m_nCoinFmt = 0;    //coin data fmt: 0->pos   1->time
	m_nCoinTimeBucket = 0;  //when coin data fmt set to time, indicate selected bucket(31:0)
	//}}AFX_DATA_INIT
}


void CFHParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFHParamDlg)
	DDX_Text(pDX, IDC_FOV, m_fov);
	DDV_MinMaxInt(pDX, m_fov, 0, 15);
	DDX_Text(pDX, IDC_RING, m_ring);
	DDV_MinMaxInt(pDX, m_ring, 0, 31);
	DDX_Radio(pDX, IDC_ENERGYMODE, m_energymode);
	DDX_Radio(pDX, IDC_STOPONCNT, m_stopType);
	DDX_Text(pDX, IDC_STOP_TIME, m_stopTime);
	DDV_MinMaxInt(pDX, m_stopTime, 10, 6000);
	DDX_Text(pDX, IDC_STOP_EVENTCNT, m_stopEvent);
	DDV_MinMaxInt(pDX, m_stopEvent, 100, 1000000000);
	DDX_Radio(pDX, IDC_WORKMODE_DEBUG, m_bFH);
	DDX_Radio(pDX, IDC_SCANMODE, m_scanmode);
	DDX_Text(pDX, IDC_BACKWORD_RR_NUM, m_Backword_RR_Num);
	DDV_MinMaxInt(pDX, m_Backword_RR_Num, 0, 30);
	DDX_Text(pDX, IDC_BACKWORD_RR_PER, m_Backword_RR_Per);
	DDV_MinMaxInt(pDX, m_Backword_RR_Per, 0, 50);
	DDX_Text(pDX, IDC_FORWORD_RR_NUM, m_Forword_RR_Num);
	DDV_MinMaxInt(pDX, m_Forword_RR_Num, 0, 30);
	DDX_Text(pDX, IDC_FORWORD_RR_PER, m_Forword_RR_Per);
	DDV_MinMaxInt(pDX, m_Forword_RR_Per, 0, 50);
	DDX_Text(pDX, IDC_LBP, m_LBP);
	DDV_MinMaxInt(pDX, m_LBP, 10, 80);
	DDX_Text(pDX, IDC_UPB, m_UBP);
	DDV_MinMaxInt(pDX, m_UBP, 60, 180);
	DDX_CBIndex(pDX, IDC_GATEMODE, m_GateMode);
	DDX_CBIndex(pDX, IDC_RODSPEED, m_RodSpeed);
	DDX_Check(pDX, IDC_RODENABLE, m_bRodEnable);
	DDX_Check(pDX, IDC_SEPTAENABLE, m_bSeptaEnable);
	DDX_Radio(pDX, IDC_COIN_POS, m_nCoinFmt);
	DDX_CBIndex(pDX, IDC_COIN_BUCKET, m_nCoinTimeBucket);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFHParamDlg, CDialog)
	//{{AFX_MSG_MAP(CFHParamDlg)
	ON_EN_CHANGE(IDC_STOP_EVENTCNT, OnChangeStopEventcnt)
	ON_EN_CHANGE(IDC_STOP_TIME, OnChangeStopTime)
	ON_BN_CLICKED(IDC_SCANMODE, OnScanmode)
	ON_BN_CLICKED(IDC_SCANMODE1, OnScanmode1)
	ON_BN_CLICKED(IDC_SCANMODE2, OnScanmode2)
	ON_BN_CLICKED(IDC_SCANMODE3, OnScanmode3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHParamDlg message handlers

void CFHParamDlg::OnChangeStopEventcnt() 
{
	// TODO: Add your control notification handler code here
	CButton* pBtn;
	pBtn = (CButton*)GetDlgItem(IDC_STOPONCNT); pBtn->SetCheck(1);
	pBtn = (CButton*)GetDlgItem(IDC_STOPONTIME); pBtn->SetCheck(0);
	pBtn = (CButton*)GetDlgItem(IDC_STOPONUSER); pBtn->SetCheck(0);
}

void CFHParamDlg::OnChangeStopTime() 
{
	// TODO: Add your control notification handler code here
	CButton* pBtn;
	pBtn = (CButton*)GetDlgItem(IDC_STOPONCNT); pBtn->SetCheck(0);
	pBtn = (CButton*)GetDlgItem(IDC_STOPONTIME); pBtn->SetCheck(1);
	pBtn = (CButton*)GetDlgItem(IDC_STOPONUSER); pBtn->SetCheck(0);
}


BOOL CFHParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_dlgMode){
	case expert:
		GetDlgItem(IDC_WORKMODE_DEBUG)->EnableWindow();
		GetDlgItem(IDC_WORKMODE_FH)->EnableWindow();
		GetDlgItem(IDC_COIN_POS)->EnableWindow();
		GetDlgItem(IDC_COIN_TIME)->EnableWindow();

		GetDlgItem(IDC_SCANMODE)->EnableWindow();
		GetDlgItem(IDC_SCANMODE1)->EnableWindow();
		GetDlgItem(IDC_SCANMODE2)->EnableWindow();
		GetDlgItem(IDC_SCANMODE3)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE2)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE3)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE4)->EnableWindow();

		GetDlgItem(IDC_UPB)->EnableWindow();
		GetDlgItem(IDC_LBP)->EnableWindow();

		GateParamEnable(TRUE);

		SetWindowText("符合\BUCKET  采集参数");
		break;
	case coin_pos:
		GetDlgItem(IDC_WORKMODE_DEBUG)->EnableWindow(FALSE);
		GetDlgItem(IDC_WORKMODE_FH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COIN_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COIN_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COIN_BUCKET)->EnableWindow(FALSE);
		  
		GetDlgItem(IDC_SCANMODE)->EnableWindow();
		GetDlgItem(IDC_SCANMODE1)->EnableWindow();
		GetDlgItem(IDC_SCANMODE2)->EnableWindow();
		GetDlgItem(IDC_SCANMODE3)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE2)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE3)->EnableWindow();
		GetDlgItem(IDC_ENERGYMODE4)->EnableWindow();

		if(m_scanmode == 3) GateParamEnable(TRUE);
		else GateParamEnable(FALSE);

		SetWindowText("符合  采集参数");
		break;
	case coin_time:
		GetDlgItem(IDC_COIN_BUCKET)->EnableWindow();

		GetDlgItem(IDC_WORKMODE_DEBUG)->EnableWindow(FALSE);
		GetDlgItem(IDC_WORKMODE_FH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COIN_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COIN_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENERGYMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENERGYMODE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENERGYMODE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENERGYMODE4)->EnableWindow(FALSE);
		
		GateParamEnable(FALSE);

		SetWindowText("符合时间谱  采集参数");
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFHParamDlg::GateParamEnable(BOOL bEnable)
{

	int checked;
	checked = ((CButton*)GetDlgItem(IDC_STOPONTIME))->GetCheck();
	if(bEnable){
		if( checked ){
			((CButton*)GetDlgItem(IDC_STOPONTIME))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_STOPONUSER))->SetCheck(1);
		}
	}
	if( m_dlgMode == expert ){
		GetDlgItem(IDC_STOPONTIME)->EnableWindow();
		GetDlgItem(IDC_STOP_TIME)->EnableWindow();
	}else{
		GetDlgItem(IDC_STOPONTIME)->EnableWindow(!bEnable);
		GetDlgItem(IDC_STOP_TIME)->EnableWindow(!bEnable);
	}

	GetDlgItem(IDC_UPB)->EnableWindow(bEnable);
	GetDlgItem(IDC_LBP)->EnableWindow(bEnable);
	GetDlgItem(IDC_FORWORD_RR_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDC_FORWORD_RR_PER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BACKWORD_RR_NUM)->EnableWindow(bEnable);
	GetDlgItem(IDC_BACKWORD_RR_PER)->EnableWindow(bEnable);
	GetDlgItem(IDC_GATEMODE)->EnableWindow(bEnable);
}

void CFHParamDlg::OnScanmode() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgMode == expert ) return;

	GateParamEnable(FALSE);
}

void CFHParamDlg::OnScanmode1() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgMode == expert ) return;

	GateParamEnable(FALSE);
}

void CFHParamDlg::OnScanmode2() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgMode == expert ) return;

	GateParamEnable(FALSE);
}

void CFHParamDlg::OnScanmode3() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgMode == expert ) return;

	GateParamEnable(TRUE);
}
