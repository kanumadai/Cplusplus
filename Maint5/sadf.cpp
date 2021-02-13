// sadf.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "sadf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Csadf dialog


Csadf::Csadf(CWnd* pParent /*=NULL*/)
	: CDialog(Csadf::IDD, pParent)
{
	//{{AFX_DATA_INIT(Csadf)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Csadf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Csadf)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Csadf, CDialog)
	//{{AFX_MSG_MAP(Csadf)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Csadf message handlers
