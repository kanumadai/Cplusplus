// WidthStat.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "WidthStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWidthStat dialog

CWidthStat::CWidthStat(CView *pView)
{
	m_pView = pView;  //akang add
}

CWidthStat::CWidthStat(CWnd* pParent /*=NULL*/)
	: CDialog(CWidthStat::IDD, pParent)
{
	m_pView = NULL;   //akang add

	//{{AFX_DATA_INIT(CWidthStat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWidthStat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWidthStat)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWidthStat, CDialog)
	//{{AFX_MSG_MAP(CWidthStat)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWidthStat message handlers

BOOL CWidthStat::Create()  //akang add
{

	
	return CDialog::Create(CWidthStat::IDD);
}


BOOL CWidthStat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_WIDTH_VALUE)->SetWindowText("2");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWidthStat::OnOK() 
{
	CString strvalue;
	int ivalue;
	UpdateData(TRUE);
	GetDlgItem(IDC_WIDTH_VALUE)->GetWindowText(strvalue);
	if (strvalue.IsEmpty()) {
		return;
	} 
	else {
		ivalue = atoi(strvalue);
	}

	m_pView->PostMessage(WM_WIDTHSTAT_IDOK,(int)ivalue);			
	
}

void CWidthStat::OnCancel() 
{
	if (m_pView != NULL) {
		m_pView->PostMessage(WM_WIDTHSTAT_GOODBYE,IDCANCEL);
	}
	else CDialog::OnCancel();		
}

