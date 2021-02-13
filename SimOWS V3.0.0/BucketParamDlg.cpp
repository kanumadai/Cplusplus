// BucketParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "BucketParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBucketParamDlg dialog


CBucketParamDlg::CBucketParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBucketParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBucketParamDlg)
	m_nBucket = 0;
	m_bRodOut = 0;
	m_strFile = _T("");
	m_bSeptaOut = 0;
	m_nStopType = 0;
	m_nStopEvent = 1000000;
	m_nStopTime = 60;
	m_nDatFmt = 0;
	//}}AFX_DATA_INIT
}


void CBucketParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBucketParamDlg)
	DDX_Text(pDX, IDC_BUCKET, m_nBucket);
	DDV_MinMaxInt(pDX, m_nBucket, 0, 31);
	DDX_Radio(pDX, IDC_ROD_IN, m_bRodOut);
	DDX_Text(pDX, IDC_SAVEPATH, m_strFile);
	DDV_MaxChars(pDX, m_strFile, 512);
	DDX_Radio(pDX, IDC_SEPTA_IN, m_bSeptaOut);
	DDX_Radio(pDX, IDC_STOPONCNT, m_nStopType);
	DDX_Text(pDX, IDC_STOP_EVENTCNT, m_nStopEvent);
	DDV_MinMaxInt(pDX, m_nStopEvent, 1000, 2147483647);
	DDX_Text(pDX, IDC_STOP_TIME, m_nStopTime);
	DDV_MinMaxInt(pDX, m_nStopTime, 1, 1000000000);
	DDX_Radio(pDX, IDC_DAT_SD, m_nDatFmt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBucketParamDlg, CDialog)
	//{{AFX_MSG_MAP(CBucketParamDlg)
	ON_BN_CLICKED(IDC_BROWN, OnBrown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBucketParamDlg message handlers

void CBucketParamDlg::OnBrown() 
{
	// TODO: Add your control notification handler code here
	char szFile[512];
	sprintf(szFile, "B%2.2d.bkt", GetDlgItemInt(IDC_BUCKET) );

	CFileDialog fileDlg( FALSE,"bkt",
						 szFile,
						 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		                 "PET BUCKETÎÄ¼þ(*.bkt)|*.bkt|All Files (*.*)|*.*||"
					   );
	if(fileDlg.DoModal() != IDOK ) return;
	
	SetDlgItemText(IDC_SAVEPATH, fileDlg.GetPathName() );
}
