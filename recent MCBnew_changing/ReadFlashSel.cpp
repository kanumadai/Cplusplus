// ReadFlashSel.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "ReadFlashSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadFlashSel dialog


CReadFlashSel::CReadFlashSel(CWnd* pParent /*=NULL*/)
	: CDialog(CReadFlashSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReadFlashSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReadFlashSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadFlashSel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReadFlashSel, CDialog)
	//{{AFX_MSG_MAP(CReadFlashSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadFlashSel message handlers

BOOL CReadFlashSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox * pComSel = (CComboBox *)GetDlgItem(IDC_FLASH_SEL);
	if (iFlashSel > 3) {
		iFlashSel = 0;
	}
	pComSel->SetCurSel(iFlashSel);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReadFlashSel::OnOK() 
{
	CComboBox * pComSel = (CComboBox *)GetDlgItem(IDC_FLASH_SEL);
	iFlashSel = pComSel->GetCurSel();
	
	CDialog::OnOK();
}
