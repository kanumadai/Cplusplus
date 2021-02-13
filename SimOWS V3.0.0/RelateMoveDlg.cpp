// RelateMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "RelateMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRelateMoveDlg dialog


CRelateMoveDlg::CRelateMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRelateMoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRelateMoveDlg)
	m_bOut = 1;
	m_len = 100;
	//}}AFX_DATA_INIT
}


void CRelateMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRelateMoveDlg)
	DDX_Radio(pDX, IDC_IN, m_bOut);
	DDX_Text(pDX, IDC_LEN, m_len);
	DDV_MinMaxInt(pDX, m_len, 1, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRelateMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CRelateMoveDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelateMoveDlg message handlers
