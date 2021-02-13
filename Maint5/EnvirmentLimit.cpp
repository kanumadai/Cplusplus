// EnvirmentLimit.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "EnvirmentLimit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentLimit dialog


CEnvirmentLimit::CEnvirmentLimit(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvirmentLimit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvirmentLimit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEnvirmentLimit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvirmentLimit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvirmentLimit, CDialog)
	//{{AFX_MSG_MAP(CEnvirmentLimit)
	ON_BN_CLICKED(IDC_OPERATE, OnOperate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentLimit message handlers

BOOL CEnvirmentLimit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  
}


void CEnvirmentLimit::OnOperate() 
{
	
}
