// FastOpen32File.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "FastOpen32File.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastOpen32File dialog


CFastOpen32File::CFastOpen32File(CWnd* pParent /*=NULL*/)
	: CDialog(CFastOpen32File::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastOpen32File)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFastOpen32File::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastOpen32File)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastOpen32File, CDialog)
	//{{AFX_MSG_MAP(CFastOpen32File)
	ON_BN_CLICKED(IDC_SEL_FILENAME, OnSelFilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastOpen32File message handlers

void CFastOpen32File::OnSelFilename() 
{
	// TODO: Add your control notification handler code here
	
}
