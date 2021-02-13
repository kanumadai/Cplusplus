// AutoSampleOption.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "AutoSampleOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoSampleOption dialog


CAutoSampleOption::CAutoSampleOption(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoSampleOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoSampleOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAutoSampleOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSampleOption)
	DDX_Control(pDX, IDC_PARA_TIMESERIER, m_Para_TimeSerier);
	DDX_Control(pDX, IDC_PARA_TIMEMODFY, m_Para_TimeModify);
	DDX_Control(pDX, IDC_PARA_POSITION_LIMIT, m_Para_Position_Limit);
	DDX_Control(pDX, IDC_PARA_ENERGE_LIMIT, m_Para_Energe_Limit);
	DDX_Control(pDX, IDC_PARA_CYSNUM, m_Para_Cysnum);
	DDX_Control(pDX, IDC_PARA_ANOL_PARA, m_Para_Anol_Para);
	DDX_Control(pDX, IDC_CHECK_TIME, m_Check_Time);
	DDX_Control(pDX, IDC_CHECK_ENERGE, m_Check_Energe);
	DDX_Control(pDX, IDC_CHECK_FUHE1, m_Check_Fuhe1);
	DDX_Control(pDX, IDC_CHECK_FUHE2, m_Check_Fuhe2);
	DDX_Control(pDX, IDC_CHECK_SDOT, m_Check_Sdot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoSampleOption, CDialog)
	//{{AFX_MSG_MAP(CAutoSampleOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSampleOption message handlers

BOOL CAutoSampleOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char text[10];
	sprintf(text,"%d",m_iSampleTime);
	GetDlgItem(IDC_EDIT_SAMPL_TIME)->SetWindowText(text);
	sprintf(text,"%d",m_iReSampleNum);
	GetDlgItem(IDC_EDIT_REPEAT_NUM)->SetWindowText(text);
	
	if (m_arrSampleModSel[0] == 0xc7) {
		m_Check_Sdot.SetCheck(1);
	}
	if (m_arrSampleModSel[1] == 0xc5) {
		m_Check_Energe.SetCheck(1);
	}
	if (m_arrSampleModSel[2] == 0xc6 ) {
		m_Check_Time.SetCheck(1);
	}
	if (m_arrSampleModSel[3] == 0xc8) {		
		m_Check_Fuhe1.SetCheck(1);
	}
	if (m_arrSampleModSel[4] == 0xc9) {
		m_Check_Fuhe2.SetCheck(1);
	}	


	if (m_arrAutoSampParaSel[0] == 1) {
		m_Para_Position_Limit.SetCheck(1);
	}
	if (m_arrAutoSampParaSel[1] == 1) {
		m_Para_Energe_Limit.SetCheck(1);
	}
	if (m_arrAutoSampParaSel[2] == 1) {
		m_Para_Anol_Para.SetCheck(1);
	}
	if (m_arrAutoSampParaSel[3] == 1) {
		m_Para_TimeModify.SetCheck(1);
	}
	if (m_arrAutoSampParaSel[4] == 1) {
		m_Para_Cysnum.SetCheck(1);
	}
	if (m_arrAutoSampParaSel[5] == 1) {
		m_Para_TimeSerier.SetCheck(1);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CAutoSampleOption::OnOK() 
{
	UpdateData(true);
	memset(m_arrSampleModSel,0,5*4);
	bool flag = false;
	if (m_Check_Sdot.GetCheck() == 1) {
		m_arrSampleModSel[0] = 0xc7;
		flag = true;
	}
	if (m_Check_Energe.GetCheck() == 1) {
		m_arrSampleModSel[1] = 0xc5;
		flag = true;
	}
	if (m_Check_Time.GetCheck() == 1) {
		m_arrSampleModSel[2] = 0xc6;
		flag = true;
	}
	if (m_Check_Fuhe1.GetCheck() == 1) {
		m_arrSampleModSel[3] = 0xc8;
		flag = true;
	}
	if (m_Check_Fuhe2.GetCheck() == 1) {
		m_arrSampleModSel[4] = 0xc9;
		flag = true;
	}
	
	CString strSampleTime;
	CString strReSamplNum;

	GetDlgItem(IDC_EDIT_SAMPL_TIME)->GetWindowText(strSampleTime);
	GetDlgItem(IDC_EDIT_REPEAT_NUM)->GetWindowText(strReSamplNum);
	m_iSampleTime = _atoi64(strSampleTime);
	m_iReSampleNum = _atoi64(strReSamplNum);

	if ((m_iReSampleNum > 100)||(m_iReSampleNum < 1)) {
		AfxMessageBox("输入的循环次数越界");
		return;
	}

	if ((m_iSampleTime > 65535)||(m_iSampleTime < 1)) {
		AfxMessageBox("输入的采样时间越界");
		return;
	}
	if (!flag) {
		AfxMessageBox("请选择采样模式");
		return;
	}
	

	memset(m_arrAutoSampParaSel,0,10*4);
	flag = false;
	if (m_Para_Position_Limit.GetCheck() == 1) {
		m_arrAutoSampParaSel[0] = 1;
		flag = true;
	}
	if (m_Para_Energe_Limit.GetCheck() == 1) {
		m_arrAutoSampParaSel[1] = 1;
		flag = true;
	}
	if (m_Para_Anol_Para.GetCheck() == 1) {
		m_arrAutoSampParaSel[2] = 1;
		flag = true;
	}
	if (m_Para_TimeModify.GetCheck() == 1) {
		m_arrAutoSampParaSel[3] = 1;
		flag = true;
	}
	if (m_Para_Cysnum.GetCheck() == 1) {
		m_arrAutoSampParaSel[4] = 1;
		flag = true;
	}
	if (m_Para_TimeSerier.GetCheck() == 1) {
		m_arrAutoSampParaSel[5] = 1;
		flag = true;
	}

	if (!flag) {
		if(IDYES != AfxMessageBox("真的不准备上传参数文件了?",MB_YESNO) ) {
			return;
		}
	}

	CDialog::OnOK();
}
