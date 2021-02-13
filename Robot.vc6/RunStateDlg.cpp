// RunStateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "RunStateDlg.h"
#include "robotDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunStateDlg dialog


CRunStateDlg::CRunStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunStateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunStateDlg)
	m_velocity1 = 0;//500*33.3
	m_velocity2 = 0;
	m_velocity3 = 0;
	m_velocity4 = 0;
	m_velocity5 = 0;
	m_velocity6 = 0;
	m_velocity7 = 0;
	m_velocity8 = 0;
	m_AD1 = 0;
	m_AD2 = 0;
	m_AD3 = 0;
	m_AD4 = 0;
	m_IO1 = 0;
	m_IO2 = 0;
	m_IO3 = 0;
	m_IO4 = 0;
	m_IO5 = 0;
	m_IO6 = 0;
	m_IO7 = 0;
	m_IO8 = 0;
	m_RevTurnPos1 = 0;
	m_RevTurnPos3 = 0;
	m_RevTurnPos5 = 0;
	m_RevTurnPos7 = 0;
	m_RevTurnPos2 = 0;
	m_RevTurnPos4 = 0;
	m_RevTurnPos6 = 0;
	m_RevTurnPos8 = 0;

	m_UintSensor1 = 0;
	m_UintSensor2 = 0;	
	m_UintSensor3 = 0;
	m_UintSensor4 = 0;
	m_UintSensor21 = 0;
	m_UintSensor22 = 0;
	m_UintSensor23 = 0;
	m_UintSensor24 = 0;
	//}}AFX_DATA_INIT
	for (UINT i = 0; i < 11; i++)
	{
		IR[i] = 0;
	}
	for (i = 0; i < 13; i++)
	{
		m_ReturnShow[i] = 0;
	}

}


void CRunStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunStateDlg)

		DDX_Text(pDX, IDC_VELOCITY1, m_velocity1);
		DDX_Text(pDX, IDC_VELOCITY2, m_velocity2);
		DDX_Text(pDX, IDC_VELOCITY3, m_velocity3);
		DDX_Text(pDX, IDC_VELOCITY4, m_velocity4);
		DDX_Text(pDX, IDC_VELOCITY5, m_velocity5);
		DDX_Text(pDX, IDC_VELOCITY6, m_velocity6);
		DDX_Text(pDX, IDC_VELOCITY7, m_velocity7);
		DDX_Text(pDX, IDC_VELOCITY8, m_velocity8);
		DDX_Text(pDX, IDC_AD1, m_AD1);
		DDX_Text(pDX, IDC_AD2, m_AD2);
		DDX_Text(pDX, IDC_AD3, m_AD3);
		DDX_Text(pDX, IDC_AD4, m_AD4);
		DDX_Text(pDX, IDC_IO1, m_IO1);
		DDX_Text(pDX, IDC_IO2, m_IO2);
		DDX_Text(pDX, IDC_IO3, m_IO3);
		DDX_Text(pDX, IDC_IO4, m_IO4);
		DDX_Text(pDX, IDC_IO5, m_IO5);
		DDX_Text(pDX, IDC_IO6, m_IO6);
		DDX_Text(pDX, IDC_IO7, m_IO7);
		DDX_Text(pDX, IDC_IO8, m_IO8);
		DDX_Text(pDX, IDC_RevPosition1, m_RevTurnPos1);
		DDX_Text(pDX, IDC_RevPosition3, m_RevTurnPos3);
		DDX_Text(pDX, IDC_RevPosition5, m_RevTurnPos5);
		DDX_Text(pDX, IDC_RevPosition7, m_RevTurnPos7);
		DDX_Text(pDX, IDC_RevPosition2, m_RevTurnPos2);
		DDX_Text(pDX, IDC_RevPosition4, m_RevTurnPos4);
		DDX_Text(pDX, IDC_RevPosition6, m_RevTurnPos6);
		DDX_Text(pDX, IDC_RevPosition8, m_RevTurnPos8);
		DDX_Text(pDX, IDC_SSensor1, m_UintSensor1);
		DDX_Text(pDX, IDC_SSensor2, m_UintSensor2);
		DDX_Text(pDX, IDC_SSensor3, m_UintSensor3);
		DDX_Text(pDX, IDC_SSensor4, m_UintSensor4);
		DDX_Text(pDX, IDC_SorSensor5, m_UintSensor21);
		DDX_Text(pDX, IDC_SorSensor6, m_UintSensor22);
		DDX_Text(pDX, IDC_SSensor7, m_UintSensor23);
		DDX_Text(pDX, IDC_SSensor8, m_UintSensor24);

	//}}AFX_DATA_MAP
    	DDX_Text(pDX, IDC_IR1, IR[0]);
		DDX_Text(pDX, IDC_IR2, IR[1]);
		DDX_Text(pDX, IDC_IR3, IR[2]);
		DDX_Text(pDX, IDC_IR4, IR[3]);
		DDX_Text(pDX, IDC_IR5, IR[4]);
		DDX_Text(pDX, IDC_IR6, IR[5]);
		DDX_Text(pDX, IDC_IR7, IR[6]);
		DDX_Text(pDX, IDC_IR8, IR[7]);
		DDX_Text(pDX, IDC_IR9, IR[8]);
		DDX_Text(pDX, IDC_IR10, IR[9]);
		DDX_Text(pDX, IDC_IR11, IR[10]);
	DDX_Text(pDX, IDC_IR14, m_ReturnShow[0]);
	DDX_Text(pDX, IDC_IR15, m_ReturnShow[1]);
	DDX_Text(pDX, IDC_IR13, m_ReturnShow[2]);
	DDX_Text(pDX, IDC_IR16, m_ReturnShow[3]);
	DDX_Text(pDX, IDC_IR17, m_ReturnShow[4]);
	DDX_Text(pDX, IDC_IR18, m_ReturnShow[5]);
	DDX_Text(pDX, IDC_IR19, m_ReturnShow[6]);
	DDX_Text(pDX, IDC_IR20, m_ReturnShow[7]);
	DDX_Text(pDX, IDC_IR21, m_ReturnShow[8]);
	DDX_Text(pDX, IDC_IR22, m_ReturnShow[9]);
	DDX_Text(pDX, IDC_IR23, m_ReturnShow[10]);
	DDX_Text(pDX, IDC_IR24, m_ReturnShow[11]);
    DDX_Text(pDX, IDC_IR25, m_ReturnShow[12]);
}


