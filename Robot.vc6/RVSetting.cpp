// RVSetting.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "RVSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRVSetting dialog


CRVSetting::CRVSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CRVSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRVSetting)
	m_intFrontVelocity = 0;
	m_intMiddleVelocity = 0;
	m_intRearVelocity = 0;
	m_intTurnVelocity = 0;
	m_intSonerLimted = 0;
	m_intIRLimted = 0;
	//}}AFX_DATA_INIT
}


void CRVSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRVSetting)
	DDX_Control(pDX, IDC_SLIDERFrontVel, m_FrontVelocity);
	DDX_Control(pDX, IDC_SLIDERMiddelVel, m_MiddleVelocity);
	DDX_Control(pDX, IDC_SLIDERRearVel, m_RearVelocity);
	DDX_Control(pDX, IDC_SLIDERTurnVel, m_TurnVelocity);
	DDX_Control(pDX, IDC_SLIDERSonerLimted, m_SonerLimted);
	DDX_Control(pDX, IDC_SLIDERIRLimted, m_IRLimted);
	DDX_Text(pDX, IDC_EDIT1, m_intFrontVelocity);
	DDX_Text(pDX, IDC_EDIT2, m_intMiddleVelocity);
	DDX_Text(pDX, IDC_EDIT3, m_intRearVelocity);
	DDX_Text(pDX, IDC_EDIT4, m_intTurnVelocity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRVSetting, CDialog)
	//{{AFX_MSG_MAP(CRVSetting)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRVSetting message handlers

BOOL CRVSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_FrontVelocity.SetRange(0,1200);
	m_MiddleVelocity.SetRange(0,1200);
	m_RearVelocity.SetRange(0,1200);
	m_TurnVelocity.SetRange(0,4000);
	m_SonerLimted.SetRange(0,500);
	m_IRLimted.SetRange(0,50);

	m_FrontVelocity.SetPos(600);
	m_MiddleVelocity.SetPos(600);
	m_RearVelocity.SetPos(600);	
	m_TurnVelocity.SetPos(2000);
	m_SonerLimted.SetPos(80);	
	m_IRLimted.SetPos(40);	
	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRVSetting::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_intFrontVelocity = m_FrontVelocity.GetPos();
    m_intMiddleVelocity = m_MiddleVelocity.GetPos();
	m_intRearVelocity = m_RearVelocity.GetPos();
	m_intTurnVelocity = m_TurnVelocity.GetPos(); 
	m_intSonerLimted = m_SonerLimted.GetPos();
	m_intIRLimted = (50-m_IRLimted.GetPos())*10; 	
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
