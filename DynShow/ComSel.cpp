// ComSel.cpp : implementation file
//

#include "stdafx.h"
#include "DynShow.h"
#include "ComSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSel dialog


CComSel::CComSel(CWnd* pParent /*=NULL*/)
	: CDialog(CComSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSel)
	m_com = 1;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSel)
	DDX_CBIndex(pDX, IDC_COMSEL, m_com);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSel, CDialog)
	//{{AFX_MSG_MAP(CComSel)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSel message handlers
