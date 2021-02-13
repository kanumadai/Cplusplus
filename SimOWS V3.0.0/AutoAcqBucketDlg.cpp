// AutoAcqBucketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "AutoAcqBucketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqBucketDlg dialog


CAutoAcqBucketDlg::CAutoAcqBucketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoAcqBucketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoAcqBucketDlg)
	m_nBucketBegin = 0;
	m_nBucketEnd = 31;
	m_nDatFmt = 0;
	m_bRodOut = 0;
	m_strPath = _T("c:");
	m_bSeptaOut = 0;
	m_nStopEvent = 1000000;
	m_nStopTime = 60;
	m_nStopType = 1;
	//}}AFX_DATA_INIT
}


void CAutoAcqBucketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoAcqBucketDlg)
	DDX_Text(pDX, IDC_BUCKET_BEGIN, m_nBucketBegin);
	DDV_MinMaxInt(pDX, m_nBucketBegin, 0, 31);
	DDX_Text(pDX, IDC_BUCKET_END, m_nBucketEnd);
	DDV_MinMaxInt(pDX, m_nBucketEnd, 0, 31);
	DDX_Radio(pDX, IDC_DAT_SD, m_nDatFmt);
	DDX_Radio(pDX, IDC_ROD_IN, m_bRodOut);
	DDX_Text(pDX, IDC_SAVEPATH, m_strPath);
	DDV_MaxChars(pDX, m_strPath, 512);
	DDX_Radio(pDX, IDC_SEPTA_IN, m_bSeptaOut);
	DDX_Text(pDX, IDC_STOP_EVENTCNT, m_nStopEvent);
	DDV_MinMaxInt(pDX, m_nStopEvent, 1000, 1000000000);
	DDX_Text(pDX, IDC_STOP_TIME, m_nStopTime);
	DDV_MinMaxInt(pDX, m_nStopTime, 1, 10000000);
	DDX_Radio(pDX, IDC_STOPONCNT, m_nStopType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoAcqBucketDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoAcqBucketDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqBucketDlg message handlers
