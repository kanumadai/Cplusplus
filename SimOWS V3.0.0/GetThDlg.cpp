// GetThDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "GetThDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetThDlg dialog


CGetThDlg::CGetThDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetThDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetThDlg)
	m_lth = 1;
	m_uth = 255;
	//}}AFX_DATA_INIT
}


void CGetThDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetThDlg)
	DDX_Text(pDX, IDC_EDIT_LTH, m_lth);
	DDV_MinMaxInt(pDX, m_lth, 1, 255);
	DDX_Text(pDX, IDC_EDIT_UTH, m_uth);
	DDV_MinMaxInt(pDX, m_uth, 1, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetThDlg, CDialog)
	//{{AFX_MSG_MAP(CGetThDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetThDlg message handlers
