// BedCoordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "BedCoordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBedCoordDlg dialog


CBedCoordDlg::CBedCoordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBedCoordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBedCoordDlg)
	m_coord = 0;
	//}}AFX_DATA_INIT
}


void CBedCoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBedCoordDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_coord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBedCoordDlg, CDialog)
	//{{AFX_MSG_MAP(CBedCoordDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBedCoordDlg message handlers