BEGIN_MESSAGE_MAP(CRunStateDlg, CDialog)
	//{{AFX_MSG_MAP(CRunStateDlg)
			ON_MESSAGE(WM_SHOWWINDOW ,OnShowWindow)	
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunStateDlg message handlers

void CRunStateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog :: OnShowWindow(bShow, nStatus); 
	CRobotDlg *pParent=(CRobotDlg *)GetParent(); //加入语句
	m_velocity1 = pParent->m_longVelocity[0];//500*33.3
	m_velocity2 = pParent->m_longVelocity[1];
	m_velocity3 = pParent->m_longVelocity[2];
	m_velocity4 = pParent->m_longVelocity[3];
	m_velocity5 = pParent->m_longVelocity[4];
	m_velocity6 = pParent->m_longVelocity[5];
	m_velocity7 = pParent->m_longVelocity[6];
	m_velocity8 = pParent->m_longVelocity[7];

	m_AD1 = pParent->m_AD1;
	m_AD2 = pParent->m_AD2;
	m_AD3 = pParent->m_AD3;
	m_AD4 = pParent->m_AD4;
	m_IO1 = pParent->m_IO1;
	m_IO2 = pParent->m_IO2;
	m_IO3 = pParent->m_IO3;
	m_IO4 = pParent->m_IO4;
	m_IO5 = pParent->m_IO5;
	m_IO6 = pParent->m_IO6;
	m_IO7 = pParent->m_IO7;
	m_IO8 = pParent->m_IO8;
	m_RevTurnPos1 = pParent->m_RevTurnPos1;
	m_RevTurnPos3 = pParent->m_RevTurnPos3;
	m_RevTurnPos5 = pParent->m_RevTurnPos5;
	m_RevTurnPos7 = pParent->m_RevTurnPos7;
	m_RevTurnPos2 = pParent->m_RevTurnPos2;
	m_RevTurnPos4 = pParent->m_RevTurnPos4;
	m_RevTurnPos6 = pParent->m_RevTurnPos6;
	m_RevTurnPos8 = pParent->m_RevTurnPos8;

	m_UintSensor1 = pParent->SonorPosition[RightSonor1].SonerPosition;
	m_UintSensor2 = pParent->SonorPosition[RightSonor2].SonerPosition;
	m_UintSensor3 = pParent->SonorPosition[RightSonor3].SonerPosition;
	m_UintSensor4 = pParent->SonorPosition[RightSonor4].SonerPosition;
	m_UintSensor21 = pParent->SonorPosition[LeftSonor1].SonerPosition;
	m_UintSensor22 = pParent->SonorPosition[LeftSonor2].SonerPosition;
	m_UintSensor23 = pParent->SonorPosition[LeftSonor3].SonerPosition;
	m_UintSensor24 = pParent->SonorPosition[LeftSonor4].SonerPosition;

	IR[0]=pParent->IR[0];
	IR[1]=pParent->IR[1];
	IR[2]=pParent->IR[2];
	IR[3]=pParent->IR[3];
	IR[4]=pParent->IR[4];
	IR[5]=pParent->IR[5];
	IR[6]=pParent->IR[6];
	IR[7]=pParent->IR[7];
	IR[8]=pParent->IR[8];
	IR[9]=pParent->IR[9];
	IR[10]=pParent->IR[10];
	UpdateData(FALSE);//加入语句 

	return ; 
}

