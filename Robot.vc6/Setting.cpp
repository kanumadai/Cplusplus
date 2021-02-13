// Setting.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "Setting.h"
#include "robot.h"
#include "robotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting dialog


CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetting)
	m_intvelocity2 = 100;
	m_Slongposition1 = 500;
	m_intvelocity4 = -100;
	m_Slongposition3 = 500;
	m_intvelocity6 = 100;
	m_Slongposition5 = 500;
	m_intvelocity8 = -100;
	m_Slongposition7 = 500;
    m_intvelocity1 = 30000;
	m_intvelocity3 = 30000;
	m_intvelocity5=30000;
	m_intvelocity7=30000;
	m_Slongposition2 = 500;
	m_Slongposition4 = 500;
	m_Slongposition6 = 500;
	m_Slongposition8 = 500;
	m_intHoldPos = 310;
    m_floPetLimtedPosition = 19000;
    m_floPetCtLimtedPosition = 25000;
    m_floCtLimtedPosition = 19000;
    m_intPetHoldPosition = 185; 
	m_intTopHoldPosition = 390; 
	m_intCTHoldPosition  = 250; 
	m_CTFisrtPosition = 1000;
	m_CTSecondPosition = 1000;
	m_PETFisrtPosition = 1000;
	m_PETSecondPosition = 1000;
	//}}AFX_DATA_INIT
}


void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting)
	DDX_Text(pDX, IDC_VELOCITY2, m_intvelocity2);
	DDX_Text(pDX, IDC_POSITION1, m_Slongposition1);
	DDX_Text(pDX, IDC_VELOCITY4, m_intvelocity4);
	DDX_Text(pDX, IDC_POSITION3, m_Slongposition3);
	DDX_Text(pDX, IDC_VELOCITY6, m_intvelocity6);
	DDX_Text(pDX, IDC_POSITION5, m_Slongposition5);
	DDX_Text(pDX, IDC_VELOCITY8, m_intvelocity8);
	DDX_Text(pDX, IDC_POSITION7, m_Slongposition7);
	DDX_Text(pDX, IDC_VELOCITY1, m_intvelocity1);
	DDX_Text(pDX, IDC_VELOCITY3, m_intvelocity3);
	DDX_Text(pDX, IDC_VELOCITY5, m_intvelocity5);
	DDX_Text(pDX, IDC_VELOCITY7, m_intvelocity7);
	DDX_Text(pDX, IDC_POSITION2, m_Slongposition2);
	DDX_Text(pDX, IDC_POSITION4, m_Slongposition4);
	DDX_Text(pDX, IDC_POSITION6, m_Slongposition6);
	DDX_Text(pDX, IDC_POSITION8, m_Slongposition8);
	DDX_Text(pDX, IDC_PetLimtedPosition, m_floPetLimtedPosition);
	DDX_Text(pDX, IDC_PetCtLimtedPosition, m_floPetCtLimtedPosition);
	DDX_Text(pDX, IDC_CtLimtedPosition, m_floCtLimtedPosition);
	DDX_Text(pDX, IDC_PETHOLDPOSITION, m_intPetHoldPosition);
	DDX_Text(pDX, IDC_TOPHOLDPOSITION, m_intTopHoldPosition);
	DDX_Text(pDX, IDC_CTHOLDPOSITION, m_intCTHoldPosition);
	DDX_Text(pDX, IDC_CTFirstPosition, m_CTFisrtPosition);
	DDX_Text(pDX, IDC_CTSecondPosition, m_CTSecondPosition);
	DDX_Text(pDX, IDC_PetFirstPosition, m_PETFisrtPosition);
	DDX_Text(pDX, IDC_PETSecondPosition, m_PETSecondPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
	//{{AFX_MSG_MAP(CSetting)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting message handlers

void CSetting::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CSetting::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	CRobotDlg *pParent=(CRobotDlg *)GetParent(); //加入语句	
    m_floPetLimtedPosition   = pParent->DevicePosition.PET;

	m_floPetCtLimtedPosition = pParent->DevicePosition.PETCT;
	m_floCtLimtedPosition    = pParent->DevicePosition.CT;
	m_floPetLimtedPosition   = pParent->DevicePosition.PET;
	m_intPetHoldPosition     = pParent->DevicePosition.PETHoldPosition;
	m_intTopHoldPosition     = pParent->DevicePosition.TOPHoldPosition;
	m_intCTHoldPosition      = pParent->DevicePosition.CTHoldPosition;
	m_CTFisrtPosition        = pParent->DevicePosition.FisrtCT;
	m_CTSecondPosition       = pParent->DevicePosition.SecondCT;
	m_PETFisrtPosition       = pParent->DevicePosition.FisrtPET;
	m_PETSecondPosition      = pParent->DevicePosition.SecondPET;
	UpdateData(FALSE);//加入语句 
}
