// TestParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "TestParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestParamDlg dialog


CTestParamDlg::CTestParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestParamDlg)
	m_bucket = 0;
	m_DatFmt = 1;
	//}}AFX_DATA_INIT
}


void CTestParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestParamDlg)
	DDX_CBIndex(pDX, IDC_BUCKET, m_bucket);
	DDX_Radio(pDX, IDC_DATFMT, m_DatFmt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestParamDlg, CDialog)
	//{{AFX_MSG_MAP(CTestParamDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestParamDlg message handlers