BOOL CRunStateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetTimer(1,300,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunStateDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CRobotDlg *pParent=(CRobotDlg *)GetParent(); //加入语句
	m_velocity1 = pParent->m_longVelocity[0];//500*33.3
	m_velocity2 = pParent->m_longVelocity[1];
	m_velocity3 = pParent->m_longVelocity[2];
	m_velocity4 = pParent->m_longVelocity[3];
	m_velocity5 = pParent->m_longVelocity[4];
	m_velocity6 = pParent->m_longVelocity[5];
	m_velocity7 = pParent->m_longVelocity[6];
	m_velocity8 = pParent->m_longVelocity[7];

	m_AD1 = pParent->m_AD1;
	m_AD2 = pParent->m_AD2;
	m_AD3 = pParent->m_AD3;
	m_AD4 = pParent->m_AD4;
	m_IO1 = pParent->m_IO1;
	m_IO2 = pParent->m_IO2;
	m_IO3 = pParent->m_IO3;
	m_IO4 = pParent->m_IO4;
	m_IO5 = pParent->m_IO5;
	m_IO6 = pParent->m_IO6;
	m_IO7 = pParent->m_IO7;
	m_IO8 = pParent->m_IO8;
	m_RevTurnPos1 = pParent->m_RevTurnPos1;
	m_RevTurnPos3 = pParent->m_RevTurnPos3;
	m_RevTurnPos5 = pParent->m_RevTurnPos5;
	m_RevTurnPos7 = pParent->m_RevTurnPos7;
	m_RevTurnPos2 = pParent->m_RevTurnPos2;
	m_RevTurnPos4 = pParent->m_RevTurnPos4;
	m_RevTurnPos6 = pParent->m_RevTurnPos6;
	m_RevTurnPos8 = pParent->m_RevTurnPos8;

	m_UintSensor1 = pParent->SonorPosition[RightSonor1].SonerPosition;
	m_UintSensor2 = pParent->SonorPosition[RightSonor2].SonerPosition;
	m_UintSensor3 = pParent->SonorPosition[RightSonor3].SonerPosition;
	m_UintSensor4 = pParent->SonorPosition[RightSonor4].SonerPosition;
	m_UintSensor21 = pParent->SonorPosition[LeftSonor1].SonerPosition;
	m_UintSensor22 = pParent->SonorPosition[LeftSonor2].SonerPosition;
	m_UintSensor23 = pParent->SonorPosition[LeftSonor3].SonerPosition;
	m_UintSensor24 = pParent->SonorPosition[LeftSonor4].SonerPosition;

	IR[0]=pParent->IR[0];
	IR[1]=pParent->IR[1];
	IR[2]=pParent->IR[2];
	IR[3]=pParent->IR[3];
	IR[4]=pParent->IR[4];
	IR[5]=pParent->IR[5];
	IR[6]=pParent->IR[6];
	IR[7]=pParent->IR[7];
	IR[8]=pParent->IR[8];
	IR[9]=pParent->IR[9];
	IR[10]=pParent->IR[10];

//	   m_ReturnShow[i] = pParent -> m_ReturnSensorAD[i];


	UpdateData(FALSE);//加入语句 

	CDialog::OnTimer(nIDEvent);
}
