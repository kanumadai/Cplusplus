// WorkModeSel.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "WorkModeSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkModeSel dialog


CWorkModeSel::CWorkModeSel(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkModeSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorkModeSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWorkModeSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkModeSel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkModeSel, CDialog)
	//{{AFX_MSG_MAP(CWorkModeSel)
	ON_BN_CLICKED(IDC_RADIO_X, OnRadioX)
	ON_BN_CLICKED(IDC_RADIO_Y, OnRadioY)
	ON_BN_CLICKED(IDC_RADIO_E, OnRadioE)
	ON_BN_CLICKED(IDC_RADIO_T, OnRadioT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkModeSel message handlers

BOOL CWorkModeSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	CButton *pmode_x = (CButton *)GetDlgItem(IDC_RADIO_X);
	CButton *pmode_y = (CButton *)GetDlgItem(IDC_RADIO_Y);
	CButton *pmode_e = (CButton *)GetDlgItem(IDC_RADIO_E);
	CButton *pmode_t = (CButton *)GetDlgItem(IDC_RADIO_T);
	pmode_x->SetCheck(0);
	pmode_y->SetCheck(0);
	pmode_e->SetCheck(0);
	pmode_t->SetCheck(0);


	if (m_imodeSel == 0) {
		pmode_x->SetCheck(1);
	}
	if (m_imodeSel == 1) {
		pmode_y->SetCheck(1);
	}
	if (m_imodeSel == 2) {
		pmode_e->SetCheck(1);
	}
	if (m_imodeSel == 3) {
		pmode_t->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkModeSel::OnRadioX() 
{
	m_imodeSel = 0;	
}

void CWorkModeSel::OnRadioY() 
{
	m_imodeSel = 1;	
}

void CWorkModeSel::OnRadioE() 
{
	m_imodeSel = 2;	
}

void CWorkModeSel::OnRadioT() 
{
	m_imodeSel = 3;	
}
