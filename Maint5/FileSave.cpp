// FileSave1.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "FileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSave dialog


CFileSave::CFileSave(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSave)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSave, CDialog)
	//{{AFX_MSG_MAP(CFileSave)
	ON_BN_CLICKED(IDC_NEW_FILEPACK, OnNewFilepack)
	ON_BN_CLICKED(IDC_EDIT_FILEPACK, OnEditFilepack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSave message handlers

void CFileSave::OnNewFilepack() 
{
	bNewfilepackage = true;
	
}

void CFileSave::OnEditFilepack() 
{
	bNewfilepackage = false;
	
}

BOOL CFileSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CButton *pnewfile = (CButton *)GetDlgItem(IDC_NEW_FILEPACK);
	CButton *peditfile = (CButton *)GetDlgItem(IDC_EDIT_FILEPACK);

	if(bNewfilepackage) {
		pnewfile->SetCheck(1);
	}
	else {	peditfile->SetCheck(1); }

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
