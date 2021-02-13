// SysInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACQCenter.h"
#include "SysInitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysInitDlg dialog


CSysInitDlg::CSysInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysInitDlg)
	m_bEnDbg = 1;
	m_bEnSIOBucket = FALSE;
	m_bEnSIOEMC = FALSE;
	m_bEnDbgBucket = FALSE;
	//}}AFX_DATA_INIT
}


void CSysInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysInitDlg)
	DDX_Radio(pDX, IDC_RADIO_ENDBG, m_bEnDbg);
//	DDX_Check(pDX, IDC_EN_SIOBUCKET, m_bEnSIOBucket);
	DDX_Check(pDX, IDC_EN_SIOEMC, m_bEnSIOEMC);
	DDX_Check(pDX, IDC_EN_DBGBUCKET, m_bEnDbgBucket);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysInitDlg, CDialog)
	//{{AFX_MSG_MAP(CSysInitDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysInitDlg message handlers
BOOL CSysInitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
#ifdef _FH_SIM
	m_dwTimeOut = 4;
#else
	m_dwTimeOut = 4;  //15s timeout deadline
#endif
	SetDlgItemText(IDC_STATIC_TIMEOUT, "Waiting For Selection: 4 S");

	SetTimer(100, 1000/*ms*/,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSysInitDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	char szTemp[128];

	if(nIDEvent == 100){
		m_dwTimeOut--;

		sprintf(szTemp, "Waiting For Selection: %d S", m_dwTimeOut);
		SetDlgItemText(IDC_STATIC_TIMEOUT, szTemp);

		if(m_dwTimeOut == 0){
			KillTimer(100);
			OnOK();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

